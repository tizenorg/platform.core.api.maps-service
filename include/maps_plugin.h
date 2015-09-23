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

#ifndef __MAPS_PLUGIN_H__
#define __MAPS_PLUGIN_H__

#include <maps_service.h>
#include <maps_plugin_info.h>
#include <map_view.h>
#include <maps_extra_types.h>

/**
 *
 * @ingroup	CAPI_MAPS_PLUGIN_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_AND_PROVIDERS_MODULE Maps Plugin and Providers
 *
 * @file maps_plugin.h
 * @brief This file contains the top level functions of Maps Plugin API
 *
 * @addtogroup CAPI_MAPS_PLUGIN_AND_PROVIDERS_MODULE
 * @{
 * @brief This provides APIs related to operations with Maps Plugins
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Maps Plugin handle.
 * @details The handle of Maps Plugin instance.
 * @since_tizen 2.4
 */
typedef void *maps_plugin_h;

/*----------------------------------------------------------------------------*/
/*
 * Plugin dedicated functions
 */

/**
 * @brief	Initialize a new Maps Plugin.
 * @details A maps plugin handle can be used to access a specified plugin.
 * @since_tizen 2.4
 * @remarks @a plugin and it resources must be released in
 * maps_plugin_shutdown().
 *
 * @param[out]	plugin		A handle of a new plugin on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 *
 * @see maps_plugin_shutdown()
 */
int maps_plugin_init(maps_plugin_h *plugin);

/**
 * @brief	Destroys the Maps Plugin handle.
 * @details This function destroys the maps plugin handle and releases all its
 * resources.
 * @since_tizen 2.4
 *
 * @param[in]	plugin		The maps plugin handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_plugin_init()
 */
int maps_plugin_shutdown(maps_plugin_h plugin);

/**
 * @brief	Returns a plugin info.
 * @details This function returns a plugin info.
 * @since_tizen 2.4
 *
 * @param[out]	info	A plugin info
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 *
 * @see maps_plugin_init()
 */
int maps_plugin_get_info(maps_plugin_info_h *info);

/*----------------------------------------------------------------------------*/
/*
 * Maps Provider access key, preference and capabilities
 */

/**
 * @brief	Sets the Maps Key to be used in the Maps Plugin requests.
 * @details This function sets the Maps Key which will be used in each Maps
 * Plugin request to Maps Provider.
 * @since_tizen 2.4
 * @remarks To obtain the @a provider_key refer to corresponding Maps Provider
 * documentation.
 * \n For HERE Maps refer to https://developer.here.com/,
 * https://developer.here.com/rest-apis.
 *
 * @param[in]	provider_key	The Maps Key to be used
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see maps_plugin_get_provider_key()
 */
int maps_plugin_set_provider_key(const char *provider_key);

/**
 * @brief	Gets the Maps Key which is to be used in the Maps Service
 * requests
 * @details This function gets the Maps Key which is to be used in each Maps
 * Service request to Maps Provider.
 * \n Maps key can be set with maps_plugin_set_provider_key().
 * @since_tizen 2.4
 * @remarks The string @a provider_key must be released using free().
 *
 * @param[out]	provider_key	The Maps Key
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see maps_plugin_set_provider_key()
 */
int maps_plugin_get_provider_key(char **provider_key);

/**
 * @brief	Sets the Maps Preference.
 * @details This function sets the Maps Preferences which are used in each Maps
 * Plugin request to Maps Provider.
 * @since_tizen 2.4
 *
 * @param[in]	preference	The handle of Maps Preference
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @see maps_plugin_get_preference()
 */
int maps_plugin_set_preference(maps_item_hashtable_h preference);

/**
 * @brief	Gets the Maps Preference.
 * @details This function gets the Maps Preferences which are used in each Maps
 * Plugin request to Maps Provider.
 * \n Preferences can be set with maps_plugin_set_preference().
 * @since_tizen 2.4
 * @remarks @a preference must be released using maps_preference_destroy().
 *
 * @param[out]	preference	The handle of Maps Preference
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see maps_plugin_set_preference()
 */
int maps_plugin_get_preference(maps_item_hashtable_h *preference);

/**
 * @brief	Checks if the Maps Plugin supports a request.
 * @details This function checks if the Maps Plugin supports a specified
 * request.
 * @since_tizen 2.4
 *
 * @param[in]	service		The service to be checked
 * @param[out]	supported	Is the service supported
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful, the service is supported
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 *
 * @see maps_service_create()
 */
int maps_plugin_is_service_supported(maps_service_e service,
				     bool *supported);

/**
 * @brief	Checks if the Maps Plugin supports a data feature.
 * @details This function checks if the Maps Plugin supports a specified data
 * feature.
 * @since_tizen 2.4
 *
 * @param[in]	data		The data feature to be checked
 * @param[out]	supported	Is the data feature supported
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful, the data feature is supported
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 *
 * @see maps_service_create()
 */
int maps_plugin_is_data_supported(maps_service_data_e data, bool *supported);

/*----------------------------------------------------------------------------*/
/*
 * Geocode
 */

/**
 * @brief	Gets the position coordinates for a given address.
 * @details This function obtains position coordinates for a given free-formed
 * address string.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 * \n To cancel the request use maps_plugin_cancel_request().
 *
 * @param[in]	address		The free-formed address
 * @param[in]	preference	The set of preferences for processing Geocode
 * @param[in]	callback	The callback which will receive position
 * coordinates
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @param[out]	request_id	The id of request
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network connection failed
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @post It invokes maps_service_geocode_cb() to deliver obtained position
 * coordinates.
 *
 * @see maps_plugin_geocode_inside_area()
 * @see maps_service_geocode_cb()
 * @see maps_plugin_cancel_request()
 * @see maps_plugin_reverse_geocode()
 */
int maps_plugin_geocode(const char *address,
			const maps_item_hashtable_h preference,
			maps_service_geocode_cb callback, void *user_data,
			int *request_id);

/**
 * @brief	Gets the position for a given address, within the specified
 * bounding box.
 * @details This function obtains position coordinates for a given free-formed
 * address string within the specified bounding box.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 * \n Polygonal bounding box is not supported.
 * \n To cancel the request use maps_plugin_cancel_request().
 * \n To check if Maps Provider is capable of Geocoding and which Geocode
 * preferences are supported see the lists of capacities and preferences above.
 *
 * @param[in]	address		The free-formed address
 * @param[in]	bounds		The bounding box
 * @param[in]	preference	The set of preferences for processing Geocode
 * @param[in]	callback	The callback which will receive position
 * coordinates
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @param[out]	request_id	The id of request
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network connection failed
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @post It invokes maps_service_geocode_cb() to deliver obtained position
 * coordinates.
 *
 * @see maps_plugin_geocode_by_structured_address
 * @see maps_plugin_geocode_inside_area()
 * @see maps_service_geocode_cb()
 * @see maps_plugin_geocode()
 * @see maps_plugin_cancel_request()
 * @see maps_plugin_reverse_geocode()
 */
int maps_plugin_geocode_inside_area(const char *address,
				    const maps_area_h bounds,
				    const maps_item_hashtable_h preference,
				    maps_service_geocode_cb callback,
				    void *user_data, int *request_id);

/**
 * @brief	Gets the position coordinates for a given address.
 * @details This function obtains position coordinates for a given structured
 * address.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 * \n To cancel the request use maps_plugin_cancel_request().
 * \n To check if Maps Provider is capable of Geocoding and which Geocode
 * preferences are supported see the lists of capacities and preferences above.
 *
 * @param[in]	address		The structured address
 * @param[in]	preference	The set of preferences for processing Geocode
 * @param[in]	callback	The callback which will receive position
 * coordinates
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @param[out]	request_id	The id of request
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network connection failed
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @post It invokes maps_service_geocode_cb() to deliver obtained position
 * coordinates.

 * @see maps_plugin_geocode()
 * @see maps_plugin_geocode_inside_area()
 * @see maps_service_geocode_cb()
 * @see maps_plugin_cancel_request()
 * @see maps_plugin_reverse_geocode()
 */
int maps_plugin_geocode_by_structured_address(const maps_address_h address,
					      const maps_preference_h
					      preference,
					      maps_service_geocode_cb
					      callback, void *user_data,
					      int *request_id);

/**
 * @brief	Gets the address for a given position coordinates.
 * @details This function obtains structured address information for a given
 * position coordinates.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 * \n To cancel the request use maps_plugin_cancel_request().
 * \n To check if Maps Provider is capable of Reverse Geocoding and which
 * Reverse Geocode preferences are supported see the lists of capacities and
 * preferences above.
 *
 * @param[in]	latitude	The latitude [-90.0 ~ 90.0] (degrees)
 * @param[in]	longitude	The longitude [-180.0 ~ 180.0] (degrees)
 * @param[in]	preference	The set of preferences for processing Reverse
 * Geocode
 * @param[in]	callback	The callback which will receive address
 * information
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @param[out]	request_id	The id of request
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network connection failed
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @post This function invokes maps_service_reverse_geocode_cb().
 *
 * @see maps_service_reverse_geocode_cb()
 * @see maps_plugin_cancel_request()
 * @see maps_plugin_geocode()
 */
int maps_plugin_reverse_geocode(double latitude, double longitude,
				const maps_item_hashtable_h preference,
				maps_service_reverse_geocode_cb callback,
				void *user_data, int *request_id);

/*----------------------------------------------------------------------------*/
/*
 * Place
 */

/**
 * @brief	Queries a Place information by a coordinates position and a
 * distance.
 * @details This function obtains the Place information for a specified distance
 * around a given coordinates position.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 * \n To cancel the search request use maps_plugin_cancel_request().
 * \n To check if Maps Provider is capable of Place Search and which Place
 * preferences are supported see the lists of capacities and preferences above.
 *
 * @param[in]	position	The interested position
 * @param[in]	distance	The search area distance
 * @param[in]	filter		The filter handle
 * @param[in]	preference	The place preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @param[out]	request_id	A Request id, can be set to NULL if does not
 * require operation cancel
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_RESOURCE_BUSY Places service busy
 * @retval	#MAPS_ERROR_CANCELED Places service aborted
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @post It invokes maps_service_search_place_cb() to deliver obtained Place
 * information.
 *
 * @see maps_plugin_search_place_by_area()
 * @see maps_plugin_search_place_by_address()
 * @see maps_plugin_cancel_request()
 * @see maps_service_search_place_cb()
 */
int maps_plugin_search_place(const maps_coordinates_h position, int distance,
			     const maps_place_filter_h filter,
			     maps_preference_h preference,
			     maps_service_search_place_cb callback,
			     void *user_data, int *request_id);

/**
 * @brief	Queries a Place information by a coordinates boundary.
 * @details This function obtains the Place information for a specified
 * coordinates boundary.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 * \n @a boundary is supporting only circle type bounds for search.
 * \n To cancel the search request use maps_plugin_cancel_request().
 * \n To check if Maps Provider is capable of Place Search and which Place
 * preferences are supported see the lists of capacities and preferences above.
 *
 * @param[in]	boundary	The interested area
 * @param[in]	filter		The filter handle
 * @param[in]	preference	The place preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @param[out]	request_id	A Request id, can be set to NULL if does not
 * required operation cancel
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_RESOURCE_BUSY Places service busy
 * @retval	#MAPS_ERROR_CANCELED Places service aborted
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @post It invokes maps_service_search_place_cb() to deliver obtained Place
 * information.
 *
 * @see maps_plugin_search_place()
 * @see maps_plugin_search_place_by_address()
 * @see maps_plugin_cancel_request()
 * @see maps_service_search_place_cb()
 */
int maps_plugin_search_place_by_area(const maps_area_h boundary,
				     const maps_place_filter_h filter,
				     maps_preference_h preference,
				     maps_service_search_place_cb callback,
				     void *user_data, int *request_id);

/**
 * @brief	Queries a Place information by a free-formed address string.
 * @details This function obtains the Place information for a specified free-
 * formed address string.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 * \n @a boundary is supporting only circle type bounds for search.
 * \n To cancel the search request use maps_plugin_cancel_request().
 * \n To check if Maps Provider is capable of Place Search and which Place
 * preferences are supported see the lists of capacities and preferences above.
 *
 * @param[in]	address		The interested address
 * @param[in]	boundary	The interested area
 * @param[in]	filter		The filter handle
 * @param[in]	preference	The place preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @param[out]	request_id	A Request id, can be set to NULL if does not
 * required operation cancel
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_RESOURCE_BUSY Places service busy
 * @retval	#MAPS_ERROR_CANCELED Places service aborted
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @post It invokes maps_service_search_place_cb() to deliver obtained Place
 * information.
 *
 * @see maps_plugin_search_place()
 * @see maps_plugin_search_place_by_area()
 * @see maps_plugin_cancel_request()
 * @see maps_service_search_place_cb()
 */
int maps_plugin_search_place_by_address(const char *address,
					const maps_area_h boundary,
					const maps_place_filter_h filter,
					maps_preference_h preference,
					maps_service_search_place_cb callback,
					void *user_data, int *request_id);

/*----------------------------------------------------------------------------*/
/*
 * Route
 */

/**
 * @brief	Queries the Route from origin coordinate to a destination.
 * @details This function obtains the Route information for a specified origin
 * and destination coordinates.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 * \n To cancel the search request use maps_plugin_cancel_request().
 * \n To check if Maps Provider is capable of Route Search and which Route
 * preferences are supported see the lists of capacities and preferences above.
 *
 * @param[in]	origin		The starting point
 * @param[in]	destination	The destination
 * @param[in]	preference	The Route preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @param[out]	request_id	The request ID
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Service unavailable
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @post It invokes maps_service_search_route_cb() to deliver obtained Route
 * information.
 *
 * @see maps_plugin_search_route_waypoints()
 * @see maps_plugin_cancel_request()
 * @see maps_service_search_route_cb()
 */
int maps_plugin_search_route(const maps_coordinates_h origin,
			     const maps_coordinates_h destination,
			     maps_preference_h preference,
			     maps_service_search_route_cb callback,
			     void *user_data, int *request_id);

/**
 * @brief	Queries the Route, passing through a specified way points.
 * @details This function obtains the Route information for the Route, passing
 * through a specified set of way points.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 * \n To cancel the search request use maps_plugin_cancel_request().
 * \n To check if Maps Provider is capable of Route Search and which Route
 * preferences are supported see the lists of capacities and preferences above.
 *
 * @param[in]	waypoint_list	The list of way points to go through
 * @param[in]	waypoint_num	The number of way points to go through
 * @param[in]	preference	The Route preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @param[out]	request_id	The request ID
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Service unavailable
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @post It invokes maps_service_search_route_cb() to deliver obtained Route
 * information.
 *
 * @see maps_plugin_search_route()
 * @see maps_plugin_cancel_request()
 * @see maps_service_search_route_cb()
 */
int maps_plugin_search_route_waypoints(const maps_coordinates_h *waypoint_list,
				       int waypoint_num,
				       maps_preference_h preference,
				       maps_service_search_route_cb callback,
				       void *user_data, int *request_id);

/*----------------------------------------------------------------------------*/
/*
 * Cancel Request
 */

/**
 * @brief	Cancels the service request.
 * @details This function cancels the service request initiated by geocode,
 * route or place search.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks This function requires network access.
 *
 * @param[in]	request_id	The id of request
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network connection failed
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @pre @a request_id is obtained from one of following:
 *  - maps_plugin_geocode()
 *  - maps_plugin_geocode_inside_area()
 *  - maps_plugin_geocode_by_structured_address()
 *  - maps_plugin_reverse_geocode()
 *  - maps_plugin_search_place()
 *  - maps_plugin_search_place_by_area()
 *  - maps_plugin_search_place_by_address()
 *  - maps_plugin_search_route()
 *  - maps_plugin_search_route_waypoints()
 *  .
 *
 * @see maps_plugin_geocode()
 * @see maps_plugin_geocode_inside_area()
 * @see maps_plugin_geocode_by_structured_address()
 * @see maps_plugin_reverse_geocode()
 * @see maps_plugin_search_place()
 * @see maps_plugin_search_place_by_area()
 * @see maps_plugin_search_place_by_address()
 * @see maps_plugin_search_route()
 * @see maps_plugin_search_route_waypoints()
 */
int maps_plugin_cancel_request(int request_id);


/*----------------------------------------------------------------------------*/

/**
 * @brief	Set a maps view.
 * @details This function sets a maps view to the plugin.
 * @since_tizen 3.0
 * @remarks When the Maps View is being destroying, the parameter @a view is set
 * to NULL.
 *
 * @param[in]	view		The maps view
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #map_view_h
 */
int maps_plugin_set_map_view(const map_view_h view);

/**
 * @brief	Called when the map rendering is finished.
 * @details The Plugin invokes this callback when the rendering of the requested
 * part of map is finished.
 * @since_tizen 3.0
 * @remarks The parameter @a center must be released using
 * maps_coordinates_destroy().
 * \n To use the @a center outside of this function, copy the handle using
 * maps_coordinates_clone() function.
 * \n The parameter @a area must be released using maps_area_destroy()
 * \n To use the @a area outside of this function, copy the handle using
 * maps_area_clone() function.
 *
 * @param[in]	result		The result of request
 * @param[in]	request_id	The id of request, start from 0
 * @param[in]	centes		The coordinates of the center of requested map
 * area
 * @param[in]	area		The requested map area
 * @param[in]	user_data	The user data passed from
 * maps_plugin_render_map()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_plugin_render_map() will invoke this callback.
 *
 * @see maps_plugin_render_map()
 */
typedef void(*maps_plugin_render_map_cb) (maps_error_e result, int request_id,
					  maps_coordinates_h center,
					  maps_area_h area,
					  void* user_data);

/**
 * @brief	Request a map rendering.
 * @details This function request a draw routine of the map location with a
 * specified zoom factor and rotation angle.
 * @since_tizen 3.0
 *
 * @param[in]	coordinates	The coordinates of location to draw
 * @param[in]	zoom_factor	The zoom factor
 * @param[in]	rotation_angle	The rotation factor
 * @param[in]	tilt		The tilt
 * @param[in]	callback	The callback to notify that the rendering is
 * finished
 * @param[in]	user_data	The user data to be passed to the callback
 * @param[out]	request_id	The id of request
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre the maps view is set with maps_plugin_set_map_view().
 * @post It invokes maps_plugin_render_map_cb() to notify that the rendering is
 * finished
 *
 * @see maps_plugin_set_view()
 * @see maps_plugin_render_map_cb()
 * @see maps_plugin_draw_map()
 */
int maps_plugin_render_map(const maps_coordinates_h coordinates,
			  const double zoom_factor,
			  const double rotation_angle,
			  const double tilt,
			  maps_plugin_render_map_cb callback,
			  void* user_data,
			  int* request_id);

/**
 * @brief	Request the Plugin to move a map on a given delta.
 * @details This function request the Plugin to move a map on a given delta
 * screen coordinates. The current values of zoom, orientation or tilt are
 * remaining same.
 * @since_tizen 3.0
 *
 * @param[in]	delta_x		The delta x
 * @param[in]	delta_y		The delta y
 * @param[in]	callback	The callback to notify that the rendering is
 * finished
 * @param[in]	user_data	The user data to be passed to the callback
 * @param[out]	request_id	The id of request
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre the maps view is set with maps_plugin_set_map_view().
 * @post It invokes maps_plugin_render_map_cb() to notify that the rendering is
 * finished
 *
 * @see maps_plugin_set_view()
 * @see maps_plugin_render_map_cb()
 * @see maps_plugin_render_map()
 * @see maps_plugin_draw_map()
 */
int maps_plugin_move_center(const int delta_x,
			    const int delta_y,
			    maps_plugin_render_map_cb callback,
			    void* user_data,
			    int* request_id);

/**
 * @brief	Draw a map on the maps view panel.
 * @details This function draws the map, requested previously on the maps view
 * panel in accordance with the current maps settings.
 * @since_tizen 3.0
 *
 * @param[in]	canvas		The canvas to draw on
 * @param[in]	x		The x coordinate on the canvas top left
 * @param[in]	y		The y coordinate on the canvas top left
 * @param[in]	width		The width of the cancas
 * @param[in]	height		The height of the cancas
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre the draing routine is requested with preliminary call of
 * maps_plugin_render_map().
 *
 * @see maps_plugin_set_view()
 * @see maps_plugin_render_map()
 */
int maps_plugin_draw_map(Evas* canvas,
			 const int x,
			 const int y,
			 const int width,
			 const int height);

/**
 * @brief	Notifyes that the visual object is changed.
 * @details This function notifyes the Plugin that the visual object is
 * changed. The possible causes of changes are enumerated in
 * #map_object_operation_e ind nclude object adding, moving, removing,
 * visibility modificating or editing object specific properties.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[in]	operation	The operation over the object
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_plugin_create()
 */
int maps_plugin_on_object(const map_object_h object,
			       const map_object_operation_e operation);

/**
 * @brief	Converts screen coordinates to the geographical coordinates.
 * @details This function converts screen coordinates to the geographical
 * coordinates accordingly to the current maps settings.
 * @since_tizen 3.0
 *
 * @param[in]	x		The x coordinate on the screen
 * @param[in]	y		The y coordinate on the screen
 * @param[out]	coordinates	The corresponding geographical coordinates
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_plugin_create()
 * @see maps_plugin_geography_to_screen()
 * @see #maps_coordinates_h
 */
int maps_plugin_screen_to_geography(const int x, const int y,
				    maps_coordinates_h* coordinates);

/**
 * @brief	Converts geographical coordinates to the screen coordinates.
 * @details This function converts geographical coordinates to the screen
 * coordinates accordingly to the current maps settings.
 * @since_tizen 3.0
 * @privlevel public
 *
 * @param[in]	coordinates	The geographical coordinates
 * @param[out]	x		The corresponding x coordinate on the screen
 * @param[out]	y		The corresponding y coordinate on the screen
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_plugin_create()
 * @see maps_plugin_screen_to_geography()
 * @see #maps_coordinates_h
 */
int maps_plugin_geography_to_screen(const maps_coordinates_h coordinates,
				    int* x, int* y);

/**
 * @brief	Gets the minimal zooms level of the Map.
 * @details This function gets the minimally available zoom level of the Map.
 * @since_tizen 3.0
 *
 * @param[out]	min_zoom_level	The minimally available zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_plugin_get_min_zoom_level(int *min_zoom_level);

/**
 * @brief	Gets the maximal zooms level of the Map.
 * @details This function gets the maximally available zoom level of the Map.
 * @since_tizen 3.0
 *
 * @param[out]	max_zoom_level	The maximally available zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_plugin_get_max_zoom_level(int *max_zoom_level);

/**
 * @brief	Gets the minimal tilt of the Map.
 * @details This function gets the minimally available tilt of the Map
 * on the Map View.
 * @since_tizen 3.0
 *
 * @param[out]	min_tilt	The minimally available tilt
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see map_plugin_get_tilt()
 * @see map_plugin_get_max_tilt()
 */
int maps_plugin_get_min_tilt(int *min_tilt);

/**
 * @brief	Gets the maximal tilt of the Map.
 * @details This function gets the maximally available tilt of the Map
 * on the Map View.
 * @since_tizen 3.0
 *
 * @param[out]	max_tilt	The maximally available tilt
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see map_plugin_get_tilt()
 * @see map_plugin_get_min_tilt()
 */
int maps_plugin_get_max_tilt(int *max_tilt);

/**
 * @brief	Get the central coordinates of a Map.
 * @details This function gets the central coordinates of a Map.
 * @since_tizen 3.0
 * @remarks @a coordinates must be released using maps_coordinates_destroy().
 *
 * @param[out]	coordinates	The pointer to #maps_coordinates_h in which to
 * store the geographical coordinates of the central position of the Map
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_plugin_get_min_zoom_level()
 * @see maps_plugin_get_max_zoom_level
 * @see maps_plugin_get_min_tilt
 * @see maps_plugin_get_max_tilt
 */
int maps_plugin_get_center(maps_coordinates_h *coordinates);


#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLUGIN_H__ */
