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

#ifndef __MAPS_VIEW_GESTURE_DETECTOR_H__
#define __MAPS_VIEW_GESTURE_DETECTOR_H__


#include "map_view.h"
#include "runtime_data.h"
#include "gesture_processor.h"


namespace view
{
	class gesture_detector {
	public:
		map_view_h _view;

		/* The gesture processing unit */
		gesture_processor _gp;

		/* Long press gesture timer */
		Ecore_Timer *_long_press_timer;

		/* The current gesture info */
		runtime_touch_info _info;

		/* The history includes the touch info from previous gesture */
		runtime_touch_info _info_history;

	protected:
		static const int __MIL_SEC = 1;
		static const int __SEC = 1000 * __MIL_SEC;

		/* Half a second is a popular duration of double click in UI */
		static const unsigned int __CLICK_DURATION = 500 * __MIL_SEC;
		static const unsigned int __DOUBLE_CLICK_DURATION =
			2 * __CLICK_DURATION;;

		/* Half a second is a popular duration of double click in UI
		* however, sometimes it is 1 second */
		static const double __LONG_PRESS_DURATION = 0.5;

	public:

		/* May be same as hit test accuracy */
		/*
		 * TODO: It is needed to calculate this value accordingly to the
		 *  device pixel dencity
		 */
		static const int __CLICK_AREA = 50;

	public:
		gesture_detector(map_view_h v);
		virtual ~gesture_detector();
	public:
		virtual void tap(int finger_no, const touch_point &tp);
		virtual void move(int finger_no, const touch_point &tp);
		virtual void up(int finger_no, const touch_point &tp);
		virtual void on_long_press_timer();
		virtual void halt_gesture();
	protected:
		virtual void reset();
	public:
		static Eina_Bool __on_long_press_timer(void *data);
		void start_long_press_timer();
		void stop_long_press_timer();
	public:
	protected:
		bool is_gesture_available(map_gesture_e gesture);
		bool finger_dragged_enough(int finger_no);
		bool finger_pressed_enough(int finger_no,
					   unsigned int duration_min,
					   unsigned int duration_max);
	public:
		static int get_trajectory_effective_length(const touch_point
							   &p1,
							   const touch_point
							   &p2);
	public:
		enum log_colors {
			FG_BLACK	= 30,
			FG_RED		= 31,
			FG_GREEN	= 32,
			FG_YELLOW	= 33,
			FG_BLUE		= 34,
			FG_MAGENTA	= 35,
			FG_CYAN		= 36,
			FG_WHITE	= 37,
			FG_LITE_BLACK	= 90,
			FG_LITE_RED	= 91,
			FG_LITE_GREEN	= 92,
			FG_LITE_YELLOW	= 93,
			FG_LITE_BLUE	= 94,
			FG_LITE_MAGENTA	= 95,
			FG_LITE_CYAN	= 96,
			FG_LITE_WHITE	= 97

		};
		static void log(const char *str, log_colors color = FG_WHITE);
		void log_map_center(int color = FG_LITE_BLUE);
	};
};

#endif				/* __MAPS_VIEW_GESTURE_DETECTOR_H__ */
