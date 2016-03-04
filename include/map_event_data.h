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
 * @brief This file contains the functions related to Maps View Event Data information.
 *
 * @addtogroup CAPI_MAP_EVENT_MODULE
 * @{
 * @brief This provides APIs related to operations with Maps View Event Data
 * @details Supported Event types are following:
 *  * User gesture over the Map
 *  * Map action, such as zooming or rotation
 *  * Object Event, such as pressing or moving
 *  * View readiness
 *  .
 * @par Features available by Event Type
 * The correspondence between the type of event and available event data
 * is following:
 * | Feature/Event Type                  | Gesture | Action | Object | Ready |
 * | :---------------------------------- | :-----: | :----: | :----: | :---: |
 * | #map_event_type_e                   | +       | +      | +      | +     |
 * | #map_gesture_e                      | +       |        | +      |       |
 * | #map_action_e                       |         | +      |        |       |
 * | map_event_data_destroy()            | +       | +      | +      | +     |
 * | map_event_data_clone()              | +       | +      | +      | +     |
 * | map_event_data_get_type()           | +       | +      | +      | +     |
 * | map_event_data_get_gesture_type()   | +       |        | +      |       |
 * | map_event_data_get_action_type()    |         | +      |        |       |
 * | map_event_data_get_center()         |         | +      |        |       |
 * | map_event_data_get_delta()          |         | +      |        |       |
 * | map_event_data_get_position()       | +       |        | +      |       |
 * | map_event_data_get_fingers()        | +       |        | +      |       |
 * | map_event_data_get_zoom_factor()    | +       | +      |        |       |
 * | map_event_data_get_rotation_angle() | +       | +      |        |       |
 * | map_event_data_get_object()         |         |        | +      |       |
 *
 * Note: the event of type #MAP_EVENT_READY has no additional data besides the
 * type.
 *

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
 * @brief	Enumerations of map view actions
 * @details This is an enumeration of actions map view is capable to perform.
 * @since_tizen 3.0
 * @remarks The view performs actions in response on user gestures, such as tap,
 * zoom, long press, or APIs, such as set center or change zoom level.
 * \n To re-assign view actions to user gestures use map_view_set_gesture_action().
 *
 * @see #map_gesture_e
 */
typedef enum _map_action_e {
	MAP_ACTION_NONE,            /** Indicates the empty action */
	MAP_ACTION_SCROLL,          /** Indicates the set center action */
	MAP_ACTION_ZOOM,            /** Indicates the zoom action */
	MAP_ACTION_ZOOM_IN,         /** Indicates the zoom-in action */
	MAP_ACTION_ZOOM_OUT,        /** Indicates the zoom-out action */
	MAP_ACTION_ZOOM_AND_SCROLL, /** Indicates the zoom and scrolling action */
	MAP_ACTION_ZOOM_AND_ROTATE, /** Indicates the zoom and rotation action */
	MAP_ACTION_ROTATE,          /** Indicates the rotation action */
} map_action_e;

/**
 * @brief	Enumeration of user gestures over map view
 * @since_tizen 3.0
 */
typedef enum _map_gesture_e {
	MAP_GESTURE_NONE,               /**< Indicates the empty gesture */
	MAP_GESTURE_SCROLL,             /**< Indicates the move map user gesture */
	MAP_GESTURE_ZOOM,               /**< Indicates the zoom user gesture */
	MAP_GESTURE_TAP,                /**< Indicates the tap user gesture */
	MAP_GESTURE_DOUBLE_TAP,         /**< Indicates the double tap user gesture */
	MAP_GESTURE_2_FINGER_TAP,       /**< Indicates the two-finger tap user gesture */
	MAP_GESTURE_SINGLE_FINGER_ZOOM, /**< Indicates the single finger zoom user gesture*/
	MAP_GESTURE_LONG_PRESS,         /**< Indicates the long press user gesture */
} map_gesture_e;

/**
 * @brief	Enumerations of map event types
 * @since_tizen 3.0
 */
typedef enum _map_event_type_e {
	MAP_EVENT_GESTURE, /** Indicates the gesture event callback */
	MAP_EVENT_ACTION,  /** Indicates the action callback */
	MAP_EVENT_OBJECT,  /** Indicates the object event callback */
	MAP_EVENT_READY    /** Indicates the map view readiness callback */
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
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_data_h
 * @see map_event_data_clone()
 */
int map_event_data_destroy(const map_event_data_h event);

/**
 * @brief	Clones the event handle.
 * @details This function clones the event handle @a origin and all its
 * resources.
 * @since_tizen 3.0
 * @remarks @a cloned must be released using map_event_data_destroy().
 *
 * @param[in]	origin		The original event handle
 * @param[out]	cloned		A cloned event handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_data_h
 * @see map_event_data_destroy()
 * @see map_view_on_event_cb()
 */
int map_event_data_clone(const map_event_data_h origin, map_event_data_h *cloned);

/**
 * @brief	Gets the event type.
 * @details This function gets the event type.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	event_type	The pointer to the #map_event_type_e in which
 * to store the event type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_event_data_destroy()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_type(const map_event_data_h event, map_event_type_e *event_type);

/**
 * @brief	Gets the event gesture type.
 * @details This function gets the event gesture type if the event type is #MAP_EVENT_GESTURE.
 * Otherwise it returns #MAPS_ERROR_INVALID_PARAMETER.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	gesture_type	The pointer to the #map_gesture_e in which to
 * store the gesture type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_gesture_e
 * @see #map_event_data_h
 * @see map_event_data_get_type()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_gesture_type(const map_event_data_h event, map_gesture_e *gesture_type);

/**
 * @brief	Gets the event action type.
 * @details This function gets the event action type if the event type is #MAP_EVENT_ACTION.
 * Otherwise it returns #MAPS_ERROR_INVALID_PARAMETER.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	action_type	The pointer to the #map_action_e in which to
 * store the action type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_action_e
 * @see #map_event_data_h
 * @see map_event_data_get_type()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_action_type(const map_event_data_h event, map_action_e *action_type);

/**
 * @brief	Gets the new map center coordinates.
 * @details This function gets the new map center coordinates if the event type is
 * #MAP_EVENT_ACTION.
 * Otherwise it returns #MAPS_ERROR_INVALID_PARAMETER.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	center		The pointer to the #maps_coordinates_h in which
 * to store the new center coordinates
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see #maps_coordinates_h
 * @see map_event_data_get_type()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_center(const map_event_data_h event, maps_coordinates_h *center);

/**
 * @brief	Gets the new map center movement coordinates delta.
 * @details This function gets the screen coordinates delta of map center
 * movement if the event type is #MAP_EVENT_ACTION.
 * Otherwise it returns #MAPS_ERROR_INVALID_PARAMETER.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	delta_x		The pointer to an integer in which to store the delta x
 * @param[out]	delta_y		The pointer to an integer in which to store the delta y
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_event_data_get_type()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_delta(const map_event_data_h event, int *delta_x, int *delta_y);

/**
 * @brief	Gets the event screen coordinates.
 * @details This function gets the event screen coordinates if the event type is
 * #MAP_EVENT_GESTURE.
 * Otherwise it returns #MAPS_ERROR_INVALID_PARAMETER.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	x		The pointer to an integer in which to store tha
 * x position of the event
 * @param[out]	y		The pointer to an integer in which to store the
 * y position of the event
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_event_data_get_type()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_position(const map_event_data_h event, int *x, int *y);

/**
 * @brief	Gets the number of fingers, detected in the event.
 * @details This function gets the number of user's fingers, detected in the
 * event if the event type is #MAP_EVENT_GESTURE.
 * Otherwise it returns #MAPS_ERROR_INVALID_PARAMETER.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	fingers		The pointer to an integer in which to store the
 * number of fingers, detected in the gesture
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_event_data_get_type()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_fingers(const map_event_data_h event, int *fingers);

/**
 * @brief	Gets zoom factor of zoom gesture or action.
 * @details This function gets the corresponding zoom factor if the event type is
 * #MAP_EVENT_GESTURE or #MAP_EVENT_ACTION.
 * Otherwise it returns the #MAPS_ERROR_INVALID_PARAMETER.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	zoom_factor	The pointer to a double in which to store the zoom factor
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_event_data_get_type()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_zoom_factor(const map_event_data_h event, double *zoom_factor);

/**
 * @brief	Gets rotation angle of rotation gesture or action.
 * @details This function gets the corresponding rotation angle if the event type is
 * #MAP_EVENT_ACTION.
 * Otherwise it returns #MAPS_ERROR_INVALID_PARAMETER.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	rotation_angle	The pointer to a double in which to store the rotation angle
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_event_data_get_type()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_rotation_angle(const map_event_data_h event, double *rotation_angle);

/**
 * @brief	Gets object of event.
 * @details When the event type is #MAP_EVENT_OBJECT, this function gets
 * the corresponding object handle.
 * Otherwise it returns #MAPS_ERROR_INVALID_PARAMETER.
 * @since_tizen 3.0
 * @remarks The @a object must not be released.
 *
 * @param[in]	event		The event data handle
 * @param[out]	object		The pointer to #map_object_h in which to store the object handle
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_event_data_get_type()
 * @see map_view_on_event_cb()
 */
int map_event_data_get_object(const map_event_data_h event, map_object_h *object);


#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_EVENT_DATA_H__*/
