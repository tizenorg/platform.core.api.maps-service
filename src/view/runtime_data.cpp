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
#include "runtime_data.h"
#include "maps_util.h"
#include "map_view_plugin.h"


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

	MAPS_LOGI("%c[%d;%d;%dm"
		  "central coordinates: RESET"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, 31,
		  0x1B, 0, 0, 0);
}

void view::map_state::capture(map_view_h view)
{
	if(!view)
		return;

	reset();

	map_view_get_center(view, &_center);
	map_view_get_scale_factor(view, &_zoom_factor);
	map_view_get_orientation(view, &_rotation_angle);


	/* DEBUG */
	maps_coordinates_h central_coords = NULL;
	map_view_get_center(view, &central_coords);
	double lat = .0, lon = .0;
	maps_coordinates_get_latitude_longitude(central_coords, &lat, &lon);
	maps_coordinates_destroy(central_coords);
	MAPS_LOGI("%c[%d;%d;%dm"
		  "central coordinates: (%f, %f)"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, 31,
		  lat, lon,
		  0x1B, 0, 0, 0);
}

void view::map_state::trace()
{
	double latitude = .0;
	double longitude = .0;
	maps_coordinates_get_latitude_longitude(_center, &latitude, &longitude);

	MAPS_LOGI("%c[%d;%d;%dm"
		  "Map State: center(%f,%f), zoom(%f), angle(%f)"
		  "%c[%d;%d;%dm",
		  0x1B, 1, 0, 93,
		  latitude, longitude, _zoom_factor, _rotation_angle,
		  0x1B, 0, 0, 0);
}
/* ---------------------------------------------------------------------------*/

view::runtime_touch_info::runtime_touch_info()
	: _fingers_pressed(0)
	  , _cur_gesture(MAP_GESTURE_NONE)
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
		_fingers_pressed = src._fingers_pressed;
		_cur_gesture = src._cur_gesture;
		/*_going = src._going;*/
		_start_view_state = src._start_view_state;

		for(unsigned int i = 0; i < MAX_FINGERS; i ++) {
			_finger_down[i] = src._finger_down[i];
			_finger_move[i] = src._finger_move[i];
			_prev_finger_down[i] = src._prev_finger_down[i];
			_finger_up[i] = src._finger_up[i];
			_is_finger_pressed[i] = src._is_finger_pressed[i];
		}
	}
	return *this;
}

void view::runtime_touch_info::reset()
{
	/*_going = false;*/

	_fingers_pressed = 0;

	for(unsigned int i = 0; i < MAX_FINGERS; i ++) {
		_finger_down[i].reset();
		_finger_move[i].reset();
		_prev_finger_down[i].reset();
		_finger_up[i].reset();
		_is_finger_pressed[i] = false;
	}

	_cur_gesture = MAP_GESTURE_NONE;

	_start_view_state.reset();
}

int view::runtime_touch_info::calc_finger_pressed()
{
	int cnt = 0;
	for(unsigned int i = 0; i < MAX_FINGERS; i ++)
		if(_is_finger_pressed[i])
			cnt ++;
	return cnt;
}
