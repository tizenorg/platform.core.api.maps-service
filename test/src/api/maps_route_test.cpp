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

#include "maps_route_test.h"
#include "maps_route_plugin.h"
#include "maps_route_segment_plugin.h"
#include "maps_error.h"
#include <glib.h>
#include "maps_route_private.h"
#include "maps_util.h"

/* int maps_route_create(maps_route_h* route); */
/* int maps_route_destroy(maps_route_h route); */
void utc_maps_route_create_p(void)
{
	maps_route_h route = NULL;
	int error = maps_route_create(&route);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(route);

	error = maps_route_destroy(route);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_create_n(void)
{
	int error = maps_route_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/

class test_env
{
 public:
	maps_route_h h;
	int iterations;
 public:
	 test_env():h(NULL), iterations(0)
	{
		const int error = maps_route_create(&h);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(h);

		maps_int_hashtable_h data_supported = NULL;
		if (maps_int_hashtable_create(&data_supported) !=
			MAPS_ERROR_NONE)
			 return;

		maps_int_hashtable_set(data_supported,
				       MAPS_ROUTE_PATH,
				       MAPS_ROUTE_PATH);
		maps_int_hashtable_set(data_supported,
				       MAPS_ROUTE_SEGMENTS_PATH,
				       MAPS_ROUTE_SEGMENTS_PATH);
		maps_int_hashtable_set(data_supported,
				       MAPS_ROUTE_SEGMENTS_MANEUVERS,
				       MAPS_ROUTE_SEGMENTS_MANEUVERS);

		 _maps_route_set_supported_data(h, data_supported);
		 maps_int_hashtable_destroy(data_supported);

	}
	~test_env()
	{
		const int error = maps_route_destroy(h);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_route_clone(const maps_route_h origin, maps_route_h* cloned); */
void utc_maps_route_clone_p(void)
{
	test_env e;

	maps_route_h cloned = NULL;
	int error = maps_route_clone(e.h, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* TODO: add checking of other fields */

	error = maps_route_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_clone_n(void)
{
	test_env e;

	maps_route_h cloned = NULL;
	int error = maps_route_clone(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_set_route_id(maps_route_h route, const char* route_id); */
/* int maps_route_get_route_id(const maps_route_h route, char** route_id); */
void utc_maps_route_route_id_p(void)
{
	test_env e;

	int error = maps_route_set_route_id(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* ret = NULL;
	error = maps_route_get_route_id(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(ret, ==, "test_string");
	g_free(ret);
}

void utc_maps_route_route_id_n(void)
{
	test_env e;

	int error = maps_route_set_route_id(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_set_route_id(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* ret = NULL;
	error = maps_route_get_route_id(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_get_route_id(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_set_origin(maps_route_h route, maps_coordinates_h origin); */
/* int maps_route_get_origin(maps_route_h route, maps_coordinates_h* origin); */
void utc_maps_route_origin_p(void)
{
	test_env e;

	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(11.1, 22.2, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_set_origin(e.h, coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obtained_coords = NULL;
	error = maps_route_get_origin(e.h, &obtained_coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obtained_coords);
	maps_coordinates_destroy(obtained_coords);
	/* finish test ------------------------------------------------------ */

	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_origin_n(void)
{
	test_env e;

	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(11.1, 22.2, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_set_origin(NULL, coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_set_origin(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_coordinates_h obtained_coords = NULL;
	error = maps_route_get_origin(NULL, &obtained_coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_get_origin(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* finish test ------------------------------------------------------ */

	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_route_set_destination(maps_route_h route,
*  maps_coordinates_h destination); */
/* int maps_route_get_destination(maps_route_h route,
*  maps_coordinates_h* destination); */
void utc_maps_route_destination_p(void)
{
	test_env e;

	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(11.1, 22.2, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_set_destination(e.h, coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obtained_coords = NULL;
	error = maps_route_get_destination(e.h, &obtained_coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obtained_coords);
	maps_coordinates_destroy(obtained_coords);
	/* finish test ------------------------------------------------------ */

	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_destination_n(void)
{
	test_env e;

	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(11.1, 22.2, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_set_destination(NULL, coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_set_destination(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_coordinates_h obtained_coords = NULL;
	error = maps_route_get_destination(NULL, &obtained_coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_get_destination(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* finish test ------------------------------------------------------ */

	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_route_set_bounding_box(maps_route_h route,
*  maps_area_h bounding_box); */
/* int maps_route_get_bounding_box(maps_route_h route,
*  maps_area_h* bounding_box); */
void utc_maps_route_bounding_box_p(void)
{
	test_env e;

	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(11.1, 22.2, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_area_h bounding_box = NULL;
	error = maps_area_create_circle(coords, 123.4, &bounding_box);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_set_bounding_box(e.h, bounding_box);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obtained_bounding_box = NULL;
	error = maps_route_get_bounding_box(e.h, &obtained_bounding_box);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obtained_bounding_box);
	maps_area_destroy(obtained_bounding_box);
	/* finish test ------------------------------------------------------ */

	error = maps_area_destroy(bounding_box);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_bounding_box_n(void)
{
	test_env e;

	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(11.1, 22.2, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_area_h bounding_box = NULL;
	error = maps_area_create_circle(coords, 123.4, &bounding_box);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_set_bounding_box(NULL, bounding_box);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_set_bounding_box(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_coordinates_h obtained_bounding_box = NULL;
	error = maps_route_get_bounding_box(NULL, &obtained_bounding_box);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_get_bounding_box(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* finish test ------------------------------------------------------ */

	error = maps_area_destroy(bounding_box);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_route_set_total_distance(maps_route_h route,
*  double total_distance); */
/* int maps_route_get_total_distance(maps_route_h route,
*  double* total_distance); */
void utc_maps_route_total_distance_p(void)
{
	test_env e;

	int error = maps_route_set_total_distance(e.h, 42.0);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	double ret = .0;
	error = maps_route_get_total_distance(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(ret, ==, 42.0);
}

void utc_maps_route_total_distance_n(void)
{
	test_env e;

	int error = maps_route_set_total_distance(NULL, 42.0);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	double ret = .0;
	error = maps_route_get_total_distance(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_get_total_distance(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_set_total_duration(maps_route_h route,
*  long total_duration); */
/* int maps_route_get_total_duration(maps_route_h route,
*  long* total_duration); */
void utc_maps_route_total_duration_p(void)
{
	test_env e;

	int error = maps_route_set_total_duration(e.h, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	long ret = 0;
	error = maps_route_get_total_duration(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(ret, ==, 42);
}

void utc_maps_route_total_duration_n(void)
{
	test_env e;

	int error = maps_route_set_total_duration(NULL, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	long ret = 0;
	error = maps_route_get_total_duration(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_get_total_duration(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_set_transport_mode(maps_route_h route,
*  maps_route_transport_mode_e transport_mode); */
/* int maps_route_get_transport_mode(const maps_route_h route,
*  maps_route_transport_mode_e* transport_mode); */
void utc_maps_route_transport_mode_p(void)
{
	test_env e;

	int error =
		maps_route_set_transport_mode(e.h,
		MAPS_ROUTE_TRANSPORT_MODE_CAR);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_transport_mode_e transport_mode =
		MAPS_ROUTE_TRANSPORT_MODE_TRUCK;
	error = maps_route_get_transport_mode(e.h, &transport_mode);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(transport_mode, ==, MAPS_ROUTE_TRANSPORT_MODE_CAR);
}

void utc_maps_route_transport_mode_n(void)
{
	test_env e;

	int error =
		maps_route_set_transport_mode(NULL,
		MAPS_ROUTE_TRANSPORT_MODE_CAR);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_route_transport_mode_e transport_mode =
		MAPS_ROUTE_TRANSPORT_MODE_TRUCK;
	error = maps_route_get_transport_mode(NULL, &transport_mode);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_get_transport_mode(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_route_get_distance_unit(const maps_route_h route,
*  maps_distance_unit_e* distance_unit); */
/* int maps_route_set_distance_unit(const maps_route_h route,
*  const maps_distance_unit_e distance_unit); */
void utc_maps_route_distance_unit_p(void)
{
	test_env e;

	int error = maps_route_set_distance_unit(e.h, MAPS_DISTANCE_UNIT_M);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_distance_unit_e ret = MAPS_DISTANCE_UNIT_YD;
	error = maps_route_get_distance_unit(e.h, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(ret, ==, MAPS_DISTANCE_UNIT_M);
}

void utc_maps_route_distance_unit_n(void)
{
	test_env e;

	int error = maps_route_set_distance_unit(NULL, MAPS_DISTANCE_UNIT_M);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_distance_unit_e ret = MAPS_DISTANCE_UNIT_YD;
	error = maps_route_get_distance_unit(NULL, &ret);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_get_distance_unit(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* typedef bool(*maps_route_properties_cb)(const char* key, const char* value,
*  void* user_data); */
/* int maps_route_set_properties(maps_route_h route,
*  maps_item_hashtable_h properties); */
/* int maps_route_foreach_property(maps_route_h route,
*  maps_route_properties_cb callback, void* user_data); */
static bool __utc_maps_route_properties_cb(int index, int total, char* key,
	void* value, void* user_data)
{
	g_assert(key);
	g_assert(value);

	g_assert_cmpstr(key, ==, "key");
	g_assert_cmpstr((const char*) value, ==, "value");

	g_assert(user_data);
	test_env* e = (test_env*) user_data;

	e->iterations++;

	g_free(key);
	g_free(value);

	return true;
}

void utc_maps_route_properties_p(void)
{
	test_env e;

	maps_item_list_h obj = NULL;
	int error = maps_item_hashtable_create(&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_hashtable_set(obj, "key", "value",
		maps_item_hashtable_clone_string,
		maps_item_hashtable_free_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_set_properties(e.h, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_foreach_property(e.h, __utc_maps_route_properties_cb,
		&e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 1);
	/* finish test ------------------------------------------------------ */

	error = maps_item_hashtable_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_properties_n(void)
{
	test_env e;

	maps_item_list_h obj = NULL;
	int error = maps_item_hashtable_create(&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* start test ------------------------------------------------------- */
	error = maps_route_set_properties(NULL, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_set_properties(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_foreach_property(NULL,
		__utc_maps_route_properties_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_foreach_property(e.h, NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* finish test ------------------------------------------------------ */

	error = maps_item_hashtable_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* typedef bool(*maps_route_path_cb)(int index, int total,
*  maps_coordinates_h coordinates, void* user_data); */
/* int maps_route_foreach_path(const maps_route_h route,
*  maps_route_path_cb callback, void* user_data); */
/* int maps_route_set_path(maps_route_h route, const maps_item_list_h path); */
static bool __utc_maps_route_path_cb(int index, int total,
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

void utc_maps_route_path_p(void)
{
	test_env e;

	maps_item_list_h path = NULL;
	int error = maps_item_list_create(&path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obj = NULL;
	error = maps_coordinates_create(11.1, 22.2, &obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_append(path, obj, maps_coordinates_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	maps_coordinates_destroy(obj);

	/* test start --------------------------------- */
	error = maps_route_set_path(e.h, path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_foreach_path(e.h, __utc_maps_route_path_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 1);
	/* test finish --------------------------------- */

	error = maps_item_list_remove_all(path, maps_coordinates_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_path_n(void)
{
	test_env e;

	maps_item_list_h path = NULL;
	int error = maps_item_list_create(&path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h obj = NULL;
	error = maps_coordinates_create(11.1, 22.2, &obj);

	error = maps_item_list_append(path, obj, maps_coordinates_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	maps_coordinates_destroy(obj);

	/* test start --------------------------------- */
	error = maps_route_set_path(NULL, path);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_set_path(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_foreach_path(NULL, __utc_maps_route_path_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);

	error = maps_route_foreach_path(e.h, NULL, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);
	/* test finish --------------------------------- */

	/*error = maps_coordinates_destroy(obj); */
	/*g_assert_cmpint(error, ==, MAPS_ERROR_NONE); */

	error = maps_item_list_remove_all(path, maps_coordinates_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* typedef bool(*maps_route_segment_cb)(int index, int total,
*  maps_route_segment_h segment, void* user_data); */
/* int maps_route_foreach_segment(const maps_route_h route,
*  maps_route_segment_cb callback, void* user_data); */
/* int maps_route_set_segments(maps_route_h route,
*  const maps_item_list_h segments); */
static bool __utc_maps_route_segment_cb(int index, int total,
	maps_route_segment_h segment, void* user_data)
{
	test_env* e = (test_env*) user_data;
	g_assert(e);
	e->iterations++;

	g_assert(segment);

	g_assert_cmpint(index, >=, 0);
	g_assert_cmpint(total, >=, 1);

	maps_route_segment_destroy(segment);

	return true;
}

void utc_maps_route_segments_p(void)
{
	test_env e;

	maps_item_list_h segments = NULL;
	int error = maps_item_list_create(&segments);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_segment_h obj = NULL;
	error = maps_route_segment_create(&obj);

	error = maps_item_list_append(segments, obj, maps_route_segment_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	maps_route_segment_destroy(obj);

	/* test start --------------------------------- */
	error = maps_route_set_segments(e.h, segments);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_route_foreach_segment(e.h, __utc_maps_route_segment_cb,
		&e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 1);
	/* test finish --------------------------------- */

	error = maps_item_list_remove_all(segments, maps_route_segment_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(segments);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_route_segments_n(void)
{
	test_env e;

	maps_item_list_h segments = NULL;
	int error = maps_item_list_create(&segments);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_segment_h obj = NULL;
	error = maps_route_segment_create(&obj);

	error = maps_item_list_append(segments, obj, maps_route_segment_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	maps_route_segment_destroy(obj);

	/* test start --------------------------------- */
	error = maps_route_set_segments(NULL, segments);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_set_segments(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_route_foreach_segment(NULL, __utc_maps_route_segment_cb,
		&e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);

	error = maps_route_foreach_segment(e.h, NULL, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);
	/* test finish --------------------------------- */

	error = maps_item_list_remove_all(segments, maps_route_segment_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(segments);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

