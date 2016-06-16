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
#include "maps_place_editorial_plugin.h"
#include "maps_place_media_plugin.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_editorial_s
{
	char *description;
	char *language;
	maps_place_media_h media;
} maps_place_editorial_s;

const gsize _MAPS_PLACE_EDITORIAL_DESCRIPTION_MAX_LENGTH = 64;
const gsize _MAPS_PLACE_EDITORIAL_LANGUAGE_MAX_LENGTH = 32;

/*----------------------------------------------------------------------------*/
//LCOV_EXCL_START
EXPORT_API int maps_place_editorial_create(maps_place_editorial_h *place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_editorial_h) g_slice_new0(maps_place_editorial_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}
//LCOV_EXCL_STOP

EXPORT_API int maps_place_editorial_destroy(maps_place_editorial_h place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_editorial_s *e = (maps_place_editorial_s *) place;

	if (e->description)
		g_free(e->description);
	if (e->language)
		g_free(e->language);

	if (e->media)
		maps_place_media_destroy(e->media);

	g_slice_free(maps_place_editorial_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_editorial_clone(const maps_place_editorial_h origin,
								maps_place_editorial_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_editorial_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_editorial_s *e = (maps_place_editorial_s *) origin;

		if (e->description) {
			error = maps_place_editorial_set_description(*cloned, e->description);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (e->language) {
			error = maps_place_editorial_set_language(*cloned, e->language);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (e->media) {
			maps_place_editorial_s *e_cloned =
				(maps_place_editorial_s *) (*cloned);
			error = maps_place_media_clone(e->media, &e_cloned->media);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	//LCOV_EXCL_START
	maps_place_editorial_destroy(*cloned);
	*cloned = NULL;
	return error;
	//LCOV_EXCL_STOP
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_editorial_get_description(const maps_place_editorial_h place,
								char **description)
{
	if (!place || !description)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_editorial_s *) place)->description,
		_MAPS_PLACE_EDITORIAL_DESCRIPTION_MAX_LENGTH, description);
}

EXPORT_API int maps_place_editorial_get_language(const maps_place_editorial_h place,
								char **language)
{
	if (!place || !language)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_editorial_s *) place)->language,
		_MAPS_PLACE_EDITORIAL_LANGUAGE_MAX_LENGTH, language);
}

EXPORT_API int maps_place_editorial_get_media(const maps_place_editorial_h place,
								maps_place_media_h *media)
{
	if (!place || !media)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_place_media_clone(((maps_place_editorial_s *) place)->media, media);
}

/*----------------------------------------------------------------------------*/

//LCOV_EXCL_START
EXPORT_API int maps_place_editorial_set_description(maps_place_editorial_h place,
								const char *description)
{
	if (!place || !description)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(description,
		_MAPS_PLACE_EDITORIAL_DESCRIPTION_MAX_LENGTH,
		&((maps_place_editorial_s *) place)->description);
}

EXPORT_API int maps_place_editorial_set_language(maps_place_editorial_h place,
								const char *language)
{
	if (!place || !language)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(language,
		_MAPS_PLACE_EDITORIAL_LANGUAGE_MAX_LENGTH,
		&((maps_place_editorial_s *) place)->language);
}

EXPORT_API int maps_place_editorial_set_media(maps_place_editorial_h place,
								const maps_place_media_h media)
{
	if (!place || !media)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_editorial_s *e = (maps_place_editorial_s *) place;
	if (e->media)
		maps_place_media_destroy(e->media);
	return maps_place_media_clone(media, &e->media);
}
//LCOV_EXCL_STOP
