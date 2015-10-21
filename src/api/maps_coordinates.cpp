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
#include "maps_extra_types.h"
#include "maps_plugin_types.h"
#include "maps_coordinates.h"
#include "maps_util.h"


typedef struct _list_callback_s{
		int index;
		void *callback;
		void *user_data;
} list_callback_s;


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

/*
 * Tizen 3.0
 */

EXPORT_API int maps_coordinates_list_create(maps_coordinates_list_h *coordinates_list)
{
	if (!coordinates_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_alloc();
	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");
	*coordinates_list = list;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_append(maps_coordinates_list_h coordinates_list, maps_coordinates_h coordinates)
{
	if (!coordinates_list || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_append(coordinates_list, (gpointer) coordinates);
	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");
	coordinates_list = list;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_remove(maps_coordinates_list_h coordinates_list, maps_coordinates_h coordinates)
{
	if (!coordinates_list || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_remove(coordinates_list, (gpointer) coordinates);
	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");
	coordinates_list = list;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_list_length(maps_coordinates_list_h coordinates_list, int *length)
{
	if (!coordinates_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	*length = g_list_length(coordinates_list);

	return MAPS_ERROR_NONE;
}

#if 0
static void _list_cb(gpointer data, gpointer user_data)
{
	list_callback_s *cb_data = (list_callback_s) user_data;
	int ret = 0;

	if (cb_data) {
		cb_data->callback(cb_data->index++, cb_data->latitude, cb_data->longitude, cb_data->user_data);
		//ret = cb_data->callback(cb_data->index++, cb_data->coordinates, cb_data->user_data);
		if (!ret) break;
	}

}
#endif

EXPORT_API int maps_coordinates_list_foreach_coordinates(maps_coordinates_list_h coordinates_list, coordinates_cb callback, void *user_data)
{
	if (!coordinates_list || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

#if 0
	list_callback_s *cb_data = g_new0(list_callback_s);
	MAPS_CHECK_CONDITION(cb_data, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");

	cb_data->callback = (void *)coordinates_cb;
	cb_data->user_data = user_data;

	g_list_foreach(coordinates_list, _list_cb, cb_data);
#endif

	GList *l = coordinates_list;
	while (l != NULL)
	{
		GList *next = l->next;
		int index = 0;
		maps_coordinates_s *coordinates = (maps_coordinates_s *)l->data;
		if (coordinates) {
			/*
			int ret = callback(index++, coordinates->latitude, coordinates->longitude, user_data);
			*/
			int ret = callback(index++, coordinates, user_data);
			if (ret)
				l = next;
			else
				break;
		}
	}

	return MAPS_ERROR_NONE;
}

static void _free_coordinates(gpointer data)
{
	maps_coordinates_h coordinates = (maps_coordinates_h) data;
	int ret = maps_coordinates_destroy(coordinates);
	if (!ret) {
		MAPS_LOGI("Failed to maps_coordinates_destroy!!!");
	}
}

EXPORT_API int maps_coordinates_list_destroy(maps_coordinates_list_h coordinates_list, maps_coordinates_h coordinates)
{
	if (!coordinates_list || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	g_list_free_full(coordinates_list, _free_coordinates);
	coordinates_list = NULL;

	return MAPS_ERROR_NONE;
}
