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

#include "maps_route_segment_test.h"
#include "maps_route_segment_plugin.h"
#include "maps_route_maneuver_plugin.h"
#include "maps_coordinates.h"
#include "maps_area.h"
#include "maps_error.h"
#include <glib.h>
#include "maps_route_segment_private.h"
#include "maps_util.h"
#include "maps_extra_types_private.h"

#include <maps_object.h>
using namespace maps;

/* int maps_route_segment_create(maps_route_segment_h* segment); */
/* int maps_route_segment_destroy(maps_route_segment_h segment); */
void utc_maps_route_segment_create_p(void)
{
	maps_route_segment_h segment = NULL;
	int error = maps_route_segment_create(&segment);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(segment);

	error = maps_route_segment_destroy(segment);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_segment_create_n(void)
{
	int error = maps_route_segment_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/

class test_env
{
public:
	maps_route_segment_h h;
	int iterations;
public:
	test_env() : h(NULL), iterations(0)
	{
		const int error = maps_route_segment_create(&h);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(h);

		maps_int_hashtable_h data_supported = NULL;
		if (maps_int_hashtable_create(&data_supported) !=
		    MAPS_ERROR_NONE)
			return;

		maps_int_hashtable_set(data_supported,
				       MAPS_ROUTE_SEGMENTS_PATH,
				       MAPS_ROUTE_SEGMENTS_PATH);
		maps_int_hashtable_set(data_supported,
				       MAPS_ROUTE_SEGMENTS_MANEUVERS,
				       MAPS_ROUTE_SEGMENTS_MANEUVERS);

		_maps_route_segment_set_supported_data(h, data_supported);
		maps_int_hashtable_destroy(data_supported);

	}
	~test_env()
	{
		const int error = maps_route_segment_destroy(h);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

static bool __utc_maps_route_segment_path_clone_cb(int index, int total,
	maps_coordinates_h coordinates, void* user_data)
{
	g_assert_cmpint(index, >=, 0);
	g_assert_cmpint(total, >=, 1);
	g_assert_cmpint(index, <, total);
	g_assert(coordinates);
	g_assert(user_data);

	int* cnt = (int*) user_data;
	*cnt =* cnt + 1;
	maps_coordinates_destroy(coordinates);
	return true;
}

static bool __utc_maps_route_segment_maneuver_clone_cb(int index, int total,
	maps_route_maneuver_h maneuver, void* user_data)
{
	g_assert_cmpint(index, >=, 0);
	g_assert_cmpint(total, >=, 1);
	g_assert_cmpint(index, <, total);
	g_assert(maneuver);
	g_assert(user_data);

	int* cnt = (int*) user_data;
	*cnt =* cnt + 1;
	maps_route_maneuver_destroy(maneuver);
	return true;
}

/* int maps_route_segment_clone(const maps_route_segment_h origin,
*  maps_route_segment_h* cloned); */
void utc_maps_route_segment_clone_p(void)
{
	test_env e;

	/*------------------------------------ */
	/* set test values for original */
	const coordinates origin(11.1, 12.2);
	int error = maps_route_segment_set_origin(e.h, origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	const coordinates destination(21.1, 22.2);
	error = maps_route_segment_set_destination(e.h, destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	const area bounding_box(44.4, 22.2, 11.1, 55.5);
	error = maps_route_segment_set_bounding_box(e.h, bounding_box);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_segment_set_distance(e.h, 6.0221);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_segment_set_duration(e.h, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	item_list < coordinates > path;
	path.add(origin);
	path.add(destination);
	error = maps_route_segment_set_path(e.h, path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	route_maneuver m1, m2;
	item_list < route_maneuver > maneuvers;
	maneuvers.add(m1);
	maneuvers.add(m2);
	error = maps_route_segment_set_maneuvers(e.h, maneuvers);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_route_segment_h cloned = NULL;
	error = maps_route_segment_clone(e.h, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/*------------------------------------ */
	/* test if values are cloned correctly */
	coordinates origin_cloned;
	error = maps_route_segment_get_origin(cloned, origin_cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(origin == origin_cloned);

	coordinates destination_cloned;
	error = maps_route_segment_get_destination(cloned, destination_cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(destination == destination_cloned);

	area bounding_box_cloned;
	error = maps_route_segment_get_bounding_box(cloned,
		bounding_box_cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(bounding_box == bounding_box_cloned);

	double distance_cloned = .0;
	error = maps_route_segment_get_distance(cloned, &distance_cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(__equal_double(distance_cloned, 6.0221));

	long duration_cloned = 0;
	error = maps_route_segment_get_duration(cloned, &duration_cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(duration_cloned, ==, 42);

	int iterations = 0;
	error = maps_route_segment_foreach_path(cloned,
		__utc_maps_route_segment_path_clone_cb, &iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(iterations, ==, 2);

	iterations = 0;
	error = maps_route_segment_foreach_maneuver(cloned,
		__utc_maps_route_segment_maneuver_clone_cb, &iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(iterations, ==, 2);
	/*------------------------------------ */

	error = maps_route_segment_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_segment_clone_n(void)
{
	test_env e;

	maps_route_segment_h cloned = NULL;
	int error = maps_route_segment_clone(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_segment_get_origin(const maps_route_segment_h segment,
*  maps_coordinates_h* origin); */
/* int maps_route_segment_set_origin(maps_route_segment_h segment,
*  const maps_coordinates_h origin); */
void utc_maps_route_segment_origin_p(void)
{
	test_env e;

	maps_coordinates_h obj = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_segment_set_origin(e.h, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obtained_obj = NULL;
	error = maps_route_segment_get_origin(e.h, &obtained_obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obtained_obj);
	maps_coordinates_destroy(obtained_obj);
	/* finish test ------------------------------------------------------ */

	error = maps_coordinates_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_segment_origin_n(void)
{
	test_env e;

	maps_coordinates_h obj = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_segment_set_origin(NULL, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_set_origin(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_coordinates_h obtained_obj = NULL;
	error = maps_route_segment_get_origin(NULL, &obtained_obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_get_origin(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* finish test ------------------------------------------------------ */

	error = maps_coordinates_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_route_segment_get_destination(const maps_route_segment_h segment,
*  maps_coordinates_h* destination); */
/* int maps_route_segment_set_destination(maps_route_segment_h segment,
*  const maps_coordinates_h destination); */
void utc_maps_route_segment_destination_p(void)
{
	test_env e;

	maps_coordinates_h obj = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_segment_set_destination(e.h, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obtained_obj = NULL;
	error = maps_route_segment_get_destination(e.h, &obtained_obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obtained_obj);
	maps_coordinates_destroy(obtained_obj);
	/* finish test ------------------------------------------------------ */

	error = maps_coordinates_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_segment_destination_n(void)
{
	test_env e;

	maps_coordinates_h obj = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------ */
	error = maps_route_segment_set_destination(NULL, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_set_destination(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_coordinates_h obtained_obj = NULL;
	error = maps_route_segment_get_destination(NULL, &obtained_obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_get_destination(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* finish test ------------------------------------------------------ */

	error = maps_coordinates_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_route_segment_get_bounding_box(const maps_route_segment_h segment,
*  maps_area_h* bounding_box); */
/* int maps_route_segment_set_bounding_box(maps_route_segment_h segment,
*  maps_area_h bounding_box); */
void utc_maps_route_segment_bounding_box_p(void)
{
	test_env e;

	maps_coordinates_h center = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&center);
	g_assert(center);
	maps_area_h bounds = NULL;
	error = maps_area_create_circle(center, 200, &bounds);
	g_assert(bounds);

	/* test start --------------------------------- */
	error = maps_route_segment_set_bounding_box(e.h, bounds);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_area_h obtained_bounds = NULL;
	error = maps_route_segment_get_bounding_box(e.h, &obtained_bounds);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obtained_bounds);
	maps_area_destroy(obtained_bounds);
	/* test finish --------------------------------- */

	error = maps_area_destroy(bounds);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_coordinates_destroy(center);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_segment_bounding_box_n(void)
{
	test_env e;

	maps_coordinates_h center = NULL;
	int error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&center);
	g_assert(center);
	maps_area_h bounds = NULL;
	error = maps_area_create_circle(center, 200, &bounds);
	g_assert(bounds);

	/* test start --------------------------------- */
	error = maps_route_segment_set_bounding_box(NULL, bounds);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_set_bounding_box(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_area_h obtained_bounds = NULL;
	error = maps_route_segment_get_bounding_box(NULL, &obtained_bounds);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!obtained_bounds);

	error = maps_route_segment_get_bounding_box(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */

	error = maps_area_destroy(bounds);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_coordinates_destroy(center);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_route_segment_get_distance(const maps_route_segment_h segment,
*  double* distance); */
/*int maps_route_segment_set_distance(maps_route_segment_h segment,
* double distance); */
void utc_maps_route_segment_distance_p(void)
{
	test_env e;

	int error = maps_route_segment_set_distance(e.h, 42.1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	double ret = 0;
	error = maps_route_segment_get_distance(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(ret, >=, 42.0);
	g_assert_cmpfloat(ret, <=, 42.2);
}

void utc_maps_route_segment_distance_n(void)
{
	test_env e;

	int error = maps_route_segment_set_distance(NULL, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	double ret = 0;
	error = maps_route_segment_get_distance(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_get_distance(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_segment_get_duration(const maps_route_segment_h segment,
*  long* duration); */
/* int maps_route_segment_set_duration(maps_route_segment_h segment,
*  long duration); */
void utc_maps_route_segment_duration_p(void)
{
	test_env e;

	int error = maps_route_segment_set_duration(e.h, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	long ret = 0;
	error = maps_route_segment_get_duration(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(ret, ==, 42);
}

void utc_maps_route_segment_duration_n(void)
{
	test_env e;

	int error = maps_route_segment_set_duration(NULL, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	long ret = 0;
	error = maps_route_segment_get_duration(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_get_duration(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* typedef bool(*maps_route_segment_path_cb)(int index, int total,
*  maps_coordinates_h coordinates, void* user_data); */
/* int maps_route_segment_foreach_path(const maps_route_segment_h route,
*  maps_route_path_cb callback, void* user_data); */
/* int maps_route_segment_set_path(const maps_route_segment_h segment,
*  maps_item_list_h path); */
static bool __utc_maps_route_segment_path_cb(int index, int total,
	maps_coordinates_h coordinates, void* user_data)
{
	test_env* e = (test_env*) user_data;
	g_assert(e);
	e->iterations++;

	g_assert(coordinates);

	g_assert_cmpint(index, >=, 0);
	g_assert_cmpint(total, >=, 1);

	maps_coordinates_destroy(coordinates);

	return true;
}

void utc_maps_route_segment_path_p(void)
{
	test_env e;

	maps_item_list_h path = NULL;
	int error = maps_item_list_create(&path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obj = NULL;
	error = maps_coordinates_create(11.1, 22.2, &obj);

	error = maps_item_list_append(path, obj, maps_coordinates_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	error = maps_route_segment_set_path(e.h, path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_segment_foreach_path(e.h,
		__utc_maps_route_segment_path_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 1);
	/* test finish --------------------------------- */

	error = maps_item_list_remove_all(path, maps_coordinates_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_segment_path_n(void)
{
	test_env e;

	maps_item_list_h path = NULL;
	int error = maps_item_list_create(&path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obj = NULL;
	error = maps_coordinates_create(11.1, 22.2, &obj);

	error = maps_item_list_append(path, obj, maps_coordinates_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	error = maps_route_segment_set_path(NULL, path);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_set_path(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_foreach_path(NULL,
		__utc_maps_route_segment_path_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);

	error = maps_route_segment_foreach_path(e.h, NULL, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);
	/* test finish --------------------------------- */

	error = maps_item_list_remove_all(path, maps_coordinates_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* typedef bool(*maps_route_segment_maneuver_cb)(int index, int total,
*  maps_route_maneuver_h maneuver, void* user_data); */
/* int maps_route_segment_foreach_maneuver(const maps_route_segment_h route,
*  maps_route_maneuver_cb callback, void* user_data); */
/* int maps_route_segment_set_maneuvers(maps_route_segment_h route,
*  const maps_item_list_h maneuvers); */
static bool __utc_maps_route_segment_maneuver_cb(int index, int total,
	maps_route_maneuver_h maneuver, void* user_data)
{
	test_env* e = (test_env*) user_data;
	g_assert(e);
	e->iterations++;

	g_assert(maneuver);

	g_assert_cmpint(index, >=, 0);
	g_assert_cmpint(total, >=, 1);

	maps_route_maneuver_destroy(maneuver);

	return true;
}

void utc_maps_route_segment_maneuvers_p(void)
{
	test_env e;

	maps_item_list_h maneuvers = NULL;
	int error = maps_item_list_create(&maneuvers);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_maneuver_h obj = NULL;
	error = maps_route_maneuver_create(&obj);

	error = maps_item_list_append(maneuvers, obj,
		maps_route_maneuver_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	maps_route_maneuver_destroy(obj);

	/* test start --------------------------------- */
	error = maps_route_segment_set_maneuvers(e.h, maneuvers);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_segment_foreach_maneuver(e.h,
		__utc_maps_route_segment_maneuver_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 1);
	/* test finish --------------------------------- */

	error = maps_item_list_remove_all(maneuvers,
		maps_route_maneuver_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(maneuvers);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_segment_maneuvers_n(void)
{
	test_env e;

	maps_item_list_h maneuvers = NULL;
	int error = maps_item_list_create(&maneuvers);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_maneuver_h obj = NULL;
	error = maps_route_maneuver_create(&obj);

	error = maps_item_list_append(maneuvers, obj,
		maps_route_maneuver_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	maps_route_maneuver_destroy(obj);

	/* test start --------------------------------- */
	error = maps_route_segment_set_maneuvers(NULL, maneuvers);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_set_maneuvers(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_segment_foreach_maneuver(NULL,
		__utc_maps_route_segment_maneuver_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);

	error = maps_route_segment_foreach_maneuver(e.h, NULL, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);
	/* test finish --------------------------------- */

	error = maps_item_list_remove_all(maneuvers,
		maps_route_maneuver_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(maneuvers);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

