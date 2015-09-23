/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __MAPS_VIEW_TEST_POLY_SHAPE_HIT_TEST_H__
#define __MAPS_VIEW_TEST_POLY_SHAPE_HIT_TEST_H__



/*bool hit_test(const float x, const float y,
	      const bool polygon) const;*/
void utc_poly_shape_hit_test_hit_test_p(void);
void utc_poly_shape_hit_test_hit_test_p2(void);
void utc_poly_shape_hit_test_hit_test_n(void);

/*bool hit_test_bounding_box(const float x, const float y) const;*/
void utc_poly_shape_hit_test_hit_test_bounding_box_p(void);
void utc_poly_shape_hit_test_hit_test_bounding_box_n(void);

/*bool hit_test_polyline(const float x, const float y,
		       const bool polygon) const;*/
void utc_poly_shape_hit_test_hit_test_polyline_p(void);
void utc_poly_shape_hit_test_hit_test_polyline_n(void);

/*bool hit_test_segment(const float x1, const float y1,
		      const float x2, const float y2,
		      const float x, const float y) const;*/
void utc_poly_shape_hit_test_hit_test_segment_p(void);
void utc_poly_shape_hit_test_hit_test_segment_n(void);

/*bool hit_test_vertices(const float x, const float y) const;*/
void utc_poly_shape_hit_test_hit_test_vertices_p(void);
void utc_poly_shape_hit_test_hit_test_vertices_n(void);

/*int pnpoly(float x, float y) const;*/
void utc_poly_shape_hit_test_pnpoly_p(void);
void utc_poly_shape_hit_test_pnpoly_n(void);

#endif /* __MAPS_VIEW_TEST_POLY_SHAPE_HIT_TEST_H__ */
