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
 * @brief This file contains the functions related to Map View Event Data
 * information.
 *
 * @addtogroup CAPI_MAP_EVENT_MODULE
 * @{
 * @brief This provides APIs related to operations with Map View Event Data
 * @details The handle of Map View Event Data is retrieved to the Application
 * via Map View Event Callbacks when the Map View processes one of supported
 * event types.
 * \n The callbacks of type map_view_on_event_cb() are registered with
 * map_view_set_event_callback().
 * \n Supported event types are following:
 *  * Gesture: the user performed a gesture over the map, for example
 *  are tap, double tap, rotate map, long press, etc. Gesture types are listed
 *  in #map_gesture_e
 *  * Action: map performed some action, such as zooming or rotating. Actions
 *  are listed in #map_action_e
 *  * Ready: indicates, that the view finished its previous task
 *  .
 *
 * @par Features availability by Event Type
 * |                                     | Gesture | Action | Ready |
 * | :---------------------------------- | :-----: | :----: | :---: |
 * | map_event_data_destroy()            | +       | +      | +     |
 * | map_event_data_clone()              | +       | +      | +     |
 * | map_event_data_get_type()           | +       | +      | +     |
 * | map_event_data_get_gesture_type()   | +       |        |       |
 * | map_event_data_get_action_type()    |         | +      |       |
 * | map_event_data_get_center()         |         | +      |       |
 * | map_event_data_get_delta()          |         | +      |       |
 * | map_event_data_get_xy()             | +       |        |       |
 * | map_event_data_get_fingers()        | +       |        |       |
 * | map_event_data_get_zoom_factor()    | +       | +      |       |
 * | map_event_data_get_rotation_angle() | +       | +      |       |
 *
 * @par Enumerations availability by Event Type
 * | Enumeration/Event Type              | Gesture | Action | Ready |
 * | :---------------------------------- | :-----: | :----: | :---: |
 * | #map_event_type_e                   | +       | +      | +     |
 * | #map_gesture_e                      | +       |        |       |
 * | #map_action_e                       |         | +      |       |
 *
 * @remarks Note: the event of type #MAP_EVENT_READY has no additional data
 * besides the type.
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	The Map View Event data handle
 * @details The handle of Map View Event Data instance which is retrieved in
 * map_view_on_event_cb().
 * @remarks To release the handle use map_event_data_destroy().
 * \n To clone the handle use map_event_data_clone().
 * @since_tizen 3.0
 *
 * @see map_event_data_destroy()
 * @see map_event_data_clone()
 * @see map_view_on_event_cb().
 */
typedef void *map_event_data_h;


/**
 * @brief	Enumerations of user gestures over Map View
 * @details This enumeration is valuable if the event type is
 * #MAP_EVENT_GESTURE.
 * @since_tizen 3.0
 */
typedef enum _map_gesture_e {

	MAP_GESTURE_PAN, /**< Indicates the pan user gesture */

	MAP_GESTURE_FLICK, /**< Indicates the flick user gesture */

	MAP_GESTURE_PINCH, /**< Indicates the pinch user gesture */

	MAP_GESTURE_TAP, /**< Indicates the tap user gesture */

	MAP_GESTURE_DOUBLE_TAP, /**< Indicates the double tap user gesture */

	MAP_GESTURE_2_FINGER_TAP, /**< Indicates the two-finger tap user
				    gesture */

	MAP_GESTURE_ZOOM, /**< Indicates the zoom user gesture (performing with
			    pinch) */

	MAP_GESTURE_SINGLE_FINGER_ZOOM, /**< Indicates the single finger zoom user
					  gesture (the gesture consist of initial
					  click and following pan) */

	MAP_GESTURE_ROTATE, /**< Indicates the rotate user gesture */

	MAP_GESTURE_PRESS, /**< Indicates the long press user gesture */

	MAP_GESTURE_NONE /**< Indicates the empty gesture */

} map_gesture_e;

/**
 * @brief	Enumerations of Map View actions
 * @details This enumeration is valuable if the event type is
 * #MAP_EVENT_ACTION.
 * @since_tizen 3.0
 */
typedef enum _map_action_e {

	MAP_ACTION_SET_CENTER, /**< Indicates the set center action */

	MAP_ACTION_MOVE_CENTER, /**< Indicates the move center action */

	MAP_ACTION_ZOOM, /**< Indicates the zoom action */

	MAP_ACTION_ROTATE, /**< Indicates the rotation action */

	MAP_ACTION_NONE, /**< Indicates the empty action */

} map_action_e;

/**
 * @brief	Enumerations of map event types
 * @since_tizen 3.0
 */
typedef enum _map_event_type_e {

	MAP_EVENT_GESTURE, /**< Indicates the user's gesture event callback */

	MAP_EVENT_ACTION, /**< Indicates the map view action callback */

	MAP_EVENT_READY /**< Indicates the map view readiness callback */

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
int map_event_data_destroy(map_event_data_h event);

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
int map_event_data_clone(const map_event_data_h origin,
			 map_event_data_h *cloned);


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
 * @par Example
 * @code
#include <map_view.h>

static void map_view_event_cb(maps_error_e result,
			      const map_event_type_e type,
			      map_event_data_h event_data,
			      void *user_data)
{
	LOG("map_view_event_cb enter");

	map_gesture_e gesture_type = MAP_GESTURE_NONE;
	map_action_e action_type = MAP_ACTION_NONE;

	switch (type) {
		case MAP_EVENT_GESTURE:
			LOG("MAP_EVENT_GESTURE");
			map_event_data_get_gesture_type(event_data,
							&gesture_type);
			switch (gesture_type) {
				case MAP_GESTURE_PAN:
					LOG("MAP_GESTURE_PAN");
					break;
				case MAP_GESTURE_TAP:
					LOG("MAP_GESTURE_TAP");
					break;
				case MAP_GESTURE_DOUBLE_TAP:
					LOG("MAP_GESTURE_PAN");
					break;
				case MAP_GESTURE_2_FINGER_TAP:
					LOG("MAP_GESTURE_2_FINGER_TAP");
					break;
				case MAP_GESTURE_PRESS:
					LOG("MAP_GESTURE_PRESS");
					break;
				case MAP_GESTURE_NONE:
				default:
					LOG("default gesture_type");
					break;
			}
			break;
		case MAP_EVENT_ACTION:
			LOG("MAP_EVENT_ACTION");
			map_event_data_get_action_type(event_data,
						       &action_type);
			switch (action_type) {
				case MAP_ACTION_SET_CENTER:
					LOG("MAP_ACTION_SET_CENTER");
					break;
				case MAP_ACTION_MOVE_CENTER:
					LOG("MAP_ACTION_MOVE_CENTER");
					break;
				case MAP_ACTION_ZOOM:
					LOG("MAP_ACTION_ZOOM");
					break;
				case MAP_ACTION_ROTATE:
					LOG("MAP_ACTION_ROTATE");
					break;
				case MAP_ACTION_NONE:
				default:
					LOG("default action_type");
					break;
			}
			break;
		case MAP_EVENT_READY:
			LOG("MAP_EVENT_READY");
			break;
		default:
			LOG("default type");
			break;
	}
	map_event_data_destroy(event_data);
}
 * @endcode
 *
 * @see #map_event_type_e
 * @see map_view_on_event_cb()
 * @see map_event_data_destroy()
 * @see #map_event_data_h
 */
int map_event_data_get_type(const map_event_data_h event,
			    map_event_type_e *event_type);
/**
 * @brief	Gets the event gesture type.
 * @details This function gets the event gesture type if the event type is
 * #MAP_EVENT_GESTURE or returns the #MAPS_ERROR_NOT_SUPPORTED instead.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	gesture_type	The pointer to the #map_gesture_e in which to
 * store the gesture type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The request is not supported
 * with this event type
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see map_event_data_get_type()
 * @see #map_gesture_e
 * @see #map_event_data_h
 * @see map_view_on_event_cb()
 */
int map_event_data_get_gesture_type(map_event_data_h event,
				    map_gesture_e *gesture_type);
/**
 * @brief	Gets the event action type.
 * @details This function gets the event action type if the event type is
 * #MAP_EVENT_ACTION or returns the #MAPS_ERROR_NOT_SUPPORTED instead.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	action_type	The pointer to the #map_action_e in which to
 * store the action type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The request is not supported
 * with this event type
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @see map_event_data_get_type()
 * @see #map_action_e
 * @see #map_event_data_h
 * @see map_view_on_event_cb()
 */
int map_event_data_get_action_type(map_event_data_h event,
				   map_action_e *action_type);

/**
 * @brief	Gets the new geographical coordinates of the map center
 * @details This function gets the new geographical coordinates of the map
 * center if the event type is #MAP_EVENT_ACTION or returns the
 * #MAPS_ERROR_NOT_SUPPORTED
 * instead.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	center		The pointer to the #maps_coordinates_h in which
 * to store the new center coordinates
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The request is not supported
 * with this event type
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// event obtained earlier in the map_view_on_event_cb

int error = MAPS_ERROR_NONE;
maps_coordinates_h center = NULL;

// Getting coordinates of new center of the map
error = map_event_data_get_center(event, &center);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Do something with center coordinates ...

// Release coordinates
maps_coordinates_destroy(center);
 * @endcode
 *
 * @see map_event_data_get_type()
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see #maps_coordinates_h
 * @see map_view_on_event_cb()
 */
int map_event_data_get_center(map_event_data_h event,
			      maps_coordinates_h *center);

/**
 * @brief	Gets the delta of center movement, in screen coordinates.
 * @details This function gets the delta of center movement, in screen
 * coordinates, if the event type is #MAP_EVENT_ACTION or returns the
 * #MAPS_ERROR_NOT_SUPPORTED instead.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	delta_x		The pointer to an integer in which to store the
 * delta x
 * @param[out]	delta_y		The pointer to an integer in which to store the
 * delta y
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The request is not supported
 * with this event type
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// event obtained earlier in the map_view_on_event_cb()

int error = MAPS_ERROR_NONE;
int delta_x = 0;
int delta_y = 0;

// Getting the delta coordinates of moved center of the map
error = map_event_data_get_delta(event, &delta_x,  &delta_y);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_event_data_get_type()
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_view_on_event_cb()
 */
int map_event_data_get_delta(map_event_data_h event,
			     int *delta_x,
			     int *delta_y);

/**
 * @brief	Gets the event screen coordinates.
 * @details This function gets the event screen coordinates if the event type is
 * #MAP_EVENT_GESTURE or returns the #MAPS_ERROR_NOT_SUPPORTED instead.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	x		The pointer to an integer in which to store tha
 * x coordinate of the event
 * @param[out]	y		The pointer to an integer in which to store the
 * y coordinate of the event
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The request is not supported
 * with this event type
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// event obtained earlier in the map_view_on_event_cb()

int error = MAPS_ERROR_NONE;
int x = 0;
int y = 0;

// Getting screen coordinates of the event
error = map_event_data_get_xy(event, &x, &y);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_event_data_get_type()
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_view_on_event_cb()
 */
int map_event_data_get_xy(map_event_data_h event, int *x, int* y);

/**
 * @brief	Gets the number of fingers, detected in the event.
 * @details This function gets the number of user's fingers, detected in the
 * event, when the event type is #MAP_EVENT_GESTURE or returns the
 * #MAPS_ERROR_NOT_SUPPORTED instead.
 * @since_tizen 3.0
 *
 * @param[in]	event		The event data handle
 * @param[out]	fingers		The pointer to an integer in which to store the
 * number of fingers, detected in the gesture
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The request is not supported
 * with this event type
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// event obtained earlier in the map_view_on_event_cb()

int error = MAPS_ERROR_NONE;
int fingers = 0;

// Getting the number of user's fingers
error = map_event_data_get_fingers(event, &fingers);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_event_data_get_type()
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_view_on_event_cb()
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
 * @param[out]	zoom_factor	The pointer to a double in which to store the
 * zoom factor
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The request is not supported
 * with this event type
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// event obtained earlier in the map_view_on_event_cb()

int error = MAPS_ERROR_NONE;
double zoom_factor = .0;

// Getting the new zoom factor
error = map_event_data_get_zoom_factor(event, &zoom_factor);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_event_data_get_type()
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_view_on_event_cb()
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
 * @param[out]	rotation_angle	The pointer to a double in which to store the
 * rotation angle
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED The request is not supported
 * with this event type
 *
 * @pre @a event may be obtained in map_view_on_event_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// event obtained earlier in the map_view_on_event_cb()

int error = MAPS_ERROR_NONE;
double rotation_angle = .0;

// Getting the new rotation angle
error = map_event_data_get_rotation_angle(event, &rotation_angle);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_event_data_get_type()
 * @see #map_event_type_e
 * @see #map_event_data_h
 * @see map_view_on_event_cb()
 */
int map_event_data_get_rotation_angle(map_event_data_h event,
				      double *rotation_angle);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_EVENT_DATA_H__*/
