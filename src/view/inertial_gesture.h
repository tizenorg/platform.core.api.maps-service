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

#ifndef __MAPS_VIEW_INERTIAL_GESTURE_H__
#define __MAPS_VIEW_INERTIAL_GESTURE_H__


#include "gesture_detector.h"

//LCOV_EXCL_START
namespace view
{
	class inertial_gesture : public gesture_detector {
	protected:
		gesture_detector *_d;
		touch_point _down[MAX_FINGERS];
		touch_point _last[MAX_FINGERS];
		touch_point _prev[MAX_FINGERS];
		double _cur_x[MAX_FINGERS];
		double _cur_y[MAX_FINGERS];
		double _derivative_x[MAX_FINGERS];
		double _derivative_y[MAX_FINGERS];
		bool transiting;
		bool transiting_part[MAX_FINGERS];
		unsigned transiting_start[MAX_FINGERS];
		double _dt[MAX_FINGERS];
	private:
		static const double __ACCURACY = 1;
	public:
		inertial_gesture(maps_view_h view);
		~inertial_gesture();
	private:
		inertial_gesture(const inertial_gesture &src);
		inertial_gesture &operator=(const inertial_gesture &src);
	public:
		virtual void tap(int finger_no, const touch_point &tp);
		virtual void move(int finger_no, const touch_point &tp);
		virtual void up(int finger_no, const touch_point &tp);

		bool is_transiting() const { return transiting; }
		void set_transiting(bool b) { transiting = b; }
		bool next_transition_step();
	public:
		//static void on_idle(void *data);
	protected:
		double get_next_point(const double &start,
				      const double &derivative,
				      const double &dt);
		double get_next_derivative(const double &derivative,
					   const double &dt);
		static unsigned int get_cur_time();
		unsigned int get_transition_time(int finger_no) const;
		void reset();
	};
};
//LCOV_EXCL_STOP

#endif				/* __MAPS_VIEW_INERTIAL_GESTURE_H__ */
