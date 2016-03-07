/* Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __MAPS_VIEW_GESTURE_DETECTOR_STATEMACHINE_H__
#define __MAPS_VIEW_GESTURE_DETECTOR_STATEMACHINE_H__


#include "maps_view.h"
#include "runtime_data.h"
#include "gesture_detector.h"


namespace view
{
	enum view_event_e {
		FINGER_DOWN,
		FINGER_MOVE,
		FINGER_UP,
		FINGER2_DOWN,
		FINGER2_MOVE,
		FINGER2_UP,
		TAP_TIMER,
		LONG_PRESS_TIMER
	};

	enum detector_states_e {
		/* Initial state */
		STATE_NONE,

		/* After finger down */
		STATE_PRESSED,

		/* After finger down, no significant move, finger up */
		STATE_CLICKED,

		/* After finger down and hold */
		STATE_LONG_PRESSED,

		/* Finger down and recently was click */
		STATE_SECOND_PRESSED,

		/* Second click */
		STATE_SECOND_CLICKED,

		/* Click and then finger down and hold for awhile */
		STATE_SECOND_LONG_PRESSED,

		/* Moving single finger */
		STATE_MOVING,

		/* Moving the second finger after the first one was untouched */
		STATE_FINGER2_MOVING,

		/* Both Finger 1 and 2 are pressed */
		STATE_FINGER2_PRESSED,

		/* Finger 1 or finger 2 was up */
		STATE_FINGER1_PRESSED,

		/* Both fingers are pressed now */
		STATE_2FINGERS_PRESSED,

		/* Clicked with two fingers */
		STATE_2FINGERS_CLICKED,

		/* Moving Finger1 or 2 when both pressed */
		STATE_2FINGERS_MOVING,

		/* Moving after first click */
		STATE_MOVING_AFTER_SECOND_PRESS
	};


	class gesture_detector_statemachine : public gesture_detector {
	public:
		/* Current state of detector state machine */
		detector_states_e _current_state;

		bool is_panning[MAX_FINGERS];

	public:
		gesture_detector_statemachine(maps_view_h v);
		virtual ~gesture_detector_statemachine();
	public:
		virtual void tap(int finger_no, const touch_point &tp);
		virtual void move(int finger_no, const touch_point &tp);
		virtual void up(int finger_no, const touch_point &tp);
		virtual void on_tap_timer();
		virtual void on_long_press_timer();
		virtual void halt_gesture();
	public:
		void state_machine_on_event(view_event_e event);
	private:
		void detected_tap();	/* Tap */
		void detected_long_press();	/* Long Press */
		void detected_double_tap();	/* Double Tap */
		void detected_second_long_press();	/* Second Long Press */
		void detected_single_finger_zoom();	/* Single Finger Zoom */
		void detected_pan();		/* Pan */
		void detected_finger2_pan();	/* Pan (by second finger) */
		void detected_zoom_rotate();		/* Zoom, Rotate etc */
		void detected_2finger_tap();	/* 2Finger Tap */
	private:
		void start_panning(int finger_no);
		void finish_panning(int finger_no);

	private:
		void log_state(view_event_e event, detector_states_e state);
		void log_event(view_event_e event);
		string get_state_str(detector_states_e state);
		string get_event_str(view_event_e event);
	};
};

#endif				/* __MAPS_VIEW_GESTURE_DETECTOR_STATEMACHINE_H__ */
