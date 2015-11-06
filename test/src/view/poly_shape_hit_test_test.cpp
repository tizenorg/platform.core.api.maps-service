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

#include "poly_shape_hit_test_test.h"
#include "poly_shape_hit_test.h"
#include <glib.h>

class utc_poly_shape_hit_test : public view::poly_shape_hit_test {
public:
	bool utc_hit_test_bounding_box(const float x, const float y) const;
	bool utc_hit_test_polyline(const float x, const float y,
				   const bool polygon) const;
	bool utc_hit_test_segment(const float x1, const float y1,
				  const float x2, const float y2,
				  const float x, const float y) const;
	bool utc_hit_test_vertices(const float x, const float y) const;
	bool utc_pnpoly(const float x, const float y) const;
};

bool utc_poly_shape_hit_test::utc_hit_test_bounding_box(const float x,
					       const float y) const
{
	return hit_test_bounding_box(x, y);
}

bool utc_poly_shape_hit_test::utc_hit_test_polyline(const float x, const float y,
					   const bool polygon) const
{
	return hit_test_polyline(x, y, polygon);
}

bool utc_poly_shape_hit_test::utc_hit_test_segment(const float x1, const float y1,
					  const float x2, const float y2,
					  const float x, const float y) const
{
	return hit_test_segment(x1, y1, x2, y2, x, y);
}

bool utc_poly_shape_hit_test::utc_hit_test_vertices(const float x, const float y) const
{
	return hit_test_vertices(x, y);
}

bool utc_poly_shape_hit_test::utc_pnpoly(float x, float y) const
{
	return pnpoly(x, y);
}

/*bool hit_test(const float x, const float y,
	      const bool polygon) const;*/
void utc_poly_shape_hit_test_hit_test_p(void)
{
	utc_poly_shape_hit_test pdt;
	pdt.add_point(100, 50);
	pdt.add_point(200, 50);
	g_assert(pdt.hit_test(150, 50, false));
}

void utc_poly_shape_hit_test_hit_test_p2(void)
{
	utc_poly_shape_hit_test pdt;

	/* \_/ form */
	float x[] = { 50, 200, 225, 175, 150, 100,  75,  25};
	float y[] = {250, 250, 100, 100, 200, 200, 100, 100};
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		pdt.add_point(x[i], y[i]);

	/* Hit test polygon */
	g_assert(pdt.hit_test(100, 225, true) == true);
	g_assert(pdt.hit_test(125, 150, true) == false);

	/* Hit test polyline */
	g_assert(pdt.hit_test(100, 225, false) == false);
	g_assert(pdt.hit_test(125, 150, false) == false);

	/* Hit test both polygon and polyline to include their vertices */
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++) {
		g_assert(pdt.hit_test(x[i], y[i], true) == true);
		g_assert(pdt.hit_test(x[i], y[i], false) == true);
	}
}

void utc_poly_shape_hit_test_hit_test_n(void)
{
	utc_poly_shape_hit_test pdt;
	pdt.add_point(100, 50);
	pdt.add_point(200, 50);
	g_assert(!pdt.hit_test(150, 65, false));
}

/*bool hit_test_bounding_box(const float x, const float y) const;*/
void utc_poly_shape_hit_test_hit_test_bounding_box_p(void)
{
	utc_poly_shape_hit_test pdt;
	pdt.add_point(50, 100);
	pdt.add_point(200, 50);
	g_assert(pdt.utc_hit_test_bounding_box(65, 70) == true);
}

void utc_poly_shape_hit_test_hit_test_bounding_box_n(void)
{
	utc_poly_shape_hit_test pdt;
	pdt.add_point(50, 100);
	pdt.add_point(200, 50);
	g_assert(pdt.utc_hit_test_bounding_box(220, 90) == false);
}

/*bool hit_test_polyline(const float x, const float y,
		       const bool polygon) const;*/
void utc_poly_shape_hit_test_hit_test_polyline_p(void)
{
	utc_poly_shape_hit_test pdt;

	/* \_/ form */
	float x[] = { 50, 200, 225, 175, 150, 100,  75,  25};
	float y[] = {250, 250, 100, 100, 200, 200, 100, 100};
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		pdt.add_point(x[i], y[i]);

	/* Hit test */
	g_assert(pdt.utc_hit_test_polyline(125, 250, true) == true);
	g_assert(pdt.utc_hit_test_polyline(125, 250, false) == true);

	g_assert(pdt.utc_hit_test_polyline(40, 184, true) == true);
}

void utc_poly_shape_hit_test_hit_test_polyline_n(void)
{
	utc_poly_shape_hit_test pdt;

	/* \_/ form */
	float x[] = { 50, 200, 225, 175, 150, 100,  75,  25};
	float y[] = {250, 250, 100, 100, 200, 200, 100, 100};
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		pdt.add_point(x[i], y[i]);

	/* Hit test */
	g_assert(pdt.utc_hit_test_polyline(125, 270, true) == false);
	g_assert(pdt.utc_hit_test_polyline(125, 270, false) == false);

	g_assert(pdt.utc_hit_test_polyline(40, 184, false) == false);
}


/*bool hit_test_segment(const float x1, const float y1,
		      const float x2, const float y2,
		      const float x, const float y) const;*/
void utc_poly_shape_hit_test_hit_test_segment_p(void)
{
	utc_poly_shape_hit_test pdt;
	g_assert(pdt.utc_hit_test_segment(50, 100,
					   200, 50,
					   100, 84) == true);

	g_assert(pdt.utc_hit_test_segment(50, 100,
					   200, 50,
					   100, 90) == true);

	g_assert(pdt.utc_hit_test_segment(50, 100,
					   200, 50,
					   100, 77) == true);

	g_assert(pdt.utc_hit_test_segment(50, 100,
					   200, 50,
					   50, 100) == true);

	g_assert(pdt.utc_hit_test_segment(50, 100,
					   200, 50,
					   199, 49) == true);
}

void utc_poly_shape_hit_test_hit_test_segment_n(void)
{
	utc_poly_shape_hit_test pdt;
	g_assert(pdt.utc_hit_test_segment(50, 100,
					   200, 50,
					   100, 64) == false);

	g_assert(pdt.utc_hit_test_segment(50, 100,
					   200, 50,
					   30, 64) == false);

	g_assert(pdt.utc_hit_test_segment(50, 100,
					   200, 50,
					   220, 64) == false);
}


/*bool hit_test_vertices(const float x, const float y) const;*/
void utc_poly_shape_hit_test_hit_test_vertices_p(void)
{
	utc_poly_shape_hit_test pdt;

	/* \_/ form */
	float x[] = { 50, 200, 225, 175, 150, 100,  75,  25};
	float y[] = {250, 250, 100, 100, 200, 200, 100, 100};
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		pdt.add_point(x[i], y[i]);

	/* Hit test */
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		g_assert(pdt.utc_hit_test_vertices(x[i], y[i]) == true);
}

void utc_poly_shape_hit_test_hit_test_vertices_n(void)
{
	utc_poly_shape_hit_test pdt;

	/* \_/ form */
	float x[] = { 50, 200, 225, 175, 150, 100,  75,  25};
	float y[] = {250, 250, 100, 100, 200, 200, 100, 100};
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		pdt.add_point(x[i], y[i]);

	/* Hit test */
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		g_assert(pdt.utc_hit_test_vertices(x[i] - 20, y[i]) == false);


	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		g_assert(pdt.utc_hit_test_vertices(x[i], y[i] - 20) == false);

}


/*int pnpoly(float x, float y) const;*/
void utc_poly_shape_hit_test_pnpoly_p(void)
{
	utc_poly_shape_hit_test pdt;

	/* \_/ form */
	float x[] = { 50, 200, 225, 175, 150, 100,  75,  25};
	float y[] = {250, 250, 100, 100, 200, 200, 100, 100};
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		pdt.add_point(x[i], y[i]);

	/* Hit test */
	g_assert(pdt.utc_pnpoly(100, 225) == true);
}

void utc_poly_shape_hit_test_pnpoly_n(void)
{
	utc_poly_shape_hit_test pdt;

	/* \_/ form */
	float x[] = { 50, 200, 225, 175, 150, 100,  75,  25};
	float y[] = {250, 250, 100, 100, 200, 200, 100, 100};
	for(unsigned int i = 0; i < (sizeof(x) / sizeof(x[0])); i ++)
		pdt.add_point(x[i], y[i]);

	/* Hit test */
	g_assert(pdt.utc_pnpoly(125, 150) == false);
}

