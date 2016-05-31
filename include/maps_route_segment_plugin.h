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

#ifndef __MAPS_SEGMENT_PLUGIN_H__
#define __MAPS_SEGMENT_PLUGIN_H__

#include <maps_route_segment.h>
#include <maps_extra_types.h>
/**
 * @ingroup	CAPI_MAPS_PLUGIN_ROUTE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_ROUTE_SEGMENT_MODULE Segment
 *
 * @file maps_route_segment_plugin.h
 * @brief This file contains the functions related to Route Segment
 * information, needed in plug-in development.
 * @addtogroup CAPI_MAPS_PLUGIN_ROUTE_SEGMENT_MODULE
 * @{
 * @brief This provides APIs related to Route Segment information, used
 * in Route Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Creates a new route segment handle.
 * @details This function creates a new route segment handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a segment must be released using maps_route_segment_destroy().
 * \n @a segment may be cloned using maps_route_segment_clone().
 *
 * @param[out]	segment		The segment handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_route_segment_destroy()
 * @see maps_route_segment_clone()
 */
int maps_route_segment_create(maps_route_segment_h *segment);

/**
 * @brief	Sets the route segment origin.
 * @details This function sets the route segment origin.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	segment		The segment handle
 * @param[in]	origin		The origin of segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a segment is created using maps_route_segment_create().
 *
 * @see maps_route_segment_create()
 * @see maps_route_segment_get_origin()
 */
int maps_route_segment_set_origin(maps_route_segment_h segment,
				  const maps_coordinates_h origin);

/**
 * @brief	Sets the route segment destination.
 * @details This function sets the route segment destination.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	segment		The segment handle
 * @param[in]	destination	The destination of segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a segment is created using maps_route_segment_create().
 *
 * @see maps_route_segment_create()
 * @see maps_route_segment_get_destination()
 */
int maps_route_segment_set_destination(maps_route_segment_h segment,
				       const maps_coordinates_h destination);

/**
 * @brief	Sets the route segment bounding box.
 * @details This function sets the route segment bounding box.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	segment		The segment handle
 * @param[in]	bounding_box	The start angle of the segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a segment is created using maps_route_segment_create().
 *
 * @see maps_route_segment_create()
 * @see maps_route_segment_get_bounding_box()
 */
int maps_route_segment_set_bounding_box(maps_route_segment_h segment,
					const maps_area_h bounding_box);

/**
 * @brief	Sets the route segment distance.
 * @details This function sets the route segment distance.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	segment		The segment handle
 * @param[in]	distance	The distance of the segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a segment is created using maps_route_segment_create().
 *
 * @see maps_route_segment_create()
 * @see maps_route_segment_get_distance()
 */
int maps_route_segment_set_distance(maps_route_segment_h segment,
				    const double distance);

/**
 * @brief	Sets the route segment duration.
 * @details This function sets the route segment duration.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	segment		The segment handle
 * @param[in]	duration	The duration of the segment
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a segment is created using maps_route_segment_create().
 *
 * @see maps_route_segment_create()
 * @see maps_route_segment_get_duration()
 */
int maps_route_segment_set_duration(maps_route_segment_h segment,
				    const long duration);

/**
 * @brief	Sets the route segment path.
 * @details This function sets the route segment path.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	segment		The segment handle
 * @param[in]	path		The maneuver list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a segment is created using maps_route_segment_create().
 *
 * @see maps_route_segment_create()
 * @see maps_route_segment_foreach_path()
 */
int maps_route_segment_set_path(maps_route_segment_h segment,
				const maps_item_list_h path);

/**
 * @brief	Sets the route segment maneuver list.
 * @details This function sets the route segment maneuver list.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	segment		The segment handle
 * @param[in]	maneuvers	The maneuver list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a segment is created using maps_route_segment_create().
 *
 * @see maps_route_segment_create()
 * @see maps_route_segment_foreach_maneuver()
 */
int maps_route_segment_set_maneuvers(maps_route_segment_h segment,
				     const maps_item_list_h maneuvers);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_SEGMENT_PLUGIN_H__ */
