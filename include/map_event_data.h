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
 * @file map_gesture_data.h
 * @brief This file contains functions related to user gestures on map view.
 *
 * @addtogroup CAPI_MAP_EVENT_MODULE
 * @{
 * @brief This provides APIs related to operations with user gestures on the
 * map view.
 * @details The handle of gesture event data is retrieved to the application
 * via map view callback when the map view meets one of supported
 * gesture types.
 * \n The callback of type map_view_on_gesture_cb() are registered with
 * map_view_set_gesture_callback().
 * \n Supported gesture types are listed in #map_gesture_e.
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	The map gesture data handle
 * @details The handle of map gesture event data instance which is retrieved in
 * map_view_on_gesture_cb().
 * @remarks To release the handle use map_gesture_data_destroy().
 * \n To clone the handle use map_gesture_data_clone().
 * @since_tizen 3.0
 *
 * @see map_gesture_data_destroy()
 * @see map_gesture_data_clone()
 * @see map_view_on_gesture_cb().
 */
typedef void *map_gesture_data_h;


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
 * @pre @a event may be obtained in map_view_on_gesture_cb()
 *
 * @see #map_gesture_data_h
 * @see map_gesture_data_clone()
 */
int map_gesture_data_destroy(map_gesture_data_h gesture);

/**
 * @brief	Clones the event handle.
 * @details This function clones the event handle @a origin and all its
 * resources.
 * @since_tizen 3.0
 * @remarks @a cloned must be released using map_gesture_data_destroy().
 *
 * @param[in]	origin		The original event handle
 * @param[out]	cloned		A cloned event handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a event may be obtained in map_view_on_gesture_cb()
 *
 * @see #map_gesture_data_h
 * @see map_gesture_data_destroy()
 * @see map_view_on_gesture_cb()
 */
int map_gesture_data_clone(const map_gesture_data_h origin,
			   map_gesture_data_h *cloned);


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
 * @pre @a event may be obtained in map_view_on_gesture_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

static void map_view_gesture_cb(maps_error_e result,
			      map_gesture_data_h gesture_data,
			      void *user_data)
{
	LOG("map_view_gesture_cb enter");

	map_gesture_e gesture_type = MAP_GESTURE_NONE;
	map_gesture_data_get_type(event_data, &gesture_type);
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
	map_gesture_data_destroy(gesture_data);
}
 * @endcode
 *
 * @see map_view_on_gesture_cb()
 * @see map_gesture_data_destroy()
 * @see #map_gesture_data_h
 */
int map_gesture_data_get_type(const map_gesture_data_h event,
			      map_gesture_e *gesture_type);


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
 * @pre @a event may be obtained in map_view_on_gesture_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// event obtained earlier in the map_view_on_gesture_cb()

int error = MAPS_ERROR_NONE;
int fingers = 0;

// Getting the number of user's fingers
error = map_gesture_data_get_fingers(event, &fingers);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_gesture_data_get_type()
 * @see #map_event_type_e
 * @see #map_gesture_data_h
 * @see map_view_on_gesture_cb()
 */
int map_gesture_data_get_fingers(map_gesture_data_h gesture, int *fingers);

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
 * @pre @a event may be obtained in map_view_on_gesture_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// event obtained earlier in the map_view_on_gesture_cb()

int error = MAPS_ERROR_NONE;
double zoom_factor = .0;

// Getting the new zoom factor
error = map_gesture_data_get_zoom_factor(event, &zoom_factor);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_gesture_data_get_type()
 * @see #map_gesture_data_h
 * @see map_view_on_gesture_cb()
 */
int map_gesture_data_get_zoom_factor(map_gesture_data_h gesture,
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
 * @pre @a event may be obtained in map_view_on_gesture_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// event obtained earlier in the map_view_on_gesture_cb()

int error = MAPS_ERROR_NONE;
double rotation_angle = .0;

// Getting the new rotation angle
error = map_gesture_data_get_rotation_angle(event, &rotation_angle);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_gesture_data_get_type()
 * @see #map_event_type_e
 * @see #map_gesture_data_h
 * @see map_view_on_gesture_cb()
 */
int map_gesture_data_get_rotation_angle(map_gesture_data_h event,
				      double *rotation_angle);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_EVENT_DATA_H__*/
