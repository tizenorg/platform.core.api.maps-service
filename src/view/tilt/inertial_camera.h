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

#ifndef __MAPS_VIEW_INERTIAL_CAMERA_H__
#define __MAPS_VIEW_INERTIAL_CAMERA_H__


#include "map_view.h"

namespace view
{
	class inertial_camera {
	protected:
		map_view_h _view;

		/* Targets */
		double target_zoom_factor;
		double target_rotation_angle;
		maps_coordinates_h target_center;

		/* Current state of transition (or begining) */
		double cur_zoom_factor;
		double cur_rotation_angle;
		maps_coordinates_h cur_center;

		/* Is transition performing right now */
		bool transiting;
	protected:
		static const double __GEO_ACCURACY = 0.00001;
		static const double __ZOOM_ACCURACY = 0.1;
		static const double __ROTATE_ACCURACY = 0.5;
	public:
		inertial_camera(map_view_h view);
		~inertial_camera();
	private:
		inertial_camera(const inertial_camera &src);
		inertial_camera &operator=(const inertial_camera &src);
	public:
		void set_targets(const maps_coordinates_h center,
				 const double &zoom_factor,
				 const double &rotation_angle);
		void set_center_target(const maps_coordinates_h center);
		void set_zoom_target(const double &zoom_factor);
		void set_rotation_target(const double &rotation_angle);

		void next_transition_step();

		bool is_transiting() const { return transiting; }
		void set_transiting(bool b) { transiting = b; }
		maps_coordinates_h get_cur_center() const { return cur_center; }
		double get_cur_zoom_factor() const { return cur_zoom_factor; }
		double get_cur_rotation_angle() const {
			return cur_rotation_angle; }

	protected:
		double calc_next_step(const double &start,
				     const double &finish,
				     const double step_ratio) const;
		void set_cur_state();
	};
};

#endif /* __MAPS_VIEW_INERTIAL_CAMERA_H__ */
