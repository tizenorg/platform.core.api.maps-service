/*
 * Copyright (c) 2011-2014 Samsung Electronics Co., Ltd All Rights Reserved
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
#include "maps_util.h"
#include "maps_route_plugin.h"
#include "maps_error.h"
#include "maps_preference.h"
#include "maps_route_segment_plugin.h"
#include "maps_route_private.h"
#include "maps_route_segment_private.h"
#include "maps_condition.h"

static bool __is_supported(const maps_route_h route, maps_service_data_e data)
{
	bool supported = false;
	_maps_route_is_data_supported(route, data, &supported);
	return supported;
}

static bool __maps_route_set_supported_data_foreach_cb(int index, int total,
						       void *data,
						       void *user_data)
{
	_maps_route_segment_set_supported_data(data, user_data);
	return true;
}

/*----------------------------------------------------------------------------*/

/*
 * maps_route_s module
 */
typedef struct _maps_route_s
{
	char *route_id;			/*< route_id */
	maps_coordinates_h origin;	/*< Coordinate StartCoord */
	maps_coordinates_h destination;	/*< Coordinates destCoord */
	maps_area_h bounding_box;	/*< A rectangular geographical area */
	maps_route_transport_mode_e transport_mode;	/*< The transport mode
							  for the route */
	double total_distance;		/*< Total distance */
	long total_duration;		/*< Total duration */
	maps_distance_unit_e distance_unit;	/*< Distance units */
	maps_item_list_h segments;	/*< List of segments,
						  maps_route_segment_h */
	maps_item_list_h path;		/*< Path, list of maps_coordinates_h */
	maps_item_hashtable_h properties;	/*< Key/Value> */

	/* The table of available data features */
	maps_int_hashtable_h supported_data;
	/* TODO: implement hashtable<int, int> */
} maps_route_s;

const gsize _MAPS_ROUTE_ID_MAX_LENGTH = 32;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_route_create(maps_route_h *route)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route)
		return MAPS_ERROR_INVALID_PARAMETER;
	*route = (maps_route_h) g_slice_new0(maps_route_s);

	if (*route == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_destroy(maps_route_h route)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_route_s *p = (maps_route_s *) route;

	if (p->route_id)
		g_free(p->route_id);
	if (p->origin)
		maps_coordinates_destroy(p->origin);
	if (p->destination)
		maps_coordinates_destroy(p->destination);
	if (p->bounding_box)
		maps_area_destroy(p->bounding_box);
	if (p->properties)
		maps_item_hashtable_destroy(p->properties);

	if (p->segments) {
		maps_item_list_remove_all(p->segments,
			maps_route_segment_destroy);
		maps_item_list_destroy(p->segments);
	}

	if (p->path) {
		maps_item_list_remove_all(p->path, maps_coordinates_destroy);
		maps_item_list_destroy(p->path);
	}

	if (p->supported_data)
		maps_int_hashtable_destroy(p->supported_data);

	g_slice_free(maps_route_s, route);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_clone(const maps_route_h origin,
								maps_route_h *cloned)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_route_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_route_s *r = (maps_route_s *) origin;

		if (r->route_id) {
			error = maps_route_set_route_id(*cloned, r->route_id);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (r->origin) {
			error = maps_route_set_origin(*cloned, r->origin);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (r->destination) {
			error = maps_route_set_destination(*cloned, r->destination);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (r->bounding_box) {
			error = maps_route_set_bounding_box(*cloned, r->bounding_box);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		error = maps_route_set_transport_mode(*cloned, r->transport_mode);
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_route_set_total_distance(*cloned, r->total_distance);
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_route_set_total_duration(*cloned, r->total_duration);
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_route_set_distance_unit(*cloned, r->distance_unit);
		if (error != MAPS_ERROR_NONE)
			break;

		if (r->segments) {
			error = maps_route_set_segments(*cloned, r->segments);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (r->path) {
			error = maps_route_set_path(*cloned, r->path);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (r->properties) {
			error = maps_route_set_properties(*cloned, r->properties);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (r->supported_data) {
			error = _maps_route_set_supported_data(*cloned, r->supported_data);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	maps_route_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_route_get_route_id(const maps_route_h route,
								char **route_id)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !route_id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_route_s *) route)->route_id,
		_MAPS_ROUTE_ID_MAX_LENGTH, route_id);
}

EXPORT_API int maps_route_get_origin(const maps_route_h route,
								maps_coordinates_h *origin)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_coordinates_clone(((maps_route_s *) route)->origin, origin);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_get_destination(const maps_route_h route,
								maps_coordinates_h *destination)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !destination)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_coordinates_clone(((maps_route_s *) route)->destination,
		destination);
}

EXPORT_API int maps_route_get_bounding_box(const maps_route_h route,
								maps_area_h *bounding_box)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !bounding_box)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_area_clone(((maps_route_s *) route)->bounding_box,
		bounding_box);
}

EXPORT_API int maps_route_get_transport_mode(const maps_route_h route,
								maps_route_transport_mode_e *transport_mode)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !transport_mode)
		return MAPS_ERROR_INVALID_PARAMETER;
	*transport_mode = ((maps_route_s *) route)->transport_mode;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_get_total_distance(const maps_route_h route,
								double *total_distance)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !total_distance)
		return MAPS_ERROR_INVALID_PARAMETER;
	*total_distance = ((maps_route_s *) route)->total_distance;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_get_total_duration(const maps_route_h route,
								long *total_duration)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !total_duration)
		return MAPS_ERROR_INVALID_PARAMETER;
	*total_duration = ((maps_route_s *) route)->total_duration;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_foreach_path(const maps_route_h route,
								maps_route_path_cb callback,
								void *user_data)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (!__is_supported(route, MAPS_ROUTE_PATH))
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_route_s *) route)->path,
		maps_coordinates_clone, callback, user_data);
}

EXPORT_API int maps_route_foreach_segment(const maps_route_h route,
								maps_route_segment_cb callback,
								void *user_data)
{
	if (!route)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (!__is_supported(route, MAPS_ROUTE_SEGMENTS_PATH)
		&& !__is_supported(route, MAPS_ROUTE_SEGMENTS_MANEUVERS))
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_route_s *) route)->segments,
		maps_route_segment_clone, callback, user_data);
}

EXPORT_API int maps_route_foreach_property(const maps_route_h route,
								maps_route_properties_cb callback,
								void *user_data)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_foreach(((maps_route_s *) route)->properties,
		callback, user_data);
}

EXPORT_API int maps_route_get_distance_unit(const maps_route_h route,
								maps_distance_unit_e *distance_unit)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !distance_unit)
		return MAPS_ERROR_INVALID_PARAMETER;
	*distance_unit = ((maps_route_s *) route)->distance_unit;
	return MAPS_ERROR_NONE;
}

int _maps_route_is_data_supported(const maps_route_h route,
								maps_service_data_e data, bool *supported)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !supported)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_route_s *r = (maps_route_s *)route;

	if (!r->supported_data) {
		/* This is a case when the "supported" flags are not set yet */
		/* No need to limit access to fields */
		*supported = true;
		return MAPS_ERROR_NONE;
	}

	*supported = false;
	return maps_int_hashtable_contains(r->supported_data, data, supported);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_route_set_route_id(const maps_route_h route,
								const char *route_id)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !route_id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(route_id, _MAPS_ROUTE_ID_MAX_LENGTH,
		&((maps_route_s *) route)->route_id);
}

EXPORT_API int maps_route_set_origin(maps_route_h route,
								const maps_coordinates_h origin)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_s *p = (maps_route_s *) route;
	if (p->origin)
		maps_coordinates_destroy(p->origin);
	return maps_coordinates_clone(origin, &p->origin);
}

EXPORT_API int maps_route_set_destination(maps_route_h route,
								const maps_coordinates_h destination)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !destination)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_s *p = (maps_route_s *) route;
	if (p->destination)
		maps_coordinates_destroy(p->destination);
	return maps_coordinates_clone(destination, &p->destination);
}

EXPORT_API int maps_route_set_bounding_box(maps_route_h route,
								const maps_area_h bounding_box)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !bounding_box)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_s *p = (maps_route_s *) route;
	if (p->bounding_box)
		maps_area_destroy(p->bounding_box);
	return maps_area_clone(bounding_box, &p->bounding_box);
}

EXPORT_API int maps_route_set_total_distance(maps_route_h route,
								const double total_distance)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || total_distance < 0)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_s *) route)->total_distance = total_distance;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_set_total_duration(maps_route_h route,
								const long total_duration)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || total_duration < 0)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_s *) route)->total_duration = total_duration;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_set_transport_mode(maps_route_h route,
								const maps_route_transport_mode_e transport_mode)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((transport_mode < MAPS_ROUTE_TRANSPORT_MODE_CAR) ||
		(transport_mode > MAPS_ROUTE_TRANSPORT_MODE_TRUCK))
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_s *) route)->transport_mode = transport_mode;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_set_path(maps_route_h route,
								const maps_item_list_h path)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !path)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_s *p = (maps_route_s *) route;
	if (p->path) {
		maps_item_list_remove_all(p->path, maps_coordinates_destroy);
		maps_item_list_destroy(p->path);
	}
	maps_item_list_clone(path, maps_coordinates_clone, &p->path);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_set_segments(maps_route_h route,
								const maps_item_list_h segments)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !segments)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_s *p = (maps_route_s *) route;
	if (p->segments) {
		maps_item_list_remove_all(p->segments,
			maps_route_segment_destroy);
		maps_item_list_destroy(p->segments);
	}
	maps_item_list_clone(segments, maps_route_segment_clone, &p->segments);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_set_properties(maps_route_h route,
								const maps_item_hashtable_h properties)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route || !properties)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_s *p = (maps_route_s *) route;
	if (p->properties)
		maps_item_hashtable_destroy(p->properties);
	return maps_item_hashtable_clone(properties, &p->properties);
}

EXPORT_API int maps_route_set_distance_unit(maps_route_h route,
								const maps_distance_unit_e distance_unit)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!route)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((distance_unit < MAPS_DISTANCE_UNIT_M) ||
		(distance_unit > MAPS_DISTANCE_UNIT_YD))
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_s *) route)->distance_unit = distance_unit;
	return MAPS_ERROR_NONE;
}

int _maps_route_set_supported_data(maps_route_h route,
								const maps_int_hashtable_h supported_data)
{
	if (!route || !supported_data)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_s *p = (maps_route_s *) route;
	if (p->supported_data)
		maps_int_hashtable_destroy(p->supported_data);
	int error =
		maps_int_hashtable_clone(supported_data, &p->supported_data);
	if (error != MAPS_ERROR_NONE)
		return error;

	if (p->segments)
		error = maps_item_list_foreach(p->segments, NULL,
			__maps_route_set_supported_data_foreach_cb, supported_data);
	return error;
}
