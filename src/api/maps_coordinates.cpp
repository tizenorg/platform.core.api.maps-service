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
#include "maps_plugin_types.h"
#include "maps_coordinates.h"
#include "maps_util.h"
#include "maps_condition.h"

EXPORT_API int maps_coordinates_create(const double latitude,
				       const double longitude,
				       maps_coordinates_h *coords)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coords)
		return MAPS_ERROR_INVALID_PARAMETER;

	MAPS_CHECK_CONDITION(latitude >= -90
		&& latitude <= 90, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	MAPS_CHECK_CONDITION(longitude >= -180
		&& longitude <= 180, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");

	maps_coordinates_s *coord = g_new0(maps_coordinates_s, 1);
	if (coord == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}
	coord->latitude = latitude;
	coord->longitude = longitude;

	*coords = (maps_coordinates_h) coord;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_destroy(maps_coordinates_h coords)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coords)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_coordinates_s *handle = (maps_coordinates_s *) coords;
	g_free(handle);
	coords = NULL;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_clone(const maps_coordinates_h origin,
				      maps_coordinates_h *cloned)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;
	int error = MAPS_ERROR_NONE;
	do {
		maps_coordinates_s *c = (maps_coordinates_s *) origin;
		error = maps_coordinates_create(c->latitude, c->longitude, cloned);
		if (!(*cloned) or(error != MAPS_ERROR_NONE))
			break;
		return MAPS_ERROR_NONE;
	} while (false);
	maps_coordinates_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_coordinates_get_latitude(const maps_coordinates_h coords,
					     double *latitude)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coords || !latitude)
		return MAPS_ERROR_INVALID_PARAMETER;
	*latitude = ((maps_coordinates_s *) coords)->latitude;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_get_longitude(const maps_coordinates_h coords,
					      double *longitude)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coords || !longitude)
		return MAPS_ERROR_INVALID_PARAMETER;
	*longitude = ((maps_coordinates_s *) coords)->longitude;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_get_latitude_longitude(const maps_coordinates_h coords,
					      double *latitude,
					      double *longitude)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coords || !latitude || !longitude)
		return MAPS_ERROR_INVALID_PARAMETER;
	*latitude = ((maps_coordinates_s *) coords)->latitude;
	*longitude = ((maps_coordinates_s *) coords)->longitude;
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_coordinates_set_latitude(maps_coordinates_h coords,
					     const double latitude)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coords)
		return MAPS_ERROR_INVALID_PARAMETER;
	MAPS_CHECK_CONDITION(latitude >= -90
		&& latitude <= 90, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	((maps_coordinates_s *) coords)->latitude = latitude;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_set_longitude(maps_coordinates_h coords,
	const double longitude)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coords)
		return MAPS_ERROR_INVALID_PARAMETER;
	MAPS_CHECK_CONDITION(longitude >= -180
		&& longitude <= 180, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	((maps_coordinates_s *) coords)->longitude = longitude;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_set_latitude_longitude(maps_coordinates_h coords,
	const double latitude,
	const double longitude)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coords)
		return MAPS_ERROR_INVALID_PARAMETER;
	MAPS_CHECK_CONDITION(latitude >= -90
		&& latitude <= 90, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	MAPS_CHECK_CONDITION(longitude >= -180
		&& longitude <= 180, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	((maps_coordinates_s *) coords)->latitude = latitude;
	((maps_coordinates_s *) coords)->longitude = longitude;
	return MAPS_ERROR_NONE;
}

/*
 * Tizen 3.0
 */

EXPORT_API int maps_coordinates_list_create(maps_coordinates_list_h *coordinates_list)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coordinates_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_alloc();
	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");

	*coordinates_list = (void *) list;

	return MAPS_ERROR_NONE;
}

static void _free_coordinates(gpointer data)
{
	if (!data) return;

	int ret = 0;
	maps_coordinates_s *coordinates = (maps_coordinates_s *) data;
	ret = maps_coordinates_destroy(coordinates);
	if (ret) {
		MAPS_LOGI("Failed to maps_coordinates_destroy!!!");
	}
}

EXPORT_API int maps_coordinates_list_destroy(maps_coordinates_list_h coordinates_list)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coordinates_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = (GList *) coordinates_list;

	g_list_free_full(list, (GDestroyNotify) _free_coordinates);
	coordinates_list = NULL;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_append(maps_coordinates_list_h coordinates_list, maps_coordinates_h coordinates)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coordinates_list || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = (GList *)coordinates_list;
	maps_coordinates_s *coord = (maps_coordinates_s *)coordinates;
	list = g_list_append(list, coord);
	coordinates_list = (void *) list;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_remove(maps_coordinates_list_h coordinates_list, maps_coordinates_h coordinates)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coordinates_list || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = (GList *)coordinates_list;
	maps_coordinates_s *coord = (maps_coordinates_s *)coordinates;
	list = g_list_remove(list, coord);
	coordinates_list = (void *) list;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_get_length(maps_coordinates_list_h coordinates_list, int *length)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coordinates_list || !length)
		return MAPS_ERROR_INVALID_PARAMETER;

	*length = g_list_length((GList *)coordinates_list) - 1;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_foreach(maps_coordinates_list_h coordinates_list, maps_coordinates_cb callback, void *user_data)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!coordinates_list || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	bool ret = true;
	int index = 0;

	GList *list = g_list_first((GList *)coordinates_list);

	GList *l;
	for (l = (GList *)list; l != NULL; l = l->next)
	 {
		maps_coordinates_s *coordinates = (maps_coordinates_s *)l->data;
		if (coordinates) {
			ret = callback(index++, coordinates, user_data);
			if(!ret) break;
		}
	}

	return MAPS_ERROR_NONE;
}

bool maps_coordinates_is_valid(const maps_coordinates_h coordinates)
{
	if (!coordinates)
		return false;

	maps_coordinates_s *coord = (maps_coordinates_s *)coordinates;

	MAPS_CHECK_CONDITION(coord->latitude >= -90 && coord->latitude <= 90, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	MAPS_CHECK_CONDITION(coord->longitude >= -180 && coord->longitude <= 180, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");

	return true;
}
