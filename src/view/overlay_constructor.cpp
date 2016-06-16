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


#include "overlay_constructor.h"

extern int _maps_view_object_create(const maps_view_object_type_e type, maps_view_object_h *object);

extern int _maps_view_object_overlay_set_object(maps_view_object_h overlay, Evas_Object *object);

extern int _maps_view_object_overlay_set_type(maps_view_object_h overlay, maps_view_overlay_type_e type);

maps_view_object_h view::overlay_constructor::construct(maps_coordinates_h coordinates,
	Evas_Object *object, maps_view_overlay_type_e type)
{
	if (!coordinates || !object) {
		__error = MAPS_ERROR_INVALID_PARAMETER;
		return NULL;
	}

	__error = MAPS_ERROR_NONE;

	maps_view_object_h overlay = NULL;
	do {
		/* 1. Create a visual object for marker */
		__error = _maps_view_object_create(MAPS_VIEW_OBJECT_OVERLAY, &overlay);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 2. Set the overlay type */
		__error = _maps_view_object_overlay_set_type(overlay, type);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 3. Set the evas object to the overlay */
		__error = _maps_view_object_overlay_set_object(overlay, object);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* 4. Move the overlay to the given coordinates */
		__error = maps_view_object_overlay_set_coordinates(overlay, coordinates);
		if (__error != MAPS_ERROR_NONE)
			break;

		/* SUCCESS */
		return overlay;
	} while (false);

	/* FAILURE: Releasing objects */
	maps_view_object_destroy(overlay);
	return NULL;
}
