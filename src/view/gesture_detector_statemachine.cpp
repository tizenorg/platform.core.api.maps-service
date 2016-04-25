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


#include "gesture_detector_statemachine.h"
#include <glib.h>


view::gesture_detector_statemachine::gesture_detector_statemachine(maps_view_h v)
	: gesture_detector(v)
	  , _current_state(STATE_NONE)
{
	for(int i = 0; i < MAX_FINGERS; i ++)
		is_panning[i] = false;
}

view::gesture_detector_statemachine::~gesture_detector_statemachine()
{
}

void view::gesture_detector_statemachine::tap(int finger_no,
					      const touch_point &tp)
{

	switch(finger_no) {

	case 0:	/* Single finger pressed */
		_info._finger_down[0] = tp;
		_info._finger_move[0] = tp;
		_info._prev_finger_down[0] = tp;
		_info._is_finger_pressed[0] = true;

		_info._fingers_pressed = _info.calc_finger_pressed();

		if(_info._fingers_pressed > 2)
			log("ERROR! Too many fingers pressed!!!", FG_RED);

		/* Run state machine */
		state_machine_on_event(FINGER_DOWN);
		break;

	case 1:	/* Second finger pressed */
		_info._finger_down[1] = tp;
		_info._is_finger_pressed[1] = true;
		_info._finger_move[1] = tp;
		_info._prev_finger_down[1] = tp;

		_info._fingers_pressed = _info.calc_finger_pressed();

		if(_info._fingers_pressed > 2)
			log("ERROR! Too many fingers pressed!!!", FG_RED);

		/* Run state machine */
		state_machine_on_event(FINGER2_DOWN);
		break;

	default:
		break;
	}

	stop_tap_timer();

	MAPS_LOGI("%c[%d;%d;%dm"
		  "Fingers pressed [after press]: %d"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, FG_LITE_WHITE,
		  _info._fingers_pressed,
		  0x1B, 0, 0, 0);
}

void view::gesture_detector_statemachine::up(int finger_no,
					     const touch_point &tp)
{
	switch(finger_no) {

	case 0: /* Up the single finger */
		_info._finger_up[0] = tp;

		/* Run state machine */
		state_machine_on_event(FINGER_UP);

		/* Remove the info about lifted finger */
		_info._is_finger_pressed[0] = false;

		_info._fingers_pressed = _info.calc_finger_pressed();

		if(_info._fingers_pressed < 0)
			log("ERROR! Too few fingers pressed!!!", FG_RED);

		/* This is the end of the gesture */
		if(_info._fingers_pressed <= 0)
			reset();
		break;

	case 1: /* Up the second finger */

		_info._finger_move[1] = tp;
		_info._finger_up[1] = tp;

		/* Run state machine */
		state_machine_on_event(FINGER2_UP);

		/* Remove the info about lifted finger */
		_info._is_finger_pressed[1] = false;

		_info._fingers_pressed = _info.calc_finger_pressed();

		if(_info._fingers_pressed < 0)
			log("ERROR! Too few fingers pressed!!!", FG_RED);

		/* This is the end of the gesture */
		if(_info._fingers_pressed <= 0)
			reset();
		break;

	default:
		break;
	}

	stop_long_press_timer();

	MAPS_LOGI("%c[%d;%d;%dm"
		  "Fingers pressed [after unpress]: %d"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, FG_LITE_WHITE,
		  _info._fingers_pressed,
		  0x1B, 0, 0, 0);
}

void view::gesture_detector_statemachine::move(int finger_no,
					       const touch_point &tp)
{
	/* There can not be a long press any more */
	stop_long_press_timer();

	switch(finger_no) {

	case 0: /* Moving the first (single) finger */
		_info._prev_finger_down[0] = _info._finger_move[0];
		_info._finger_move[0] = tp;

		/* Run state machine */
		state_machine_on_event(FINGER_MOVE);
		break;

	case 1: /* Moving the second finger */
		_info._prev_finger_down[1] = _info._finger_move[1];
		_info._finger_move[1] = tp;

		/* Run state machine */
		state_machine_on_event(FINGER2_MOVE);
		break;
	default:
		break;
	}
}

void view::gesture_detector_statemachine::start_panning(int finger_no)
{
	if(!is_panning[finger_no]) {
		/*_info._prev_finger_down[finger_no] =
			_info._finger_move[finger_no];*/
		is_panning[finger_no] = true;
	}
}

void view::gesture_detector_statemachine::finish_panning(int finger_no)
{
	if(is_panning[finger_no]) {
		_gp.on_panning_finished(finger_no);

		_info._start_view_state.capture(_view);
		_info._finger_down[finger_no] = _info._finger_move[finger_no];
	}
	is_panning[finger_no] = false;
}

void view::gesture_detector_statemachine::on_tap_timer()
{
	/* TODO: think about concurent access to following member variables */

	/* Run state machine */
	state_machine_on_event(TAP_TIMER);
}

void view::gesture_detector_statemachine::on_long_press_timer()
{
	/* TODO: think about concurent access to following member variables */

	/* Run state machine */
	state_machine_on_event(LONG_PRESS_TIMER);
}

void view::gesture_detector_statemachine::halt_gesture()
{
	_current_state = STATE_NONE;
	g_usleep(5 * 1000);
	reset();
}

void view::gesture_detector_statemachine::state_machine_on_event(view_event_e
								 event)
{
	log_event(event);
	detector_states_e old_state = _current_state;

	switch(_current_state) {

	case STATE_NONE: {

		_info._start_view_state.capture(_view);
		log_map_center(FG_WHITE);

		switch(event) {
		case FINGER_DOWN:
			_current_state = STATE_PRESSED;
			start_long_press_timer();
			break;

		default:
			log_state(event, _current_state);
			break;
		}

		break;
	}

	case STATE_PRESSED: {
		switch(event) {
		case FINGER_MOVE:
			_current_state = STATE_MOVING;
			break;
		case FINGER_UP: {
			if(finger_pressed_enough(0, 0, __CLICK_DURATION)) {
				_current_state = STATE_CLICKED;
				start_tap_timer();
			} else
				_current_state = STATE_NONE;
			break;
		}
		case LONG_PRESS_TIMER:
			_current_state = STATE_LONG_PRESSED;
			detected_long_press();	/* Long Press */
			break;
		case FINGER2_DOWN:
			_current_state = STATE_2FINGERS_PRESSED;
			break;
		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_CLICKED: {
		switch(event) {
		case FINGER_DOWN: {
			/* Calculate the time between clicks */
			unsigned int last_click_time =
				_info_history._finger_up[0]._timestamp;
			unsigned int cur_time =
				_info._finger_down[0]._timestamp;
			unsigned int duration = cur_time - last_click_time;

			/* Decide if we can have a souble click, or just a
			 *  second click */
			if(duration < __DOUBLE_CLICK_DURATION)
				_current_state = STATE_SECOND_PRESSED;
			else
				_current_state = STATE_PRESSED;

			/* Start timer expecting the long press */
			start_long_press_timer();
			break;
		}
		case TAP_TIMER:
			stop_long_press_timer();
			_current_state = STATE_NONE;
			detected_tap();	/* Tap */
			break;
		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_LONG_PRESSED: {
		switch(event) {
		case FINGER_UP:
			_current_state = STATE_NONE;
			break;
		case FINGER_MOVE:
			_current_state = STATE_MOVING;
			break;
		case FINGER2_DOWN:
			_current_state = STATE_2FINGERS_PRESSED;
			break;
		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_SECOND_PRESSED: {
		switch(event) {
		case FINGER_UP: {

			/* First click position */
			const touch_point p1 = _info_history._finger_down[0];

			/* Second click position */
			const touch_point p2 = _info._finger_down[0];

			MAPS_LOGI("Checking double tap: [%d, %d] -> [%d, %d]",
				  p1._x, p1._y, p2._x, p2._y);

			/*
			 * Check if tapped in the same point
			 * Note: accuracy is decreased for the sake of user's
			 * convenience
			 */

			if(get_trajectory_effective_length(p1, p2)
			   <= (4 * __CLICK_AREA)) {

				/* Switching to Double Tap state */
				_current_state = STATE_SECOND_CLICKED;

				detected_double_tap();	/* Double Tap */

				/*
				 * No need to expect other events here.
				 * Immediatelly switching to the initial State
				 */
				_current_state = STATE_NONE;

			} else {
				/* Seems like it is a simple click */
				_current_state = STATE_CLICKED;
				stop_long_press_timer();
				start_tap_timer();
			}

			break;
		}
		case TAP_TIMER:
			_current_state = STATE_NONE;
			detected_tap();	/* Tap */
			break;
		case LONG_PRESS_TIMER:
			_current_state = STATE_SECOND_LONG_PRESSED;
			detected_second_long_press();	/* Second Long Press */
			break;
		case FINGER_MOVE: {
			_current_state = STATE_MOVING;
			detected_pan();	/* Tap */
			break;
		}
		case FINGER2_DOWN:
			_current_state = STATE_2FINGERS_PRESSED;
			break;
		default:
			log_state(event, _current_state);
			break;
		}

		break;
	}

	case STATE_SECOND_CLICKED: {
		/* Empty */
		_current_state = STATE_NONE;
		break;
	}

	case STATE_SECOND_LONG_PRESSED: {
		switch(event) {
		case FINGER_UP:
			_current_state = STATE_NONE;
			break;
		case FINGER_MOVE: {
			_current_state = STATE_MOVING;
			detected_pan();
			break;
		}
		case FINGER_DOWN:
			_current_state = STATE_2FINGERS_PRESSED;
			break;
		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_MOVING: {

		if(event != FINGER_MOVE)
			finish_panning(0);

		switch(event) {
		case FINGER_MOVE:
			_current_state = STATE_MOVING;
			detected_pan();		/* Pan */
			break;
		case FINGER_UP:

			/* When two fingers untouched simultaneously,
			 * Ecore sends finger1_up and then finger2_up
			 * sequentially.
			 * The finger1_up switches the detector from
			 * STATE_2FINGERS_MOVING to STATE_MOVING.
			 * Now we are receiving finger2_up and should switch to
			 * the initial state.
			 */

			_current_state = STATE_NONE;
			break;
		case FINGER2_DOWN:
			_current_state = STATE_2FINGERS_PRESSED;
			break;
		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_FINGER2_MOVING: {

		if(event != FINGER2_MOVE)
			finish_panning(1);

		switch(event) {
		case FINGER2_MOVE:
			_current_state = STATE_FINGER2_MOVING;
			detected_finger2_pan();		/* Pan (finger2) */
			break;
		case FINGER2_UP:
			_current_state = STATE_NONE;
			break;
		case FINGER_DOWN:
			_current_state = STATE_2FINGERS_PRESSED;
			break;

		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_2FINGERS_PRESSED: {
		switch(event) {
		case FINGER_UP:
			_current_state = STATE_FINGER2_PRESSED;
			break;
		case FINGER2_UP:
			_current_state = STATE_FINGER1_PRESSED;
			break;
		case FINGER_MOVE:
		case FINGER2_MOVE:
			_current_state = STATE_2FINGERS_MOVING;
			detected_zoom_rotate();		/* Zoom, Rotate etc */
			break;
		case LONG_PRESS_TIMER:
			/* Just ignoring this event */
			_current_state = STATE_2FINGERS_PRESSED;
			break;
		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_FINGER2_PRESSED: {
		switch(event) {
		case FINGER2_UP:

			if(finger_pressed_enough(0, 0, __CLICK_DURATION)
			   &&  finger_pressed_enough(1, 0, __CLICK_DURATION)){
				_current_state = STATE_2FINGERS_CLICKED;
				detected_2finger_tap();	/* 2Finger Tap */
			}

			/* Nothing to do here: switching to initial state */
			_current_state = STATE_NONE;
			break;
		case FINGER2_MOVE:
			_current_state = STATE_FINGER2_MOVING;
			detected_finger2_pan();	/* Pan (by second finger) */
			break;
		case FINGER_DOWN:
			_current_state = STATE_2FINGERS_PRESSED;
			break;
		case LONG_PRESS_TIMER:
			/* Just ignoring this event */
			_current_state = STATE_FINGER2_PRESSED;
			break;
		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_FINGER1_PRESSED: {
		switch(event) {
		case FINGER_UP:

			if(finger_pressed_enough(0, 0, __CLICK_DURATION)
			   &&  finger_pressed_enough(1, 0, __CLICK_DURATION)){
				_current_state = STATE_2FINGERS_CLICKED;
				detected_2finger_tap();	/* 2Finger Tap */
			}

			/* No need to expect other events here.
			*  Immediatelly switching to the initial State */
			_current_state = STATE_NONE;
			break;
		case FINGER2_DOWN:
			_current_state = STATE_2FINGERS_PRESSED;
			break;
		case FINGER_MOVE:
			_current_state = STATE_MOVING;
			detected_pan();	/* Pan */
			break;

		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_2FINGERS_CLICKED: {
		/* Empty */
		break;
	}

	case STATE_2FINGERS_MOVING: {

		if((event != FINGER_MOVE) && (event != FINGER2_MOVE))
			/* Finish zooming */
			_info._start_view_state.capture(_view);

		switch(event) {
		case FINGER_MOVE:
			_current_state = STATE_2FINGERS_MOVING;
			detected_zoom_rotate();		/* Zoom, Rotate etc */
			break;
		case FINGER2_MOVE:
			_current_state = STATE_2FINGERS_MOVING;
			detected_zoom_rotate();		/* Zoom, Rotate etc */
			break;
		case FINGER_UP:
			_current_state = STATE_FINGER2_MOVING;
			break;
		case FINGER2_UP:
			_current_state = STATE_MOVING;
			break;
		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	case STATE_MOVING_AFTER_SECOND_PRESS: {
		switch(event) {
		case FINGER_UP:
			_current_state = STATE_NONE;
			break;
		case FINGER2_DOWN:
			_current_state = STATE_2FINGERS_PRESSED;
			break;
		case FINGER_MOVE:
			_current_state = STATE_MOVING_AFTER_SECOND_PRESS;
			break;
		default:
			log_state(event, _current_state);
			break;
		}
		break;
	}

	default:
		log_state(event, _current_state);
		_current_state = STATE_NONE;
		break;
	}

	string s_old = get_state_str(old_state);
	string s = get_state_str(_current_state);
	MAPS_LOGI("%c[%d;%d;%dm"
		  "switched from %s[%d] to %s[%d]"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, FG_LITE_CYAN,
		  s_old.c_str(), old_state,
		  s.c_str(), _current_state,
		  0x1B, 0, 0, 0);

	/* DEBUG: trace current central coordinates */
	log_map_center();
}

void view::gesture_detector_statemachine::detected_tap()	/* Tap */
{
	if (!is_gesture_available(MAPS_VIEW_GESTURE_TAP))
		return;

	log("GESTURE TAP DETECTED", FG_GREEN);
	_gp.on_tap();
}

void view::gesture_detector_statemachine::detected_long_press()	/* Long Press */
{
	if (!is_gesture_available(MAPS_VIEW_GESTURE_LONG_PRESS))
		return;

	log("GESTURE LONG PRESS DETECTED", FG_GREEN);
	_gp.on_long_press();
}

void view::gesture_detector_statemachine::detected_double_tap()	/* Double Tap */
{
	if (!is_gesture_available(MAPS_VIEW_GESTURE_DOUBLE_TAP))
		return;

	log("GESTURE DOUBLE TAP DETECTED", FG_GREEN);
	_gp.on_double_tap();
}

void view::gesture_detector_statemachine::detected_second_long_press()	/* Second Long Press */
{
	if (!is_gesture_available(MAPS_VIEW_GESTURE_LONG_PRESS))
		return;

	log("GESTURE SECOND LONG PRESS DETECTED", FG_GREEN);
	_gp.on_long_press();
}

void view::gesture_detector_statemachine::detected_pan()		/* Pan */
{
	if (!is_gesture_available(MAPS_VIEW_GESTURE_SCROLL))
		return;

	log("GESTURE PAN DETECTED", FG_GREEN);
	start_panning(0);
	_gp.on_pan(0);
}

void view::gesture_detector_statemachine::detected_finger2_pan()	/* Pan (second finger)*/
{
	if (!is_gesture_available(MAPS_VIEW_GESTURE_SCROLL))
		return;

	log("GESTURE FINGER2 PAN DETECTED", FG_GREEN);
	start_panning(1);
	_gp.on_pan(1);
}

/* Zoom, Rotate etc */
void view::gesture_detector_statemachine::detected_zoom_rotate()
{
	if (!is_gesture_available(MAPS_VIEW_GESTURE_ZOOM)
	    && !is_gesture_available(MAPS_VIEW_GESTURE_ROTATE))
		return;

	log("GESTURE ZOOM ROTATE DETECTED", FG_GREEN);
	_gp.on_zoom_rotate();
}

void view::gesture_detector_statemachine::detected_2finger_tap()	/* 2Finger Tap */
{
	if (!is_gesture_available(MAPS_VIEW_GESTURE_2_FINGER_TAP))
		return;

	log("GESTURE 2 FINGER TAP DETECTED", FG_GREEN);
	_gp.on_two_finger_tap();
}

string view::gesture_detector_statemachine::get_state_str(detector_states_e
							  state)
{
	string s;
	switch(state) {
	case STATE_NONE:
		s = "STATE_NONE";
		break;
	case STATE_PRESSED:
		s = "STATE_PRESSED";
		break;
	case STATE_CLICKED:
		s = "STATE_CLICKED";
		break;
	case STATE_LONG_PRESSED:
		s = "STATE_LONG_PRESSED";
		break;
	case STATE_SECOND_PRESSED:
		s = "STATE_SECOND_PRESSED";
		break;
	case STATE_SECOND_CLICKED:
		s = "STATE_SECOND_CLICKED";
		break;
	case STATE_SECOND_LONG_PRESSED:
		s = "STATE_SECOND_LONG_PRESSED";
		break;
	case STATE_MOVING:
		s = "STATE_MOVING";
		break;
	case STATE_FINGER2_MOVING:
		s = "STATE_FINGER2_MOVING";
		break;
	case STATE_FINGER2_PRESSED:
		s = "STATE_FINGER2_PRESSED";
		break;
	case STATE_FINGER1_PRESSED:
		s = "STATE_FINGER1_PRESSED";
		break;
	case STATE_2FINGERS_PRESSED:
		s = "STATE_2FINGERS_PRESSED";
		break;
	case STATE_2FINGERS_CLICKED:
		s = "STATE_2FINGERS_CLICKED";
		break;
	case STATE_2FINGERS_MOVING:
		s = "STATE_2FINGERS_MOVING";
		break;
	case STATE_MOVING_AFTER_SECOND_PRESS:
		s = "STATE_MOVING_AFTER_SECOND_PRESS";
		break;
	default:
		s = "UNKNOWN";
		break;
	}
	return s;
}

string view::gesture_detector_statemachine::get_event_str(view_event_e event)
{
	string e;
	switch(event) {
	case FINGER_DOWN:
		e = "FINGER_DOWN";
		break;
	case FINGER_MOVE:
		e = "FINGER_MOVE";
		break;
	case FINGER_UP:
		e = "FINGER_UP";
		break;
	case FINGER2_DOWN:
		e = "FINGER2_DOWN";
		break;
	case FINGER2_MOVE:
		e = "FINGER2_MOVE";
		break;
	case FINGER2_UP:
		e = "FINGER2_UP";
		break;
	case LONG_PRESS_TIMER:
		e = "LONG_PRESS_TIMER";
		break;
	case TAP_TIMER:
		e = "TAP_TIMER";
		break;
	default:
		e = "UNKNOWN";
		break;
	}
	return e;
}

void view::gesture_detector_statemachine::log_event(view_event_e event)
{
	string e = get_event_str(event);
	log(e.c_str(), FG_BLUE);
}

void view::gesture_detector_statemachine::log_state(view_event_e event,
				       detector_states_e state)
{

	string e = get_event_str(event);
	string s = get_state_str(state);

	MAPS_LOGE("%c[%d;%d;%dm"
		  "DEFAULT: state: %s[%d],\tevent: %s[%d]"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, FG_RED,
		  s.c_str(), state, e.c_str(), event,
		  0x1B, 0, 0, 0);
}
