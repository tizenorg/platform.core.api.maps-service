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

#ifndef __MAPS_PREFERENCE_H__
#define __MAPS_PREFERENCE_H__

#include <tizen_type.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup		CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAPS_PREFERENCE_MODULE Preference
 *
 * @file maps_preference.h
 * @brief This file contains the enumerations of Maps API preferences
 *
 * @addtogroup CAPI_MAPS_PREFERENCE_MODULE
 * @{
 * @brief This provides enumerations of Maps API preferences

*
* The Preferences are organized as a key-value table where available Preference
* keys are following strings:
*
* * Place search preferences
*  - #MAPS_PLACE_FILTER_TYPE
*  - #MAPS_PLACE_FILTER_SORT_BY
*
* * Route search preferences
*  - #MAPS_ROUTE_FREEFORM_ADDR_TO_AVOID
*  - #MAPS_ROUTE_STRUCTED_ADDR_TO_AVOID
*  - #MAPS_ROUTE_CIRCLE_AREA_TO_AVOID
*  - #MAPS_ROUTE_RECT_AREA_TO_AVOID
*
*  - #MAPS_ROUTE_GEOMETRY_BOUNDING_BOX
*  - #MAPS_ROUTE_GEOMETRY_RETRIEVAL
*  - #MAPS_ROUTE_INSTRUCTION_GEOMETRY
*  - #MAPS_ROUTE_INSTRUCTION_BOUNDING_BOX
*  - #MAPS_ROUTE_INSTRUCTION_RETRIEVAL
*  - #MAPS_ROUTE_REALTIME_TRAFFIC
*  .
* .
*/

/**
 * @brief	The name of the preference indicating place type while searching
 * the place
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_PLACE_FILTER_TYPE	"MAPS_PLACE_FILTER_TYPE"

/**
 * @brief	The name of the preference indicating sorting key while
 * searching the place
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_PLACE_FILTER_SORT_BY "MAPS_PLACE_FILTER_SORT_BY"

/**
 * @brief	The name of the preference indicating free-form address to avoid
 * while computing the route
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_FREEFORM_ADDR_TO_AVOID "MAPS_ROUTE_FREEFORM_ADDR_TO_AVOID"

/**
 * @brief	The name of the preference indicating structured address to
 * avoid while computing the route
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_STRUCTED_ADDR_TO_AVOID "MAPS_ROUTE_STRUCTED_ADDR_TO_AVOID"

/**
 * @brief	The name of the preference indicating circular geographical area
 * to avoid while computing the route
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_CIRCLE_AREA_TO_AVOID "MAPS_ROUTE_CIRCLE_AREA_TO_AVOID"

/**
 * @brief	The name of the preference indicating rectangular geographical
 * area to avoid while computing the route
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_RECT_AREA_TO_AVOID "MAPS_ROUTE_RECT_AREA_TO_AVOID"

/**
 * @brief	The name of the preference indicating that route should be
 * computed within a specified bounding box
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_GEOMETRY_BOUNDING_BOX "MAPS_ROUTE_GEOMETRY_BOUNDING_BOX"

/**
 * @brief	The name of the preference indicating that geometry parameters
 * should be retrieved while route processing
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_GEOMETRY_RETRIEVAL "MAPS_ROUTE_GEOMETRY_RETRIEVAL"

/**
 * @brief	The name of the preference indicating that route should be
 * computed with geometry instructions
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_INSTRUCTION_GEOMETRY "MAPS_ROUTE_INSTRUCTION_GEOMETRY"

/**
 * @brief	The name of the preference indicating that route should be
 * computed with bounding box instructions
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_INSTRUCTION_BOUNDING_BOX "MAPS_ROUTE_INSTRUCTION_BOUNDING_BOX"

/**
 * @brief	The name of the preference indicating that route should be
 * computed correspondingly to retrieval instructions
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_INSTRUCTION_RETRIEVAL "MAPS_ROUTE_INSTRUCTION_RETRIEVAL"

/**
 * @brief	The name of the preference indicating that route should be
 * computed in accordance to real time traffic
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
#define MAPS_ROUTE_REALTIME_TRAFFIC "MAPS_ROUTE_REALTIME_TRAFFIC"

/**
 * @brief	The Maps Preference handle.
 * @details The Maps Preference handle can be obtained via call of
 * maps_preference_create().
 * \n To release the handle use maps_preference_destroy().
 * \n To clone the handle use maps_preference_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see maps_preference_create()
 * @see maps_preference_destroy()
 * @see maps_preference_clone()
 */
typedef void *maps_preference_h;

/**
 * @brief	Enumeration of allowed distance units.
 * @details This enumeration represents allowed distance units used in Maps
 * Services.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see #maps_preference_h
 */
typedef enum _maps_distance_unit_e {
	MAPS_DISTANCE_UNIT_M,	/**< for Meter */
	MAPS_DISTANCE_UNIT_KM,	/**< for Kilometer */
	MAPS_DISTANCE_UNIT_FT,	/**< for Foot */
	MAPS_DISTANCE_UNIT_YD	/**< for Yard */
} maps_distance_unit_e;

/**
 * @brief	Enumeration of allowed route optimization option.
 * @details This enumeration represents allowed route optimization option used
 * in Route Service.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see #_maps_route_transport_mode_e
 * @see #_maps_route_feature_weight_e
 * @see #_maps_route_request_feature_e
 */
typedef enum _maps_route_optimization_e {
	MAPS_ROUTE_TYPE_FASTEST,    /**< Indicates the fastest route */
	MAPS_ROUTE_TYPE_SHORTEST,   /**< Indicates the shortest route (car mode only) */
	MAPS_ROUTE_TYPE_ECONOMIC,   /**< Indicates the most economic route (car mode only) */
	MAPS_ROUTE_TYPE_SCENIC,     /**< Indicates the most scenic route */
	MAPS_ROUTE_TYPE_FASTESTNOW, /**< Indicates the most fastest route now */
	MAPS_ROUTE_TYPE_DIRECTDRIVE /**< Indicates direct drive */
} maps_route_optimization_e;

/**
 * @brief	Enumeration of preferable route types.
 * @details This enumeration represents allowed route types used in Route
 * Service.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see #_maps_distance_unit_e
 * @see #_maps_route_feature_weight_e
 * @see #_maps_route_request_feature_e
 */
typedef enum _maps_route_transport_mode_e {
	MAPS_ROUTE_TRANSPORT_MODE_CAR,           /**< Indicates that the route is to be traveled by car. */
	MAPS_ROUTE_TRANSPORT_MODE_PEDESTRIAN,    /**< Indicates that the route is for a pedestrian. */
	MAPS_ROUTE_TRANSPORT_MODE_BICYCLE,       /**< Indicates that the route is for a cyclist. */
	MAPS_ROUTE_TRANSPORT_MODE_PUBLICTRANSIT, /**< Indicates that the route
	                                              is to be traveled  using public transport. */
	MAPS_ROUTE_TRANSPORT_MODE_TRUCK          /**< Indicates that the route is for a truck. */
} maps_route_transport_mode_e;

/**
 * @brief	Enumeration of route feature weights.
 * @details This enumeration represents allowed route feature weights used in
 * Route Service.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see #_maps_distance_unit_e
 * @see #_maps_route_transport_mode_e
 * @see #_maps_route_request_feature_e
 */
typedef enum _maps_route_feature_weight_e {
	MAPS_ROUTE_FEATURE_WEIGHT_NORMAL,       /**< Indicates normal weighting. */
	MAPS_ROUTE_FEATURE_WEIGHT_PREFER,       /**< Indicates that a feature is preferred. */
	MAPS_ROUTE_FEATURE_WEIGHT_AVOID,        /**< Indicates that a feature is to be avoided. */
	MAPS_ROUTE_FEATURE_WEIGHT_SOFTEXCLUDE,  /**< Indicates that soft-exclude applies to the feature. */
	MAPS_ROUTE_FEATURE_WEIGHT_STRICTEXCLUDE /**< Indicates that the feature is to be strictly excluded. */
} maps_route_feature_weight_e;

/**
 * @brief	Enumeration of route features.
 * @details This enumeration represents allowed route features used in Route
 * Service.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see #_maps_distance_unit_e
 * @see #_maps_route_transport_mode_e
 * @see #_maps_route_feature_weight_e
 */
typedef enum _maps_route_request_feature_e {
	MAPS_ROUTE_FEATURE_NO,             /**< Indicates no route features (are selected). */
	MAPS_ROUTE_FEATURE_TOLL,           /**< Indicates toll roads (toll gates/booths). */
	MAPS_ROUTE_FEATURE_MOTORWAY,       /**< Indicates motorway. */
	MAPS_ROUTE_FEATURE_BOATFERRY,      /**< Indicates a boat ferry. */
	MAPS_ROUTE_FEATURE_RAILFERRY,      /**< Indicates rail (train) ferry. */
	MAPS_ROUTE_FEATURE_PUBLICTTRANSIT, /**< Indicates public transport. */
	MAPS_ROUTE_FEATURE_TUNNEL,         /**< Indicates tunnel. */
	MAPS_ROUTE_FEATURE_DIRTROAD,       /**< Indicates dirt road. */
	MAPS_ROUTE_FEATURE_PARKS,          /**< Indicates park. */
	MAPS_ROUTE_FEATURE_HOVLANE,        /**< Indicates a high-occupancy vehicle lane. */
	MAPS_ROUTE_FEATURE_STAIRS          /**< Indicates stairs. */
} maps_route_feature_e;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Called when requesting the list of Maps Properties.
 * @details This callback is invoked while iterating through the list of Maps
 * Properties.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a key and @a value must be released using free() and corresponding
 * release method for property value correspondingly.
 *
 * @param[in]	index		The current index of property
 * @param[in]	total		The total amount of properties
 * @param[in]	key		The key of property
 * @param[in]	value		The value of property
 * @param[in]	user_data	The user data passed from
 * maps_preference_foreach_property()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_preference_foreach_property() will invoke this callback.
 *
 * @see maps_preference_foreach_property()
 */
typedef bool(*maps_preference_properties_cb) (int index, int total, char *key,
								char *value, void *user_data);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Creates a new maps preference handle.
 * @details This function creates a new maps preference handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a preference must be released using maps_preference_destroy().
 * \n @a preference may be cloned using maps_preference_clone().
 *
 * @param[out]	preference	A newly created preference handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see maps_preference_destroy()
 * @see maps_preference_clone()
 */
int maps_preference_create(maps_preference_h *preference);

/**
 * @brief	Destroys the maps preference handle and releases all its
 * resources.
 * @details This function destroys the maps preference handle and releases all
 * its resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_preference_clone()
 */
int maps_preference_destroy(maps_preference_h preference);

/**
 * @brief	Clones the maps preference handle.
 * @details This function clones the maps preference handle @a origin and all
 * its resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a cloned must be released using maps_preference_destroy().
 *
 * @param[in]	origin		The original preference handle
 * @param[out]	cloned		A cloned preference handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see maps_preference_destroy()
 */
int maps_preference_clone(const maps_preference_h origin,
								maps_preference_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the distance unit.
 * @details This function gets the maps distance unit.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[out]	unit	The distance unit
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get_distance_unit(const maps_preference_h preference,
								maps_distance_unit_e *unit);

/**
 * @brief	Gets the language.
 * @details This function gets the maps language.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a language must be released using free().
 *
 * @param[in]	preference	The preference handle
 * @param[out]	language	The language
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get_language(const maps_preference_h preference,
								char **language);

/**
 * @brief	Gets the max amount of results.
 * @details This function gets the max amount of results.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[out]	max_results	The max amount of results
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get_max_results(const maps_preference_h preference,
								int *max_results);

/**
 * @brief	Gets the country code.
 * @details This function gets the country code.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a country_code must be released using free().
 *
 * @param[in]	preference	The preference handle
 * @param[out]	country_code	The country code
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get_country_code(const maps_preference_h preference,
								char **country_code);

/**
 * @brief	Gets the route optimization.
 * @details This function gets the route optimization.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference		The preference handle
 * @param[out]	optimization	The route optimization
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get_route_optimization(const maps_preference_h preference,
								maps_route_optimization_e * optimization);

/**
 * @brief	Gets the route transport mode.
 * @details This function gets the route transport mode.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[out]	transport_mode	The transport mode
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get_route_transport_mode(const maps_preference_h preference,
								maps_route_transport_mode_e *transport_mode);

/**
 * @brief	Gets the route feature weight.
 * @details This function gets the route feature weight.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[out]	feature_weight	The feature weight
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get_route_feature_weight(const maps_preference_h preference,
								maps_route_feature_weight_e *feature_weight);

/**
 * @brief	Gets the route feature.
 * @details This function gets the route feature.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[out]	feature		The feature
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get_route_feature(const maps_preference_h preference,
								maps_route_feature_e *feature);

/**
 * @brief	Gets the enable status of alternative routes.
 * @details This function retrieves the enable status of alternative route.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[out]	enable		The enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get_route_alternatives_enabled(const maps_preference_h preference,
								bool *enable);

/**
 * @brief	Gets the maps preference value by key.
 * @details This function gets the maps preference value by key.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a value must be released using free().
 *
 * @param[in]	preference	The preference handle
 * @param[in]	key		The preference key
 * @param[out]	value		The preference value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_preference_get(const maps_preference_h preference,
								const char *key, char **value);

/**
 * @brief	Retrieves all maps properties.
 * @details This function retrieves all maps properties.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks The properties will be delivered via
 * maps_preference_properties_cb().
 *
 * @param[in]	preference	The preference handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @post This function invokes maps_preference_properties_cb() repeatedly to
 * retrieve each property.
 *
 * @see maps_preference_properties_cb()
 */
int maps_preference_foreach_property(const maps_preference_h preference,
								maps_preference_properties_cb callback, void *user_data);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Sets the maps distance unit.
 * @details This function sets the maps distance unit.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	unit		The distance unit
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_distance_unit()
 */
int maps_preference_set_distance_unit(maps_preference_h preference,
								const maps_distance_unit_e unit);

/**
 * @brief	Sets the maps language.
 * @details This function sets the maps language.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	language	The maps language.
 * A language is specified as an ISO 3166 alpha-2 two letter country-code 
 * followed by ISO 639-1 for the two-letter language code.
 * For example, "ko_KR" for Korean, "en_US" for American English.
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_language()
 */
int maps_preference_set_language(maps_preference_h preference,
								const char *language);

/**
 * @brief	Sets the max amount of results.
 * @details This function sets the max amount of results.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	max_results	The max amount of results
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_max_results()
 */
int maps_preference_set_max_results(maps_preference_h preference,
								const int max_results);

/**
 * @brief	Sets the maps country code.
 * @details This function sets the maps country code.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	country_code	The maps country code
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_country_code()
 */
int maps_preference_set_country_code(maps_preference_h preference,
								const char *country_code);

/**
 * @brief	Sets the route optimization.
 * @details This function sets the route optimization.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	optimization	The route optimization
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_route_optimization()
 */
int maps_preference_set_route_optimization(maps_preference_h preference,
								const maps_route_optimization_e optimization);

/**
 * @brief	Sets the route transport mode.
 * @details This function sets the route transport mode.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	transport_mode	The route transport mode
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_route_optimization()
 */
int maps_preference_set_route_transport_mode(maps_preference_h preference,
								const maps_route_transport_mode_e transport_mode);

/**
 * @brief	Sets the route feature weight.
 * @details This function sets the route feature weight.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	feature_weight	The route feature weight
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_route_feature_weight()
 */
int maps_preference_set_route_feature_weight(maps_preference_h preference,
								const maps_route_feature_weight_e feature_weight);

/**
 * @brief	Sets the route feature.
 * @details This function sets the route feature.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	feature		The route feature
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_route_feature()
 */
int maps_preference_set_route_feature(maps_preference_h preference,
								const maps_route_feature_e feature);

/**
 * @brief	Sets the enable status of alternative routes.
 * @details This function sets the alternative routes status.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	enable		The value to set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_route_alternatives_enabled()
 */
int maps_preference_set_route_alternatives_enabled(maps_preference_h preference,
								bool enable);

/**
 * @brief	Sets the preference value by key.
 * @details This function sets the preference value assigned with a specified
 * key.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	preference	The preference handle
 * @param[in]	key		The key
 * @param[in]	value		The value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_property()
 * @see maps_preference_foreach_property()
 */
int maps_preference_set_property(maps_preference_h preference,
								const char *key, const char *value);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PREFERENCE_H__ */
