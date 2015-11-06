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


#include "inertial_gesture.h"
#include "maps_util.h"
#include <glib.h>
#include "gesture_detector_statemachine.h"
#include <unistd.h>
#include <time.h>

extern void _map_view_set_idle_listener(map_view_h view,
					void (*callback)(void *user_data),
					void *user_data);

extern void _map_view_halt_inertial_camera(map_view_h view);

view::inertial_gesture::inertial_gesture(map_view_h view)
	: gesture_detector(view)
	  , _d(NULL)
	  , transiting(false)
{
	reset();

	_d = new gesture_detector_statemachine(view);

	_map_view_set_idle_listener(view, on_idle, this);
}

view::inertial_gesture::~inertial_gesture()
{
	_map_view_set_idle_listener(_view, NULL, NULL);

	if(_d)
		delete _d;
	_d = NULL;
}

view::inertial_gesture::inertial_gesture(const inertial_gesture &src)
	: gesture_detector(NULL)
	  , _d(NULL)
	  , transiting(false)
{
}

view::inertial_gesture &view::inertial_gesture::operator=(const inertial_gesture
							  &src)
{
	return *this;
}

void view::inertial_gesture::tap(int finger_no, const touch_point &tp)
{
	MAPS_LOGI("TRANSITION finger %d tap time: %d",
		  finger_no, tp._timestamp);

	if(transiting) { /* Halt the transition */
		for(int i = 0; i < MAX_FINGERS; i ++) {
			if(!transiting_part[i])
				continue;

			unsigned int timestamp = _last[i]._timestamp
				+ get_transition_time(i);

			const touch_point tp(_cur_x[i], _cur_y[i], timestamp);
			MAPS_LOGI("TRANSITION finger %d up FAKE BRAKE time: %d",
				  i, tp._timestamp);
			_d->up(i, tp);
		}

		_map_view_halt_inertial_camera(_view);
		_d->halt_gesture();
		reset();
	}

	#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	_down[finger_no] = tp;
	#endif
	_last[finger_no] = tp;
	_prev[finger_no] = tp;
	_cur_x[finger_no] = tp._x;
	_cur_y[finger_no] = tp._y;

	if(_d)
		_d->tap(finger_no, tp);
}

void view::inertial_gesture::move(int finger_no, const touch_point &tp)
{
	MAPS_LOGI("TRANSITION finger %d move", finger_no);

	_prev[finger_no] = _last[finger_no];
	_last[finger_no] = tp;
	_cur_x[finger_no] = tp._x;
	_cur_y[finger_no] = tp._y;

	if(_d)
		_d->move(finger_no, tp);
}

void view::inertial_gesture::up(int finger_no, const touch_point &tp)
{
	MAPS_LOGI("TRANSITION finger %d up time: %d",
		  finger_no, tp._timestamp);

	_last[finger_no] = tp;

	const int delta_x = _last[finger_no]._x - _prev[finger_no]._x;
	const int delta_y = _last[finger_no]._y - _prev[finger_no]._y;
	unsigned int dt =
		_last[finger_no]._timestamp - _prev[finger_no]._timestamp;

	#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	int trajectory = get_trajectory_effective_length(_down[finger_no], tp);
	MAPS_LOGD("trajectory=%d", trajectory);
	if (trajectory <= 5) dt = 0;
	#endif

	if(dt == 0) {
		_derivative_x[finger_no] = .0;
		_derivative_y[finger_no] = .0;
	} else {
		_derivative_x[finger_no] = 40. * (delta_x) / dt;
		_derivative_y[finger_no] = 40. * (delta_y) / dt;
	}
	_dt[finger_no] = MIN(1.*dt, 1.);

	/* Continue "move" with negative acceleration */
	transiting_part[finger_no] = true;
	transiting_start[finger_no] = get_cur_time();
	transiting = true;
}

void view::inertial_gesture::next_transition_step()
{
	MAPS_LOGI("TRANSITION get next transition step");
	transiting = false;

	//static const double dt = 1.;
	for(int i = 0; i < MAX_FINGERS; i ++) {
		if(!transiting_part[i])
			continue;

		transiting_part[i] = false;

		if(ABS(_derivative_x[i]) > __ACCURACY) {
			_cur_x[i] = get_next_point(_cur_x[i],
						   _derivative_x[i],
						   _dt[i]);
			_derivative_x[i] = get_next_derivative(_derivative_x[i],
							       _dt[i]);
			transiting_part[i] |=
				ABS(_derivative_x[i]) > __ACCURACY;
		}

		if(ABS(_derivative_y[i]) > __ACCURACY) {
			_cur_y[i] = get_next_point(_cur_y[i],
						   _derivative_y[i],
						   _dt[i]);
			_derivative_y[i] =
				get_next_derivative(_derivative_y[i],
						    _dt[i]);
			transiting_part[i] |=
				ABS(_derivative_y[i]) > __ACCURACY;
		}

		unsigned int timestamp = _last[i]._timestamp
			+ get_transition_time(i);

		const touch_point tp(_cur_x[i], _cur_y[i], timestamp);
		if(transiting_part[i]) {
			MAPS_LOGI("TRANSITION finger %d move FAKE time: %d",
				  i, tp._timestamp);
			_d->move(i, tp);
		} else {
			MAPS_LOGI("TRANSITION finger %d up FAKE time: %d",
				  i, tp._timestamp);
			_d->up(i, tp);
		}

		transiting = true; /* There were changes, so may be one more
				      step in transient is needed */
	}

	if(!transiting)
		reset();
}

double view::inertial_gesture::get_next_point(const double &start,
					      const double &derivative,
					      const double &dt)
{
	/* Simple square parabola */
	return (start + derivative * dt);
}

double view::inertial_gesture::get_next_derivative(const double &derivative,
						   const double &dt)
{
	/* Simple square parabola */
	/*if(derivative > 0)
		return (derivative + __ACCEL * dt);
	else
		return (derivative - __ACCEL * dt);*/

	/* Exponential spped down */
	if(_d->_info._fingers_pressed <= 1)
		return (derivative * .9);
	else
		return (derivative * .5);
}

unsigned int view::inertial_gesture::get_cur_time()
{
	struct timespec ts;
	unsigned int theTick = 0U;
	clock_gettime(CLOCK_REALTIME, &ts);
	theTick  = ts.tv_nsec / 1000000;
	theTick += ts.tv_sec * 1000;
	return theTick;
}

unsigned int view::inertial_gesture::get_transition_time(int finger_no) const
{
	return get_cur_time() - transiting_start[finger_no];
}

void view::inertial_gesture::on_idle(void *data)
{
	inertial_gesture *ig = (inertial_gesture *)data;
	if(ig && ig->transiting) {
		MAPS_LOGI("TRANSITION on idle");
		ig->next_transition_step();
		g_usleep(5*1000);
	}
}

void view::inertial_gesture::reset()
{
	transiting = false;
	for(int i = 0; i < MAX_FINGERS; i ++) {
		transiting_part[i] = false;
		transiting_start[i] = 0;
	}
}
