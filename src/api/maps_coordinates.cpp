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
#include "maps_coordinates.h"
#include "maps_util.h"

EXPORT_API int maps_coordinates_create(const double latitude,
				       const double longitude,
				       maps_coordinates_h *coords)
{
	if (not coords)
		return MAPS_ERROR_INVALID_PARAMETER;

	MAPS_CHECK_CONDITION(latitude >= -90
		&& latitude <= 90, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	MAPS_CHECK_CONDITION(longitude >= -180
		&& longitude <= 180, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");

	maps_coordinates_s *coord = g_slice_new0(maps_coordinates_s);

	if (coord == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}
	coord->latitude = latitude;
	coord->longitude = longitude;

	*coords = (maps_coordinates_h) coord;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_destroy(maps_coordinates_h coords)
{
	if (not coords)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_coordinates_s *handle = (maps_coordinates_s *) coords;
	g_slice_free(maps_coordinates_s, handle);

	coords = NULL;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_clone(const maps_coordinates_h origin,
				      maps_coordinates_h *cloned)
{
	if (not cloned or not origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		maps_coordinates_s *c = (maps_coordinates_s *) origin;

		error = maps_coordinates_create(c->latitude, c->longitude,
			cloned);

		if (!(*cloned) or(error != MAPS_ERROR_NONE))
			break;

		return MAPS_ERROR_NONE;
	} while (false);

	maps_coordinates_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_coordinates_get_latitude(const maps_coordinates_h coords,
					     double *latitude)
{
	if (not coords or not latitude)
		return MAPS_ERROR_INVALID_PARAMETER;
	*latitude = ((maps_coordinates_s *) coords)->latitude;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_get_longitude(const maps_coordinates_h coords,
					      double *longitude)
{
	if (not coords or not longitude)
		return MAPS_ERROR_INVALID_PARAMETER;
	*longitude = ((maps_coordinates_s *) coords)->longitude;
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_coordinates_set_latitude(maps_coordinates_h coords,
					     const double latitude)
{
	if (not coords)
		return MAPS_ERROR_INVALID_PARAMETER;
	MAPS_CHECK_CONDITION(latitude >= -90
		&& latitude <= 90, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	((maps_coordinates_s *) coords)->latitude = latitude;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_coordinates_set_longitude(maps_coordinates_h coords,
	const double longitude)
{
	if (not coords)
		return MAPS_ERROR_INVALID_PARAMETER;
	MAPS_CHECK_CONDITION(longitude >= -180
		&& longitude <= 180, MAPS_ERROR_INVALID_PARAMETER,
		"MAPS_ERROR_INVALID_PARAMETER");
	((maps_coordinates_s *) coords)->longitude = longitude;
	return MAPS_ERROR_NONE;
}
