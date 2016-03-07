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

#include "marker_constructor_test.h"
#include "marker_constructor.h"
#include "maps_object.h"
#include <glib.h>


void utc_construct_marker_p(void)
{
	/* Construct a marker */
	view::marker_constructor mc;
	map_object_h marker = mc.construct(maps::coordinates(10.1, 20.1),
					   "test_marker.png",
					   MAP_MARKER_PIN);
	g_assert_cmpint(mc.get_error(), ==, MAPS_ERROR_NONE);
	g_assert(marker);

	/* Check Marker Coordinates */
	maps_coordinates_h coords = NULL;
	int error = map_object_marker_get_coordinates(marker, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	double lat = 0, lon = 0;
	error = maps_coordinates_get_latitude_longitude(coords, &lat, &lon);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(lat, >, 10.);
	g_assert_cmpfloat(lon, >, 20.);
	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* Check Marker Image File */
	char *file_path = NULL;
	error = map_object_marker_get_image_file(marker, &file_path);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(file_path);
	g_assert_cmpstr(file_path, ==, "test_marker.png");
	g_free(file_path);

	/* Check Marker Type */
	map_marker_type_e type = MAP_MARKER_NONE;
	error = map_object_marker_get_type(marker, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(type, ==, MAP_MARKER_PIN);

	error = map_object_destroy(marker);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_construct_marker_n(void)
{
	view::marker_constructor mc;

	/* Negative Test 1 */
	map_object_h marker = mc.construct(NULL,
					   "test_marker.png",
					   MAP_MARKER_PIN);
	g_assert_cmpint(mc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!marker);

	/* Negative Test 2 */
	marker = mc.construct(maps::coordinates(10.1, 20.1),
			      NULL,
			      MAP_MARKER_PIN);
	g_assert_cmpint(mc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!marker);

	/* Negative Test 3 */
	marker = mc.construct(maps::coordinates(10.1, 20.1),
			      "test_marker.png",
			      map_marker_type_e(-2));
	g_assert_cmpint(mc.get_error(), ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!marker);
}
