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

#include <maps_service.h>
#include <Ecore_Evas.h>
#include <map_object.h>
#include <map_event_data.h>

/**
 *
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_VIEW_PANEL_MODULE Map Panel
 *
 * @file map_view.h
 * @brief This file contains the top level functions of Map View API
 *
 * @addtogroup CAPI_MAP_VIEW_PANEL_MODULE
 * @{
 * @brief This provides APIs related to operations with Map View
 * @details Supported Map View operations include:
 *  * Creating and destroying the Map View panel, moving and resizing it
 *  * Changing map zoom, orientation and tilt
 *  * Changing map properties: theme (day, night, satellite ,terrain),  mode
 *  (2D, 2.5D, 3D, etc.),  language (English, Russian, Chinese, Italian, German,
 *  Spanish, etc.)
 *  * Converting screen coordinates to geographical and vise versa
 *  * Assigning user gestures and corresponding actions over the map
 *  * Handling the Map View events (user gestures, map actions, visual object
 *  events, etc.)
 *  * Adding and removing basic visual objects (markers, polylines, polygons)
 *  .
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	The Map Visual Panel handle
 * @details The handle of Map View Panel instance.
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
typedef enum _map_view_typ_e {

	/** Indicates the day theme */
	MAP_VIEW_TYPE_DAY,

	/** Indicates the night theme */
	MAP_VIEW_TYPE_NIGHT,

	/** Indicates the public transit theme */
	MAP_VIEW_TYPE_PUBLIC_TRANSIT,

	/** Indicates the combination of the night and public transit theme */
	MAP_VIEW_TYPE_NIGHT_PUBLIC_TRANSIT,

	/** Indicates the satellite theme */
	MAP_VIEW_TYPE_SATELLITE,

	/** Indicates the hybrid theme which is the satellite and roads theme */
	MAP_VIEW_TYPE_HYBRID,

	/** Indicates the combination of the hybrid and public transit theme */
	MAP_VIEW_TYPE_HYBRID_PUBLIC_TRANSIT,

	/** Indicates the car navigation theme */
	MAP_VIEW_TYPE_CAR_NAVIGATION,

	/** Indicates the terrain theme */
	MAP_VIEW_TYPE_TERRAIN,

	/** Indicates the indoor (raster) theme */
	MAP_VIEW_TYPE_INDOOR
} map_view_type_e;

/**
 * @brief	Enumerations of Map View modes
 * @since_tizen 3.0
 */
typedef enum _map_view_mode_e {
	/** Indicates the regular 2D mode */
	MAP_VIEW_MODE_2D,

	/**
	 * Indicates the regular 2.5D mode with ghost-like see-through
	 * buildings that are not exactly 3D
	 */
	MAP_VIEW_MODE_2_5D,

	/** Indicates the photo-realistic 3D mode */
	MAP_VIEW_MODE_3D,

	/** Indicates the street view mode */
	MAP_VIEW_MODE_STREET_VIEW
} map_view_mode_e;

/**
 * @brief	Called when requesting the list of visual objects of the Map
 * View.
 * @details This callback is invoked while iterating through the list of
 * visual objects, added to the Map View.
 * @since_tizen 3.0
 * @remarks @a object must be released using maps_visual_object_destroy().
 * \n To use @a object outside this function, clone it with
 * maps_visual_object_clone().
 *
 * @param[in]	index		The current index of path point
 * @param[in]	total		The total amount of path points
 * @param[in]	object		The visual object
 * @param[in]	user_data	The user data pointer passed from
 * map_view_foreach_object()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre map_view_foreach_object() will invoke this callback.
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

static bool draw_map_object_cb(int index, int total,
			     map_object_h object,
			     void *user_data)
{
	if (!object)
		return false;

	bool visible = false;
	map_object_type_e type = MAP_OBJECT_UNKNOWN;
	int error = MAPS_ERROR_NONE;

	error = map_object_get_visible(object, &visible);
	if (error != MAPS_ERROR_NONE)
		return false;

	if (!visible)
		return true; // This object should not be drawn

	// Extract the type of the object
	error = map_object_get_type(object, &type);
	if (error != MAPS_ERROR_NONE)
		return false;

	// Dispatch the drawing routines
	switch(type) {
	case MAP_OBJECT_MARKER:
		__draw_marker(object);
		break;
	case MAP_OBJECT_POLYLINE:
		__draw_polyline(object);
		break;
	case MAP_OBJECT_POLYGON:
		__draw_polygon(object);
		break;
	case MAP_OBJECT_GROUP:
		__draw_group(object);
		break;
	case MAP_OBJECT_UNKNOWN:
	default:
		// Handle the error of unsupported Map View Object type
		break;
	}
	return true;
}
 * @endcode
 *
 * @see #map_object_h
 * @see map_view_foreach_object()
 */
typedef bool(*map_object_cb) (int index, int total,
			      map_object_h object,
			      void *user_data);

/**
 * @brief	Called when the map event occurs.
 * @details The Map View Panel invokes this callback when the map event occurs.
 * @since_tizen 3.0
 * @remarks @a event_data must be released using map_event_data_destroy().
 * \n To use @a event_data outside this function, clone it with
 * map_event_data_clone().
 *
 * @param[in]	result		The result of request
 * @param[in]	type		The new type of callback, listed
 * in #map_event_type_e
 * @param[in]	cb_data		The event data
 * @param[in]	user_data	The user data pointer registered in
 * map_view_set_event_callback()
 *
 * @pre map_view_set_event_callback() sets the callback.
 * @pre map_view_set_center(),
 * map_view_set_zoom_level(),
 * map_view_set_zoom_factor(),
 * map_view_set_orientation(),
 * map_view_set_tilt()
 * invoke the callback with the type #MAP_EVENT_ACTION.
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
		case MAP_EVENT_OBJECT:
			LOG("MAP_EVENT_GESTURE or MAP_EVENT_OBJECT");
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
				case MAP_ACTION_TILT:
					LOG("MAP_ACTION_TILT");
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
 * @see map_view_set_zoom_factor()
 * @see map_view_set_orientation()
 * @see map_view_set_tilt()
 * @see map_view_redraw()
 */
typedef void(*map_view_on_event_cb) (maps_error_e result,
				     const map_event_type_e type,
				     map_event_data_h event_data,
				     void *user_data);

/* ----------------------CREATE AND DESTROY-----------------------------------*/

/**
 * @brief	Creates the Maps View panel and link it to the instance of
 * Maps Service.
 * @details This function creates a new maps view panel, allocate all needed
 * resources and issues its handle. The newly created Map View is linked to the
 * specified Maps Service.
 * \n The view by default is resized to fill whole parent area.
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
 */
int map_view_create(const maps_service_h maps,
		    Evas_Object *parent,
		    map_view_h *view);

/**
 * @brief	Destroys the Map View panel.
 * @details This function destroys the Map View panel, releases its handle and
 * all its resources, unlinks the panel from the instance of Maps Service.
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
 * @details This function centers the map on a given geographical coordinates,
 * using current zoom, orientation and tilt.
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
 *
 * @pre @a view is created using map_view_create().
 * @pre @a coordinates is created using maps_coordinates_create().
 * @post This function invokes map_view_on_event_cb() with the event type
 * #MAP_EVENT_ACTION and action type #MAP_ACTION_SET_CENTER.
 * \n Geographical coordinates of new center will be accessible via
 * map_event_data_get_center().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
maps_coordinates_h coordinates = NULL;

// Obtain a geographical position to be focused
error = maps_coordinates_create(39.929386, 23.696087, &coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Center a Map View on a specified coordinates
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
 * @see map_view_set_zoom_factor()
 * @see map_view_set_orientation()
 * @see map_view_set_tilt()
 * @see map_view_create()
 */
int map_view_set_center(const map_view_h view,
			const maps_coordinates_h coordinates);
/**
 * @brief	Get the central coordinates of a Map.
 * @details This function gets the central coordinates of a Map.
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

// Fetch the central coordinates on the Map View
error = map_view_get_center(view, &coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Do something with obtained coordinates...
double latitude = .0;
double longitude = .0;
error = maps_coordinates_get_latlon(coordinates, &latitude, &longitude);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Release coordinates handle
maps_coordinates_destroy(coordinates);
 * @endcode
 *
 * @see map_view_set_center()
 * @see map_view_get_zoom_level()
 * @see map_view_get_zoom_factor()
 * @see map_view_get_orientation()
 * @see map_view_get_tilt()
 * @see map_view_create()
 */
int map_view_get_center(const map_view_h view,
			maps_coordinates_h *coordinates);

/**
 * @brief	Sets zooms level of the Map.
 * @details This function sets the integer zoom level of the Map
 * on the Map View.
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
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_event_cb() with the event type
 * #MAP_EVENT_ACTION and action type #MAP_ACTION_ZOOM.
 * \n The value of new zoom level will be accessible via
 * map_event_data_get_zoom_factor().
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
 * @see map_view_set_zoom_factor()
 * @see map_view_set_orientation()
 * @see map_view_set_tilt()
 * @see map_view_on_event_cb()
 * @see map_view_set_event_callback()
 * @see map_view_create()
 */
int map_view_set_zoom_level(map_view_h view, const int level);

/**
 * @brief	Gets zooms level of the Map.
 * @details This function gets the integer zoom level of the Map
 * on the Map View.
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

// Fetch the current zoom level of the Map View
error = map_view_get_zoom_level(view, &zoom_level);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_zoom_level()
 * @see map_view_get_min_zoom_level()
 * @see map_view_get_max_zoom_level()
 * @see map_view_get_zoom_factor()
 * @see map_view_create()
 */
int map_view_get_zoom_level(const map_view_h view, int *level);

/**
 * @brief	Gets the minimal zooms level of the Map.
 * @details This function gets the minimally allowed zoom level of the Map
 * on the Map View.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[out]	min_zoom_level	The pointer to an integer in which to store the
 * minimally available zoom level
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
int min_zoom_level = 0;

// Fetch the minimally allowed zoom level of the Map View
error = map_view_get_min_zoom_level(view, &min_zoom_level);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_max_zoom_level()
 * @see map_view_get_zoom_level()
 * @see map_view_get_zoom_factor()
 * @see map_view_set_zoom_level()
 * @see map_view_create()
 */
int map_view_get_min_zoom_level(const map_view_h view, int *min_zoom_level);

/**
 * @brief	Gets the maximal zooms level of the Map.
 * @details This function gets the maximally allowed zoom level of the Map
 * on the Map View.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[out]	max_zoom_level	The pointer to an integer in which to store the
 * maximally available zoom level
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
int max_zoom_level = 0;

// Fetch the maximally allowed zoom level of the Map View
error = map_view_get_max_zoom_level(view, &max_zoom_level);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_min_zoom_level()
 * @see map_view_get_zoom_level()
 * @see map_view_get_zoom_factor()
 * @see map_view_set_zoom_level()
 * @see map_view_create()
 */
int map_view_get_max_zoom_level(const map_view_h view, int *max_zoom_level);

/**
 * @brief	Zooms the map on the view.
 * @details This function zooms the map on the Map View with real value of zoom
 * factor.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[out]	zoom_factor	The new zoom factor
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_event_cb() with the event type
 * #MAP_EVENT_ACTION and action type #MAP_ACTION_ZOOM.
 * \n The value of new zoom factor will be accessible via
 * map_event_data_get_zoom_factor().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set a specified zoom factor for the Map View
error = map_view_set_zoom_factor(view, 8.5);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_zoom_factor()
 * @see map_view_set_zoom_level()
 * @see map_view_get_min_zoom_level()
 * @see map_view_get_max_zoom_level()
 * @see map_view_on_event_cb()
 * @see map_view_set_event_callback()
 * @see map_view_create()
*/
int map_view_set_zoom_factor(const map_view_h view, const double zoom_factor);

/**
 * @brief	Gets the map zoom factor.
 * @details This function gets the current zoom factor of the Map on the Map
 * View.
 * @since_tizen 3.0
 * @remarks zoom @a factor can be obtained also in map_view_on_event_cb()
 * callback, assigned using map_view_set_event_callback() with the event
 * type #MAP_EVENT_ACTION specified.
 *
 * @param[in]	view		The view handle
 * @param[out]	zoom_factor	The pointer to a double in which to store the
 * current zoom factor
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
double current_zoom_factor = .0;

// Fetch the current zoom factor of the Map View
error = map_view_get_zoom_factor(view, &zoom_factor);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_zoom_factor()
 * @see map_view_set_event_callback()
 * @see map_view_on_zoom_cb()
 * @see map_view_create()
 */
int map_view_get_zoom_factor(const map_view_h view, double *zoom_factor);

/**
 * @brief	Set the map orientation on the Map View.
 * @details This function sets the rotation angle of the Map on the Map View.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[out]	angle		The new rotation angle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_event_cb() with the event type
 * #MAP_EVENT_ACTION and action type #MAP_ACTION_ROTATE.
 * \n The value of new rotation angle will be accessible via
 * map_event_data_get_rotation_angle().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set a specified orientation of the Map View
error = map_view_set_orientation(view, 60.);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
*
 * @see map_view_get_orientation()
 * @see map_view_set_zoom_level()
 * @see map_view_set_zoom_factor()
 * @see map_view_set_tilt()
 * @see map_view_on_event_cb()
 * @see map_view_set_event_callback()
 * @see map_view_create()
 */
int map_view_set_orientation(const map_view_h view, const double angle);

/**
 * @brief	Gets the map orientation.
 * @details This function gets the current Map rotation angle on the  Map View.
 * @since_tizen 3.0
 * @remarks @a rotation_angel can be obtained also in map_view_on_event_cb()
 * callback, assigned using map_view_set_event_callback() with the event
 * type #MAP_EVENT_ACTION specified.
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

// Fetch the current orientation of the Map View
error = map_view_get_zoom_level(view, &orientation);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_orientation()
 * @see map_view_set_event_callback()
 * @see map_view_on_event_cb()
 * @see map_view_create()
 */
int map_view_get_orientation(const map_view_h view, double *rotation_angle);

/**
 * @brief	Sets tilt of Map view port.
 * @details This function sets the incline of Map view port.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	tilt		The tilting degree
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_event_cb() with the event type
 * #MAP_EVENT_ACTION and action type #MAP_ACTION_TILT.
 * \n The value of new tilt will be accessible via
 * map_event_data_get_tilt().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set a specified incline of the Map View
error = map_view_set_tilt(view, 15.);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_tilt()
 * @see map_view_get_orientation()
 * @see map_view_get_zoom_level()
 * @see map_view_get_min_tilt()
 * @see map_view_get_max_tilt()
 * @see map_view_on_event_cb()
 * @see map_view_set_event_callback()
 * @see map_view_create()
 */
int map_view_set_tilt(map_view_h view, const double tilt);

/**
 * @brief	Gets tilt of Map.
 * @details This function gets the incline of Map view port.
 * @since_tizen 3.0
 * @remarks @a tilt can be obtained also in map_view_on_event_cb()
 * callback, assigned using map_view_set_event_callback() with the event
 * type #MAP_EVENT_ACTION specified.
 *
 * @param[in]	view		The view handle
 * @param[out]	tilt		The pointer to a double in which to store the
 * current tilting degree
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
double current_tilt = .0;

// Fetch the current incline of the Map View
error = map_view_get_tilt(view, &tilt);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_tilt()
 * @see map_view_get_min_tilt()
 * @see map_view_get_max_tilt()
 * @see map_view_set_event_callback()
 * @see map_view_on_event_cb()
 * @see map_view_create()
 */
int map_view_get_tilt(const map_view_h view, double *tilt);


/**
 * @brief	Gets the minimal tilt of the Map.
 * @details This function gets the minimally allowed tilt of the Map
 * on the Map View.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[out]	min_tilt	The pointer to a double in which to store the
 * minimally available tilting degree
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
double min_tilt = .0;

// Fetch the minimally allowed tilt of the Map View
error = map_view_get_min_tilt(view, &min_tilt);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_max_tilt()
 * @see map_view_get_tilt()
 * @see map_view_create()
 */
int map_view_get_min_tilt(const map_view_h view, int *min_tilt);

/**
 * @brief	Gets the maximal tilt of the Map.
 * @details This function gets the maximally allowed tilt of the Map
 * on the Map View.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[out]	max_tilt	The pointer to a double in which to store the
 * maximally available tilting degree
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_zoom_cb().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
double min_tilt = .0;

// Fetch the maximally allowed tilt of the Map View
error = map_view_get_max_tilt(view, &min_tilt);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_min_tilt()
 * @see map_view_get_tilt()
 * @see map_view_create()
 */
int map_view_get_max_tilt(const map_view_h view, int *max_tilt);


/*----------------------SCREEN <--> GEOGRAPHY---------------------------------*/

/**
 * @brief	Converts screen coordinates to the geographical coordinates.
 * @details This function converts screen coordinates to the geographical
 * coordinates accordingly to the current Maps settings.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
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
 * @see map_view_create()
 */
int map_view_screen_to_geography(const map_view_h view,
				 const int x,
				 const int y,
				 maps_coordinates_h *coordinates);

/**
 * @brief	Converts geographical coordinates to the screen coordinates.
 * @details This function converts geographical coordinates to the screen
 * coordinates accordingly to the current Maps settings.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
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
 * @pre @a coordinates is created using maps_coordinates_create().
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
 * @see map_view_create()
 */
int map_view_geography_to_screen(const map_view_h view,
				 const maps_coordinates_h coordinates,
				 int *x,
				 int *y);


/* --------------------MAPS VIEW PREFERENCES----------------------------------*/


/**
 * @brief	Sets Map View type.
 * @details This function sets to the given Map View a type such as normal,
 * satellite, terrain, hybrid.
 * \n The type options are defined in the #map_view_type_e enumeration.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	type		The view type, listed in #map_view_type_e
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

// Set Satellite Map view theme
error = map_view_set_type(view, MAP_VIEW_TYPE_SATELLITE);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_view_type_e
 * @see map_view_get_type()
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
 * @see map_view_create()
 */
int map_view_get_type(const map_view_h view, map_view_type_e *type);

/**
 * @brief	Sets the maps view mode.
 * @details This function sets the current mode of the maps view.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	mode		The maps mode, listed in #map_view_mode_e
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

// Set the conventional 2D ap view mode
error = map_view_set_mode(view, MAP_VIEW_MODE_2D);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_view_mode_e
 * @see map_view_get_mode()
 * @see map_view_create()
 */
int map_view_set_mode(map_view_h view, const map_view_mode_e mode);

/**
 * @brief	Gets the maps view mode.
 * @details This function gets the current mode of the maps view.
 * @since_tizen 3.0
 *
 * @param[in]	view	The view handle
 * @param[out]	mode	The pointer to a #map_view_mode_e in which to store
 * current maps mode
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
map_view_mode_e mode = MAP_VIEW_MODE_2D;

// Obtain current view mode
error = map_view_get_mode(view, &mode);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_view_mode_e
 * @see map_view_set_view_mode()
 * @see map_view_create()
 */
int map_view_get_mode(map_view_h view, map_view_mode_e* mode);

/**
 * @brief	Sets Map View language.
 * @details This function sets the language to the given Map View.
 * \n Note that display language is different from places and route language.
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
 * @see map_view_create()
 */
int map_view_set_language(map_view_h view, const char *language);

/**
 * @brief	Gets Map View language.
 * @details This function gets the language to the given Map View.
 * \n Note that display language is different from places and route language.
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
 * @see map_view_create()
 */
int map_view_get_language(const map_view_h view, char **language);


/* --------------------MAPS PANEL MANIPULATIONS-------------------------------*/


/**
 * @brief	Gets the Map View panel.
 * @details This function gets the maps view panel as a pointer on Evas_Object.
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
 * map view panel.
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
 * given map view panel.
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
 * @see map_view_create()
 */
int map_view_get_geometry(const map_view_h view,
			   int *x,
			   int *y,
			   int *width,
			   int *height);

/**
 * @brief	Resizes the Map View.
 * @details This function changes the size of the given map view panel.
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
 * @see map_view_create()
 */
int map_view_resize(const map_view_h view, const int width, const int height);

/**
 * @brief	Shows the Map View.
 * @details This function changes the visibility of Map View panel.
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
 *
 * @pre @a view is created using map_view_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set Map View visible
error = map_view_set_visible(view, true);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_visible()
 * @see map_view_create()
 */
int map_view_set_visible(const map_view_h view, const bool visible);

/**
 * @brief	Gets the Map View panel visibility.
 * @details This function retrieves whether or not the given map view panel is
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
error = map_view_get_visible(view, &visible);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_visible()
 * @see map_view_create()
 */
int map_view_get_visible(const map_view_h view, bool *visible);

/**
 * @brief	Redraws the Map View panel.
 * @details This function forces the redraw routine of the Map View panel on
 * the screen.
 * @remarks Note that this function only marks the Map View as needed to be
 * redrawn. Actual redraw is happening when the OS drawing system is ready for
 * it.
 * \n When redraw is finished, the callback map_view_on_event_cb() is invoked
 * with the type parameter #MAP_EVENT_READY.
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
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 */
int map_view_redraw(const map_view_h view);


/* ---------------------USER CONTROL------------------------------------------*/


/**
 * @brief	Sets the map event callback.
 * @details This function sets the callback which will be invoked every time the
 * Map View Panel detects the event over the map.
 * @since_tizen 3.0
 * @remarks To unregister the callback use map_view_unset_event_callback.
 * \n Only one callback may be registered for a given event type.
 *
 * @param[in]	view		The view handle
 * @param[in]	type		The type of event, listed in #map_event_type_e
 * @param[in]	callback	The callback, matching the
 * map_view_on_event_cb() signature
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

// Define callback to react on Map View events, such as
// - user's gestures: tap, long press, double tap, etc.
// - actions: map zooming, rotating, redrawing, etc.
// - object events: object tap, object long press, etc.
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

// Register the Map View event callback
static int register_event_callback()
{
	const int error = map_view_set_event_callback(view,
						      MAP_EVENT_GESTURE,
						      my_map_view_event_cb,
		      				      NULL);

	if(error != MAPS_ERROR_NONE) {
		// Handle the error
	}

	return error;
}

// Unregister the Map View event callback
static int unregister_event_callback()
{
	const int error = map_view_unset_event_callback(view,
							MAP_EVENT_GESTURE);

	if(error != MAPS_ERROR_NONE) {
		// Handle the error
	}

	return error;
}
 * @endcode
 *
 * #map_event_type_e
 * @see map_view_on_event_cb()
 * @see map_view_unset_event_callback()
 * @see map_view_create()
 */
int map_view_set_event_callback(map_view_h view,
				const map_event_type_e type,
				map_view_on_event_cb callback,
				void *user_data);

/**
 * @brief	Unsets the map event callback.
 * @details This function unsets the callback which was assigned to be invoked
 * every time the Map View Panel detects the event over the map.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	type		The type of event, listed in #map_event_type_e
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @pre the callback for the event of @a view is registered using
 * map_view_set_event_callback()
 *
 * @see map_view_set_event_callback()
 * @see map_view_create()
 */
int map_view_unset_event_callback(map_view_h view, const map_event_type_e type);

/**
 * @brief	Assigns the action to the user gesture.
 * @details This function sets the behaviour of Map View by assigning a type of
 * action which should be taken in response on a given user gesture.
 * @since_tizen 3.0
 * @remarks This function enables the @a gesture even if it was disabled
 * previously with map_view_set_gesture_enabled()
 *
 * @param[in]	view		The view handle
 * @param[in]	gesture		The user gesture, listed in #map_gesture_e
 * @param[in]	action		The action which should be taken, on of
 * #map_action_e
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
 * @details This function gets the behaviour of Map View by retrieving the
 * action, assigned as a response on a given user gesture.
 * @since_tizen 3.0
 * @remarks This function enables the @a gesture even if it was disabled
 * previously with map_view_set_gesture_enabled()
 *
 * @param[in]	view		The view handle
 * @param[in]	gesture		The user gesture
 * @param[out]	action		The pointer to a #map_action_e action in which
 * to store the assigned reaction on the user gesture
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @pre @a action is set by default or assigned using
 * map_view_set_gesture_action().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
map_action_e action = MAP_ACTION_NONE;

// Obtain the action, assigned to the tap gesture
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
 * @details This function allows or blocks any reaction on a given user gesture.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	gesture		The user gesture
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
 * @param[in]	gesture		The user gesture
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


/* ---------------------VISUAL OBJECTS ON THE MAP-----------------------------*/

/**
 * @brief	Adds a visual object on the map.
 * @details This function adds a visual object on the map.
 * @since_tizen 3.0
 * @remarks The @a object handle will be released automatically when the view is
 * destroyed in the map_view_destroy().
 *
 * @param[in]	view		The view handle
 * @param[in]	object		The handle of the object to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @pre @a object is created using #map_object_create_marker(),
 * map_object_create_polygon(), map_object_create_polyline() or
 * map_object_create_group()
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;
map_object_h marker = NULL;
maps_coordinates_h coordinates = NULL;


// Obtain a geographical position to put a marker
error = maps_coordinates_create(39.929386, 23.696087, &coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the create coordinates error
}

// Create an instance of a marker
error = map_object_create_marker(coordinates,
				"resources/marker_icon.png",
				MAP_MARKER_POI,
				&marker);
if (error != MAPS_ERROR_NONE) {
	// Handle the create marker error
}

// Add marker on the map
error = map_view_add_object(maps_view, marker);
if (error != MAPS_ERROR_NONE) {
	// Handle the add marker error
}

// Release coordinates
maps_coordinates_destroy(coordinates);
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polygon()
 * @see map_object_create_polyline()
 * @see map_object_create_group()
 * @see map_view_remove_object()
 * @see map_view_remove_all_objects()
 * @see map_view_create()
 */
int map_view_add_object(map_view_h view, map_object_h object);


/**
 * @brief	Removes a visual object from the map.
 * @details This function removes a visual object from the map.
 * @since_tizen 3.0
 * @remarks The @a object handle will be released automatically by the Map View.
 *
 * @param[in]	view		The view handle
 * @param[in]	object		The object to remove
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @pre @a object is added using map_view_add_object().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead
// map_object_h marker is created ahead

int error = MAPS_ERROR_NONE;

// Removing a visual object from the map
error = map_view_remove_object(view, marker);
if (error != MAPS_ERROR_NONE) {
	// Handle the add marker error
}
 * @endcode
 *
 * @see map_view_add_object()
 * @see map_view_remove_all_objects()
 * @see #map_object_h
 * @see map_view_create()
 */
int map_view_remove_object(map_view_h view, const map_object_h object);

/**
 * @brief	Removes all visual objects from the map.
 * @details This function removes all visual object from the map.
 * @since_tizen 3.0
 * @remarks All object handles will be released automatically by the Map View.
 *
 * @param[in]	view		The view handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @pre objects are added using map_view_add_object().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Removing a visual object from the map
error = map_view_remove_all_objects(view);
if (error != MAPS_ERROR_NONE) {
	// Handle the add marker error
}
 * @endcode
 *
 * @see #map_object_h
 * @see map_view_add_object()
 * @see map_view_remove_all_objects()
 * @see map_view_create()
 */
int map_view_remove_all_objects(map_view_h view);


/**
 * @brief	Retrieves all visual objects on the map.
 * @details This function retrieves all visual objects, previously added to the
 * map.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_object_cb().
 *
 * @param[in]	view		The view handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data pointer to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a view is created using map_view_create().
 * @pre objects are added using map_view_add_object().
 * @post This function invokes map_object_cb() repeatedly to retrieve each
 * visual object.
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

static bool draw_map_object_cb(int index, int total,
			     map_object_h object,
			     void *user_data)
{
	if (!object)
		return false;

	bool visible = false;
	map_object_type_e type = MAP_OBJECT_UNKNOWN;
	int error = MAPS_ERROR_NONE;

	error = map_object_get_visible(object, &visible);
	if (error != MAPS_ERROR_NONE)
		return false;

	if (!visible)
		return true; // This object should not be drawn

	// Extract the type of the object
	error = map_object_get_type(object, &type);
	if (error != MAPS_ERROR_NONE)
		return false;

	// Dispatch the drawing routines
	switch(type) {
	case MAP_OBJECT_MARKER:
		__draw_marker(object);
		break;
	case MAP_OBJECT_POLYLINE:
		__draw_polyline(object);
		break;
	case MAP_OBJECT_POLYGON:
		__draw_polygon(object);
		break;
	case MAP_OBJECT_GROUP:
		__draw_group(object);
		break;
	case MAP_OBJECT_UNKNOWN:
	default:
		// Handle the error of unsupported Map View Object type
		break;
	}
	return true;
}

static void iterate_through_objects()
{
	const int error = map_view_foreach_object(view,
						  draw_map_object_cb,
						  NULL);
	if (error != MAPS_ERROR_NONE) {
		// Handle the error
	}
	return error;
}
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_cb()
 * @see map_view_add_object()
 * @see map_view_remove_object()
 * @see map_view_remove_all_objects()
 * @see map_view_create()
 */
int map_view_foreach_object(const map_view_h view,
			    map_object_cb callback,
			    void *user_data);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_VIEW_H__*/
