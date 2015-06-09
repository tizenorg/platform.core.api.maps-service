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

#include "maps_place_filter_test.h"
#include "maps_place_filter.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_place_filter_create(maps_place_filter_h* filter); */
/* int maps_place_filter_destroy(maps_place_filter_h filter); */
void utc_maps_place_filter_create_p(void)
{
	maps_place_filter_h filter = NULL;
	int error = maps_place_filter_create(&filter);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(filter);

	error = maps_place_filter_destroy(filter);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_filter_create_n(void)
{
	int error = maps_place_filter_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_filter_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_filter_h h;
	int iterations;
 public:
	 test_env():h(NULL), iterations(0)
	{
		const int error = maps_place_filter_create(&h);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(h);
	}
	~test_env()
	{
		const int error = maps_place_filter_destroy(h);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_place_filter_clone(const maps_place_filter_h origin,
*  maps_place_filter_h* cloned); */
void utc_maps_place_filter_clone_p(void)
{
	test_env e;

	/* set test values for original */
	/*------------------------------------ */
	int error = maps_place_filter_set(e.h, "key1", "value1");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_filter_set(e.h, "key2", "value2");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_place_filter_h cloned = NULL;
	error = maps_place_filter_clone(e.h, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	char* value1 = NULL;
	error = maps_place_filter_get(cloned, "key1", &value1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(value1, ==, "value1");
	g_free(value1);

	char* value2 = NULL;
	error = maps_place_filter_get(cloned, "key2", &value2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(value2, ==, "value2");
	g_free(value2);
	/*------------------------------------ */

	error = maps_place_filter_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_filter_clone_n(void)
{
	test_env e;

	maps_place_filter_h cloned = NULL;
	int error = maps_place_filter_clone(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_filter_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* typedef bool (*maps_place_filter_properties_cb)(char* key, char* value ,
*  void* user_data); */
/* int maps_place_filter_set(maps_place_filter_h filter, const char* key,
*  const char* value); */
/* int maps_place_filter_get(maps_place_filter_h filter, const char* key,
*  char** value); */
/* int maps_place_filter_foreach_property(maps_place_filter_h filter,
*  maps_place_filter_properties_cb callback , void* user_data); */
static bool __utc_maps_place_filter_cb(int index, int total, char* key,
	void* value, void* user_data)
{
	g_assert(key);
	g_assert(value);
	g_assert(user_data);

	test_env* e = (test_env*) user_data;

	e->iterations++;

	g_free(key);
	g_free(value);

	return true;
}

void utc_maps_place_filter_set_p(void)
{
	test_env e;

	int error = maps_place_filter_set(e.h, "key", "value");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* value = NULL;
	error = maps_place_filter_get(e.h, "key", &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(value, ==, "value");
	g_free(value);

	error = maps_place_filter_foreach_property(e.h,
		__utc_maps_place_filter_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 1);
}

void utc_maps_place_filter_set_n(void)
{
	test_env e;

	int error = maps_place_filter_set(NULL, "key", "value");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_filter_set(e.h, NULL, "value");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_filter_set(e.h, "key", NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* value = NULL;
	error = maps_place_filter_get(NULL, "key", &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpstr(value, !=, "value");

	error = maps_place_filter_get(e.h, NULL, &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpstr(value, !=, "value");

	error = maps_place_filter_get(e.h, "key", NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpstr(value, !=, "value");

	error = maps_place_filter_foreach_property(NULL,
		__utc_maps_place_filter_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);

	error = maps_place_filter_foreach_property(e.h, NULL, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);
};

