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

#ifndef __MAPS_ROUTE_H__
#define __MAPS_ROUTE_H__

#include <tizen_type.h>
#include <maps_area.h>
#include <maps_route_segment.h>
#include <maps_route_maneuver.h>
#include <maps_preference.h>

/**
 * @ingroup	CAPI_MAPS_ROUTE_MODULE
 * @defgroup	CAPI_MAPS_ROUTE_DATA_MODULE Route
 *
 * @file maps_route.h
 * @brief This file contains the functions related to Route information.
 *
 * @addtogroup CAPI_MAPS_ROUTE_DATA_MODULE
 * @{
 * @brief This provides APIs related to Route information, used in Route Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Route handle
 * @details The handle of Route instance.
 * @remarks To release the handle use maps_route_destroy().
 * \n To clone the handle use maps_route_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @see maps_route_destroy()
 * @see maps_route_clone()
 */
typedef void *maps_route_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Called when requesting the path of the Route.
 * @details This callback is invoked while iterating through the list of
 * coordinates, composing the Route.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a coordinates must be released using maps_coordinates_destroy().
 * \n To use @a coordinates outside this function, clone it with
 * maps_coordinates_clone().
 *
 * @param[in]	index		The current index of path point
 * @param[in]	total		The total amount of path points
 * @param[in]	coordinates	The coordinates
 * @param[in]	user_data	The user data passed from
 * maps_route_foreach_path()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_route_foreach_path() will invoke this callback.
 *
 * @see maps_route_foreach_path()
 * @see #maps_coordinates_h
 */
typedef bool(*maps_route_path_cb) (int index, int total,
				   maps_coordinates_h coordinates,
				   void *user_data);

/**
 * @brief	Called when requesting the segments of the Route.
 * @details This callback is invoked while iterating through the list of
 * segments, composing the Route.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a segment is valid only in this function and must be released using
 * maps_route_segment_destroy().
 * \n To use @a segment outside this function, clone it with
 * maps_route_segment_clone().
 *
 * @param[in]	index		The current index of segment
 * @param[in]	total		The total amount of segments
 * @param[in]	segment		The segment or leg of route
 * @param[in]	user_data	The user data passed from
 * maps_route_foreach_segment()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_route_foreach_segment() will invoke this callback.
 *
 * @see maps_route_foreach_segment()
 * @see #maps_route_segment_h
 */
typedef bool(*maps_route_segment_cb) (int index, int total,
				      maps_route_segment_h segment,
				      void *user_data);

/**
 * @brief	Called when requesting the list of Route Properties.
 * @details This callback is invoked while iterating through the list of Route
 * Properties.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a key and @a value must be released using free() and corresponding
 * release method for property value correspondingly.
 * \n To use @a key and @a value outside this function, you should clone it.
 *
 * @param[in]	index		The current index of property
 * @param[in]	total		The total amount of properties
 * @param[in]	key		The key of property
 * @param[in]	value		The value of property
 * @param[in]	user_data	The user data passed from
 * maps_route_foreach_property()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_route_foreach_property() will invoke this callback.
 *
 * @see maps_route_foreach_property()
 */
typedef bool(*maps_route_properties_cb) (int index, int total, char *key,
					 void *value, void *user_data);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the route handle and releases all its resources.
 * @details This function destroys the route handle and releases all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_route_clone()
 */
int maps_route_destroy(maps_route_h route);

/**
 * @brief	Clones the route handle.
 * @details This function clones the route handle @a origin and all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a cloned must be released using maps_route_destroy().
 *
 * @param[in]	origin		The original route handle
 * @param[out]	cloned		A cloned route handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see maps_route_destroy()
 */
int maps_route_clone(const maps_route_h origin, maps_route_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the route id.
 * @details This function gets the route id.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a id must be released using free().
 *
 * @param[in]	route		The route handle
 * @param[out]	route_id	The id of route
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_route_get_route_id(const maps_route_h route, char **route_id);

/**
 * @brief	Gets the route origin.
 * @details This function gets the route origin.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a origin must be released using maps_coordinates_destroy().
 *
 * @param[in]	route		The route handle
 * @param[out]	origin		The origin of route
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_route_get_destination()
 * @see #maps_coordinates_h
 */
int maps_route_get_origin(const maps_route_h route,
			  maps_coordinates_h *origin);

/**
 * @brief	Gets the route destination.
 * @details This function gets the route destination.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a destination must be released using maps_coordinates_destroy().
 *
 * @param[in]	route		The route handle
 * @param[out]	destination	The destination of route
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_route_get_origin()
 * @see #maps_coordinates_h
 */
int maps_route_get_destination(const maps_route_h route,
			       maps_coordinates_h *destination);

/**
 * @brief	Gets the route bounding box.
 * @details This function gets the route bounding box.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a bounding_box must be released using maps_area_destroy().
 *
 * @param[in]	route			The route handle
 * @param[out]	bounding_box		The bounding box of route
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_area_h
 */
int maps_route_get_bounding_box(const maps_route_h route,
				maps_area_h *bounding_box);

/**
 * @brief	Gets the route transport mode.
 * @details This function gets the route transport mode.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[out]	transport_mode	The transport mode
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_route_transport_mode_e
 */
int maps_route_get_transport_mode(const maps_route_h route,
				  maps_route_transport_mode_e *transport_mode);

/**
 * @brief	Gets the route total distance.
 * @details This function gets the route total distance.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[out]	total_distance	The distance of route. You can get the distance
 * unit by route_get_distance_unit()
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_route_get_total_duration()
 */
int maps_route_get_total_distance(const maps_route_h route,
				  double *total_distance);

/**
 * @brief	Gets the route total duration.
 * @details This function gets the route total duration.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	route		The route handle
 * @param[out]	total_duration	The duration
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_route_get_total_distance()
 */
int maps_route_get_total_duration(const maps_route_h route,
				  long *total_duration);

/**
 * @brief	Gets the route distance units.
 * @details This function gets the route distance units.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]		route	The route handle
 * @param[out]		distance_unit	The distance units
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_distance_unit_e
 */
int maps_route_get_distance_unit(const maps_route_h route,
				 maps_distance_unit_e *distance_unit);

/**
 * @brief	Retrieves all properties.
 * @details This function retrieves all route properties.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The properties will be delivered via maps_route_properties_cb().
 *
 * @param[in]	route		The route handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @post This function invokes maps_route_properties_cb() repeatedly to retrieve
 * each property.
 *
 * @see maps_route_properties_cb()
 */
int maps_route_foreach_property(const maps_route_h route,
				maps_route_properties_cb callback,
				void *user_data);

/**
 * @brief	Retrieves all coordinates of the path.
 * @details This function retrieves all coordinates of the route path.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The coordinates will be delivered via maps_route_path_cb().
 *
 * @param[in]	route		The route handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_route_path_cb() repeatedly to retrieve each
 * coordinate point.
 *
 * @see maps_route_path_cb()
 */
int maps_route_foreach_path(const maps_route_h route,
			    maps_route_path_cb callback, void *user_data);

/**
 * @brief	Retrieves all segments of the route.
 * @details This function retrieves all segments of the route.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks The segments will be delivered via maps_route_path_cb().
 *
 * @param[in]	route		The route handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_route_segment_cb() repeatedly to retrieve
 * each segment.
 *
 * @see maps_route_segment_cb()
 */
int maps_route_foreach_segment(const maps_route_h route,
			       maps_route_segment_cb callback,
			       void *user_data);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_ROUTE_H__ */
