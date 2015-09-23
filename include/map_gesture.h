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


#ifndef __MAP_GESTURE_H__
#define __MAP_GESTURE_H__

/**
 *
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_GESTURE_MODULE User's Gestures
 *
 * @file map_gesture.h
 * @brief This file contains functions related to user's gestures on the
 * map view.
 *
 * @addtogroup CAPI_MAP_GESTURE_MODULE
 * @{
 * @brief This provides APIs related to operations with user's gestures on
 * map view.
 * @details The handle of gesture data is retrieved to the application
 * via map_view_on_gesture_cb() callback when one of gesture types, listed in
 * #map_gesture_e, occurs.
 * \n To register and unregister the callback use
 * map_view_set_gesture_cb() and map_view_unset_gesture_cb()
 * respectively.
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	The user's gesture data handle
 * @details The handle of user's gesture data instance which is retrieved via
 * map_view_on_gesture_cb().
 * @remarks To release the handle use map_gesture_destroy().
 * \n To get the type of the gesture use map_gesture_get_type().
 * @since_tizen 3.0
 *
 * @see map_gesture_destroy()
 * @see map_view_on_gesture_cb().
 */
typedef void *map_gesture_h;


/**
 * @brief	Enumeration of user gestures over map view
 * @since_tizen 3.0
 */
typedef enum _map_gesture_e {
#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	MAP_GESTURE_SCROLL, /**< Indicates the move map user gesture */

	MAP_GESTURE_ZOOM, /**< Indicates the zoom user gesture */

	MAP_GESTURE_FLICK, /**< Indicates the flick user gesture */

	MAP_GESTURE_TAP, /**< Indicates the tap user gesture */

	MAP_GESTURE_DOUBLE_TAP, /**< Indicates the double tap user gesture */

	MAP_GESTURE_2_FINGER_TAP, /**< Indicates the two-finger tap user
				    gesture */

	MAP_GESTURE_SINGLE_FINGER_ZOOM, /**< Indicates the single finger zoom
					  user gesture (the gesture consist of
					  initial click and following pan) */

	MAP_GESTURE_LONG_PRESS, /**< Indicates the long press user gesture */

	MAP_GESTURE_NONE, /**< Indicates the empty gesture */
#else
	MAP_GESTURE_SCROLL, /**< Indicates the move map user gesture */

	MAP_GESTURE_FLICK, /**< Indicates the flick user gesture */

	MAP_GESTURE_PINCH, /**< Indicates the pinch user gesture */

	MAP_GESTURE_TAP, /**< Indicates the tap user gesture */

	MAP_GESTURE_DOUBLE_TAP, /**< Indicates the double tap user gesture */

	MAP_GESTURE_2_FINGER_TAP, /**< Indicates the two-finger tap user
				    gesture */

	MAP_GESTURE_ZOOM, /**< Indicates the zoom user gesture (performing with
			    pinch) */

	MAP_GESTURE_SINGLE_FINGER_ZOOM, /**< Indicates the single finger zoom
					  user gesture (the gesture consist of
					  initial click and following pan) */

	MAP_GESTURE_ROTATE, /**< Indicates the rotate user gesture */

	MAP_GESTURE_LONG_PRESS, /**< Indicates the long press user gesture */

	MAP_GESTURE_NONE /**< Indicates the empty gesture */
#endif
} map_gesture_e;

/**
 * @brief	Destroys the gesture data handle.
 * @details This function destroys the gesture data handle and releases all its
 * resources.
 * @since_tizen 3.0
 *
 * @param[in]	gesture		The gesture data handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a gesture may be obtained in map_view_on_gesture_cb()
 *
 * @see #map_gesture_h
 * @see map_view_on_gesture_cb()
 */
int map_gesture_destroy(map_gesture_h gesture);

/**
 * @brief	Gets the gesture type.
 * @details This function gets the gesture type.
 * @since_tizen 3.0
 *
 * @param[in]	gesture		The gesture data handle
 * @param[out]	type		The pointer to the #map_gesture_e in which
 * to store the gesture type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a gesture may be obtained in map_view_on_gesture_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

static void map_view_gesture_cb(map_gesture_h gesture, void *user_data)
{
	map_gesture_e gesture_type = MAP_GESTURE_NONE;
	map_gesture_get_type(gesture, &gesture_type);
	switch (gesture_type) {
		case MAP_GESTURE_SCROLL:
			LOG("MAP_GESTURE_SCROLL");
			break;
		case MAP_GESTURE_TAP:
			LOG("MAP_GESTURE_TAP");
			break;
		case MAP_GESTURE_DOUBLE_TAP:
			LOG("MAP_GESTURE_SCROLL");
			break;
		case MAP_GESTURE_2_FINGER_TAP:
			LOG("MAP_GESTURE_2_FINGER_TAP");
			break;
		case MAP_GESTURE_LONG_PRESS:
			LOG("MAP_GESTURE_LONG_PRESS");
			break;
		case MAP_GESTURE_NONE:
		default:
			LOG("default gesture type");
			break;
	}
	map_gesture_destroy(gesture);
}
 * @endcode
 *
 * @see #map_gesture_e
 * @see map_gesture_get_fingers()
 * @see map_gesture_get_zoom()
 * @see map_gesture_get_rotation()
 * @see map_view_on_gesture_cb()
 */
int map_gesture_get_type(const map_gesture_h gesture, map_gesture_e *type);


/**
 * @brief	Gets the number of fingers, detected in the gesture.
 * @details This function gets the number of user's fingers, detected in the
 * gesture.
 * @since_tizen 3.0
 *
 * @param[in]	gesture		The gesture data handle
 * @param[out]	fingers		The pointer to an integer in which to store the
 * number of fingers, detected in the gesture
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a gesture may be obtained in map_view_on_gesture_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// gesture obtained earlier in the map_view_on_gesture_cb()

int error = MAPS_ERROR_NONE;
int fingers = 0;

// Getting the number of user's fingers
error = map_gesture_get_fingers(gesture, &fingers);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_gesture_get_type()
 * @see map_gesture_get_zoom()
 * @see map_gesture_get_rotation()
 * @see map_view_on_gesture_cb()
 */
int map_gesture_get_fingers(map_gesture_h gesture, int *fingers);

/**
 * @brief	Gets zoom factor of gesture.
 * @details This function gets the zoom factor, set to the map in response on
 * zoom gesture.
 * @since_tizen 3.0
 *
 * @param[in]	gesture		The gesture data handle
 * @param[out]	zoom_factor	The pointer to a double in which to store the
 * zoom factor
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a gesture may be obtained in map_view_on_gesture_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// gesture obtained earlier in the map_view_on_gesture_cb()

int error = MAPS_ERROR_NONE;
double zoom_factor = .0;

// Getting the new zoom factor
error = map_gesture_get_zoom(gesture, &zoom_factor);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_gesture_get_type()
 * @see map_gesture_get_fingers()
 * @see map_gesture_get_rotation()
 * @see map_view_on_gesture_cb()
 */
int map_gesture_get_zoom(map_gesture_h gesture, double *zoom_factor);

/**
 * @brief	Gets rotation angle of gesture.
 * @details This function gets the rotation angle, set to the map in response on
 * rotation gesture.
 * @since_tizen 3.0
 *
 * @param[in]	gesture		The gesture data handle
 * @param[out]	rotation_angle	The pointer to a double in which to store the
 * rotation angle
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a gesture may be obtained in map_view_on_gesture_cb()
 *
 * @par Example
 * @code
#include <map_view.h>

// gesture obtained earlier in the map_view_on_gesture_cb()

int error = MAPS_ERROR_NONE;
double rotation_angle = .0;

// Getting the new rotation angle
error = map_gesture_get_rotation(gesture, &rotation_angle);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_gesture_get_type()
 * @see map_gesture_get_zoom()
 * @see map_gesture_get_fingers()
 * @see map_view_on_gesture_cb()
 */
int map_gesture_get_rotation(map_gesture_h gesture, double *rotation_angle);


#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_GESTURE_H__*/
