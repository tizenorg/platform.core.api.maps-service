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
 *  * Changing map properties: theme (day, satellite ,terrain),
 *  language (English, Russian, Chinese, Italian, German, Spanish, etc.)
 *  * Converting screen coordinates to geographical and vise versa
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

	/** Indicates the satellite theme */
	MAP_VIEW_TYPE_SATELLITE,

	/** Indicates the hybrid theme which is the satellite and roads theme */
	MAP_VIEW_TYPE_HYBRID,

	/** Indicates the terrain theme */
	MAP_VIEW_TYPE_TERRAIN,

} map_view_type_e;

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

	error = map_object_get_visibility(object, &visible);
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
 * @see map_view_set_orientation()
 * @see map_view_set_tilt()
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
 * @see map_view_set_zoom_level()
 * @see map_view_create()
 */
int map_view_get_max_zoom_level(const map_view_h view, int *max_zoom_level);

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
 * @see map_view_set_event_callback()
 * @see map_view_create()
 */
int map_view_set_orientation(const map_view_h view, const double angle);

/**
 * @brief	Gets the map orientation.
 * @details This function gets the current Map rotation angle on the  Map View.
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

// Fetch the current orientation of the Map View
error = map_view_get_zoom_level(view, &orientation);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_orientation()
 * @see map_view_set_event_callback()
 * @see map_view_create()
 */
int map_view_get_orientation(const map_view_h view, double *rotation_angle);


/*----------------------SCREEN <--> GEOGRAPHY---------------------------------*/

/**
 * @brief	Converts screen coordinates to the geographical coordinates.
 * @details This function converts screen coordinates to the geographical
 * coordinates accordingly to the current Maps settings.
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
 * @brief	Shows or Hides the Map View.
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
error = map_view_set_visibility(view, true);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_get_visibility()
 * @see map_view_create()
 */
int map_view_set_visibility(const map_view_h view, const bool visible);

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
error = map_view_get_visibility(view, &visible);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_view_set_visibility()
 * @see map_view_create()
 */
int map_view_get_visibility(const map_view_h view, bool *visible);

/**
 * @brief	Redraws the Map View panel.
 * @details This function forces the redraw routine of the Map View panel on
 * the screen.
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
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 */
int map_view_redraw(const map_view_h view);


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

	error = map_object_get_visibility(object, &visible);
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
