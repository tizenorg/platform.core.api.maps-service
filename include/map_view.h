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

#ifndef __MAP_VIEW_H__
#define __MAP_VIEW_H__

#include <Evas.h>
#include <maps_service.h>
#include <maps_gesture.h>
#include <map_snapshot.h>

/**
 *
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_VIEW_PANEL_MODULE Map View
 *
 * @file map_view.h
 * @brief This file contains the top level functions of mapping API
 *
 * @addtogroup CAPI_MAP_VIEW_PANEL_MODULE
 * @{
 * @brief This provides mapping APIs related to operations with map view
 * @details Supported mapping API features include:
 *  * Creating and destroying the map view port, moving and resizing it
 *  * Changing map zoom, and orientation
 *  * Changing map properties: theme (day, satellite ,terrain),
 *  language (English, Russian, Chinese, Italian, German, Spanish, etc.)
 *  * Converting screen coordinates to geographical and vise versa
 *  * Managing user's gestures: receiving user's gesture events, enabling or
 *  disabling specified user gestures, assigning map view actions, which must be
 *  taken in response on user gestures
 *  .
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	The map view handle
 * @details The handle of map view instance.
 * @remarks To release the handle use map_view_destroy().
 * @since_tizen 3.0
 *
 * @see map_view_create()
 * @see map_view_destroy()
 */
typedef void *map_view_h;


/**
 * @brief	Enumeration of map view types (themes)
 * @since_tizen 3.0
 */
typedef enum _map_view_type_e {

	MAP_VIEW_TYPE_DAY, /**< Indicates the day theme */

	MAP_VIEW_TYPE_SATELLITE, /**< Indicates the satellite theme */

	MAP_VIEW_TYPE_TRANSIT, /**< Indicates the public transport scheme in
				 daylight mode */

	MAP_VIEW_TYPE_TRAFFIC, /**< Indicates the traffic scheme in
				 daylight mode */

	MAP_VIEW_TYPE_HYBRID_DAY, /**< Indicates the hybrid theme which
				    composes satellite and roads themes */

	MAP_VIEW_TYPE_HYBRID_TRANSIT, /**< Indicates the satellite map view with
				public transport scheme in daylight mode */

	MAP_VIEW_TYPE_HYBRID_TRAFFIC, /**< Indicates the satellite map view with
				streets scheme designed for traffic */


	MAP_VIEW_TYPE_TERRAIN,	/**< Indicates the terrain theme */

} map_view_type_e;

/**
 * @brief	Enumerations of map view actions
 * @details This is an enumeration of actions map view is capable to perform.
 * @since_tizen 3.0
 * @remarks The view performs actions in response on user gestures, such as tap,
 * zoom, long press, or APIs, such as set center or change zoom level.
 * \n To re-assign view actions to user gestures use map_view_set_gesture_action().
 *
 * @see map_view_set_gesture_action()
 * @see #map_gesture_e
 */
typedef enum _map_action_e {

	MAP_ACTION_SCROLL, /**< Indicates the move center action */

	MAP_ACTION_ZOOM, /**< Indicates the zoom action */

	MAP_ACTION_ROTATE, /**< Indicates the rotation action */

	MAP_ACTION_NONE, /**< Indicates the empty action */

} map_action_e;


/**
 * @brief	Called when user's gesture occurs.
 * @details The map view invokes this callback when one of user's gestures,
 * listed in #map_gesture_e, occurs.
 * @since_tizen 3.0
 * @remarks @a gesture handle must be released using map_gesture_destroy().
 * \n To use @a gesture outside this function, clone it with
 * map_gesture_clone().
 *
 * @param[in]	gesture		The gesture data handle
 * @param[in]	user_data	The user data pointer registered in
 * map_view_set_gesture_callback()
 *
 * @pre map_view_set_gesture_callback() sets this callback.
 * @pre User's gestures, such as tap, double tap, map rotate or long press
 * invoke the callback.
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
		case MAP_GESTURE_PRESS:
			LOG("MAP_GESTURE_PRESS");
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
 * @see map_view_set_gesture_callback()
 * @see map_view_unset_gesture_callback()
 * @see #map_gesture_h
 */
typedef void(*map_view_on_gesture_cb) (map_gesture_h gesture, void *user_data);


/* ----------------------CREATE AND DESTROY-----------------------------------*/

/**
 * @brief	Creates the map view and link it to the instance of
 * maps service.
 * @details This function creates a new map view widget, allocate all needed
 * resources and issues its handle.
 * \n The newly created map view is linked to the specified maps service.
 * \n The view by default is resized to fill whole parent area.
 * \n To center the map on a specifies geographical coordinates with a desired
 * zoom level and rotation angle use map_view_set_center(),
 * map_view_set_zoom_level() and map_view_set_orientation() respectively.
 * \n To change map theme, size and visibility properties use
 * map_view_set_type(), map_view_set_geometry() and  map_view_set_visibility()
 * respectively.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks @a view must be released using map_view_destroy().
 *
 * @param[in]	maps		The maps service handle
 * @param[in]	parent		The parent visual object
 * @param[out]	view		The handle pointer to a map_viwe_h,
 * in which to store the newly created map view handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a maps is created using maps_service_create().
 *
 * @par Example
 * @code
#include <maps_service.h>
#include <map_view.h>

int main (int argc, char *argv[])
{
	maps_service_h maps = NULL;
	map_view_h view = NULL;
	Evas_Object *map_view_place_holder = NULL;
	int error = MAPS_ERROR_NONE;
	do {
		// Create App UI ...
		// map_view_place_holder = ...

		// Create an instance of maps service
		error = maps_service_create("maps Provider", &maps);
		if(error != MAPS_ERROR_NONE)
			break;

		// Set the provider security key
		error = maps_service_set_provider_key(maps, "provider_key");
		if(error != MAPS_ERROR_NONE)
			break;

		// Create a map view
		error = map_view_create(maps, map_view_place_holder, &view);
		if(error != MAPS_ERROR_NONE)
			break;

		// Work with map view ...

	} while(false);

	// Release the map view, maps service and all used resources
	map_view_destroy(view);
	maps_service_destroy(maps);
	return error;
}
 * @endcode
 *
 * @see map_view_destroy()
 * @see maps_service_create()
 * @see map_view_set_center()
 * @see map_view_set_zoom_level()
 * @see map_view_set_orientation()
 * @see map_view_set_type()
 * @see map_view_set_geometry()
 * @see map_view_set_visibility()
 */
int map_view_create(const maps_service_h maps,
		    Evas_Object *parent,
		    map_view_h *view);

/**
 * @brief	Destroys the map view.
 * @details This function destroys the map view, releases its handle and
 * all allocated resources, unlinks the view from the instance of maps service.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 *
 * @param[in]	view		The view handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 *
 */
int map_view_destroy(map_view_h view);


/*----------------------MAP ZOOM, ROTATE, SET CENTER--------------------------*/


/**
 * @brief	Centers the map on a given geographical coordinates.
 * @details This function centers the map on a given geographical coordinates
 * using current zoom and orientation.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	coordinates	The geographical coordinates of the position to
 * be centered on the map view
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
maps_coordinates_h coordinates = NULL;

// Obtain a geographical location to be focused
error = maps_coordinates_create(39.929386, 23.696087, &coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Center a map view on specified coordinates
error = map_view_set_center(view, coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Release coordinates handle
maps_coordinates_destroy(coordinates);
 * @endcode
 *
 * @see map_view_get_center()
 * @see map_view_set_zoom_level()
 * @see map_view_set_orientation()
 * @see maps_coordinates_create()
 * @see map_view_create()
 */
int map_view_set_center(const map_view_h view,
			const maps_coordinates_h coordinates);

/**
 * @brief	Get the central coordinates of a map.
 * @details This function gets the current central coordinates of a map.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	coordinates	The pointer to #maps_coordinates_h in which to
 * store the geographical coordinates of the central position of the map
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
maps_coordinates_h coordinates = NULL;

// Get the central coordinates of the map
error = map_view_get_center(view, &coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Do something with obtained coordinates...
double latitude = .0;
double longitude = .0;
error = maps_coordinates_get(coordinates, &latitude, &longitude);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Release coordinates handle
maps_coordinates_destroy(coordinates);
 * @endcode
 *
 * @see map_view_set_center()
 * @see map_view_get_zoom_level()
 * @see map_view_get_orientation()
 * @see maps_coordinates_destroy()
 * @see map_view_create()
 */
int map_view_get_center(const map_view_h view,
			maps_coordinates_h *coordinates);

/**
 * @brief	Sets zooms level of the map.
 * @details This function sets the integer zoom level of the map.
 * \n If the specified zoom level exceeds the maps Provider allowed zoom range,
 * the function returns #MAPS_ERROR_INVALID_PARAMETER error.
 * \n To check the range of allowed zoom level use map_view_get_min_zoom_level()
 * and map_view_get_max_zoom_level().
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	level		The new zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set a specified zoom level for the map view
error = map_view_set_zoom_level(view, 8);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_zoom_level()
 * @see map_view_set_orientation()
 * @see map_view_get_min_zoom_level()
 * @see map_view_get_max_zoom_level()
 * @see map_view_create()
 */
int map_view_set_zoom_level(map_view_h view, const int level);

/**
 * @brief	Gets zooms level of the map.
 * @details This function gets the current integer zoom level of the map.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	level		The pointer to an integer in which to store the
 * current zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
int current_zoom_level = 0;

// Get the current zoom level of the map
error = map_view_get_zoom_level(view, &zoom_level);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_zoom_level()
 * @see map_view_get_min_zoom_level()
 * @see map_view_get_max_zoom_level()
 * @see map_view_create()
 */
int map_view_get_zoom_level(const map_view_h view, int *level);

/**
 * @brief	Gets the minimal zooms level of the map.
 * @details This function gets the minimally allowed zoom level of the map.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[out]	min_zoom_level	The pointer to an integer in which to store the
 * minimally allowed zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
int min_zoom_level = 0;

// Get the minimally allowed zoom level of the map
error = map_view_get_min_zoom_level(view, &min_zoom_level);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_max_zoom_level()
 * @see map_view_get_zoom_level()
 * @see map_view_set_zoom_level()
 * @see map_view_create()
 */
int map_view_get_min_zoom_level(const map_view_h view, int *min_zoom_level);

/**
 * @brief	Gets the maximal zooms level of the map.
 * @details This function gets the maximally allowed zoom level of the map.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[out]	max_zoom_level	The pointer to an integer in which to store the
 * maximally allowed zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
int max_zoom_level = 0;

// Get the maximally allowed zoom level of the map
error = map_view_get_max_zoom_level(view, &max_zoom_level);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_min_zoom_level()
 * @see map_view_get_zoom_level()
 * @see map_view_set_zoom_level()
 * @see map_view_create()
 */
int map_view_get_max_zoom_level(const map_view_h view, int *max_zoom_level);

/**
 * @brief	Set the map orientation on the map view.
 * @details This function sets the rotation angle of the map on the map view.
 * \n If the specified rotation angle exceeds the [0..360] range, the
 * function returns #MAPS_ERROR_INVALID_PARAMETER error.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[out]	angle		The new orientation angle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set a specified orientation of the map
error = map_view_set_orientation(view, 60.);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
*
 * @see map_view_get_orientation()
 * @see map_view_set_zoom_level()
 * @see map_view_create()
 */
int map_view_set_orientation(const map_view_h view, const double angle);

/**
 * @brief	Gets the map orientation.
 * @details This function gets the current map rotation angle on the map view.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	rotation_angle	The pointer to a double in which to store the
 * current rotation angle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
double current_orientation = .0;

// Get the current orientation of the map
error = map_view_get_orientation(view, &orientation);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_orientation()
 * @see map_view_get_zoom_level()
 * @see map_view_create()
 */
int map_view_get_orientation(const map_view_h view, double *rotation_angle);


/*----------------------SCREEN <--> GEOGRAPHY---------------------------------*/

/**
 * @brief	Converts screen coordinates to the geographical coordinates.
 * @details This function converts screen coordinates to the geographical
 * coordinates accordingly to the current map zoom and orientation.
 * @since_tizen 3.0
 * @remarks @a coordinates must be released using maps_coordinates_destroy().
 *
 * @param[in]	view		The view handle
 * @param[in]	x		The x coordinate on the screen
 * @param[in]	y		The y coordinate on the screen
 * @param[out]	coordinates	The pointer to a #maps_coordinates_h in which to
 * store the handle of geographical coordinates, corresponding to given screen
 * coordinates
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
maps_coordinates_h coordinates = NULL;

// Convert screen coordinates to geographical
error = map_view_screen_to_geography(view, 100, 150, &coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Do something with obtained geographical coordinates...

// Release the coordinates
maps_coordinates_destroy(coordinates);
 * @endcode
 *
 * @see map_view_geography_to_screen()
 * @see #maps_coordinates_h
 * @see maps_coordinates_destroy()
 * @see map_view_create()
 */
int map_view_screen_to_geography(const map_view_h view,
				 const int x,
				 const int y,
				 maps_coordinates_h *coordinates);

/**
 * @brief	Converts geographical coordinates to the screen coordinates.
 * @details This function converts geographical coordinates to the screen
 * coordinates accordingly to the current map zoom and orientation.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	coordinates	The geographical coordinates
 * @param[out]	x		The pointer to an integer in which to store the
 * x coordinate on the screen, corresponding to given geographical coordinates
 * @param[out]	y		The pointer to an integer in which to store the
 * y coordinate on the screen, corresponding to given geographical coordinates
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
maps_coordinates_h coordinates = NULL;
int x = 0;
int y = 0;

// Prepare a geographical coordinates
error = maps_coordinates_create(39.929386, 23.696087, &coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Convert screen coordinates to geographical
error = map_view_geography_to_screen(view, coordinates, &x, &y);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Do something with obtained screen coordinates x,y...

// Release the coordinates
maps_coordinates_destroy(coordinates);
 * @endcode
 *
 * @see map_view_screen_to_geography()
 * @see #maps_coordinates_h
 * @see maps_coordinates_create()
 * @see map_view_create()
 */
int map_view_geography_to_screen(const map_view_h view,
				 const maps_coordinates_h coordinates,
				 int *x,
				 int *y);


/* --------------------MAPS VIEW PREFERENCES----------------------------------*/


/**
 * @brief	Sets map view type.
 * @details This function switches the map view to a specified type, one of
 * listed in #map_view_type_e enumeration.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	type		The view type, one of listed in #map_view_type_e
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set Satellite map view theme
error = map_view_set_type(view, MAP_VIEW_TYPE_SATELLITE);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_view_type_e
 * @see map_view_get_type()
 * @see map_view_set_language()
 * @see map_view_create()
 */
int map_view_set_type(map_view_h view, const map_view_type_e type);

/**
 * @brief	Gets map view type.
 * @details This function gets the type of the given map view.
 * \n The type options are defined in the #map_view_type_e enumeration.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	type		The pointer to a #map_view_type_e in which to
 * store current view type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
map_view_type_e type = MAP_VIEW_TYPE_DAY;

// Obtain current map view theme
error = map_view_get_type(view, &type);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 * @see #map_view_type_e
 * @see map_view_set_type()
 * @see map_view_get_language()
 * @see map_view_create()
 */
int map_view_get_type(const map_view_h view, map_view_type_e *type);

/**
 * @brief	Sets map view language.
 * @details This function sets the language to the given map view.
 * \n Note that map display language is different from places and route
 * language.
 * @remarks Available language options are:
 *  - "ara"
 *  - "chi"
 *  - "cht"
 *  - "dut"
 *  - "eng"
 *  - "ger"
 *  - "gle"
 *  - "fre"
 *  - "ita"
 *  - "spa"
 *  - "rus"
 *  - "pol"
 *  - "gre"
 *  - "wel"
 *  .
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	language	The language string value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set Russian as map view language
error = map_view_set_language(view, "rus");
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_language()
 * @see map_view_set_type()
 * @see map_view_create()
 */
int map_view_set_language(map_view_h view, const char *language);

/**
 * @brief	Gets map view language.
 * @details This function gets the language set to the given map view.
 * \n Note that map display language is different from places and route
 * language.
 * @remarks @a language must be released using free().
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	language	The pointer to a char* in which to store the
 * language string value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
char *language = NULL;

// Set Russian as map view language
error = map_view_get_language(view, &language);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Do something with obtained language value

free(language);
 * @endcode
 *
 * @see map_view_set_language()
 * @see map_view_get_type()
 * @see map_view_create()
 */
int map_view_get_language(const map_view_h view, char **language);


/* --------------------MAPS PANEL MANIPULATIONS-------------------------------*/


/**
 * @brief	Gets the map view port.
 * @details This function gets the map view port as a pointer on Evas_Object.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	viewport	The pointer to Evas_Object in which to store
 * the map view port
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see Evas_Object
 */
int map_view_get_port(const map_view_h view, Evas_Object **viewport);

/**
 * @brief	Sets geometry of map view port.
 * @details This function set the position and (rectangular) size of the given
 * map view.
 * \n The position, naturally, will be relative to the top left corner of the
 * parent window.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	x		X screen coordinate to move the map view to
 * @param[in]	y		Y screen coordinate to move the map view to
 * @param[in]	width		The new width of map view, in screen units
 * @param[in]	height		The new height of map view, in screen units
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set map view geometry
error = map_view_set_geometry(view, 0, 0, 720, 1280);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_geometry()
 * @see map_view_resize()
 * @see map_view_set_visibility()
 * @see map_view_create()
 */
int map_view_set_geometry(const map_view_h view,
			  const int x,
			  const int y,
			  const int width,
			  const int height);

/**
 * @brief	Gets geometry of map view port.
 * @details This function retrieves the position and (rectangular) size of the
 * given map view.
 * \n The position, naturally, will be relative to the top left corner of the
 * parent window.
 * @remarks Use NULL pointers on the geometry components you're not
 * interested in.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	x		The pointer to an integer in which to store the X
 * coordinate of the view
 * @param[in]	y		The pointer to an integer in which to store the Y
 * coordinate of the view
 * @param[in]	width		The pointer to an integer in which to store the
 * width of the view
 * @param[in]	height		The pointer to an integer in which to store the
 * height of the view
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
int x = 0;
int y = 0;
int width = 0;
int height = 0;

// Obtain map view geometry
error = map_view_get_geometry(view, &x, &y, &width, &height);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_geometry()
 * @see map_view_resize()
 * @see map_view_get_visibility()
 * @see map_view_create()
 */
int map_view_get_geometry(const map_view_h view,
			  int *x,
			  int *y,
			  int *width,
			  int *height);

/**
 * @brief	Resizes the map view.
 * @details This function changes the size of the given map view.
 * @remarks Newly created map view port has the size of its parent.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	width		The new new width, in screen units
 * @param[in]	height		The new new height, in screen units
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Resize the map view
error = map_view_resize(view, 720, 1280);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_geometry()
 * @see map_view_get_geometry()
 * @see map_view_set_visibility()
 * @see map_view_create()
 */
int map_view_resize(const map_view_h view, const int width, const int height);

/**
 * @brief	Shows or hides the map view.
 * @details This function changes the visibility of map view.
 * on the screen.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	visible		The new visibility of the map view
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set map view visible
error = map_view_set_visibility(view, true);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_visibility()
 * @see map_view_set_geometry()
 * @see map_view_create()
 */
int map_view_set_visibility(const map_view_h view, const bool visible);

/**
 * @brief	Gets the map view visibility.
 * @details This function retrieves whether or not the given map view is
 * visible.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	visible		The pointer to a boolean in which to store the
 * visibility of the map view
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
bool visible = false;

// Set map view visible
error = map_view_get_visibility(view, &visible);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_visibility()
 * @see map_view_set_geometry()
 * @see map_view_create()
 */
int map_view_get_visibility(const map_view_h view, bool *visible);

/**
 * @brief	Redraws the map view.
 * @details This function forces the redraw routine of the map view on the
 * screen.
 * @remarks Note that this function only marks the map view as needed to be
 * redrawn. Actual redraw is happening when the OS drawing system is ready for
 * it.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_get_visibility()
 * @see map_view_set_visibility()
 * @see map_view_create()
 */
int map_view_redraw(const map_view_h view);


/* ---------------------USER CONTROL------------------------------------------*/


/**
 * @brief	Sets the user's gesture callback.
 * @details This function sets the callback which will be invoked every time the
 * map view processes the user's gesture over the map.
 * @since_tizen 3.0
 * @remarks To unregister the callback use map_view_unset_gesture_callback().
 *
 * @param[in]	view		The view handle
 * @param[in]	callback	The callback, matching the
 * map_view_on_gesture_cb() prototype
 * @param[in]	user_data	The user data pointer to be passed to the
 * callback
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

static void my_user_gesture_cb(map_gesture_h gesture, void *user_data)
{
	// Handle the gesture...

	// Release the gesture data
	map_gesture_destroy(gesture);
}

static int register_gesture_callbacks()
{
	// Register user's gesture callback
	int error = map_view_set_gesture_callback(view,
		my_user_gesture_cb, NULL);
	if (error != MAPS_ERROR_NONE) {
		// Handle the error...
	}
	return error;
}
 * @endcode
 *
 * @see map_view_unset_gesture_callback()
 * @see map_view_on_gesture_cb()
 * @see map_view_set_gesture_action()
 * @see map_view_set_gesture_enabled()
 * @see map_view_create()
 */
int map_view_set_gesture_callback(map_view_h view,
				  map_view_on_gesture_cb callback,
				  void *user_data);

/**
 * @brief	Unsets the user's gesture callback.
 * @details This function unsets the user's gesture callback.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @pre the user's gesture callback is registered using
 * map_view_set_gesture_callback()
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

// Unregister the user's gesture callback
int error = map_view_unset_gesture_callback(view);
if (error != MAPS_ERROR_NONE) {
	// Handle the error...
}
 * @endcode
 *
 * @see map_view_set_gesture_callback()
 * @see map_view_get_gesture_action()
 * @see map_view_get_gesture_enabled()
 * @see map_view_create()
 */
int map_view_unset_gesture_callback(map_view_h view);

/**
 * @brief	Assigns the action to the user gesture.
 * @details This function sets the behaviour of map view by assigning a type of
 * action, listed in #map_action_e, which should be taken in response on a
 * given user's gesture, one of #map_gesture_e.
 * @remarks Note: not all actions can be assigned with a specified gesture.
 * @par Matching gestures to actions
 * | Gesture                         | Allowed Action                       |
 * | :------------------------------ | :----------------------------------- |
 * | #MAP_GESTURE_SCROLL             | #MAP_ACTION_SCROLL                   |
 * | #MAP_GESTURE_FLICK              | #MAP_ACTION_SCROLL                   |
 * | #MAP_GESTURE_PINCH              | #MAP_ACTION_ZOOM                     |
 * | #MAP_GESTURE_TAP                | #MAP_ACTION_SCROLL, #MAP_ACTION_ZOOM |
 * | #MAP_GESTURE_DOUBLE_TAP         | #MAP_ACTION_ZOOM, #MAP_ACTION_SCROLL |
 * | #MAP_GESTURE_2_FINGER_TAP       | #MAP_ACTION_ZOOM                     |
 * | #MAP_GESTURE_ZOOM               | #MAP_ACTION_ZOOM                     |
 * | #MAP_GESTURE_SINGLE_FINGER_ZOOM | #MAP_ACTION_ZOOM                     |
 * | #MAP_GESTURE_ROTATE             | #MAP_ACTION_ROTATE                   |
 * | #MAP_GESTURE_PRESS              | #MAP_ACTION_SCROLL, #MAP_ACTION_ZOOM |
 * @remarks Note: each gesture can be assigned with #MAP_ACTION_NONE.
 * The Application will keep receiving corresponding gesture notifications,
 * however the map view will not be changing.
 *
 * @param[in]	view		The view handle
 * @param[in]	gesture		The user gesture, one of listed in
 * #map_gesture_e
 * @param[in]	action		The action which should be taken, on of
 * listed in #map_action_e
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Center the map on the position, taped by the User
error = map_view_set_gesture_action(view,
				    MAP_GESTURE_TAP,
				    MAP_ACTION_SET_CENTER);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_gesture_e
 * @see #map_action_e
 * @see map_view_get_gesture_action()
 * @see map_view_set_gesture_enabled()
 * @see map_view_get_gesture_enabled()
 * @see map_view_create()
 */
int map_view_set_gesture_action(map_view_h view,
				const map_gesture_e gesture,
				const map_action_e action);

/**
 * @brief	Gets the action, assigned to the user gesture.
 * @details This function gets the behaviour of map view: it retrieves the
 * action, assigned as a reaction on a given user gesture.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	gesture		The user gesture
 * @param[out]	action		The pointer to a #map_action_e in which
 * to store the action assigned to the user gesture
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
map_action_e action = MAP_ACTION_NONE;

// Get the action, assigned to the tap gesture
error = map_view_get_gesture_action(view, MAP_GESTURE_TAP, &action);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_gesture_e
 * @see #map_action_e
 * @see map_view_set_gesture_action()
 * @see map_view_set_gesture_enabled()
 * @see map_view_get_gesture_enabled()
 * @see map_view_create()
 */
int map_view_get_gesture_action(map_view_h view,
				const map_gesture_e gesture,
				map_action_e *action);

/**
 * @brief	Enables or disables the user gesture.
 * @details This function allows or blocks reaction on a given user gesture.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	gesture		The user gesture, one of listed in
 * #map_gesture_e
 * @param[in]	enabled		The enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Disabling long press gesture
error = map_view_set_gesture_enabled(view, MAP_GESTURE_PRESS, false);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_gesture_e
 * @see map_view_get_gesture_enabled()
 * @see map_view_set_gesture_action()
 * @see map_view_get_gesture_action()
 * @see map_view_create()
 */
int map_view_set_gesture_enabled(map_view_h view,
				 const map_gesture_e gesture,
				 const bool enabled);

/**
 * @brief	Gets the user gesture enable status.
 * @details This function retrieves the enable status of a given user gesture.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	gesture		The user gesture, one of listed in
 * #map_gesture_e
 * @param[out]	enabled		The pointer to a boolean in which to store the
 * enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @pre @a enabled status set ad default or modified using
 * map_view_set_gesture_enabled().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
bool enabled = false;

// Find out if the long press gesture is enabled
error = map_view_get_gesture_enabled(view, MAP_GESTURE_PRESS, &enabled);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_gesture_e
 * @see map_view_set_gesture_enabled()
 * @see map_view_set_gesture_action()
 * @see map_view_get_gesture_action()
 * @see map_view_create()
 */
int map_view_get_gesture_enabled(map_view_h view,
				 const map_gesture_e gesture,
				 bool *enabled);

/*----------------------------------------------------------------------------*/
/*
 * Snapshot Capture Service
 */

/**
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_VIEW_SNAPSHOT_MODULE Snapshot
 *
 * @addtogroup CAPI_MAP_VIEW_SNAPSHOT_MODULE
 * @{
 * @brief This provides APIs allowing to capture Map Snapshots.
 *
 */

/**
 * @brief	Called when snapshot requested with the
 * map_view_capture_snapshot().
 * @details The Map View invokes this callback while retrieving a snapshot of
 * the current Map View.
 * \n If snapshot capturing is failed, the value of @a snapshot is NULL.
 * @since_tizen 3.0
 * @remarks The @a snapshot must be released using map_snapshot_destroy().
 *
 * @param[in]	result		The result of request
 * @param[in]	snapshot	The snapshot handle
 * @param[in]	user_data	The pointer to user data passed from
 * map_view_capture_snapshot()
 *
 * @pre map_view_capture_snapshot() will invoke this callback.
 *
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 */
typedef void (*map_view_snapshot_cb) (maps_error_e result,
				     map_snapshot_h snapshot,
				     void *user_data);

/**
 * @brief	Captures a snapshot of the Map View.
 * \n The request is synchronous.
 * @details This function retrieves an image of a map currently represented on
 * the Map View.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks To check if Maps Provider is capable of capturing snapshots use
 * maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_CAPTURE_SNAPSHOT passed as @a service parameter.
 *
 * @param[in]	view		The Map View handle
 * @param[in]	capture_overlay	The flag indicating if capturing overlay (markers,
 * polylines, polygons, etc.) needed
 * @param[in]	callback	The callback which will receive snapshot
 * @param[in]	user_data	The pointer to user data to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @pre Call maps_service_create() and map_view_create() to issue Maps Service
 * and Map View handles respectively.
 * @post It invokes map_view_snapshot_cb() to deliver requested snapshot.
 *
 * @par Example
 * @code
#include <map_view.h>

static void _snapshot_cb(maps_error_e result, int request_id,
			 map_snapshot_h snapshot, void *user_data)
{
	if (!snapshot || !user_data)
		return false;

	Evas_Object *map_snapshot = (Evas_Object *)user_data;

	// Get snapshot pixels
	unsigned int *pixels = NULL;
	int error = map_snapshot_get_data(snapshot, &pixels);
	if (error == MAPS_ERROR_NONE) {
		// Draw snapshot on the canvas
		evas_object_image_data_set(map_snapshot, pixels);
	}

	// Optionally it is possible to fetch also the snapshot
	// - central coordinates and area
	// - zoom level and factor
	// - orientation
	// - Evas color space
	// - etc.

	map_snapshot_destroy(snapshot);
}

int main(int argc, char *argv[])
{
	maps_service_h maps = NULL;
	map_view_h view = NULL;
	Evas_Object *parent = NULL;
	Evas_Object *map_snapshot = NULL;
	int error = MAPS_ERROR_NONE;
	int request_id = 0;

	do {

		// Create an instance of Maps Service
		error = maps_service_create("MapsProvider", &maps);
		if (error != MAPS_ERROR_NONE)
			break;

		// Set security key
		maps_service_set_provider_key(maps, "your-security-key");

		// create an instance of Map View and application UI controls
		// parent = evas_object_box_add ...
		// map_snapshot = evas_object_image_add(parent_canvas);
		error = map_view_create(maps, parent, &view);
		if (error != MAPS_ERROR_NONE)
			break;

		// Specify Snapshot center
		maps_coordinates_h center = NULL;
		maps_coordinates_create(center, 39.930661, 23.695172);

		error = map_view_set_center(view, center);
		if (error != MAPS_ERROR_NONE)
			break;

		error = map_view_set_zoom_level(view, 12);
		if (error != MAPS_ERROR_NONE)
			break;

		// Capture snapshot
		error = map_view_capture_snapshot(view,
		    				  center,
		    				  false,
		    				  _snapshot_cb,
 						  map_snapshot);

		maps_coordinates_destroy(center);

		if (error != MAPS_ERROR_NONE)
			break;

	} while(false);

	// Release the Map View, Maps Service and used resources
	map_view_destroy(view);
	maps_service_destroy(maps);
	return error;
}
 * @endcode
 *
 * @see map_view_snapshot_cb()
 * @see maps_service_capture_snapshot()
 * @see #map_snapshot_h
 * @see map_snapshot_destroy()
 * @see map_view_create()
 * @see maps_service_create()
 */
int map_view_capture_snapshot(const map_view_h view,
			      bool capture_overlay,
			      map_view_snapshot_cb callback,
			      void *user_data);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_VIEW_H__*/
