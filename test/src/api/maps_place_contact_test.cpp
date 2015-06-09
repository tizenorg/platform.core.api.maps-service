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

#include "maps_place_contact_test.h"
#include "maps_place_contact_plugin.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_place_contact_create(maps_place_contact_h* place); */
/* int maps_place_contact_destroy(maps_place_contact_h place); */
void utc_maps_place_contact_create_p(void)
{
	maps_place_contact_h contact = NULL;
	int error = maps_place_contact_create(&contact);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(contact);

	error = maps_place_contact_destroy(contact);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_contact_create_n(void)
{
	int error = maps_place_contact_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_contact_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_contact_h c;
 public:
	test_env():c(NULL)
	{
		const int error = maps_place_contact_create(&c);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(c);
	}
	~test_env()
	{
		const int error = maps_place_contact_destroy(c);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_place_contact_clone(const maps_place_contact_h origin,
*  maps_place_contact_h* cloned); */
void utc_maps_place_contact_clone_p(void)
{
	test_env e;

	/* set test values for original */
	/*------------------------------------ */
	int error = maps_place_contact_set_label(e.c, "label");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_contact_set_type(e.c, "type");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_contact_set_value(e.c, "value");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_place_contact_h cloned = NULL;
	error = maps_place_contact_clone(e.c, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	char* label = NULL;
	error = maps_place_contact_get_label(cloned, &label);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(label, ==, "label");
	g_free(label);

	char* type = NULL;
	error = maps_place_contact_get_type(cloned, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(type, ==, "type");
	g_free(type);

	char* value = NULL;
	error = maps_place_contact_get_value(cloned, &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(value, ==, "value");
	g_free(value);
	/*------------------------------------ */

	error = maps_place_contact_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_contact_clone_n(void)
{
	test_env e;

	maps_place_contact_h cloned = NULL;
	int error = maps_place_contact_clone(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_contact_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_contact_set_label(maps_place_contact_h place,
*  const char* label); */
/* int maps_place_contact_get_label(maps_place_contact_h place,
*  char** label); */
void utc_maps_place_contact_label_p(void)
{
	test_env e;

	int error = maps_place_contact_set_label(e.c, "contact_label");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* label = NULL;
	error = maps_place_contact_get_label(e.c, &label);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(label, ==, "contact_label");
	g_free(label);
}

void utc_maps_place_contact_label_n(void)
{
	test_env e;

	int error = maps_place_contact_set_label(NULL, "contact_label");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_contact_set_label(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* label = NULL;
	error = maps_place_contact_get_label(NULL, &label);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_contact_get_label(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_contact_set_type(maps_place_contact_h place,
*  const char* type); */
/* int maps_place_contact_get_type(maps_place_contact_h place, char** type); */
void utc_maps_place_contact_type_p(void)
{
	test_env e;

	int error = maps_place_contact_set_type(e.c, "contact_type");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* type = NULL;
	error = maps_place_contact_get_type(e.c, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(type, ==, "contact_type");
	g_free(type);
}

void utc_maps_place_contact_type_n(void)
{
	test_env e;

	int error = maps_place_contact_set_type(NULL, "contact_type");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_contact_set_type(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* type = NULL;
	error = maps_place_contact_get_type(NULL, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_contact_get_type(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_contact_set_value(maps_place_contact_h place,
*  const char* value); */
/* int maps_place_contact_get_value(maps_place_contact_h place,
*  char** value); */
void utc_maps_place_contact_value_p(void)
{
	test_env e;

	int error = maps_place_contact_set_value(e.c, "contact_value");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* value = NULL;
	error = maps_place_contact_get_value(e.c, &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(value, ==, "contact_value");
	g_free(value);
}

void utc_maps_place_contact_value_n(void)
{
	test_env e;

	int error = maps_place_contact_set_value(NULL, "contact_value");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_contact_set_value(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* value = NULL;
	error = maps_place_contact_get_value(NULL, &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_contact_get_value(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

