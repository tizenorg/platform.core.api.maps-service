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

#include "maps_place_url_test.h"
#include "maps_place_url_plugin.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_place_url_create(maps_place_url_h* place); */
/* int maps_place_url_destroy(maps_place_url_h place); */
void utc_maps_place_url_create_p(void)
{
	maps_place_url_h h = NULL;
	int error = maps_place_url_create(&h);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(h);

	error = maps_place_url_destroy(h);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_url_create_n(void)
{
	int error = maps_place_url_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_url_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_url_h h;
 public:
	test_env():h(NULL)
	{
		const int error = maps_place_url_create(&h);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(h);
	}
	~test_env()
	{
		const int error = maps_place_url_destroy(h);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_place_url_clone(const maps_place_url_h origin,
*  maps_place_url_h* cloned); */
void utc_maps_place_url_clone_p(void)
{
	test_env e;

	/* set test values for original */
	/*------------------------------------ */
	int error = maps_place_url_set_path(e.h, "path");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_url_set_description(e.h, "desc");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_place_url_h cloned = NULL;
	error = maps_place_url_clone(e.h, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	char* path = NULL;
	error = maps_place_url_get_path(cloned, &path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(path, ==, "path");
	g_free(path);

	char* desc = NULL;
	error = maps_place_url_get_description(cloned, &desc);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(desc, ==, "desc");
	g_free(desc);
	/*------------------------------------ */

	error = maps_place_url_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_url_clone_n(void)
{
	test_env e;

	maps_place_url_h cloned = NULL;
	int error = maps_place_url_clone(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_url_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_url_set_path(maps_place_url_h place, const char* path); */
/* int maps_place_url_get_path(maps_place_url_h place, char** path); */
void utc_maps_place_url_path_p(void)
{
	test_env e;

	int error = maps_place_url_set_path(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* str = NULL;
	error = maps_place_url_get_path(e.h, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(str, ==, "test_string");
	g_free(str);
}

void utc_maps_place_url_path_n(void)
{
	test_env e;

	int error = maps_place_url_set_path(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_url_set_path(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* str = NULL;
	error = maps_place_url_get_path(NULL, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_url_get_path(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_url_set_description(maps_place_url_h place,
*  const char* desc); */
/* int maps_place_url_get_description(maps_place_url_h place, char** desc); */
void utc_maps_place_url_description_p(void)
{
	test_env e;

	int error = maps_place_url_set_description(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* str = NULL;
	error = maps_place_url_get_description(e.h, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(str, ==, "test_string");
	g_free(str);
}

void utc_maps_place_url_description_n(void)
{
	test_env e;

	int error = maps_place_url_set_description(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_url_set_description(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* str = NULL;
	error = maps_place_url_get_description(NULL, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_url_get_description(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

