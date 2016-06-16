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
#include "maps_place_url_plugin.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_url_s
{
	char *path;
	char *desc;
} maps_place_url_s;

const gsize _MAPS_PLACE_URL_PATH_MAX_LENGTH = 264;
const gsize _MAPS_PLACE_URL_DESC_MAX_LENGTH = 512;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_url_create(maps_place_url_h *place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_url_h) g_slice_new0(maps_place_url_s);

	if (*place == NULL) {
//LCOV_EXCL_START
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
//LCOV_EXCL_STOP
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_url_destroy(maps_place_url_h place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_url_s *u = (maps_place_url_s *) place;

	if (u->path)
		g_free(u->path);
	if (u->desc)
		g_free(u->desc);

	g_slice_free(maps_place_url_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_url_clone(const maps_place_url_h origin,
								maps_place_url_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_url_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_url_s* u = (maps_place_url_s*) origin;

		if (u->path) {
			error = maps_place_url_set_path(*cloned, u->path);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (u->desc) {
			error = maps_place_url_set_description(*cloned,
				u->desc);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

//LCOV_EXCL_START
	maps_place_url_destroy(*cloned);
	*cloned = NULL;
	return error;
//LCOV_EXCL_STOP
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_url_get_path(const maps_place_url_h place, char **path)
{
	if (!place || !path)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_url_s *) place)->path,
		_MAPS_PLACE_URL_PATH_MAX_LENGTH, path);
}

EXPORT_API int maps_place_url_get_description(const maps_place_url_h place, char **desc)
{
	if (!place || !desc)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_url_s *) place)->desc,
		_MAPS_PLACE_URL_DESC_MAX_LENGTH, desc);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_url_set_path(maps_place_url_h place, const char *path)
{
	if (!place || !path)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(path, _MAPS_PLACE_URL_PATH_MAX_LENGTH,
		&((maps_place_url_s *) place)->path);
}

EXPORT_API int maps_place_url_set_description(maps_place_url_h place,
								const char *desc)
{
	if (!place || !desc)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(desc, _MAPS_PLACE_URL_DESC_MAX_LENGTH,
		&((maps_place_url_s *) place)->desc);
}
