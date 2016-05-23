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
#include "maps_place_link_object_plugin.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_link_object_s
{
	char *id;
	char *name;
	char *type;
	char *string;
} maps_place_link_object_s;

const gsize _MAPS_PLACE_LINK_OBJECT_ID_MAX_LENGTH = 32;
const gsize _MAPS_PLACE_LINK_OBJECT_NAME_MAX_LENGTH = 64;
const gsize _MAPS_PLACE_LINK_OBJECT_TYPE_MAX_LENGTH = 32;
const gsize _MAPS_PLACE_LINK_OBJECT_STRING_MAX_LENGTH = 256;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_link_object_create(maps_place_link_object_h *place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_link_object_h)
		g_slice_new0(maps_place_link_object_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_link_object_destroy(maps_place_link_object_h place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_link_object_s *lo = (maps_place_link_object_s *) place;

	if (lo->id)
		g_free(lo->id);
	if (lo->name)
		g_free(lo->name);
	if (lo->type)
		g_free(lo->type);
	if (lo->string)
		g_free(lo->string);

	g_slice_free(maps_place_link_object_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_link_object_clone(const maps_place_link_object_h
					    origin, maps_place_link_object_h *
					    cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_link_object_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_link_object_s *lo =
			(maps_place_link_object_s *) origin;

		if (lo->id) {
			error = maps_place_link_object_set_id(*cloned, lo->id);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (lo->name) {
			error = maps_place_link_object_set_name(*cloned,
				lo->name);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (lo->type) {
			error = maps_place_link_object_set_type(*cloned,
				lo->type);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (lo->string) {
			error = maps_place_link_object_set_string(*cloned,
				lo->string);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	maps_place_link_object_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_link_object_get_string(const maps_place_link_object_h place,
						 char **string)
{
	if (!place || !string)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_link_object_s *) place)->string,
		_MAPS_PLACE_LINK_OBJECT_STRING_MAX_LENGTH, string);
}

EXPORT_API int maps_place_link_object_get_type(const maps_place_link_object_h place,
					       char **type)
{
	if (!place || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_link_object_s *) place)->type,
		_MAPS_PLACE_LINK_OBJECT_TYPE_MAX_LENGTH, type);
}

EXPORT_API int maps_place_link_object_get_id(const maps_place_link_object_h place,
					     char **id)
{
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_link_object_s *) place)->id,
		_MAPS_PLACE_LINK_OBJECT_ID_MAX_LENGTH, id);
}

EXPORT_API int maps_place_link_object_get_name(const maps_place_link_object_h place,
					     char **name)
{
	if (!place || !name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_link_object_s *) place)->name,
		_MAPS_PLACE_LINK_OBJECT_NAME_MAX_LENGTH, name);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_link_object_set_id(maps_place_link_object_h place,
					     const char *id)
{
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(id, _MAPS_PLACE_LINK_OBJECT_ID_MAX_LENGTH,
		&((maps_place_link_object_s *) place)->id);
}

EXPORT_API int maps_place_link_object_set_string(maps_place_link_object_h place,
						 const char *string)
{
	if (!place || !string)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(string,
		_MAPS_PLACE_LINK_OBJECT_STRING_MAX_LENGTH,
		&((maps_place_link_object_s *) place)->string);
}

EXPORT_API int maps_place_link_object_set_type(maps_place_link_object_h place,
					       const char *type)
{
	if (!place || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(type, _MAPS_PLACE_LINK_OBJECT_TYPE_MAX_LENGTH,
		&((maps_place_link_object_s *) place)->type);
}

EXPORT_API int maps_place_link_object_set_name(maps_place_link_object_h place,
					       const char *name)
{
	if (!place || !name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(name, _MAPS_PLACE_LINK_OBJECT_NAME_MAX_LENGTH,
		&((maps_place_link_object_s *) place)->name);
}
