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

#ifndef __MAPS_VIEW_GESTURE_DETECTOR_SCRIPT_H__
#define __MAPS_VIEW_GESTURE_DETECTOR_SCRIPT_H__


#include "map_view.h"
#include "runtime_data.h"
#include "gesture_detector.h"


namespace view
{
	/* This class defines the automate, aimed at recognition of user
	 *  gestures on the base of gesture info, sich as a number of fingers
	 *  involved, the initial and current position of the fingers on the
	 *  Map View, the time diration of the gesture etc.
	 *  Note, it is enoug for the automate to process only each finger
	 *  the "tap", "move" and "up" events.
	 *  If the supported gesture is detected, the gesture_detector_script invokes
	 *  appropriate processing by gesture_processor.
	 */
	class gesture_detector_script : public gesture_detector {
	public:
		gesture_detector_script(map_view_h v);
		virtual ~gesture_detector_script();
	public:
		virtual void tap(int finger_no, const touch_point &tp);
		virtual void move(int finger_no, const touch_point &tp);
		virtual void up(int finger_no, const touch_point &tp);
		virtual void on_long_press_timer();
	private:
		bool test_long_press_gesture();
		bool test_double_tap_gesture();
		bool test_tap_gesture();
		bool test_two_finger_tap_gesture();
		bool test_flick_gesture();
		bool test_pan_gesture();
		bool test_panning_gesture_finished();
		bool test_zoom_rotate_gesture();
		#ifndef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
		bool test_pinch_gesture();
		#endif
	};
};

#endif				/* __MAPS_VIEW_GESTURE_DETECTOR_SCRIPT_H__ */
