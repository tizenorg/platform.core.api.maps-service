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

extern int _map_view_object_create(map_view_h view,
				   const map_view_object_type_e type,
				   map_view_object_h *object);

extern int _map_view_object_destroy(map_view_object_h object);

extern int _map_view_object_set_view(map_view_object_h object, map_view_h view);
extern int  _map_view_on_object_operation(map_view_h view,
					  const map_view_object_h object,
				  map_view_object_operation_e operation);

map_view_object_h view::polygon_constructor::construct(map_view_h view,
						       const maps_item_list_h
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

	map_view_object_h polygon = NULL;
	do {
		/* 1. Create a visual object for marker */
		__error = _map_view_object_create(NULL,
						  MAP_VIEW_OBJECT_POLYGON,
						  &polygon);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 2. Set the polygon points */
		__error = map_view_object_poly_set_points(polygon, coordinates);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 3. Set the polygon color */
		__error = map_view_object_polygon_set_fill_color(polygon,
								 r, g, b, a);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 5. Set the polygon visible */
		__error = map_view_object_set_visible(polygon, true);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* Notify view, that the object was constructed */
		_map_view_object_set_view(polygon, view);
		if(view)
			_map_view_on_object_operation(view,
						      polygon,
						      MAP_VIEW_OBJECT_ADD);
		/* SUCCESS */
		return polygon;

	} while(false);

	/* FAILURE: Releasing objects */
	_map_view_object_destroy(polygon);
	return NULL;
}
