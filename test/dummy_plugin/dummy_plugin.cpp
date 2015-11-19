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

#include <maps_service.h>
#include <maps_error.h>
#include <maps_plugin.h>
#include <maps_address.h>
#include <maps_place_plugin.h>
#include <maps_route_plugin.h>
#include <glib.h>

#include <maps_object.h>
using namespace maps;

static int dummy_geocode_request_id = 101;
static char __provider_key[1024] = { 0 };

/*----------------------------------------------------------------------------*/
/* Service API */

EXPORT_API int maps_plugin_init(maps_plugin_h *plugin)
{
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_shutdown(maps_plugin_h plugin)
{
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_get_info(maps_plugin_info_h *info)
{
	/*g_print("\n\nplugin maps_plugin_get_info\n\n"); */
	maps_plugin_info_create(info);
	maps_plugin_info_set_provider_name(*info, "Maps Provider");
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/
/*
 * Maps API
 */

EXPORT_API int maps_plugin_set_provider_key(const char *map_key)
{
	g_return_val_if_fail(map_key, MAPS_ERROR_INVALID_PARAMETER);
	g_snprintf(__provider_key, 1024, "%s", map_key);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_get_provider_key(char **map_key)
{
	g_return_val_if_fail(map_key, MAPS_ERROR_INVALID_PARAMETER);
	*map_key = g_strndup(__provider_key, 1024);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_get_preference(maps_item_hashtable_h *preference)
{
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_set_preference(maps_item_hashtable_h preference)
{
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_is_service_supported(maps_service_e service,
						bool *supported)
{
	switch (service) {
	case MAPS_SERVICE_GEOCODE:
	case MAPS_SERVICE_GEOCODE_INSIDE_AREA:
	case MAPS_SERVICE_GEOCODE_BY_STRUCTURED_ADDRESS:
	case MAPS_SERVICE_REVERSE_GEOCODE:
	case MAPS_SERVICE_SEARCH_PLACE:
	case MAPS_SERVICE_SEARCH_PLACE_BY_AREA:
	case MAPS_SERVICE_SEARCH_PLACE_BY_ADDRESS:
	case MAPS_SERVICE_SEARCH_ROUTE:
	case MAPS_SERVICE_SEARCH_ROUTE_WAYPOINTS:
	case MAPS_SERVICE_CANCEL_REQUEST:
	case MAPS_SERVICE_MULTI_REVERSE_GEOCODE:
		*supported = true;
		return MAPS_ERROR_NONE;
	default:
		*supported = false;
		return MAPS_ERROR_NOT_SUPPORTED;
	}
}

EXPORT_API int maps_plugin_is_data_supported(maps_service_data_e data,
					     bool *supported)
{
	switch (data) {
	case MAPS_PLACE_ADDRESS:
	case MAPS_PLACE_RATING:
	case MAPS_PLACE_CATEGORIES:
	case MAPS_PLACE_ATTRIBUTES:
	case MAPS_PLACE_CONTACTS:
	case MAPS_PLACE_EDITORIALS:
	case MAPS_PLACE_REVIEWS:
	case MAPS_PLACE_IMAGE:
	case MAPS_PLACE_SUPPLIER:
	case MAPS_PLACE_RELATED:
	case MAPS_ROUTE_PATH:
	case MAPS_ROUTE_SEGMENTS_PATH:
	case MAPS_ROUTE_SEGMENTS_MANEUVERS:
		*supported = true;
		return MAPS_ERROR_NONE;
	default:
		*supported = false;
		return MAPS_ERROR_NOT_SUPPORTED;
	}
}


EXPORT_API int maps_plugin_geocode(const char *address,
				   const maps_item_hashtable_h preference,
				   maps_service_geocode_cb callback,
				   void *user_data, int *request_id)
{
	g_return_val_if_fail(address, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(callback, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(request_id, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(preference, MAPS_ERROR_INVALID_PARAMETER);

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	coordinates c(11.1, 22.2);
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, c.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_geocode_inside_area(const char *address,
					       const maps_area_h bounds,
					       const maps_item_hashtable_h
					       preference,
					       maps_service_geocode_cb callback,
					       void *user_data, int *request_id)
{
	g_return_val_if_fail(address, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(bounds, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(callback, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(request_id, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(preference, MAPS_ERROR_INVALID_PARAMETER);

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	coordinates c(11.1, 22.2);
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, c.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_geocode_by_structured_address(const maps_address_h
							 address,
							 const maps_preference_h
							 preference,
							maps_service_geocode_cb
							callback,
							void *user_data,
							int *request_id)
{
	g_return_val_if_fail(address, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(callback, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(request_id, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(preference, MAPS_ERROR_INVALID_PARAMETER);

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	coordinates c(11.1, 22.2);
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, c.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_reverse_geocode(double latitude, double longitude,
					   const maps_item_hashtable_h
					   preference,
					   maps_service_reverse_geocode_cb
					   callback,
					   void *user_data,
					   int *request_id)
{
	g_return_val_if_fail(request_id, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(preference, MAPS_ERROR_INVALID_PARAMETER);

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	address a;
	maps_address_set_building_number(a, "12");
	maps_address_set_street(a, "Na Boyishti");
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, a.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_place(const maps_coordinates_h position,
					int distance,
					const maps_place_filter_h filter,
					maps_preference_h preference,
					maps_service_search_place_cb callback,
					void *user_data, int *request_id)
{

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	place p;
	maps_place_set_id(p, "1722/12");
	maps_place_set_name(p, "U Kalicha");
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, p.clone(), user_data);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_place_by_area(const maps_area_h boundary,
						const maps_place_filter_h
						filter,
						maps_preference_h preference,
						maps_service_search_place_cb
						callback,
						void *user_data,
						int *request_id)
{

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	place p;
	maps_place_set_id(p, "1722/12");
	maps_place_set_name(p, "U Kalicha");
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, p.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_place_by_address(const char *address,
						   const maps_area_h boundary,
						   const maps_place_filter_h
						   filter,
						   maps_preference_h preference,
						   maps_service_search_place_cb
						   callback,
						   void *user_data,
						   int *request_id)
{

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	place p;
	maps_place_set_id(p, "1722/12");
	maps_place_set_name(p, "U Kalicha");
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, p.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_route(const maps_coordinates_h origin,
					const maps_coordinates_h destination,
					maps_preference_h preference,
					maps_service_search_route_cb callback,
					void *user_data,
					int *request_id)
{

	/*g_print("Maps Plugin Search Route\n\n"); */
	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	route route;

	item_list <route_maneuver> maneuvers;
	route_maneuver m1, m2;
	maneuvers.add(m1);
	maneuvers.add(m2);

	item_list <route_segment> segments;
	route_segment s1, s2;

	maps_route_segment_set_origin(s1, coordinates(11.1, 12.1));
	maps_route_segment_set_destination(s1, coordinates(21.1, 22.1));

	maps_route_segment_set_origin(s2, coordinates(11.1, 12.1));
	maps_route_segment_set_destination(s2, coordinates(21.1, 22.1));

	maps_route_segment_set_maneuvers(s1, maneuvers);
	maps_route_segment_set_maneuvers(s2, maneuvers);

	segments.add(s1);
	segments.add(s2);
	maps_route_set_segments(route, segments);

	callback(MAPS_ERROR_NONE, *request_id, 0, 1, route.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_route_waypoints(const maps_coordinates_h *
						  waypoint_list,
						  int waypoint_num,
						  maps_preference_h preference,
						  maps_service_search_route_cb
						  callback,
						  void *user_data,
						  int *request_id)
{
	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	route route;

	item_list <route_maneuver> maneuvers;
	route_maneuver m1, m2;
	maneuvers.add(m1);
	maneuvers.add(m2);

	item_list <route_segment> segments;
	route_segment s1, s2;

	maps_route_segment_set_origin(s1, coordinates(11.1, 12.1));
	maps_route_segment_set_destination(s1, coordinates(21.1, 22.1));

	maps_route_segment_set_origin(s2, coordinates(11.1, 12.1));
	maps_route_segment_set_destination(s2, coordinates(21.1, 22.1));

	maps_route_segment_set_maneuvers(s1, maneuvers);
	maps_route_segment_set_maneuvers(s2, maneuvers);

	segments.add(s1);
	segments.add(s2);
	maps_route_set_segments(route, segments);

	callback(MAPS_ERROR_NONE, *request_id, 0, 1, route.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_cancel_request(int request_id)
{
	return MAPS_ERROR_NONE;
}
