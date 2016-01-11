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

#include "discovery.h"
#include "empty_module.h"


/* Set of empty functions */

/* Plugin dedicated functions */
int maps_plugin_init_empty(maps_plugin_h *plugin)
{
	return 0;
}

int maps_plugin_shutdown_empty(maps_plugin_h plugin)
{
	return 0;
}

int maps_plugin_get_info_empty(maps_plugin_info_h *info)
{
	return 0;
}

/* Maps Provider access key, preference and capabilities */
int maps_plugin_set_provider_key_empty(const char *provider_key)
{
	return 0;
}

int maps_plugin_get_provider_key_empty(char **provider_key)
{
	return 0;
}

int maps_plugin_set_preference_empty(maps_preference_h preference)
{
	return 0;
}

int maps_plugin_get_preference_empty(maps_preference_h *preference)
{
	return 0;
}

int maps_plugin_is_service_supported_empty(maps_service_e service,
					   bool *supported)
{
	return 0;
}

int maps_plugin_is_data_supported_empty(maps_service_data_e data,
					bool *supported)
{
	return 0;
}

/* Geocode */
int maps_plugin_geocode_empty(const char *address,
	const maps_preference_h preference,
	const maps_service_geocode_cb callback,
	void *user_data, int *request_id)
{
	return 0;
}

int maps_plugin_geocode_inside_area_empty(const char *address,
					  const maps_area_h bounds,
					  const maps_preference_h preference,
					  maps_service_geocode_cb callback,
					  void *user_data, int *request_id)
{
	return 0;
}

int maps_plugin_geocode_by_structured_address_empty(const maps_address_h
						    address,
						    const maps_preference_h
						    preference,
						    maps_service_geocode_cb
						    callback, void *user_data,
						    int *request_id)
{
	return 0;
}

int maps_plugin_reverse_geocode_empty(double latitude, double longitude,
				      const maps_preference_h preference,
				      maps_service_reverse_geocode_cb
				      callback, void *user_data,
				      int *request_id)
{
	return 0;
}

int maps_plugin_multi_reverse_geocode_empty(const maps_coordinates_list_h maps_list,
						const maps_preference_h preference,
						maps_service_multi_reverse_geocode_cb callback,
						void *user_data, int *request_id)
{
	return 0;
}

/* Place */
int maps_plugin_search_place_empty(const maps_coordinates_h position,
				   int distance,
				   const maps_place_filter_h filter,
				   maps_preference_h preference,
				   maps_service_search_place_cb callback,
				   void *user_data, int *request_id)
{
	return 0;
}

int maps_plugin_search_place_by_area_empty(const maps_area_h boundary,
					   const maps_place_filter_h filter,
					   maps_preference_h preference,
					   maps_service_search_place_cb
					   callback, void *user_data,
					   int *request_id)
{
	return 0;
}

int maps_plugin_search_place_by_address_empty(const char *address,
					      const maps_area_h boundary,
					      const maps_place_filter_h filter,
					      maps_preference_h preference,
					      maps_service_search_place_cb
					      callback, void *user_data,
					      int *request_id)
{
	return 0;
}

int maps_plugin_search_place_list_empty(const maps_area_h boundary,
				   const maps_place_filter_h filter,
				   maps_preference_h preference,
				   maps_service_search_place_list_cb callback,
				   void *user_data, int *request_id)
{
	return 0;
}

int maps_plugin_get_place_details_empty(const char *url,
						maps_service_get_place_details_cb callback,
						void *user_data, int *request_id)
{
	return 0;
}

/* Route */
int maps_plugin_search_route_empty(const maps_coordinates_h origin,
				   const maps_coordinates_h destination,
				   maps_preference_h preference,
				   maps_service_search_route_cb callback,
				   void *user_data, int *request_id)
{
	return 0;
}

int maps_plugin_search_route_waypoints_empty(const maps_coordinates_h *
					     waypoint_list, int waypoint_num,
					     maps_preference_h preference,
					     maps_service_search_route_cb
					     callback, void *user_data,
					     int *request_id)
{
	return 0;
}

/* Cancel Request */
int maps_plugin_cancel_request_empty(int request_id)
{
	return 0;
}


/* Interface of a plugin with all empty functions */
plugin::interface_s empty_interface = {
	/* Plugin dedicated functions */
	maps_plugin_init_empty,
	maps_plugin_shutdown_empty,
	maps_plugin_get_info_empty,

	maps_plugin_set_provider_key_empty,
	maps_plugin_get_provider_key_empty,
	maps_plugin_set_preference_empty,
	maps_plugin_get_preference_empty,
	maps_plugin_is_service_supported_empty,
	maps_plugin_is_data_supported_empty,

	/* Geocode */
	maps_plugin_geocode_empty,
	maps_plugin_geocode_inside_area_empty,
	maps_plugin_geocode_by_structured_address_empty,
	maps_plugin_reverse_geocode_empty,
	maps_plugin_multi_reverse_geocode_empty,

	/* Place */
	maps_plugin_search_place_empty,
	maps_plugin_search_place_by_area_empty,
	maps_plugin_search_place_by_address_empty,
	maps_plugin_search_place_list_empty,
	maps_plugin_get_place_details_empty,

	/* Route */
	maps_plugin_search_route_empty,
	maps_plugin_search_route_waypoints_empty,

	/* Cancel Request */
	maps_plugin_cancel_request_empty,
};


plugin::interface_s &plugin::get_empty_interface()
{
	return empty_interface;
}

plugin::interface_s *plugin::get_empty_interface_ptr()
{
	return &empty_interface;
}
