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
#include "maps_place_contact_plugin.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_contact_s
{
	char *label;
	char *type;
	char * value;
} maps_place_contact_s;

const gsize _MAPS_PLACE_CONTACT_LABEL_MAX_LENGTH = 64;
const gsize _MAPS_PLACE_CONTACT_TYPE_MAX_LENGTH = 32;
const gsize _MAPS_PLACE_CONTACT_VALUE_MAX_LENGTH = 512;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_contact_create(maps_place_contact_h *place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_contact_h) g_slice_new0(maps_place_contact_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_contact_destroy(maps_place_contact_h place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_contact_s *c = (maps_place_contact_s *) place;

	if (c->label)
		g_free(c->label);
	if (c->type)
		g_free(c->type);
	if (c->value)
		g_free(c->value);

	g_slice_free(maps_place_contact_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_contact_clone(const maps_place_contact_h origin,
					maps_place_contact_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_contact_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_contact_s *c = (maps_place_contact_s *) origin;

		if (c->label) {
			error = maps_place_contact_set_label(*cloned, c->label);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (c->type) {
			error = maps_place_contact_set_type(*cloned, c->type);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (c->value) {
			error = maps_place_contact_set_value(*cloned, c->value);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	maps_place_contact_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_contact_get_label(const maps_place_contact_h place, char **label)
{
	if (!place || !label)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_contact_s *) place)->label,
		_MAPS_PLACE_CONTACT_LABEL_MAX_LENGTH, label);
}

EXPORT_API int maps_place_contact_get_type(const maps_place_contact_h place, char **type)
{
	if (!place || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_contact_s *) place)->type,
		_MAPS_PLACE_CONTACT_TYPE_MAX_LENGTH, type);
}

EXPORT_API int maps_place_contact_get_value(const maps_place_contact_h place, char **value)
{
	if (!place || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_contact_s *) place)->value,
		_MAPS_PLACE_CONTACT_VALUE_MAX_LENGTH, value);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_contact_set_label(maps_place_contact_h place, const char *label)
{
	if (!place || !label)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(label, _MAPS_PLACE_CONTACT_LABEL_MAX_LENGTH,
		&((maps_place_contact_s *) place)->label);
}

EXPORT_API int maps_place_contact_set_type(maps_place_contact_h place, const char *type)
{
	if (!place || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(type, _MAPS_PLACE_CONTACT_TYPE_MAX_LENGTH,
		&((maps_place_contact_s *) place)->type);
}

EXPORT_API int maps_place_contact_set_value(maps_place_contact_h place, const char *value)
{
	if (!place || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(value, _MAPS_PLACE_CONTACT_VALUE_MAX_LENGTH,
		&((maps_place_contact_s *) place)->value);
}