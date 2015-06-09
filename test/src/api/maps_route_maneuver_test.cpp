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

#include "maps_route_maneuver_test.h"
#include "maps_route_maneuver_plugin.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_route_maneuver_create(maps_route_maneuver_h* maneuver); */
/* int maps_route_maneuver_destroy(maps_route_maneuver_h maneuver); */
void utc_maps_route_maneuver_create_p(void)
{
	maps_route_maneuver_h maneuver = NULL;
	int error = maps_route_maneuver_create(&maneuver);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(maneuver);

	error = maps_route_maneuver_destroy(maneuver);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_maneuver_create_n(void)
{
	int error = maps_route_maneuver_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_route_maneuver_h h;
 public:
	test_env():h(NULL)
	{
		const int error = maps_route_maneuver_create(&h);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(h);
	}
	~test_env()
	{
		const int error = maps_route_maneuver_destroy(h);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_route_maneuver_clone(const maps_route_maneuver_h origin,
*  maps_route_maneuver_h* cloned); */
void utc_maps_route_maneuver_clone_p(void)
{
	test_env e;

	/* set test values for original */
	/*------------------------------------ */
	int error =
		maps_route_maneuver_set_direction_id(e.h,
		MAPS_ROUTE_DIRECTION_WEST);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_maneuver_set_turn_type(e.h,
		MAPS_ROUTE_TURN_TYPE_STRAIGHT);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_maneuver_set_road_name(e.h, "road_name");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_maneuver_set_instruction_text(e.h,
		"instruction_text");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_maneuver_set_locale(e.h, "locale");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h coordinates = NULL;
	error = (maps_error_e) maps_coordinates_create(11.1, 22.2,
		&coordinates);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_route_maneuver_set_position(e.h, coordinates);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_coordinates_destroy(coordinates);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_maneuver_set_time_to_next_instruction(e.h, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_maneuver_set_distance_to_next_instruction(e.h,
		6.0221);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_route_maneuver_h cloned = NULL;
	error = maps_route_maneuver_clone(e.h, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	maps_route_direction_e direction_id = MAPS_ROUTE_DIRECTION_NONE;
	error = maps_route_maneuver_get_direction_id(cloned, &direction_id);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(direction_id, ==, MAPS_ROUTE_DIRECTION_WEST);

	maps_route_turn_type_e turn_type = MAPS_ROUTE_TURN_TYPE_NONE;
	error = maps_route_maneuver_get_turn_type(cloned, &turn_type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(turn_type, ==, MAPS_ROUTE_TURN_TYPE_STRAIGHT);

	maps_coordinates_h position_obtained = NULL;
	error = maps_route_maneuver_get_position(cloned, &position_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(position_obtained);
	maps_coordinates_destroy(position_obtained);

	char* road_name = NULL;
	error = maps_route_maneuver_get_road_name(cloned, &road_name);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(road_name, ==, "road_name");
	g_free(road_name);

	char* instruction_text = NULL;
	error = maps_route_maneuver_get_instruction_text(cloned,
		&instruction_text);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(instruction_text, ==, "instruction_text");
	g_free(instruction_text);

	char* locale = NULL;
	error = maps_route_maneuver_get_locale(cloned, &locale);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(locale, ==, "locale");
	g_free(locale);

	int time_to_next_instruction = 0;
	error = maps_route_maneuver_get_time_to_next_instruction(cloned,
		&time_to_next_instruction);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(time_to_next_instruction, ==, 42);

	double distance_to_next_instruction = .0;
	error = maps_route_maneuver_get_distance_to_next_instruction(cloned,
		&distance_to_next_instruction);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(distance_to_next_instruction, >=, 6.0220);
	g_assert_cmpfloat(distance_to_next_instruction, <=, 6.0222);
	/*------------------------------------ */

	error = maps_route_maneuver_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_maneuver_clone_n(void)
{
	test_env e;

	maps_route_maneuver_h cloned = NULL;
	int error = maps_route_maneuver_clone(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_maneuver_get_direction_id(
*  const maps_route_maneuver_h maneuver,
*  maps_route_direction_e* direction_id); */
/* int maps_route_maneuver_set_direction_id(maps_route_maneuver_h maneuver,
*  const maps_route_direction_e direction_id); */
void utc_maps_route_maneuver_direction_id_p(void)
{
	test_env e;

	int error =
		maps_route_maneuver_set_direction_id(e.h,
		MAPS_ROUTE_DIRECTION_WEST);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_direction_e ret = MAPS_ROUTE_DIRECTION_NONE;
	error = maps_route_maneuver_get_direction_id(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(ret, ==, MAPS_ROUTE_DIRECTION_WEST);
}

void utc_maps_route_maneuver_direction_id_n(void)
{
	test_env e;

	int error =
		maps_route_maneuver_set_direction_id(NULL,
		MAPS_ROUTE_DIRECTION_WEST);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_route_direction_e ret = MAPS_ROUTE_DIRECTION_NONE;
	error = maps_route_maneuver_get_direction_id(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_get_direction_id(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_maneuver_get_turn_type(const maps_route_maneuver_h maneuver,
*  maps_route_turn_type_e* turn_type); */
/* int maps_route_maneuver_set_turn_type(maps_route_maneuver_h maneuver,
*  const maps_route_turn_type_e turn_type); */
void utc_maps_route_maneuver_turn_type_p(void)
{
	test_env e;

	int error =
		maps_route_maneuver_set_turn_type(e.h,
		MAPS_ROUTE_TURN_TYPE_STRAIGHT);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_turn_type_e ret = MAPS_ROUTE_TURN_TYPE_NONE;
	error = maps_route_maneuver_get_turn_type(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(ret, ==, MAPS_ROUTE_TURN_TYPE_STRAIGHT);
}

void utc_maps_route_maneuver_turn_type_n(void)
{
	test_env e;

	int error =
		maps_route_maneuver_set_turn_type(NULL,
		MAPS_ROUTE_TURN_TYPE_STRAIGHT);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_route_turn_type_e ret = MAPS_ROUTE_TURN_TYPE_NONE;
	error = maps_route_maneuver_get_turn_type(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_get_turn_type(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_maneuver_get_position(maps_route_maneuver_h maneuver,
*  maps_coordinates_h* position); */
/* int maps_route_maneuver_set_position(maps_route_maneuver_h maneuver,
*  const maps_coordinates_h position); */
void utc_maps_route_maneuver_position_p(void)
{
	test_env e;

	maps_coordinates_h obj = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ---------------------------------------------------------- */
	error = maps_route_maneuver_set_position(e.h, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obtained_obj = NULL;
	error = maps_route_maneuver_get_position(e.h, &obtained_obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obtained_obj);
	maps_coordinates_destroy(obtained_obj);
	/* finish test ---------------------------------------------------------- */

	error = maps_coordinates_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_maneuver_position_n(void)
{
	test_env e;

	maps_coordinates_h obj = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ---------------------------------------------------------- */
	error = maps_route_maneuver_set_position(NULL, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_set_position(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_coordinates_h obtained_obj = NULL;
	error = maps_route_maneuver_get_position(NULL, &obtained_obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_get_position(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* finish test ---------------------------------------------------------- */

	error = maps_coordinates_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_route_maneuver_get_road_name(maps_route_maneuver_h maneuver,
*  char** road_name); */
/* int maps_route_maneuver_set_road_name(maps_route_maneuver_h maneuver,
*  const char* road_name); */
void utc_maps_route_maneuver_road_name_p(void)
{
	test_env e;

	int error = maps_route_maneuver_set_road_name(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* ret = NULL;
	error = maps_route_maneuver_get_road_name(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(ret, ==, "test_string");
	g_free(ret);
}

void utc_maps_route_maneuver_road_name_n(void)
{
	test_env e;

	int error = maps_route_maneuver_set_road_name(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_set_road_name(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* ret = NULL;
	error = maps_route_maneuver_get_road_name(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_get_road_name(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_maneuver_get_locale(const maps_route_maneuver_h maneuver,
*  char** locale); */
/* int maps_route_maneuver_set_locale(maps_route_maneuver_h maneuver,
*  const char* locale); */
void utc_maps_route_maneuver_locale_p(void)
{
	test_env e;

	int error = maps_route_maneuver_set_locale(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* ret = NULL;
	error = maps_route_maneuver_get_locale(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(ret, ==, "test_string");
	g_free(ret);
}

void utc_maps_route_maneuver_locale_n(void)
{
	test_env e;

	int error = maps_route_maneuver_set_locale(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_set_locale(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* ret = NULL;
	error = maps_route_maneuver_get_locale(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_get_locale(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_maneuver_get_time_to_next_instruction(
*  const maps_route_maneuver_h maneuver, int* time_to_next_instruction); */
/* int maps_route_maneuver_set_time_to_next_instruction(
*  maps_route_maneuver_h maneuver, int time_to_next_instruction); */
void utc_maps_route_maneuver_time_to_next_instruction_p(void)
{
	test_env e;

	int error = maps_route_maneuver_set_time_to_next_instruction(e.h, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	int ret = 0;
	error = maps_route_maneuver_get_time_to_next_instruction(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(ret, ==, 42);
}

void utc_maps_route_maneuver_time_to_next_instruction_n(void)
{
	test_env e;

	int error = maps_route_maneuver_set_time_to_next_instruction(NULL, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	int ret = 0;
	error = maps_route_maneuver_get_time_to_next_instruction(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_get_time_to_next_instruction(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_maneuver_get_distance_to_next_instruction(
*  const maps_route_maneuver_h maneuver,
*  double* distance_to_next_instruction); */
/* int maps_route_maneuver_set_distance_to_next_instruction(
*  maps_route_maneuver_h maneuver, double distance_to_next_instruction); */
void utc_maps_route_maneuver_distance_to_next_instruction_p(void)
{
	test_env e;

	int error =
		maps_route_maneuver_set_distance_to_next_instruction(e.h, 42.1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	double ret = 0;
	error = maps_route_maneuver_get_distance_to_next_instruction(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(ret, >=, 42.0);
	g_assert_cmpfloat(ret, <=, 42.2);
}

void utc_maps_route_maneuver_distance_to_next_instruction_n(void)
{
	test_env e;

	int error =
		maps_route_maneuver_set_distance_to_next_instruction(NULL, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	double ret = 0;
	error = maps_route_maneuver_get_distance_to_next_instruction(NULL,
		&ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_get_distance_to_next_instruction(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_maneuver_get_instruction_text(maps_route_maneuver_h maneuver,
*  char** instruction_text); */
/* int maps_route_maneuver_set_instruction_text(maps_route_maneuver_h maneuver,
*  const char* instruction_text); */
void utc_maps_route_maneuver_instruction_text_p(void)
{
	test_env e;

	int error =
		maps_route_maneuver_set_instruction_text(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* ret = NULL;
	error = maps_route_maneuver_get_instruction_text(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(ret, ==, "test_string");
	g_free(ret);
}

void utc_maps_route_maneuver_instruction_text_n(void)
{
	test_env e;

	int error =
		maps_route_maneuver_set_instruction_text(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_set_instruction_text(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* ret = NULL;
	error = maps_route_maneuver_get_instruction_text(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_maneuver_get_instruction_text(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

