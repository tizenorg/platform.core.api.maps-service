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

#include "maps_place_category_test.h"
#include "maps_place_category.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_place_category_create(maps_place_category_h* place); */
/* int maps_place_category_destroy(maps_place_category_h place); */
void utc_maps_place_category_create_p(void)
{
	maps_place_category_h category = NULL;
	int error = maps_place_category_create(&category);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(category);

	error = maps_place_category_destroy(category);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_category_create_n(void)
{
	int error = maps_place_category_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_category_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_category_h c;
	int iteration;
 public:
	 test_env():c(NULL), iteration(0)
	{
		const int error = maps_place_category_create(&c);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(c);
	}
	~test_env()
	{
		const int error = maps_place_category_destroy(c);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

void utc_maps_place_category_clone_p(void)
{
	test_env e;

	/* set test values for original */
	/*------------------------------------ */
	int error = maps_place_category_set_id(e.c, "id");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_category_set_name(e.c, "name");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_category_set_url(e.c, "url");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_category_h cloned = NULL;
	error = maps_place_category_clone(e.c, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	char* name = NULL;
	error = maps_place_category_get_name(cloned, &name);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(name, ==, "name");
	g_free(name);

	char* id = NULL;
	error = maps_place_category_get_id(cloned, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(id, ==, "id");
	g_free(id);

	char* url = NULL;
	error = maps_place_category_get_url(cloned, &url);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(url, ==, "url");
	g_free(url);

	error = maps_place_category_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_category_clone_n(void)
{
	test_env e;

	maps_place_category_h cloned = NULL;
	int error = maps_place_category_clone(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_category_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_category_get_name(maps_place_category_h place,
*  char** name); */
/* int maps_place_category_set_name(maps_place_category_h place,
*  const char* name); */
void utc_maps_place_category_name_p(void)
{
	test_env e;

	int error = maps_place_category_set_name(e.c, "category_name");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* name = NULL;
	error = maps_place_category_get_name(e.c, &name);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(name, ==, "category_name");
	g_free(name);
}

void utc_maps_place_category_name_n(void)
{
	test_env e;

	int error = maps_place_category_set_name(NULL, "category_name");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_category_set_name(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* name = NULL;
	error = maps_place_category_get_name(NULL, &name);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_category_get_name(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_category_get_id(maps_place_category_h place, char** id); */
/* int maps_place_category_set_id(maps_place_category_h place,
*  const char* id); */
void utc_maps_place_category_id_p(void)
{
	test_env e;

	int error = maps_place_category_set_id(e.c, "category_id");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* id = NULL;
	error = maps_place_category_get_id(e.c, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(id, ==, "category_id");
	g_free(id);
}

void utc_maps_place_category_id_n(void)
{
	test_env e;

	int error = maps_place_category_set_id(NULL, "category_id");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_category_set_id(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* id = NULL;
	error = maps_place_category_get_id(NULL, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_category_get_id(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_category_get_url(maps_place_category_h place,  char** url); */
/* int maps_place_category_set_url(maps_place_category_h place,
*  const char* url); */
void utc_maps_place_category_url_p(void)
{
	test_env e;

	int error = maps_place_category_set_url(e.c, "http://category_url.com");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* url = NULL;
	error = maps_place_category_get_url(e.c, &url);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(url, ==, "http://category_url.com");
	g_free(url);
}

void utc_maps_place_category_url_n(void)
{
	test_env e;

	int error =
		maps_place_category_set_url(NULL, "http://category_url.com");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_category_set_url(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* url = NULL;
	error = maps_place_category_get_url(NULL, &url);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_category_get_url(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

