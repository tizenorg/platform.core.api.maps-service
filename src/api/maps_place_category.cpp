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

#include "maps_error.h"
#include "maps_extra_types.h"
#include <glib.h>
#include "maps_util.h"
#include "maps_place_category.h"

typedef struct _maps_place_category_s
{
	char *name;
	char *id;
	char *url;
} maps_place_category_s;

const gsize _MAPS_PLACE_CATEGORY_ID_MAX_LENGTH = 32;
const gsize _MAPS_PLACE_CATEGORY_NAME_MAX_LENGTH = 64;
const gsize _MAPS_PLACE_CATEGORY_URL_MAX_LENGTH = 128;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_category_create(maps_place_category_h *place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = g_slice_new0(maps_place_category_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_category_destroy(maps_place_category_h place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	int error = MAPS_ERROR_NONE;

	maps_place_category_s *c = (maps_place_category_s *) place;

	if (c->id)
		g_free(c->id);
	if (c->name)
		g_free(c->name);
	if (c->url)
		g_free(c->url);

	g_slice_free(maps_place_category_s, place);
	return error;
}

EXPORT_API int maps_place_category_clone(const maps_place_category_h origin,
					 maps_place_category_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;

	do {
		error = maps_place_category_create(cloned);
		if (error != MAPS_ERROR_NONE)
			return error;

		maps_place_category_s *c = (maps_place_category_s *) origin;

		if (c->id) {
			error = maps_place_category_set_id(*cloned, c->id);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (c->name) {
			error = maps_place_category_set_name(*cloned, c->name);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (c->url) {
			error = maps_place_category_set_url(*cloned, c->url);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;

	} while (false);

	maps_place_category_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_category_get_name(const maps_place_category_h place,
					    char **name)
{
	if (!place || !name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_category_s *) place)->name,
		_MAPS_PLACE_CATEGORY_NAME_MAX_LENGTH, name);
}

EXPORT_API int maps_place_category_get_id(const maps_place_category_h place,
					  char **id)
{
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_category_s *) place)->id,
		_MAPS_PLACE_CATEGORY_ID_MAX_LENGTH, id);
}

EXPORT_API int maps_place_category_get_url(const maps_place_category_h place,
					   char **url)
{
	if (!place || !url)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_category_s *) place)->url,
		_MAPS_PLACE_CATEGORY_URL_MAX_LENGTH, url);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_category_set_id(maps_place_category_h place,
					  const char *id)
{
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(id, _MAPS_PLACE_CATEGORY_ID_MAX_LENGTH,
		&((maps_place_category_s *) place)->id);
}

EXPORT_API int maps_place_category_set_name(maps_place_category_h place,
					    const char *name)
{
	if (!place || !name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(name, _MAPS_PLACE_CATEGORY_NAME_MAX_LENGTH,
		&((maps_place_category_s *) place)->name);
}

EXPORT_API int maps_place_category_set_url(maps_place_category_h place,
					   const char *url)
{
	if (!place || !url)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(url, _MAPS_PLACE_CATEGORY_URL_MAX_LENGTH,
		&((maps_place_category_s *) place)->url);
}
