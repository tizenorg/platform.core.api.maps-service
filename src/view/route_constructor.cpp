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


#include "route_constructor.h"
#include <glib.h>

#ifdef TIZEN_3_0_NEXT_MS
extern int _map_object_create(const map_object_type_e type,
			      map_object_h *object);
extern int _map_object_route_set_content(map_object_h route,
					      const maps_route_h content);


map_object_h view::route_constructor::construct(maps_route_h route)
{
	if (!route) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return NULL;
	}

	__error = MAPS_ERROR_NONE;

	map_object_h route_object = NULL;
	do {
		/* 1. Create a visual object for route */
		__error = _map_object_create(MAP_OBJECT_ROUTE,
						 &route_object);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 2. Assign route object to route group */
		__error =  _map_object_route_set_content(route_object,
							      route);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 3. Set Route visible */
		__error = map_object_set_visible(route_object, true);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* SUCCESS */
		return route_object;

	} while(false);

	/* FAILURE: Releasing objects */
	map_object_destroy(route_object);
	return NULL;
}
#endif /* TIZEN_3_0_NEXT_MS */


	/* !!! The old route constructor will be deleted soom!!! */
#if 0

#include "marker_constructor.h"
#include "polyline_constructor.h"

extern int _map_object_group_set_route(map_object_h group,
					    const maps_route_h route);

const char *view::route_constructor::get_start_marker_image()
{
	static const char *image = "start_marker.png";
	return image;
}

const char *view::route_constructor::get_finish_marker_image()
{
	static const char *image = "finish_marker.png";
	return image;
}

const char *view::route_constructor::get_default_marker_image()
{
	static const char *image = "route_marker.png";
	return image;
}


int view::route_constructor::add_marker(map_object_h route_object,
					 const maps_coordinates_h coords,
					 map_marker_type_e type)
{
	if(!route_object || !coords)
		return MAPS_ERROR_INVALID_PARAMETER;

	const char *image_file = NULL;
	switch(type) {
	case MAP_MARKER_START:
		image_file = get_start_marker_image();
		break;
	case MAP_MARKER_FINISH:
		image_file = get_finish_marker_image();
		break;
	default:
		image_file = get_default_marker_image();
		break;
	}

	marker_constructor mc;
	map_object_h marker = mc.construct(coords, image_file, type);
	if(!marker)
		return mc.get_error();
	return map_object_group_add_object(route_object, marker);
}

bool view::route_constructor::add_trajectory_markers(int index,
						     int total,
						     void *data,
						     void *user_data)
{
	if(!data || !user_data)
		return false;
	add_marker(user_data, data, MAP_MARKER_NONE);
	return true;
}

int view::route_constructor::add_trajectory(map_object_h route_object,
					    maps_item_list_h points)
{
	if(!route_object || !points)
		return MAPS_ERROR_INVALID_PARAMETER;

	polyline_constructor pc;
	map_object_h polyline = pc.construct(points, 255, 0, 0, 0, 1);
	if(!polyline)
		return pc.get_error();
	int e = map_object_group_add_object(route_object, polyline);

	maps_item_list_foreach(points,
			   maps_item_no_clone,
			   add_trajectory_markers,
			   route_object);
	return e;
}

void view::route_constructor::add_start_marker(map_object_h route_object,
					       maps_route_h route)
{
	if(!route_object || !route) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return;
	}

	maps_coordinates_h origin = NULL;
	int __error = maps_route_get_origin(route, &origin);
	if (__error == MAPS_ERROR_NONE) {
		__error = add_marker(route_object,
				     origin,
				     MAP_MARKER_START);
		maps_coordinates_destroy(origin);
	}
}

void view::route_constructor::add_finish_marker(map_object_h route_object,
						maps_route_h route)
{
	if(!route_object || !route) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return;
	}

	maps_coordinates_h destination = NULL;
	__error = maps_route_get_destination(route, &destination);
	if (__error == MAPS_ERROR_NONE) {
		__error = add_marker(route_object,
				     destination,
				     MAP_MARKER_FINISH);
		maps_coordinates_destroy(destination);
	}
}

void view::route_constructor::add_route_path(map_object_h route_object,
					     maps_route_h route)
{
	if(!route_object || !route) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return;
	}

	maps_item_list_h points = NULL;
	__error = maps_item_list_create(&points);
	if (__error != MAPS_ERROR_NONE)
		return;

	/* Add Path coordinates */
	maps_route_foreach_path(route, collect_path_points, points);

	/* Add trajectory to the group */
	__error = add_trajectory(route_object, points);

	maps_item_list_remove_all(points, maps_coordinates_destroy);
	maps_item_list_destroy(points);
}

bool view::route_constructor::collect_path_points(int index, int total,
						 maps_coordinates_h coords,
						 void *user_data)
{
	if (!coords)
		return false;

	maps_item_list_h list = (maps_item_list_h)user_data;
	if (list)
		maps_item_list_append(list, coords, maps_coordinates_clone);

	maps_coordinates_destroy(coords);
	return true;
}

void view::route_constructor::add_route_segments(map_object_h route_object,
						 maps_route_h route)
{
	if(!route_object || !route) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return;
	}

	maps_route_foreach_segment(route, collect_segments, route_object);
}

bool view::route_constructor::collect_segments(int index, int total,
					       maps_route_segment_h segment,
					       void *user_data)
{
	if (!segment)
		return false;

	map_object_h route_object = (map_object_h)user_data;
	if(!route_object) {
		maps_route_segment_destroy(segment);
		return false;
	}


	maps_item_list_h points = NULL;
	int e = maps_item_list_create(&points);
	if (e != MAPS_ERROR_NONE) {
		maps_route_segment_destroy(segment);
		return false;
	}

	/* Add origin coordinates */
	maps_coordinates_h origin = NULL;
	maps_route_segment_get_origin(segment, &origin);
	if(origin)
		maps_item_list_append(points, origin, maps_item_no_clone);

	/* Add Segment Path coordinates */
	maps_route_segment_foreach_path(segment, collect_path_points, points);

	/* Add destination coordinates */
	maps_coordinates_h destination = NULL;
	maps_route_segment_get_destination(segment, &destination);
	if(destination)
		maps_item_list_append(points, destination, maps_item_no_clone);

	/* Add trajectory to the group */
	view::route_constructor::add_trajectory(route_object, points);

	maps_item_list_remove_all(points, maps_coordinates_destroy);
	maps_item_list_destroy(points);

	maps_route_segment_destroy(segment);
	return true;
}

map_object_h view::route_constructor::construct(map_view_h view,
						     maps_route_h route)
{
	if (!route) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return NULL;
	}

	__error = MAPS_ERROR_NONE;

	map_object_h route_object = NULL;
	do {
		/* 1. Create a visual object for route */
		__error = _map_object_create(MAP_OBJECT_GROUP,
						 &route_object);
		if (__error != MAPS_ERROR_NONE)
			break;


		/* 2.a Add Start Marker */
		add_start_marker(route_object, route);

		/* 2.b Add Finish Marker */
		add_finish_marker(route_object, route);


		/* 3.a Construct trajectory on the base of Route Path */
		add_route_path(route_object, route);

		/* 3.b Construct trajectory on the base of Route Path */
		add_route_segments(route_object, route);



		/* 3. Assign route object to route group */
		__error =  _map_object_group_set_route(route_object,
							    route);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 4. Set Route visible */
		__error = map_object_set_visible(route_object, true);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* SUCCESS */
		return route_object;

	} while(false);

	/* FAILURE: Releasing objects */
	map_object_destroy(route_object);
	return NULL;
}

#endif
