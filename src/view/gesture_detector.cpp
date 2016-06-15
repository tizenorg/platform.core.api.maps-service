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


#include "gesture_detector.h"
#include <glib.h>
#include <math.h>


extern bool _maps_view_is_gesture_available(maps_view_h view,
					   maps_view_gesture_e gesture);


/*----------------------------------------------------------------------------*/


view::gesture_detector::gesture_detector(maps_view_h v)
	: _view(v)
	, _gp(this)
	, _tap_timer(NULL)
	, _long_press_timer(NULL)
{
}

view::gesture_detector::~gesture_detector()
{
	stop_tap_timer();
	stop_long_press_timer();
}

void view::gesture_detector::halt_gesture()
{
}

void view::gesture_detector::reset()
{
	MAPS_LOGI("%c[%d;%d;%dm"
		  "%s"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 41, FG_GREEN,
		  "RESET RUNTIME INFO",
		  0x1B, 0, 0, 0);

	/* Store the previous gesture data for possible future analysis */
	_info_history = _info;

	/* Save Map View state for complex multi-click gestures */
	const map_state saved_state = _info._start_view_state;

	/* Reset current gesture info */
	_info.reset();

	/* We would use this state in the complex gestures */
	_info._start_view_state = saved_state;

	/* There is definitelly not long enough for "long press" gesture */
	stop_long_press_timer();

	/*MAPS_LOGI("Historic timestamps: tap: %d, move: %d, up: %d",
		  _info_history._finger_down[0]._timestamp,
		  _info_history._finger_move[0]._timestamp,
		  _info_history._finger_up[0]._timestamp);*/
}

void view::gesture_detector::tap(int finger_no, const touch_point &tp)
{
}

void view::gesture_detector::up(int finger_no, const touch_point &tp)
{
}

void view::gesture_detector::move(int finger_no, const touch_point &tp)
{
}

void view::gesture_detector::on_tap_timer()
{
}

void view::gesture_detector::start_tap_timer()
{
	log("START TAP TIMER", FG_MAGENTA);
	if(_tap_timer)
		stop_tap_timer();
	_tap_timer = ecore_timer_add(__TAP_DURATION,
					__on_tap_timer,
					this);
}

void view::gesture_detector::stop_tap_timer()
{
	log("STOP TAP TIMER", FG_MAGENTA);
	if(_tap_timer)
		ecore_timer_del(_tap_timer);
	_tap_timer = NULL;
}

Eina_Bool view::gesture_detector::__on_tap_timer(void *data)
{
	gesture_detector *d = (gesture_detector *)data;
	if (!d)
		return ECORE_CALLBACK_CANCEL;

	d->log("------- TAP TIMER EVENT -------", FG_MAGENTA);
	d->_tap_timer = NULL;
	d->on_tap_timer();

	return ECORE_CALLBACK_CANCEL;
}

void view::gesture_detector::on_long_press_timer()
{
}

void view::gesture_detector::start_long_press_timer()
{
	log("START LONG PRESS TIMER", FG_MAGENTA);
	if(_long_press_timer)
		stop_long_press_timer();
	_long_press_timer = ecore_timer_add(__LONG_PRESS_DURATION,
					    __on_long_press_timer,
					    this);
}

void view::gesture_detector::stop_long_press_timer()
{
	log("STOP LONG PRESS TIMER", FG_MAGENTA);
	if(_long_press_timer)
		ecore_timer_del(_long_press_timer);
	_long_press_timer = NULL;
}

Eina_Bool view::gesture_detector::__on_long_press_timer(void *data)
{
	gesture_detector *d = (gesture_detector *)data;
	if (!d)
		return ECORE_CALLBACK_CANCEL;

	d->log("------- LONG PRESS TIMER EVENT -------", FG_MAGENTA);
	d->_long_press_timer = NULL;
	d->on_long_press_timer();

	return ECORE_CALLBACK_CANCEL;
}

bool view::gesture_detector::is_gesture_available(maps_view_gesture_e gesture)
{
	return _maps_view_is_gesture_available(_view, gesture);
}

bool view::gesture_detector::finger_dragged_enough(int finger_no)
{
	const touch_point p1 = _info._finger_down[finger_no];
	const touch_point p2 = _info._finger_move[finger_no];
	return (get_trajectory_effective_length(p1, p2) >= __CLICK_AREA);
}

bool view::gesture_detector::finger_pressed_enough(int finger_no,
						   unsigned int duration_min,
						   unsigned int duration_max)
{
	unsigned int press_time = _info._finger_down[finger_no]._timestamp;
	unsigned int up_time = _info._finger_up[finger_no]._timestamp;

	unsigned int press_duration_tmp = up_time - press_time;
	MAPS_LOGI("finger_pressed_enough: finger: %d, press: %d, up: %d [%d]",
		  finger_no, press_time, up_time, press_duration_tmp);

	if(up_time < press_time)
		return false; /* Not yet unpressed */
	unsigned int press_duration = up_time - press_time;
	if ((press_duration >= duration_min) && (press_duration <= duration_max))
		return true;
	return false;
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

void view::gesture_detector::log(const char *str, log_colors color)
{
	MAPS_LOGI("%c[%d;%d;%dm"
		  "%s"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, color,
		  str,
		  0x1B, 0, 0, 0);
}


void view::gesture_detector::log_map_center(int color)
{
	maps_coordinates_h central_coords = NULL;
	maps_view_get_center(_view, &central_coords);
	double lat = .0, lon = .0;
	maps_coordinates_get_latitude_longitude(central_coords, &lat, &lon);
	maps_coordinates_destroy(central_coords);
	MAPS_LOGI("%c[%d;%d;%dm"
		  "central coordinates: (%f, %f)"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, color,
		  lat, lon,
		  0x1B, 0, 0, 0);
}
