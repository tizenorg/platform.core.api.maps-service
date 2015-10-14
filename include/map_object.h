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


#ifndef __MAP_OBJECT_H__
#define __MAP_OBJECT_H__

#include <maps_service.h>

/**
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_OBJECTS_MODULE Maps View Objects
 *
 * @file map_object.h
 * @brief This file contains the functions of Map Object API
 *
 * @addtogroup CAPI_MAP_OBJECTS_MODULE
 * @{
 * @brief This file contains the functions related to Map View Objects
 * @details Supported Object types are following:
 *  * Markers
 *  * Polygons
 *  * Polylines
 *  * Group Objects
 *  .
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	The Map Visual Object handle
 * @details The handle of an arbitrary visual object instance.
 * @remarks The handle may be issued with one of following functions:
 *  * map_object_create_marker()
 *  * map_object_create_polygon()
 *  * map_object_create_polyline()
 *  * map_object_create_group()
 * \n To release the handle use map_object_destroy().
 * \n Note that when the object is added to Map View it will be released
 * automatically when the Map View is destroyed
 * @since_tizen 3.0
 *
 * @see map_object_destroy()
 * @see map_view_add_object()
 */
typedef void *map_object_h;

/**
 * @brief	Enumerations of visual object types
 * @since_tizen 3.0
 */
typedef enum _map_object_type_e {

	/** Indicates the group of visual objects */
	MAP_OBJECT_GROUP,

	/** Indicates the polyline */
	MAP_OBJECT_POLYLINE,

	/** Indicates the polygon */
	MAP_OBJECT_POLYGON,

	/** Indicates the marker */
	MAP_OBJECT_MARKER,

	/** Indicates the unknown type of visual object */
	MAP_OBJECT_UNKNOWN

} map_object_type_e;

/**
 * @brief	Called when requesting the list of visual objects of the group.
 * @details This callback is invoked while iterating through the list of
 * visual objects, added to the group.
 * @since_tizen 3.0
 * @remarks no need to release @a object.
 *
 * @param[in]	index		The current index of object
 * @param[in]	total		The total amount of objects in the group
 * @param[in]	object		The visual object handle
 * @param[in]	user_data	The user data pointer passed from
 * map_object_group_foreach()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre map_object_group_foreach() will invoke this callback.
 *
 * @par Example
 * @code
#include <map_view.h>

static bool foreach_marker_cb(int index, int total,
				      map_object_h object,
	 			      void *user_data)
{
	if (!object)
		return false;

	map_object_type_e type = MAP_OBJECT_UNKNOWN;
	int error = MAPS_ERROR_NONE;

	// Extract the type of the object
	error = map_object_get_type(object, &type);
	if (error != MAPS_ERROR_NONE)
		return false;

	if (type != MAP_OBJECT_MARKER)
		return true; // This is not a marker

	// Do something...

	// Continue iteration
	return true;
}
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_group_foreach()
 * @see map_object_group_add()
 * @see map_object_group_remove()
 */
typedef bool(*map_object_group_object_cb) (int index, int total,
					   map_object_h object,
					   void *user_data);

/**
 * @brief	Called when requesting the list of points of the polyline.
 * @details This callback is invoked while iterating through the list of
 * points, comprising the polyline.
 * @since_tizen 3.0
 * @remarks @a point must be released using maps_coordinates_destroy().
 * \n To use @a point outside this function, clone it with
 * maps_coordinates_clone().
 *
 * @param[in]	index		The current index of a point
 * @param[in]	total		The total amount of points, comprising the
 * polyline
 * @param[in]	point		The point
 * @param[in]	user_data	The user data pointer passed from
 * map_object_polyline_foreach_point()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre map_object_polyline_foreach_point() will invoke this callback.
 *
 * @par Example
 * @code
#include <map_view.h>

static bool path_point_cb(int index, int total,
			  maps_coordinates_h point,
			  void *user_data)
{
	double latitude = .0;
	double longitude = .0;
	int error = MAPS_ERROR_NONE;

	// Get latitude and longitude of the current point
	error = maps_coordinates_get(point, &latitude, &longitude);
	if(error != MAPS_ERROR_NONE) {
		// Handle the error
	}

	return true; // Continue iteration
}
 * @endcode
 *
 * @see map_object_polyline_foreach_point()
 * @see #maps_coordinates_h
 */
typedef bool(*map_object_polyline_point_cb) (int index, int total,
					     maps_coordinates_h point,
					     void *user_data);


/**
 * @brief	Called when requesting the list of points of the polygon.
 * @details This callback is invoked while iterating through the list of
 * points, comprising the polygon.
 * @since_tizen 3.0
 * @remarks @a point must be released using maps_coordinates_destroy().
 * \n To use @a point outside this function, clone it with
 * maps_coordinates_clone().
 *
 * @param[in]	index		The current index of a point
 * @param[in]	total		The total amount of points, comprising the
 * polygon
 * @param[in]	point		The point
 * @param[in]	user_data	The user data pointer passed from
 * map_object_polygon_foreach_point()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre map_object_polygon_foreach_point() will invoke this callback.
 *
 * @par Example
 * @code
#include <map_view.h>

static bool border_point_cb(int index, int total,
			    maps_coordinates_h point,
			    void *user_data)
{
	double latitude = .0;
	double longitude = .0;
	int error = MAPS_ERROR_NONE;

	// Get latitude and longitude of the current point
	error = maps_coordinates_get(point, &latitude, &longitude);
	if(error != MAPS_ERROR_NONE) {
		// Handle the error
	}

	return true; // Continue iteration
}
 * @endcode
 *
 * @see map_object_polygon_foreach_point()
 * @see #maps_coordinates_h
 */
typedef bool(*map_object_polygon_point_cb) (int index, int total,
					    maps_coordinates_h point,
					    void *user_data);


/*----------------------------------------------------------------------------*/
/*
 * Common Visual Object Features
 */

/**
 * @brief	Creates a marker visual object.
 * @details This function creates a marker on a given geographical coordinates.
 * The marker is specified with a given image file.
 * @since_tizen 3.0
 * @remarks @a marker can be released by using map_object_destroy().
 * \n If added to the Map View using map_view_add_object(), @a marker will be
 * released automatically when the Map View is destroyed.
 *
 * @param[in]	coordinates	The geographical coordinates to add marker on
 * @param[in]	image_file_path	The file name with the image for the marker
 * @param[out]	marker		The pointer to #map_object_h in which to store
 * the handle of newly created marker
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a coordinates are created using map_coordinates_create().
 *
 * @par Example
 * @code
#include <map_view.h>

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
				&marker);
if (error != MAPS_ERROR_NONE) {
	// Handle the create marker error
}

// Release coordinates
maps_coordinates_destroy(coordinates);
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_create_polygon()
 * @see map_object_create_polyline()
 * @see map_object_create_group()
 * @see map_view_add_object()
 * @see map_object_destroy()
 * @see #maps_coordinates_h
 */
int map_object_create_marker(const maps_coordinates_h coordinates,
			     const char *image_file_path,
			     map_object_h *marker);

/**
 * @brief	Creates a polyline visual object.
 * @details This function creates a polyline specified with a list of
 * geographical coordinates, line width and color.
 * @since_tizen 3.0
 * @remarks @a polyline can be released by using map_object_destroy().
 * \n If added to the Map View using map_view_add_object(), @a polyline will be
 * released automatically when the Map View is destroyed.
 *
 * @param[in]	coordinates	The list of geographical coordinates
 * @param[in]	r		The red component of polyline color
 * @param[in]	g		The green component of polyline color
 * @param[in]	b		The blue component of polyline color
 * @param[in]	a		The alpha  component of polyline color
 * @param[in]	width		The width of line
 * @param[out]	polyline	The pointer to #map_object_h in which to store
 * the handle of newly created polyline
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a coordinates is created using maps_coordinates_list_create().
 *
 * @par Example
 * @code
#include <map_view.h>

int error = MAPS_ERROR_NONE;
map_object_h polyline = NULL;
maps_coordinates_list_h coordinates = NULL;
maps_coordinates_h c = NULL;

// Prepare a list of polyline coordinates
error = maps_coordinates_list_create(&coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the create coordinate list error
}

double latitudes[] = {40.206056, 40.140998, 40.123673, 40.087435, 40.042768,
	40.003331, 39.978080, 39.934394, 39.929129, 39.915964};
double longitudes[] = {23.327389, 23.363781, 23.412533, 23.437939, 23.479137,
	23.564281, 23.665905, 23.688564, 23.743496, 23.742809};
for(int i = 0; i < sizeof(latitudes) / sizeof(latitudes[0])) {
	maps_coordinates_create(latitudes[i], longitudes[i], &c);
	maps_coordinates_list_append(coordinates, c);
}

// Create an instance of a polyline
error = map_object_create_polyline(coordinates, 255, 242, 0, 255, &polyline);
if (error != MAPS_ERROR_NONE) {
	// Handle the create polyline error
}

// Release coordinates
maps_coordinates_list_destroy(coordinates);
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polygon()
 * @see map_object_create_group()
 * @see map_view_add_object()
 * @see map_object_destroy()
 * @see #maps_coordinates_list_h
 * @see maps_coordinates_list_create()
 */
int map_object_create_polyline(const maps_coordinates_list_h coordinates,
			       const unsigned char r,
			       const unsigned char g,
			       const unsigned char b,
			       const unsigned char a,
			       const int width,
			       map_object_h *polyline);

/**
 * @brief	Create a polygon visual object.
 * @details This function creates a polygon, specified with a list of
 * geographical coordinates and fill color.
 * @since_tizen 3.0
 * @remarks @a polygon can be released by using map_object_destroy().
 * \n If added to the Map View using map_view_add_object(), @a polygon will be
 * released automatically when the Map View is destroyed.
 *
 * @param[in]	coordinates	The list of geographical coordinates
 * @param[in]	r		The red component of polygon background color
 * @param[in]	g		The green component of polygon background color
 * @param[in]	b		The blue component of polygon background color
 * @param[in]	a		The alpha  component of polygon background color
 * @param[out]	polygon		The pointer to #map_object_h in which to store
 * the handle of newly created polygon
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a coordinates is created using maps_coordinates_list_create().
 *
 * @par Example
 * @code
#include <map_view.h>

int error = MAPS_ERROR_NONE;
map_object_h polygon = NULL;
maps_coordinates_list_h coordinates = NULL;
maps_coordinates_h c = NULL;

// Prepare a list of polygon coordinates
error = maps_coordinates_list_create(&coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the create coordinate list error
}

double latitudes[] = {11.990638, 11.998026, 11.981234, 11.969983, 11.954029,
	11.956045, 11.946976, 11.941098, 11.940426, 11.965785, 11.982074};
double longitudes[] = {121.907848, 121.925873, 121.929478, 121.926903,
	121.937717, 121.945442, 121.947330, 121.942009, 121.934456,
	121.921581, 121.908878};
for(int i = 0; i < sizeof(latitudes) / sizeof(latitudes[0])) {
	maps_coordinates_create(latitudes[i], longitudes[i], &c);
	maps_coordinates_list_append(coordinates, c);
}

// Create an instance of a polygon
error = map_object_create_polygon(coordinates, 255, 242, 0, 255, &polygon);
if (error != MAPS_ERROR_NONE) {
	// Handle the create polygon error
}

// Release coordinates
maps_coordinates_list_destroy(coordinates);
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_group()
 * @see map_view_add_object()
 * @see map_object_destroy()
 * @see #maps_coordinates_list_h
 * @see maps_coordinates_list_create()
 */
int map_object_create_polygon(const maps_coordinates_list_h coordinates,
			      const unsigned char r,
			      const unsigned char g,
			      const unsigned char b,
			      const unsigned char a,
			      map_object_h *polygon);

/**
 * @brief	Creates a group of visual objects.
 * @details This function creates an empty group of visual objects.
 * @since_tizen 3.0
 * @remarks @a group can be released by using map_object_destroy().
 * \n If added to the Map View using map_view_add_object(), @a group will be
 * released automatically when the Map View is destroyed.
 *
 * @param[out]	group		The pointer to #map_object_h in which to store
 * the handle of newly created group
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h path is created ahead using map_object_create_polyline;
// map_object_h marker_start and marker_finish are created ahead
// using map_object_create marker;

int error = MAPS_ERROR_NONE;
map_object_h route = NULL;

// Create an instance of a route as an object group
error = map_object_create_group(&route);
if (error != MAPS_ERROR_NONE) {
	// Handle the create group error
}

// Composing a route out of the path, start and stop markers
error = map_object_group_add(route, path);
if (error != MAPS_ERROR_NONE) {
	// Handle the add to group error
}
error = map_object_group_add(group, marker_start);
if (error != MAPS_ERROR_NONE) {
	// Handle the add to group error
}
error = map_object_group_add(group, marker_finish);
if (error != MAPS_ERROR_NONE) {
	// Handle the add to group error
}
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_group()
 * @see map_object_group_add()
 * @see map_view_add_object()
 * @see map_object_destroy()
 * @see #maps_coordinates_list_h
 * @see maps_coordinates_list_create()
 */
int map_object_create_group(map_object_h *group);

/**
 * @brief	Destroys the object.
 * @details This function destroys the object handle and releases all its
 * resources.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polygon()
 * @see map_object_create_polyline()
 * @see map_object_create_group()
 */
int map_object_destroy(map_object_h object);

/**
 * @brief	Gets the object type.
 * @details This function gets the object type.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[out]	type		The pointer to #map_object_type_e in which to
 * store the object type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a object is created using map_object_create_marker(),
 * map_object_create_polyline(), map_object_create_polygon() or
 * map_object_create_group().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h object is created ahead

int error = MAPS_ERROR_NONE;
map_object_type_e type = MAP_OBJECT_UNKNOWN;

// Fetching a type of the object
error = map_object_get_type(object, &type);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_polygon()
 * @see map_object_create_group()
 */
int map_object_get_type(const map_object_h object, map_object_type_e *type);

/**
 * @brief	Moves the object to specified geographical coordinates.
 * @details This function moves the object to specified geographical
 * coordinates on the map.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[in]	coordinates	The coordinates to move the object to
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a object is created using map_object_create_marker(),
 * map_object_create_polyline(), map_object_create_polygon() or
 * map_object_create_group().
 * @pre @a coordinates are created using maps_coordinates_create(),
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_polygon()
 * @see map_object_create_group()
 * @see maps_coordinates_create()
 */
int map_object_move(map_object_h object, const maps_coordinates_h coordinates);

/**
 * @brief	Shows or hides the object.
 * @details This function changes the visibility of the given object on
 * the map.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[in]	visible		The new visibility of the object
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a object is created using map_object_create_marker(),
 * map_object_create_polyline(), map_object_create_polygon() or
 * map_object_create_group().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h object is created ahead

int error = MAPS_ERROR_NONE;

// Hiding the object on the view
error = map_object_set_visibility(object, false);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_get_visibility()
 */
int map_object_set_visibility(map_object_h object, const bool visible);

/**
 * @brief	Gets the object visibility.
 * @details This function retrieves whether the given object is visible.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[out]	visible		The pointer to a boolean in which to store the
 * object visibility
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a object is created using map_object_create_marker(),
 * map_object_create_polyline(), map_object_create_polygon() or
 * map_object_create_group().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h object is created ahead

int error = MAPS_ERROR_NONE;
bool visible = false;

// Obtaining the visibility of the object
error = map_object_get_visibility(object, &visible);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_set_visibility()
 */
int map_object_get_visibility(map_object_h object, bool *visible);


/*----------------------------------------------------------------------------*/
/*
 * Group of Visual Objects
 */

/**
 * @brief	Adds a visual object into the group.
 * @details This function adds an arbitrary visual object into the
 * specified group.
 * @since_tizen 3.0
 *
 * @param[in]	group		The group object handle
 * @param[in]	object		The object handle to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a group is created using map_object_create_group().
 * @pre @a object is created using map_object_create_marker(),
 * map_object_create_polyline(), map_object_create_polygon() or
 * map_object_create_group().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h route is created ahead using map_object_create_group;
// map_object_h path is created ahead using map_object_create_polyline;
// map_object_h marker_start and marker_finish are created ahead
// using map_object_create marker;

int error = MAPS_ERROR_NONE;

// Composing a route out of the path, start and stop markers
error = map_object_group_add(route, path);
if (error != MAPS_ERROR_NONE) {
	// Handle the add to group error
}
error = map_object_group_add(group, marker_start);
if (error != MAPS_ERROR_NONE) {
	// Handle the add to group error
}
error = map_object_group_add(group, marker_finish);
if (error != MAPS_ERROR_NONE) {
	// Handle the add to group error
}
 * @endcode
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_polygon()
 * @see map_object_create_group()
 * @see map_object_group_foreach()
 * @see map_object_group_remove()
 */
int map_object_group_add(map_object_h group, const map_object_h object);

/**
 * @brief	Removes a visual object from the group.
 * @details This function removes a visual object from the group.
 * @since_tizen 3.0
 *
 * @param[in]	group		The group object handle
 * @param[in]	object		The object handle to remove
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a group is created using map_object_create_group().
 * @pre @a object is created using map_object_create_marker(),
 * map_object_create_polyline(), map_object_create_polygon() or
 * map_object_create_group().
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_polygon()
 * @see map_object_create_group()
 * @see map_object_group_add()
 * @see map_object_group_foreach()
 */

int map_object_group_remove(map_object_h group, map_object_h object);

/**
 * @brief	Retrieves all visual objects, added to the group.
 * @details This function retrieves all visual objects, added to the group.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_object_group_object_cb().
 *
 * @param[in]	group		The group object handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data pointer to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a group is created using map_object_create_group().
 * @post This function invokes map_object_group_object_cb() repeatedly to
 * retrieve each visual object.
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h group is created ahead

static bool foreach_marker_cb(int index, int total,
			      map_object_h object,
			      void *user_data)
{
	if (!object)
		return false;

	map_object_type_e type = MAP_OBJECT_UNKNOWN;
	int error = MAPS_ERROR_NONE;

	// Extract the type of the object
	error = map_object_get_type(object, &type);
	if (error != MAPS_ERROR_NONE)
		return false;

	if (type != MAP_OBJECT_MARKER)
		return true; // This is not a marker

	// Do something...

	// Continue iteration
	return true;
}

static void process_markers()
{
	const int error = map_object_group_foreach(group,
							  foreach_marker_cb,
							  NULL);
	if (error != MAPS_ERROR_NONE) {
		// Handle the error
	}
	return error;
}
 * @endcode
 *
 * @see map_object_create_group()
 * @see map_object_group_object_cb()
 * @see map_object_group_add()
 * @see map_object_group_remove()
 */
int map_object_group_foreach(const map_object_h group,
				    map_object_group_object_cb callback,
				    void* user_data);


/*----------------------------------------------------------------------------*/
/*
 * Polyline
 */

/**
 * @brief	Adds a point to the polyline.
 * @details This function adds a point, specified with geographical coordinates
 * to the polyline.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[in]	point		The point handle to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h path is created ahead using map_object_create_polyline;

int error = MAPS_ERROR_NONE;
maps_coordinates_h point = NULL;

// Create a point to add to the path
error = maps_coordinates_create(50.073482, 14.427961, &point);
if (error != MAPS_ERROR_NONE) {
	// Handle the create coordinates error
}

// Appending a point to the path
error = map_object_polyline_add_point(path, point);
if (error != MAPS_ERROR_NONE) {
	// Handle the add to polyline error
}

// Release the coordinates handle
maps_coordinates_destroy(point);
 * @endcode
 *
 * @see #map_object_h
 * @see #maps_coordinates_h
 * @see map_object_create_polyline()
 * @see maps_coordinates_create()
 * @see map_object_polyline_set_points()
 * @see map_object_polyline_foreach_point()
 */
int map_object_polyline_add_point(map_object_h polyline,
				  const maps_coordinates_h point);

/**
 * @brief	Sets points to the polyline.
 * @details This function sets geographical coordinates point list to the
 * specified polyline.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[in]	points		The points to set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 * @pre @a points are created using maps_coordinates_list_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h polyline is created ahead using map_object_create_polyline.

int error = MAPS_ERROR_NONE;
maps_coordinates_list_h coordinates = NULL;
maps_coordinates_h c = NULL;

// Prepare a list of polyline coordinates
error = maps_coordinates_list_create(&coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}

double latitudes[] = {40.206056, 40.140998, 40.123673, 40.087435, 40.042768,
	40.003331, 39.978080, 39.934394, 39.929129, 39.915964};
double longitudes[] = {23.327389, 23.363781, 23.412533, 23.437939, 23.479137,
	23.564281, 23.665905, 23.688564, 23.743496, 23.742809};
for(int i = 0; i < sizeof(latitudes) / sizeof(latitudes[0])) {
	maps_coordinates_create(latitudes[i], longitudes[i], &c);
	maps_coordinates_list_append(coordinates, c);
}

// Set a list of points to the polyline
error = map_object_polyline_set_points(polyline, coordinates);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Release coordinates
maps_coordinates_list_destroy(coordinates);
 * @endcode
 *
 * @see map_object_create_polyline()
 * @see #map_object_h
 * @see #maps_coordinates_list_h
 * @see maps_coordinates_list_create()
 * @see map_object_polyline_add_point()
 * @see map_object_polyline_foreach_point()
 */
int map_object_polyline_set_points(map_object_h polyline,
				     const maps_coordinates_list_h points);


/**
 * @brief	Retrieves all points, added to the polyline.
 * @details This function retrieves all points, comprising the polyline.
 * @since_tizen 3.0
 * @remarks The points will be delivered via map_object_polyline_point_cb().
 *
 * @param[in]	polyline	The polyline object handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data pointer to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a polyline is created using map_object_create_polyline().
 * @post This function invokes map_object_polyline_point_cb() repeatedly to
 * retrieve each point.
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h path is created ahead using map_object_create_polyline.

static bool path_point_cb(int index, int total,
			  maps_coordinates_h point,
			  void *user_data)
{
	double latitude = .0;
	double longitude = .0;
	int error = MAPS_ERROR_NONE;

	// Get latitude and longitude of the current point
	error = maps_coordinates_get(point, &latitude, &longitude);
	if(error != MAPS_ERROR_NONE) {
		// Handle the error
	}

	return true; // Continue iteration
}

static void move_through_path()
{
	int error = MAPS_ERROR_NONE;

	// Iterate through the path
	error = map_object_polyline_foreach_point(path, path_point_cb, NULL);
	if(error != MAPS_ERROR_NONE) {
		// Handle the error
	}
}
 * @endcode
 *
 * @see map_object_polyline_point_cb()
 * @see map_object_create_polyline()
 * @see #map_object_h
 * @see map_object_polyline_add_point()
 * @see map_object_polyline_set_points()
 */
int map_object_polyline_foreach_point(map_object_h polyline,
				      map_object_polyline_point_cb callback,
				      void *user_data);

/**
 * @brief	Sets the polyline color.
 * @details This function sets the polyline color on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[in]	r		The red component of the color
 * @param[in]	g		The green component of the color
 * @param[in]	b		The blue component of the color
 * @param[in]	a		The alpha component of the color
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h polyline is created ahead using map_object_create_polyline.

int error = MAPS_ERROR_NONE;

// Change the color of the polyline
error = map_object_polyline_set_color(polyline, 0xC0, 0xFF, 0xEE, 0xFF);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_polyline_get_color()
 * @see map_object_polyline_set_width()
 * @see map_object_create_polyline()
 */
int map_object_polyline_set_color(map_object_h polyline,
				  const unsigned char r,
				  const unsigned char g,
				  const unsigned char b,
				  const unsigned char a);

/**
 * @brief	Gets the polyline color.
 * @details This function gets the polyline color on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[out]	r		The unsigned char pointer in which to store the
 * red component of the color, or NULL if not interested
 * @param[out]	g		The unsigned char pointer in which to store the
 * green component of the color, or NULL if not interested
 * @param[out]	b		The unsigned char pointer in which to store the
 * blue component of the color, or NULL if not interested
 * @param[out]	a		The unsigned char pointer in which to store the
 * alpha component of the color, or NULL if not interested
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 * @pre color components may be previously set using
 * map_object_polyline_set_color().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h polyline is created ahead using map_object_create_polyline.

int error = MAPS_ERROR_NONE;
unsigned char r = 0;
unsigned char g = 0;
unsigned char b = 0;

// Get the color components of the polyline
error = map_object_polyline_get_color(polyline, &r, &g, &b, NULL);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_polyline_set_color()
 * @see map_object_polyline_get_width()
 * @see map_object_create_polyline()
 */
int map_object_polyline_get_color(const map_object_h polyline,
				  unsigned char *r,
				  unsigned char *g,
				  unsigned char *b,
				  unsigned char *a);

/**
 * @brief	Sets the polyline width.
 * @details This function sets the polyline width on canvas (in screen units).
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[in]	width		The new width in screen units
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h polyline is created ahead using map_object_create_polyline.

int error = MAPS_ERROR_NONE;

// Change the width of the polyline
error = map_object_polyline_set_width(polyline, 5);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_polyline_get_width()
 * @see map_object_polyline_set_color()
 * @see map_object_create_polyline()
 */
int map_object_polyline_set_width(map_object_h polyline, const int width);

/**
 * @brief	Gets the polyline width.
 * @details This function gets the polyline width on canvas (in screen units).
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[out]	width		The pointer to an integer in which to store the
 * width of polyline (in screen units)
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 * @pre @a width may be previously set using map_object_polyline_set_width().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h polyline is created ahead using map_object_create_polyline.

int error = MAPS_ERROR_NONE;
int width = 0;

// Get the width of the polyline
error = map_object_polyline_get_width(polyline, &width);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_polyline_set_width()
 * @see map_object_polyline_get_color()
 * @see map_object_create_polyline()
 */
int map_object_polyline_get_width(const map_object_h polyline, int *width);


/*----------------------------------------------------------------------------*/
/*
 * Polygon
 */

/**
 * @brief	Adds a point to the polygon.
 * @details This function adds a geographical coordinates point to the
 * specified polygon.
 * @since_tizen 3.0
 *
 * @param[in]	polygon		The polygon object handle
 * @param[in]	point		The point handle to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polygon is created using map_object_create_polygon().
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h border is created ahead using map_object_create_polygon;

int error = MAPS_ERROR_NONE;
maps_coordinates_h point = NULL;

// Create a point to add to the border
error = maps_coordinates_create(50.073482, 14.427961, &point);
if (error != MAPS_ERROR_NONE) {
	// Handle the create coordinates error
}

// Appending a point to the border
error = map_object_polygon_add_point(border, point);
if (error != MAPS_ERROR_NONE) {
	// Handle the add to polygon error
}

// Release the coordinates handle
maps_coordinates_destroy(point);
 * @endcode
 *
 * @see #map_object_h
 * @see #maps_coordinates_h
 * @see map_object_create_polygon()
 * @see maps_coordinates_create()
 * @see map_object_polygon_set_points()
 * @see map_object_polygon_foreach_point()
 */
int map_object_polygon_add_point(map_object_h polygon,
				 const maps_coordinates_h point);

/**
 * @brief	Sets points to the polygon.
 * @details This function sets geographical coordinates point list to the
 * specified polygon.
 * @since_tizen 3.0
 *
 * @param[in]	polygon		The polygon object handle
 * @param[in]	points		The points to set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polygon is created using map_object_create_polygon().
 * @pre @a points are created using maps_coordinates_list_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h polygon is created ahead using map_object_create_polygon.

int error = MAPS_ERROR_NONE;
maps_coordinates_list_h coordinates = NULL;
maps_coordinates_h c = NULL;

// Prepare a list of polygon coordinates
error = maps_coordinates_list_create(&coordinates);
if(error != MAPS_ERROR_NONE) {
	// Handle the create coordinate list error
}

double latitudes[] = {11.990638, 11.998026, 11.981234, 11.969983, 11.954029,
	11.956045, 11.946976, 11.941098, 11.940426, 11.965785, 11.982074};
double longitudes[] = {121.907848, 121.925873, 121.929478, 121.926903,
	121.937717, 121.945442, 121.947330, 121.942009, 121.934456,
	121.921581, 121.908878};
for(int i = 0; i < sizeof(latitudes) / sizeof(latitudes[0])) {
	maps_coordinates_create(latitudes[i], longitudes[i], &c);
	maps_coordinates_list_append(coordinates, c);
}

// Set a list of points to the polygon
error = map_object_polygon_set_points(polygon, coordinates);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Release coordinates
maps_coordinates_list_destroy(coordinates);
 * @endcode
 *
 * @see map_object_create_polygon()
 * @see #map_object_h
 * @see #maps_coordinates_list_h
 * @see maps_coordinates_list_create()
 * @see map_object_polygon_add_point()
 * @see map_object_polygon_foreach_point()
 */
int map_object_polygon_set_points(map_object_h polygon,
				   const maps_coordinates_list_h points);


/**
 * @brief	Retrieves all points, added to the polygon.
 * @details This function retrieves all points, comprising the polygon.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_object_polygon_point_cb().
 *
 * @param[in]	polygon		The polygon object handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a polygon is created using map_object_create_polygon().
 * @post This function invokes map_object_polygon_point_cb() repeatedly to
 * retrieve each point.
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h border is created ahead using map_object_create_polygon.

static bool border_point_cb(int index, int total,
			    maps_coordinates_h point,
			    void *user_data)
{
	double latitude = .0;
	double longitude = .0;
	int error = MAPS_ERROR_NONE;

	// Get latitude and longitude of the current point
	error = maps_coordinates_get(point, &latitude, &longitude);
	if(error != MAPS_ERROR_NONE) {
		// Handle the error
	}

	return true; // Continue iteration
}

static void move_along_border()
{
	int error = MAPS_ERROR_NONE;

	// Iterate through the path
	error = map_object_polygon_foreach_point(border, border_point_cb, NULL);
	if(error != MAPS_ERROR_NONE) {
		// Handle the error
	}
}
 * @endcode
 *
 * @see map_object_polygon_point_cb()
 * @see map_object_create_polygon()
 * @see #map_object_h
 * @see map_object_polygon_add_point()
 * @see map_object_polygon_set_points()
 */
int map_object_polygon_foreach_point(map_object_h polygon,
				     map_object_polygon_point_cb callback,
				     void *user_data);
/**
 * @brief	Sets polygon fill color.
 * @details This function sets the polygon fill color on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polygon		The polygon object handle
 * @param[in]	r		The red component of the fill color
 * @param[in]	g		The green component of the fill color
 * @param[in]	b		The blue component of the fill color
 * @param[in]	a		The alpha component of the fill color
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polygon is created using map_object_create_polygon().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h polygon is created ahead using map_object_create_polygon.

int error = MAPS_ERROR_NONE;

// Change the fill color of the polygon
error = map_object_polygon_set_fill_color(polygon, 0xC0, 0xFF, 0xEE, 0xFF);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_polygon_get_fill_color()
 * @see map_object_create_polygon()
 */
int map_object_polygon_set_fill_color(map_object_h polygon,
				      const unsigned char r,
				      const unsigned char g,
				      const unsigned char b,
				      const unsigned char a);

/**
 * @brief	Gets polygon fill color.
 * @details This function gets the polygon fill color on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polygon		The polygon data handle
 * @param[in]	r		The unsigned char pointer in which to store the
red component of the background color
 * @param[in]	g		The unsigned char pointer in which to store the
green component of the background color
 * @param[in]	b		The unsigned char pointer in which to store the
blue component of the background color
 * @param[in]	a		The unsigned char pointer in which to store the
alpha component of the background color
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polygon is created using map_object_create_polygon().
 * @pre fill color components may be previously set using
 * map_object_polygon_set_fill_color().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h polygon is created ahead using map_object_create_polygon.

int error = MAPS_ERROR_NONE;
unsigned char r = 0;
unsigned char g = 0;
unsigned char b = 0;

// Get the fill color components of the polygon
error = map_object_polygon_get_color(polygon, &r, &g, &b, NULL);
if(error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_polygon_set_fill_color()
 * @see map_object_create_polygon()
 */
int map_object_polygon_get_fill_color(const map_object_h polygon,
				      unsigned char *r,
				      unsigned char *g,
				      unsigned char *b,
				      unsigned char *a);


/*----------------------------------------------------------------------------*/
/*
 * Marker
 */

/**
 * @brief	Sets the marker coordinates.
 * @details This function sets the marker geographical coordinates.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[in]	point		The marker geographical coordinates handle
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h marker is created ahead using map_object_create_marker;

int error = MAPS_ERROR_NONE;
maps_coordinates_h point = NULL;

// Create a point to set as marker coordinates
error = maps_coordinates_create(50.073482, 14.427961, &point);
if (error != MAPS_ERROR_NONE) {
	// Handle the create coordinates error
}

// Set marker coordinates
error = map_object_marker_set_point(marker, point);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Release the coordinates handle
maps_coordinates_destroy(point);
 * @endcode
 *
 * @see map_object_create_marker()
 * @see map_object_marker_get_point()
 * @see #maps_coordinates_h
 * @see maps_coordinates_create()
 */
int map_object_marker_set_point(map_object_h marker,
				const maps_coordinates_h point);

/**
 * @brief	Sets the marker screen size.
 * @details This function sets the marker size on the screen.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[in]	screen_width	The marker width on the screen
 * @param[in]	screen_height	The marker height on the screen
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h marker is created ahead using map_object_create_marker;

int error = MAPS_ERROR_NONE;

// Set marker screen size
error = map_object_marker_resize(marker, 50, 50);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_create_marker()
 * @see map_object_marker_get_size()
 */
int map_object_marker_resize(map_object_h marker,
			     const int screen_width,
			     const int screen_height);
/**
 * @brief	Sets the marker image file path.
 * @details This function sets the marker image file path.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[in]	file_path	The marker image file path
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h marker is created ahead using map_object_create_marker;

int error = MAPS_ERROR_NONE;

// Set marker image file path
error = map_object_marker_set_image_file(marker, "resources/maker_icon.png");
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_marker_get_image_file()
 * @see map_object_create_marker()
 */
int map_object_marker_set_image_file(map_object_h marker,
				     const char *file_path);

/**
 * @brief	Sets the marker text.
 * @details This function sets the marker text.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[in]	text		The marker text
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h marker is created ahead using map_object_create_marker;

int error = MAPS_ERROR_NONE;

// Set marker text
error = map_object_marker_set_text(marker, "Destination");
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_marker_get_text()
 * @see map_object_create_marker()
 */
int map_object_marker_set_text(map_object_h marker,
			       const char *text);

/**
 * @brief	Gets the marker coordinates.
 * @details This function gets the marker geographical coordinates.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	point		The pointer to #maps_coordinates_h in which to
 * store the marker geographical coordinates
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 * @pre @a coordinates may be set previously using
 * map_object_marker_set_point().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h marker is created ahead using map_object_create_marker;

int error = MAPS_ERROR_NONE;
maps_coordinates_h point = NULL;

// Fetch the geographical coordinates of a marker
error = map_object_marker_get_point(marker, &point);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Do something with coordinates...

// Release the coordinates handle
maps_coordinates_destroy(point);
 * @endcode
 *
 * @see map_object_marker_set_point()
 * @see #maps_coordinates_h
 * @see map_object_create_marker()
 */
int map_object_marker_get_point(const map_object_h marker,
				      maps_coordinates_h *point);

/**
 * @brief	Gets the marker screen size.
 * @details This function gets the marker size on the screen.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	width		The pointer to an integer in which to store the
 * marker width on the screen
 * @param[out]	height		The pointer to an integer in which to store the
 * marker height on the screen
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 * @pre @a width ind @a height may be set previously using
 * map_object_marker_resize().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h marker is created ahead using map_object_create_marker;

int error = MAPS_ERROR_NONE;
int width = 0;
int height = 0;

// Get marker screen size
error = map_object_marker_get_size(marker, &width, &height);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}
 * @endcode
 *
 * @see map_object_marker_resize()
 * @see map_object_create_marker()
 */
int map_object_marker_get_size(map_object_h marker, int *width, int *height);

/**
 * @brief	Gets the marker image file path.
 * @details This function gets the marker image file path.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	file_path	The marker image file path
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 * @pre @a file_path may be set previously using
 * map_object_marker_set_image_file().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h marker is created ahead using map_object_create_marker;

int error = MAPS_ERROR_NONE
char *image_file = NULL;

// Get marker image file path
error = map_object_marker_get_image_file(marker, &image_file);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Do something with image_file...

free(image_file);
 * @endcode
 *
 * @see map_object_marker_set_image_file()
 * @see map_object_create_marker()
 */
int map_object_marker_get_image_file(const map_object_h marker,
				     char **file_path);


/* TODO: Check if marker text is supported by map provider */
/**
 * @brief	Gets the marker text.
 * @details This function gets the text.
 * @remarks Release the @ text with free().
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	text		The text
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a marker is created using map_object_create_marker().
 * @pre @a text may be set previously using map_object_marker_set_text().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_object_h marker is created ahead using map_object_create_marker;

int error = MAPS_ERROR_NONE;
char *text = NULL;

// Get marker text
error = map_object_marker_get_text(marker, &text);
if (error != MAPS_ERROR_NONE) {
	// Handle the error
}

// Do something with text ...

free(text);
 * @endcode
 *
 * @see map_object_marker_set_text()
 * @see map_object_create_marker()
 */
int map_object_marker_get_text(const map_object_h marker, char **text);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_OBJECT_H__*/
