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


#include "polyline_constructor.h"

extern int _map_object_create(const map_object_type_e type,
			      map_object_h *object);

map_object_h view::polyline_constructor::construct(const maps_coord_list_h
						   coordinates,
						   const unsigned char r,
						   const unsigned char g,
						   const unsigned char b,
						   const unsigned char a,
						   const int width)
{
	if (!coordinates || (width <= 0)) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return NULL;
	}

	__error = MAPS_ERROR_NONE;

	map_object_h polyline = NULL;
	do {
		/* 1. Create a visual object for polyline */
		__error = _map_object_create(MAP_OBJECT_POLYLINE,
						 &polyline);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 2. Set the polyline points */
		__error = map_object_polyline_set_polyline(polyline,
							   coordinates);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 3. Set the polyline color */
		__error = map_object_polyline_set_color(polyline,
							     r, g, b, a);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 4. Set the polyline width */
		__error = map_object_polyline_set_width(polyline, width);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 5. Set the polyline visible */
		__error = map_object_set_visible(polyline, true);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* SUCCESS */
		return polyline;

	} while(false);

	/* FAILURE: Releasing objects */
	map_object_destroy(polyline);
	return NULL;
}
