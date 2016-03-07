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

#include "route_constructor_test.h"
#include "route_constructor.h"
#include "maps_object.h"
#include <glib.h>

#ifdef TIZEN_3_0_NEXT_MS
class utc_route {
public:
	maps::route route;
public:
	utc_route();
};

utc_route::utc_route()
{
	maps_route_set_origin(route, maps::coordinates(-85.0, 60.0));
	maps_route_set_destination(route, maps::coordinates(-45.0, 120.0));

	maps::item_list <maps::coordinates> path;
	path.add(maps::coordinates(-85, 80));
	path.add(maps::coordinates(-75, 110));
	path.add(maps::coordinates(-65, 100));
	path.add(maps::coordinates(-50, 90));
	path.add(maps::coordinates(-40, 70));
	path.add(maps::coordinates(-30, 80));
	path.add(maps::coordinates(-45, 140));
	maps_route_set_path(route, path);

	maps::item_list <maps::route_maneuver> maneuvers;
	maps::route_maneuver m1, m2;
	maneuvers.add(m1);
	maneuvers.add(m2);

	maps::item_list <maps::route_segment> segments;
	maps::route_segment s1, s2, s3;

	/* First segment with detailed path */
	maps_route_segment_set_origin(s1, maps::coordinates(-85.0, 60.0));
	maps_route_segment_set_destination(s1, maps::coordinates(-65.0, 80.0));
	maps::item_list <maps::coordinates> s1_path;
	s1_path.add(maps::coordinates(-85, 60));
	s1_path.add(maps::coordinates(-79, 55));
	s1_path.add(maps::coordinates(-75, 60));
	s1_path.add(maps::coordinates(-73, 85));
	s1_path.add(maps::coordinates(-69, 95));
	s1_path.add(maps::coordinates(-65, 80));
	maps_route_segment_set_path(s1, s1_path);
	maps_route_segment_set_maneuvers(s1, maneuvers);
	segments.add(s1);

	maps_route_segment_set_origin(s2, maps::coordinates(-65.0, 80.0));
	maps_route_segment_set_destination(s2, maps::coordinates(-40.0, 50.0));
	maps_route_segment_set_maneuvers(s2, maneuvers);
	segments.add(s2);

	maps_route_segment_set_origin(s3, maps::coordinates(-40.0, 50.0));
	maps_route_segment_set_destination(s3, maps::coordinates(-45.0, 120.0));
	maps_route_segment_set_maneuvers(s3, maneuvers);
	segments.add(s3);

	maps_route_set_segments(route, segments);
}

void utc_construct_route_p(void)
{
	utc_route r;

	/* Construct route view object */
	view::route_constructor rc;

	maps_view_object_h route_object = rc.construct(NULL,
						      r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_NONE);
	g_assert(route_object);

	/* Check if the route object includes route handle */
	maps_route_h route_handle = NULL;
	int error = maps_view_object_route_get_content(route_object,
						      &route_handle);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(route_handle);

	error = maps_route_destroy(route_handle);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_view_object_destroy(route_object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_construct_route_n(void)
{
	view::route_constructor rc;
	maps_view_object_h route_object = rc.construct(NULL, NULL);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!route_object);
}



	/* !!! The old marker constructor will be deleted soom!!! */
#if 0
static bool __utc_map_object_group_object_cb(int index, int total,
						  maps_view_object_h object,
						  void *user_data)
{
	if(user_data) {
		int *iterations = (int *)user_data;
		*iterations = *iterations + 1;
	}
	g_assert(object);
	return true;
}

void utc_construct_route_p(void)
{
	/* Preparing test route */
	utc_route r;


	/* Construct route view object */
	view::route_constructor rc;
	maps_view_object_h route_object = rc.construct(r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_NONE);
	g_assert(route_object);

	/* Check if the route object includes route handle */
	maps_route_h route_handle = NULL;
	int error = map_object_group_get_route(route_object,
						     &route_handle);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(route_handle);

	error = maps_route_destroy(route_handle);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* Check if route includes several objects */
	int iterations = 0;
	error = map_object_group_foreach_object(route_object,
				   __utc_map_object_group_object_cb,
				   &iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(iterations, >, 0);

	error = maps_view_object_destroy(route_object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_construct_route_n(void)
{
	view::route_constructor rc;
	maps_view_object_h route_object = rc.construct(NULL);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!route_object);
}


/*----------------------------------------------------------------------------*/
/*void static const char *get_start_marker_image();*/
void utc_get_start_marker_p(void)
{
	g_assert(view::route_constructor::get_start_marker_image());
}

/*static const char *get_finish_marker_image();*/
void utc_get_finish_marker_p(void)
{
	g_assert(view::route_constructor::get_finish_marker_image());
}

/*static const char *get_default_marker_image();*/
void utc_get_default_marker_p(void)
{
	g_assert(view::route_constructor::get_default_marker_image());
}

/*----------------------------------------------------------------------------*/
class utc_route_constructor : public view::route_constructor {
public:
	static int utc_add_marker(maps_view_object_h route_object,
				  const maps_coordinates_h coords,
				  maps_view_marker_type_e type);
	void utc_add_start_marker(maps_view_object_h route_object,
				  maps_route_h route);
	void utc_add_finish_marker(maps_view_object_h route_object,
				   maps_route_h route);

	static int utc_add_trajectory(maps_view_object_h route_object,
				      maps_item_list_h route_points);

	static bool utc_add_trajectory_markers(int index,
					       int total,
					       void *data,
					       void *user_data);

	void utc_add_route_path(maps_view_object_h route_object,
				maps_route_h route);
	static bool utc_collect_path_points(int index, int total,
					    maps_coordinates_h coordinates,
					    void *user_data);

	void utc_add_route_segments(maps_view_object_h route_object,
				    maps_route_h route);
	static bool utc_collect_segments(int index, int total,
					 maps_route_segment_h segment,
					 void *user_data);
};

int utc_route_constructor::utc_add_marker(maps_view_object_h route_object,
					  const maps_coordinates_h coords,
					  maps_view_marker_type_e type)
{
	return add_marker(route_object, coords, type);
}

void utc_route_constructor::utc_add_start_marker(maps_view_object_h route_object,
						 maps_route_h route)
{
	add_start_marker(route_object, route);
}

void utc_route_constructor::utc_add_finish_marker(maps_view_object_h
						  route_object,
						  maps_route_h route)
{
	add_finish_marker(route_object, route);
}

int utc_route_constructor::utc_add_trajectory(maps_view_object_h route_object,
					      maps_item_list_h route_points)
{
	return add_trajectory(route_object, route_points);
}

bool utc_route_constructor::utc_add_trajectory_markers(int index,
						       int total,
						       void *data,
						       void *user_data)
{
	return add_trajectory_markers(index, total, data, user_data);
}

void utc_route_constructor::utc_add_route_path(maps_view_object_h route_object,
					       maps_route_h route)
{
	add_route_path(route_object, route);
}

bool utc_route_constructor::utc_collect_path_points(int index, int total,
						    maps_coordinates_h
						    coordinates,
						    void *user_data)
{
	return collect_path_points(index, total, coordinates, user_data);
}

void utc_route_constructor::utc_add_route_segments(maps_view_object_h
						   route_object,
						   maps_route_h route)
{
	add_route_segments(route_object, route);
}

bool utc_route_constructor::utc_collect_segments(int index, int total,
						 maps_route_segment_h segment,
						 void *user_data)
{
	return collect_segments(index, total, segment, user_data);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_view_object_h o;
	int iterations;
 public:
	test_env() : o(NULL), iterations(0)
	{
		/* Create a test view object */
		int error = map_object_create(MAP_OBJECT_GROUP, &o);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(o);
	}
	~test_env()
	{
		/* Destroy a test view object */
		const int error = maps_view_object_destroy(o);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};


/*----------------------------------------------------------------------------*/
/*static int add_marker(maps_view_object_h route_object,
				       const maps_coordinates_h coords,
				       maps_view_marker_type_e type);*/
void utc_add_marker_p(void)
{
	test_env e;
	utc_route_constructor rc;

	int error = rc.utc_add_marker(e.o,
				      maps::coordinates(-85.0, 60.0),
				      MAP_MARKER_PIN);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(e.iterations, ==, 1);

}
void utc_add_marker_n(void)
{
	test_env e;
	utc_route_constructor rc;

	/* Negative test 1 */
	int error = rc.utc_add_marker(NULL,
				      maps::coordinates(-85.0, 60.0),
				      MAP_MARKER_PIN);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/* Negative test 2 */
	error = rc.utc_add_marker(e.o,
				  NULL,
				  MAP_MARKER_PIN);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NOT_FOUND);
	g_assert_cmpint(e.iterations, ==, 0);
}

/*void add_start_marker(maps_view_object_h route_object,
		      maps_route_h route);*/
void utc_add_start_marker_p(void)
{
	test_env e;
	utc_route_constructor rc;
	utc_route r;

	rc.utc_add_start_marker(e.o, r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_NONE);

	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 1);
}
void utc_add_start_marker_n(void)
{
	test_env e;
	utc_route_constructor rc;
	utc_route r;

	rc.utc_add_start_marker(NULL, r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);

	rc.utc_add_start_marker(e.o, NULL);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);

	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NOT_FOUND);
	g_assert_cmpint(e.iterations, ==, 0);

}

/*void add_finish_marker(maps_view_object_h route_object,
		       maps_route_h route);*/
void utc_add_finish_marker_p(void)
{
	test_env e;
	utc_route_constructor rc;
	utc_route r;

	rc.utc_add_finish_marker(e.o, r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_NONE);

	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 1);
}
void utc_add_finish_marker_n(void)
{
	test_env e;
	utc_route_constructor rc;
	utc_route r;

	rc.utc_add_finish_marker(NULL, r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);

	rc.utc_add_finish_marker(e.o, NULL);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);

	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NOT_FOUND);
	g_assert_cmpint(e.iterations, ==, 0);
}

/*static int add_trajectory(maps_view_object_h route_object,
			  maps_item_list_h route_points);*/
void utc_add_trajectory_p(void)
{
	test_env e;
	utc_route_constructor rc;

	maps::item_list <maps::coordinates> path;
	path.add(maps::coordinates(-85, 80));
	path.add(maps::coordinates(-75, 110));
	path.add(maps::coordinates(-65, 100));

	int error = rc.utc_add_trajectory(e.o, path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, >=, 3);
}
void utc_add_trajectory_n(void)
{
	test_env e;
	utc_route_constructor rc;

	maps::item_list <maps::coordinates> path;
	path.add(maps::coordinates(-85, 80));
	path.add(maps::coordinates(-75, 110));
	path.add(maps::coordinates(-65, 100));

	/* Negative test 1 */
	int error = rc.utc_add_trajectory(NULL, path);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = rc.utc_add_trajectory(e.o, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NOT_FOUND);
	g_assert_cmpint(e.iterations, ==, 0);
}

/*static bool add_trajectory_markers(int index,
				   int total,
				   void *data,
				   void *user_data);*/
void utc_add_trajectory_markers_p(void)
{
	test_env e;
	utc_route_constructor rc;

	bool ret = rc.utc_add_trajectory_markers(0, 1,
						 maps::coordinates(-75, 110),
						 e.o);
	g_assert(ret);
	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, ==, 1);
}
void utc_add_trajectory_markers_n(void)
{
	test_env e;
	utc_route_constructor rc;

	/* Negative test 1 */
	bool ret = rc.utc_add_trajectory_markers(0, 1, NULL, e.o);
	g_assert(!ret);

	/* Negative test 2 */
	ret = rc.utc_add_trajectory_markers(0, 1,
					    maps::coordinates(-75, 110),
					    NULL);
	g_assert(!ret);


	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NOT_FOUND);
	g_assert_cmpint(e.iterations, ==, 0);
}

/*void add_route_path(maps_view_object_h route_object,
		    maps_route_h route);*/
void utc_add_route_path_p(void)
{
	test_env e;
	utc_route_constructor rc;
	utc_route r;

	rc.utc_add_route_path(e.o, r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_NONE);
	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, >, 0);
}
void utc_add_route_path_n(void)
{
	test_env e;
	utc_route_constructor rc;
	utc_route r;

	/* Negative test 1 */
	rc.utc_add_route_path(NULL, r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);

	/* Negative test 2 */
	rc.utc_add_route_path(e.o, NULL);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);

	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NOT_FOUND);
	g_assert_cmpint(e.iterations, ==, 0);
}

/*static bool collect_path_points(int index, int total,
				maps_coordinates_h coordinates,
				void *user_data);*/
void utc_collect_path_points_p(void)
{
	utc_route_constructor rc;
	maps::item_list <maps::coordinates> path;
	bool ret = rc.utc_collect_path_points(0, 1,
					maps::coordinates(-75, 110).clone(),
					path);
	g_assert(ret);
}
void utc_collect_path_points_n(void)
{
	utc_route_constructor rc;
	bool ret = rc.utc_collect_path_points(0, 1, NULL, NULL);
	g_assert(!ret);
}

/*void add_route_segments(maps_view_object_h route_object,
			maps_route_h route);*/
void utc_add_route_segments_p(void)
{
	test_env e;
	utc_route_constructor rc;
	utc_route r;

	rc.utc_add_route_segments(e.o, r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_NONE);

	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, >, 0);
}
void utc_add_route_segments_n(void)
{
	test_env e;
	utc_route_constructor rc;
	utc_route r;

	/* Negative test 1 */
	rc.utc_add_route_segments(NULL, r.route);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);

	/* Negative test 2 */
	rc.utc_add_route_segments(e.o, NULL);
	g_assert_cmpint(rc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);

	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NOT_FOUND);
	g_assert_cmpint(e.iterations, ==, 0);
}

/*static bool collect_segments(int index, int total,
			     maps_route_segment_h segment,
			     void *user_data);*/
void  utc_collect_segments_p(void)
{
	test_env e;
	utc_route_constructor rc;

	/* Preparing test segment */
	maps::route_segment s1;
	maps_route_segment_set_origin(s1, maps::coordinates(-85.0, 60.0));
	maps_route_segment_set_destination(s1, maps::coordinates(-65.0, 80.0));

	maps::item_list <maps::coordinates> s1_path;
	s1_path.add(maps::coordinates(-85, 60));
	s1_path.add(maps::coordinates(-79, 55));
	s1_path.add(maps::coordinates(-75, 60));
	s1_path.add(maps::coordinates(-73, 85));
	s1_path.add(maps::coordinates(-69, 95));
	s1_path.add(maps::coordinates(-65, 80));
	maps_route_segment_set_path(s1, s1_path);

	maps::item_list <maps::route_maneuver> maneuvers;
	maps::route_maneuver m1, m2;
	maneuvers.add(m1);
	maneuvers.add(m2);
	maps_route_segment_set_maneuvers(s1, maneuvers);

	bool ret = rc.utc_collect_segments(0, 1, s1.clone(), e.o);
	g_assert(ret);

	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iterations, >, 0);
}
void  utc_collect_segments_n(void)
{
	test_env e;
	utc_route_constructor rc;

	/* Preparing test segment */
	maps::route_segment s1;
	maps_route_segment_set_origin(s1, maps::coordinates(-85.0, 60.0));
	maps_route_segment_set_destination(s1, maps::coordinates(-65.0, 80.0));

	maps::item_list <maps::coordinates> s1_path;
	s1_path.add(maps::coordinates(-85, 60));
	s1_path.add(maps::coordinates(-79, 55));
	s1_path.add(maps::coordinates(-75, 60));
	s1_path.add(maps::coordinates(-73, 85));
	s1_path.add(maps::coordinates(-69, 95));
	s1_path.add(maps::coordinates(-65, 80));
	maps_route_segment_set_path(s1, s1_path);

	maps::item_list <maps::route_maneuver> maneuvers;
	maps::route_maneuver m1, m2;
	maneuvers.add(m1);
	maneuvers.add(m2);
	maps_route_segment_set_maneuvers(s1, maneuvers);

	/* Negative test 1 */
	bool ret = rc.utc_collect_segments(0, 1, NULL, e.o);
	g_assert(!ret);

	/* Negative test 2 */
	ret = rc.utc_collect_segments(0, 1, s1.clone(), NULL);
	g_assert(!ret);

	int error = map_object_group_foreach_object(e.o,
				   __utc_map_object_group_object_cb,
				   &e.iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NOT_FOUND);
	g_assert_cmpint(e.iterations, ==, 0);
}
#endif

#endif /* TIZEN_3_0_NEXT_MS */
