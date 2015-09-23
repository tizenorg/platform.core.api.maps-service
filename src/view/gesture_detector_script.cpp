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


#include "gesture_detector_script.h"
#include <glib.h>


view::gesture_detector_script::gesture_detector_script(map_view_h v)
	: gesture_detector(v)
{
}

view::gesture_detector_script::~gesture_detector_script()
{
}

void view::gesture_detector_script::tap(int finger_no, const touch_point &tp)
{
	if(_info._fingers_pressed == 0) {

		/* This is the begining position of each gesture,
		 *  so we'd better to store the initial state of
		 *  the Map View */
		_info._start_view_state.capture(_view);

		/* Processing of single finger gesture:
		 * - long press
		 */

		_info._fingers_pressed = 1;
		_info._finger_down[0] = tp;
		_info._is_finger_pressed[0] = true;

		/* Check if it is a long press gesure */
		test_long_press_gesture();

	} else { /* TODO: els if(_info._fingers_pressed == 1) */
		/* Processing of multi-finger gesture:
		 * - none
		 */

		_info._fingers_pressed ++;
		_info._finger_down[finger_no] = tp;
		_info._is_finger_pressed[finger_no] = true;
	}
}

void view::gesture_detector_script::up(int finger_no, const touch_point &tp)
{
	if(_info._fingers_pressed == 1) {
		/* Processing of single finger gesture:
		 * - tap
		 * - double tap
		 */

		_info._finger_up[0] = tp;

		/* Check if it is a tap or double tap gesture */
		if(!test_panning_gesture_finished())
			if(!test_two_finger_tap_gesture())
				if(!test_double_tap_gesture())
					test_tap_gesture();

		/* This is the end of each gesture */
		reset();

	} else { /* TODO: els if(_info._fingers_pressed == 2) */
		/* Processing of multi-finger gesture:
		 * - 2-finger tap
		 */

		_info._finger_move[finger_no] = tp;
		_info._finger_up[finger_no] = tp;

		/* Check if it is a two finger tap gesture */
		test_two_finger_tap_gesture();


		/* Remove the info about lifted finger */
		_info._fingers_pressed --;
		_info._finger_down[finger_no].reset();
		_info._is_finger_pressed[finger_no] = false;

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

void view::gesture_detector_script::move(int finger_no, const touch_point &tp)
{
	if(_info._fingers_pressed == 1) {
		/* Processing of single finger gesture:
		 * - pan
		 * - flick
		 */

		_info._prev_finger_down[0] = _info._finger_move[0];
		_info._finger_move[0] = tp;

		/* Check if it is a pan or flick gesture */
		if(!test_flick_gesture())
			test_pan_gesture();


	} else {
		/* Processing of multi-finger gesture:
		 * - zoom
		 * - rotate
		 * - pinch
		 */

		_info._prev_finger_down[finger_no] =
			_info._finger_move[finger_no];
		_info._finger_move[finger_no] = tp;

		/* Check if it is a zoom or rotate or pinch gesture */
		#ifndef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
		if(!test_pinch_gesture())
		#endif
			test_zoom_rotate_gesture();
	}

}


void view::gesture_detector_script::on_long_press_timer()
{
	MAPS_LOGI("GESTURE LONG PRESS PRE-DETECTED");

	/* TODO: think about concurent access to following member variables */

	if(_info._fingers_pressed != 1)
		return;

	/*if(_info._cur_gesture != MAP_GESTURE_LONG_PRESS)*/
		/*return ECORE_CALLBACK_CANCEL;*/ /* Another gesture happend */

	/* Check if the gesture event is short enough */
	if(finger_dragged_enough(0))
		return;

	_info._cur_gesture = MAP_GESTURE_LONG_PRESS;
	MAPS_LOGI("GESTURE LONG PRESS DETECTED");
	_gp.on_long_press();
}

bool view::gesture_detector_script::test_long_press_gesture()
{
	/* Check if the gesture is available */
	if (!is_gesture_available(MAP_GESTURE_LONG_PRESS))
		return false;

	/* Note, We expect, that the gesture is done with a single finger */
	if(_info._fingers_pressed != 1)
		return false;

	/* Start long press timer */
	/*_info._cur_gesture = MAP_GESTURE_LONG_PRESS;*/
	start_long_press_timer();

	return false;
}

bool view::gesture_detector_script::test_double_tap_gesture()
{
	/* Check if the gesture is available */
	if (!is_gesture_available(MAP_GESTURE_DOUBLE_TAP))
		return false;

	/* If a single finger taped twice on the Map View
	 *  and it happed whithin a short time frame,
	 *  than it is likely a double tap gesture;
	 *  The time frame is calculated as the current time - previous click
	 *  time */

	/* Note, We expect, that the gesture is done with a single finger */
	if(_info._fingers_pressed != 1)
		return false;


	const touch_point prev_up = _info_history._finger_up[0];
	if(prev_up._timestamp == 0)
		return false; /* It is very first up of the Map View ever */

	const touch_point cur_up = _info._finger_up[0];
	const unsigned int time = cur_up._timestamp - prev_up._timestamp;
	if((time > 0) && (time < __CLICK_DURATION)) {
		_info._cur_gesture = MAP_GESTURE_DOUBLE_TAP;
		MAPS_LOGI("GESTURE DOUBLE TAP DETECTED");
		_gp.on_double_tap();
		return true;
	}
	return false;
}

bool view::gesture_detector_script::test_panning_gesture_finished()
{
#ifndef _MOVE_CENTER_COMMAND_DEFINED_
	return false;
#endif /* _MOVE_CENTER_COMMAND_DEFINED_ */
	if(_info._cur_gesture == MAP_GESTURE_SCROLL) {
		/* Finalize the panning gesture */
		MAPS_LOGI("GESTURE PAN FINISHED");
		_gp.on_panning_finished(0);
		return true;
	}

	return false;
}

bool view::gesture_detector_script::test_tap_gesture()
{
	/* Check if the gesture is available */
	if (!is_gesture_available(MAP_GESTURE_TAP))
		return false;

	/* If a single finger touched a map view,
	 * and lifted before long press gesture is detected,
	 * than it is a tap gesture */

	/* Note, We expect, that the gesture is done with a single finger */
	if(_info._fingers_pressed != 1)
		return false;

	/* Check if the gesture event is short enough */
	touch_point p1 = _info._finger_down[0];
	touch_point p2 = _info._finger_up[0];
	unsigned int time = p2._timestamp - p1._timestamp;
	if((time > 0) && (time < __CLICK_DURATION)) {
		/* Check if it is a click, but not the movement */
		if(get_trajectory_effective_length(p1, p2) < __CLICK_AREA) {
			_info._cur_gesture = MAP_GESTURE_TAP;
			MAPS_LOGI("GESTURE TAP DETECTED");
			_gp.on_tap();
			return true;
		}
	}
	return false;
}

bool view::gesture_detector_script::test_two_finger_tap_gesture()
{
	/* Check if the gesture is available */
	if (!is_gesture_available(MAP_GESTURE_2_FINGER_TAP))
		return false;

	/* If Two fingers tapped a map view, than it is detected as
	 *  corresponding "two-finger-tap gesture */


	if((_info._fingers_pressed != 2) && (_info._fingers_pressed != 1))
		return false;

	if(_info._finger_down[1].empty())
		return false; /* The second finger was not pressed */

	const touch_point f1_start = _info._finger_down[0];
	const touch_point f1_finish = _info._finger_up[0];
	const touch_point f2_start = _info._finger_down[1];
	const touch_point f2_finish = _info._finger_up[1];

	switch(_info._fingers_pressed) {
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

	_info._cur_gesture = MAP_GESTURE_2_FINGER_TAP;
	MAPS_LOGI("GESTURE TWO FINGER TAP DETECTED");
	_gp.on_two_finger_tap();
	return true;
}

bool view::gesture_detector_script::test_flick_gesture()
{
	/* Check if the gesture is available */
	if (!is_gesture_available(MAP_GESTURE_FLICK))
		return false;

	/* If a finger is moved rapidely left-to-right or right-to-left,
	 *  than it is considered as flicj gesture */

	/* TODO: */
	/*_info._cur_gesture = MAP_GESTURE_FLICK;*/
	/*MAPS_LOGI("GESTURE FLICK DETECTED");*/
	return false;
}

bool view::gesture_detector_script::test_pan_gesture()
{
	/* Check if the gesture is available */
	if (!is_gesture_available(MAP_GESTURE_SCROLL))
		return false;

	/* If a single finger is moved on the map view, than it is recognized
	 *  as a pan gesture */

	/* Note, We expect, that the gesture is done with a single finger */
	if(_info._fingers_pressed != 1)
		return false;

	/* Check If the effective trajectory is more than the limit */
	if (_info._cur_gesture != MAP_GESTURE_SCROLL)
		if(!finger_dragged_enough(0))
			return false;

	/* No special checks are need here */
	_info._cur_gesture = MAP_GESTURE_SCROLL;
	MAPS_LOGI("GESTURE PAN DETECTED");
	_gp.on_pan(0);
	return true;
}

bool view::gesture_detector_script::test_zoom_rotate_gesture()
{
#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	if (!is_gesture_available(MAP_GESTURE_ZOOM))
#else
	/* Check if the gesture is available */
	if ((!is_gesture_available(MAP_GESTURE_ZOOM))
	    && (!is_gesture_available(MAP_GESTURE_ROTATE)))
#endif
		return false;

	/* It two fingetrs are moving in arbitrary trajectories,
	 *  than it is a zoom and/or rotate gesture */

	/* Note, We expect, that the gesture is done with a pair of fingers */
	if(_info._fingers_pressed != 2)
		return false;

	if(_info._cur_gesture != MAP_GESTURE_ZOOM)
		if(!finger_dragged_enough(0) && finger_dragged_enough(1))
			return false;

	/* No special checks are need here */
	_info._cur_gesture = MAP_GESTURE_ZOOM;
	MAPS_LOGI("GESTURE ZOOM & ROTATE DETECTED");
	_gp.on_zoom_rotate();
	return true;
}

#ifndef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
bool view::gesture_detector_script::test_pinch_gesture()
{
	/* Check if the gesture is available */
	if (!is_gesture_available(MAP_GESTURE_PINCH))
		return false;

	/* If two or more fingers moved rapidly toward each other,
	 *  than it is recognized as pinch gesture */

	/*_info._cur_gesture = MAP_GESTURE_PINCH;*/
	/*MAPS_LOGI("GESTURE PINCH DETECTED");*/
	return false;
}
#endif
