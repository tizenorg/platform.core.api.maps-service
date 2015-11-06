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

/**
 *
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_VIEW_PANEL_MODULE Map View
 *
 * @file map_view.h
 * @brief This file contains the top level functions of Mapping API
 *
 * @addtogroup CAPI_MAP_VIEW_PANEL_MODULE
 * @{
 * @brief This provides Mapping APIs related to operations with Map View
 * @details Supported Mapping features include:
 *  * Creating and destroying the Map View panel, moving and resizing it
 *  * Changing map zoom, and orientation
 *  * Changing map properties: theme (day, satellite ,terrain),
 *  language (English, Russian, Chinese, Italian, German, Spanish, etc.)
 *  * Converting screen coordinates to geographical and vise versa
 *  .
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	The Map View handle
 * @details The handle of Map View instance.
 * @remarks To release the handle use map_view_destroy().
 * @since_tizen 3.0
 *
 * @see map_view_create()
 * @see map_view_destroy()
 */
typedef void *map_view_h;


/**
 * @brief	Enumerations of Map View types
 * @since_tizen 3.0
 */
typedef enum _map_view_type_e {

	MAP_VIEW_TYPE_DAY, /**< Indicates the day theme */

	MAP_VIEW_TYPE_SATELLITE, /**< Indicates the satellite theme */

	MAP_VIEW_TYPE_HYBRID, /**< Indicates the hybrid theme which
				composes satellite and roads themes */

	MAP_VIEW_TYPE_TERRAIN,	/**< Indicates the terrain theme */

} map_view_type_e;

/**
 * @brief	Called when the map event occurs.
 * @details The Map View invokes this callback when one of map events, listed
 * in #map_event_type_e occurs.
 * @since_tizen 3.0
 * @remarks @a event_data must be released using map_event_data_destroy().
 * \n To use @a event_data outside this function, clone it with
 * map_event_data_clone().
 *
 * @param[in]	result		The result of request
 * @param[in]	type		The type of the occurred event, one of listed
 * in #map_event_type_e
 * @param[in]	cb_data		The event data
 * @param[in]	user_data	The user data pointer registered in
 * map_view_set_event_callback()
 *
 * @pre map_view_set_event_callback() sets the callback.
 * @pre map_view_set_center(),
 * map_view_set_zoom_level(),
 * map_view_set_orientation(),
 * invoke the callback with the type #MAP_EVENT_ACTION.
 * @pre User gestures, such as tap, double tap, map rotate on long press invoke
 * the callback with the type #MAP_EVENT_GESTURE.
 * @pre map_view_redraw() invoke the callback with the type #MAP_EVENT_READY.
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
					LOG("MAP_GESTURE_DOUBLE_TAP");
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
 * @see map_view_set_event_callback()
 * @see map_view_set_center()
 * @see map_view_set_zoom_level()
 * @see map_view_set_orientation()
 * @see map_view_redraw()
 */
typedef void(*map_view_on_event_cb) (maps_error_e result,
				     const map_event_type_e type,
				     map_event_data_h event_data,
				     void *user_data);

/* ----------------------CREATE AND DESTROY-----------------------------------*/

/**
 * @brief	Creates the Map View and link it to the instance of
 * Maps Service.
 * @details This function creates a new Map View widget, allocate all needed
 * resources and issues its handle.
 * \n The newly created Map View is linked to the specified Maps Service.
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

		// Create an instance of Maps Service
		error = maps_service_create("Maps Provider", &maps);
		if(error != MAPS_ERROR_NONE)
			break;

		// Set the provider security key
		error = maps_service_set_provider_key(maps, "provider_key");
		if(error != MAPS_ERROR_NONE)
			break;

		// Create a Map View
		error = map_view_create(maps, map_view_place_holder, &view);
		if(error != MAPS_ERROR_NONE)
			break;

		// Work with Map View ...

	} while(false);

	// Release the Map View, Maps Service and all used resources
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
 * @brief	Destroys the Map View.
 * @details This function destroys the Map View widget, releases its handle and
 * all allocated resources, unlinks the view from the instance of Maps Service.
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
 * using current zoom and orientation
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	coordinates	The geographical coordinates of the position to
 * be centered on the Map View
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

// Center a Map View on specified coordinates
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
 * @brief	Get the central coordinates of a Map.
 * @details This function gets the current central coordinates of a map.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	coordinates	The pointer to #maps_coordinates_h in which to
 * store the geographical coordinates of the central position of the Map
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

// Get the central coordinates of the Map
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
 * @brief	Sets zooms level of the Map.
 * @details This function sets the integer zoom level of the map.
 * \n If the specified zoom level exceeds the Maps Provider allowed zoom range,
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

// Set a specified zoom level for the Map View
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
 * @brief	Gets zooms level of the Map.
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

// Get the current zoom level of the Map
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
 * @brief	Gets the minimal zooms level of the Map.
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

// Get the minimally allowed zoom level of the Map
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
 * @brief	Gets the maximal zooms level of the Map.
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

// Get the maximally allowed zoom level of the Map
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
 * @brief	Set the map orientation on the Map View.
 * @details This function sets the rotation angle of the Map on the Map View.
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

// Set a specified orientation of the Map
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
 * @details This function gets the current map rotation angle on the Map View.
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

// Get the current orientation of the Map
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
 * @brief	Sets Map View type.
 * @details This function switches the Map View to a specified type, one of
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

// Set Satellite Map view theme
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
 * @brief	Gets Map View type.
 * @details This function gets the type of the given Map View.
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
 * @brief	Sets Map View language.
 * @details This function sets the language to the given Map View.
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
 * @brief	Gets Map View language.
 * @details This function gets the language set to the given Map View.
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
 * @brief	Gets the Map View panel.
 * @details This function gets the Map View panel as a pointer on Evas_Object.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	panel		The pointer to Evas_Object in which to store
 * the Map View panel
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see Evas_Object
 */
int map_view_get_panel(const map_view_h view, Evas_Object **panel);

/**
 * @brief	Sets geometry of map view port.
 * @details This function set the position and (rectangular) size of the given
 * Map View.
 * \n The position, naturally, will be relative to the top left corner of the
 * parent window.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	x		X screen coordinate to move the Map View to
 * @param[in]	y		Y screen coordinate to move the Map View to
 * @param[in]	width		The new width of Map View, in screen units
 * @param[in]	height		The new height of Map View, in screen units
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

// Set Map View geometry
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
 * given Map View.
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

// Obtain Map View geometry
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
 * @brief	Resizes the Map View.
 * @details This function changes the size of the given Map View.
 * @remarks Newly created map view panel has the size of its parent.
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

// Resize the Map View
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
 * @brief	Shows or hides the Map View.
 * @details This function changes the visibility of Map View.
 * on the screen.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	visible		The new visibility of the Map View
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

// Set Map View visible
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
 * @brief	Gets the Map View visibility.
 * @details This function retrieves whether or not the given Map View is
 * visible.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	visible		The pointer to a boolean in which to store the
 * visibility of the Map View
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

// Set Map View visible
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
 * @brief	Redraws the Map View.
 * @details This function forces the redraw routine of the Map View on the
 * screen.
 * @remarks Note that this function only marks the Map View as needed to be
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
 * @brief	Sets the map event callback.
 * @details This function sets the callback which will be invoked every time the
 * Map View processes the event over the map.
 * @since_tizen 3.0
 * @remarks To unregister the callback use map_view_unset_event_callback().
 * \n Only one callback may be registered for a each event type.
 *
 * @param[in]	view		The view handle
 * @param[in]	type		The type of event, one of listed in
 * #map_event_type_e
 * @param[in]	callback	The callback, matching the
 * map_view_on_event_cb() prototype
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

// Following is a callback of Map View events, such as:
// - user's gestures: tap, long press, double tap, etc.
// - view actions: map zooming, rotating, etc.
// - view ready
static void my_map_view_event_cb(maps_error_e result,
				 const map_event_type_e type,
				 map_event_data_h event_data,
				 void *user_data)
{
	if ((result != MAPS_ERROR_NONE) || !event_data) {
		// Handle the error
	}

	// Handle the event...

	// Release the event data
	map_event_data_destroy(event_data);
}

// Register all Map View event callbacks
static int register_event_callbacks()
{
	int error = MAPS_ERROR_NONE;

	do {
		// Register user gesture callback
		error = map_view_set_event_callback(view,
					MAP_EVENT_GESTURE,
					my_map_view_event_cb,
					NULL);
		if(error != MAPS_ERROR_NONE)
			break;

		// Register view action callback
		error = map_view_set_event_callback(view,
					MAP_EVENT_ACTION,
					my_map_view_event_cb,
					NULL);
		if(error != MAPS_ERROR_NONE)
			break;

		// Register view readiness callback
		error = map_view_set_event_callback(view,
					MAP_EVENT_READY,
					my_map_view_event_cb,
					NULL);
		if(error != MAPS_ERROR_NONE)
			break;

	} while(false);

	return error;
}
 * @endcode
 *
 * @see map_view_unset_event_callback()
 * @see #map_event_type_e
 * @see map_view_on_event_cb()
 * @see map_view_set_gesture_action()
 * @see map_view_set_gesture_enabled()
 * @see map_view_create()
 */
int map_view_set_event_callback(map_view_h view,
				const map_event_type_e type,
				map_view_on_event_cb callback,
				void *user_data);

/**
 * @brief	Unsets the map event callback.
 * @details This function unsets the callback which was assigned to the
 * map event.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	type		The type of event, one of listed in
 * #map_event_type_e
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @pre the callback for the event of @a view is registered using
 * map_view_set_event_callback()
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

// Unregister the Map View gesture event callback
static int unregister_event_callbacks()
{
	int error = MAPS_ERROR_NONE;

	do {
		error = map_view_unset_event_callback(view, MAP_EVENT_GESTURE);
		if(error != MAPS_ERROR_NONE)
			// Handle the error
		}

		error = map_view_unset_event_callback(view, MAP_EVENT_ACTION);
		if(error != MAPS_ERROR_NONE)
			// Handle the error
		}

		error = map_view_unset_event_callback(view, MAP_EVENT_READY);
		if(error != MAPS_ERROR_NONE)
			// Handle the error
		}

	} while(false);

	return error;
}
 * @endcode
 *
 * @see map_view_set_event_callback()
 * @see #map_event_type_e
 * @see map_view_on_event_cb()
 * @see map_view_get_gesture_action()
 * @see map_view_get_gesture_enabled()
 * @see map_view_create()
 */
int map_view_unset_event_callback(map_view_h view, const map_event_type_e type);

/**
 * @brief	Assigns the action to the user gesture.
 * @details This function sets the behaviour of Map View by assigning a type of
 * action, listed in #map_action_e, which should be taken in response on a
 * given user gesture, one of #map_gesture_e.
 * @remarks Note: not all actions can be assigned with a specified gesture.
 * @par Matching gestures to actions
 * | Gesture                         | Allowed Action                              |
 * | :------------------------------ | :------------------------------------------ |
 * | #MAP_GESTURE_PAN                | #MAP_ACTION_MOVE_CENTER                     |
 * | #MAP_GESTURE_FLICK              | #MAP_ACTION_MOVE_CENTER                     |
 * | #MAP_GESTURE_PINCH              | #MAP_ACTION_ZOOM                            |
 * | #MAP_GESTURE_TAP                | #MAP_ACTION_SET_CENTER, #MAP_ACTION_ZOOM |
 * | #MAP_GESTURE_DOUBLE_TAP         | #MAP_ACTION_ZOOM, #MAP_ACTION_SET_CENTER |
 * | #MAP_GESTURE_2_FINGER_TAP       | #MAP_ACTION_ZOOM                            |
 * | #MAP_GESTURE_ZOOM               | #MAP_ACTION_ZOOM                            |
 * | #MAP_GESTURE_SINGLE_FINGER_ZOOM | #MAP_ACTION_ZOOM                            |
 * | #MAP_GESTURE_ROTATE             | #MAP_ACTION_ROTATE                          |
 * | #MAP_GESTURE_PRESS              | #MAP_ACTION_SET_CENTER, #MAP_ACTION_ZOOM |
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
 * @details This function gets the behaviour of Map View: it retrieves the
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



#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_VIEW_H__*/
