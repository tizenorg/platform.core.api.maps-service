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

extern int _map_view_object_destroy(map_view_object_h object);

static bool __utc_map_view_object_poly_point_cb(int index, int total,
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

	maps::item_list <maps::coordinates> path;
	path.add(maps::coordinates(-85, 80));
	path.add(maps::coordinates(-75, 110));
	path.add(maps::coordinates(-65, 100));
	path.add(maps::coordinates(-50, 90));
	path.add(maps::coordinates(-40, 70));
	path.add(maps::coordinates(-30, 80));
	path.add(maps::coordinates(-45, 140));

	map_view_object_h polyline = pc.construct(NULL,
						  path,
						  0x01, 0x02, 0x03, 0x04,
						  3);
	g_assert_cmpint(pc.get_error(), ==, MAPS_ERROR_NONE);
	g_assert(polyline);

	/* Check Polygon path */
	int iterations = 0;
	int error = map_view_object_poly_foreach_point(polyline,
				       __utc_map_view_object_poly_point_cb,
				       &iterations);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(iterations, ==, 7);

	/* Check fill color */
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
	error = map_view_object_polyline_get_color(polyline, &r, &g, &b, &a);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(r, ==, 0x01);
	g_assert_cmpint(g, ==, 0x02);
	g_assert_cmpint(b, ==, 0x03);
	g_assert_cmpint(a, ==, 0x04);

	/* Check line width */
	int width = 0;
	error = map_view_object_polyline_get_width(polyline, &width);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(width, ==, 3);

	error = _map_view_object_destroy(polyline);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_construct_polyline_n(void)
{
	view::polyline_constructor pc;

	/* Negative test 1 */
	map_view_object_h polyline = pc.construct(NULL,
						  NULL,
						  0x01, 0x02, 0x03, 0x04,
						  1);
	g_assert_cmpint(pc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!polyline);


	/* Negative test 2 */
	maps::item_list <maps::coordinates> path;
	path.add(maps::coordinates(-85, 80));
	polyline = pc.construct(NULL,
				path,
				0x01, 0x02, 0x03, 0x04,
				-1);
	g_assert_cmpint(pc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!polyline);
}
