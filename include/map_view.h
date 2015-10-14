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
//#include <Ecore_Evas.h>

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

	/** Indicates the  public transit theme */
	MAP_VIEW_TYPE_PUBLIC_TRANSIT,

	/** Indicates the combination of the night and public transit theme */
	MAP_VIEW_TYPE_NIGHT_PUBLIC_TRANSIT,

	/** Indicates the satellite theme */
	MAP_VIEW_TYPE_SATELITE,

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
 * @param[in]	user_data	The user data passed from
 * map_view_foreach_object()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre map_view_foreach_object() will invoke this callback.
 *
 * @see map_view_foreach_object()
 * @see #map_object_h
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
 * @param[in]	type		The new type of callback
 * @param[in]	cb_data		The event data
 * @param[in]	user_data	The user data registered in
 * map_view_set_event_callback()
 *
 * @pre map_view_set_on_zoom_callback() sets the callback.
 * @pre map_view_set_zoom_factor() invokes the callback.
 *
 * @see map_view_set_on_zoom_callback()
 * @see map_view_set_zoom_factor()
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
 * resources and issues its handle.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks @a view must be released using map_view_destroy().
 *
 * @param[in]	maps		The maps service handle
 * @param[in]	parent		The parent visual object
 * @param[out]	view		The maps view handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @see map_view_destroy()
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
 */
int map_view_destroy(map_view_h view);


/*----------------------MAP ZOOM, ROTATE, SET CENTER--------------------------*/


/**
 * @brief	Shows the map with a given position centered.
 * @details This function shows the map with a given position centered, using
 * current zoom level and rotation angle.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
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
 *
 * @see map_view_get_center()
 * @see map_view_set_zoom_factor()
 * @see map_view_set_orientation()
 */
int map_view_set_center(const map_view_h view,
			const maps_coordinates_h coordinates);
/**
 * @brief	Get the central coordinates of a Map.
 * @details This function gets the central coordinates of a Map.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 *
 * @param[in]	view		The view handle
 * @param[out]	coordinates	The geographical coordinates of the central
 * position of the Map
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_set_center()
 * @see map_view_set_zoom_factor()
 * @see map_view_set_orientation()
 */
int map_view_get_center(const map_view_h view,
			maps_coordinates_h *coordinates);

/**
 * @brief	Sets zooms level of the Map.
 * @details This function sets the integer zoom level of the Map
 * on the Map View.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	level		The new zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_zoom_cb().
 *
 * @see map_view_create()
 * @see map_view_get_zoom_level()
 * @see map_view_on_zoom_cb()
 */

int map_view_set_zoom_level(map_view_h view, const int level);

/**
 * @brief	Gets zooms level of the Map.
 * @details This function gets the integer zoom level of the Map
 * on the Map View.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	level		The current zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_get_zoom_level()
 */
int map_view_get_zoom_level(const map_view_h view, int *level);

/**
 * @brief	Gets the minimal zooms level of the Map.
 * @details This function gets the minimally available zoom level of the Map
 * on the Map View.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	min_zoom_level	The minimally available zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_get_zoom_level()
 */
int map_view_get_min_zoom_level(const map_view_h view, int *min_zoom_level);

/**
 * @brief	Gets the maximal zooms level of the Map.
 * @details This function gets the maximally available zoom level of the Map
 * on the Map View.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	max_zoom_level	The maximally available zoom level
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_zoom_cb().
 *
 * @see map_view_create()
 * @see map_view_get_zoom_level()
 * @see map_view_on_zoom_cb()
 */
int map_view_get_max_zoom_level(const map_view_h view, int *max_zoom_level);

/* TODO: It may be removed from API */
/**
 * @brief	Zooms the map on the view.
 * @details This function zooms the map on the Map View with real value of zoom
 * factor.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	factor		The new zoom factor
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_zoom_cb().
 *
 * @see map_view_create()
 * @see map_view_get_zoom_factor()
 * @see map_view_set_on_zoom_callback()
 * @see map_view_on_zoom_cb()
 */
int map_view_set_zoom_factor(const map_view_h view, const double factor);

/* TODO: It may be removed from API */
/**
 * @brief	Gets the map zoom factor.
 * @details This function gets the current zoom factor of the Map on the Map
 * View.
 * @since_tizen 3.0
 * @remarks zoom @a factor can be obtained also in zoom callback, specified with
 * map_view_set_on_zoom_callback().
 *
 * @param[in]	view		The view handle
 * @param[out]	factor		The current zoom factor
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_set_zoom_factor()
 * @see map_view_set_on_zoom_callback()
 * @see map_view_on_zoom_cb()
 */
int map_view_get_zoom_factor(const map_view_h view, double *factor);

/**
 * @brief	Set the map orientation on the Map View.
 * @details This function sets the rotation angle of the Map on the Map View.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	angle		The new rotation angle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_rotate_cb().
 *
 * @see map_view_create()
 * @see map_view_get_orientation()
 * @see map_view_set_on_rotate_callback()
 * @see map_view_on_rotate_cb()
 */
int map_view_set_orientation(const map_view_h view, const double angle);

/**
 * @brief	Gets the map orientation.
 * @details This function gets the current Map rotation angle on the  Map View.
 * @since_tizen 3.0
 * @remarks rotation @a angle can be obtained also in rotation callback,
 * specified with map_view_set_on_rotation_callback().
 *
 * @param[in]	view		The view handle
 * @param[out]	angle		The current rotation angle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_set_orientation()
 * @see map_view_set_on_rotate_callback()
 * @see map_view_on_rotate_cb()
 */
int map_view_get_orientation(const map_view_h view, double *angle);

/**
 * @brief	Sets incline of Map view port.
 * @details This function sets the incline of Map view port.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	tilt		The tilting degree
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_rotate_cb().
 *
 * @see map_view_create()
 * @see map_view_get_tilt()
 */
int map_view_set_tilt(map_view_h view, const double tilt);

/**
 * @brief	Gets incline of Map view port.
 * @details This function gets the incline of Map view port.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	tilt		The tilting degree
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_rotate_cb().
 *
 * @see map_view_set_tilt()
 */
int map_view_get_tilt(const map_view_h view, double *tilt);


/**
 * @brief	Gets the minimal tilt of the Map.
 * @details This function gets the minimally available tilt of the Map
 * on the Map View.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	min_tilt		The minimally available tilt
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_get_tilt()
 * @see map_view_get_max_tilt()
 */
int map_view_get_min_tilt(const map_view_h view, int *min_tilt);

/**
 * @brief	Gets the maximal tilt of the Map.
 * @details This function gets the maximally available tilt of the Map
 * on the Map View.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	max_tilt		The maximally available tilt
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_zoom_cb().
 *
 * @see map_view_create()
 * @see map_view_get_tilt()
 * @see map_view_get_min_tilt()
 */
int map_view_get_max_tilt(const map_view_h view, int *max_tilt);


/*----------------------SCREEN <--> GEOGRAPHY---------------------------------*/

/**
 * @brief	Converts screen coordinates to the geographical coordinates.
 * @details This function converts screen coordinates to the geographical
 * coordinates accordingly to the current Maps settings.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	x		The x coordinate on the screen
 * @param[in]	y		The y coordinate on the screen
 * @param[out]	coordinates	The corresponding geographical coordinates
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_geography_to_screen()
 * @see #maps_coordinates_h
 */
int map_view_screen_to_geography(const map_view_h view,
				  const int x, const int y,
				  maps_coordinates_h *coordinates);

/**
 * @brief	Converts geographical coordinates to the screen coordinates.
 * @details This function converts geographical coordinates to the screen
 * coordinates accordingly to the current Maps settings.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 *
 * @param[in]	view		The view handle
 * @param[in]	coordinates	The geographical coordinates
 * @param[out]	x		The corresponding x coordinate on the screen
 * @param[out]	y		The corresponding y coordinate on the screen
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_screen_to_geography()
 * @see #maps_coordinates_h
 */
int map_view_geography_to_screen(const map_view_h view,
				  const maps_coordinates_h coordinates,
				  int *x, int *y);


/* --------------------MAPS VIEW PREFERENCES----------------------------------*/


/**
 * @brief	Sets Map View type.
 * @details This function sets to the given Map View a type such as normal,
 * satellite, terrain, hybrid.
 * \n The type options are defined in the #map_view_type_e enumeration.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	type		The view type
 * @return       0 on success, otherwise a negative error value
 * @retval       #MAPS_ERROR_NONE Successful
 * @retval       #MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see #map_view_type_e
 * @see map_view_get_type()
 */
int map_view_set_type(map_view_h view, const map_view_type_e type);

/**
 * @brief	Gets Map View type.
 * @details This function gets the type of the given Map View.
 * \n The type options are defined in the #map_view_type_e enumeration.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	type		The view type
 * @return       0 on success, otherwise a negative error value
 * @retval       #MAPS_ERROR_NONE Successful
 * @retval       #MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see #map_view_type_e
 * @see map_view_set_type()
 */
int map_view_get_type(const map_view_h view, map_view_type_e *type);

/**
 * @brief	Sets the maps view mode.
 * @details This function sets the current mode of the maps view.
 * @since_tizen 2.4
 *
 * @param[in]	view		The view handle
 * @param[in]	mode		The maps mode
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_get_view_mode()
 * @see #map_view_mode_e
 */
int map_view_set_mode(map_view_h view, const map_view_mode_e mode);

/**
 * @brief	Gets the maps view mode.
 * @details This function gets the current mode of the maps view.
 * @since_tizen 2.4
 *
 * @param[in]	view	The preference handle
 * @param[out]	mode		The maps mode
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a preference is created using maps_preference_create().
 *
 * @see maps_preference_create()
 * @see maps_preference_set_view_mode()
 * @see #map_view_mode_e
 */
int maps_view_get_mode(map_view_h view, map_view_mode_e* mode);

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
 *
 * @param[in]	view		The view handle
 * @param[in]	language	The language
 * @return       0 on success, otherwise a negative error value
 * @retval       #MAPS_ERROR_NONE Successful
 * @retval       #MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_get_language()
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
 * @param[in]	language	The language
 * @return       0 on success, otherwise a negative error value
 * @retval       #MAPS_ERROR_NONE Successful
 * @retval       #MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_set_language()
 */
int map_view_get_language(const map_view_h view, char **language);


/* --------------------MAPS PANEL MANIPULATIONS-------------------------------*/


/**
 * @brief	Gets the Map View panel.
 * @details This function gets the maps view panel as a pointer on Evas_Object.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 *
 * @param[in]	view		The view handle
 * @param[out]	panel		The Evas_Object pointer on Map View panel
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 */
int map_view_get_panel(const map_view_h view, Evas_Object **panel);

/**
 * @brief	Sets geometry of map view port.
 * @details This function set the position and (rectangular) size of the given
 * map view panel.
 * \n The position, naturally, will be relative to the top left corner of the
 * parent window.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	x		X position to move the map view to, in
 * screen units
 * @param[in]	y		Y position to move the map view to, in
 * screen units
 * @param[in]	width		The new width of map view
 * @param[in]	height		The new height of map view
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_set_geometry()
 * @see map_view_resize()
 */
int map_view_set_geometry(const map_view_h view,
			   const int x, const int y,
			   const int width, const int height);

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
 * @param[in]	x		Pointer to an integer in which to store the X
 * coordinate of the view
 * @param[in]	y		Pointer to an integer in which to store the Y
 * coordinate of the view
 * @param[in]	width		Pointer to an integer in which to store the
 * width of the view
 * @param[in]	height		Pointer to an integer in which to store the
 * height of the view
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_get_geometry()
 */
int map_view_get_geometry(const map_view_h view,
			   int *x, int *y,
			   int *width, int *height);

/**
 * @brief	Resizes the Map View.
 * @details This function changes the size of the given map view panel.
 * @remarks Newly created map view panel has the size of its parent.
 * @since_tizen 3.0
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
 * @see map_view_create()
 * @see map_view_get_geometry()
 */
int map_view_resize(const map_view_h view, const int width, const int height);

/**
 * @brief	Shows the Map View.
 * @details This function changes the visibility of Map View panel.
 * on the screen.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	visible		The new visibility of the Map View
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_get_visible()
 */
int map_view_set_visible(const map_view_h view, const bool visible);

/**
 * @brief	Gets the Map View panel visibility.
 * @details This function retrieves whether or not the given map view panel is
 * visible.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[out]	visible		The visibility of the Map View
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_set_visible()
 */
int map_view_get_visible(const map_view_h view, bool *visible);

/**
 * @brief	Redraws the Map View panel.
 * @details This function forces the redraw routine of the Map View panel on
 * the screen.
 * @remarks Note that this function only marks the Map View as needed to be
 * redrawn. Actual redraw is happening when the OS drawing system is ready for it.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
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
 * @remarks To unregister the callback use map_view_unset_event_callback
 *
 * @param[in]	view		The view handle
 * @param[in]	type		The new type of callback
 * @param[in]	callback	The gesture detected callback
 * @param[in]	user_data	The user data to be passed to the callback
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_on_event_cb()
 * @see map_view_unset_event_callback()
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
 * @param[in]	type		The new type of callback
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_set_event_callback()
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
 * @param[in]	gesture		The user gesture
 * @param[in]	action		The action which should be taken
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_set_gesture_enabled()
 * @see map_view_get_gesture_enabled()
 * @see #map_gesture_e
 * @see #map_action_e
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
 * @param[out]	action		The action which assigned as a reaction on a
 * user gesture
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_set_gesture_enabled()
 * @see map_view_get_gesture_enabled()
 * @see #map_gesture_e
 * @see #map_action_e
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
 * @see map_view_create()
 * @see map_view_set_gesture_action()
 * @see #map_gesture_e
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
 * @param[out]	enabled		The enable status
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see map_view_set_gesture_action()
 * @see #map_gesture_e
 */
int map_view_get_gesture_enabled(map_view_h view,
				 const map_gesture_e gesture,
				 bool *enabled);


/* ---------------------VISUAL OBJECTS ON THE MAP-----------------------------*/

/**
 * @brief	Adds a visual object on the map.
 * @details This function adds a visual object on the map.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	object		The object to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see #map_object_h
 */
int map_view_add_object(map_view_h view, map_object_h object);


/**
 * @brief	Removes a visual object from the map.
 * @details This function removes a visual object from the map.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @param[in]	object		The object to remove
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see #map_object_h
 */
int map_view_remove_object(map_view_h view, const map_object_h object);

/**
 * @brief	Removes all visual objects from the map.
 * @details This function removes all visual object from the map.
 * @since_tizen 3.0
 *
 * @param[in]	view		The view handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see #map_object_h
 */
int map_view_remove_all_objects(map_view_h view);


/**
 * @brief	Retrieves all visual objects, added to the map.
 * @details This function retrieves all visual objects, added to the map.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_object_cb().
 *
 * @param[in]	view		The view handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes map_object_cb() repeatedly to retrieve each
 * visual object.
 *
 * @see map_object_cb()
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
