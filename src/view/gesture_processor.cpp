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

#include <glib.h>
#include <math.h>  /* for sqrt */
#include "gesture_processor.h"
#include "commands.h"
#include "gesture_detector_statemachine.h"
#include "map_view_plugin.h"



/*----------------------------------------------------------------------------*/


/* Using protected functions of Map View */
extern bool _map_view_is_gesture_available(map_view_h view,
					   map_gesture_e gesture);
extern map_action_e _map_view_get_gesture_action(map_view_h view,
					      map_gesture_e gesture);
extern void *_map_view_get_maps_service_instance(map_view_h view);

extern map_object_h _map_object_hit_test(map_view_h view,
						   const int x,
						   const int y,
						   map_gesture_e gesture);

extern int _map_event_data_set_gesture_type(map_event_data_h event,
					 const map_gesture_e gesture_type);

extern int _map_event_data_set_xy(map_event_data_h event,
				       const int x, const int y);

extern int _map_event_data_set_center(map_event_data_h event,
				      const maps_coordinates_h center);

extern int _map_event_data_set_fingers(map_event_data_h event,
					    const int fingers);

extern int _map_event_data_set_scale_factor(map_event_data_h event,
						const double zoom_factor);

extern int _map_event_data_set_rotation_angle(map_event_data_h event,
						   const double rotation_angle);

extern map_event_data_h _map_view_create_event_data(map_event_type_e
							 type);
extern void _map_view_invoke_event_callback(map_view_h view,
					    map_event_data_h event_data);


#ifdef _MOVE_CENTER_COMMAND_DEFINED_
extern int _map_view_set_center_directly(const map_view_h view,
				  const maps_coordinates_h coordinates);
extern int _map_view_get_plugin_center(const map_view_h view,
				       maps_coordinates_h *center);
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
		#if 1
		if (cur_r > start_r)
			_new_zoom_factor = (cur_r / start_r) - 1;
		else
			_new_zoom_factor = -((start_r / cur_r) - 1);
		#else
		_new_zoom_factor = cur_r / start_r;
		#endif
		_zoom_happend = true;
	}

	/* Calculating the rotation angle */
	const double start_angle = atan2(start_dy, start_dx);
	const double cur_angle = atan2(cur_dy, cur_dx);
	_new_rotation_angle = (cur_angle - start_angle) * 180. / M_PI;
	_rotation_happend = true;
}


/* ---------------------------------------------------------------------------*/


view::gesture_processor::gesture_processor(gesture_detector *gd)
	: _gd(gd)
{
}

view::gesture_processor::~gesture_processor()
{
}


/* TODO: | DEBUG ONLY | REMOVE WHEN NO NEEDED */
map_object_h view::gesture_processor::__DEBUG_add_marker(const int x,
							 const int y,
							 const map_marker_type_e type)

{
	maps_coordinates_h coords = NULL;
	map_view_screen_to_geolocation(_gd->_view, x,  y, &coords);
	map_object_h marker = NULL;
	map_object_create_marker(coords,
				 "/tmp/maps/icon_bluestar.png",
				 type,
				 &marker);
	map_view_add_object(_gd->_view, marker);
	maps_coordinates_destroy(coords);
	return marker;
}

map_object_h view::gesture_processor::__DEBUG_add_geo_marker(
					const maps_coordinates_h coords,
					const map_marker_type_e type)

{
	return NULL;
	map_object_h marker = NULL;
	map_object_create_marker(coords,
				 "/tmp/maps/icon_redstar.png",
				 type,
				 &marker);
	map_view_add_object(_gd->_view, marker);
	return marker;
}
/* ----- | DEBUG ONLY | REMOVE WHEN NO NEEDED */

extern session::command_queue *__map_view_select_q();

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
	return __map_view_select_q();
}

void *view::gesture_processor::get_maps()
{
	return _map_view_get_maps_service_instance(_gd->_view);
}

session::command *view::gesture_processor::construct_gesture_command(
						map_gesture_e gesture,
						const maps_coordinates_h c,
						const bool zoom_changed,
						const double zoom,
						const bool rotation_changed,
						const double angle)
{
	/* Check if the gesture is available */
	if (!_map_view_is_gesture_available(_gd->_view, gesture)) {
		return session::command::empty_ptr();
	}

	/* Perform gesture action */
	switch(_map_view_get_gesture_action(_gd->_view, gesture)) {
	case MAP_ACTION_SCROLL: {
		maps_coordinates_h coords = c;
		maps_coordinates_h center_clone = NULL;
		if(!coords) {
			map_view_get_center(_gd->_view, &center_clone);
			coords = center_clone;
		}
		session::command *cmd =
			new session::command_view_set_center(get_maps(),
							     _gd->_view,
							     coords);
		maps_coordinates_destroy(center_clone);
		return cmd;
	}
#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	case MAP_ACTION_ZOOM: {
		MAPS_LOGD("MAP_ACTION_ZOOM");
		double zoom_factor = zoom;
		if(zoom_factor == .0 || !zoom_changed)
			map_view_get_scale_factor(_gd->_view, &zoom_factor);
		return new session::command_view_zoom(get_maps(), _gd->_view,
							zoom_factor);
	}
	case MAP_ACTION_ZOOM_IN: {
		MAPS_LOGD("MAP_ACTION_ZOOM_IN");
		double zoom_factor = zoom;
		if (zoom_factor == .0 || !zoom_changed)
			map_view_get_scale_factor(_gd->_view, &zoom_factor);
		return new session::command_view_zoom(get_maps(), _gd->_view,
							zoom_factor + 1.);
	}
	case MAP_ACTION_ZOOM_OUT: {
		MAPS_LOGD("MAP_ACTION_ZOOM_OUT");
		double zoom_factor = zoom;
		if (zoom_factor == .0 || !zoom_changed)
			map_view_get_scale_factor(_gd->_view, &zoom_factor);
		return new session::command_view_zoom(get_maps(), _gd->_view,
							zoom_factor - 1.);
	}
	case MAP_ACTION_ZOOM_AND_SCROLL: {
		MAPS_LOGD("MAP_ACTION_ZOOM_AND_SCROLL");
		double cur_zoom_factor;
		map_view_get_scale_factor(_gd->_view, &cur_zoom_factor);

		double new_zoom_factor = zoom;
		if (new_zoom_factor == .0 || !zoom_changed)
			map_view_get_scale_factor(_gd->_view, &new_zoom_factor);

		if (new_zoom_factor == cur_zoom_factor) new_zoom_factor++;
		map_view_set_scale_factor(_gd->_view, new_zoom_factor);

		maps_coordinates_h coords = c;
		maps_coordinates_h center_clone = NULL;
		if(!coords) {
			map_view_get_center(_gd->_view, &center_clone);
			coords = center_clone;
		}
		session::command *cmd =
			new session::command_view_set_center(get_maps(),
							     _gd->_view,
							     coords);
		maps_coordinates_destroy(center_clone);
		return cmd;
	}
	case MAP_ACTION_ZOOM_AND_ROTATE: {
		MAPS_LOGD("MAP_ACTION_ZOOM_AND_ROTATE");
		double zoom_factor = zoom;
		if(zoom_factor == .0 || !zoom_changed)
			map_view_get_scale_factor(_gd->_view, &zoom_factor);
		double rotation_angle = angle;
		if(rotation_angle == .0 || !rotation_changed)
			map_view_get_orientation(_gd->_view, &rotation_angle);
		rotation_angle -= (int(rotation_angle) / 360) * 360;
		return new session::command_view_zoom_rotate(get_maps(),
							 _gd->_view,
							 zoom_factor,
							 rotation_angle);
	}
	case MAP_ACTION_ROTATE: {
		MAPS_LOGD("MAP_ACTION_ROTATE");
		double rotation_angle = angle;
		if(rotation_angle == .0 || !rotation_changed)
			map_view_get_orientation(_gd->_view, &rotation_angle);
		rotation_angle -= (int(rotation_angle) / 360) * 360;
		return new session::command_view_rotate(get_maps(), _gd->_view,
							rotation_angle);
	}
#else
	case MAP_ACTION_ZOOM:
	case MAP_ACTION_ROTATE: {
		if(zoom_changed & rotation_changed) {
			double zoom_factor = zoom;
			if(zoom_factor == .0)
				map_view_get_scale_factor(_gd->_view,
							 &zoom_factor);
			double rotation_angle = angle;
			if(rotation_angle == .0)
				map_view_get_orientation(_gd->_view,
							 &rotation_angle);
			rotation_angle -= (int(rotation_angle) / 360) * 360;
			return new session::command_view_zoom_rotate(get_maps(),
							 _gd->_view,
							 zoom_factor,
							 rotation_angle);
		} else if(zoom_changed) {
			double zoom_factor = zoom;
			if(zoom_factor == .0)
				map_view_get_scale_factor(_gd->_view,
							 &zoom_factor);
			MAPS_LOGI("\t set new zoom command: %f\n", zoom_factor);
			return new session::command_view_zoom(get_maps(),
							      _gd->_view,
							      zoom_factor);
		} else if(rotation_changed) {
			double rotation_angle = angle;
			if(rotation_angle == .0)
				map_view_get_orientation(_gd->_view,
							 &rotation_angle);
			rotation_angle -= (int(rotation_angle) / 360) * 360;
			return new session::command_view_rotate(get_maps(),
								_gd->_view,
								rotation_angle);
		}
	}
#endif
	case MAP_ACTION_NONE:
		MAPS_LOGI("GESTURE: This Gesture is assigned with no Action");
	default:
		return session::command::empty_ptr();
	}
}

void view::gesture_processor::on_long_press()
{
	/* Assumed that we can tap only with a single finger */
	touch_point tp = _gd->_info._finger_move[0];

#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
#if DEBUG
	/* Check if any object was affected */
	map_object_h hit = _map_object_hit_test(_gd->_view,
						tp._x,
						tp._y,
						MAP_CONTROL_LONG_PRESS);
	if (hit) return;
#endif
	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geolocation(_gd->_view, tp._x, tp._y, &c);
	q()->push(construct_gesture_command(MAP_CONTROL_LONG_PRESS, c,
					    false, .0, false, .0));
#else
	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geolocation(_gd->_view, tp._x, tp._y, &c);
	q()->push(construct_gesture_command(MAP_CONTROL_LONG_PRESS, c,
					    false, .0, false, .0));

	double lat = .0;
	double lon = .0;
	maps_coordinates_get_latitude_longitude(c, &lat, &lon);
	MAPS_LOGI("GESTURE PRESS coordinates: %f, %f", lat, lon);

	/* Check if any object was affected */
	_map_object_hit_test(_gd->_view,
				  tp._x,
				  tp._y,
				  MAP_GESTURE_LONG_PRESS);
#endif

	/* Invoke user registered event callback */
	map_event_data_h ed =
		_map_view_create_event_data(MAP_EVENT_GESTURE);
	if(ed) {
		_map_event_data_set_gesture_type(ed, MAP_CONTROL_LONG_PRESS);
		_map_event_data_set_xy(ed, tp._x, tp._y);
		_map_event_data_set_center(ed, c);
		_map_event_data_set_fingers(ed, 1);
		_map_view_invoke_event_callback(_gd->_view, ed);
	}

	maps_coordinates_destroy(c);
}

void view::gesture_processor::on_double_tap()
{
#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	/* Assumed that we can tap only with a single finger */
	touch_point tp = _gd->_info_history._finger_up[0];

#if DEBUG
	/* Check if any object was affected */
	map_object_h hit = _map_object_hit_test(_gd->_view,
						tp._x,
						tp._y,
						MAP_CONTROL_DOUBLE_TAP);
	if (hit) return;
#endif

	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geolocation(_gd->_view, tp._x, tp._y, &c);
	q()->push(construct_gesture_command(MAP_CONTROL_DOUBLE_TAP, c,
					    false, .0, false, .0));
#else
	/* Default double tap effect is zoom level up */
	double new_zoom_factor = 1. + int(_gd->_info._start_view_state._zoom_factor);

	/* TODO: it also may be cashed in the _info._start_view_state */
	int max_zoom_level = 0;
	map_view_get_max_zoom_level(_gd->_view, &max_zoom_level);

	if(new_zoom_factor > max_zoom_level)
		new_zoom_factor = max_zoom_level;

	/* Enqueue the detected command */
	q()->push(construct_gesture_command(MAP_CONTROL_DOUBLE_TAP,
					    NULL,
					    true, new_zoom_factor,
					    false, .0));

	/* Assumed that we can double tap only with a single finger */
	touch_point tp = _gd->_info._finger_move[0];

	/* Check if any object was affected */
	_map_object_hit_test(_gd->_view,
				  tp._x,
				  tp._y,
				  MAP_CONTROL_DOUBLE_TAP);
#endif

	/* Invoke user registered event callback */
	map_event_data_h ed =
		_map_view_create_event_data(MAP_EVENT_GESTURE);
	if(ed) {
		_map_event_data_set_gesture_type(ed,
						      MAP_CONTROL_DOUBLE_TAP);
		_map_event_data_set_xy(ed, tp._x, tp._y);
		_map_event_data_set_fingers(ed, 1);
		_map_view_invoke_event_callback(_gd->_view, ed);
	}
}

void view::gesture_processor::on_tap()
{
#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	/* Assumed that we can tap only with a single finger */
	touch_point tp = _gd->_info_history._finger_up[0];

#if DEBUG
	/* Check if any object was affected */
	map_object_h hit = _map_object_hit_test(_gd->_view,
						tp._x,
						tp._y,
						MAP_CONTROL_TAP);
	if (hit) return;
#endif

	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geolocation(_gd->_view, tp._x, tp._y, &c);
	q()->push(construct_gesture_command(MAP_CONTROL_TAP, c,
					    false, .0, false, .0));
	maps_coordinates_destroy(c);
#else
	/* Assumed that we can tap only with a single finger */
	touch_point tp = _gd->_info._finger_move[0];

	/* TODO: Soon it might be a good time to reject the
	*  use case "tap for set center", because
	*  it may lead to inconvenient sharpy UX */

	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geolocation(_gd->_view, tp._x, tp._y, &c);
	q()->push(construct_gesture_command(MAP_CONTROL_TAP, c,
					    false, .0, false, .0));
	maps_coordinates_destroy(c);

	/* Check if any object was affected */
	_map_object_hit_test(_gd->_view,
				  tp._x,
				  tp._y,
				  MAP_GESTURE_TAP);
#endif

	/* Invoke user registered event callback */
	map_event_data_h ed =
		_map_view_create_event_data(MAP_EVENT_GESTURE);
	if(ed) {
		_map_event_data_set_gesture_type(ed, MAP_CONTROL_TAP);
		_map_event_data_set_xy(ed, tp._x, tp._y);
		_map_event_data_set_fingers(ed, 1);
		_map_view_invoke_event_callback(_gd->_view, ed);
	}
}

void view::gesture_processor::on_two_finger_tap()
{
	MAPS_LOGW("\nON TWO FINGER TAP\n");

#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	const touch_point tp_f1 = _gd->_info._finger_move[0];
	const touch_point tp_f2 = _gd->_info._finger_move[1];
	const touch_point gesture_center = calc_center(tp_f1, tp_f2);

	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geolocation(_gd->_view, gesture_center._x,
						gesture_center._y, &c);

	/* Enqueue the detected command */
	q()->push(construct_gesture_command(MAP_CONTROL_TWO_FINGER_TAP, c,
					    false, .0, false, .0));
#else
	/* Default double tap effect is zoom level up */
	double new_zoom_factor =
		-1. + int(_gd->_info._start_view_state._zoom_factor);

	/* DEBUG */
	_gd->_info._start_view_state.trace();
	_gd->_info_history._start_view_state.trace();

	/* TODO: it also may be cashed in the _info._start_view_state */
	int min_zoom_level = 0;
	int error = map_view_get_min_zoom_level(_gd->_view, &min_zoom_level);
	if(error != MAPS_ERROR_NONE)
		MAPS_LOGE("ERROR [%d]: map_view_get_min_zoom_level", error);

	if(new_zoom_factor < min_zoom_level) {
		MAPS_LOGI("\t ...Correcting to min zoom level %d, "
			  "from predicted factor %f, "
			  "view: %p\n",
			  min_zoom_level, new_zoom_factor, _gd->_view);
		new_zoom_factor = min_zoom_level;
	}

	MAPS_LOGI("\t new zoom factor: %f\n", new_zoom_factor);

	/* Enqueue the detected command */
	q()->push(construct_gesture_command(MAP_CONTROL_TWO_FINGER_TAP,
					    NULL,
					    true, new_zoom_factor,
					    false, .0));

	const touch_point tp = _gd->_info._finger_move[0];

#if DEBUG
	/* TODO: desicde if two-finger tap is applicable for object hit test */
	/* Check if any object was affected */
	_map_object_hit_test(_gd->_view,
				  tp._x,
				  tp._y,
				  MAP_GESTURE_2_FINGER_TAP);
#endif
#endif

	/* Invoke user registered event callback */
	map_event_data_h ed =
		_map_view_create_event_data(MAP_EVENT_GESTURE);
	if(ed) {
		_map_event_data_set_gesture_type(ed, MAP_CONTROL_TWO_FINGER_TAP);

#ifndef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
		/* Calculate proper coordinates: center between fingers */
		const touch_point tp_f1 = _gd->_info._finger_move[0];
		const touch_point tp_f2 = _gd->_info._finger_move[1];
		const touch_point gesture_center = calc_center(tp_f1, tp_f2);
#endif
		_map_event_data_set_xy(ed, gesture_center._x,
					   gesture_center._y);

		_map_event_data_set_fingers(ed, 2);
		_map_view_invoke_event_callback(_gd->_view, ed);
	}
}

void view::gesture_processor::on_flick()
{
}

void view::gesture_processor::on_panning_finished(int finger_no)
{
#ifndef _MOVE_CENTER_COMMAND_DEFINED_
	return;
#endif /* _MOVE_CENTER_COMMAND_DEFINED_ */

	/* Obtain fresh central coordinates of the map in the Plugin */
	maps_coordinates_h c = NULL;
	_map_view_get_plugin_center(_gd->_view, &c);

	/* Directly set the updated center of the map */
	_map_view_set_center_directly(_gd->_view, c);

	maps_coordinates_destroy(c);

#if 0
	/* Assumed that we can tap only with a single finger */
	const touch_point cur_tp = _gd->_info._finger_move[finger_no];
	const touch_point start_tp = _gd->_info._finger_down[finger_no];

	/* Calculate the new center of the map by adding the delta
	* to the original center (e.g. the center of the map before the event
	* has started) */

	/* a. Calculating the delta of the gesture */
	int delta_x = cur_tp._x - start_tp._x;
	int delta_y = cur_tp._y - start_tp._y;

	/* b. Get the initial screen coordinates of the center */
	int center_x = 0;
	int center_y = 0;
	map_view_geolocation_to_screen(_gd->_view,
				     _gd->_info._start_view_state._center,
				     &center_x,
				     &center_y);

	/* c. Apply the delta to the intital center coordinates */
	center_x -= delta_x;
	center_y -= delta_y;

	/* d. Converting screent coordinates of new center to
	* the geographical */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geolocation(_gd->_view, center_x, center_y, &c);

	/* Directly set the updated center of the map */
	_map_view_set_center_directly(_gd->_view, c);

	maps_coordinates_destroy(c);
#endif
}

void view::gesture_processor::on_pan(int finger_no)
{
	/* Assumed that we can tap only with a single finger */
	const touch_point cur_tp = _gd->_info._finger_move[finger_no];


	/* Calculate the new center of the map by adding the delta
	* to the original center (e.g. the center of the map before the event
	* has started) */


#ifdef _MOVE_CENTER_COMMAND_DEFINED_
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


#else
	const touch_point start_tp = _gd->_info._finger_down[finger_no];

	/* a. Calculating the delta of the gesture */
	int delta_x = cur_tp._x - start_tp._x;
	int delta_y = cur_tp._y - start_tp._y;

	/* b. Get the initial screen coordinates of the center */
	int center_x = 0;
	int center_y = 0;
	map_view_geolocation_to_screen(_gd->_view,
				     _gd->_info._start_view_state._center,
				     &center_x,
				     &center_y);

	/* c. Apply the delta to the intital center coordinates */
	center_x -= delta_x;
	center_y -= delta_y;

	/* d. Converting screent coordinates of new center to
	* the geographical */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geolocation(_gd->_view, center_x, center_y, &c);

	/* e. Construct the corresponding command */
	q()->push(construct_gesture_command(MAP_GESTURE_SCROLL, c,
					    false, .0, false, .0));
	maps_coordinates_destroy(c);
#endif /* _MOVE_CENTER_COMMAND_DEFINED_ */

#if DEBUG
	/* TODO: desicde if panning is applicable for object hit test */
	/* Check if any object was affected */
	_map_object_hit_test(_gd->_view,
				  tp._x,
				  tp._y,
				  MAP_GESTURE_SCROLL);
#endif

	/* Invoke user registered event callback */
	map_event_data_h ed =
		_map_view_create_event_data(MAP_EVENT_GESTURE);
	if(ed) {
		_map_event_data_set_gesture_type(ed, MAP_GESTURE_SCROLL);
		_map_event_data_set_xy(ed, cur_tp._x, cur_tp._y);
		_map_event_data_set_fingers(ed, 1);
		_map_view_invoke_event_callback(_gd->_view, ed);
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

void view::gesture_processor::on_single_finger_zoom()
{
	gesture_detector::log("view::gesture_processor::on_single_finger_zoom",
			      gesture_detector::FG_YELLOW);

	/* Assumed that we do the zoom using single finger */

	/* First finger effective way by now */
	const touch_point start_tp_f1 = _gd->_info._finger_down[0];
	const touch_point cur_tp_f1 = _gd->_info._finger_move[0];


	/***********************/
	MAPS_LOGI("%c[%d;%d;%dm"
		  "Finger1: start(%d, %d), cur(%d, %d)\t"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, gesture_detector::FG_YELLOW,
		  start_tp_f1._x, start_tp_f1._y, cur_tp_f1._x, cur_tp_f1._y,
		  0x1B, 0, 0, 0);
	/***********************/


	/* Calculating the current zoom factor, accordingly to vertical way
	 *  of finger */
	const int vertical_way = cur_tp_f1._y - start_tp_f1._y;
	if(vertical_way == 0)
		return; /* No zoom happend */

	int map_height = 0;
	map_view_get_screen_location(_gd->_view, NULL, NULL, NULL, &map_height);
	const int half_height = map_height / 2;

	const double delta_zoom = 1. * vertical_way / half_height;
	double new_zoom_factor =
		_gd->_info._start_view_state._zoom_factor + delta_zoom;


	/* Correct the zoom factor accordingly to allowed limits */
	/* TODO: it also may be cashed in the _info._start_view_state */
	int min_zoom_level = 0;
	int max_zoom_level = 0;
	map_view_get_min_zoom_level(_gd->_view, &min_zoom_level);
	map_view_get_max_zoom_level(_gd->_view, &max_zoom_level);
	if(new_zoom_factor < min_zoom_level)
		new_zoom_factor = min_zoom_level;
	if(new_zoom_factor > max_zoom_level)
		new_zoom_factor = max_zoom_level;

	/* Invoke user registered event callback for ZOOM */
	do {
		map_event_data_h ed =
			_map_view_create_event_data(MAP_EVENT_GESTURE);
		if(!ed)
			break;
		_map_event_data_set_gesture_type(ed,
						MAP_GESTURE_DOUBLE_TAP_ZOOM);
		_map_event_data_set_scale_factor(ed, new_zoom_factor);
		_map_event_data_set_fingers(ed, 2);

		/* Find the current center of the gesture */
		const touch_point cur_center = cur_tp_f1;
			/*_gd->_info._start_view_state._center;*/
		_map_event_data_set_xy(ed, cur_center._x, cur_center._y);
		_map_view_invoke_event_callback(_gd->_view, ed);
	} while(false);

#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	/* Enqueue the detected zomm command */
	q()->push(construct_gesture_command(MAP_GESTURE_DOUBLE_TAP_ZOOM,
					    _gd->_info._start_view_state._center,
					    true,
					    new_zoom_factor,
					    false,
					    .0));
#else
	/* Enqueue the detected zomm command */
	q()->push(construct_gesture_command(MAP_GESTURE_DOUBLE_TAP_ZOOM,
					    NULL,
					    true,
					    new_zoom_factor,
					    false,
					    .0));
#endif
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
	if(zc.zoom_happend()) {
		#if 1
		/* Apply newly calculated zoom factor */
		new_zoom_factor += _gd->_info._start_view_state._zoom_factor;
		#else
		/* Apply newly calculated zoom factor */
		if(new_zoom_factor > 1.)
			new_zoom_factor =
				_gd->_info._start_view_state._zoom_factor
				+ new_zoom_factor - 1.;
		else if(new_zoom_factor != 0.) {
			 new_zoom_factor =
				 _gd->_info._start_view_state._zoom_factor
				 - (1. - new_zoom_factor);
		}
		else
			 new_zoom_factor =
				 _gd->_info._start_view_state._zoom_factor;
		#endif

		/* Correct the zoom factor accordingly to allowed limits */
		/* TODO: it also may be cashed in the _info._start_view_state */
		int min_zoom_level = 0;
		int max_zoom_level = 0;
		map_view_get_min_zoom_level(_gd->_view, &min_zoom_level);
		map_view_get_max_zoom_level(_gd->_view, &max_zoom_level);
		if(new_zoom_factor < min_zoom_level)
			new_zoom_factor = min_zoom_level;
		if(new_zoom_factor > max_zoom_level)
			new_zoom_factor = max_zoom_level;

		/* Check if the zoom changed relatively to initial state */
		zoom_changed = _gd->_info._start_view_state._zoom_factor
			!= new_zoom_factor;
	}

	/* Analyze rotation angle changes */
	bool rotation_changed = false;
	if(zc.rotation_happend()) {
		/* Apply newly calculated rotation angle */
		new_rotation_angle =
			_gd->_info._start_view_state._rotation_angle
			+ new_rotation_angle;

		/* Correct the rotation angle to normalize it
		 *  inside the diapazone of 0..360 degree */
		new_rotation_angle -= 360. * (int(new_rotation_angle) / 360);
		new_rotation_angle += 360. * (int(new_rotation_angle) / 360);

		/* Check if the angle changed relatively to initial state */
		rotation_changed =  _gd->_info._start_view_state._rotation_angle
			!= new_rotation_angle;
	}

#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	if(!zoom_changed && !rotation_changed)
		return; // Seems nothing changed, we can return

	do {
		map_event_data_h ed =
			_map_view_create_event_data(MAP_EVENT_GESTURE);
		if(!ed)
			break;
		_map_event_data_set_gesture_type(ed, MAP_GESTURE_PINCH_ZOOM);
		_map_event_data_set_scale_factor(ed, new_zoom_factor);
		_map_event_data_set_rotation_angle(ed, new_rotation_angle);
		_map_event_data_set_fingers(ed, 2);

		/* Find the current center of the gesture */
		const touch_point cur_center = calc_center(cur_tp_f1,
							   cur_tp_f2);
		_map_event_data_set_xy(ed,
					    cur_center._x,
					    cur_center._y);
		_map_view_invoke_event_callback(_gd->_view, ed);
	} while(false);
#else
	/* Invoke user registered event callback for ZOOM */
	do {
		if(!zoom_changed)
			break;
		map_event_data_h ed =
			_map_view_create_event_data(MAP_EVENT_GESTURE);
		if(!ed)
			break;
		_map_event_data_set_gesture_type(ed, MAP_GESTURE_ZOOM);
		_map_event_data_set_scale_factor(ed, new_zoom_factor);
		_map_event_data_set_fingers(ed, 2);

		/* Find the current center of the gesture */
		const touch_point cur_center = calc_center(cur_tp_f1,
							   cur_tp_f2);
		_map_event_data_set_xy(ed,
					    cur_center._x,
					    cur_center._y);
		_map_view_invoke_event_callback(_gd->_view, ed);
	} while(false);

	/* Invoke user registered event callback for ROTATION */
	do {
		if(rotation_changed)
			break;

		map_event_data_h ed =
			_map_view_create_event_data(MAP_EVENT_GESTURE);
		if(!ed)
			break;
		_map_event_data_set_gesture_type(ed, MAP_GESTURE_ROTATE);
		_map_event_data_set_rotation_angle(ed, new_rotation_angle);
		_map_event_data_set_fingers(ed, 2);

		/* Find the current center of the gesture */
		const touch_point cur_center = calc_center(cur_tp_f1,
							   cur_tp_f2);
		_map_event_data_set_xy(ed,
					    cur_center._x,
					    cur_center._y);
		_map_view_invoke_event_callback(_gd->_view, ed);
	} while(false);


	if(!zoom_changed && !rotation_changed)
		return; // Seems nothing changed, we can return
#endif

	/* Ignore center move if zoom is not available */
	const bool movable_center =
		_map_view_is_gesture_available(_gd->_view, MAP_GESTURE_PINCH_ZOOM);
	if(!movable_center) {
		q()->push(construct_gesture_command(MAP_GESTURE_PINCH_ZOOM,
						    NULL,
						    zoom_changed,
						    new_zoom_factor,
						    rotation_changed,
						    new_rotation_angle));
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
	map_view_geolocation_to_screen(_gd->_view,
				     _gd->_info._start_view_state._center,
				     &center_x,
				     &center_y);


	/* c. Apply the delta to the intital center coordinates */
	center_x -= delta_x;
	center_y -= delta_y;


	/* d. Converting screent coordinates of new center to
	* the geographical */
	maps_coordinates_h new_center = NULL;
	map_view_screen_to_geolocation(_gd->_view,
				     center_x,
				     center_y,
				     &new_center);

	/* e. Enque the command to move the center */
	q()->push(new session::command_view_set_center(get_maps(),
							       _gd->_view,
							       new_center));

	/* f. Enqueue the detected zomm command */
	q()->push(construct_gesture_command(MAP_GESTURE_PINCH_ZOOM,
					    new_center,
					    zoom_changed,
					    new_zoom_factor,
					    rotation_changed,
					    new_rotation_angle));

	maps_coordinates_destroy(new_center);
}

void view::gesture_processor::on_pinch()
{
}



/* ---------------------------------------------------------------------------*/
/*    VIEW EVENT STREAM                                                       */
/* ---------------------------------------------------------------------------*/


view::finger_event_stream::finger_event_stream(map_view_h v)
	: _d(NULL)
{
	/* TODO: extract in dedicated factory in map_view.cpp */
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
	if(finger_no > MAX_FINGERS)
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
	if(finger_no > MAX_FINGERS)
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
	if(finger_no > MAX_FINGERS)
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
