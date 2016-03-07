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

#include "polyline_constructor_test.h"
#include "polyline_constructor.h"
#include "maps_object.h"
#include <glib.h>


static bool __utc_map_object_poly_point_cb(int index, int total,
						maps_coordinates_h point,
						void *user_data)
{
	if(user_data) {
		int *iterations = (int *)user_data;
		*iterations = *iterations + 1;
	}
	int error = maps_coordinates_destroy(point);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	return true;
}

void utc_construct_polyline_p(void)
{
	/* Construct polyline */
	view::polyline_constructor pc;

	maps_coordinates_list_h path = NULL;
	maps_coordinates_list_create(&path);
	double lats[] = {-85, -75, -65, -50, -40, -30, -45};
	double lons[] = { 80, 110, 100,  90,  70,  80, 140};
	for(unsigned int i = 0; i < (sizeof(lats) / sizeof(lats[0])); i ++) {
		maps_coordinates_h c = NULL;
		maps_coordinates_create(lats[i], lons[i], &c);
		maps_coordinates_list_append(path, c);
		maps_coordinates_destroy(c);
	}

	maps_view_object_h polyline = pc.construct(path, 0x01, 0x02, 0x03, 0x04, 3);
	g_assert_cmpint(pc.get_error(), ==, MAPS_ERROR_NONE);
	g_assert(polyline);

	maps_coordinates_list_destroy(path);

	/* Check Polygon path */
	int iterations = 0;
	int error = maps_view_object_polyline_foreach_point(polyline,
				       __utc_map_object_poly_point_cb,
				       &iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(iterations, ==, 7);

	/* Check fill color */
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
	error = maps_view_object_polyline_get_color(polyline, &r, &g, &b, &a);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(r, ==, 0x01);
	g_assert_cmpint(g, ==, 0x02);
	g_assert_cmpint(b, ==, 0x03);
	g_assert_cmpint(a, ==, 0x04);

	/* Check line width */
	int width = 0;
	error = maps_view_object_polyline_get_width(polyline, &width);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(width, ==, 3);

	error = maps_view_object_destroy(polyline);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_construct_polyline_n(void)
{
	view::polyline_constructor pc;

	/* Negative test 1 */
	maps_view_object_h polyline = pc.construct(NULL, 0x01, 0x02, 0x03, 0x04, 1);
	g_assert_cmpint(pc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!polyline);


	/* Negative test 2 */
	maps_coordinates_list_h path = NULL;
	maps_coordinates_list_create(&path);
	maps_coordinates_list_append(path, maps::coordinates(-85, 80));
	polyline = pc.construct(path, 0x01, 0x02, 0x03, 0x04, -1);
	g_assert_cmpint(pc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!polyline);

	maps_coordinates_list_destroy(path);
}
