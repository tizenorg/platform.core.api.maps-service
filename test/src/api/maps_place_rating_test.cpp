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

#include "maps_place_rating_test.h"
#include "maps_place_rating_plugin.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_place_rating_create(maps_place_rating_h* place); */
/* int maps_place_rating_destroy(maps_place_rating_h place); */
void utc_maps_place_rating_create_p(void)
{
	maps_place_rating_h h = NULL;
	int error = maps_place_rating_create(&h);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(h);

	error = maps_place_rating_destroy(h);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_rating_create_n(void)
{
	int error = maps_place_rating_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_rating_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_rating_h h;
 public:
	test_env():h(NULL)
	{
		const int error = maps_place_rating_create(&h);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(h);
	}
	~test_env()
	{
		const int error = maps_place_rating_destroy(h);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_place_rating_clone(const maps_place_rating_h origin,
*  maps_place_rating_h* cloned); */
void utc_maps_place_rating_clone_p(void)
{
	test_env e;

	/* set test values for original */
	/*------------------------------------ */
	int error = maps_place_rating_set_count(e.h, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_rating_set_average(e.h, 6.0221);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_place_rating_h cloned = NULL;
	error = maps_place_rating_clone(e.h, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	int count = 0;
	error = maps_place_rating_get_count(cloned, &count);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(count, ==, 42);

	double average = .0;
	error = maps_place_rating_get_average(cloned, &average);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(average, ==, 6.0221);
	/*------------------------------------ */

	error = maps_place_rating_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_rating_clone_n(void)
{
	test_env e;

	maps_place_rating_h cloned = NULL;
	int error = maps_place_rating_clone(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_rating_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_rating_set_count(maps_place_rating_h place,
*  const int count); */
/* int maps_place_rating_get_count(maps_place_rating_h place, int* count); */
void utc_maps_place_rating_count_p(void)
{
	test_env e;

	int error = maps_place_rating_set_count(e.h, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	int n = 0;
	error = maps_place_rating_get_count(e.h, &n);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(n, ==, 42);
}

void utc_maps_place_rating_count_n(void)
{
	test_env e;

	int error = maps_place_rating_set_count(NULL, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	int n = 0;
	error = maps_place_rating_get_count(NULL, &n);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_rating_get_count(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_rating_set_average(maps_place_rating_h place,
*  const double average); */
/* int maps_place_rating_get_average(maps_place_rating_h place,
*  double* average); */
void utc_maps_place_rating_average_p(void)
{
	test_env e;

	int error = maps_place_rating_set_average(e.h, 1.1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	double d = .0;
	error = maps_place_rating_get_average(e.h, &d);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(d, ==, 1.1);
}

void utc_maps_place_rating_average_n(void)
{
	test_env e;

	int error = maps_place_rating_set_average(NULL, 1.1);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	double d = .0;
	error = maps_place_rating_get_average(NULL, &d);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_rating_get_average(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

