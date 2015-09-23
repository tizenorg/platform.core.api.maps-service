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

#include <glib.h>
#include "maps_error.h"
#include "maps_area.h"
#include "maps_util.h"

EXPORT_API int maps_area_create_rectangle(const maps_coordinates_h top_left,
					  const maps_coordinates_h bottom_right,
					  maps_area_h *area)
{
	MAPS_LOG_API;
	if (!top_left || !bottom_right || !area)
		return MAPS_ERROR_INVALID_PARAMETER;

	double tf_lat = .0;
	double tf_lon = .0;
	double rb_lat = .0;
	double rb_lon = .0;

	maps_coordinates_get_latitude(top_left, &tf_lat);
	maps_coordinates_get_latitude(bottom_right, &rb_lat);
	maps_coordinates_get_longitude(top_left, &tf_lon);
	maps_coordinates_get_longitude(bottom_right, &rb_lon);

	double lon_interval = rb_lon - tf_lat;

	if (lon_interval < 180 && lon_interval > -180) {
		if (rb_lon <= tf_lon || rb_lat >= tf_lat)
			return MAPS_ERROR_INVALID_PARAMETER;
	}
	else {
		if (rb_lon >= tf_lon || rb_lat >= tf_lat)
			return MAPS_ERROR_INVALID_PARAMETER;
	}

	maps_area_s *bound = g_slice_new0(maps_area_s);

	if (bound == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	bound->type = MAPS_AREA_RECTANGLE;

	bound->rect.top_left = *((maps_coordinates_s *) top_left);
	bound->rect.bottom_right = *((maps_coordinates_s *) bottom_right);

	*area = (maps_area_h) bound;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_area_create_circle(const maps_coordinates_h center,
				       const double radius, maps_area_h *area)
{
	MAPS_LOG_API;
	if (!center || !area)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (radius < 0)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* MAPS_CHECK_CONDITION(radius > 0, MAPS_ERROR_INVALID_PARAMETER,
	* "MAPS_ERROR_INVALID_PARAMETER"); */
	maps_area_s *bound = g_slice_new0(maps_area_s);

	if (bound == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	bound->type = MAPS_AREA_CIRCLE;
	bound->circle.center = *((maps_coordinates_s *) center);
	bound->circle.radius = radius;
	*area = (maps_area_h) bound;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_area_destroy(maps_area_h area)
{
	MAPS_LOG_API;
	if (!area)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_area_s *handle = (maps_area_s *) area;

	g_slice_free(maps_area_s, handle);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_area_clone(const maps_area_h origin, maps_area_h *cloned)
{
	MAPS_LOG_API;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_area_s * origin_handle = (maps_area_s *) origin;
	if (origin_handle->type == MAPS_AREA_RECTANGLE) {
		maps_area_h new_rect = NULL;
		maps_area_rectangle_s rect = origin_handle->rect;
		maps_coordinates_s rec_tl = rect.top_left;
		maps_coordinates_s rec_br = rect.bottom_right;
		maps_area_create_rectangle((maps_coordinates_h) & rec_tl,
			(maps_coordinates_h) & rec_br, &new_rect);
		if (new_rect) {
			*cloned = new_rect;
		}
		else
			return MAPS_ERROR_INVALID_PARAMETER;

	}
	else if (origin_handle->type == MAPS_AREA_CIRCLE) {
		maps_area_h new_circle = NULL;
		maps_area_circle_s cir = origin_handle->circle;
		maps_coordinates_s center = cir.center;
		double radius = cir.radius;
		maps_area_create_circle((maps_coordinates_h) & center, radius,
			&new_circle);
		if (new_circle) {
			*cloned = new_circle;
		}
		else
			return MAPS_ERROR_INVALID_PARAMETER;
	}

	return MAPS_ERROR_NONE;
}
