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


#include "polygon_constructor.h"

extern int _maps_view_object_create(const maps_view_object_type_e type,
			      maps_view_object_h *object);

maps_view_object_h view::polygon_constructor::construct(const maps_coordinates_list_h
						  coordinates,
						  const unsigned char r,
						  const unsigned char g,
						  const unsigned char b,
						  const unsigned char a)
{
	if (!coordinates) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return NULL;
	}

	__error = MAPS_ERROR_NONE;

	maps_view_object_h polygon = NULL;
	do {
		/* 1. Create a visual object for marker */
		__error = _maps_view_object_create(MAPS_VIEW_OBJECT_POLYGON,
						 &polygon);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 2. Set the polygon points */
		__error = maps_view_object_polygon_set_polygon(polygon, coordinates);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 3. Set the polygon color */
		__error = maps_view_object_polygon_set_fill_color(polygon,
								 r, g, b, a);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 5. Set the polygon visible */
		__error = maps_view_object_set_visible(polygon, true);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* SUCCESS */
		return polygon;
	} while (false);

	/* FAILURE: Releasing objects */
	maps_view_object_destroy(polygon);
	return NULL;
}
