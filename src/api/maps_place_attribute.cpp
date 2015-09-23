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
#include "maps_place_attribute_plugin.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_attribute_s
{
	char *id;
	char *label;
	char *text;
} maps_place_attribute_s;

const gsize _MAPS_PLACE_ATTRIBUTE_ID_MAX_LENGTH = 32;
const gsize _MAPS_PLACE_ATTRIBUTE_LABEL_MAX_LENGTH = 64;
const gsize _MAPS_PLACE_ATTRIBUTE_TEXT_MAX_LENGTH = 512;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_attribute_create(maps_place_attribute_h *place)
{
	MAPS_LOG_API;
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_attribute_h) g_slice_new0(maps_place_attribute_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_attribute_destroy(maps_place_attribute_h place)
{
	MAPS_LOG_API;
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_attribute_s *a = (maps_place_attribute_s *) place;

	if (a->id)
		g_free(a->id);
	if (a->label)
		g_free(a->label);
	if (a->text)
		g_free(a->text);

	g_slice_free(maps_place_attribute_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_attribute_clone(const maps_place_attribute_h origin,
					  maps_place_attribute_h *cloned)
{
	MAPS_LOG_API;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_attribute_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_attribute_s *a = (maps_place_attribute_s *) origin;

		if (a->id) {
			error = maps_place_attribute_set_id(*cloned, a->id);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->label) {
			error = maps_place_attribute_set_label(*cloned,
				a->label);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->text) {
			error = maps_place_attribute_set_text(*cloned, a->text);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	maps_place_attribute_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_attribute_get_id(const maps_place_attribute_h place,
					   char **id)
{
	MAPS_LOG_API;
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_attribute_s *) place)->id,
		_MAPS_PLACE_ATTRIBUTE_ID_MAX_LENGTH, id);
}

EXPORT_API int maps_place_attribute_get_text(const maps_place_attribute_h place,
					     char **text)
{
	MAPS_LOG_API;
	if (!place || !text)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_attribute_s *) place)->text,
		_MAPS_PLACE_ATTRIBUTE_TEXT_MAX_LENGTH, text);
}

EXPORT_API int maps_place_attribute_get_label(const maps_place_attribute_h
					      place, char **label)
{
	MAPS_LOG_API;
	if (!place || !label)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_attribute_s *) place)->label,
		_MAPS_PLACE_ATTRIBUTE_LABEL_MAX_LENGTH, label);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_attribute_set_id(maps_place_attribute_h place,
					   const char * id)
{
	MAPS_LOG_API;
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(id, _MAPS_PLACE_ATTRIBUTE_ID_MAX_LENGTH,
		&((maps_place_attribute_s *) place)->id);
}

EXPORT_API int maps_place_attribute_set_label(maps_place_attribute_h place,
					      const char *label)
{
	MAPS_LOG_API;
	if (!place || !label)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(label, _MAPS_PLACE_ATTRIBUTE_LABEL_MAX_LENGTH,
		&((maps_place_attribute_s *) place)->label);
}

EXPORT_API int maps_place_attribute_set_text(maps_place_attribute_h place,
					     const char *text)
{
	MAPS_LOG_API;
	if (!place || !text)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(text, _MAPS_PLACE_ATTRIBUTE_TEXT_MAX_LENGTH,
		&((maps_place_attribute_s *) place)->text);
}
