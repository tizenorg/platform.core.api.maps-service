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

#include "maps_coordinates_test.h"
#include "maps_coordinates.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_coordinates_create(double latitude, double longitude, double
*  altitude, const maps_coordinates_h* coordinates); */
void utc_maps_coordinates_create_p(void)
{
	maps_coordinates_h c = NULL;
	int error = maps_coordinates_create(44.4, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(c);

	error = maps_coordinates_destroy(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_coordinates_create_n(void)
{
	int error = maps_coordinates_create(44.4, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_coordinates_destroy(maps_coordinates_h coordinates); */
void utc_maps_coordinates_destroy_p(void)
{
	/* empty */
	/* same as utc_maps_coordinates_create_p */
}

void utc_maps_coordinates_destroy_n(void)
{
	const int error = maps_coordinates_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

class test_env
{
 public:
	maps_coordinates_h c;
	double lat;
	double lon;
#if _MAPS_COORDS_3D_
	double alt;
#endif
 public:
	 test_env():c(NULL), lat(44.4), lon(22.2)
#if _MAPS_COORDS_3D_
	, alt(33.3)
#endif
	{
		const int error = maps_coordinates_create(lat, lon,
#if _MAPS_COORDS_3D_
			alt,
#endif
			&c);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(c);
	}
	~test_env()
	{
		const int error = maps_coordinates_destroy(c);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/* int maps_coordinates_clone(const maps_coordinates_h origin,
*maps_coordinates_h* cloned); */
void utc_maps_coordinates_clone_p(void)
{
	test_env e;

	maps_coordinates_h cloned = NULL;
	int error = maps_coordinates_clone(e.c, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	double lat = .0, lon = .0;

#if _MAPS_COORDS_3D_
	double alt = .0;
#endif

	error = maps_coordinates_get_latitude(cloned, &lat);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(lat, ==, e.lat);

	error = maps_coordinates_get_longitude(cloned, &lon);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(lon, ==, e.lon);

#if _MAPS_COORDS_3D_
	error = maps_coordinates_get_altitude(cloned, &alt);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(alt, ==, e.alt);
#endif

	error = maps_coordinates_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_coordinates_clone_n(void)
{
	test_env e;

	maps_coordinates_h cloned = NULL;
	int error = maps_coordinates_clone(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!cloned);

	error = maps_coordinates_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!cloned);
}

/* int maps_coordinates_get_latitude(maps_coordinates_h coordinates,
*  double* latitude); */
/* int maps_coordinates_set_latitude(maps_coordinates_h coordinates,
*  double latitude); */
void utc_maps_coordinates_latitude_p(void)
{
	test_env e;
	double lat = .0;
	const int error = maps_coordinates_get_latitude(e.c, &lat);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(lat, ==, e.lat);
}

void utc_maps_coordinates_latitude_n(void)
{
	test_env e;
	double lat = .0;

	int error = maps_coordinates_get_latitude(NULL, &lat);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpfloat(lat, ==, 0);

	error = maps_coordinates_get_latitude(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpfloat(lat, ==, 0);
}

/* int maps_coordinates_get_longitude(maps_coordinates_h coordinates,
*  double* longitude); */
/* int maps_coordinates_set_longitude(maps_coordinates_h coordinates,
*  double longitude); */
void utc_maps_coordinates_longitude_p(void)
{
	test_env e;
	double lon = .0;
	const int error = maps_coordinates_get_longitude(e.c, &lon);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(lon, ==, e.lon);
}

void utc_maps_coordinates_longitude_n(void)
{
	test_env e;
	double lon = .0;

	int error = maps_coordinates_get_longitude(NULL, &lon);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpfloat(lon, ==, .0);

	error = maps_coordinates_get_longitude(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpfloat(lon, ==, .0);
}

#if _MAPS_COORDS_3D_
/* int maps_coordinates_get_altitude(maps_coordinates_h coordinates,
*  double* altitude); */
/* int maps_coordinates_set_altitude(maps_coordinates_h coordinates,
*  double altitude); */
void utc_maps_coordinates_altitude_p(void)
{
	test_env e;
	double alt = .0;
	const int error = maps_coordinates_get_altitude(e.c, &alt);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(alt, ==, e.alt);
}

void utc_maps_coordinates_altitude_n(void)
{
	test_env e;
	double alt = .0;

	int error = maps_coordinates_get_altitude(NULL, &alt);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpfloat(alt, ==, .0);

	error = maps_coordinates_get_altitude(e.c, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpfloat(alt, ==, .0);
}
#endif

