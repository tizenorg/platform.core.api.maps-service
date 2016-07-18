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
#include "maps_condition.h"

extern bool maps_coordinates_is_valid(const maps_coordinates_h coordinates);

EXPORT_API int maps_area_create_rectangle(const maps_coordinates_h top_left,
					  const maps_coordinates_h bottom_right,
					  maps_area_h *area)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;

	if (!top_left || !bottom_right || !area)
		return MAPS_ERROR_INVALID_PARAMETER;

	if (!maps_coordinates_is_valid(top_left) ||
		!maps_coordinates_is_valid(bottom_right))
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_area_s *bound = g_new0(maps_area_s, 1);

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
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;

	if (!center || !area || radius <= 0)
		return MAPS_ERROR_INVALID_PARAMETER;

	if (!maps_coordinates_is_valid(center))
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_area_s *bound = g_new0(maps_area_s, 1);

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
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!area)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_area_s *handle = (maps_area_s *) area;
	g_free(handle);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_area_clone(const maps_area_h origin, maps_area_h *cloned)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
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
		} else {
			return MAPS_ERROR_INVALID_PARAMETER;
		}
	} else if (origin_handle->type == MAPS_AREA_CIRCLE) {
		maps_area_h new_circle = NULL;
		maps_area_circle_s cir = origin_handle->circle;
		maps_coordinates_s center = cir.center;
		double radius = cir.radius;
		maps_area_create_circle((maps_coordinates_h)&center, radius, &new_circle);
		if (new_circle) {
			*cloned = new_circle;
		} else {
			return MAPS_ERROR_INVALID_PARAMETER;
		}
	}

	return MAPS_ERROR_NONE;
}

bool maps_area_is_valid(const maps_area_h area)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!area) return false;

	bool ret = true;
	maps_area_s *handle = (maps_area_s *) area;

	do {
		if (handle->type == MAPS_AREA_RECTANGLE) {
			maps_area_rectangle_s rect = handle->rect;

			if (!maps_coordinates_is_valid(&rect.top_left) ||
				!maps_coordinates_is_valid(&rect.bottom_right)) {
				ret = false;
				break;
			}
		} else if (handle->type == MAPS_AREA_CIRCLE) {
			maps_area_circle_s cir = handle->circle;
			maps_coordinates_s center = cir.center;

			if (cir.radius <= 0) {
				ret = false;
				break;
			}
			if (!maps_coordinates_is_valid(&center)) {
				ret = false;
				break;
			}
		} else {
			ret = false;
		}
	} while (false);

	return ret;
}
