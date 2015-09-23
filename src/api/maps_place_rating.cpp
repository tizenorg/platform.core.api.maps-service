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
#include "maps_place_rating_plugin.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_rating_s
{
	int count;
	double average;
} maps_place_rating_s;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_rating_create(maps_place_rating_h *place)
{
	MAPS_LOG_API;
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_rating_h) g_slice_new0(maps_place_rating_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_rating_destroy(maps_place_rating_h place)
{
	MAPS_LOG_API;
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	g_slice_free(maps_place_rating_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_rating_clone(const maps_place_rating_h origin,
				       maps_place_rating_h *cloned)
{
	MAPS_LOG_API;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_rating_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_rating_s *r = (maps_place_rating_s *) origin;

		error = maps_place_rating_set_count(*cloned, r->count);
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_place_rating_set_average(*cloned, r->average);
		if (error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while (false);

	maps_place_rating_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_rating_get_count(const maps_place_rating_h place,
					   int *count)
{
	MAPS_LOG_API;
	if (!place || !count)
		return MAPS_ERROR_INVALID_PARAMETER;
	*count = ((maps_place_rating_s *) place)->count;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_rating_get_average(const maps_place_rating_h place,
					     double *average)
{
	MAPS_LOG_API;
	if (!place || !average)
		return MAPS_ERROR_INVALID_PARAMETER;
	*average = ((maps_place_rating_s *) place)->average;
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_rating_set_count(maps_place_rating_h place,
					   const int count)
{
	MAPS_LOG_API;
	if (!place || (count < 0))
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_place_rating_s *) place)->count = count;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_rating_set_average(maps_place_rating_h place,
					     const double average)
{
	MAPS_LOG_API;
	if (!place || (average < 0))
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_place_rating_s *) place)->average = average;
	return MAPS_ERROR_NONE;
}
