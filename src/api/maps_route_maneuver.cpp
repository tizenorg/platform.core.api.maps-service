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

#include "maps_error.h"
#include "maps_route_maneuver_plugin.h"
#include "maps_extra_types.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_route_maneuver_s
{
	maps_route_direction_e direction_id;
	maps_route_turn_type_e turn_type;
	maps_coordinates_h position;
	char *instruction_text;
	char *road_name;
	char *locale;		/* locale of instruction_text; */
	int time_to_next_instruction;	/* duration */
	double distance_to_next_instruction;	/* distance */
} maps_route_maneuver_s;

const gsize _MAPS_ROUTE_MANEUVER_ROAD_NAME_MAX_LENGTH = 128;
const gsize _MAPS_ROUTE_MANEUVER_INSTRUCTION_TEXT_MAX_LENGTH = 256;
const gsize _MAPS_ROUTE_MANEUVER_LOCALE_MAX_LENGTH = 32;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_route_maneuver_create(maps_route_maneuver_h *maneuver)
{
	if (!maneuver)
		return MAPS_ERROR_INVALID_PARAMETER;
	*maneuver = (maps_route_maneuver_h) g_slice_new0(maps_route_maneuver_s);

	if (*maneuver == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_maneuver_destroy(maps_route_maneuver_h maneuver)
{
	if (!maneuver)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_route_maneuver_s *p = (maps_route_maneuver_s *) maneuver;

	if (p->position)
		maps_coordinates_destroy(p->position);
	if (p->instruction_text)
		g_free(p->instruction_text);
	if (p->road_name)
		g_free(p->road_name);
	if (p->locale)
		g_free(p->locale);

	g_slice_free(maps_route_maneuver_s, maneuver);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_maneuver_clone(const maps_route_maneuver_h origin,
					 maps_route_maneuver_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_route_maneuver_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_route_maneuver_s *p = (maps_route_maneuver_s *) origin;

		error = maps_route_maneuver_set_direction_id(*cloned,
			p->direction_id);
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_route_maneuver_set_turn_type(*cloned,
			p->turn_type);
		if (error != MAPS_ERROR_NONE)
			break;

		if (p->position) {
			error = maps_route_maneuver_set_position(*cloned,
				p->position);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->instruction_text) {
			error = maps_route_maneuver_set_instruction_text
				(*cloned, p->instruction_text);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->road_name) {
			error = maps_route_maneuver_set_road_name(*cloned,
				p->road_name);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->locale) {
			error = maps_route_maneuver_set_locale(*cloned,
				p->locale);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		error = maps_route_maneuver_set_time_to_next_instruction
			(*cloned, p->time_to_next_instruction);
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_route_maneuver_set_distance_to_next_instruction
			(*cloned, p->distance_to_next_instruction);
		if (error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while (false);

	maps_route_maneuver_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_route_maneuver_get_direction_id(const maps_route_maneuver_h
						    maneuver,
						    maps_route_direction_e *
						    direction_id)
{
	if (!maneuver || !direction_id)
		return MAPS_ERROR_INVALID_PARAMETER;
	*direction_id = ((maps_route_maneuver_s *) maneuver)->direction_id;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_maneuver_get_turn_type(const maps_route_maneuver_h
						 maneuver,
						 maps_route_turn_type_e *
						 turn_type)
{
	if (!maneuver || !turn_type)
		return MAPS_ERROR_INVALID_PARAMETER;
	*turn_type = ((maps_route_maneuver_s *) maneuver)->turn_type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_maneuver_get_position(const maps_route_maneuver_h
						maneuver,
						maps_coordinates_h * position)
{
	if (!maneuver || !position)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_coordinates_clone(((maps_route_maneuver_s *) maneuver)->
		position, position);
}

EXPORT_API int maps_route_maneuver_get_road_name(const maps_route_maneuver_h
						 maneuver,
						 char ** road_name)
{
	if (!maneuver || !road_name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_route_maneuver_s *) maneuver)->road_name,
		_MAPS_ROUTE_MANEUVER_ROAD_NAME_MAX_LENGTH, road_name);
}

EXPORT_API int maps_route_maneuver_get_instruction_text(const
							maps_route_maneuver_h
							maneuver,
							char **
							instruction_text)
{
	if (!maneuver || !instruction_text)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_route_maneuver_s *) maneuver)->
		instruction_text,
		_MAPS_ROUTE_MANEUVER_INSTRUCTION_TEXT_MAX_LENGTH,
		instruction_text);
}

EXPORT_API int maps_route_maneuver_get_locale(const maps_route_maneuver_h
					      maneuver,
					      char **locale)
{
	if (!maneuver || !locale)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_route_maneuver_s *) maneuver)->locale,
		_MAPS_ROUTE_MANEUVER_LOCALE_MAX_LENGTH, locale);
}

EXPORT_API int maps_route_maneuver_get_time_to_next_instruction(const
							maps_route_maneuver_h
							maneuver,
						int *time_to_next_instruction)
{
	if (!maneuver || !time_to_next_instruction)
		return MAPS_ERROR_INVALID_PARAMETER;
	*time_to_next_instruction =
		((maps_route_maneuver_s *) maneuver)->time_to_next_instruction;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_maneuver_get_distance_to_next_instruction(const
					maps_route_maneuver_h maneuver,
					double *distance_to_next_instruction)
{
	if (!maneuver || !distance_to_next_instruction)
		return MAPS_ERROR_INVALID_PARAMETER;
	*distance_to_next_instruction =
		((maps_route_maneuver_s *) maneuver)->
		distance_to_next_instruction;
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_route_maneuver_set_direction_id(maps_route_maneuver_h
						    maneuver,
						    const
						    maps_route_direction_e
						    direction_id)
{
	if (!maneuver)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_maneuver_s *) maneuver)->direction_id = direction_id;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_maneuver_set_turn_type(maps_route_maneuver_h maneuver,
						 const maps_route_turn_type_e
						 turn_type)
{
	if (!maneuver)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_maneuver_s *) maneuver)->turn_type = turn_type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_maneuver_set_position(maps_route_maneuver_h maneuver,
						const maps_coordinates_h
						position)
{
	if (!maneuver || !position)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_maneuver_s *p = (maps_route_maneuver_s *) maneuver;
	if (p->position)
		maps_coordinates_destroy(p->position);
	return maps_coordinates_clone(position, &p->position);
}

EXPORT_API int maps_route_maneuver_set_road_name(maps_route_maneuver_h maneuver,
						 const char *road_name)
{
	if (!maneuver || !road_name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(road_name,
		_MAPS_ROUTE_MANEUVER_ROAD_NAME_MAX_LENGTH,
		&((maps_route_maneuver_s *) maneuver)->road_name);
}

EXPORT_API int maps_route_maneuver_set_instruction_text(maps_route_maneuver_h
							maneuver,
							const char *
							instruction_text)
{
	if (!maneuver || !instruction_text)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(instruction_text,
		_MAPS_ROUTE_MANEUVER_INSTRUCTION_TEXT_MAX_LENGTH,
		&((maps_route_maneuver_s *) maneuver)->instruction_text);
}

EXPORT_API int maps_route_maneuver_set_locale(maps_route_maneuver_h maneuver,
					      const char *locale)
{
	if (!maneuver || !locale)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_route_maneuver_s *p = (maps_route_maneuver_s *) maneuver;
	return maps_set_string(locale, _MAPS_ROUTE_MANEUVER_LOCALE_MAX_LENGTH,
		&p->locale);
}

EXPORT_API int maps_route_maneuver_set_time_to_next_instruction(
					maps_route_maneuver_h maneuver,
					const int time_to_next_instruction)
{
	if (!maneuver)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_maneuver_s *) maneuver)->time_to_next_instruction =
		time_to_next_instruction;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_route_maneuver_set_distance_to_next_instruction(
				maps_route_maneuver_h maneuver,
				const double distance_to_next_instruction)
{
	if (!maneuver)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_route_maneuver_s *) maneuver)->distance_to_next_instruction =
		distance_to_next_instruction;
	return MAPS_ERROR_NONE;
}
