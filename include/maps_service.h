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

#ifndef __MAPS_SERVICE_H__
#define __MAPS_SERVICE_H__

#include <tizen_type.h>
#include <maps_error.h>
#include <maps_address.h>
#include <maps_area.h>
#include <maps_coordinates.h>
#include <maps_place.h>
#include <maps_place_filter.h>
#include <maps_route.h>

/**
 *
 * @file maps_service.h
 * @brief This file contains the top level functions of Maps API
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
/*
 * Maps Service and Preference
 */

/**
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAPS_SERVICE_AND_PREFERENCE_MODULE Service and Providers
 *
 * @addtogroup CAPI_MAPS_SERVICE_AND_PREFERENCE_MODULE
 * @{
 * @brief This provides APIs related Search and Preference.
 *
 */

/**
 * @brief	Enumerations of maps requests available in the Maps Service
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
typedef enum _maps_service_e {
	MAPS_SERVICE_GEOCODE,					/**< Indicates that maps_service_geocode() service is allowed */
	MAPS_SERVICE_GEOCODE_INSIDE_AREA,		/**< Indicates that maps_service_geocode_inside_area() service is allowed */
	MAPS_SERVICE_GEOCODE_BY_STRUCTURED_ADDRESS,	/**< Indicates that	maps_service_geocode_by_structured_address() service is	allowed */
	MAPS_SERVICE_REVERSE_GEOCODE,			/**< Indicates that maps_service_reverse_geocode() service is allowed */
	MAPS_SERVICE_SEARCH_PLACE,				/**< Indicates that maps_service_search_place() service is allowed */
	MAPS_SERVICE_SEARCH_PLACE_BY_AREA,		/**< Indicates that maps_service_search_place_by_area() service is allowed */
	MAPS_SERVICE_SEARCH_PLACE_BY_ADDRESS,	/**< Indicates that maps_service_search_place_by_address() service is allowed */
	MAPS_SERVICE_SEARCH_ROUTE,				/**< Indicates that maps_service_search_route() service is allowed */
	MAPS_SERVICE_SEARCH_ROUTE_WAYPOINTS, 	/**< Indicates that maps_service_search_route_waypoints() service is allowed */
	MAPS_SERVICE_CANCEL_REQUEST,			/**< Indicates that maps_service_cancel_request() service is allowed */
	MAPS_SERVICE_MULTI_REVERSE_GEOCODE,		/**< Indicates that maps_service_multi_reverse_geocode() service is allowed @if MOBILE (Since 3.0) @endif */
	MAPS_SERVICE_SEARCH_PLACE_LIST,			/**< Indicates that maps_service_search_place_list() service is allowed @if MOBILE (Since 3.0) @endif */
	MAPS_SERVICE_SEARCH_GET_PLACE_DETAILS,	/**< Indicates that maps_service_search_get_place_details() service is allowed @if MOBILE (Since 3.0) @endif */

	MAPS_SERVICE_VIEW = 0x100				/**< Indicates that maps view service is allowed @if MOBILE (Since 3.0) @endif */
} maps_service_e;

/**
 * @brief Enumerations of maps features available in the Maps Service
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
typedef enum _maps_service_data_e {
	MAPS_PLACE_ADDRESS,		/**< Indicates the availability of address value in the Place data */
	MAPS_PLACE_RATING,		/**< Indicates the availability of rating value in the Place data */
	MAPS_PLACE_CATEGORIES,	/**< Indicates the availability of place category list in the Place data */
	MAPS_PLACE_ATTRIBUTES,	/**< Indicates the availability of place attribute list in the Place data */
	MAPS_PLACE_CONTACTS,	/**< Indicates the availability of place contact list in the Place data */
	MAPS_PLACE_EDITORIALS,	/**< Indicates the availability of place editorial list in the Place data */
	MAPS_PLACE_REVIEWS,		/**< Indicates the availability of place review list in the Place data */
	MAPS_PLACE_IMAGE,		/**< Indicates the availability of place image in Place the data */
	MAPS_PLACE_SUPPLIER,	/**< Indicates the availability of place supplier link value in the Place data */
	MAPS_PLACE_RELATED,		/**< Indicates the availability of related place link in the Place data */
	MAPS_ROUTE_PATH,		/**< Indicates that the Route Data Structure is defined as a Path (a list of geographical coordinates) */
	MAPS_ROUTE_SEGMENTS_PATH,		/**< Indicates that the Route Data Structure is defined as a list of Segments while each segment is defined as a Path */
	MAPS_ROUTE_SEGMENTS_MANEUVERS,	/**< Indicates that the Route Data Structure is defined as a list of Segments while each segment is defined as a list of Maneuvers*/

	MAPS_VIEW_TRAFFIC = 0x100,	/**< Indicates the availability of traffic information on the Map @if MOBILE (Since 3.0) @endif */
	MAPS_VIEW_PUBLIC_TRANSIT,	/**< Indicates the availability of public transit information on the Map @if MOBILE (Since 3.0) @endif */
	MAPS_VIEW_BUILDING,		/**< Indicates the availability of 3D building drawable on the Map @if MOBILE (Since 3.0) @endif */
	MAPS_VIEW_SCALEBAR		/**< Indicates the availability of scale bar on the Map @if MOBILE (Since 3.0) @endif */
} maps_service_data_e;

/**
 * @brief	The Maps Service handle
 * @details The Maps Service handle can be created by calling of maps_service_create().
 * \n To release the handle use maps_service_destroy().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @see maps_service_create()
 * @see maps_service_destroy()
 */
typedef void *maps_service_h;


/**
 * @brief	Called when requesting available Maps Providers.
 * @details A Maps Service invokes this callback iteratively as long as available Maps Providers exist.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The string @a maps_provider must be released using free().
 *
 * @param[in]	maps_provider	The info of Maps Provider
 * @param[in]	user_data	The user data passed from
 * maps_service_foreach_provider()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_service_foreach_provider() will invoke this callback.
 *
 * @see maps_service_foreach_provider()
 * @see maps_service_create()
 */
typedef bool(*maps_service_provider_info_cb) (char *maps_provider,
					      void *user_data);

/**
 * @brief	Gets available Maps Providers.
 * @details This function delivers available Maps Providers via
 * maps_service_provider_info_cb() callback.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	callback	The callback function to receive available Maps Providers
 * information
 * @param[out]	user_data	The user data to be passed to the callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @post This function invokes maps_service_provider_info_cb() to deliver Maps
 * Provider information.
 *
 * @see maps_service_provider_info_cb()
 * @see maps_service_create()
 */
int maps_service_foreach_provider(maps_service_provider_info_cb callback,
				  void *user_data);

/**
 * @brief	Creates a new Maps Service and assigns it with a handle.
 * @details While Maps Service is being created, a Maps Provider is initialized
 * and linked with Maps Service handle.
 * \n A Maps Service handle is used to query Maps Provider performing services,
 * such as Geocoding, Searching Places and Routing.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/network.get
 *
 * @remarks @a maps service handle must be released using maps_service_destroy().
 * \n use maps_service_foreach_provider() to choose one of available Providers.
 * \n use maps_service_set_provider_key() to set provider's key.
 * \n use use maps_service_set_preference() to set various options of Maps Provider.
 * \n use maps_service_provider_is_service_supported() and
 * maps_service_provider_is_data_supported() to check the Maps Provider's
 * capabilities
 *
 * @param[in]	maps_provider	The name of Maps Provider
 * @param[out]	maps		A handle of the new Maps Service on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre Call maps_service_foreach_provider() to get a available Maps Providers.
 *
 * @see maps_service_foreach_provider()
 * @see maps_service_destroy()
 * @see maps_service_geocode()
 * @see maps_service_geocode_inside_area()
 * @see maps_service_reverse_geocode()
 * @see maps_service_search_place()
 * @see maps_service_search_place_by_area()
 * @see maps_service_search_place_by_address()
 * @see maps_service_search_route()
 * @see maps_service_search_route_waypoints()
 * @see maps_service_set_provider_key()
 * @see maps_service_set_preference()
 * @see maps_service_provider_is_service_supported()
 * @see maps_service_provider_is_data_supported()
 */
int maps_service_create(const char *maps_provider, maps_service_h *maps);

/**
 * @brief	Destroys the Maps Service handle and releases all its resources.
 * @details This function releases all used resources of the Maps Service and Maps Provider.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 *
 * @param[in]	maps		The Maps Service handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 *
 * @see maps_service_create()
 */
int maps_service_destroy(maps_service_h maps);

/**
 * @brief	Sets the Maps Key to be used in the requests of Maps Service.
 * @details This function sets the Maps Provider's Key which will be used in each Maps
 * Service request to Maps Provider.
 * \n Maps key can be obtained with maps_service_get_provider_key().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks To get the @a provider_key, refer to corresponding Maps Provider
 * documentation.
 * \n To get app_id and app_code of HERE, visit https://developer.here.com/,
 * https://developer.here.com/rest-apis.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	provider_key	The Maps Key to be used
 * \n In case of combining two more strings, use slash("/") as a delimiter.
 * \n e.g. For HERE "app_id/app_code"
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 *
 * @see maps_service_create()
 * @see maps_service_get_provider_key()
 */
int maps_service_set_provider_key(maps_service_h maps,
				  const char *provider_key);

/**
 * @brief	Gets the Maps Key which is to be used in the Maps Service
 * requests
 * @details This function gets the Maps Key which is to be used in each Maps
 * Service request to Maps Provider.
 * \n Maps key can be set with maps_service_set_provider_key().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The string @a provider_key must be released using free().
 * \n @a maps_service_get_provider_key is always synchronous function.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[out]	provider_key	The Maps Key
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 *
 * @see maps_service_set_provider_key()
 * @see maps_service_create()
 */
int maps_service_get_provider_key(const maps_service_h maps,
				  char **provider_key);

/**
 * @brief	Sets the Maps Preference.
 * @details This function sets the Maps Preferences which are used in each Maps
 * Service request to Maps Provider.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	maps		The handle of Maps Service
 * @param[in]	preference	The handle of Maps Preference
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 *
 * @see maps_service_get_preference()
 * @see maps_service_create()
 */
int maps_service_set_preference(maps_service_h maps,
				maps_preference_h preference);

/**
 * @brief	Gets the Maps Preference.
 * @details This function gets the Maps Preferences which are used in each Maps
 * Service request to Maps Provider.
 * \n Preferences can be set with maps_service_set_preference().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a preference must be released using maps_preference_destroy().
 * \n @a maps_service_get_preference is always synchronous function.
 *
 * @param[in]	maps		The handle of Maps Service
 * @param[out]	preference	The handle of Maps Preference
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 *
 * @see maps_service_set_preference()
 * @see maps_service_create()
 */
int maps_service_get_preference(maps_service_h maps,
				maps_preference_h *preference);

/**
 * @brief	Checks if the Maps Service supports a request.
 * @details This function checks if the Maps Service supports a specified request.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a maps_service_provider_is_service_supported is always synchronous
 * function.
 *
 * @param[in]	maps		The handle of Maps Service
 * @param[in]	service		The service to be checked
 * @param[out]	supported	Is the service supported
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 *
 * @see maps_service_create()
 */
int maps_service_provider_is_service_supported(const maps_service_h maps,
					       maps_service_e service,
					       bool *supported);

/**
 * @brief	Checks if the Maps Service supports a data feature.
 * @details This function checks if the Maps Service supports a specified data
 * feature.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a maps_service_provider_is_data_supported is always synchronous
 * function.
 *
 * @param[in]	maps		The handle of Maps Service
 * @param[in]	data		The data feature to be checked
 * @param[out]	supported	Is the data feature supported
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 *
 * @see maps_service_create()
 */
int maps_service_provider_is_data_supported(const maps_service_h maps,
					    maps_service_data_e data,
					    bool *supported);


/*----------------------------------------------------------------------------*/
/*
 * Cancel Service Request
 */

/**
 * @brief	Cancels the service request.
 * @details This function cancels the service request initiated by geocoding
 * and searching places and routes.

 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 *
 * @param[in]	maps		The Maps Service handle
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
 * @pre @a request_id is obtained from one of followings:
 *  * maps_service_geocode()
 *  * maps_service_geocode_inside_area()
 *  * maps_service_geocode_by_structured_address()
 *  * maps_service_reverse_geocode()
 *  * maps_service_search_place()
 *  * maps_service_search_place_by_area()
 *  * maps_service_search_place_by_address()
 *  * maps_service_search_route()
 *  * maps_service_search_route_waypoints()
 * .
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 *
 * @see maps_service_geocode()
 * @see maps_service_geocode_inside_area()
 * @see maps_service_geocode_by_structured_address()
 * @see maps_service_reverse_geocode()
 * @see maps_service_search_place()
 * @see maps_service_search_place_by_area()
 * @see maps_service_search_place_by_address()
 * @see maps_service_search_route()
 * @see maps_service_search_route_waypoints()
 */
int maps_service_cancel_request(const maps_service_h maps, int request_id);

/**
 * @}
 */

/*----------------------------------------------------------------------------*/
/*
 * Geocoder Service
 */

/**
 *
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAPS_GEOCODER_MODULE Geocoder
 *
 * @addtogroup CAPI_MAPS_GEOCODER_MODULE
 * @{
 * @brief This provides APIs for Geocoder Service
 * @details The Maps Geocoding API allows translating an address to its
 * geographical location defined in terms of latitude and longitude;
 * the input can be a qualified, structured address or a free form single search
 * text with full or partial address information.
 * \n The Maps Reverse Geocoding API allows to inverse translating a
 * geographical location (longitude, latitude) to an address;
 * it can be used to answer the question "Where am I?".
 *
 */

/**
 * @brief	Called once for each position coordinates converted from the
 * given address information.
 * @details The Maps Service invokes this callback while iterating through the
 * list of obtained coordinates of the specified place.
 * \n If search is failed, the value of @a total is 0 and @a coordinates is NULL
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The parameter @a coordinates must be released using
 * maps_coordinates_destroy().
 * \n This error code will be reported. \n
 *			#MAPS_ERROR_NONE \n
 *			#MAPS_ERROR_OUT_OF_MEMORY \n
 *			#MAPS_ERROR_INVALID_PARAMETER \n
 *			#MAPS_ERROR_NOT_FOUND.
 *
 * @param[in]	result		The result of request
 * @param[in]	request_id	The id of request
 * @param[in]	index		The current index of coordinates in result set,
 * start from 0
 * @param[in]	total		The total number of results
 * @param[in]	coordinates	The coordinates of the place
 * @param[in]	user_data	The user data passed from maps_service_geocode()
 * or maps_service_geocode_inside_area()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_service_geocode() or maps_service_geocode_inside_area() will invoke
 * this callback.
 *
 * @see maps_service_geocode()
 * @see maps_service_geocode_inside_area()
 * @see maps_service_cancel_request()
 */
typedef bool(*maps_service_geocode_cb) (maps_error_e result, int request_id,
					int index, int total,
					maps_coordinates_h coordinates,
					void *user_data);

/**
 * @brief	Called when the address information is converted from position
 * information.
 * @details The Maps Service invokes this callback when the address is obtained
 * from the specified coordinates.
 * \n If search is failed, the value of @a total is 0 and @a address is NULL
 * @remarks The parameter @a address must be released using
 * maps_address_destroy().
 * \n This error code will be reported. \n
 *			#MAPS_ERROR_NONE \n
 *			#MAPS_ERROR_OUT_OF_MEMORY \n
 *			#MAPS_ERROR_INVALID_PARAMETER \n
 *			#MAPS_ERROR_NOT_FOUND.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	result		The result of request
 * @param[in]	request_id	The id of request
 * @param[in]	index		The current index of address data in result set,
 * start from 0
 * @param[in]	total		The total number of results
 * @param[in]	address		The requested address information
 * @param[in]	user_data	The user data passed from
 * maps_service_reverse_geocode()
 *
 * @pre maps_service_reverse_geocode() will invoke this callback.
 *
 * @see maps_service_reverse_geocode()
 * @see maps_service_cancel_request()
 */
typedef void (*maps_service_reverse_geocode_cb) (maps_error_e result,
						 int request_id, int index,
						 int total,
						 maps_address_h address,
						 void *user_data);

/**
 * @brief	Gets the position coordinates for a given address.
 * \n The request is asynchronous.
 * @details This function obtains position coordinates for a given free-formed
 * address string.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To cancel the request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of geocoding,
 * use maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_GEOCODE passed as @a service parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	address		The free-formed address
 * @param[in]	preference	The set of preferences for processing geocoding
 * @param[in]	callback	The callback which will receive position
 * coordinates
 * @param[in]	user_data	The user data to be passed to the callback function
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
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @post It invokes maps_service_geocode_cb() to deliver obtained position
 * coordinates.
 *
 * @see maps_service_geocode_inside_area()
 * @see maps_service_geocode_cb()
 * @see maps_service_cancel_request()
 * @see maps_service_reverse_geocode()
 * @see maps_service_provider_is_service_supported()
 */
int maps_service_geocode(const maps_service_h maps, const char *address,
			 const maps_preference_h preference,
			 maps_service_geocode_cb callback, void *user_data,
			 int *request_id);

/**
 * @brief	Gets the position for a given address, within the specified
 * bounding box.
 * \n The request is asynchronous.
 * @details This function obtains position coordinates for a given free-formed
 * address string within the specified bounding box.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks Polygonal bounding box is not supported.
 * \n To cancel the request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of geocoding inside area,
 * use maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_GEOCODE_INSIDE_AREA passed as @a service parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	address		The free-formed address
 * @param[in]	bounds		The bounding box
 * @param[in]	preference	The set of preferences for processing geocoding
 * @param[in]	callback	The callback which will receive position
 * coordinates
 * @param[in]	user_data	The user data to be passed to the callback function
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
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @post It invokes maps_service_geocode_cb() to deliver obtained position
 * coordinates.
 *
 * @see maps_service_geocode_by_structured_address()
 * @see maps_service_geocode_inside_area()
 * @see maps_service_geocode_cb()
 * @see maps_service_geocode()
 * @see maps_service_cancel_request()
 * @see maps_service_reverse_geocode()
 * @see maps_service_provider_is_service_supported()
 */
int maps_service_geocode_inside_area(const maps_service_h maps,
				     const char *address,
				     const maps_area_h bounds,
				     const maps_preference_h preference,
				     maps_service_geocode_cb callback,
				     void *user_data, int *request_id);

/**
 * @brief	Gets the position coordinates for a given address.
 * \n The request is asynchronous.
 * @details This function obtains position coordinates for a given structured
 * address.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To cancel the request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of geocoding by structured address,
 * use maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_GEOCODE_BY_STRUCTURED_ADDRESS passed as @a service parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	address		The structured address
 * @param[in]	preference	The set of preferences for processing geocoding
 * @param[in]	callback	The callback which will receive position
 * coordinates
 * @param[in]	user_data	The user data to be passed to the callback function
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
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @post It invokes maps_service_geocode_cb() to deliver obtained position
 * coordinates.

 * @see maps_service_geocode()
 * @see maps_service_geocode_inside_area()
 * @see maps_service_geocode_cb()
 * @see maps_service_cancel_request()
 * @see maps_service_reverse_geocode()
 * @see maps_service_provider_is_service_supported()
 */
int maps_service_geocode_by_structured_address(const maps_service_h maps,
					       const maps_address_h address,
					       const maps_preference_h
					       preference,
					       maps_service_geocode_cb
					       callback, void *user_data,
					       int *request_id);

/**
 * @brief	Gets the address for a given position coordinates.
 * \n The request is asynchronous.
 * @details This function obtains structured address information for a given
 * position coordinates.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To cancel the request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of reverse geocoding,
 * use maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_REVERSE_GEOCODE passed as @a service parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	latitude	The latitude [-90.0 ~ 90.0] (degrees)
 * @param[in]	longitude	The longitude [-180.0 ~ 180.0] (degrees)
 * @param[in]	preference	The set of preferences for processing reverse geocoding
 * @param[in]	callback	The callback which will receive address information
 * @param[in]	user_data	The user data to be passed to the callback function
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
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @post This function invokes maps_service_reverse_geocode_cb().
 *
 * @see maps_service_reverse_geocode_cb()
 * @see maps_service_cancel_request()
 * @see maps_service_geocode()
 * @see maps_service_provider_is_service_supported()
 */
int maps_service_reverse_geocode(const maps_service_h maps, double latitude,
				 double longitude,
				 const maps_preference_h preference,
				 maps_service_reverse_geocode_cb callback,
				 void *user_data, int *request_id);


/**
 * @brief	Called when responsed the request of multi reverse geocoding.
 * @details The Maps Service invokes this callback once when gets the response
 * from map service provider.
 * \n The value of @a total is same with requested coordinates list size.
 * Even though one of address is not provided valid address handle is retrieved.
 * @since_tizen 3.0
 * @remarks	You can get the respective address information of @a address_list
 * using maps_address_list_foreach().
 * \n This error code will be reported. \n
 *			#MAPS_ERROR_NONE \n
 *			#MAPS_ERROR_OUT_OF_MEMORY \n
 *			#MAPS_ERROR_INVALID_PARAMETER \n
 *			#MAPS_ERROR_NOT_FOUND.
 *
 * @param[in]	result			The result of request
 * @param[in]	request_id		The id of request
 * @param[in]	total			The total number of results
 * @param[in]	address_list		The converted address list from the
 * requested coordinates list
 * @param[in]	user_data		The user data pointer passed from
 * maps_service_multi_reverse_geocode()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_service_multi_reverse_geocode() will invoke this callback.
 *
 * @see maps_service_multi_reverse_geocode()
 * @see maps_address_list_foreach()
 * @see maps_service_cancel_request()
 */
typedef bool (*maps_service_multi_reverse_geocode_cb) (maps_error_e result,
					int request_id,
					int total,
					maps_address_list_h address_list,
					void *user_data);

/**
 * @brief	Gets the address list for a given position coordinates list.
 * \n The request is asynchronous.
 * @details This function obtains structured address information.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To cancel the request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of multi reverse geocoding,
 * use maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_MULTI_REVERSE_GEOCODE passed as @a service parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	coordinates_list The coordinates list [2 ~ 100] (a set of coordinates)
 * @param[in]	preference	The set of preferences for processing Reverse
 * Geocode
 * @param[in]	callback	The callback which will retrieve address list
 * @param[in]	user_data	The user data pointer to be passed to the
 * callback
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
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @post This function invokes maps_service_multi_reverse_geocode_cb().
 *
 * @see maps_service_multi_reverse_geocode_cb()
 * @see maps_service_cancel_request()
 * @see maps_service_reverse_geocode()
 * @see maps_service_provider_is_service_supported()
 */
int maps_service_multi_reverse_geocode(const maps_service_h maps,
				maps_coordinates_list_h coordinates_list,
				const maps_preference_h preference,
				maps_service_multi_reverse_geocode_cb callback,
				void *user_data, int *request_id);
/**
 * @}
 */

/*----------------------------------------------------------------------------*/
/*
 * Place Service
 */

/**
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_MODULE Places
 *
 * @addtogroup CAPI_MAPS_PLACE_MODULE
 * @{
 * @brief This provides APIs for Place Service
 * @details The Maps Place API allows to find places that are relevant to user
 * discovery context.
 *
 */

/**
 * @brief	Called for each result of Place Search request
 * @details The Maps Service invokes this callback while iterating through the
 * set of obtained Place data.
 * \n If search is failed, the value of @a total is 0 and @a place is NULL
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The parameter @a place must be released using maps_place_destroy().
 * \n This error code will be reported. \n
 *			#MAPS_ERROR_NONE \n
 *			#MAPS_ERROR_OUT_OF_MEMORY \n
 *			#MAPS_ERROR_INVALID_PARAMETER \n
 *			#MAPS_ERROR_NOT_FOUND.
 *
 * @param[in]	error		The result of request
 * @param[in]	request_id	The request id
 * @param[in]	index		The current index of place data in result set,
 * start from 0
 * @param[in]	total		The total number of results
 * @param[in]	place		The resulting Place data
 * @param[in]	uesr_data	The user data passed from
 * maps_service_search_place(), maps_service_search_place_by_area() or
 * maps_service_search_place_by_address()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_service_search_place(), maps_service_search_place_by_area() or
 * maps_service_search_place_by_address() will invoke this callback.
 *
 * @see maps_service_search_place()
 * @see maps_service_search_place_by_area()
 * @see maps_service_search_place_by_address()
 */
typedef bool(*maps_service_search_place_cb) (maps_error_e error,
					     int request_id, int index,
					     int total, maps_place_h place,
					     void *user_data);

/**
 * @brief	Called for Place list of Place Search request
 * @details The Maps Service invokes this callback when the Place data obtained
 * from the specified coordinates.
 * \n If search is failed, the value of @a total is 0 and @a place_list is NULL
 * @since_tizen 3.0
 * @remarks	The parameter @a place_list must be released using maps_place_list_destroy().
 * \n This error code will be reported. \n
 *			#MAPS_ERROR_NONE \n
 *			#MAPS_ERROR_OUT_OF_MEMORY \n
 *			#MAPS_ERROR_INVALID_PARAMETER \n
 *			#MAPS_ERROR_NOT_FOUND.
 *
 * @param[in]	error		The result of request
 * @param[in]	request_id	The request id
 * @param[in]	total		The total number of results
 * @param[in]	place_list	The resulting Place list data
 * @param[in]	uesr_data	The user data passed from maps_service_search_place_list()
 *
 * @pre maps_service_search_place_list() will invoke this callback.
 *
 * @see maps_service_search_place_list()
 * @see maps_place_list_foreach()
 */
typedef void(*maps_service_search_place_list_cb) (maps_error_e error,
					     int request_id, int total, maps_place_list_h place_list, void *user_data);

/**
 * @brief	Called for Place datail information of Place Search request
 * @details The Maps Service invokes this callback when the Place detail data
 * obtained from the specified uri.
 * \n If search is failed, @a place is NULL
 * @since_tizen 3.0
 * @remarks The parameter @a place must be released using maps_place_destroy().
 * \n This error code will be reported. \n
 *			#MAPS_ERROR_NONE \n
 *			#MAPS_ERROR_OUT_OF_MEMORY \n
 *			#MAPS_ERROR_INVALID_PARAMETER \n
 *			#MAPS_ERROR_NOT_FOUND.
 *
 * @param[in]	error		The result of request
 * @param[in]	request_id	The request id
 * @param[in]	place		The resulting Place data
 * @param[in]	uesr_data	The user data passed from maps_service_get_place_details()
 *
 * @pre maps_service_get_place_details() will invoke this callback.
 *
 * @see maps_service_get_place_details()
 */
typedef void(*maps_service_get_place_details_cb) (maps_error_e error,
					     int request_id, maps_place_h place, void *user_data);

/**
 * @brief	Queries a Place information by a coordinates position and a
 * distance.
 * \n The request is asynchronous.
 * @details This function obtains the Place information for a specified distance
 * around a given coordinates position.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To cancel the search request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of searching places, use
 * maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_SEARCH_PLACE passed as @a service parameter.
 * \n Check available data features in the search result using
 * maps_service_provider_is_data_supported() with values, listed in
 * #maps_service_data_e passed as @a data parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	position	The interested position
 * @param[in]	distance	The search area distance
 * @param[in]	filter		The filter handle
 * @param[in]	preference	The place preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	request_id	The request id
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
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @post It invokes maps_service_search_place_cb() to deliver obtained Place
 * information.
 *
 * @see maps_service_search_place_by_area()
 * @see maps_service_search_place_by_address()
 * @see maps_service_cancel_request()
 * @see maps_service_search_place_cb()
 * @see maps_service_provider_is_service_supported()
 * @see maps_service_provider_is_data_supported()
 */
int maps_service_search_place(const maps_service_h maps,
			      const maps_coordinates_h position, int distance,
			      const maps_place_filter_h filter,
			      maps_preference_h preference,
			      maps_service_search_place_cb callback,
			      void *user_data, int *request_id);

/**
 * @brief	Queries a Place information by a coordinates boundary.
 * \n The request is asynchronous.
 * @details This function obtains the Place information for a specified
 * coordinates boundary.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks @a boundary is supporting only circle type bounds for search.
 * \n To cancel the search request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of searching places by coordinates
 * boundaries, use maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_SEARCH_PLACE_BY_AREA  passed as @a service parameter.
 * \n Check available data features in the search result using
 * maps_service_provider_is_data_supported() with values, listed in
 * #maps_service_data_e passed as @a data parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	boundary	The interested area
 * @param[in]	filter		The filter handle
 * @param[in]	preference	The place preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	request_id	The request id
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
 * @pre Call maps_service_create() to create Maps Service and obtain its
 * handle.
 * @post It invokes maps_service_search_place_cb() to deliver obtained Place
 * information.
 *
 * @see maps_service_search_place()
 * @see maps_service_search_place_by_address()
 * @see maps_service_cancel_request()
 * @see maps_service_search_place_cb()
 * @see maps_service_provider_is_service_supported()
 * @see maps_service_provider_is_data_supported()
 */
int maps_service_search_place_by_area(const maps_service_h maps,
				      const maps_area_h boundary,
				      const maps_place_filter_h filter,
				      maps_preference_h preference,
				      maps_service_search_place_cb callback,
				      void *user_data, int *request_id);

/**
 * @brief	Queries a Place information by a free-formed address string.
 * \n The request is asynchronous.
 * @details This function obtains the Place information for a specified
 * free-formed address string.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks @a boundary is supporting only circle type bounds for search.
 * \n To cancel the search request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of searching places by free-formed
 * address, use maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_SEARCH_PLACE_BY_ADDRESS passed as @a service parameter.
 * \n Check available data features in the search result using
 * maps_service_provider_is_data_supported() with values, listed in
 * #maps_service_data_e passed as @a data parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	address		The interested address
 * @param[in]	boundary	The interested area
 * @param[in]	filter		The filter handle
 * @param[in]	preference	The place preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	request_id	The request id
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
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @post It invokes maps_service_search_place_cb() to deliver obtained Place
 * information.
 *
 * @see maps_service_search_place()
 * @see maps_service_search_place_by_area()
 * @see maps_service_cancel_request()
 * @see maps_service_search_place_cb()
 * @see maps_service_provider_is_service_supported()
 * @see maps_service_provider_is_data_supported()
 */
int maps_service_search_place_by_address(const maps_service_h maps,
					 const char *address,
					 const maps_area_h boundary,
					 const maps_place_filter_h filter,
					 maps_preference_h preference,
					 maps_service_search_place_cb
					 callback, void *user_data,
					 int *request_id);

/**
 * @brief	Queries a brief Place information by a coordinates boundary.
 * /n The request is asynchronous.
 * @details This function obtains the brief Place information for a specified coordinates boundary.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To cancel the search request use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of Place Search and which Place preferences are supported,
 * see the lists of capacities and preferences above.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	boundary	The interested area
 * @param[in]	filter		The filter handle
 * @param[in]	preference	The place preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	request_id	The request id
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
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @post It invokes maps_service_search_place_list_cb() to deliver obtained Place information.
 *
 * @see maps_service_get_place_details()
 * @see maps_service_search_place_list_cb()
 * @see maps_service_cancel_request()
 * @see maps_service_provider_is_service_supported()
 */
int maps_service_search_place_list(const maps_service_h maps,
			      const maps_area_h boundary,
			      const maps_place_filter_h filter,
			      maps_preference_h preference,
			      maps_service_search_place_list_cb callback,
			      void *user_data, int *request_id);

/**
 * @brief	Queries a Detail place information by a place uri.
 * \n The request is asynchronous.
 * @details This function obtains the Detail place information for a specified place uri
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To cancel the search request use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of Place Search and which Place preferences are supported,
 * see the lists of capacities and preferences above.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	uri			The interested place uri
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	request_id	The request id
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
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @pre @a uri is obtained from maps_service_search_place_list()
 * @post It invokes maps_service_get_place_details_cb() to deliver obtained Place information.
 *
 * @see maps_service_search_place_list()
 * @see maps_service_get_place_details_cb()
 * @see maps_service_cancel_request()
 * @see maps_service_provider_is_service_supported()
 */
int maps_service_get_place_details(const maps_service_h maps,
			      const char *uri, maps_service_get_place_details_cb callback,
			      void *user_data, int *request_id);

/**
 * @}
 */

/*----------------------------------------------------------------------------*/
/*
 * Route Service
 */

/**
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAPS_ROUTE_MODULE Routes
 *
 * @addtogroup CAPI_MAPS_ROUTE_MODULE
 * @{
 * @brief This provides APIs for Route Service
 * @details The Maps Route API provides functions to calculate a route that
 * defines a path between a origin and a destination and may, optionally, pass
 * through specific intermediate locations.
 *
 */

/**
 * @brief	Called when the requested routes are found.
 * @details The Maps Service invokes this callback while iterating through the
 * set of obtained Routes.
 * \n If search is failed, the value of @a total is 0 and @a route is NULL.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The parameter @a route must be released using maps_route_destroy().
 * \n This error code will be reported. \n
 *			#MAPS_ERROR_NONE \n
 *			#MAPS_ERROR_OUT_OF_MEMORY \n
 *			#MAPS_ERROR_INVALID_PARAMETER \n
 *			#MAPS_ERROR_NOT_FOUND.
 *
 * @param[in]	error		The result of request
 * @param[in]	request_id	The id of request
 * @param[in]	index		The current index of Route in result set, start
 * from 0
 * @param[in]	total		The total number of result
 * @param[in]	route		The Route data
 * @param[in]	user_data	The user data passed from
 * maps_service_search_route() or maps_service_search_route_waypoints()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_service_search_route() or maps_service_search_route_waypoints()
 * will invoke this callback.
 *
 * @see maps_service_search_route()
 * @see maps_service_search_route_waypoints()
 * @see maps_service_cancel_request()
 */
typedef bool(*maps_service_search_route_cb) (maps_error_e error,
					     int request_id, int index,
					     int total, maps_route_h route,
					     void *user_data);

/**
 * @brief	Queries the Route from origin coordinates to destination.
 * \n The request is asynchronous.
 * @details This function gets the Route information for a specified origin and
 * destination coordinates.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To cancel the search request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of searching routes, use
 * maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_SEARCH_ROUTE passed as @a service parameter.
 * \n Check available data features in the search result using
 * maps_service_provider_is_data_supported() with values, listed in
 * #maps_service_data_e passed as @a data parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	origin		The starting point
 * @param[in]	destination	The destination
 * @param[in]	preference	The Route preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	request_id	The request id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 * @post It invokes maps_service_search_route_cb() to deliver obtained Route
 * information.
 *
 * @see maps_service_search_route_waypoints()
 * @see maps_service_cancel_request()
 * @see maps_service_search_route_cb()
 * @see maps_service_provider_is_service_supported()
 * @see maps_service_provider_is_data_supported()
 */
int maps_service_search_route(const maps_service_h maps,
			      const maps_coordinates_h origin,
			      const maps_coordinates_h destination,
			      maps_preference_h preference,
			      maps_service_search_route_cb callback,
			      void *user_data, int *request_id);

/**
 * @brief	Queries the Route, passing through a specified way points.
 * \n The request is asynchronous.
 * @details This function gets the Route information for the Route, passing
 * through a specified set of way points.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To cancel the search request, use maps_service_cancel_request().
 * \n To check if Maps Provider is capable of searching the route passing
 * through the specified way points, use
 * maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_SEARCH_ROUTE_WAYPOINTS passed as @a service parameter.
 * \n Check available data features in the search result using
 * maps_service_provider_is_data_supported() with values, listed in
 * #maps_service_data_e passed as @a data parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	waypoint_list	The list of way points to go through
 * @param[in]	waypoint_num	The number of way points to go through
 * @param[in]	preference	The Route preference handle
 * @param[in]	callback	The result callback
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	request_id	The request id
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
 * @pre Call maps_service_create() to create Maps Service and get its handle.
 * @post It invokes maps_service_search_route_cb() to deliver obtained Route
 * information.
 *
 * @see maps_service_search_route()
 * @see maps_service_cancel_request()
 * @see maps_service_search_route_cb()
 * @see maps_service_provider_is_service_supported()
 * @see maps_service_provider_is_data_supported()
 */
int maps_service_search_route_waypoints(const maps_service_h maps,
					const maps_coordinates_h *
					waypoint_list, int waypoint_num,
					maps_preference_h preference,
					maps_service_search_route_cb callback,
					void *user_data, int *request_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif				/* __MAPS_SERVICE_H__ */
