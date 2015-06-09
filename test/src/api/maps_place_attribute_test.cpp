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

#include "maps_place_attribute_test.h"
#include "maps_place_attribute_plugin.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_place_attribute_create(maps_place_attribute_h* place); */
/* int maps_place_attribute_destroy(maps_place_attribute_h place); */
void utc_maps_place_attribute_create_p(void)
{
	maps_place_attribute_h attribute = NULL;
	int error = maps_place_attribute_create(&attribute);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(attribute);

	error = maps_place_attribute_destroy(attribute);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_attribute_create_n(void)
{
	int error = maps_place_attribute_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_attribute_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_attribute_h a;
 public:
	test_env():a(NULL)
	{
		const int error = maps_place_attribute_create(&a);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(a);
	}
	~test_env()
	{
		const int error = maps_place_attribute_destroy(a);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_place_attribute_clone(const maps_place_attribute_h origin,
*  maps_place_attribute_h* cloned); */
void utc_maps_place_attribute_clone_p(void)
{
	test_env e;

	/* set test values for original */
	/*------------------------------------ */
	int error = maps_place_attribute_set_id(e.a, "id");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_attribute_set_label(e.a, "label");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_attribute_set_text(e.a, "text");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_place_attribute_h cloned = NULL;
	error = maps_place_attribute_clone(e.a, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	char* label = NULL;
	error = maps_place_attribute_get_label(cloned, &label);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(label, ==, "label");
	g_free(label);

	char* text = NULL;
	error = maps_place_attribute_get_text(cloned, &text);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(text, ==, "text");
	g_free(text);

	char* id = NULL;
	error = maps_place_attribute_get_id(cloned, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(id, ==, "id");
	g_free(id);
	/*------------------------------------ */

	error = maps_place_attribute_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_attribute_clone_n(void)
{
	test_env e;

	maps_place_attribute_h cloned = NULL;
	int error = maps_place_attribute_clone(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_attribute_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_attribute_set_id(maps_place_attribute_h place,
*  const char* id); */
/* int maps_place_attribute_get_id(maps_place_attribute_h place, char** id); */
void utc_maps_place_attribute_id_p(void)
{
	test_env e;

	int error = maps_place_attribute_set_id(e.a, "attribute_id");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* id = NULL;
	error = maps_place_attribute_get_id(e.a, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(id, ==, "attribute_id");
	g_free(id);
}

void utc_maps_place_attribute_id_n(void)
{
	test_env e;

	int error = maps_place_attribute_set_id(NULL, "attribute_id");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_attribute_set_id(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* id = NULL;
	error = maps_place_attribute_get_id(NULL, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_attribute_get_id(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_attribute_set_label(maps_place_attribute_h place,
*  const char* label); */
/* int maps_place_attribute_get_label(maps_place_attribute_h place,
*  char** label); */
void utc_maps_place_attribute_label_p(void)
{
	test_env e;

	int error = maps_place_attribute_set_label(e.a, "attribute_label");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* label = NULL;
	error = maps_place_attribute_get_label(e.a, &label);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(label, ==, "attribute_label");
	g_free(label);
}

void utc_maps_place_attribute_label_n(void)
{
	test_env e;

	int error = maps_place_attribute_set_label(NULL, "attribute_label");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_attribute_set_label(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* label = NULL;
	error = maps_place_attribute_get_label(NULL, &label);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_attribute_get_label(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_attribute_set_text(maps_place_attribute_h place,
*  const char* text); */
/* int maps_place_attribute_get_text(maps_place_attribute_h place,
*  const char** text); */
void utc_maps_place_attribute_text_p(void)
{
	test_env e;

	int error = maps_place_attribute_set_text(e.a, "attribute_text");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* text = NULL;
	error = maps_place_attribute_get_text(e.a, &text);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(text, ==, "attribute_text");
	g_free(text);
}

void utc_maps_place_attribute_text_n(void)
{
	test_env e;

	int error = maps_place_attribute_set_text(NULL, "attribute_text");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_attribute_set_text(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* text = NULL;
	error = maps_place_attribute_get_text(NULL, &text);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_attribute_get_text(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

