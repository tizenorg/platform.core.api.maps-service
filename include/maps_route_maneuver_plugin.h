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

#ifndef __MAPS_MANEUVER_PLUGIN_H__
#define __MAPS_MANEUVER_PLUGIN_H__

#include <maps_route_maneuver.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_ROUTE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_ROUTE_MANEUVER_MODULE Maneuver
 *
 * @file maps_route_maneuver_plugin.h
 * @brief This file contains the functions related to Route Maneuver
 * information, needed in plug-in development.
 * @addtogroup CAPI_MAPS_PLUGIN_ROUTE_MANEUVER_MODULE
 * @{
 * @brief This provides APIs related to Route Maneuver information,
 * used in Route Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Creates a new route maneuver handle.
 * @details This function creates a new route maneuver handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a maneuver must be released using maps_route_maneuver_destroy().
 * \n @a maneuver may be cloned using maps_route_maneuver_clone().
 *
 * @param[out]	maneuver	The maneuver handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_route_maneuver_destroy()
 * @see maps_route_maneuver_clone()
 */
int maps_route_maneuver_create(maps_route_maneuver_h *maneuver);

/**
 * @brief	Sets the route maneuver direction id.
 * @details This function sets the route maneuver direction id.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[in]	direction_id	The direction ID
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a maneuver is created using maps_route_maneuver_create().
 *
 * @see maps_route_maneuver_create()
 * @see maps_route_maneuver_get_direction_id()
 * @see #maps_route_direction_e
 */
int maps_route_maneuver_set_direction_id(maps_route_maneuver_h maneuver,
					 const maps_route_direction_e
					 direction_id);

/**
 * @brief	Sets the route maneuver turn type.
 * @details This function sets the route maneuver turn type.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[in]	turn_type	The turn type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a maneuver is created using maps_route_maneuver_create().
 *
 * @see maps_route_maneuver_create()
 * @see maps_route_maneuver_get_turn_type()
 * @see #maps_route_turn_type_e
 */
int maps_route_maneuver_set_turn_type(maps_route_maneuver_h maneuver,
				      const maps_route_turn_type_e turn_type);

/**
 * @brief	Sets the route maneuver road name.
 * @details This function sets the route maneuver road name.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[in]	road_name	The current road name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a maneuver is created using maps_route_maneuver_create().
 *
 * @see maps_route_maneuver_create()
 * @see maps_route_maneuver_get_road_name()
 */
int maps_route_maneuver_set_road_name(maps_route_maneuver_h maneuver,
				      const char *road_name);

/**
 * @brief	Sets the route maneuver instruction text.
 * @details This function sets the route maneuver instruction text.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	maneuver		The maneuver handle
 * @param[in]	instruction_text	The current road name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a maneuver is created using maps_route_maneuver_create().
 *
 * @see maps_route_maneuver_create()
 * @see maps_route_maneuver_get_instruction_text()
 */
int maps_route_maneuver_set_instruction_text(maps_route_maneuver_h maneuver,
					     const char *instruction_text);

/**
 * @brief	Sets the route maneuver locale.
 * @details This function sets the route maneuver locale.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[in]	locale		The languages of instruction text and street
 * name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a maneuver is created using maps_route_maneuver_create().
 *
 * @see maps_route_maneuver_create()
 * @see maps_route_maneuver_get_locale()
 */
int maps_route_maneuver_set_locale(maps_route_maneuver_h maneuver,
				   const char *locale);

/**
 * @brief	Sets the route maneuver position.
 * @details This function sets the route maneuver position.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[in]	position	The geographical position of maneuver
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a maneuver is created using maps_route_maneuver_create().
 *
 * @see maps_route_maneuver_create()
 * @see maps_route_maneuver_get_position()
 */
int maps_route_maneuver_set_position(maps_route_maneuver_h maneuver,
				     const maps_coordinates_h position);

/**
 * @brief	Sets the time to next instruction in route maneuver.
 * @details This function sets the time to next instruction in route maneuver.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	maneuver			The maneuver handle
 * @param[in]	time_to_next_instruction	The time to next instruction
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a maneuver is created using maps_route_maneuver_create().
 *
 * @see maps_route_maneuver_create()
 * @see maps_route_maneuver_get_time_to_next_instruction()
 */
int maps_route_maneuver_set_time_to_next_instruction(maps_route_maneuver_h
						     maneuver,
					const int time_to_next_instruction);

/**
 * @brief	Sets the distance to next instruction in route maneuver.
 * @details This function sets the distance to next instruction in route
 * maneuver.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	maneuver			The maneuver handle
 * @param[in]	distance_to_next_instruction	The distance from the current
 * maneuver to the next maneuver
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a maneuver is created using maps_route_maneuver_create().
 *
 * @see maps_route_maneuver_create()
 * @see maps_route_maneuver_get_distance_to_next_instruction()
 */
int maps_route_maneuver_set_distance_to_next_instruction(maps_route_maneuver_h
							 maneuver,
				const double distance_to_next_instruction);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_MANEUVER_PLUGIN_H__ */
