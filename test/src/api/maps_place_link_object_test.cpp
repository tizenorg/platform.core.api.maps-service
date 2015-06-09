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

#include "maps_place_link_object_test.h"
#include "maps_place_link_object_plugin.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_place_link_object_create(maps_place_link_object_h* place); */
/* int maps_place_link_object_destroy(maps_place_link_object_h place); */
void utc_maps_place_link_object_create_p(void)
{
	maps_place_link_object_h link_object = NULL;
	int error = maps_place_link_object_create(&link_object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(link_object);

	error = maps_place_link_object_destroy(link_object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_link_object_create_n(void)
{
	int error = maps_place_link_object_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_link_object_h lo;
 public:
	test_env():lo(NULL)
	{
		const int error = maps_place_link_object_create(&lo);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(lo);
	}
	~test_env()
	{
		const int error = maps_place_link_object_destroy(lo);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_place_link_object_clone(const maps_place_link_object_h origin,
*  maps_place_link_object_h* cloned); */
void utc_maps_place_link_object_clone_p(void)
{
	test_env e;

	/* set test values for original */
	/*------------------------------------ */
	int error = maps_place_link_object_set_id(e.lo, "id");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_link_object_set_string(e.lo, "string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_link_object_set_type(e.lo, "type");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_link_object_set_name(e.lo, "name");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_place_link_object_h cloned = NULL;
	error = maps_place_link_object_clone(e.lo, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	char* string = NULL;
	error = maps_place_link_object_get_string(cloned, &string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(string, ==, "string");
	g_free(string);

	char* type = NULL;
	error = maps_place_link_object_get_type(cloned, &type);
	g_assert_cmpstr(type, ==, "type");
	g_free(type);

	char* id = NULL;
	error = maps_place_link_object_get_id(cloned, &id);
	g_assert_cmpstr(id, ==, "id");
	g_free(id);

	char* name = NULL;
	error = maps_place_link_object_get_name(cloned, &name);
	g_assert_cmpstr(name, ==, "name");
	g_free(name);
	/*------------------------------------ */

	error = maps_place_link_object_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_link_object_clone_n(void)
{
	test_env e;

	maps_place_link_object_h cloned = NULL;
	int error = maps_place_link_object_clone(e.lo, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_link_object_set_id(maps_place_link_object_h place,
*  const char* id); */
/* int maps_place_link_object_get_id(maps_place_link_object_h place,
*  char** id); */
void utc_maps_place_link_object_id_p(void)
{
	test_env e;

	int error = maps_place_link_object_set_id(e.lo, "link_object_id");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* id = NULL;
	error = maps_place_link_object_get_id(e.lo, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(id, ==, "link_object_id");
	g_free(id);
}

void utc_maps_place_link_object_id_n(void)
{
	test_env e;

	int error = maps_place_link_object_set_id(NULL, "link_object_id");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_set_id(e.lo, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* id = NULL;
	error = maps_place_link_object_get_id(NULL, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_get_id(e.lo, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_link_object_set_name(maps_place_link_object_h place,
*  const char* name); */
/* int maps_place_link_object_get_name(maps_place_link_object_h place,
*  char** name); */
void utc_maps_place_link_object_name_p(void)
{
	test_env e;

	int error = maps_place_link_object_set_name(e.lo, "link_object_name");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* name = NULL;
	error = maps_place_link_object_get_name(e.lo, &name);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(name, ==, "link_object_name");
	g_free(name);
}

void utc_maps_place_link_object_name_n(void)
{
	test_env e;

	int error = maps_place_link_object_set_name(NULL, "link_object_name");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_set_name(e.lo, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* name = NULL;
	error = maps_place_link_object_get_name(NULL, &name);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_get_name(e.lo, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_link_object_set_type(maps_place_link_object_h place,
*  const char* type); */
/* int maps_place_link_object_get_type(maps_place_link_object_h place,
*  char** type); */
void utc_maps_place_link_object_type_p(void)
{
	test_env e;

	int error = maps_place_link_object_set_type(e.lo, "link_object_type");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* type = NULL;
	error = maps_place_link_object_get_type(e.lo, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(type, ==, "link_object_type");
	g_free(type);
}

void utc_maps_place_link_object_type_n(void)
{
	test_env e;

	int error = maps_place_link_object_set_type(NULL, "link_object_type");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_set_type(e.lo, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* type = NULL;
	error = maps_place_link_object_get_type(NULL, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_get_type(e.lo, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_link_object_set_string(maps_place_link_object_h place,
*  const char* string); */
/* int maps_place_link_object_get_string(maps_place_link_object_h place,
*  char** string); */
void utc_maps_place_link_object_string_p(void)
{
	test_env e;

	int error =
		maps_place_link_object_set_string(e.lo, "link_object_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* string = NULL;
	error = maps_place_link_object_get_string(e.lo, &string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(string, ==, "link_object_string");
	g_free(string);
}

void utc_maps_place_link_object_string_n(void)
{
	test_env e;

	int error =
		maps_place_link_object_set_string(NULL, "link_object_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_set_string(e.lo, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* string = NULL;
	error = maps_place_link_object_get_string(NULL, &string);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_get_string(e.lo, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

