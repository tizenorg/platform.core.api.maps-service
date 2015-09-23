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

#include "map_object_test.h"
#include "map_object.h"
#include "maps_error.h"
#include <glib.h>
#include "map_view.h"


extern int _map_object_create(const map_object_type_e type,
			      map_object_h *object);

/*int map_object_create(const map_object_type_e type,
			    map_object_h *object);
int map_object_destroy(map_object_h object);*/
void utc_map_object_create_p(void)
{
	/* Create a test view object */
	map_object_h object = NULL;
	int error = _map_object_create(MAP_OBJECT_MARKER,
					   &object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(object);

	/* Destroy a test view object */
	error = map_object_destroy(object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_create_n(void)
{
	/* Incorrectly create a test view object */
	map_object_h object = NULL;
	int error = _map_object_create((map_object_type_e)-2,
					   &object);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!object);

	error = _map_object_create(MAP_OBJECT_MARKER, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!object);
}



/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	map_object_h o;
	int iterations;
 public:
	test_env(const map_object_type_e type) : o(NULL), iterations(0)
	{
		/* Create a test view object */
		int error = _map_object_create(type, &o);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(o);
	}
	~test_env()
	{
		/* Destroy a test view object */
		const int error = map_object_destroy(o);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/*int map_object_get_type(const map_object_h object,
			      map_object_type_e *type);*/
void utc_map_object_get_type_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	map_object_type_e type = MAP_OBJECT_UNKNOWN;
	const int error = map_object_get_type(e.o, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(type, ==, MAP_OBJECT_MARKER);
}

void utc_map_object_get_type_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	map_object_type_e type = MAP_OBJECT_UNKNOWN;
	int error = map_object_get_type(NULL, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(type, ==, MAP_OBJECT_UNKNOWN);

	error = map_object_get_type(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(type, ==, MAP_OBJECT_UNKNOWN);
}


/*int map_object_move(map_object_h object,
			  const double latitude,
			  const double longitude);*/
void utc_map_object_move_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	/* Prepare coordinates to move to */
	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(25.6, 47.8, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* Move the object to the given coordinates */
	error = map_object_move(e.o, coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* Destroy coordinates, we don't need it anymore */
	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);


#if 0
	/* Get the marker data */
	map_view_marker_data_h marker_data = NULL;
	error = map_object_get_shape_data(e.o, &marker_data);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* Get the coordinates of the marker */
	coords = NULL;
	error = map_view_marker_data_get_coordinates(marker_data, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(coords);

	/* Check if the marker position is correct */
	double latitude = .0;
	double longitude = .0;
	maps_coordinates_get_latitude(coords, &latitude);
	maps_coordinates_get_longitude(coords, &longitude);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(latitude, ==, 25.6);
	g_assert_cmpfloat(longitude, ==, 47.8);

	/* Destroy coordinates, we don't need it anymore */
	maps_coordinates_destroy(coords);
#endif
}

void utc_map_object_move_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	/* Prepare coordinates to move to */
	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(25.6, 47.8, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_move(NULL, coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_move(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/* Destroy coordinates, we don't need it anymore */
	maps_coordinates_destroy(coords);
}

/*int map_object_set_visible(map_object_h object,
				   const bool visible);*/
void utc_map_object_set_visible_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_set_visible(e.o, true);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	bool visible = false;
	error = map_object_get_visible(e.o, &visible);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(visible);

	error = map_object_set_visible(e.o, false);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_get_visible(e.o, &visible);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(!visible);
}

void utc_map_object_set_visible_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_set_visible(NULL, true);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int map_object_get_visible(map_object_h object, bool *visible);*/
void utc_map_object_get_visible_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	bool visible = true;
	int error = map_object_get_visible(e.o, &visible);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(!visible);
}

void utc_map_object_get_visible_n(void)
{
	bool visible = true;
	int error = map_object_get_visible(NULL, &visible);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(visible);
}


/*int map_object_hit_test(map_object_h text,
			      const maps_coordinates_h coordinates,
			      bool *is_hit);*/
void utc_map_object_hit_test_p(void)
{
#if 0
	test_env e(MAP_OBJECT_MARKER);

	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(37.4 + 1.1, 127.2 + 1.1, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	bool is_hit = false;
	error = map_object_hit_test(e.o, coords, &is_hit);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* TODO: check after implementation of hit_test API */
	/*g_assert(is_hit);

	is_hit = true;
	error = map_object_hit_test(e.o,
					      37.4 - 1.1, 127.2 - 1.1,
					      &is_hit);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(!is_hit);*/
#endif
}

void utc_map_object_hit_test_n(void)
{
#if 0
	test_env e(MAP_OBJECT_MARKER);

	maps_coordinates_h coords = NULL;
	int error = maps_coordinates_create(37.4 + 1.1, 127.2 + 1.1, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	bool is_hit = true;
	error = map_object_hit_test(NULL, coords, &is_hit);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(is_hit);

	error = map_object_hit_test(e.o, NULL, &is_hit);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(is_hit);

	error = map_object_hit_test(e.o, coords, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(is_hit);

	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* TODO: check after implementation of hit_test API */
	/*
	is_hit = true;
	error = map_object_hit_test(e.o,
					      37.4 + 100.1, 127.2 + 1.1,
					      &is_hit);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(is_hit);

	is_hit = true;
	error = map_object_hit_test(e.o,
					      37.4 - 200.1, 127.2 + 1.1,
					      &is_hit);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(is_hit);

	is_hit = true;
	error = map_object_hit_test(e.o,
					      37.4 + 1.1, 127.2 + 100.1,
					      &is_hit);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(is_hit);

	is_hit = true;
	error = map_object_hit_test(e.o,
					      37.4 + 1.1, 127.2 - 400.1,
					      &is_hit);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(is_hit);*/
#endif
}

/*int map_object_group_add_object(map_object_h group,
			       const map_object_h object);*/
void utc_map_object_group_add_object_p(void)
{
	test_env e(MAP_OBJECT_GROUP);

	map_object_h marker = NULL;
	int error = _map_object_create(MAP_OBJECT_MARKER,
					   &marker);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(marker);

	error = map_object_group_add_object(e.o, marker);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_group_add_object_n(void)
{
	test_env e(MAP_OBJECT_GROUP);

	map_object_h marker = NULL;
	int error = _map_object_create(MAP_OBJECT_MARKER,
					   &marker);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(marker);

	error = map_object_group_add_object(NULL, marker);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_group_add_object(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_destroy(marker);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/*int map_object_group_set_objects(map_object_h group,
				     const maps_item_list_h objects);*/
void utc_map_object_group_set_objects_p(void)
{
	/* TO BE REMOVED */
#if 0
	test_env e(MAP_OBJECT_GROUP);

	map_object_h m1 = NULL;
	int error = _map_object_create(MAP_OBJECT_MARKER, &m1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(m1);

	map_object_h m2 = NULL;
	error = _map_object_create(MAP_OBJECT_MARKER, &m2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(m2);

	maps_item_list_h list = NULL;
	error = maps_item_list_create(&list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(list);

	error = maps_item_list_append(list, m1, maps_item_no_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_item_list_append(list, m2, maps_item_no_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_group_set_objects(e.o, list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
#endif
}

void utc_map_object_group_set_objects_n(void)
{
	/* TO BE REMOVED */
#if 0
	test_env e(MAP_OBJECT_GROUP);

	maps_item_list_h list = NULL;
	int error = maps_item_list_create(&list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(list);

	error = map_object_group_set_objects(NULL, list);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_group_set_objects(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_list_destroy(list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
#endif
}


/*int map_object_group_remove_object(map_object_h group,
				  const map_object_h *object);*/
void utc_map_object_group_remove_object_p(void)
{
	test_env e(MAP_OBJECT_GROUP);
	/* TODO: */
}

void utc_map_object_group_remove_object_n(void)
{
	test_env e(MAP_OBJECT_GROUP);
	/* TODO: */
}


/*int map_object_group_foreach_object(const map_object_h group,
				   map_object_group_object_cb callback,
typedef bool(*map_object_group_object_cb) (int index, int total,
					      map_object_h object,
					      void *user_data);*/
static bool __utc_map_object_group_object_cb(int index, int total,
						 map_object_h object,
						 void *user_data)
{
	g_assert_cmpint(index, >=, 0);
	g_assert_cmpint(total, >, 0);
	g_assert(object);
	g_assert(user_data);

	map_object_type_e type = MAP_OBJECT_UNKNOWN;
	map_object_get_type(object, &type);
	g_assert_cmpint(type, ==, MAP_OBJECT_MARKER);

	test_env *e = (test_env *)user_data;
	e->iterations ++;

	return true;
}

void utc_map_object_group_foreach_object_p(void)
{
	test_env e(MAP_OBJECT_GROUP);

	map_object_h m1 = NULL;
	int error = _map_object_create(MAP_OBJECT_MARKER, &m1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(m1);
	error = map_object_group_add_object(e.o, m1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	map_object_h m2 = NULL;
	error = _map_object_create(MAP_OBJECT_MARKER, &m2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(m2);
	error = map_object_group_add_object(e.o, m2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);


	error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 2);

}

void utc_map_object_group_foreach_object_n(void)
{
	test_env e(MAP_OBJECT_GROUP);

	int error = map_object_group_foreach_object(NULL,
				   __utc_map_object_group_object_cb,
				   &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);

	error = map_object_group_foreach_object(e.o,
				   NULL,
				   &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);
}


/*int map_object_polyline_add_point(map_object_h poly,
					const maps_coordinates_h point);*/
void utc_map_object_poly_add_point_p(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	maps_coordinates_h c = NULL;
	int error = maps_coordinates_create(38.4, 128.0, &c);
	g_assert(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_polyline_add_point(e.o, c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_poly_add_point_n(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	maps_coordinates_h c = NULL;
	int error = maps_coordinates_create(38.4, 128.0, &c);
	g_assert(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_polyline_add_point(NULL, c);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_polyline_add_point(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_coordinates_destroy(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}


/*int map_object_polyline_set_polyline(map_object_h poly,
				  const maps_coordinates_list_h points);*/
void utc_map_object_poly_set_points_p(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	maps_coordinates_h c1 = NULL;
	int error = maps_coordinates_create(38.4, 128.0, &c1);
	g_assert(c1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h c2 = NULL;
	error = maps_coordinates_create(39.4, 129.0, &c2);
	g_assert(c1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_list_h list = NULL;
	error = maps_coordinates_list_create(&list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(list);

	error = maps_coordinates_list_append(list, c1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_list_append(list, c2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);


	/* Test */
	error = map_object_polyline_set_polyline(e.o, list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);


	error = maps_coordinates_list_destroy(list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(c1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(c2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_poly_set_points_n(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	maps_coordinates_list_h list = NULL;
	int error = maps_coordinates_list_create(&list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(list);

	/* Test */
	error = map_object_polyline_set_polyline(NULL, list);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_polyline_set_polyline(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_coordinates_list_destroy(list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}


/*int map_object_polyline_foreach_point(map_object_h poly,
				       map_object_poly_point_cb callback,
typedef bool(*map_object_poly_point_cb) (int index, int total,
					      maps_coordinates_h point,
					      void *user_data);*/
static bool __utc_map_object_poly_point_cb(int index, int total,
					       maps_coordinates_h point,
					       void *user_data)
{
	g_assert_cmpint(index, >=, 0);
	g_assert_cmpint(total, >, 0);
	g_assert(point);
	g_assert(user_data);

	test_env *e = (test_env *)user_data;
	e->iterations ++;

	int error = maps_coordinates_destroy(point);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	return true;
}

void utc_map_object_poly_foreach_point_p(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	maps_coordinates_h c1 = NULL;
	int error = maps_coordinates_create(38.4, 128.0, &c1);
	g_assert(c1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h c2 = NULL;
	error = maps_coordinates_create(39.4, 129.0, &c2);
	g_assert(c1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_list_h list = NULL;
	error = maps_coordinates_list_create(&list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(list);

	error = maps_coordinates_list_append(list, c1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_list_append(list, c2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_polyline_set_polyline(e.o, list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* Test */
	error = map_object_polyline_foreach_point(e.o,
				       __utc_map_object_poly_point_cb,
				       &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 2);

	error = maps_coordinates_list_destroy(list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(c1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(c2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_poly_foreach_point_n(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	int error = map_object_polyline_foreach_point(NULL,
				       __utc_map_object_poly_point_cb,
				       &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);

	error = map_object_polyline_foreach_point(e.o, NULL, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iterations, ==, 0);
}


/*int map_object_polyline_set_color(map_object_h polyline,
				       const unsigned char r,
				       const unsigned char g,
				       const unsigned char b,
				       const unsigned char a);*/
void utc_map_object_polyline_set_color_p(void)
{
	test_env e(MAP_OBJECT_POLYLINE);
	int error = map_object_polyline_set_color(e.o,
						       0x11, 0x22, 0x33, 0x44);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_polyline_set_color_n(void)
{
	test_env e(MAP_OBJECT_POLYLINE);
	int error = map_object_polyline_set_color(NULL,
						       0x11, 0x22, 0x33, 0x44);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_polyline_get_color(const map_object_h
				       polyline,
				       unsigned char *r,
				       unsigned char *g,
				       unsigned char *b,
				       unsigned char *a);*/
void utc_map_object_polyline_get_color_p(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	int error = map_object_polyline_set_color(e.o,
						       0x11, 0x22, 0x33, 0x44);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	unsigned char r, g, b, a;
	error = map_object_polyline_get_color(e.o, &r, &g, &b, &a);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(r, ==, 0x11);
	g_assert_cmpint(g, ==, 0x22);
	g_assert_cmpint(b, ==, 0x33);
	g_assert_cmpint(a, ==, 0x44);
}

void utc_map_object_polyline_get_color_n(void)
{
	test_env e(MAP_OBJECT_POLYLINE);
	unsigned char r, g, b, a;
	int error = map_object_polyline_get_color(NULL, &r, &g, &b, &a);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_polyline_set_width(map_object_h polyline,
				       const int width);*/
void utc_map_object_polyline_set_width_p(void)
{
	test_env e(MAP_OBJECT_POLYLINE);
	int error = map_object_polyline_set_width(e.o, 3);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_polyline_set_width_n(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	int error = map_object_polyline_set_width(NULL, 3);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_polyline_set_width(e.o, -3);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_polyline_get_width(const map_object_h polyline,
				       int *width);*/
void utc_map_object_polyline_get_width_p(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	int error = map_object_polyline_set_width(e.o, 3);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	int width = 0;
	error = map_object_polyline_get_width(e.o, &width);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(width , ==, 3);
}

void utc_map_object_polyline_get_width_n(void)
{
	test_env e(MAP_OBJECT_POLYLINE);

	int width = 0;
	int error = map_object_polyline_get_width(NULL, &width);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_polyline_get_width(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_polygon_set_fill_color(map_object_h polygon,
					   const unsigned char r,
					   const unsigned char g,
					   const unsigned char b,
					   const unsigned char a);*/
void utc_map_object_polygon_set_fill_color_p(void)
{
	test_env e(MAP_OBJECT_POLYGON);
	int error = map_object_polygon_set_fill_color(e.o,
							   0x11,
							   0x22,
							   0x33,
							   0x44);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_polygon_set_fill_color_n(void)
{
	test_env e(MAP_OBJECT_POLYGON);
	int error = map_object_polygon_set_fill_color(NULL,
							   0x11,
							   0x22,
							   0x33,
							   0x44);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_polygon_get_fill_color(const map_object_h polygon,
					   unsigned char *r,
					   unsigned char *g,
					   unsigned char *b,
					   unsigned char *a);*/
void utc_map_object_polygon_get_fill_color_p(void)
{
	test_env e(MAP_OBJECT_POLYGON);
	int error = map_object_polygon_set_fill_color(e.o,
							   0x11,
							   0x22,
							   0x33,
							   0x44);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	unsigned char r, g, b, a;
	error = map_object_polygon_get_fill_color(e.o,
						       &r, &g, &b, &a);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(r, ==, 0x11);
	g_assert_cmpint(g, ==, 0x22);
	g_assert_cmpint(b, ==, 0x33);
	g_assert_cmpint(a, ==, 0x44);
}

void utc_map_object_polygon_get_fill_color_n(void)
{
	test_env e(MAP_OBJECT_POLYGON);
	unsigned char r, g, b, a;
	int error = map_object_polygon_get_fill_color(NULL,
						       &r, &g, &b, &a);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_marker_set_coordinates(map_object_h marker,
				   const maps_coordinates_h coordinates);*/
void utc_map_object_marker_set_coordinates_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	maps_coordinates_h c = NULL;
	int error = maps_coordinates_create(25.6, 47.8, &c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_marker_set_coordinates(e.o, c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_marker_set_coordinates_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	maps_coordinates_h c = NULL;
	int error = maps_coordinates_create(25.6, 47.8, &c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_marker_set_coordinates(NULL, c);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_set_coordinates(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_coordinates_destroy(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}


/*int map_object_marker_resize(map_object_h marker,
					  const int screen_width,
					  const int screen_height);*/
void utc_map_object_marker_set_screen_size_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_resize(e.o, 13, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_marker_set_screen_size_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_resize(NULL, 13, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_resize(e.o, -13, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_resize(e.o, 13, -42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_marker_set_imate_file(map_object_h marker,
					const char *file_path);*/
void utc_map_object_marker_set_file_path_p(void)
{
	/* TO BE REMOVED */
#if 0
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_set_image_file(e.o, "marker.png");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
#endif
}

void utc_map_object_marker_set_file_path_n(void)
{
	/* TO BE REMOVED */
#if 0
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_set_image_file(NULL, "marker.png");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_set_image_file(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
#endif
}


/*int map_object_marker_set_type(map_object_h marker,
				   const map_marker_type_e type);*/
void utc_map_object_marker_set_type_p(void)
{
	/* TO BE REMOVED */
#if 0
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_set_type(e.o, MAP_MARKER_POI);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
#endif
}

void utc_map_object_marker_set_type_n(void)
{
	/* TO BE REMOVED */
#if 0
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_set_type(NULL, MAP_MARKER_POI);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_set_type(e.o,
			map_marker_type_e(MAP_MARKER_POI - 100));
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
#endif
}


/*int map_object_marker_set_text(map_object_h marker,
				   const char *text);*/
void utc_map_object_marker_set_text_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_set_text(e.o, "Demo Marker");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_marker_set_text_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_set_text(NULL, "Demo Marker");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_set_text(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_marker_get_coordinates(const map_object_h marker,
					  maps_coordinates_h *coordinates);*/
void utc_map_object_marker_get_coordinates_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	maps_coordinates_h c = NULL;
	int error = maps_coordinates_create(25.6, 47.8, &c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_marker_set_coordinates(e.o, c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_marker_get_coordinates(e.o, &c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	double lat = 0., lon = 0.;
	error = maps_coordinates_get_latitude_longitude(c, &lat, &lon);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(lat, <=, 30.);
	g_assert_cmpfloat(lon, >=, 30.);

	error = maps_coordinates_destroy(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_object_marker_get_coordinates_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	maps_coordinates_h c = NULL;

	int error = map_object_marker_get_coordinates(NULL, &c);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_get_coordinates(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_marker_get_size(map_object_h marker,
					  int *screen_width,
					  int *screen_height);*/
void utc_map_object_marker_get_screen_size_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_resize(e.o, 13, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	int width = 0, height = 0;
	error = map_object_marker_get_size(e.o, &width, &height);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(width, ==, 13);
	g_assert_cmpint(height, ==, 42);
}

void utc_map_object_marker_get_screen_size_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int width = 0, height = 0;
	int error = map_object_marker_get_size(NULL, &width, &height);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_marker_get_image_file(const map_object_h marker,
					char **file_path);*/
void utc_map_object_marker_get_image_file_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_set_image_file(e.o, "marker.png");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char *file_path = NULL;
	error = map_object_marker_get_image_file(e.o, &file_path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(file_path);
	g_assert_cmpint(g_strcmp0(file_path, "marker.png"), ==, 0);
	g_free(file_path);
}

void utc_map_object_marker_get_image_file_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	char *file_path = NULL;
	int error = map_object_marker_get_image_file(NULL, &file_path);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_get_image_file(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_marker_get_type(const map_object_h marker,
				   map_marker_type_e *type);*/
void utc_map_object_marker_get_type_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

#if 0
	int error = map_object_marker_set_type(e.o, MAP_MARKER_POI);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
#endif

	map_marker_type_e type = MAP_MARKER_NONE;
	int error = map_object_marker_get_type(e.o, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*g_assert_cmpint(type, ==, MAP_MARKER_POI);*/
}

void utc_map_object_marker_get_type_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	map_marker_type_e type = MAP_MARKER_NONE;
	int error = map_object_marker_get_type(NULL, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_get_type(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_object_marker_get_text(const map_object_h marker,
				   char **text);*/
void utc_map_object_marker_get_text_p(void)
{
	test_env e(MAP_OBJECT_MARKER);

	int error = map_object_marker_set_text(e.o, "Demo Marker");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char *text = NULL;
	error = map_object_marker_get_text(e.o, &text);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(text);
	g_assert_cmpint(g_strcmp0(text, "Demo Marker"), ==, 0);
	g_free(text);
}

void utc_map_object_marker_get_text_n(void)
{
	test_env e(MAP_OBJECT_MARKER);

	char *text = NULL;
	int error = map_object_marker_get_text(NULL, &text);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_marker_get_text(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}
