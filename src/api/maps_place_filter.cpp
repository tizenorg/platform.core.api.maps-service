/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "maps_error.h"
#include "maps_place_filter.h"
#include "maps_extra_types.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_filter_s
{
	maps_item_hashtable_h table;
} maps_place_filter_s;

/*----------------------------------------------------------------------------*/
//LCOV_EXCL_START
EXPORT_API int maps_place_filter_create(maps_place_filter_h *filter)
{
	if (!filter)
		return MAPS_ERROR_INVALID_PARAMETER;
	*filter = (maps_place_filter_h) g_slice_new0(maps_place_filter_s);

	if (*filter == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	maps_place_filter_s *f = (maps_place_filter_s *) (*filter);
	maps_item_hashtable_create(&f->table);

	return MAPS_ERROR_NONE;
}
//LCOV_EXCL_STOP

EXPORT_API int maps_place_filter_destroy(maps_place_filter_h filter)
{
	if (!filter)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_filter_s *f = (maps_place_filter_s *) filter;

	if (f->table)
		maps_item_hashtable_destroy(f->table);

	g_slice_free(maps_place_filter_s, filter);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_filter_clone(const maps_place_filter_h origin,
								maps_place_filter_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_filter_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_filter_s *f = (maps_place_filter_s *) origin;

		if (f->table) {
			maps_place_filter_s *f_cloned =
				(maps_place_filter_s *) (*cloned);
			if (f_cloned->table)
				maps_item_hashtable_destroy(f_cloned->table);

			error = maps_item_hashtable_clone(f->table, &f_cloned->table);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	//LCOV_EXCL_START
	maps_place_filter_destroy(*cloned);
	*cloned = NULL;
	return error;
	//LCOV_EXCL_STOP
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_filter_get(const maps_place_filter_h filter,
								const char *key, char **value)
{
	if (!filter)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_string(((maps_place_filter_s *) filter)->
		table, key, value);
}

EXPORT_API int maps_place_filter_foreach_property(const maps_place_filter_h filter,
								maps_place_filter_properties_cb callback,
								void *user_data)
{
	if (!filter || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_foreach(((maps_place_filter_s *) filter)->
		table, callback, user_data);
}

EXPORT_API int maps_place_filter_get_keyword(const maps_place_filter_h filter,
								char **keyword)
{
	if (!filter || !keyword)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_string(((maps_place_filter_s *) filter)->
		table, "MAPS_PLACE_FILTER_KEYWORD", keyword);
}

EXPORT_API int maps_place_filter_get_place_name(const maps_place_filter_h filter,
								char **place_name)
{
	if (!filter || !place_name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_string(((maps_place_filter_s *) filter)->
		table, "MAPS_PLACE_FILTER_PLACE_NAME", place_name);
}

EXPORT_API int maps_place_filter_get_category(const maps_place_filter_h filter,
								maps_place_category_h *category)
{
	if (!filter || !category)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get(((maps_place_filter_s *) filter)->table,
		"MAPS_PLACE_FILTER_CATEGORY", (void **) category);
}

EXPORT_API int maps_place_filter_get_place_address(const maps_place_filter_h filter,
								char **place_address)
{
	if (!filter || !place_address)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_string(((maps_place_filter_s *) filter)->
		table, "MAPS_PLACE_FILTER_PLACE_ADDRESS", place_address);
}

/*----------------------------------------------------------------------------*/
//LCOV_EXCL_START
EXPORT_API int maps_place_filter_set(maps_place_filter_h filter,
								const char *key, const char *value)
{
	if (!filter)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_string(((maps_place_filter_s *) filter)->
		table, key, value);
}

EXPORT_API int maps_place_filter_set_keyword(maps_place_filter_h filter,
								const char *keyword)
{
	if (!filter || !keyword)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_string(((maps_place_filter_s *) filter)->
		table, "MAPS_PLACE_FILTER_KEYWORD", keyword);
}

EXPORT_API int maps_place_filter_set_place_name(maps_place_filter_h filter,
								const char *place_name)
{
	if (!filter || !place_name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_string(((maps_place_filter_s *) filter)->
		table, "MAPS_PLACE_FILTER_PLACE_NAME", place_name);
}

EXPORT_API int maps_place_filter_set_category(maps_place_filter_h filter,
								const maps_place_category_h category)
{
	if (!filter || !category)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set(((maps_place_filter_s *) filter)->table,
		"MAPS_PLACE_FILTER_CATEGORY", (void **) category,
		maps_place_category_clone, maps_place_category_destroy);
}

EXPORT_API int maps_place_filter_set_place_address(maps_place_filter_h filter,
								const char *place_address)
{
	if (!filter || !place_address)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_string(((maps_place_filter_s *) filter)->
		table, "MAPS_PLACE_FILTER_PLACE_ADDRESS", place_address);
}
//LCOV_EXCL_STOP
