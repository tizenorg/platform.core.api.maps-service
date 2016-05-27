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

#ifndef __MAPS_SEGMENT_H__
#define __MAPS_SEGMENT_H__

#include <tizen_type.h>
#include <maps_coordinates.h>
#include <maps_route_maneuver.h>
#include <maps_area.h>

/**
 * @ingroup	CAPI_MAPS_ROUTE_MODULE
 * @defgroup	CAPI_MAPS_ROUTE_SEGMENT_MODULE Segment
 *
 * @file maps_route_segment.h
 * @brief This file contains the functions related to Route Segment information.
 * @addtogroup CAPI_MAPS_ROUTE_SEGMENT_MODULE
 * @{
 * @brief This provides APIs related to Place Segment information, used in Route
 * Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Route Segment handle
 * @details The handle of Route Segment instance.
 * @remarks To release the handle use maps_route_segment_destroy().
 * \n To clone the handle use maps_route_segment_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see maps_route_segment_destroy()
 * @see maps_route_segment_clone()
 */
typedef void *maps_route_segment_h;

/**
 * @brief	Called when requesting the path of the Route Segment.
 * @details This callback is invoked while iterating through the list of
 * coordinates, composing the Route Segment.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a coordinates must be released using maps_coordinates_destroy().
 * \n To use @a coordinates outside this function, clone it with
 * maps_coordinates_clone().
 *
 * @param[in]	segment		The segment of route
 * @param[in]	user_data	The user data passed from
 * maps_route_segment_foreach_path()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_route_segment_foreach_path() will invoke this callback.
 *
 * @see maps_route_segment_foreach_path()
 * @see #maps_coordinates_h
 */
typedef bool(*maps_route_segment_path_cb) (int index, int total,
					   maps_coordinates_h coordinates,
					   void *user_data);

/**
 * @brief	Called when requesting the maneuvers of the Route Segment.
 * @details This callback is invoked while iterating through the list of
 * maneuvers, composing the Route Segment.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a maneuver is valid only in this function and must be released
 * using maps_route_maneuver_destroy().
 * \n To use @a maneuver outside this function, clone it with
 * maps_route_maneuver_clone().
 *
 * @param[in]	maneuver	The maneuver of route
 * @param[in]	user_data	The user data passed from
 * maps_route_segment_foreach_maneuver()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_route_segment_foreach_maneuver() will invoke this callback.
 *
 * @see maps_route_segment_foreach_maneuver()
 * @see #maps_route_maneuver_h
 */
typedef bool(*maps_route_segment_maneuver_cb) (int index, int total,
					       maps_route_maneuver_h maneuver,
					       void *user_data);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the route segment handle and releases all its resources.
 * @details This function destroys the route segment handle and releases all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	segment		The segment handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @see maps_route_segment_clone()
 */
int maps_route_segment_destroy(maps_route_segment_h segment);

/**
 * @brief	Clones the route segment handle.
 * @details This function clones the route handle @a origin and all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a cloned must be released using maps_route_segment_destroy().
 *
 * @param[in]	origin		The original segment handle
 * @param[out]	cloned		A cloned segment handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_route_segment_destroy()
 */
int maps_route_segment_clone(const maps_route_segment_h origin,
			     maps_route_segment_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the route segment origin.
 * @details This function gets the route segment origin.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a origin must be released using maps_coordinates_destroy().
 *
 * @param[in]	segment		The segment handle
 * @param[out]	origin		The origin of segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_coordinates_h
 */
int maps_route_segment_get_origin(const maps_route_segment_h segment,
				  maps_coordinates_h *origin);

/**
 * @brief	Gets the route segment destination.
 * @details This function gets the route segment destination.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a destination must be released using maps_coordinates_destroy().
 *
 * @param[in]	segment		The segment handle
 * @param[out]	destination	The destination of segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_coordinates_h
 */
int maps_route_segment_get_destination(const maps_route_segment_h segment,
				       maps_coordinates_h *destination);

/**
 * @brief	Gets the route segment bounding box.
 * @details This function gets the route segment bounding box.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a bounding_box must be released using maps_area_destroy().
 *
 * @param[in]	segment		The segment handle
 * @param[out]	bounding_box	The bounding box of segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_area_h
 */
int maps_route_segment_get_bounding_box(const maps_route_segment_h segment,
					maps_area_h *bounding_box);

/**
 * @brief	Gets the route segment distance.
 * @details This function gets the route segment distance.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	segment		The segment handle
 * @param[out]	distance	The distance of segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_route_segment_get_distance(const maps_route_segment_h segment,
				    double *distance);

/**
 * @brief	Gets the route segment duration.
 * @details This function gets the route segment duration.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	segment		The segment handle
 * @param[out]	duration	The distance of segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_route_segment_get_duration(const maps_route_segment_h segment,
				    long *duration);

/**
 * @brief	Retrieves all coordinates of the path.
 * @details This function retrieves all coordinates of the route segment path.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks The coordinates will be delivered via maps_route_segment_path_cb().
 *
 * @param[in]	segment		The segment handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_route_segment_path_cb() repeatedly to
 * retrieve each coordinate point.
 *
 * @see maps_route_segment_path_cb()
 */
int maps_route_segment_foreach_path(const maps_route_segment_h segment,
				    maps_route_segment_path_cb callback,
				    void *user_data);

/**
 * @brief	Retrieves all maneuvers of the path.
 * @details This function retrieves all maneuvers of the route segment path.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks The coordinates will be delivered via
 * maps_route_segment_maneuver_cb().
 *
 * @param[in]	segment		The segment handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_route_segment_maneuver_cb() repeatedly to
 * retrieve each coordinate point.
 *
 * @see maps_route_segment_maneuver_cb()
 */
int maps_route_segment_foreach_maneuver(const maps_route_segment_h segment,
					maps_route_segment_maneuver_cb
					callback, void *user_data);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_SEGMENT_H__ */
