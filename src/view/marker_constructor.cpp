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


#include "marker_constructor.h"

extern int _map_view_object_create(map_view_h view,
				   const map_view_object_type_e type,
				   map_view_object_h *object);

extern int _map_view_object_destroy(map_view_object_h object);
extern int _map_view_object_set_view(map_view_object_h object, map_view_h view);
extern int  _map_view_on_object_operation(map_view_h view,
					  const map_view_object_h object,
				  map_view_object_operation_e operation);


int view::marker_constructor::get_size_x()
{
	return 16;
}

int view::marker_constructor::get_size_y()
{
	return 16;
}

map_view_object_h view::marker_constructor::construct(map_view_h view,
						maps_coordinates_h coordinates,
						const char *image_file_path,
						map_view_marker_type_e type)
{
	if (!coordinates || !image_file_path) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return NULL;
	}

	if ((type < MAP_VIEW_MARKER_START) || (type > MAP_VIEW_MARKER_NONE)) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return NULL;
	}

	__error = MAPS_ERROR_NONE;

	map_view_object_h marker = NULL;
	do {
		/* 1. Create a visual object for marker */
		__error = _map_view_object_create(NULL,
						  MAP_VIEW_OBJECT_MARKER,
						  &marker);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 2. Set the marker image file pathe */
		__error = map_view_object_marker_set_file_path(marker,
							       image_file_path);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 3. Set the marker type */
		__error = map_view_object_marker_set_type(marker, type);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 4 Move the marker to the given coordinates */
		__error = map_view_object_marker_set_coordinates(marker,
								 coordinates);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 5 Set marker size */
		__error = map_view_object_marker_set_screen_size(marker,
								 get_size_x(),
								 get_size_y());
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 6. Make the Marker visible */
		__error = map_view_object_set_visible(marker, true);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* Notify view, that the object was constructed */
		_map_view_object_set_view(marker, view);
		if(view)
			_map_view_on_object_operation(view,
						      marker,
						      MAP_VIEW_OBJECT_ADD);


		/* SUCCESS */
		return marker;

	} while(false);

	/* FAILURE: Releasing objects */
	_map_view_object_destroy(marker);
	return NULL;
}
