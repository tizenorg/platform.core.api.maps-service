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

extern int _map_object_create(const map_object_type_e type,
			      map_object_h *object);

extern int _map_object_marker_set_type(const map_object_h marker,
				const map_marker_type_e type);

map_object_h view::marker_constructor::construct(maps_coordinates_h coordinates,
						const char *image_file_path,
						map_marker_type_e type)
{
	if (!coordinates || !image_file_path) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return NULL;
	}

	if ((type < MAP_MARKER_PIN) || (type > MAP_MARKER_NONE)) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return NULL;
	}

	__error = MAPS_ERROR_NONE;

	map_object_h marker = NULL;
	do {
		/* 1. Create a visual object for marker */
		__error = _map_object_create(MAP_OBJECT_MARKER,
					     &marker);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 2. Set the marker image file pathe */
		__error = map_object_marker_set_image_file(marker,
							    image_file_path);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 3. Set the marker type */
		__error = _map_object_marker_set_type(marker, type);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 4 Move the marker to the given coordinates */
		__error = map_object_marker_set_coordinates(marker,
								 coordinates);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* SUCCESS */
		return marker;

	} while(false);

	/* FAILURE: Releasing objects */
	map_object_destroy(marker);
	return NULL;
}
