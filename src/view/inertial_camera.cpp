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
#include <glib.h>


view::inertial_camera::inertial_camera(map_view_h view)
	: _view(view)
	  , target_zoom_factor(.0)
	  , target_rotation_angle(.0)
	  , target_tilt(.0)
	  , target_center(NULL)
	  , cur_zoom_factor(.0)
	  , cur_rotation_angle(.0)
	  , cur_tilt(.0)
	  , cur_center(NULL)
	  , transiting(false)
{
}

view::inertial_camera::inertial_camera(const inertial_camera &src)
	: _view(NULL)
	  , target_zoom_factor(.0)
	  , target_rotation_angle(.0)
	  , target_tilt(.0)
	  , target_center(NULL)
	  , cur_zoom_factor(.0)
	  , cur_rotation_angle(.0)
	  , cur_tilt(.0)
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
	map_view_get_tilt(_view, &cur_tilt);
}

void view::inertial_camera::set_targets(const maps_coordinates_h center,
					const double &zoom_factor,
					const double &rotation_angle,
					const double &tilt)
{
	if(!_view)
		return;

	/* Store the targets */
	if(!target_center)
		maps_coordinates_destroy(target_center);
	maps_coordinates_clone(center, &target_center);

	target_zoom_factor = zoom_factor;
	target_rotation_angle = rotation_angle;
	target_tilt = tilt;

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

void view::inertial_camera::set_tilt_target(const double &tilt)
{
	if(!_view)
		return;

	/* Store the target */
	target_tilt = tilt;

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
	/*double step = (finish - start) * 0.5;*/
	/*double step = (finish - start) * 0.2;*/
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
	double next_lat = calc_next_step(prev_lat, target_lat, .5);
	if(ABS(next_lat - prev_lat) > __GEO_ACCURACY)
		transiting = true;
	else
		next_lat = target_lat;

	double next_lon = calc_next_step(prev_lon, target_lon, .5);
	if(ABS(next_lon - prev_lon) > __GEO_ACCURACY)
		transiting = true;
	else
		next_lon = target_lon;

	maps_coordinates_set_latitude_longitude(cur_center, next_lat, next_lon);

	/* Transiting zoom */
	#if 0
	if (cur_zoom_factor > target_zoom_factor)
	cur_zoom_factor = calc_next_step(cur_zoom_factor,
					 target_zoom_factor,
					 .05);
	else
	#endif
	cur_zoom_factor = calc_next_step(cur_zoom_factor,
					 target_zoom_factor,
					 .2);
	if(ABS(cur_zoom_factor - target_zoom_factor) > __ZOOM_ACCURACY)
	{
		transiting = true;
	}
	else
	{
		cur_zoom_factor = target_zoom_factor;
	}

	/* Transizint orientation */
	cur_rotation_angle = calc_next_step(cur_rotation_angle,
					    target_rotation_angle,
					    .5);
	if(ABS(cur_rotation_angle - target_rotation_angle) > __ROTATE_ACCURACY)
		transiting = true;
	else
		cur_rotation_angle = target_rotation_angle;

	/* Transizint tilt */
	cur_tilt = calc_next_step(cur_tilt, target_tilt, .5);
	if(ABS(cur_tilt - target_tilt) > __TILT_ACCURACY)
		transiting = true;
	else
		cur_tilt = target_tilt;
}
