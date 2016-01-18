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

EXPORT_API int maps_coordinates_create(const double latitude,
				       const double longitude,
				       maps_coordinates_h *coords)
{
	if (!coords)
		return MAPS_ERROR_INVALID_PARAMETER;

	MAPS_CHECK_CONDITION(latitude >= -90
		&& latitude <= 90, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	MAPS_CHECK_CONDITION(longitude >= -180
		&& longitude <= 180, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");

	maps_coordinates_s *coord = g_slice_new0(maps_coordinates_s);

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
	if (!coords)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_coordinates_s *handle = (maps_coordinates_s *) coords;
	g_slice_free(maps_coordinates_s, handle);

	coords = NULL;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_clone(const maps_coordinates_h origin,
				      maps_coordinates_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		maps_coordinates_s *c = (maps_coordinates_s *) origin;

		error = maps_coordinates_create(c->latitude, c->longitude,
			cloned);

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
	if (!coords || !latitude)
		return MAPS_ERROR_INVALID_PARAMETER;
	*latitude = ((maps_coordinates_s *) coords)->latitude;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_get_longitude(const maps_coordinates_h coords,
					      double *longitude)
{
	if (!coords || !longitude)
		return MAPS_ERROR_INVALID_PARAMETER;
	*longitude = ((maps_coordinates_s *) coords)->longitude;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_get_latitude_longitude(const maps_coordinates_h coords,
					      double *latitude,
					      double *longitude)
{
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
	if (!coordinates_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_alloc();
	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");
	*coordinates_list = (void *) list;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_append(maps_coordinates_list_h coordinates_list, maps_coordinates_h coordinates)
{
	if (!coordinates_list || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_append((GList *) coordinates_list, (gpointer) coordinates);
	/*
	Although this logic is faster, it doesn't guarantee the order of item.
	GList *list = g_list_prepend((GList *) coordinates_list, (gpointer) coordinates);
	list = g_list_reverse(list);
	*/

	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");
	coordinates_list = (void *) list;

	return MAPS_ERROR_NONE;
}

static void _free_coordinates(gpointer data, gpointer user_data)
{
	if (!data) return;

	maps_coordinates_h coordinates = (maps_coordinates_h) data;
	maps_coordinates_list_h coordinates_list = (maps_coordinates_list_h) user_data;
	int ret = 0;

	if (coordinates_list) {
		ret = maps_coordinates_list_remove(coordinates_list, coordinates);
		if (ret) {
			MAPS_LOGI("Failed to maps_coordinates_list_remove!!!");
		}
	}

	if (coordinates) {
		ret = maps_coordinates_destroy(coordinates);
		if (ret) {
			MAPS_LOGI("Failed to maps_coordinates_destroy!!!");
		}
	}
}

EXPORT_API int maps_coordinates_list_destroy(maps_coordinates_list_h coordinates_list)
{
	if (!coordinates_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = (GList *) coordinates_list;
	MAPS_LOGD("length = %d", g_list_length(list));

	list = g_list_first(list);
	g_list_foreach(list, _free_coordinates, list);
	g_list_free(list);
	coordinates_list = NULL;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_remove(maps_coordinates_list_h coordinates_list, maps_coordinates_h coordinates)
{
	if (!coordinates_list || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_remove((GList *)coordinates_list, (gpointer) coordinates);
	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");
	coordinates_list = (void *) list;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_get_length(maps_coordinates_list_h coordinates_list, int *length)
{
	if (!coordinates_list || !length)
		return MAPS_ERROR_INVALID_PARAMETER;

	*length = g_list_length((GList *)coordinates_list) - 1;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_foreach(maps_coordinates_list_h coordinates_list, maps_coordinates_cb callback, void *user_data)
{
	if (!coordinates_list || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	bool ret = true;
	int index = 0;

	GList *list = (GList *)coordinates_list;
	list = g_list_first(list);
	while (list != NULL)
	{
		GList *next = list->next;
		maps_coordinates_s *coordinates = (maps_coordinates_s *)list->data;
		if (coordinates) {
			/*
			int ret = callback(index++, coordinates->latitude, coordinates->longitude, user_data);
			*/
			MAPS_LOGD("index = %d", index);
			ret = callback(index++, coordinates, user_data);
		}
		if (ret)
			list = next;
		else
			break;
	}

	return MAPS_ERROR_NONE;
}
