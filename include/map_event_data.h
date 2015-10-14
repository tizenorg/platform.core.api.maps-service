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


#ifndef __MAP_EVENT_DATA_H__
#define __MAP_EVENT_DATA_H__

/**
 *
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_EVENT_MODULE Map View Event Data
 *
 * @file map_event_data.h
 * @brief This file contains the functions related to Maps View Event Data
 * information.
 *
 * @addtogroup CAPI_MAP_EVENT_MODULE
 * @{
 * @brief This provides APIs related to operations with Maps View Event Data
 */


#include <map_object.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	The Maps View Event data handle
 * @details The handle of Maps View Event Data instance.
 * @remarks To release the handle use map_event_data_destroy().
 * \n To clone the handle use map_event_data_clone().
 * @since_tizen 3.0
 *
 * @see map_event_data_destroy()
 * @see map_event_data_clone()
 */
typedef void *map_event_data_h;


/**
 * @brief	Enumerations of user gestures over Map View
 * @since_tizen 3.0
 */
typedef enum _map_gesture_e {
	/** Indicates the pan user gesture */
	MAP_GESTURE_PAN,

	/** Indicates the flick user gesture */
	MAP_GESTURE_FLICK,

	/** Indicates the pinch user gesture */
	MAP_GESTURE_PINCH,

	/** Indicates the tap user gesture */
	MAP_GESTURE_TAP,

	/** Indicates the double tap user gesture */
	MAP_GESTURE_DOUBLE_TAP,

	/** Indicates the two-finger tap user gesture */
	MAP_GESTURE_2_FINGER_TAP,

	/** Indicates the zoom user gesture */
	MAP_GESTURE_ZOOM,

	/** Indicates the rotate user gesture */
	MAP_GESTURE_ROTATE,

	/** Indicates the long press user gesture */
	MAP_GESTURE_PRESS,

	/** Indicates the empty gesture */
	MAP_GESTURE_NONE
} map_gesture_e;

/**
 * @brief	Enumerations of map panel actions
 * @since_tizen 3.0
 */
typedef enum _map_action_e {
	/** Indicates the show location action */
	MAP_ACTION_SET_CENTER,

	/** Indicates the move center action */
	MAP_ACTION_MOVE_CENTER,

	/** Indicates the zoom action */
	MAP_ACTION_ZOOM,

	/** Indicates the rotation action */
	MAP_ACTION_ROTATE,

	/** Indicates the tilt action */
	MAP_ACTION_TILT,

	/** Indicates the empty action */
	MAP_ACTION_NONE,
} map_action_e;

/**
 * @brief	Enumerations of map panel event types
 * @since_tizen 3.0
 */
typedef enum _map_event_type_e {
	/** Indicates the gesture event callback */
	MAP_EVENT_GESTURE,

	/** Indicates the action callback */
	MAP_EVENT_ACTION,

	/** Indicates the object event callback */
	MAP_EVENT_OBJECT,

	/** Indicates the map view readiness callback */
	MAP_EVENT_READY
} map_event_type_e;


/**
 * @brief	Destroys the event data handle.
 * @details This function destroys the event data handle and releases all its
 * resources.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 *
 * @see map_event_data_clone()
 */
int map_event_data_destroy(map_event_data_h event);

/**
 * @brief	Clones the event handle.
 * @details This function clones the event handle @a origin and all its
 * resources.
 * \n Event data handle @a origin may be created using maps_view_event_data_create().
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_view_event_data_destroy().
 *
 * @param[in]	origin		The original event handle
 * @param[out]	cloned		A cloned event handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is created using maps_view_event_data_create().
 *
 * @see maps_view_event_data_create()
 * @see maps_view_event_data_destroy()
 */
int map_event_data_clone(const map_event_data_h origin,
			      map_event_data_h *cloned);


/**
 * @brief	Gets the event type.
 * @details This function gets the event type.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	event_type	The type of the event, one of values, enumerated
 * in #map_event_type_e
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #map_event_type_e
 */
int map_event_data_get_type(const map_event_data_h event,
				 map_event_type_e *event_type);
/**
 * @brief	Gets the event gesture type.
 * @details This function gets the event gesture type if the event type is
 * #MAP_EVENT_GESTURE or returns the #MAPS_ERROR_NOT_SUPPORTED instead
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	gesture_type	The type of the gesture, one of values,
 * enumerated in #map_gesture_e
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a gesture_type value is not
 * supported
 *
 * @see #map_gesture_e
 */
int map_event_data_get_gesture_type(map_event_data_h event,
					 map_gesture_e *gesture_type);
/**
 * @brief	Gets the event action type.
 * @details This function gets the event action type if the event type is
 * #MAP_EVENT_ACTION or returns the #MAPS_ERROR_NOT_SUPPORTED instead
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	action_type		The type of the action, one of values,
 * enumerated in #map_action_e
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a action_type value is not
 * supported
 *
 * @see #map_action_e
 */
int map_event_data_get_action_type(map_event_data_h event,
					map_action_e *action_type);

/**
 * @brief	Gets the new map center coordinates.
 * @details This function gets the new map center coordinates if the event type
 * is #MAP_EVENT_ACTION or returns the #MAPS_ERROR_NOT_SUPPORTED instead
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	center		The new center coordinates
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a x and @a y values is not supported
 */
int map_event_data_get_center(map_event_data_h event,
				   maps_coordinates_h *center);

/**
 * @brief	Gets the new map center movement coordinates delta.
 * @details This function gets the screen coordinates delta of  map center
 * movement if the event type is #MAP_EVENT_ACTION or returns the
 * #MAPS_ERROR_NOT_SUPPORTED instead
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	delta_x		The delta x
 * @param[out]	delta_y		The delta y
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a x and @a y values is not supported
 */
int map_event_data_get_delta(map_event_data_h event,
				  int *delta_x, int *delta_y);

/**
 * @brief	Gets the event screen coordinates.
 * @details This function gets the event screen coordinates if the event type is
 * #MAP_EVENT_GESTURE or returns the #MAPS_ERROR_NOT_SUPPORTED instead
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	x		The x coordinate of the event
 * @param[out]	y		The y coordinate of the event
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a x and @a y values is not
 * supported
 */
int map_event_data_get_xy(map_event_data_h event, int *x, int* y);

/**
 * @brief	Gets the number of fingers, detected in the event.
 * @details This function gets the number of fingers, detected in the event,
 * when the event type is #MAP_EVENT_GESTURE or returns the
 * #MAPS_ERROR_NOT_SUPPORTED instead
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	fingers		The number of fingers
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a fingers value is not supported
 */
int map_event_data_get_fingers(map_event_data_h event, int *fingers);

/**
 * @brief	Gets zoom factor of zoom gesture or action.
 * @details When the event type is #MAP_EVENT_GESTURE or
 * #MAP_EVENT_ACTION this function gets the corresponding zoom factor.
 * Otherwise it returns #MAPS_ERROR_NOT_SUPPORTED.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	zoom_factor	The zoom factor
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a zoom_factor value is not
 * supported
 */
int map_event_data_get_zoom_factor(map_event_data_h event,
					double *zoom_factor);

/**
 * @brief	Gets rotation angle of rotation gesture or action.
 * @details When the event type is #MAP_EVENT_GESTURE or
 * #MAP_EVENT_ACTION this function gets the corresponding rotation angle.
 * Otherwise it returns #MAPS_ERROR_NOT_SUPPORTED.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	rotation_angle	The rotation angle
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a rotation_angle value is not
 * supported
 */
int map_event_data_get_rotation_angle(map_event_data_h event,
					   double *rotation_angle);

/**
 * @brief	Gets tilt of action.
 * @details When the event type is  #MAP_EVENT_ACTION this function gets
 * the corresponding tilt.
 * Otherwise it returns #MAPS_ERROR_NOT_SUPPORTED.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	tilt		The tilt
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a tilt value is not supported
 */
int map_event_data_get_tilt(map_event_data_h event, double *tilt);

/**
 * @brief	Gets object of event.
 * @details When the event type is  #MAP_EVENT_OBJECT this function gets
 * the corresponding object handle.
 * Otherwise it returns #MAPS_ERROR_NOT_SUPPORTED.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	object		The object
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The @a object value is not supported
 * @see #map_object_h
 */
int map_event_data_get_object(map_event_data_h event,
				   map_object_h *object);


#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_EVENT_DATA_H__*/
