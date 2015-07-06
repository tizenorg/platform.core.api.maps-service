/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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
#include "maps_extra_types.h"
#include "maps_util.h"
#include "maps_preference.h"
#include <glib.h>

typedef struct _callback_data
{
	maps_preference_properties_cb callback;
	void *user_data;
} callback_data;

static bool __isnamed_preference(const char *key)
{
	if (!key)
		return false;

	static guint names[] = {
		g_str_hash(MAPS_PLACE_FILTER_TYPE),
		g_str_hash(MAPS_PLACE_FILTER_SORT_BY),
		g_str_hash(MAPS_PLACE_FILTER_TYPE),
		g_str_hash(MAPS_PLACE_FILTER_SORT_BY),
		g_str_hash(MAPS_ROUTE_FREEFORM_ADDR_TO_AVOID),
		g_str_hash(MAPS_ROUTE_STRUCTED_ADDR_TO_AVOID),
		g_str_hash(MAPS_ROUTE_CIRCLE_AREA_TO_AVOID),
		g_str_hash(MAPS_ROUTE_RECT_AREA_TO_AVOID),
		g_str_hash(MAPS_ROUTE_GEOMETRY_BOUNDING_BOX),
		g_str_hash(MAPS_ROUTE_GEOMETRY_RETRIEVAL),
		g_str_hash(MAPS_ROUTE_INSTRUCTION_GEOMETRY),
		g_str_hash(MAPS_ROUTE_INSTRUCTION_BOUNDING_BOX),
		g_str_hash(MAPS_ROUTE_INSTRUCTION_RETRIEVAL),
		g_str_hash(MAPS_ROUTE_REALTIME_TRAFFIC),
		g_str_hash("MAPS_PREFERENCE_DISTANCE_UNIT"),
		g_str_hash("MAPS_PREFERENCE_LANGUAGE"),
		g_str_hash("MAPS_PREFERENCE_MAX_RESULTS"),
		g_str_hash("MAPS_PREFERENCE_COUNTRY_CODE"),
		g_str_hash("MAPS_PREFERENCE_ROUTE_OPTIMIZATION"),
		g_str_hash("MAPS_PREFERENCE_ROUTE_TRANSPORT_MODE"),
		g_str_hash("MAPS_PREFERENCE_ROUTE_FEATURE_WEIGHT"),
		g_str_hash("MAPS_PREFERENCE_ROUTE_FEATURE")
	};
	const int size = sizeof(names) / sizeof(names[0]);

	guint key_hash = g_str_hash(key);
	for (int i = 0; i < size; i++)
		if (key_hash == names[i])
			return true;

	return false;
}

static bool __maps_preference_properties_helper_cb(int index, int total,
						   char *key, void *value,
						   void *user_data)
{
	/* Do not return the "named" preference, which can be obtained, */
	/* with other functions from this module. */
	/* The reason is that "named" preferences may be not only string type */
	if (__isnamed_preference(key)) {
		g_free(key);
		g_free(value);
		return true;
	}
	if (!user_data)
		return false;
	callback_data *cbd = (callback_data *) user_data;
	if (!cbd->callback)
		return false;
	return cbd->callback(index, total, key, (char *) value, cbd->user_data);
}

EXPORT_API int maps_preference_create(maps_preference_h *preference)
{
	if (!preference)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = maps_item_hashtable_create(preference);
	if (error != MAPS_ERROR_NONE)
		return error;

	do {
		error = maps_item_hashtable_set_int(*preference,
			"MAPS_PREFERENCE_DISTANCE_UNIT", MAPS_DISTANCE_UNIT_M);
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_item_hashtable_set_string(*preference,
			"MAPS_PREFERENCE_LANGUAGE", "en-US");
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_item_hashtable_set_int(*preference,
			"MAPS_PREFERENCE_MAX_RESULTS", 25);
		if (error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while (0);

	maps_item_hashtable_destroy(preference);
	return error;
}

EXPORT_API int maps_preference_destroy(maps_preference_h preference)
{
	if (!preference)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_destroy(preference);
}

EXPORT_API int maps_preference_clone(const maps_preference_h origin,
				     maps_preference_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_clone(origin, cloned);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_preference_get_distance_unit(const maps_preference_h
						 preference,
						 maps_distance_unit_e *unit)
{
	if (!preference || !unit)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_int(preference,
		"MAPS_PREFERENCE_DISTANCE_UNIT", (int *) unit);
}

EXPORT_API int maps_preference_get_language(const maps_preference_h preference,
					    char **language)
{
	if (!preference || !language)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_string(preference,
		"MAPS_PREFERENCE_LANGUAGE", language);
}

EXPORT_API int maps_preference_get_max_results(const maps_preference_h
					       preference,
					       int *max_results)
{
	if (!preference || !max_results)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_int(preference,
		"MAPS_PREFERENCE_MAX_RESULTS", max_results);
}

EXPORT_API int maps_preference_get_country_code(const maps_preference_h
						preference,
						char **country_code)
{
	if (!preference || !country_code)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_string(preference,
		"MAPS_PREFERENCE_COUNTRY_CODE", country_code);
}

EXPORT_API int maps_preference_get_route_optimization(const maps_preference_h
						      preference,
						      maps_route_optimization_e*
						      optimization)
{
	if (!preference || !optimization)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_int(preference,
		"MAPS_PREFERENCE_ROUTE_OPTIMIZATION", (int *) optimization);
}

EXPORT_API int maps_preference_get_route_transport_mode(const maps_preference_h
							preference,
						maps_route_transport_mode_e *
						transport_mode)
{
	if (!preference || !transport_mode)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_int(preference,
		"MAPS_PREFERENCE_ROUTE_TRANSPORT_MODE", (int *) transport_mode);
}

EXPORT_API int maps_preference_get_route_feature_weight(const maps_preference_h
							preference,
						maps_route_feature_weight_e *
						feature_weight)
{
	if (!preference || !feature_weight)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_int(preference,
		"MAPS_PREFERENCE_ROUTE_FEATURE_WEIGHT", (int *) feature_weight);
}

EXPORT_API int maps_preference_get_route_feature(const maps_preference_h
						 preference,
						 maps_route_feature_e * feature)
{
	if (!preference || !feature)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_int(preference,
		"MAPS_PREFERENCE_ROUTE_FEATURE", (int *) feature);
}

EXPORT_API int maps_preference_get(const maps_preference_h preference,
				   const char *key, char **value)
{
	if (!preference || !key || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_get_string(preference, key, value);
}





EXPORT_API int maps_preference_foreach_property(const maps_preference_h
						preference,
						maps_preference_properties_cb
						callback, void *user_data)
{
	if (!preference || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	callback_data cd = { callback, user_data };
	return maps_item_hashtable_foreach(preference,
		__maps_preference_properties_helper_cb, &cd);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_preference_set_distance_unit(maps_preference_h preference,
						 const maps_distance_unit_e
						 unit)
{
	if (!preference)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((unit < MAPS_DISTANCE_UNIT_M) || (unit > MAPS_DISTANCE_UNIT_YD))
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_int(preference,
		"MAPS_PREFERENCE_DISTANCE_UNIT", unit);
}

EXPORT_API int maps_preference_set_language(maps_preference_h preference,
					    const char *language)
{
	if (!preference || !language)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_string(preference,
		"MAPS_PREFERENCE_LANGUAGE", language);
}

EXPORT_API int maps_preference_set_max_results(maps_preference_h preference,
					       const int max_results)
{
	if (!preference || max_results <= 0)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_int(preference,
		"MAPS_PREFERENCE_MAX_RESULTS", max_results);
}

EXPORT_API int maps_preference_set_country_code(maps_preference_h preference,
						const char *country_code)
{
	if (!preference || !country_code)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_string(preference,
		"MAPS_PREFERENCE_COUNTRY_CODE", country_code);
}

EXPORT_API int maps_preference_set_route_optimization(maps_preference_h
						      preference,
						const maps_route_optimization_e
						optimization)
{
	if (!preference)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((optimization < MAPS_ROUTE_TYPE_FASTEST)
	   || (optimization > MAPS_ROUTE_TYPE_DIRECTDRIVE))
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_int(preference,
		"MAPS_PREFERENCE_ROUTE_OPTIMIZATION", optimization);
}

EXPORT_API int maps_preference_set_route_transport_mode(maps_preference_h
							preference,
					const maps_route_transport_mode_e
					transport_mode)
{
	if (!preference)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((transport_mode < MAPS_ROUTE_TRANSPORT_MODE_CAR)
	   || (transport_mode > MAPS_ROUTE_TRANSPORT_MODE_TRUCK))
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_int(preference,
		"MAPS_PREFERENCE_ROUTE_TRANSPORT_MODE", transport_mode);
}

EXPORT_API int maps_preference_set_route_feature_weight(maps_preference_h
							preference,
					const maps_route_feature_weight_e
					feature_weight)
{
	if (!preference)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((feature_weight < MAPS_ROUTE_FEATURE_WEIGHT_NORMAL)
	   || (feature_weight > MAPS_ROUTE_FEATURE_WEIGHT_STRICTEXCLUDE))
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_int(preference,
		"MAPS_PREFERENCE_ROUTE_FEATURE_WEIGHT", feature_weight);
}

EXPORT_API int maps_preference_set_route_feature(maps_preference_h preference,
						 const maps_route_feature_e
						 feature)
{
	if (!preference)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((feature < MAPS_ROUTE_FEATURE_NO)
	   || (feature > MAPS_ROUTE_FEATURE_STAIRS))
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_int(preference,
		"MAPS_PREFERENCE_ROUTE_FEATURE", feature);
}

EXPORT_API int maps_preference_set_property(maps_preference_h preference,
					    const char *key, const char *value)
{
	if (!preference || !key || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_set_string(preference, key, value);
}

/*----------------------------------------------------------------------------*/

#if 0
EXPORT_API int maps_preference_append_route_feature(maps_preference_h
	preference, maps_item_list_h feature_list,
	maps_route_feature_weight_e feature)
{

	if (!preference || max_result_count <= 0)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_item_list_s* list = (maps_item_list_s*) feature_list;

	if (!feature) {
		maps_item_list_create(list);
	}

	maps_item_list_append(list, feature);
	maps_item_hashtable_s* p = (maps_item_hashtable_s*) preference;

	int ret;
	ret = maps_item_hashtable_set(p, "MAPS_PREFERENCE_ROUTE_FEATURE", list);

	feature = list;

	return ret;
}

EXPORT_API int maps_preference_remove_route_feature(maps_preference_h
	preference, maps_item_list_h feature_list,
	maps_route_feature_weight_e feature)
{

	if (!preference || !feature_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_item_list_s* list = (maps_item_list_s*) feature_list;

	if (!feature) {
		maps_item_list_create(list);
	}

	maps_item_list_remove(list, feature);
	maps_item_hashtable_s* p = (maps_item_hashtable_s*) preference;

	int ret;
	ret = maps_item_hashtable_set(p, "MAPS_PREFERENCE_ROUTE_FEATURE", list);

	feature = list;

	return ret;
}
#endif
