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
 * @defgroup	CAPI_MAP_VIEW_PANEL_MODULE Map View
 *
 * @file map_view.h
 * @brief This file contains the top level functions of map view API
 *
 * @addtogroup CAPI_MAP_VIEW_PANEL_MODULE
 * @{
 * @brief This provides map view APIs related to operations with map view
 * @details Supported map view API features include:
 *  * Creating and destroying the map view port, moving and resizing it
 *  * Changing map zoom, and orientation
 *  * Changing map properties: theme (day, satellite ,terrain),
 *  language (English, Russian, Chinese, Italian, German, Spanish, etc.)
 *  * Converting screen coordinates to geographical and vise versa
 *  * Capturing snapshot with current map view image
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
typedef enum _map_type_e {
	/** Indicates no base map */
	MAP_TYPE_NONE,

	/** Indicates the normal streep map */
	MAP_TYPE_NORMAL,

	/** Indicates the satellite map */
	MAP_TYPE_SATELLITE,

	/** Indicates the terrain map */
	MAP_TYPE_TERRAIN,

	/** Indicates the hybrid theme which is the satellite and normal street map */
	MAP_TYPE_HYBRID,
} map_type_e;

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
 * invoke the callback with the type #MAP_EVENT_ACTION.
 * @pre map_view_redraw() invoke the callback with the type #MAP_EVENT_READY.
 *
 * @see map_view_set_event_callback()
 * @see map_view_set_center()
 * @see map_view_set_zoom_level()
 * @see map_view_set_zoom_factor()
 * @see map_view_set_orientation()
 * @see map_view_redraw()
 */
typedef void(*map_view_on_event_cb) (maps_error_e result,
				     const map_event_type_e type,
				     map_event_data_h event_data,
				     void *user_data);

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
 * map_view_set_map_type(), map_view_set_geolocation() and map_view_set_visibility()
 * respectively.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks The @a view must be released using map_view_destroy().
 *
 * @param[in]	maps		The maps service handle
 * @param[in]	obj		The image object
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
 * @see map_view_destroy()
 * @see maps_service_create()
 * @see map_view_set_center()
 * @see map_view_set_zoom_level()
 * @see map_view_set_orientation()
 * @see map_view_set_map_type()
 * @see map_view_set_geolocation()
 * @see map_view_set_visibility()
 */
int map_view_create(const maps_service_h maps, Evas_Image *obj, map_view_h *view);

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
 * @see map_view_get_center()
 * @see map_view_set_zoom_level()
 * @see map_view_set_orientation()
 * @see maps_coordinates_create()
 * @see map_view_create()
 */
int map_view_set_center(const map_view_h view,
			const maps_coordinates_h coordinates);

/**
 * @brief	Gets the central coordinates of a map.
 * @details This function gets the current central coordinates of a map.
 * @since_tizen 3.0
 * @remarks The @a coordinates must be released using
 * maps_coordinates_destroy().
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
 *
 * @param[in]	view		The view handle
 * @param[out]	min_zoom_level	The pointer to an integer in which to store the
 * minimally allowed zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 *
 * @pre @a view is created using map_view_create().
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
 *
 * @param[in]	view		The view handle
 * @param[out]	max_zoom_level	The pointer to an integer in which to store the
 * maximally allowed zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_get_min_zoom_level()
 * @see map_view_get_zoom_level()
 * @see map_view_set_zoom_level()
 * @see map_view_create()
 */
int map_view_get_max_zoom_level(const map_view_h view, int *max_zoom_level);

/**
 * @brief	Sets the map orientation on the map view.
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
 * @param[in]	angle		The new orientation angle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
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
 * @remarks The @a coordinates must be released using
 * maps_coordinates_destroy().
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
 * @see map_view_geolocation_to_screen()
 * @see #maps_coordinates_h
 * @see maps_coordinates_destroy()
 * @see map_view_create()
 */
int map_view_screen_to_geolocation(const map_view_h view,
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
 * @pre @a coordinates is created using maps_coordinates_create().
 *
 * @see map_view_screen_to_geolocation()
 * @see #maps_coordinates_h
 * @see maps_coordinates_create()
 * @see map_view_create()
 */
int map_view_geolocation_to_screen(const map_view_h view,
				   const maps_coordinates_h coordinates,
				   int *x,
				   int *y);


/* --------------------MAPS VIEW PREFERENCES----------------------------------*/


/**
 * @brief	Sets map view type.
 * @details This function switches the map view to a specified type, one of
 * listed in #map_type_e enumeration.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	type		The view type, one of listed in #map_type_e
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
 * @see #map_type_e
 * @see map_view_get_map_type()
 * @see map_view_set_language()
 * @see map_view_create()
 */
int map_view_set_map_type(map_view_h view, const map_type_e type);

/**
 * @brief	Gets map view type.
 * @details This function gets the type of the given map view.
 * \n The type options are defined in the #map_type_e enumeration.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	type		The pointer to a #map_type_e in which to
 * store current view type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see #map_type_e
 * @see map_view_set_map_type()
 * @see map_view_get_language()
 * @see map_view_create()
 */
int map_view_get_map_type(const map_view_h view, map_type_e *type);

/**
 * @brief	Enables or disables 3D buildings .
 * @details This function enables or disables 3D buildings on the maps view.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	enable		The enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_get_buildings_enabled()
 */
int map_view_set_buildings_enabled(map_view_h view, const bool enable);

/**
 * @brief	Gets whether 3D buildings are enabled or not.
 * @details This function gets whether 3D buildings are enabled or not.
 * @since_tizen 3.0
 *
 * @param[in]	view	The view handle
 * @param[out]	enable	The pointer to a boolean in which to store the enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_set_view_mode()
 * @see map_view_create()
 * @see map_view_set_buildings_enabled()
 */
int map_view_get_buildings_enabled(map_view_h view, bool *enable);

/**
 * @brief	Turns the traffic layer on or off.
 * @details This function turns the traffic layer on or off.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	enable		The enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 */
int map_view_set_traffic_enabled(map_view_h view, const bool enable);

/**
 * @brief	Gets whether the map is drawing traffic data.
 * @details This function gets whether the map is drawing traffic data or not.
 * @since_tizen 3.0
 *
 * @param[in]	view	The view handle
 * @param[out]	enable	The pointer to a boolean in which to store the enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_set_view_mode()
 * @see map_view_create()
 */
int map_view_get_traffic_enabled(map_view_h view, bool *enable);

/**
 * @brief	Turns the public transit layer on or off.
 * @details This function turns the public transit layer on or off.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	enable		The enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 */
int map_view_set_public_transit_enabled(map_view_h view, const bool enable);

/**
 * @brief	Gets whether the map is drawing public transit.
 * @details This function gets whether the map is drawing public transit.
 * @since_tizen 3.0
 *
 * @param[in]	view	The view handle
 * @param[out]	enable	The pointer to a boolean in which to store the enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_set_view_mode()
 * @see map_view_create()
 */
int map_view_get_public_transit_enabled(map_view_h view, bool *enable);

/**
 * @brief	Sets map view language.
 * @details This function sets the language to the given map view.
 * \n Note that map display language is different from places and route
 * language.
 * @since_tizen 3.0
 * @privlevel public
 * @remarks If specific language wasn't set explicitly or map tile doesn't
 * support the requested language, the default language of your Maps Provider
 * is used.
 * \n In general, Map Provider set the
 * default language as the mother tongue of the country or English.
 *
 * @param[in]	view		The view handle
 * @param[in]	language	The display language in the <LANGUAGE>_<COUNTRY CODE> syntax. 
 * The <LANGUAGE> is  the ISO 639-1 format and the <COUNTRY CODE> is the ISO 3166-1 alpha-2 format.
 * ISO 639 is a standardized nomenclature used to classify all known languages. Each language is assigned a two-letter (639-1).
 * ISO 3166-1 alpha-2 codes are two-letter country codes defined in ISO 3166-1.
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_get_language()
 * @see map_view_set_type()
 * @see map_view_create()
 */
int map_view_set_language(map_view_h view, const char *language);

/**
 * @brief	Gets map view language.
 * @details This function gets the language set to the map view.
 * \n Note that map display language is different from places and route
 * language.
 * @remarks The @a language must be released using free().
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
 * @see map_view_set_language()
 * @see map_view_get_map_type()
 * @see map_view_create()
 */
int map_view_get_language(const map_view_h view, char **language);


/* --------------------MAPS PANEL MANIPULATIONS-------------------------------*/


/**
 * @brief	Gets the map view port.
 * @details This function gets the map view port as a pointer on Evas_Image.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	viewport	The pointer to Evas_Image in which to store
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
int map_view_get_viewport(const map_view_h view, Evas_Image **viewport);

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
 * @param[in]	view	The view handle
 * @param[in]	x	X screen coordinate for the top left corner of map view
 * @param[in]	y	Y screen coordinate for the top left corner of maps view
 * @param[in]	width	The new width of map view, in screen units
 * @param[in]	height	The new height of map view, in screen units
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network unavailable
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_get_screen_location()
 * @see map_view_resize()
 * @see map_view_set_visibility()
 * @see map_view_create()
 */
int map_view_set_screen_location(const map_view_h view,
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
 * @param[in]	view	The view handle
 * @param[out]	x	X screen coordinate for the top left corner of map view
 * @param[out]	y	Y screen coordinate for the left top corner of maps view
 * @param[out]	width	The pointer to an integer in which to store the  width of the view
 * @param[out]	height	The pointer to an integer in which to store the height of the view
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_set_screen_location()
 * @see map_view_resize()
 * @see map_view_get_visibility()
 * @see map_view_create()
 */
int map_view_get_screen_location(const map_view_h view,
				 int *x,
				 int *y,
				 int *width,
				 int *height);

/**
 * @brief	Moves the map view.
 * @details This function moves map view.
 * @remarks Newly created map viewport has the size of its parent.
 * @since_tizen 3.0
 * @privlevel public
 *
 * @param[in]	view		The view handle
 * @param[in]	width		The new new width, in screen units
 * @param[in]	height		The new new height, in screen units
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_set_screen_location()
 * @see map_view_get_screen_location()
 * @see map_view_set_visibility()
 * @see map_view_create()
 */
int map_view_move(const map_view_h view, const int x, const int y);


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
 * @see map_view_set_screen_location()
 * @see map_view_get_screen_location()
 * @see map_view_set_visibility()
 * @see map_view_create()
 */
int map_view_resize(const map_view_h view, const int width, const int height);

/**
 * @brief	Shows or hides the map view.
 * @details This function changes the visibility of map view on the screen.
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
 * @see map_view_get_visibility()
 * @see map_view_set_screen_location()
 * @see map_view_create()
 */
int map_view_set_visibility(const map_view_h view, const bool visible);

/**
 * @brief	Gets the map view visibility.
 * @details This function retrieves whether or not the given map view is visible.
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
 * @see map_view_set_visibility()
 * @see map_view_set_screen_location()
 * @see map_view_create()
 */
int map_view_get_visibility(const map_view_h view, bool *visible);


/* ---------------------UI CONTROL------------------------------------------*/

/**
 * @brief	Sets the event callback.
 * @details This function sets the callback which will be invoked every time the
 * map view processes the user's gesture, action and objects over the map.
 * @since_tizen 3.0
 * @remarks To unregister the callback use map_view_unset_event_callback().
 *
 * @param[in]	view		The view handle
 * @param[in]	callback	The callback, matching the
 * map_view_on_event_cb() prototype
 * @param[in]	user_data	The user data pointer to be passed to the callback
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_unset_event_callback()
 * @see map_view_on_event_cb()
 * @see map_view_set_gesture_enabled()
 * @see map_view_create()
 */
int map_view_set_event_callback(map_view_h view,
				const map_event_type_e type,
				map_view_on_event_cb callback,
				void *user_data);

/**
 * @brief	Unsets the event callback.
 * @details This function unsets the event callback.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @pre the event callback is registered using map_view_set_event_callback()
 *
 * @see map_view_set_event_callback()
 * @see map_view_get_gesture_enabled()
 * @see map_view_create()
 */
int map_view_unset_event_callback(map_view_h view,
				  const map_event_type_e type);

/**
 * @brief	Enables or disables the map gesture.
 * @details This function enables or disables the map gesture.
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
 * @see #map_gesture_e
 * @see map_view_get_gesture_enabled()
 * @see map_view_create()
 */
int map_view_set_gesture_enabled(map_view_h view,
				 const map_gesture_e gesture,
				 const bool enabled);

/**
 * @brief	Checks whether the map gesture is enabled or not.
 * @details This function checks whether the map gesture is enabled or not.
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
 * @see #map_gesture_e
 * @see map_view_set_gesture_enabled()
 * @see map_view_create()
 */
int map_view_get_gesture_enabled(map_view_h view,
				 const map_gesture_e gesture,
				 bool *enabled);

/**
 * @brief	Enables or disables the map scalebar.
 * @details This function enables or disables the map scalebar.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 *
 * @param[in]	view		The view handle
 * @param[in]	enabled		The enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_get_scalebar_enabled()
 * @see map_view_create()
 */
int map_view_set_scalebar_enabled(map_view_h view, bool enable);

/**
 * @brief	Checks whether the map scalebar is enabled or not.
 * @details This function checks whether the map scalebar is enabled or not.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	enabled		The pointer to a boolean in which to store the enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_set_scalebar_enabled()
 * @see map_view_create()
 */
int map_view_get_scalebar_enabled(map_view_h view, bool *enabled);

 
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
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polygon()
 * @see map_object_create_polyline()
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
 * @see #map_object_h
 * @see map_view_add_object()
 * @see map_view_remove_all_objects()
 * @see map_view_create()
 */
int map_view_remove_all_objects(map_view_h view);

/**
 * @brief	Retrieves all visual objects on the map.
 * @details This function retrieves all visual objects, previously added to the map.
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
