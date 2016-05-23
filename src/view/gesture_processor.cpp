/* Copyright (c) 2010-2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "gesture_processor.h"
#include "commands.h"
#include <glib.h>
#include <math.h>  /* for sqrt */
#include "gesture_detector_statemachine.h"
#include "maps_view_plugin.h"



/*----------------------------------------------------------------------------*/


/* Using protected functions of Map View */
extern bool _maps_view_is_gesture_available(maps_view_h view, maps_view_gesture_e gesture);
extern maps_view_action_e _maps_view_get_gesture_action(maps_view_h view, maps_view_gesture_e gesture);
extern void *_maps_view_get_maps_service_ptr(maps_view_h view);
extern maps_view_object_h _maps_view_object_hit_test(maps_view_h view, int x, int y, maps_view_gesture_e gesture);
extern int _maps_view_event_data_set_gesture_type(maps_view_event_data_h event, maps_view_gesture_e gesture_type);
extern int _maps_view_event_data_set_position(maps_view_event_data_h event, int x, int y);
extern int _maps_view_event_data_set_center(maps_view_event_data_h event, maps_coordinates_h center);
extern int _maps_view_event_data_set_fingers(maps_view_event_data_h event, int fingers);
extern int _maps_view_event_data_set_zoom_factor(maps_view_event_data_h event, double zoom_factor);
extern int _maps_view_event_data_set_rotation_angle(maps_view_event_data_h event, double rotation_angle);
extern maps_view_event_data_h _maps_view_create_event_data(maps_view_event_type_e type);
extern void _maps_view_invoke_event_callback(maps_view_h view, maps_view_event_data_h event_data);


#ifdef _MOVE_CENTER_COMMAND_DEFINED_
extern int _maps_view_set_center_directly(maps_view_h view, maps_coordinates_h coordinates);
extern int _maps_view_get_plugin_center(const maps_view_h view, maps_coordinates_h *center);
#endif /* _MOVE_CENTER_COMMAND_DEFINED_ */

/* ---------------------------------------------------------------------------*/


view::zoom_calculator::zoom_calculator(const touch_point &start_tp_f1,
					const touch_point &cur_tp_f1,
					const touch_point &start_tp_f2,
					const touch_point &cur_tp_f2)
	: _start_tp_f1(start_tp_f1)
	, _cur_tp_f1(cur_tp_f1)
	, _start_tp_f2(start_tp_f2)
	, _cur_tp_f2(cur_tp_f2)
	, _new_zoom_factor(.0)
	, _new_rotation_angle(.0)
	, _zoom_happend(false)
	, _rotation_happend(false)
{
	/* Finding the start radius */
	const int start_dx = _start_tp_f2._x - _start_tp_f1._x;
	const int start_dy = _start_tp_f2._y - _start_tp_f1._y;
	const double start_r =
		sqrt(start_dx * start_dx + start_dy * start_dy) / 2;

	/* Finding the cur radius */
	const int cur_dx = _cur_tp_f2._x - _cur_tp_f1._x;
	const int cur_dy = _cur_tp_f2._y - _cur_tp_f1._y;
	const double cur_r = sqrt(cur_dx * cur_dx + cur_dy * cur_dy) / 2;

	/* Calculating the zoom factor */
	if((start_r != .0) && (cur_r != .0) && (start_r != cur_r)) {
		if (cur_r > start_r)
			_new_zoom_factor = (cur_r / start_r) - 1;
		else
			_new_zoom_factor = -((start_r / cur_r) - 1);
		_zoom_happend = true;
	}

	/* Calculating the rotation angle */
	const double inner1 = sqrt(start_dx * start_dx + start_dy * start_dy);
	const double inner2 = sqrt(cur_dx * cur_dx + cur_dy * cur_dy);
	const double norm = inner1 * inner2;
	const double inner = (cur_dx * start_dx) + (cur_dy * start_dy);
	const double angle = acos(inner / norm);
	const double curl = (cur_dx * start_dy) - (cur_dy * start_dx);

	if (curl < 0) {
		_new_rotation_angle = (angle / M_PI * 180.);
		_rotation_happend = true;
	}
	else if (curl > 0) {
		_new_rotation_angle = 360 - (angle / M_PI * 180.);
		_rotation_happend = true;
	}
}


/* ---------------------------------------------------------------------------*/


view::gesture_processor::gesture_processor(gesture_detector *gd)
	: _gd(gd)
{
}

view::gesture_processor::~gesture_processor()
{
}

extern session::command_queue *__maps_view_select_q();

session::command_queue *view::gesture_processor::q()
{
	/* This is a thread-friendly mode, when all heavy computations
	 *  are performed in the "idle" mode.
	 *  This mode is good for apps, which has other active widgets in
	 *  addition to Map View.
	 */
	/* return session::command_queue_view::interface(); */


	/* This is a draw-enforcing mode, when the rendering and drawing
	 *  of the map is happening as fast as it possible.
	 *  This mode is good when the Map View is a single active widget
	 *  of the App */
	/*return session::command_queue_sync::interface();*/

	/* Will use the same queue as used in Map View */
	return __maps_view_select_q();
}

void *view::gesture_processor::get_maps()
{
	return _maps_view_get_maps_service_ptr(_gd->_view);
}

session::command *view::gesture_processor::construct_gesture_command(
						maps_view_gesture_e gesture,
						const maps_coordinates_h c,
						const bool zoom_changed,
						const double zoom,
						const bool rotation_changed,
						const double angle)
{
	double zoom_factor = zoom;
	double rotation_angle = angle;

	/* Check if the gesture is available */
	if (!_maps_view_is_gesture_available(_gd->_view, gesture)) {
		return session::command::empty_ptr();
	}

	/* Perform gesture action */
	switch(_maps_view_get_gesture_action(_gd->_view, gesture)) {
	case MAPS_VIEW_ACTION_SCROLL: {
		maps_coordinates_h coords = c;
		maps_coordinates_h center_clone = NULL;
		if(!coords) {
			maps_view_get_center(_gd->_view, &center_clone);
			coords = center_clone;
		}
		session::command *cmd =
			new session::command_view_set_center(get_maps(), _gd->_view, coords);
		maps_coordinates_destroy(center_clone);
		return cmd;
	}
	case MAPS_VIEW_ACTION_ZOOM_IN: {
		MAPS_LOGD("MAPS_VIEW_ACTION_ZOOM_IN");
		if (zoom_factor == .0 || !zoom_changed)
			maps_view_get_zoom_factor(_gd->_view, &zoom_factor);
		return new session::command_view_zoom(get_maps(), _gd->_view, zoom_factor + 1.);
	}
	case MAPS_VIEW_ACTION_ZOOM_OUT: {
		MAPS_LOGD("MAPS_VIEW_ACTION_ZOOM_OUT");
		if (zoom_factor == .0 || !zoom_changed)
			maps_view_get_zoom_factor(_gd->_view, &zoom_factor);
		return new session::command_view_zoom(get_maps(), _gd->_view, zoom_factor - 1.);
	}
	case MAPS_VIEW_ACTION_ZOOM_AND_SCROLL: {
		MAPS_LOGD("MAPS_VIEW_ACTION_ZOOM_AND_SCROLL");
		double cur_zoom_factor;
		maps_view_get_zoom_factor(_gd->_view, &cur_zoom_factor);

		if (zoom_factor == .0 || !zoom_changed)
			maps_view_get_zoom_factor(_gd->_view, &zoom_factor);

		if (zoom_factor == cur_zoom_factor) zoom_factor++;
		maps_view_set_zoom_factor(_gd->_view, zoom_factor);

		maps_coordinates_h coords = c;
		maps_coordinates_h center_clone = NULL;
		if(!coords) {
			maps_view_get_center(_gd->_view, &center_clone);
			coords = center_clone;
		}
		session::command *cmd =
			new session::command_view_set_center(get_maps(), _gd->_view, coords);
		maps_coordinates_destroy(center_clone);
		return cmd;
	}
	case MAPS_VIEW_ACTION_ZOOM:
	case MAPS_VIEW_ACTION_ROTATE: {
		if(zoom_changed & rotation_changed) {
			MAPS_LOGD("rotation_angle=%f", rotation_angle);
			if(zoom_factor == .0)
				maps_view_get_zoom_factor(_gd->_view, &zoom_factor);
			if(rotation_angle == .0)
				maps_view_get_orientation(_gd->_view, &rotation_angle);
			rotation_angle -= (int(rotation_angle) / 360) * 360;
			return new session::command_view_zoom_rotate(get_maps(), _gd->_view, zoom_factor, rotation_angle);
		} else if(zoom_changed) {
			if(zoom_factor == .0)
				maps_view_get_zoom_factor(_gd->_view, &zoom_factor);
			MAPS_LOGI("\t set new zoom command: %f\n", zoom_factor);
			return new session::command_view_zoom(get_maps(), _gd->_view, zoom_factor);
		} else if(rotation_changed) {
			if(rotation_angle == .0)
				maps_view_get_orientation(_gd->_view, &rotation_angle);
			rotation_angle -= (int(rotation_angle) / 360) * 360;
			return new session::command_view_rotate(get_maps(), _gd->_view, rotation_angle);
		}
	}
	case MAPS_VIEW_ACTION_NONE:
		MAPS_LOGI("GESTURE: This Gesture is assigned with no Action");
	default:
		return session::command::empty_ptr();
	}
}

void view::gesture_processor::on_long_press()
{
	/* Assumed that we can tap only with a single finger */
	touch_point tp = _gd->_info._finger_move[0];

	/* Check if any object was affected */
	maps_view_object_h hit = _maps_view_object_hit_test(_gd->_view,
						tp._x,
						tp._y,
						MAPS_VIEW_GESTURE_LONG_PRESS);
	if (hit) return;

	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	maps_view_screen_to_geolocation(_gd->_view, tp._x, tp._y, &c);
	q()->push(construct_gesture_command(MAPS_VIEW_GESTURE_LONG_PRESS, c,
					    false, .0, false, .0));

	/* Invoke user registered event callback */
	maps_view_event_data_h ed = _maps_view_create_event_data(MAPS_VIEW_EVENT_GESTURE);
	if(ed) {
		_maps_view_event_data_set_gesture_type(ed, MAPS_VIEW_GESTURE_LONG_PRESS);
		_maps_view_event_data_set_position(ed, tp._x, tp._y);
		_maps_view_event_data_set_center(ed, c);
		_maps_view_event_data_set_fingers(ed, 1);
		_maps_view_invoke_event_callback(_gd->_view, ed);
		maps_view_event_data_destroy(ed);
	}

	maps_coordinates_destroy(c);
}

void view::gesture_processor::on_double_tap()
{
	/* Assumed that we can tap only with a single finger */
	touch_point tp = _gd->_info_history._finger_up[0];

	/* Check if any object was affected */
	maps_view_object_h hit = _maps_view_object_hit_test(_gd->_view,
						tp._x,
						tp._y,
						MAPS_VIEW_GESTURE_DOUBLE_TAP);
	if (hit) return;

	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	maps_view_screen_to_geolocation(_gd->_view, tp._x, tp._y, &c);
	q()->push(construct_gesture_command(MAPS_VIEW_GESTURE_DOUBLE_TAP, c,
					    false, .0, false, .0));

	/* Invoke user registered event callback */
	maps_view_event_data_h ed = _maps_view_create_event_data(MAPS_VIEW_EVENT_GESTURE);
	if(ed) {
		_maps_view_event_data_set_gesture_type(ed, MAPS_VIEW_GESTURE_DOUBLE_TAP);
		_maps_view_event_data_set_position(ed, tp._x, tp._y);
		_maps_view_event_data_set_fingers(ed, 1);
		_maps_view_invoke_event_callback(_gd->_view, ed);
		maps_view_event_data_destroy(ed);
	}
	maps_coordinates_destroy(c);
}

void view::gesture_processor::on_tap()
{
	/* Assumed that we can tap only with a single finger */
	touch_point tp = _gd->_info_history._finger_up[0];

	/* Check if any object was affected */
	maps_view_object_h hit = _maps_view_object_hit_test(_gd->_view,
						tp._x,
						tp._y,
						MAPS_VIEW_GESTURE_TAP);
	if (hit) return;

	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	maps_view_screen_to_geolocation(_gd->_view, tp._x, tp._y, &c);
	q()->push(construct_gesture_command(MAPS_VIEW_GESTURE_TAP, c,
					    false, .0, false, .0));

	/* Invoke user registered event callback */
	maps_view_event_data_h ed = _maps_view_create_event_data(MAPS_VIEW_EVENT_GESTURE);
	if(ed) {
		_maps_view_event_data_set_gesture_type(ed, MAPS_VIEW_GESTURE_TAP);
		_maps_view_event_data_set_position(ed, tp._x, tp._y);
		_maps_view_event_data_set_fingers(ed, 1);
		_maps_view_invoke_event_callback(_gd->_view, ed);
		maps_view_event_data_destroy(ed);
	}
	maps_coordinates_destroy(c);
}

void view::gesture_processor::on_two_finger_tap()
{
	MAPS_LOGW("\nON TWO FINGER TAP\n");

	const touch_point tp_f1 = _gd->_info._finger_move[0];
	const touch_point tp_f2 = _gd->_info._finger_move[1];
	const touch_point gesture_center = calc_center(tp_f1, tp_f2);

	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	maps_view_screen_to_geolocation(_gd->_view, gesture_center._x,
						gesture_center._y, &c);

	/* Enqueue the detected command */
	q()->push(construct_gesture_command(MAPS_VIEW_GESTURE_2_FINGER_TAP, c,
					    false, .0, false, .0));

	/* Invoke user registered event callback */
	maps_view_event_data_h ed = _maps_view_create_event_data(MAPS_VIEW_EVENT_GESTURE);
	if(ed) {
		_maps_view_event_data_set_gesture_type(ed, MAPS_VIEW_GESTURE_2_FINGER_TAP);
		_maps_view_event_data_set_position(ed, gesture_center._x, gesture_center._y);
		_maps_view_event_data_set_fingers(ed, 2);
		_maps_view_invoke_event_callback(_gd->_view, ed);
		maps_view_event_data_destroy(ed);
	}
	maps_coordinates_destroy(c);
}

void view::gesture_processor::on_panning_finished(int finger_no)
{
	/* Obtain fresh central coordinates of the map in the Plugin */
	maps_coordinates_h c = NULL;
	_maps_view_get_plugin_center(_gd->_view, &c);

	/* Directly set the updated center of the map */
	_maps_view_set_center_directly(_gd->_view, c);

	maps_coordinates_destroy(c);
}

void view::gesture_processor::on_pan(int finger_no)
{
	/* Assumed that we can tap only with a single finger */
	const touch_point cur_tp = _gd->_info._finger_move[finger_no];


	/* Calculate the new center of the map by adding the delta
	* to the original center (e.g. the center of the map before the event
	* has started) */

	/* Check if the gesture is available */
	if (!_maps_view_is_gesture_available(_gd->_view, MAPS_VIEW_GESTURE_SCROLL))
		return;

	touch_point prev_tp = _gd->_info._prev_finger_down[finger_no];
	if(prev_tp.empty())
		prev_tp = _gd->_info._finger_down[finger_no];

	/* a. Calculating the delta of the gesture */
	int delta_x = cur_tp._x - prev_tp._x;
	int delta_y = cur_tp._y - prev_tp._y;

	/* b. Enque the command to move the center */
	q()->push(new session::command_view_move_center(get_maps(),
							_gd->_view,
							-delta_x,
							-delta_y));

	/* Invoke user registered event callback */
	maps_view_event_data_h ed = _maps_view_create_event_data(MAPS_VIEW_EVENT_GESTURE);
	if(ed) {
		_maps_view_event_data_set_gesture_type(ed, MAPS_VIEW_GESTURE_SCROLL);
		_maps_view_event_data_set_position(ed, cur_tp._x, cur_tp._y);
		_maps_view_event_data_set_fingers(ed, 1);
		_maps_view_invoke_event_callback(_gd->_view, ed);
		maps_view_event_data_destroy(ed);
	}
}

view::touch_point view::gesture_processor::calc_center(
						const touch_point &tp1,
						const touch_point &tp2) const
{
	const unsigned int timestamp = (tp2._timestamp > tp1._timestamp)
		? tp2._timestamp
		: tp1._timestamp;
	return touch_point(tp1._x + (tp2._x - tp1._x) / 2,
			   tp1._y + (tp2._y - tp1._y) / 2,
			   timestamp);
}

void view::gesture_processor::on_zoom_rotate()
{
	gesture_detector::log("view::gesture_processor::on_zoom_rotate",
			      gesture_detector::FG_LITE_GREEN);
	/* Assumed that we can zoom&rotate using only a pair of fingers */

	/* First finger effective way by now */
	const touch_point start_tp_f1 = _gd->_info._finger_down[0];
	const touch_point cur_tp_f1 = _gd->_info._finger_move[0];


	/* Second finger effective way by now */
	const touch_point start_tp_f2 = _gd->_info._finger_down[1];
	const touch_point cur_tp_f2 = _gd->_info._finger_move[1];

	/***********************/
	MAPS_LOGI("%c[%d;%d;%dm"
		  "Finger1: start(%d, %d), cur(%d, %d)\t"
		  "Finger2: start(%d, %d), cur(%d, %d)\t"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, gesture_detector::FG_YELLOW,
		  start_tp_f1._x, start_tp_f1._y, cur_tp_f1._x, cur_tp_f1._y,
		  start_tp_f2._x, start_tp_f2._y, cur_tp_f2._x, cur_tp_f2._y,
		  0x1B, 0, 0, 0);
	/***********************/


	/* Calculating the current zoom factor, accordingly to effecitve ways
	 *  of fingers */
	zoom_calculator zc(start_tp_f1, cur_tp_f1, start_tp_f2, cur_tp_f2);
	double new_zoom_factor = zc.get_zoom_factor();
	double new_rotation_angle = zc.get_rotation_angle();


	/* Analyse zoom factor changes */
	bool zoom_changed = false;
	if(zc.zoom_happend() && _maps_view_is_gesture_available(_gd->_view, MAPS_VIEW_GESTURE_ZOOM)) {
		/* Apply newly calculated zoom factor */
		new_zoom_factor += _gd->_info._start_view_state._zoom_factor;

		/* Correct the zoom factor accordingly to allowed limits */
		/* TODO: it also may be cashed in the _info._start_view_state */
		int min_zoom_level = 0;
		int max_zoom_level = 0;
		maps_view_get_min_zoom_level(_gd->_view, &min_zoom_level);
		maps_view_get_max_zoom_level(_gd->_view, &max_zoom_level);
		if(new_zoom_factor < min_zoom_level)
			new_zoom_factor = min_zoom_level;
		if(new_zoom_factor > max_zoom_level)
			new_zoom_factor = max_zoom_level;

		/* Check if the zoom changed relatively to initial state */
		zoom_changed = (_gd->_info._start_view_state._zoom_factor != new_zoom_factor);
	}

	/* Analyze rotation angle changes */
	bool rotation_changed = false;
	if(zc.rotation_happend() && _maps_view_is_gesture_available(_gd->_view, MAPS_VIEW_GESTURE_ROTATE)) {
		/* Apply newly calculated rotation angle */
		new_rotation_angle =
			_gd->_info._start_view_state._rotation_angle + new_rotation_angle;

		/* Correct the rotation angle to normalize it
		 *  inside the diapazone of 0..360 degree */
		new_rotation_angle -= 360. * (int(new_rotation_angle) / 360);
		new_rotation_angle += 360. * (int(new_rotation_angle) / 360);

		/* Check if the angle changed relatively to initial state */
		rotation_changed = (_gd->_info._start_view_state._rotation_angle != new_rotation_angle);
	}

	/* Invoke user registered event callback for ZOOM */
	do {
		if(!zoom_changed)
			break;
		maps_view_event_data_h ed = _maps_view_create_event_data(MAPS_VIEW_EVENT_GESTURE);
		if(!ed)
			break;
		_maps_view_event_data_set_gesture_type(ed, MAPS_VIEW_GESTURE_ZOOM);
		_maps_view_event_data_set_zoom_factor(ed, new_zoom_factor);
		_maps_view_event_data_set_fingers(ed, 2);

		/* Find the current center of the gesture */
		const touch_point cur_center = calc_center(cur_tp_f1, cur_tp_f2);
		_maps_view_event_data_set_position(ed, cur_center._x, cur_center._y);
		_maps_view_invoke_event_callback(_gd->_view, ed);
		maps_view_event_data_destroy(ed);
	} while(false);

	/* Invoke user registered event callback for ROTATION */
	do {
		if(rotation_changed)
			break;
		maps_view_event_data_h ed = _maps_view_create_event_data(MAPS_VIEW_EVENT_GESTURE);
		if(!ed)
			break;
		_maps_view_event_data_set_gesture_type(ed, MAPS_VIEW_GESTURE_ROTATE);
		_maps_view_event_data_set_rotation_angle(ed, new_rotation_angle);
		_maps_view_event_data_set_fingers(ed, 2);

		/* Find the current center of the gesture */
		const touch_point cur_center = calc_center(cur_tp_f1, cur_tp_f2);
		_maps_view_event_data_set_position(ed, cur_center._x, cur_center._y);
		_maps_view_invoke_event_callback(_gd->_view, ed);
		maps_view_event_data_destroy(ed);
	} while(false);


	if(!zoom_changed && !rotation_changed)
		return; // Seems nothing changed, we can return

	/* Ignore center move if zoom is not available */
	const bool movable_center =
		_maps_view_is_gesture_available(_gd->_view, MAPS_VIEW_GESTURE_ZOOM);
	if(!movable_center) {
		q()->push(construct_gesture_command(MAPS_VIEW_GESTURE_ZOOM,
			NULL, zoom_changed, new_zoom_factor, rotation_changed, new_rotation_angle));
		return;
	}


	/* Shift center accordingly to performed zoom and/or rotation:
	 *  - get the original gesture center coordinates
	 *  - get the current gesture center coordinates
	 *  - calculate the coordinates delta
	 *  - move map center to the opposite value of this delta
	 *  Now we have a map, perfectly responding to the
	 *  double finger zoom and/or rotate action */


	/* a. Find delta in screen coordinates */
	const touch_point start_center = calc_center(start_tp_f1, start_tp_f2);
	const touch_point cur_center = calc_center(cur_tp_f1, cur_tp_f2);
	const int delta_x = cur_center._x - start_center._x;
	const int delta_y = cur_center._y - start_center._y;

	/* b. Get the initial screen coordinates of the center */
	int center_x = 0;
	int center_y = 0;
	double lat, lon;
	maps_coordinates_get_latitude_longitude(_gd->_info._start_view_state._center, &lat, &lon);
	maps_view_geolocation_to_screen(_gd->_view, _gd->_info._start_view_state._center, &center_x, &center_y);

	/* c. Apply the delta to the intital center coordinates */
	center_x -= delta_x;
	center_y -= delta_y;

	/* d. Converting screent coordinates of new center to
	* the geographical */
	maps_coordinates_h new_center = NULL;
	maps_view_screen_to_geolocation(_gd->_view, center_x, center_y, &new_center);

	/* e. Enque the command to move the center */
	q()->push(new session::command_view_set_center(get_maps(), _gd->_view, new_center));

	/* f. Enqueue the detected zomm command */
	q()->push(construct_gesture_command(MAPS_VIEW_GESTURE_ZOOM,
		new_center, zoom_changed, new_zoom_factor, rotation_changed, new_rotation_angle));

	maps_coordinates_destroy(new_center);
}


/* ---------------------------------------------------------------------------*/
/*    VIEW EVENT STREAM                                                       */
/* ---------------------------------------------------------------------------*/

view::finger_event_stream::finger_event_stream(maps_view_h v)
	: _d(NULL)
{
	/* TODO: extract in dedicated factory in maps_view.cpp */
	/* Issuing an instance of gestuer detector */
	_d = new gesture_detector_statemachine(v);
	/*_d = new inertial_gesture(v);*/
	/*_d = new gesture_detector(v);*/

	/* All fingers are un-pressed initially */
	for(int i = 0; i < MAX_FINGERS; i ++) {
		_finger_pressed[i] = false;
		_finger_moving[i] = false;
	}
}

void view::finger_event_stream::set_gesture_detector(gesture_detector *d)
{
	if(!d)
		return;
	if(d != _d) {
		_d->halt_gesture();
		delete _d;
		_d = d;
	}
}

view::finger_event_stream::~finger_event_stream()
{
	if(_d) {
		_d->halt_gesture();
		delete _d;
	}
	_d = NULL;
}

void view::finger_event_stream::tap(Evas_Event_Mouse_Down *ev)
{
	MAPS_LOGI("finger_event_stream::tap");
	if(!ev)
		return;

	/*
	 *  The Ecore sends "move" event before "press" event
	 *  so we have to skip this "late" event "press" for the sake of
	 *  detector simplicity
	 */
	if(_finger_pressed[0]) {
		MAPS_LOGI("finger_event_stream::tap [SKIPED]");
		return;
	}

	/* Current touch point info */
	const touch_point tp(ev->canvas.x, ev->canvas.y, ev->timestamp);

	_finger_pressed[0] = true;
	_finger_down[0] = tp;

	/* Process first finger press */
	_d->tap(0, tp);
}

void view::finger_event_stream::move(Evas_Event_Mouse_Move *ev)
{
	if(!ev)
		return;

	/* Current touch point info */
	const touch_point tp(ev->cur.canvas.x, ev->cur.canvas.y, ev->timestamp);

	MAPS_LOGI("finger_event_stream::move to (%d, %d)", tp._x, tp._y);

	/*
	 *  The Ecore sends "move" event before "press" event
	 *  so we have to emulate the "press" for the sake of
	 *  detector simplicity
	 */
	if(!_finger_pressed[0]) {
		MAPS_LOGI("finger_event_stream::tap [CORRECTION]");
		_finger_pressed[0] = true;
		_finger_down[0] = tp;

		/* Emulate finger press */
		_d->tap(0, tp);
	}

	/* It's needed to correct the touch point, when the finger moved out of
	 * assigned movement threshold */
	if(!_finger_moving[0] &&  !_finger_moving[1]
	    && finger_dragged_enough(0, tp)) {
		_d->_info._finger_down[0] = tp;
	}

	/*
	 *  We think that the movement happend when the finger moved out of
	 *  some small area.
	 */
	if(_finger_moving[0] || _finger_moving[1]
	   || finger_dragged_enough(0, tp)) {

		_finger_moving[0] = true;

		/* Process finger move */
		_d->move(0, tp);
	} else {
		MAPS_LOGI("finger_event_stream::move "
			  "Not Moved Enough [SKIPED]");
		return;
	}
}

void view::finger_event_stream::up(Evas_Event_Mouse_Up *ev)
{
	MAPS_LOGI("finger_event_stream::up");
	if(!ev)
		return;

	/* Process finger up */
	_d->up(0, view::touch_point(ev->canvas.x, ev->canvas.y, ev->timestamp));

	_finger_pressed[0] = false;
	_finger_moving[0] = false;
	_finger_down[0].reset();
}

void view::finger_event_stream::multi_tap(Evas_Event_Multi_Down *ev)
{
	MAPS_LOGI("finger_event_stream::multi_tap");
	if(!ev)
		return;

	const int finger_no = ev->device;
	if(finger_no >= MAX_FINGERS)
		return;

	/* Current touch point info */
	const touch_point tp(ev->canvas.x, ev->canvas.y, ev->timestamp);

	/*
	 *  The Ecore sends "move" event before "press" event
	 *  so we have to skip this "late" event "press" for the sake of
	 *  detector simplicity
	 */
	if(_finger_pressed[finger_no]) {
		MAPS_LOGI("finger_event_stream::multi_tap [SKIPED]");
		return;
	}

	_finger_pressed[finger_no] = true;
	_finger_down[finger_no] = tp;

	/* Process finger press */
	_d->tap(finger_no, tp);
}

void view::finger_event_stream::multi_move(Evas_Event_Multi_Move *ev)
{
	if(!ev)
		return;

	const int finger_no = ev->device;
	if(finger_no >= MAX_FINGERS)
		return;

	/* Current touch point info */
	const touch_point tp(ev->cur.canvas.x, ev->cur.canvas.y, ev->timestamp);

	MAPS_LOGI("finger_event_stream::multi_move to (%d, %d)", tp._x, tp._y);

	/*
	 *  The Ecore sends "move" event before "press" event
	 *  so we have to emulate the "press" for the sake of
	 *  detector simplicity
	 */
	if(!_finger_pressed[finger_no]) {
		MAPS_LOGI("finger_event_stream::multi_tap [CORRECTION]");
		_finger_pressed[finger_no] = true;
		_finger_down[finger_no] = tp;

		/* Emulate finger press */
		_d->tap(finger_no, tp);
	}

	/* It's needed to correct the touch point, when the finger moved out of
	 * assigned movement threshold */
	if(!_finger_moving[0] &&  !_finger_moving[1]
	    && finger_dragged_enough(finger_no, tp)) {
		_d->_info._finger_down[finger_no] = tp;
	}

	/*
	 *  We think that the movement happend when the finger moved out of
	 *  some small area.
	 */
	if(_finger_moving[0] || _finger_moving[1]
	   || finger_dragged_enough(finger_no, tp)) {
		_finger_moving[finger_no] = true;

		/* Process finger move */
		_d->move(finger_no, tp);
	} else {
		MAPS_LOGI("finger_event_stream::multi_move "
			  "Not Moved Enough [SKIPED]");
		return;
	}
}

void view::finger_event_stream::multi_up(Evas_Event_Multi_Up *ev)
{
	MAPS_LOGI("finger_event_stream::multi_up");
	if(!ev)
		return;

	const int finger_no = ev->device;
	if(finger_no >= MAX_FINGERS)
		return;

	/* Process finger up */
	_d->up(ev->device, view::touch_point(ev->canvas.x,
					    ev->canvas.y,
					    ev->timestamp));

	_finger_pressed[finger_no] = false;
	_finger_moving[finger_no] = false;
	_finger_down[finger_no].reset();
}


bool view::finger_event_stream::finger_dragged_enough(int finger_no,
						      const touch_point &tp)
{
	const touch_point start = _finger_down[finger_no];
	const int trajectory =
		gesture_detector::get_trajectory_effective_length(tp, start);
	return (trajectory >= gesture_detector::__CLICK_AREA);
}
