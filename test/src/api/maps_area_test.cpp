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

#include "maps_area_test.h"
#include "maps_area.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_area_create_rectangle (maps_coordinates_h top_left,
*  maps_coordinates_h bottom_right maps_area_h* area); */
void utc_maps_area_create_rectangle_p(void)
{

	maps_coordinates_h top_left = NULL;
	int error = maps_coordinates_create(44.4, 22.2, &top_left);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h bottom_right = NULL;
	error = maps_coordinates_create(11.1, 55.5, &bottom_right);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_area_h area = NULL;
	error = maps_area_create_rectangle(top_left, bottom_right, &area);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(area);

	error = maps_area_destroy(area);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(top_left);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(bottom_right);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_area_create_rectangle_n(void)
{
	maps_coordinates_h top_left = NULL;
	int error = maps_coordinates_create(11.1, 22.2, &top_left);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h bottom_right = NULL;
	error = maps_coordinates_create(44.4, 55.5, &bottom_right);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_area_h area = NULL;
	error = maps_area_create_rectangle(NULL, bottom_right, &area);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!area);

	error = maps_area_create_rectangle(top_left, NULL, &area);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!area);

	error = maps_area_create_rectangle(top_left, bottom_right, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!area);

	error = maps_coordinates_destroy(top_left);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(bottom_right);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_area_create_circle (maps_coordinates_h center, double radius,
*	maps_area_h* area); */
void utc_maps_area_create_circle_p(void)
{
	maps_coordinates_h center = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&center);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_area_h area = NULL;
	error = maps_area_create_circle(center, 200, &area);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(area);

	error = maps_area_destroy(area);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(center);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_area_create_circle_n(void)
{
	maps_coordinates_h center = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&center);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_area_h area = NULL;
	error = maps_area_create_circle(NULL, 200, &area);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!area);

	error = maps_area_create_circle(center, 200, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!area);

	error = maps_coordinates_destroy(center);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_area_destroy(maps_area_h area); */
void utc_maps_area_destroy_p(void)
{
	/* empty */
}

void utc_maps_area_destroy_n(void)
{
	const int error = maps_area_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_area_clone(const maps_area_h origin, maps_area_h* cloned); */
void utc_maps_area_clone_p(void)
{

	{			/* Clone circular area */
		maps_coordinates_h center = NULL;
		int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
			33.3,
#endif
			&center);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		maps_area_h area = NULL;
		error = maps_area_create_circle(center, 200, &area);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(area);

		maps_area_h cloned = NULL;
		maps_area_clone(area, &cloned);
		g_assert(cloned);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_area_destroy(area);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_area_destroy(cloned);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_coordinates_destroy(center);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}

	{			/* Clone rectangular area */
		maps_coordinates_h top_left = NULL;
		int error = maps_coordinates_create(44.4, 22.2,	&top_left);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		maps_coordinates_h bottom_right = NULL;
		error = maps_coordinates_create(11.1, 55.5, &bottom_right);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		maps_area_h area = NULL;
		error = maps_area_create_rectangle(top_left, bottom_right,
			&area);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(area);

		maps_area_h cloned = NULL;
		maps_area_clone(area, &cloned);
		g_assert(cloned);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_area_destroy(area);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_area_destroy(cloned);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_coordinates_destroy(top_left);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_coordinates_destroy(bottom_right);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
}

void utc_maps_area_clone_n(void)
{
	maps_coordinates_h center = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&center);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_area_h area = NULL;
	error = maps_area_create_circle(center, 200, &area);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(area);

	maps_area_h cloned = NULL;
	error = maps_area_clone(area, NULL);
	g_assert(!cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_area_clone(NULL, &cloned);
	g_assert(!cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_area_destroy(area);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(center);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

