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


#include "inertial_camera.h"
#include "maps_util.h"
#include "map_view_plugin.h"
#include <glib.h>


view::inertial_camera::inertial_camera(map_view_h view)
	: _view(view)
	  , target_zoom_factor(.0)
	  , target_rotation_angle(.0)
	  , target_center(NULL)
	  , cur_zoom_factor(.0)
	  , cur_rotation_angle(.0)
	  , cur_center(NULL)
	  , transiting(false)
{
}

view::inertial_camera::inertial_camera(const inertial_camera &src)
	: _view(NULL)
	  , target_zoom_factor(.0)
	  , target_rotation_angle(.0)
	  , target_center(NULL)
	  , cur_zoom_factor(.0)
	  , cur_rotation_angle(.0)
	  , cur_center(NULL)
	  , transiting(false)
{
}

view::inertial_camera &view::inertial_camera::operator=(const inertial_camera
							&src)
{
	return *this;
}

view::inertial_camera::~inertial_camera()
{
	maps_coordinates_destroy(target_center);
	maps_coordinates_destroy(cur_center);
}

void view::inertial_camera::set_cur_state()
{
	if(!_view)
		return;

	if(!cur_center)
		maps_coordinates_destroy(cur_center);
	map_view_get_center(_view, &cur_center);
	map_view_get_zoom_factor(_view, &cur_zoom_factor);
	map_view_get_orientation(_view, &cur_rotation_angle);
}

void view::inertial_camera::set_targets(const maps_coordinates_h center,
					const double &zoom_factor,
					const double &rotation_angle)
{
	if(!_view)
		return;

	/* Store the targets */
	if(!target_center)
		maps_coordinates_destroy(target_center);
	maps_coordinates_clone(center, &target_center);

	target_zoom_factor = zoom_factor;
	target_rotation_angle = rotation_angle;

	/* Store current state */
	if(!transiting)
		set_cur_state();

	/* Start transition */
	transiting = true;
}

void view::inertial_camera::set_center_target(const maps_coordinates_h center)
{
	if(!_view)
		return;

	/* Store the target */
	if(!target_center)
		maps_coordinates_destroy(target_center);
	maps_coordinates_clone(center, &target_center);

	/* Store current state */
	if(!transiting)
		set_cur_state();

	/* Start transition */
	transiting = true;
}

void view::inertial_camera::set_zoom_target(const double &zoom_factor)
{
	if(!_view)
		return;

	/* Store the target */
	target_zoom_factor = zoom_factor;

	/* Store current state */
	if(!transiting)
		set_cur_state();

	/* Start transition */
	transiting = true;
}

void view::inertial_camera::set_rotation_target(const double &rotation_angle)
{
	if(!_view)
		return;

	/* Store the target */
	target_rotation_angle = rotation_angle;

	/* Store current state */
	if(!transiting)
		set_cur_state();

	/* Start transition */
	transiting = true;
}

double view::inertial_camera::calc_next_step(const double &start,
					     const double &finish,
					     const double step_ratio) const
{
	if(start == finish)
		return start;

	/* Expanential transition */
	double step = (finish - start) * step_ratio;
	double new_pos = start + step;
	if(finish > start) {
		new_pos = MAX(new_pos, start);
		new_pos = MIN(new_pos, finish);
	} else {
		new_pos = MIN(new_pos, start);
		new_pos = MAX(new_pos, finish);
	}
	return new_pos;
}

void view::inertial_camera::next_transition_step()
{
	if(!_view)
		return;

	transiting = false;

	double prev_lat = .0;
	double prev_lon = .0;
	double target_lat = .0;
	double target_lon = .0;
	maps_coordinates_get_latitude_longitude(cur_center, &prev_lat, &prev_lon);
	maps_coordinates_get_latitude_longitude(target_center, &target_lat, &target_lon);

	/* Transiting map center */
	double next_lat = calc_next_step(prev_lat, target_lat, .15);
	if (next_lat != prev_lat) transiting = true;

	double next_lon = calc_next_step(prev_lon, target_lon, .15);
	if (next_lon != prev_lon) transiting = true;

	if (transiting)
	{
		int tx, ty, nx, ny;
		maps_coordinates_s next_center_s = { next_lat, next_lon };
		map_view_geolocation_to_screen(_view, &next_center_s, &nx, &ny);
		map_view_geolocation_to_screen(_view, target_center, &tx, &ty);
		if (nx == tx && ny == ty) {
			transiting = false;
			next_lat = target_lat;
			next_lon = target_lon;
		}
	}

	maps_coordinates_set_latitude_longitude(cur_center, next_lat, next_lon);

	/* Transiting zoom */
	cur_zoom_factor = calc_next_step(cur_zoom_factor, target_zoom_factor, .2);
	if(ABS(cur_zoom_factor - target_zoom_factor) > __ZOOM_ACCURACY)
		transiting = true;
	else
		cur_zoom_factor = target_zoom_factor;

	/* Transizint orientation */
	if(target_rotation_angle - cur_rotation_angle > 180.)
		target_rotation_angle -= 360.;
	else if(target_rotation_angle - cur_rotation_angle < -180.)
		target_rotation_angle += 360.;

	cur_rotation_angle = calc_next_step(cur_rotation_angle, target_rotation_angle, .5);
	if(ABS(cur_rotation_angle - target_rotation_angle) > __ROTATE_ACCURACY)
		transiting = true;
	else
		cur_rotation_angle = target_rotation_angle;
}
