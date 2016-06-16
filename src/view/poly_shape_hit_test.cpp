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


#include "poly_shape_hit_test.h"
#include <climits>
#include <math.h>


//LCOV_EXCL_START
void view::poly_shape_hit_test::add_point(const float x, const float y)
{
	__x.push_back(x);
	__y.push_back(y);
}

bool view::poly_shape_hit_test::hit_test(const float x, const float y,
	const bool polygon, const int w) const
{
	if (__x.empty())
		return false;

	/* 1. Check if the point in the bounding box of a poly-figure */
	if (!hit_test_bounding_box(x, y))
		return false;

	/* 2. Check if the point near the polyline */
	if (hit_test_polyline(x, y, polygon, w))
		return true;

	/* 3. Check if the point near the vertices */
	if (hit_test_vertices(x, y))
		return true;

	/* 4. Check if the point inside the polygon (for polygon only) */
	if (polygon)
		if (pnpoly(x, y))
			return true;

	return false;
}

bool view::poly_shape_hit_test::hit_test_bounding_box(const float x, const float y) const
{
	float x_min = 1. * FLT_MAX;
	float x_max = 1. * FLT_MIN;
	float y_min = 1. * FLT_MAX;
	float y_max = 1. * FLT_MIN;
	for (unsigned int i = 0; i < __x.size(); i ++) {
		if (__x[i] < x_min)
			x_min = __x[i];
		if (__x[i] > x_max)
			x_max = __x[i];
		if (__y[i] < y_min)
			y_min = __y[i];
		if (__y[i] > y_max)
			y_max = __y[i];
	}
	x_min -= accuracy;
	x_max += accuracy;
	y_min -= accuracy;
	y_max += accuracy;

	return ((x >= x_min) && (x <= x_max)
		&& (y >= y_min) && (y <= y_max));
}

bool view::poly_shape_hit_test::hit_test_segment(const float x1, const float y1,
	const float x2, const float y2, const float x, const float y, const int w) const
{
	float a = x2 - x1;
	float b = y2 - y1;
	float c = sqrt(a * a + b * b);
	if (c == 0)
		return false;
	float sina = b / c;
	float cosa = a / c;

	float x_shifted = x - x1;
	float y_shifted = y - y1;
	float x_rotated = x_shifted * cosa + y_shifted * sina;
	float y_rotated = x_shifted * sina - y_shifted * cosa;

	if ((x_rotated >= 0) && (x_rotated <= c)
	   && (y_rotated >= (-1. * (accuracy + w / 2)))
	   && (y_rotated <=  (accuracy + w / 2)))
		return true;
	return false;
}

bool view::poly_shape_hit_test::hit_test_polyline(const float x, const float y,
					const bool polygon, const int w) const
{
	if (__x.size() < 1)
		return false;

	for (unsigned int i = 1; i < __x.size(); i ++) {
		if (hit_test_segment(__x[i - 1], __y[i - 1],
				    __x[i], __y[i], x, y, w))
			return true;
	}

	if (polygon) {
		/* Final section */
		if (hit_test_segment(__x[__x.size() - 1], __y[__x.size() - 1],
				    __x[0], __y[0], x, y))
			return true;
	}

	return false;
}

bool view::poly_shape_hit_test::hit_test_vertices(const float x, const float y) const
{
	for (unsigned int i = 0; i < __x.size(); i ++) {
		float cur_x_min = __x[i] - accuracy;
		float cur_x_max = __x[i] + accuracy;
		float cur_y_min = __y[i] - accuracy;
		float cur_y_max = __y[i] + accuracy;
		if ((x >= cur_x_min) && (x <= cur_x_max)
		    && (y >= cur_y_min) && (y <= cur_y_max))
			return true;
	}
	return false;
}

/*
 * PNPOLY
 * http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
 */
bool view::poly_shape_hit_test::pnpoly(const float x, const float y) const
{
	int i, j, c = 0;
	const int nvert = int(__x.size());
	for (i = 0, j = nvert-1; i < nvert; j = i++) {
		if ( ((__y[i] > y) != (__y[j] > y)) &&
		     (x < (__x[j] - __x[i]) * (y - __y[i])
		      / (__y[j] - __y[i]) + __x[i]) ) {
			c = !c;
		}
	}
	return (c != 0);
}
//LCOV_EXCL_STOP
