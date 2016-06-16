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

#include "maps_error.h"
#include "maps_place_media_plugin.h"
#include "maps_place_link_object_plugin.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_media_s
{
	char *attribution;
	maps_place_link_object_h supplier;
	maps_place_link_object_h via;
} maps_place_media_s;

const gsize _MAPS_PLACE_MEDIA_ATTRIBUTION_MAX_LENGTH = 64;

/*----------------------------------------------------------------------------*/
//LCOV_EXCL_START
EXPORT_API int maps_place_media_create(maps_place_media_h *place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_media_h) g_slice_new0(maps_place_media_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}
//LCOV_EXCL_STOP

EXPORT_API int maps_place_media_destroy(maps_place_media_h place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_media_s *m = (maps_place_media_s *) place;

	if (m->attribution)
		g_free(m->attribution);
	if (m->supplier)
		maps_place_link_object_destroy(m->supplier);
	if (m->via)
		maps_place_link_object_destroy(m->via);

	g_slice_free(maps_place_media_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_media_clone(const maps_place_media_h origin,
								maps_place_media_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_media_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_media_s *m = (maps_place_media_s *) origin;

		if (m->attribution) {
			error = maps_place_media_set_attribution(*cloned, m->attribution);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (m->supplier) {
			error = maps_place_media_set_supplier(*cloned, m->supplier);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (m->via) {
			error = maps_place_media_set_via(*cloned, m->via);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	//LCOV_EXCL_START
	maps_place_media_destroy(*cloned);
	*cloned = NULL;
	return error;
	//LCOV_EXCL_STOP
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_media_get_attribution(const maps_place_media_h place,
								char **attribution)
{
	if (!place || !attribution)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_media_s *) place)->attribution,
		_MAPS_PLACE_MEDIA_ATTRIBUTION_MAX_LENGTH, attribution);
}

EXPORT_API int maps_place_media_get_supplier(const maps_place_media_h place,
								maps_place_link_object_h *supplier)
{
	if (!place || !supplier)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_place_link_object_clone(((maps_place_media_s *) place)->supplier, supplier);
}

EXPORT_API int maps_place_media_get_via(const maps_place_media_h place,
								maps_place_link_object_h * via)
{
	if (!place || !via)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_place_link_object_clone(((maps_place_media_s *) place)->via, via);
}

/*----------------------------------------------------------------------------*/
//LCOV_EXCL_START
EXPORT_API int maps_place_media_set_attribution(maps_place_media_h place,
								const char *attribution)
{
	if (!place || !attribution)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(attribution,
		_MAPS_PLACE_MEDIA_ATTRIBUTION_MAX_LENGTH,
		&((maps_place_media_s *) place)->attribution);
}

EXPORT_API int maps_place_media_set_supplier(maps_place_media_h place,
								const maps_place_link_object_h supplier)
{
	if (!place || !supplier)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_media_s *m = (maps_place_media_s *) place;
	if (m->supplier)
		maps_place_link_object_destroy(m->supplier);
	return maps_place_link_object_clone(supplier, &m->supplier);
}

EXPORT_API int maps_place_media_set_via(maps_place_media_h place,
								const maps_place_link_object_h via)
{
	if (!place || !via)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_media_s *m = (maps_place_media_s *) place;
	if (m->via)
		maps_place_link_object_destroy(m->via);
	return maps_place_link_object_clone(via, &m->via);
}
//LCOV_EXCL_STOP
