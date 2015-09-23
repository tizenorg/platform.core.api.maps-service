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


static const int __MIL_SEC = 1;
static const int __SEC = 1000 * __MIL_SEC;

/* Half a second is a popular duration of double click in UI */
static const int __CLICK_DURATION = 500 * __MIL_SEC;

static const int __CLICK_AREA = 10; /* May be same as hit test accuracy */


/*----------------------------------------------------------------------------*/


/* Using protected functions of Map View */
extern bool _map_viwe_is_gesture_available(map_view_h view,
					   map_view_gesture_e gesture);
extern map_view_action_e _map_view_get_gesture_action(map_view_h view,
					      map_view_gesture_e gesture);
extern void *_map_view_get_maps_service_ptr(map_view_h view);

extern map_view_object_h _map_view_object_hit_test(map_view_h view,
						   const int x,
						   const int y,
						   map_view_gesture_e gesture);

extern int _map_view_event_data_set_gesture_type(map_view_event_data_h event,
					 const map_view_gesture_e gesture_type);

extern int _map_view_event_data_set_xy(map_view_event_data_h event,
				       const int x, const int y);

extern int _map_view_event_data_set_fingers(map_view_event_data_h event,
					    const int fingers);

extern int _map_view_event_data_set_zoom_factor(map_view_event_data_h event,
						const double zoom_factor);

extern int _map_view_event_data_set_rotation_angle(map_view_event_data_h event,
						   const double rotation_angle);

extern int _map_view_event_data_set_tilt(map_view_event_data_h event,
					 const double tilt);

extern map_view_event_data_h _map_view_create_event_data(map_view_event_type_e
							 type);
extern void _map_view_invoke_event_callback(map_view_h view,
					    map_view_event_data_h event_data);


#ifdef _MOVE_CENTER_COMMAND_DEFINED_
extern int _map_view_set_center_directly(const map_view_h view,
				  const maps_coordinates_h coordinates);
#endif /* _MOVE_CENTER_COMMAND_DEFINED_ */

/* ---------------------------------------------------------------------------*/


view::touch_point::touch_point()
	: _x(0)
	, _y(0)
	, _timestamp(0)
{
	reset();
}

view::touch_point::~touch_point()
{
}

view::touch_point::touch_point(const int x,
			       const int y,
			       const unsigned int ts)
	: _x(x)
	, _y(y)
	, _timestamp(ts)
{
}

view::touch_point::touch_point(const touch_point &src)
	: _x(0)
	, _y(0)
	, _timestamp(0)
{
	*this = src;
}

view::touch_point &view::touch_point::operator=(const touch_point &src)
{
	if(this != &src) {
		_x = src._x;
		_y = src._y;
		_timestamp = src._timestamp;
	}
	return *this;
}

void view::touch_point::reset()
{
	_x = 0;
	_y = 0;
	_timestamp = 0;
}

bool view::touch_point::empty() const
{
	if(_x != 0)
		return false;
	if(_y != 0)
		return false;
	if(_timestamp != 0)
		return false;
	return true;
}
/* ---------------------------------------------------------------------------*/


view::map_state::map_state()
	: _center(NULL)
	, _zoom_factor(.1)
	, _rotation_angle(.0)
	, _tilt(.0)
{
	reset();
}

view::map_state::~map_state()
{
	if(_center)
		maps_coordinates_destroy(_center);
	_center = NULL;
}

view::map_state::map_state(const map_state &src)
	: _center(NULL)
	, _zoom_factor(.1)
	, _rotation_angle(.0)
	, _tilt(.0)
{
	*this = src;
}

view::map_state &view::map_state::operator=(const map_state &src)
{
	if(this != &src) {
		reset();
		maps_coordinates_clone(src._center, &_center);
		_zoom_factor = src._zoom_factor;
		_rotation_angle = src._rotation_angle;
		_tilt = src._tilt;
	}
	return *this;
}

void view::map_state::reset()
{
	if(_center)
		maps_coordinates_destroy(_center);
	_center = NULL;

	_zoom_factor = .1;
	_rotation_angle = .0;
	_tilt = .0;
}

void view::map_state::capture(map_view_h view)
{
	if(!view)
		return;

	reset();

	map_view_get_center(view, &_center);
	map_view_get_zoom_factor(view, &_zoom_factor);
	map_view_get_orientation(view, &_rotation_angle);
	map_view_get_tilt(view, &_tilt);
}

/* ---------------------------------------------------------------------------*/

view::runtime_touch_info::runtime_touch_info()
	: _fingers(0)
	  , _gesture_type(MAP_GESTURE_NONE)
	  /*, _going(false)*/
{
	reset();
}

view::runtime_touch_info::~runtime_touch_info()
{
}

view::runtime_touch_info::runtime_touch_info(const runtime_touch_info &src)
{
	*this = src;
}

view::runtime_touch_info &view::runtime_touch_info::operator=(const
							      runtime_touch_info
							      &src)
{
	if(this != &src) {
		_fingers = src._fingers;
		_gesture_type = src._gesture_type;
		/*_going = src._going;*/
		_start_state = src._start_state;

		for(unsigned int i = 0; i < MAX_FINGERS; i ++) {
			_start_touch[i] = src._start_touch[i];
			_cur_touch[i] = src._cur_touch[i];
			_prev_touch[i] = src._prev_touch[i];
			_up_touch[i] = src._up_touch[i];
			_finger_pressed[i] = src._finger_pressed[i];
		}
	}
	return *this;
}

void view::runtime_touch_info::reset()
{
	/*_going = false;*/

	_fingers = 0;

	for(unsigned int i = 0; i < MAX_FINGERS; i ++) {
		_start_touch[i].reset();
		_cur_touch[i].reset();
		_prev_touch[i].reset();
		_up_touch[i].reset();
		_finger_pressed[i] = false;
	}

	_gesture_type = MAP_GESTURE_NONE;

	_start_state.reset();
}


/* ---------------------------------------------------------------------------*/

view::gesture_detector::gesture_detector(map_view_h v)
	: _view(v)
	  , _gp(this)
{
}

view::gesture_detector::~gesture_detector()
{
}

void view::gesture_detector::reset()
{
	/* Store the previous gesture data for possible future analysis */
	_info_history = _info;

	/* Reset current gesture info */
	_info.reset();
}

void view::gesture_detector::tap(int finger_no, const touch_point &tp)
{
	if(_info._fingers == 0) {

		/* This is the begining position of each gesture,
		 *  so we'd better to store the initial state of
		 *  the Map View */
		_info._start_state.capture(_view);

		/* Processing of single finger gesture:
		 * - long press
		 */

		_info._fingers = 1;
		_info._start_touch[0] = tp;
		_info._finger_pressed[0] = true;

		/* Check if it is a long press gesure */
		test_long_press_gesture();

	} else { /* TODO: els if(_info._fingers == 1) */
		/* Processing of multi-finger gesture:
		 * - none
		 */

		_info._fingers ++;
		_info._start_touch[finger_no] = tp;
		_info._finger_pressed[finger_no] = true;
	}
}

void view::gesture_detector::up(int finger_no, const touch_point &tp)
{
	if(_info._fingers == 1) {
		/* Processing of single finger gesture:
		 * - tap
		 * - double tap
		 */

		_info._up_touch[0] = tp;

		/* Check if it is a tap or double tap gesture */
		if(!test_panning_gesture_finished())
			if(!test_two_finger_tap_gesture())
				if(!test_double_tap_gesture())
					test_tap_gesture();

		/* This is the end of each gesture */
		reset();

	} else { /* TODO: els if(_info._fingers == 2) */
		/* Processing of multi-finger gesture:
		 * - 2-finger tap
		 */

		_info._cur_touch[finger_no] = tp;
		_info._up_touch[finger_no] = tp;

		/* Check if it is a two finger tap gesture */
		test_two_finger_tap_gesture();


		/* Remove the info about lifted finger */
		_info._fingers --;
		_info._start_touch[finger_no].reset();
		_info._finger_pressed[finger_no] = false;

		/* TODO:
		 *
		 *  [IMPORTANT!!!!]
		 *
		 *  Arrange remaining info, so the indexes of fingers
		 *  on the canvas are the same as indexes in the gesture
		 *  processor
		 *  */
	}
}

void view::gesture_detector::move(int finger_no, const touch_point &tp)
{
	if(_info._fingers == 1) {
		/* Processing of single finger gesture:
		 * - pan (including two-finger pan to tilt)
		 * - flick
		 */

		_info._prev_touch[0] = _info._cur_touch[0];
		_info._cur_touch[0] = tp;

		/* Check if it is a pan or flick gesture */
		if(!test_flick_gesture())
			test_pan_gesture();


	} else {
		/* Processing of multi-finger gesture:
		 * - zoom
		 * - rotate
		 * - pinch
		 */

		_info._prev_touch[finger_no] = _info._cur_touch[finger_no];
		_info._cur_touch[finger_no] = tp;

		/* Check if it is a zoom or rotate or pinch gesture */
		if(!test_tilt_gesture())
			if(!test_pinch_gesture())
				test_zoom_rotate_gesture();
	}

}

int view::gesture_detector::get_trajectory_effective_length(
							const touch_point &p1,
						    const touch_point &p2)
{
	int dx = p2._x - p1._x;
	int dy = p2._y - p1._y;
	int etl = sqrt(dx * dx + dy * dy);
	return etl;
}

bool view::gesture_detector::test_long_press_gesture()
{
	/* TODO: start timer for detecting "long press" gesture */
	/*_info._gesture_type = MAP_GESTURE_PRESS;*/
	return false;
}

bool view::gesture_detector::test_double_tap_gesture()
{
	/* If a single finger taped twice on the Map View
	 *  and it happed whithin a short time frame,
	 *  than it is likely a double tap gesture;
	 *  The time frame is calculated as the current time - previous click
	 *  time */

	/* Note, We expect, that the gesture is done with a single finger */
	if(_info._fingers != 1)
		return false;


	const touch_point prev_up = _info_history._up_touch[0];
	if(prev_up._timestamp == 0)
		return false; /* It is very first up of the Map View ever */

	const touch_point cur_up = _info._up_touch[0];
	const unsigned int time = cur_up._timestamp - prev_up._timestamp;
	if((time > 0) && (time < __CLICK_DURATION)) {
		_info._gesture_type = MAP_GESTURE_DOUBLE_TAP;
		_gp.on_double_tap();
		return true;
	}
	return false;
}

bool view::gesture_detector::test_panning_gesture_finished()
{
#ifndef _MOVE_CENTER_COMMAND_DEFINED_
	return false;
#endif /* _MOVE_CENTER_COMMAND_DEFINED_ */
	if(_info._gesture_type == MAP_GESTURE_PAN)
		/* Finalize the panning gesture */
		_gp.on_panning_finished();
	return true;
}

bool view::gesture_detector::test_tap_gesture()
{
	/* If a single finger touched a map view,
	 * and lifted before long press gesture is detected,
	 * than it is a tap gesture */

	/* Note, We expect, that the gesture is done with a single finger */
	if(_info._fingers != 1)
		return false;

	/* Check if the gesture event is short enough */
	touch_point p1 = _info._start_touch[0];
	touch_point p2 = _info._up_touch[0];
	unsigned int time = p2._timestamp - p1._timestamp;
	if((time > 0) && (time < __CLICK_DURATION)) {
		/* Check if it is a click, but not the movement */
		if(get_trajectory_effective_length(p1, p2) < __CLICK_AREA) {
			_info._gesture_type = MAP_GESTURE_TAP;
			_gp.on_tap();
			return true;
		}
	}
	return false;
}

bool view::gesture_detector::test_two_finger_tap_gesture()
{
	/* If Two fingers tapped a map view, than it is detected as
	 *  corresponding "two-finger-tap gesture */


	if((_info._fingers != 2) && (_info._fingers != 1))
		return false;

	if(_info._start_touch[1].empty())
		return false; /* The second finger was not pressed */

	const touch_point f1_start = _info._start_touch[0];
	const touch_point f1_finish = _info._up_touch[0];
	const touch_point f2_start = _info._start_touch[1];
	const touch_point f2_finish = _info._up_touch[1];

	switch(_info._fingers) {
	case 1: {
		long long time = f1_finish._timestamp - f2_start._timestamp;
		if(time < 0)
			time *= -1;
		if(time > __CLICK_DURATION)
			return false;

		break;
	}
	case 2: {
		long long time = f1_finish._timestamp - f1_start._timestamp;
		if(time < 0)
			time *= -1;
		if(time > __CLICK_DURATION)
			return false;

		break;
	}
	default:
		return false;
	}

	if(!f1_start.empty() && !f1_finish.empty())
		if(get_trajectory_effective_length(f1_start, f1_finish)
		   > __CLICK_AREA)
			return false;

	if(!f2_start.empty() && !f2_finish.empty())
		if(get_trajectory_effective_length(f2_start, f2_finish)
		   > __CLICK_AREA)
			return false;

	_info._gesture_type = MAP_GESTURE_2_FINGER_TAP;
	_gp.on_two_finger_tap();
	return true;
}

bool view::gesture_detector::test_flick_gesture()
{
	/* If a finger is moved rapidely left-to-right or right-to-left,
	 *  than it is considered as flicj gesture */

	/* TODO: */
	/*_info._gesture_type = MAP_GESTURE_FLICK;*/

	return false;
}

bool view::gesture_detector::test_pan_gesture()
{
	/* If a single finger is moved on the map view, than it is recognized
	 *  as a pan gesture */

	/* Note, We expect, that the gesture is done with a single finger */
	if(_info._fingers != 1)
		return false;

	/* No special checks are need here */
	_info._gesture_type = MAP_GESTURE_PAN;
	_gp.on_pan();
	return true;
}

bool view::gesture_detector::test_tilt_gesture()
{
	/* If two fingers are moved synchronously in vertical direction,
	 *  than it is a tilt gesture */

	/* TODO: */
	/*_info._gesture_type = MAP_GESTURE_TILT;*/

	return false;
}

bool view::gesture_detector::test_zoom_rotate_gesture()
{
	/* It two fingetrs are moving in arbitrary trajectories,
	 *  than it is a zoom and/or rotate gesture */

	/* Note, We expect, that the gesture is done with a pair of fingers */
	if(_info._fingers != 2)
		return false;

	/* No special checks are need here */
	_info._gesture_type = MAP_GESTURE_ZOOM;
	_gp.on_zoom_rotate();
	return true;
}

bool view::gesture_detector::test_pinch_gesture()
{
	/* If two or more fingers moved rapidly toward each other,
	 *  than it is recognized as pinch gesture */

	/*_info._gesture_type = MAP_GESTURE_PINCH;*/
	return false;
}


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
	if((start_r != .0) && (cur_r != .0)) {
		_new_zoom_factor = cur_r / start_r;
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

void view::gesture_processor::on_long_press()
{
}

/* TODO: | DEBUG ONLY | REMOVE WHEN NO NEEDED */
map_view_object_h view::gesture_processor::__DEBUG_add_marker(const int x,
							      const int y,
					const map_view_marker_type_e type)

{
	return NULL;
	maps_coordinates_h coords = NULL;
	map_view_screen_to_geography(_gd->_view, x,  y, &coords);
	map_view_object_h marker = NULL;
	map_view_add_marker(_gd->_view, coords,
			    "/tmp/maps/icon_bluestar.png",
			    type,
			    &marker);
	maps_coordinates_destroy(coords);
	return marker;
}

map_view_object_h view::gesture_processor::__DEBUG_add_geo_marker(
					const maps_coordinates_h coords,
					const map_view_marker_type_e type)

{
	return NULL;
	map_view_object_h marker = NULL;
	map_view_add_marker(_gd->_view, coords,
			    "/tmp/maps/icon_redstar.png",
			    type,
			    &marker);
	return marker;
}
/* ----- | DEBUG ONLY | REMOVE WHEN NO NEEDED */

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
	return session::command_queue_sync::interface();
}

void *view::gesture_processor::get_maps()
{
	return _map_view_get_maps_service_ptr(_gd->_view);
}

session::command *view::gesture_processor::construct_gesture_command(
						map_view_gesture_e gesture,
						const maps_coordinates_h c,
						const double zoom,
						const double angle,
						const double tilt)
{
	/* Check if the gesture is available */
	if (!_map_viwe_is_gesture_available(_gd->_view, gesture)) {
		return session::command::empty_ptr();
	}

	/* Perform gesture action */
	switch(_map_view_get_gesture_action(_gd->_view, gesture)) {
	case MAP_ACTION_SET_CENTER: {
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
	case MAP_ACTION_ZOOM: {
		double zoom_factor = zoom;
		if(zoom_factor == .0)
			map_view_get_zoom_factor(_gd->_view, &zoom_factor);
		return new session::command_view_zoom(get_maps(),
						      _gd->_view,
						      zoom_factor);
	}
	case MAP_ACTION_ROTATE: {
		double rotation_angle = angle;
		if(rotation_angle == .0)
			map_view_get_orientation(_gd->_view, &rotation_angle);
		rotation_angle -= (int(rotation_angle) / 360) * 360;
		return new session::command_view_rotate(get_maps(),
							_gd->_view,
							rotation_angle);
	}
	case MAP_ACTION_TILT: {
		double new_tilt = tilt;
		if(new_tilt == .0)
			map_view_get_tilt(_gd->_view, &new_tilt);
		return new session::command_view_tilt(get_maps(),
						      _gd->_view,
						      new_tilt);
	}

	case MAP_ACTION_NONE:
	default:
		return session::command::empty_ptr();
	}
}

void view::gesture_processor::on_double_tap()
{
	/* Default double tap effect is zoom level up */
	double new_zoom_factor = 1. + int(_gd->_info._start_state._zoom_factor);

	/* TODO: it also may be cashed in the _info._start_state */
	int max_zoom_level = 0;
	map_view_get_max_zoom_level(_gd->_view, &max_zoom_level);

	if(new_zoom_factor > max_zoom_level)
		new_zoom_factor = max_zoom_level;

	/* Enqueue the detected command */
	q()->push(construct_gesture_command(MAP_GESTURE_DOUBLE_TAP,
					    NULL,
					    new_zoom_factor,
					    .0,
					    .0));

	/* Assumed that we can double tap only with a single finger */
	touch_point tp = _gd->_info._cur_touch[0];

	/* Check if any object was affected */
	_map_view_object_hit_test(_gd->_view,
				  tp._x,
				  tp._y,
				  MAP_GESTURE_DOUBLE_TAP);

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_GESTURE);
	if(ed) {
		_map_view_event_data_set_gesture_type(ed,
						      MAP_GESTURE_DOUBLE_TAP);
		_map_view_event_data_set_xy(ed, tp._x, tp._y);
		_map_view_event_data_set_fingers(ed, 1);
		_map_view_invoke_event_callback(_gd->_view, ed);
	}
}

void view::gesture_processor::on_tap()
{
	/* Assumed that we can tap only with a single finger */
	touch_point tp = _gd->_info._cur_touch[0];

	/* TODO: Soon it might be a good time to reject the
	*  use case "tap for set center", because
	*  it may lead to inconvenient sharpy UX */


	/* Enqueue the detected command */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geography(_gd->_view, tp._x, tp._y, &c);
	q()->push(construct_gesture_command(MAP_GESTURE_TAP, c, .0, .0, .0));
	maps_coordinates_destroy(c);

	/* Check if any object was affected */
	_map_view_object_hit_test(_gd->_view,
				  tp._x,
				  tp._y,
				  MAP_GESTURE_TAP);

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_GESTURE);
	if(ed) {
		_map_view_event_data_set_gesture_type(ed, MAP_GESTURE_TAP);
		_map_view_event_data_set_xy(ed, tp._x, tp._y);
		_map_view_event_data_set_fingers(ed, 1);
		_map_view_invoke_event_callback(_gd->_view, ed);
	}
}

void view::gesture_processor::on_two_finger_tap()
{
	MAPS_LOGW("\nON TWO FINGER TAP\n");

	/* Default double tap effect is zoom level up */
	double new_zoom_factor = -1. + int(_gd->_info._start_state._zoom_factor);

	/* TODO: it also may be cashed in the _info._start_state */
	int min_zoom_level = 0;
	map_view_get_min_zoom_level(_gd->_view, &min_zoom_level);

	if(new_zoom_factor < min_zoom_level)
		new_zoom_factor = min_zoom_level;

	/* Enqueue the detected command */
	q()->push(construct_gesture_command(MAP_GESTURE_2_FINGER_TAP,
					    NULL,
					    new_zoom_factor,
					    .0,
					    .0));

	const touch_point tp = _gd->_info._cur_touch[0];

#if 0
	/* TODO: desicde if two-finger tap is applicable for object hit test */
	/* Check if any object was affected */
	_map_view_object_hit_test(_gd->_view,
				  tp._x,
				  tp._y,
				  MAP_GESTURE_2_FINGER_TAP);
#endif

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_GESTURE);
	if(ed) {
		_map_view_event_data_set_gesture_type(ed,
						      MAP_GESTURE_2_FINGER_TAP);

		/* Calculate proper coordinates: center between fingers */
		const touch_point tp_f1 = _gd->_info._cur_touch[0];
		const touch_point tp_f2 = _gd->_info._cur_touch[1];
		const touch_point gesture_center = calc_center(tp_f1, tp_f2);
		_map_view_event_data_set_xy(ed,
					    gesture_center._x,
					    gesture_center._y);

		_map_view_event_data_set_fingers(ed, 2);
		_map_view_invoke_event_callback(_gd->_view, ed);
	}
}

void view::gesture_processor::on_flick()
{
}

void view::gesture_processor::on_panning_finished()
{
#ifndef _MOVE_CENTER_COMMAND_DEFINED_
	return;
#endif /* _MOVE_CENTER_COMMAND_DEFINED_ */

	/* Assumed that we can tap only with a single finger */
	const touch_point cur_tp = _gd->_info._cur_touch[0];
	const touch_point start_tp = _gd->_info._start_touch[0];

	/* Calculate the new center of the map by adding the delta
	* to the original center (e.g. the center of the map before the event
	* has started) */

	/* a. Calculating the delta of the gesture */
	int delta_x = cur_tp._x - start_tp._x;
	int delta_y = cur_tp._y - start_tp._y;

	/* b. Get the initial screen coordinates of the center */
	int center_x = 0;
	int center_y = 0;
	map_view_geography_to_screen(_gd->_view,
				     _gd->_info._start_state._center,
				     &center_x,
				     &center_y);

	/* c. Apply the delta to the intital center coordinates */
	center_x -= delta_x;
	center_y -= delta_y;

	/* d. Converting screent coordinates of new center to
	* the geographical */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geography(_gd->_view, center_x, center_y, &c);

	/* Directly set the updated center of the map */
	_map_view_set_center_directly(_gd->_view, c);

	maps_coordinates_destroy(c);
}

void view::gesture_processor::on_pan()
{
	/* Assumed that we can tap only with a single finger */
	const touch_point cur_tp = _gd->_info._cur_touch[0];


	/* Calculate the new center of the map by adding the delta
	* to the original center (e.g. the center of the map before the event
	* has started) */


#ifdef _MOVE_CENTER_COMMAND_DEFINED_
	touch_point prev_tp = _gd->_info._prev_touch[0];
	if(prev_tp.empty())
		prev_tp = _gd->_info._start_touch[0];

	/* a. Calculating the delta of the gesture */
	int delta_x = cur_tp._x - prev_tp._x;
	int delta_y = cur_tp._y - prev_tp._y;


	/* b. Enque the command to move the center */
	q()->push(new session::command_view_move_center(get_maps(),
							_gd->_view,
							-delta_x,
							-delta_y));
#else
	const touch_point start_tp = _gd->_info._start_touch[0];

	/* a. Calculating the delta of the gesture */
	int delta_x = cur_tp._x - start_tp._x;
	int delta_y = cur_tp._y - start_tp._y;

	/* b. Get the initial screen coordinates of the center */
	int center_x = 0;
	int center_y = 0;
	map_view_geography_to_screen(_gd->_view,
				     _gd->_info._start_state._center,
				     &center_x,
				     &center_y);

	/* c. Apply the delta to the intital center coordinates */
	center_x -= delta_x;
	center_y -= delta_y;

	/* d. Converting screent coordinates of new center to
	* the geographical */
	maps_coordinates_h c = NULL;
	map_view_screen_to_geography(_gd->_view, center_x, center_y, &c);

	/* e. Construct the corresponding command */
	q()->push(construct_gesture_command(MAP_GESTURE_PAN, c, .0, .0, .0));
	maps_coordinates_destroy(c);
#endif /* _MOVE_CENTER_COMMAND_DEFINED_ */

#if 0
	/* TODO: desicde if panning is applicable for object hit test */
	/* Check if any object was affected */
	_map_view_object_hit_test(_gd->_view,
				  tp._x,
				  tp._y,
				  MAP_GESTURE_PAN);
#endif

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_GESTURE);
	if(ed) {
		_map_view_event_data_set_gesture_type(ed, MAP_GESTURE_PAN);
		_map_view_event_data_set_xy(ed, cur_tp._x, cur_tp._y);
		_map_view_event_data_set_fingers(ed, 1);
		_map_view_invoke_event_callback(_gd->_view, ed);
	}
}

void view::gesture_processor::on_tilt()
{
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
	/* Assumed that we can zoom&rotate using only a pair of fingers */

	/* First finger effective way by now */
	const touch_point start_tp_f1 = _gd->_info._start_touch[0];
	const touch_point cur_tp_f1 = _gd->_info._cur_touch[0];

	/* Second finger effective way by now */
	const touch_point start_tp_f2 = _gd->_info._start_touch[1];
	const touch_point cur_tp_f2 = _gd->_info._cur_touch[1];


	/* Calculating the current zoom factor, accordingly to effecitve ways
	 *  of fingers */
	zoom_calculator zc(start_tp_f1, cur_tp_f1, start_tp_f2, cur_tp_f2);
	double new_zoom_factor = zc.get_zoom_factor();
	double new_rotation_angle = zc.get_rotation_angle();


	/* Analyse zoom factor changes */
	bool zoom_changed = false;
	if(zc.zoom_happend()) {
		/* Apply newly calculated zoom factor */
		if(new_zoom_factor > 1.)
			new_zoom_factor = _gd->_info._start_state._zoom_factor
				+ new_zoom_factor - 1.;
		else if(new_zoom_factor != 0.) {
			 new_zoom_factor = _gd->_info._start_state._zoom_factor
				 - (1. - new_zoom_factor);
		}
		else
			 new_zoom_factor = _gd->_info._start_state._zoom_factor;


		/* Correct the zoom factor accordingly to allowed limits */
		/* TODO: it also may be cashed in the _info._start_state */
		int min_zoom_level = 0;
		int max_zoom_level = 0;
		map_view_get_min_zoom_level(_gd->_view, &min_zoom_level);
		map_view_get_max_zoom_level(_gd->_view, &max_zoom_level);
		if(new_zoom_factor < min_zoom_level)
			new_zoom_factor = min_zoom_level;
		if(new_zoom_factor > max_zoom_level)
			new_zoom_factor = max_zoom_level;

		/* Check if the zoom changed relatively to initial state */
		zoom_changed = _gd->_info._start_state._zoom_factor
			!= new_rotation_angle;
	}

	/* Analyze rotation angle changes */
	bool rotation_changed = false;
	if(zc.rotation_happend()) {
		/* Apply newly calculated rotation angle */
		new_rotation_angle = _gd->_info._start_state._rotation_angle
			+ new_rotation_angle;

		/* Correct the rotation angle to normalize it
		 *  inside the diapazone of 0..360 degree */
		new_rotation_angle -= 360. * (int(new_rotation_angle) / 360);
		new_rotation_angle += 360. * (int(new_rotation_angle) / 360);

		/* Check if the angle changed relatively to initial state */
		rotation_changed =  _gd->_info._start_state._rotation_angle
			!= new_rotation_angle;
	}


	/* Invoke user registered event callback for ZOOM */
	do {
		if(!zoom_changed)
			break;
		map_view_event_data_h ed =
			_map_view_create_event_data(MAP_VIEW_EVENT_GESTURE);
		if(!ed)
			break;
		_map_view_event_data_set_gesture_type(ed, MAP_GESTURE_ZOOM);
		_map_view_event_data_set_zoom_factor(ed, new_zoom_factor);
		_map_view_event_data_set_fingers(ed, 2);

		/* Find the current center of the gesture */
		const touch_point cur_center = calc_center(cur_tp_f1,
							   cur_tp_f2);
		_map_view_event_data_set_xy(ed,
					    cur_center._x,
					    cur_center._y);
		_map_view_invoke_event_callback(_gd->_view, ed);
	} while(false);

	/* Invoke user registered event callback for ROTATION */
	do {
		if(rotation_changed)
			break;

		map_view_event_data_h ed =
			_map_view_create_event_data(MAP_VIEW_EVENT_GESTURE);
		if(!ed)
			break;
		_map_view_event_data_set_gesture_type(ed, MAP_GESTURE_ROTATE);
		_map_view_event_data_set_rotation_angle(ed, new_rotation_angle);
		_map_view_event_data_set_fingers(ed, 2);

		/* Find the current center of the gesture */
		const touch_point cur_center = calc_center(cur_tp_f1,
							   cur_tp_f2);
		_map_view_event_data_set_xy(ed,
					    cur_center._x,
					    cur_center._y);
		_map_view_invoke_event_callback(_gd->_view, ed);
	} while(false);


	if(!zoom_changed && !rotation_changed)
		return; // Seems nothing changed, we can return


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
	map_view_geography_to_screen(_gd->_view,
				     _gd->_info._start_state._center,
				     &center_x,
				     &center_y);

	/* c. Apply the delta to the intital center coordinates */
	center_x -= delta_x;
	center_y -= delta_y;


	/* d. Converting screent coordinates of new center to
	* the geographical */
	maps_coordinates_h new_center = NULL;
	map_view_screen_to_geography(_gd->_view,
				     center_x,
				     center_y,
				     &new_center);


	/* e. Enque the command to move the center */
	q()->push(new session::command_view_set_center(get_maps(),
							       _gd->_view,
							       new_center));

	/* f. Enqueue the detected zomm command */
	if(zoom_changed)
		q()->push(construct_gesture_command(MAP_GESTURE_ZOOM,
						    new_center,
						    new_zoom_factor,
						    new_rotation_angle,
						    .0));

	/* g. Enqueue the detected rotation command */
	if(rotation_changed)
		q()->push(construct_gesture_command(MAP_GESTURE_ROTATE,
						    new_center,
						    new_zoom_factor,
						    new_rotation_angle,
						    .0));

	maps_coordinates_destroy(new_center);
}

void view::gesture_processor::on_pinch()
{
}
