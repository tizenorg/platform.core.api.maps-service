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

#ifndef __MAPS_MANEUVER_H__
#define __MAPS_MANEUVER_H__

#include <maps_coordinates.h>

/**
 * @ingroup	CAPI_MAPS_ROUTE_MODULE
 * @defgroup	CAPI_MAPS_ROUTE_MANEUVER_MODULE Maneuver
 *
 * @file maps_route_maneuver.h
 * @brief This file contains the functions related to Route Maneuver
 * information.
 * @addtogroup CAPI_MAPS_ROUTE_MANEUVER_MODULE
 * @{
 * @brief This provides APIs related to Place Maneuver information, used in
 * Route Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Route Maneuver handle
 * @details The handle of Route Maneuver instance.
 * @remarks To release the handle use maps_route_maneuver_destroy().
 * \n To clone the handle use maps_route_maneuver_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see maps_route_maneuver_destroy()
 * @see maps_route_maneuver_clone()
 */
typedef void *maps_route_maneuver_h;

/**
 * @brief The enumeration of direction type.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum {
	MAPS_ROUTE_DIRECTION_NONE,		/**< Indicates unknown direction */
	MAPS_ROUTE_DIRECTION_NORTH,		/**< Indicates north direction */
	MAPS_ROUTE_DIRECTION_NORTHWEST,	/**< Indicates north-west direction */
	MAPS_ROUTE_DIRECTION_NORTHEAST,	/**< Indicates north-east direction */
	MAPS_ROUTE_DIRECTION_SOUTH,		/**< Indicates south direction */
	MAPS_ROUTE_DIRECTION_SOUTHEAST,	/**< Indicates south-east direction */
	MAPS_ROUTE_DIRECTION_SOUTHWEST,	/**< Indicates south-west direction */
	MAPS_ROUTE_DIRECTION_WEST,		/**< Indicates west direction */
	MAPS_ROUTE_DIRECTION_EAST		/**< Indicates east direction */
} maps_route_direction_e;

/**
 * @brief The enumeration of turn type.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum {
	MAPS_ROUTE_TURN_TYPE_NONE,			/**< Indicates unknown instruction. */
	MAPS_ROUTE_TURN_TYPE_STRAIGHT,		/**< Indicates instruction to move straight */
	MAPS_ROUTE_TURN_TYPE_BEAR_RIGHT,	/**< Indicates instruction to bear right. */
	MAPS_ROUTE_TURN_TYPE_LIGHT_RIGHT,	/**< Indicates instruction slightly to the right. */
	MAPS_ROUTE_TURN_TYPE_RIGHT,			/**< Indicates instruction to turn right. */
	MAPS_ROUTE_TURN_TYPE_HARD_RIGHT,	/**< Indicates instruction to turn hard to the right. */
	MAPS_ROUTE_TURN_TYPE_UTURN_RIGHT,	/**< Indicates instruction to u-turn to the right. */
	MAPS_ROUTE_TURN_TYPE_UTURN_LEFT,	/**< Indicates instruction to u-turn to the left. */
	MAPS_ROUTE_TURN_TYPE_HARD_LEFT,		/**< Indicates instruction to turn hard to the left. */
	MAPS_ROUTE_TURN_TYPE_LEFT,			/**< Indicates instruction to turn left. */
	MAPS_ROUTE_TURN_TYPE_LIGHT_LEFT,	/**< Indicates instruction to turn lightly to the left. */
	MAPS_ROUTE_TURN_TYPE_BEAR_LEFT,		/**< Indicates instruction to bear left. */
	MAPS_ROUTE_TURN_TYPE_RIGHT_FORK,	/**< Indicates right fork instruction. */
	MAPS_ROUTE_TURN_TYPE_LEFT_FORK,		/**< Indicates left fork instruction. */
	MAPS_ROUTE_TURN_TYPE_STRAIGHT_FORK	/**< Indicates straight fork instruction. */
} maps_route_turn_type_e;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the route maneuver handle and releases all its
 * resources.
 * @details This function destroys the route maneuver handle and releases all
 * its resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	maneuver	The maneuver handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see maps_route_maneuver_clone()
 */
int maps_route_maneuver_destroy(maps_route_maneuver_h maneuver);

/**
 * @brief	Clones the route maneuver handle.
 * @details This function clones the route maneuver handle @a origin and all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a cloned must be released using maps_route_maneuver_destroy().
 *
 * @param[in]	origin		The original maneuver handle
 * @param[out]	cloned		A cloned maneuver handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY		Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see maps_route_maneuver_destroy()
 */
int maps_route_maneuver_clone(const maps_route_maneuver_h origin,
			      maps_route_maneuver_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the route maneuver direction id.
 * @details This function gets the route maneuver direction id.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[out]	direction_id	The direction ID
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see #maps_route_direction_e
 */
int maps_route_maneuver_get_direction_id(const maps_route_maneuver_h maneuver,
					 maps_route_direction_e *direction_id);

/**
 * @brief	Gets the route maneuver turn type.
 * @details This function gets the route maneuver turn type.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[out]	turn_type	The turn type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see #maps_route_turn_type_e
 */
int maps_route_maneuver_get_turn_type(const maps_route_maneuver_h maneuver,
				      maps_route_turn_type_e *turn_type);

/**
 * @brief	Gets the route maneuver position.
 * @details This function gets the route maneuver position.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a position must be released using maps_coordinates_destroy().
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[out]	position	The start position of the maneuver
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see #maps_coordinates_h
 */
int maps_route_maneuver_get_position(const maps_route_maneuver_h maneuver,
				     maps_coordinates_h *position);

/**
 * @brief	Gets the road name of the route maneuver.
 * @details This function gets the road name of the route maneuver.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a road_name must be released using free().
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[out]	road_name	The current road name of the maneuver handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_route_maneuver_get_road_name(const maps_route_maneuver_h maneuver,
				      char **road_name);

/**
 *
 * @brief	Gets the instruction text (narrative) of the route maneuver.
 * @details This function gets the instruction text (narrative) of the route
 * maneuver.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a instruction_text must be released using free().
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[in]	instruction_text	The instruction text
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_route_maneuver_get_instruction_text(const maps_route_maneuver_h
					     maneuver,
					     char **instruction_text);

/**
 * @brief	Gets the route maneuver locale.
 * @details This function gets the route maneuver locale.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a locale must be released using free().
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[out]	locale		The languages of instruction text and street
 * name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_route_maneuver_get_locale(const maps_route_maneuver_h maneuver,
				   char **locale);

/**
 * @brief	Gets the time to next instruction of the route maneuver.
 * @details This function gets the time to next instruction of the route
 * maneuver.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[out]	time_to_next_instruction	The time to next instruction on the
 * given maneuver
 * handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_route_maneuver_get_time_to_next_instruction(const maps_route_maneuver_h
						     maneuver,
						int *time_to_next_instruction);

/**
 * @brief	Gets the distance to next instruction of the route maneuver.
 * @details This function gets the distance to next instruction of the route
 * maneuver.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	maneuver	The maneuver handle
 * @param[out]	distance_to_next_instruction	The distance from the current
 * maneuver to the next maneuver
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_route_maneuver_get_distance_to_next_instruction(const
							 maps_route_maneuver_h
							 maneuver,
					double *distance_to_next_instruction);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_MANEUVER_H__ */
