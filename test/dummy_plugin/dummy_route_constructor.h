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

#ifndef __MAPS_VIEW_ROUTE_CONSTRUCTOR_H__
#define __MAPS_VIEW_ROUTE_CONSTRUCTOR_H__


#include "map_object.h"
#include "maps_extra_types.h"


#ifdef TIZEN_3_0_NEXT_MS
namespace view
{

	class dummy_route_constructor {
	private:
		int __error;
	public:
		dummy_route_constructor()
			: __error(MAPS_ERROR_NONE)
		{
		}
	public:
		map_object_h construct(maps_route_h route);
		int get_error() const
		{
			return __error;
		}
	public:
		static const char *get_start_marker_image();
		static const char *get_finish_marker_image();
		static const char *get_default_marker_image();
	protected:
		static int add_marker(map_object_h route_object,
				       const maps_coordinates_h coords,
				       map_marker_type_e type);
		void add_start_marker(map_object_h route_object,
				      maps_route_h route);
		void add_finish_marker(map_object_h route_object,
				       maps_route_h route);
	protected:
		static int add_trajectory(map_object_h route_object,
					  maps_item_list_h route_points);

		static bool add_trajectory_markers(int index,
						   int total,
						   void *data,
						   void *user_data);

		void add_route_path(map_object_h route_object,
				    maps_route_h route);
		static bool collect_path_points(int index, int total,
						maps_coordinates_h coordinates,
						void *user_data);

		void add_route_segments(map_object_h route_object,
					maps_route_h route);
		static bool collect_segments(int index, int total,
						   maps_route_segment_h segment,
						   void *user_data);
	};

};
#endif /* TIZEN_3_0_NEXT_MS */

#endif				/* __MAPS_VIEW_ROUTE_CONSTRUCTOR_H__ */
