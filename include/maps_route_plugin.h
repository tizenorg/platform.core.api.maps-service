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

#ifndef __MAPS_ROUTE_PLUGIN_H__
#define __MAPS_ROUTE_PLUGIN_H__

#include <maps_route.h>
#include <maps_extra_types.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_ROUTE_MODULE Routes
 */

/**
 *
 * @ingroup	CAPI_MAPS_PLUGIN_ROUTE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_ROUTE_DATA_MODULE Route
 *
 * @file maps_route_plugin.h
 * @brief This file contains the functions related to Route
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_ROUTE_DATA_MODULE
 * @{
 * @brief This provides APIs related to Route information, used in
 * Route Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Creates a new route handle.
 * @details This function creates a new route handle and allocates all needed
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a route must be released using maps_route_destroy().
 * \n @a route may be cloned using maps_route_clone().
 *
 * @param[out]	route		A newly created route handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see maps_route_destroy()
 * @see maps_route_clone()
 */
int maps_route_create(maps_route_h *route);

/**
 * @brief	Sets the route id.
 * @details This function sets the route id.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[in]	id		The route ID
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_get_route_id()
 */
int maps_route_set_route_id(maps_route_h route, const char *id);

/**
 * @brief	Sets the route origin.
 * @details This function sets the route origin.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[in]	origin		The origin of route
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_get_origin()
 * @see maps_route_set_destination()
 */
int maps_route_set_origin(maps_route_h route,
			  const maps_coordinates_h origin);

/**
 * @brief	Sets the route destination.
 * @details This function sets the route destination.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[in]	destination	The destination of route
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_get_destination()
 * @see maps_route_set_origin()
 */
int maps_route_set_destination(maps_route_h route,
			       const maps_coordinates_h destination);

/**
 * @brief	Sets the route bounding box.
 * @details This function sets the route bounding box.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[in]	bounding_box	The top bounding box
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_get_bounding_box()
 */
int maps_route_set_bounding_box(maps_route_h route,
				const maps_area_h bounding_box);

/**
 * @brief	Sets the route total distance.
 * @details This function sets the route total distance.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[in]	total_distance	The distance of route in meters
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_get_total_distance()
 */
int maps_route_set_total_distance(maps_route_h route,
				  const double total_distance);

/**
 * @brief	Sets the route total duration.
 * @details This function sets the route total duration.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[in]	total_duration	The duration
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_get_total_duration()
 */
int maps_route_set_total_duration(maps_route_h route,
				  const long total_duration);

/**
 * @brief	Sets the route transport mode.
 * @details This function sets the route transport mode.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route	The route handle
 * @param[in]	transport_mode	The transportation mode
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_get_transport_mode()
 * @see #maps_route_transport_mode_e
 */
int maps_route_set_transport_mode(maps_route_h route,
				  const maps_route_transport_mode_e
				  transport_mode);

/**
 * @brief	Sets the route distance units.
 * @details This function sets the route distance units.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]		maneuver	The maneuver handle
 * @param[in]		distance_unit	The distance units
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_get_distance_unit()
 * @see #maps_distance_unit_e
 */
int maps_route_set_distance_unit(maps_route_maneuver_h maneuver,
				 const maps_distance_unit_e distance_unit);

/**
 * @brief	Sets the route path.
 * @details This function sets the route path.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[in]	path		The coordinates list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_foreach_path()
 */
int maps_route_set_path(maps_route_h route, const maps_item_list_h path);

/**
 * @brief	Sets the route segment list.
 * @details This function sets the route segment list.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[in]	segments	The maneuvers list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_foreach_segment()
 */

int maps_route_set_segments(maps_route_h route,
			    const maps_item_list_h segments);

/**
 * @brief	Sets the route properties.
 * @details This function sets the route properties.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]		route		The route handle
 * @param[in]		properties	The table of properties
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @see maps_route_set_total_distance()
 *
 * @pre @a route is created using maps_route_create().
 *
 * @see maps_route_create()
 * @see maps_route_foreach_property()
 */
int maps_route_set_properties(maps_route_h route,
			      const maps_item_hashtable_h properties);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_ROUTE_PLUGIN_H__ */
