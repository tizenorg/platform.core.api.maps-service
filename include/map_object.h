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
#include <maps_coord_list.h>

/**
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_OBJECTS_MODULE Maps View Objects
 */


/**
 *
 * @ingroup	CAPI_MAP_OBJECTS_MODULE
 * @defgroup	CAPI_MAP_OBJECT_MODULE Visual Object
 *
 * @file map_object.h
 * @brief This file contains the functions related to Route information.
 *
 * @addtogroup CAPI_MAP_OBJECT_MODULE
 * @{
 * @brief This provides APIs related to Route information, used in Route Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*#define TIZEN_3_0_NEXT_MS*/

/**
 * @brief	The Maps Visual Object handle
 * @details The handle of an arbitrary visual object instance.
 * @remarks To release the handle use maps_visual_object_destroy().
 * \n To clone the handle use maps_visual_object_clone().
 * @since_tizen 3.0
 *
 * @see maps_visual_object_destroy()
 * @see maps_visual_object_clone()
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

#ifdef TIZEN_3_0_NEXT_MS
	/** Indicates the route */
	MAP_OBJECT_ROUTE,
#endif /* TIZEN_3_0_NEXT_MS */

	/** Indicates the unknown type of visual object */
	MAP_OBJECT_UNKNOWN,
} map_object_type_e;

/**
 * @brief	Enumerations of map marker types
 * @since_tizen 3.0
 */
typedef enum _map_view_marker_type_e{
/* REVIEW: You seem like defining the marker to display the origin and destination of route.
It is not clear purpose.
We had better redesing marker type. We can think bubble type, pin type, or etc..
*/

/* REDESIGN */
#ifdef TIZEN_3_0_NEXT_MS
	MAP_MARKER_START,		/** Indicates the start marker type*/
	MAP_MARKER_FINISH,	/** Indicates the finish marker type*/
	MAP_MARKER_FIRST,		/** Indicates the "first" marker type*/
	MAP_MARKER_SECOND,	/** Indicates the "second" marker type*/
	MAP_MARKER_FAVOURITE,	/** Indicates the favourite marker
					  type*/
	MAP_MARKER_CONSTRUCTION,	/** Indicates the construction marker
					  type*/
#endif /* TIZEN_3_0_NEXT_MS */
	MAP_MARKER_POI,		/** Indicates the POI marker type*/
	MAP_MARKER_NONE,		/** Indicates the empty marker type*/
} map_view_marker_type_e;

/**
 * @brief	Enumerations of operation types over the map view object
 * @since_tizen 3.0
 */
typedef enum _map_object_operation_e{
	MAP_OBJECT_ADD,	/** Indicates the add object operation */
	MAP_OBJECT_SET_VISIBLE,	/** Indicates the change
					  object visibility operation */
	MAP_OBJECT_MOVE,	/** Indicates the move object operation */
	MAP_OBJECT_CHANGE,	/** Indicates the operation of changing
				 object specific properties */
	MAP_OBJECT_REMOVE,	/** Indicates the remove object operation */
	MAP_OBJECT_REMOVE_ALL,/** Indicates the remove all objects
				     operation */

} map_object_operation_e;


/**
 * @brief	Called when requesting the list of visual objects of the group.
 * @details This callback is invoked while iterating through the list of
 * visual objects, added to the group.
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
typedef bool(*map_object_group_object_cb) (int index, int total,
					      map_object_h object,
					      void *user_data);

/**
 * @brief	Called when requesting the list of points of the polygon.
 * @details This callback is invoked while iterating through the list of
 * points, added to the polygon.
 * @since_tizen 3.0
 * @remarks @a point must be released using maps_coordinates_destroy().
 * \n To use @a point outside this function, clone it with
 * maps_coordinates_clone().
 *
 * @param[in]	index		The current index of path point
 * @param[in]	total		The total amount of path points
 * @param[in]	point		The point
 * @param[in]	user_data	The user data passed from
 * map_view_polyline_data_foreach_point()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre map_view_polyline_data_foreach_point() will invoke this callback.
 *
 * @see map_view_polyline_data_foreach_point()
 * @see #maps_coordinates_h
 */
typedef bool(*map_object_polygon_point_cb) (int index, int total,
					    maps_coordinates_h point,
					    void *user_data);

/**
 * @brief	Called when requesting the list of points of the polyline.
 * @details This callback is invoked while iterating through the list of
 * points, added to the polyline.
 * @since_tizen 3.0
 * @remarks @a point must be released using maps_coordinates_destroy().
 * \n To use @a point outside this function, clone it with
 * maps_coordinates_clone().
 *
 * @param[in]	index		The current index of path point
 * @param[in]	total		The total amount of path points
 * @param[in]	point		The point
 * @param[in]	user_data	The user data passed from
 * map_view_polyline_data_foreach_point()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre map_view_polyline_data_foreach_point() will invoke this callback.
 *
 * @see map_view_polyline_data_foreach_point()
 * @see #maps_coordinates_h
 */
typedef bool(*map_object_polyline_point_cb) (int index, int total,
					    maps_coordinates_h point,
					    void *user_data);



/*----------------------------------------------------------------------------*/
/*
 * Common Visual Object Features
 */


/**
 * @brief	Creates a marker visual object.
 * @details This function creates a marker on a given geographical position.
 * The marker is specified with a given image file path and a type.
 * @since_tizen 3.0
 * @remarks @a marker will be released by map view.
 *
 * @param[in]	coordinates	The geographical coordinates to add marker on
 * @param[in]	image_file_path	The file name with the image for the marker
 * @param[in]	type		The type of the marker
 * @param[out]	marker		The added marker handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see #map_object_h
 * @see #map_view_marker_type_e
 * @see map_object_create_marker_image()
 */
int map_object_create_marker(const maps_coordinates_h coordinates,
			     const char *image_file_path,
			     const map_view_marker_type_e type,
			     map_object_h *marker);


#ifdef TIZEN_3_0_NEXT_MS
/**
 * @brief	Creates a route visual object.
 * @details This function creates a visual object group, representing the route,
 * specified with @a route parameter.
 * @since_tizen 3.0
 * @remarks @a route_object will be released by map view.
 *
 * @param[in]	view		The view handle
 * @param[in]	route		The route data
 * @param[out]	route_object	The added route handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see #map_object_h
 * @see #maps_route_h
 */

/*
REVIEW: The API is not clear. Moreover, we had better add route related API in the next MS.
*/

int map_object_create_route(const maps_route_h route,
			    map_object_h *route_object);
#endif /* TIZEN_3_0_NEXT_MS */


/**
 * @brief	Creates a goup of visual objects.
 * @details This function creates a group of visual objects.
 * @since_tizen 3.0
 * @remarks @a group will be released by map view.
 *
 * @param[in]	view		The view handle
 * @param[out]	group		The added route handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see #map_object_h
 */
int map_object_create_group(map_object_h *route_object);


/**
 * @brief	Creates a polyline visual object.
 * @details This function creates a polyline visual object, specified
 * with a list of geographical coordinates.
 * @since_tizen 3.0
 * @remarks @a polyline will be released by map view.
 *
 * @param[in]	coordinates	The list of geographical coordinates
 * @param[in]	r		The red component of polyline color
 * @param[in]	g		The green component of polyline color
 * @param[in]	b		The blue component of polyline color
 * @param[in]	a		The alpha  component of polyline color
 * @param[in]	width		The width of line
 * @param[out]	polyline	The added polyline handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see #map_object_h
 * @see #maps_coord_list_h
 * @see #maps_coordinates_h
 */
int map_object_create_polyline(const maps_coord_list_h coordinates,
			       const unsigned char r,
			       const unsigned char g,
			       const unsigned char b,
			       const unsigned char a,
			       const int width,
			       map_object_h *polyline);

/**
 * @brief	Create a polygon visual object.
 * @details This function creates a polygon visual object, specified
 * with a list of geographical coordinates.
 * @since_tizen 3.0
 * @remarks @a polygon will be released by map view.
 *
 * @param[in]	coordinates	The list of geographical coordinates
 * @param[in]	r		The red component of polygon background color
 * @param[in]	g		The green component of polygon background color
 * @param[in]	b		The blue component of polygon background color
 * @param[in]	a		The alpha  component of polygon background color
 * @param[out]	polygon		The added polygon handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_create()
 * @see #map_object_h
 * @see #maps_coord_list_h
 * @see #maps_coordinates_h
 */
int map_object_create_polygon(const maps_coord_list_h coordinates,
			      const unsigned char r,
			      const unsigned char g,
			      const unsigned char b,
			      const unsigned char a,
			      map_object_h *polygon);

/**
 * @brief	Destroys the object data handle.
 * @details This function destroys the object handle and releases all its
 * resources.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int map_object_destroy(map_object_h object);

/**
 * @brief	Gets the object type.
 * @details This function gets the object type.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[out]	type		The object type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_polygon()
 * @see map_object_group_create()
 * @see map_object_set_type()
 */

/*
I recondiderate map_object API, netherless we have to provider create API.
Tizen object's life cyle is creating, getting, foreach, and destroying...
Please see other CAPIs.

After creating object with create(), you have to get properites.
*/


/*
REVIEW: I think map_object_xxx is better than map_object_xxx()
*/

int map_object_get_type(const map_object_h object,
			      map_object_type_e *type);

/**
 * @brief	Moves the object to specified geographical coordinates.
 * @details This function moves the object on the map to specified geographical
 * coordinates.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[in]	coordinates	The coordinates to move the object to
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a object is created using map_object_create_marker()
 * or map_object_create_polyline()
 * or map_object_create_polygon()
 * or map_object_group_create()
 *
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_polygon()
 * @see map_object_group_create()
 */
int map_object_move(map_object_h object,
			 const maps_coordinates_h coordinates);

/**
 * @brief	Shows the object.
 * @details This function changes the visibility of the given object on the map
 * view.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[in]	visible		The new visibility of the object
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a object is created using map_object_create_marker()
 * or map_object_create_polyline()
 * or map_object_create_polygon()
 * or map_object_group_create()
 *
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_polygon()
 * @see map_object_group_create()
 */
int map_object_set_visible(map_object_h object, const bool visible);

/**
 * @brief	Gets the object visibility.
 * @details This function retrieves whether or not the given object is visible.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[out]	visible		The object visibility
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a object is created using map_object_create_marker()
 * or map_object_create_polyline()
 * or map_object_create_polygon()
 * or map_object_group_create()
 *
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_polygon()
 * @see map_object_group_create()
 */
int map_object_get_visible(map_object_h object, bool *visible);

/*----------------------------------------------------------------------------*/
/*
 * Layer
 */

/**
 * @brief	Adds an arbitrary visual object on the group.
 * @details This function adds an arbitrary visual object on the group.
 * @since_tizen 3.0
 *
 * @param[in]	group		The group object handle
 * @param[in]	object		The object to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a group is created using map_object_group_create().
 *
 * @see map_object_group_create()
 * @see #map_object_h
 */

/*
REVIEW: We will support layer for real map groups such as 2D, traffic, satellite, and terrain.
object word is repeately. Please change the name.
Please change the name of API.
*/

int map_object_group_add_object(map_object_h group,
				     const map_object_h object);

#if 0
/* TO BE REMOVED */
/**
 * @brief	Sets objects to the group.
 * @details This function sets object list to the group.
 * @since_tizen 3.0
 *
 * @param[in]	group		The group object handle
 * @param[in]	objects		The objects to set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_group_create().
 *
 * @see map_object_group_create()
 * @see #map_object_h
 */

/*
REVIEW: As I told you your before, maps_item_list API is not for public API, but it is platform API.
Please don't use it. Please public CAPI such as eina_list. Don't provide redundant data structure API.
If you want to provide list as a prarameter, please define wrapping API.
*/
int map_object_group_set_objects(map_object_h group,
				 const maps_coord_list_h objects);
#endif

/**
 * @brief	Removes a visual object from the group.
 * @details This function removes a visual object from the group.
 * @since_tizen 3.0
 *
 * @param[in]	group		The group object handle
 * @param[in]	object		The object to remove
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a group is created using map_view_group_create().
 *
 * @see map_object_group_create()
 * @see #map_object_h
 */

int map_object_group_remove_object(map_object_h group, map_object_h object);

/**
 * @brief	Retrieves all visual objects, added to the group.
 * @details This function retrieves all visual objects, added to the group.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_object_group_object_cb().
 *
 * @param[in]	group		The group object handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a group is created using map_object_group_create().
 * @post This function invokes map_object_group_object_cb() repeatedly to
 * retrieve each visual object.
 *
 * @see map_object_group_create()
 * @see map_object_group_object_cb()
 */
int map_object_group_foreach_object(const map_object_h group,
				   map_object_group_object_cb callback,
				   void* user_data);


/*----------------------------------------------------------------------------*/
/*
 * Polyline
 */

/**
 * @brief	Adds a point to the polyline.
 * @details This function adds a point to the polyline.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline or polygon object handle
 * @param[in]	point		The point to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @see map_object_create_polyline()
 * @see #map_object_h
 */

/*
REVIEW: I think we don't need this API.
If a developer need a new polyline, developer can call map_object_create_polygon() and map_object_set_polyline().
As you defined API, a developer, after removing polyline and the adding polyline.

Don't use _poly_. It is different between polyline and polygon. You have to check when you create polygon, polygon is created with locations.
Please see location code. It has the checking routine.

API Name have to be clear like below:
map_object_create_polyline()
map_object_create_polygon()

*/
int map_object_polyline_add_point(map_object_h polyline,
				  const maps_coordinates_h point);

/**
 * @brief	Sets points to the polyline.
 * @details This function sets point list to the polyline.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline or polygon object handle
 * @param[in]	points		The points to set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @see map_object_create_polyline()
 * @see #map_object_h
 */

/*
REVIEW: maps_item_list API is not for public API, but it is platform API.
Please don't use it. Please public CAPI such as eina_list. Don't provide redundant data structure API.
If you want to provide list as a prarameter, please define wrapping API.
*/

int map_object_polyline_set_polyline(map_object_h polyline,
				     const maps_coord_list_h points);


/**
 * @brief	Retrieves all points, added to the polyline.
 * @details This function retrieves all points, added to the polyline.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_view_polyline_data_point_cb().
 *
 * @param[in]	polyline	The polyline or polygon object handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a polyline is created using map_object_create_polyline().
 * @post This function invokes map_view_polyline_data_point_cb() repeatedly to
 * retrieve each point.
 *
 * @see map_object_create_polyline()
 * @see map_view_polyline_data_point_cb()
 */

/*
REVIEW: As I told you your before, maps_item_list API is not for public API, but it is platform API.
Please don't use it. Please public CAPI such as eina_list. Don't provide redundant data structure API.
If you want to provide list as a prarameter, please define wrapping API.
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
 * @param[out]	r		The red component of the color
 * @param[out]	g		The green component of the color
 * @param[out]	b		The blue component of the color
 * @param[out]	a		The alpha component of the color
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @see map_object_create_polyline()
 */
int map_object_polyline_get_color(const map_object_h polyline,
				  unsigned char *r,
				  unsigned char *g,
				  unsigned char *b,
				  unsigned char *a);

/**
 * @brief	Sets the polyline width.
 * @details This function sets the polyline width on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[in]	width		The new width
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @see map_object_create_polyline()
 */
int map_object_polyline_set_width(map_object_h polyline, const int width);

/**
 * @brief	Gets the polyline width.
 * @details This function gets the polyline width on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[out]	width		The width
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @see map_object_create_polyline()
 */
int map_object_polyline_get_width(const map_object_h polyline, int *width);


/*----------------------------------------------------------------------------*/
/*
 * Polygon
 */

/**
 * @brief	Adds a point to the polyline.
 * @details This function adds a point to the polyline.
 * @since_tizen 3.0
 *
 * @param[in]	polygon		The polyline or polygon object handle
 * @param[in]	point		The point to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @see map_object_create_polyline()
 * @see #map_object_h
 */

/*
REVIEW: I think we don't need this API.
If a developer need a new polyline, developer can call map_object_create_polygon() and map_object_set_polyline().
As you defined API, a developer, after removing polyline and the adding polyline.

Don't use _poly_. It is different between polyline and polygon. You have to check when you create polygon, polygon is created with locations.
Please see location code. It has the checking routine.

API Name have to be clear like below:
map_object_create_polyline()
map_object_create_polygon()

*/
int map_object_polygon_add_point(map_object_h polygon,
				 const maps_coordinates_h point);

/**
 * @brief	Sets points to the polyline.
 * @details This function sets point list to the polyline.
 * @since_tizen 3.0
 *
 * @param[in]	polygon		The polyline or polygon object handle
 * @param[in]	points		The points to set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @see map_object_create_polyline()
 * @see #map_object_h
 */

/*
REVIEW: maps_item_list API is not for public API, but it is platform API.
Please don't use it. Please public CAPI such as eina_list. Don't provide redundant data structure API.
If you want to provide list as a prarameter, please define wrapping API.
*/

int map_object_polygon_set_polygon(map_object_h polygon,
				    const maps_coord_list_h points);


/**
 * @brief	Retrieves all points, added to the polyline.
 * @details This function retrieves all points, added to the polyline.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_view_polyline_data_point_cb().
 *
 * @param[in]	polygon		The polyline or polygon object handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a polyline is created using map_object_create_polyline().
 * @post This function invokes map_view_polyline_data_point_cb() repeatedly to
 * retrieve each point.
 *
 * @see map_object_create_polyline()
 * @see map_view_polyline_data_point_cb()
 */

/*
REVIEW: As I told you your before, maps_item_list API is not for public API, but it is platform API.
Please don't use it. Please public CAPI such as eina_list. Don't provide redundant data structure API.
If you want to provide list as a prarameter, please define wrapping API.
*/


int map_object_polygon_foreach_point(map_object_h polygon,
				     map_object_polygon_point_cb callback,
				     void *user_data);
/**
 * @brief	Sets the polygon color.
 * @details This function sets the polygon color on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polygon		The polygon object handle
 * @param[in]	r		The red component of the background color
 * @param[in]	g		The green component of the background color
 * @param[in]	b		The blue component of the background color
 * @param[in]	a		The alpha component of the background color
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polygon is created using map_object_create_polygon().
 *
 * @see map_object_create_polygon()
 * @see map_view_polygon_get_color()
 */
int map_object_polygon_set_fill_color(map_object_h polygon,
				      const unsigned char r,
				      const unsigned char g,
				      const unsigned char b,
				      const unsigned char a);

/**
 * @brief	Gets the polygon color.
 * @details This function gets the polygon color on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polygon	The polygon data handle
 * @param[in]	r		The red component of the background color
 * @param[in]	g		The green component of the background color
 * @param[in]	b		The blue component of the background color
 * @param[in]	a		The alpha component of the background color
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polygon is created using map_object_create_polygon().
 *
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
 * @details This function sets the marker coordinates.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[in]	coordinates	The marker geographical coordinates
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 * @see map_object_marker_get_coordinates()
 * @see #maps_coordinates_h
 */
int map_object_marker_set_coordinates(map_object_h marker,
				   const maps_coordinates_h coordinates);

/**
 * @brief	Sets the marker screen size.
 * @details This function sets the marker screen size.
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
 * @see map_object_create_marker()
 * @see map_object_marker_get_size()
 */

/*
REVIEW: I don't know what this API does.
Maker's size is decided height and width. If a developer want to change the size, a developer will call resize().
If we don't provide resize API, we have to provide it.
*/
int map_object_marker_resize(map_object_h marker,
			     const int screen_width,
			     const int screen_height);
/**
 * @brief	Sets the marker file path.
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
 * @see map_object_create_marker()
 */

/*
REVIEW: evas_object can be our reference. I don't mean that we following evas_object...
map_object_marker_set_image_file is better.
*/
int map_object_marker_set_image_file(map_object_h marker,
					const char *file_path);

#if 0
/* TO BE REMOVED */
/**
 * @brief	Sets the marker type.
 * @details This function sets the marker type.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[in]	type		The marker type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 * @see #map_view_marker_type_e
 */

/*
REVIEW: I think we don't need this API.
When marker is created, market type is decided. The market type is not changed.
map_object_marker_set_image_file is better.
*/

int map_object_marker_set_type(map_object_h marker,
			       const map_view_marker_type_e type);
#endif

/* TODO: Check if marker text is supported by map provider */
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
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 */

/*
REVIEW: I heard that HERE didn't support text
Please check it
*/
int map_object_marker_set_text(map_object_h marker,
				   const char *text);

/**
 * @brief	Gets the marker coordinates.
 * @details This function gets the marker geographical coordinates.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	coordinates	The marker geographical coordinates
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 * @see map_object_marker_set_coordinates()
 * @see #maps_coordinates_h
 */
int map_object_marker_get_coordinates(const map_object_h marker,
					  maps_coordinates_h *coordinates);

/**
 * @brief	Gets the marker screen size.
 * @details This function gets the marker size on the screen.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	screen_width	The marker width on the screen
 * @param[out]	screen_height	The marker height on the screen
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 * @see map_object_marker_resize()
 */

/*
REVIEW: Same as above about screen_size.
*/
int map_object_marker_get_size(map_object_h marker,
					  int *screen_width,
					  int *screen_height);

/**
 * @brief	Gets the marker file path.
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
 *
 * @see map_object_create_marker()
 * @see map_object_marker_set_image_file()
 */

/*
REVIEW: I think we don't need this API.
When marker is created, market type is decided. The market type is not changed.
map_object_marker_get_image_file is better.
*/
/* Remaining this API for a while: it may be useful to deliver marker file name
*  to the plugin; there is no other way to do it now */
int map_object_marker_get_image_file(const map_object_h marker,
					char **file_path);

/**
 * @brief	Gets the marker type.
 * @details This function gets the marker type.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	type		The type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 * @see #map_view_marker_type_e
 */
int map_object_marker_get_type(const map_object_h marker,
				   map_view_marker_type_e *type);

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
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 */
/*
REVIEW: I heard that HERE didn't support text
Please check it
*/
int map_object_marker_get_text(const map_object_h marker,
				   char **text);

#ifdef TIZEN_3_0_NEXT_MS
/**
 * @brief	Gets the route content.
 * @details This function gets the content of the route.
 * @remarks Release the @ content with maps_route_destroy().
 * @since_tizen 3.0
 *
 * @param[in]	route		The route object handle
 * @param[out]	content		The content of the route of maps_route_h type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a route is created using map_object_create_route().
 *
 * @see map_object_create_route()
 */


/*
REVIEW: The API is not clear. Moreover, we had better add route related API in the next MS.
*/

int map_object_route_get_content(const map_object_h route,
				      maps_route_h *content);
#endif /* TIZEN_3_0_NEXT_MS */

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_OBJECT_H__*/
