/* Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __MAPS_VIEW_POLY_SHAPE_HIT_TEST_H__
#define __MAPS_VIEW_POLY_SHAPE_HIT_TEST_H__


#include <maps_util.h>


namespace view
{
	class poly_shape_hit_test {
	private:
		vector<float> __x;
		vector<float> __y;
		float accuracy;
	public:
		poly_shape_hit_test() : accuracy(10.) {}
	public:
		void add_point(const float x, const float y);
		bool hit_test(const float x, const float y, const bool polygon, const int w = 0) const;
	protected:
		bool hit_test_bounding_box(const float x, const float y) const;
		bool hit_test_polyline(const float x, const float y, const bool polygon, const int w) const;
		bool hit_test_segment(const float x1, const float y1, const float x2, const float y2,
			const float x, const float y, const int w = 0) const;
		bool hit_test_vertices(const float x, const float y) const;
		bool pnpoly(const float x, const float y) const;
	};
};

#endif				/* __MAPS_VIEW_POLY_SHAPE_HIT_TEST_H__ */
