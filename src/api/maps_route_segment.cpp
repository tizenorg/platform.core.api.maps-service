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
#include "maps_error.h"
#include "maps_route_segment_plugin.h"
#include "maps_route_maneuver_plugin.h"
#include "maps_extra_types.h"
#include "maps_util.h"
#include "maps_route_segment_private.h"

static bool __is_supported(const maps_route_segment_h route_segment,
								maps_service_data_e data)
{
	bool supported = false;
	_maps_route_segment_is_data_supported(route_segment, data, &supported);
	return supported;
}

/*----------------------------------------------------------------------------*/

typedef struct _maps_route_segment_s
{
	maps_coordinates_h origin;
	maps_coordinates_h destination;
	maps_area_h bounding_box;
	double distance;
	long duration;
	maps_item_list_h path;	/*< path, list of maps_coordinates_h */
	maps_item_list_h maneuvers;	/*< list of maneuvers,
					  maps_route_maneuver_h */

	/* The table of available data features */
	maps_int_hashtable_h supported_data;
} maps_route_segment_s;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_route_segment_create(maps_route_segment_h *segment)
{
	if (!segment)
		return MAPS_ERROR_INVALID_PARAMETER;
	*segment = (maps_route_segment_h) g_slice_new0(maps_route_segment_s);

	if (*segment == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_segment_destroy(maps_route_segment_h segment)
{
	if (!segment)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_route_segment_s *p = (maps_route_segment_s *) segment;

	if (p->origin)
		maps_coordinates_destroy(p->origin);
	if (p->destination)
		maps_coordinates_destroy(p->destination);
	if (p->bounding_box)
		maps_area_destroy(p->bounding_box);
	if (p->path) {
		maps_item_list_remove_all(p->path, maps_coordinates_destroy);
		maps_item_list_destroy(p->path);
	}
	if (p->maneuvers) {
		maps_item_list_remove_all(p->maneuvers,
			maps_route_maneuver_destroy);
		maps_item_list_destroy(p->maneuvers);
	}

	if (p->supported_data)
		maps_int_hashtable_destroy(p->supported_data);

	g_slice_free(maps_route_segment_s, segment);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_segment_clone(const maps_route_segment_h segment,
								maps_route_segment_h *cloned)
{
	if (!cloned || !segment)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_route_segment_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_route_segment_s *p = (maps_route_segment_s *) segment;

		if (p->origin) {
			error = maps_route_segment_set_origin(*cloned, p->origin);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->destination) {
			error = maps_route_segment_set_destination(*cloned, p->destination);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->bounding_box) {
			error = maps_route_segment_set_bounding_box(*cloned, p->bounding_box);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		error = maps_route_segment_set_distance(*cloned, p->distance);
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_route_segment_set_duration(*cloned, p->duration);
		if (error != MAPS_ERROR_NONE)
			break;

		if (p->path) {
			error = maps_route_segment_set_path(*cloned, p->path);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->maneuvers) {
			error = maps_route_segment_set_maneuvers(*cloned, p->maneuvers);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->supported_data) {
			error = _maps_route_segment_set_supported_data(*cloned, p->supported_data);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	maps_route_segment_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_route_segment_get_origin(const maps_route_segment_h segment,
								maps_coordinates_h *origin)
{
	if (!segment || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_coordinates_clone(((maps_route_segment_s *) segment)->
		origin, origin);
}

EXPORT_API int maps_route_segment_get_destination(const maps_route_segment_h segment,
								maps_coordinates_h *destination)
{
	if (!segment || !destination)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_coordinates_clone(((maps_route_segment_s *) segment)->
		destination, destination);
}

EXPORT_API int maps_route_segment_get_bounding_box(const maps_route_segment_h segment,
								maps_area_h *bounding_box)
{
	if (!segment || !bounding_box)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_area_clone(((maps_route_segment_s *) segment)->bounding_box,
		bounding_box);
}

EXPORT_API int maps_route_segment_get_distance(const maps_route_segment_h segment,
								double *distance)
{
	if (!segment || !distance)
		return MAPS_ERROR_INVALID_PARAMETER;
	*distance = ((maps_route_segment_s *) segment)->distance;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_segment_get_duration(const maps_route_segment_h segment,
								long *duration)
{
	if (!segment || !duration)
		return MAPS_ERROR_INVALID_PARAMETER;
	*duration = ((maps_route_segment_s *) segment)->duration;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_segment_foreach_path(const maps_route_segment_h segment,
								maps_route_segment_path_cb callback,
								void *user_data)
{
	/* Check if the handle of the Segment is valid */
	if (!segment)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(segment, MAPS_ROUTE_SEGMENTS_PATH))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_route_segment_s *) segment)->path,
		maps_coordinates_clone, callback, user_data);
}

EXPORT_API int maps_route_segment_foreach_maneuver(const maps_route_segment_h segment,
								maps_route_segment_maneuver_cb callback,
								void *user_data)
{
	/* Check if the handle of the Segment is valid */
	if (!segment)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(segment, MAPS_ROUTE_SEGMENTS_MANEUVERS))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_route_segment_s *) segment)->
		maneuvers, maps_route_maneuver_clone, callback, user_data);
}

int _maps_route_segment_is_data_supported(const maps_route_segment_h segment,
								maps_service_data_e data,
								bool *supported)
{
	if (!segment || !supported)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_route_segment_s *s = (maps_route_segment_s *)segment;
	if (!s->supported_data) {
		/* This is a case when the "supported" flags are not set yet */
		/* No need to limit access to fields */
		*supported = true;
		return MAPS_ERROR_NONE;
	}

	*supported = false;
	return maps_int_hashtable_contains(s->supported_data, data, supported);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_route_segment_set_origin(maps_route_segment_h segment,
								const maps_coordinates_h origin)
{
	if (!segment || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_segment_s *p = (maps_route_segment_s *) segment;
	if (p->origin)
		maps_coordinates_destroy(p->origin);
	return maps_coordinates_clone(origin, &p->origin);
}

EXPORT_API int maps_route_segment_set_destination(maps_route_segment_h segment,
								const maps_coordinates_h destination)
{
	if (!segment || !destination)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_segment_s *p = (maps_route_segment_s *) segment;
	if (p->destination)
		maps_coordinates_destroy(p->destination);
	return maps_coordinates_clone(destination, &p->destination);
}

EXPORT_API int maps_route_segment_set_bounding_box(maps_route_segment_h segment,
								const maps_area_h bounding_box)
{
	if (!segment || !bounding_box)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_segment_s *p = (maps_route_segment_s *) segment;
	if (p->bounding_box)
		maps_area_destroy(p->bounding_box);
	return maps_area_clone(bounding_box, &p->bounding_box);
}

EXPORT_API int maps_route_segment_set_distance(maps_route_segment_h segment,
								const double distance)
{
	if (!segment || distance < 0)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_segment_s *) segment)->distance = distance;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_segment_set_duration(maps_route_segment_h segment,
								const long duration)
{
	if (!segment || duration < 0)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_segment_s *) segment)->duration = duration;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_segment_set_path(maps_route_segment_h segment,
								const maps_item_list_h path)
{
	if (!segment || !path)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_segment_s *s = (maps_route_segment_s *) segment;
	if (s->path) {
		maps_item_list_remove_all(s->path, maps_coordinates_destroy);
		maps_item_list_destroy(s->path);
	}
	maps_item_list_clone(path, maps_coordinates_clone, &s->path);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_segment_set_maneuvers(maps_route_segment_h segment,
								const maps_item_list_h maneuvers)
{
	if (!segment || !maneuvers)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_segment_s *s = (maps_route_segment_s *) segment;
	if (s->maneuvers) {
		maps_item_list_remove_all(s->maneuvers,
			maps_route_maneuver_destroy);
		maps_item_list_destroy(s->maneuvers);
	}
	maps_item_list_clone(maneuvers, maps_route_maneuver_clone,
		&s->maneuvers);
	return MAPS_ERROR_NONE;
}

int _maps_route_segment_set_supported_data(maps_route_segment_h segment,
								const maps_int_hashtable_h supported_data)
{
	if (!segment || !supported_data)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_segment_s *p = (maps_route_segment_s *) segment;
	if (p->supported_data)
		maps_int_hashtable_destroy(p->supported_data);
	return maps_int_hashtable_clone(supported_data, &p->supported_data);
}