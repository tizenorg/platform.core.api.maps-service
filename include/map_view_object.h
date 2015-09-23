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


#ifndef __MAP_VIEW_OBJECT_H__
#define __MAP_VIEW_OBJECT_H__

#include <maps_service.h>
#include <maps_extra_types.h>
#include <maps_route.h>

/**
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_VIEW_OBJECTS_MODULE Maps View Objects
 */


/**
 *
 * @ingroup	CAPI_MAP_VIEW_OBJECTS_MODULE
 * @defgroup	CAPI_MAP_VIEW_OBJECT_MODULE Visual Object
 *
 * @file map_view_object.h
 * @brief This file contains the functions related to Route information.
 *
 * @addtogroup CAPI_MAP_VIEW_OBJECT_MODULE
 * @{
 * @brief This provides APIs related to Route information, used in Route Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

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
typedef void *map_view_object_h;

/**
 * @brief	Enumerations of visual object types
 * @since_tizen 3.0
 */
typedef enum _map_view_object_type_e {
	/** Indicates the layer of visual objects */
	MAP_VIEW_OBJECT_LAYER,

	/** Indicates the polyline */
	MAP_VIEW_OBJECT_POLYLINE,

	/** Indicates the polygon */
	MAP_VIEW_OBJECT_POLYGON,

	/** Indicates the marker */
	MAP_VIEW_OBJECT_MARKER,

	/** Indicates the route */
	MAP_VIEW_OBJECT_ROUTE,

	/** Indicates the unknown type of visual object */
	MAP_VIEW_OBJECT_UNKNOWN,
} map_view_object_type_e;

/**
 * @brief	Enumerations of map marker types
 * @since_tizen 3.0
 */
typedef enum _map_view_marker_type_e{
	MAP_VIEW_MARKER_START,		/** Indicates the start marker type*/
	MAP_VIEW_MARKER_FINISH,	/** Indicates the finish marker type*/
	MAP_VIEW_MARKER_FIRST,		/** Indicates the "first" marker type*/
	MAP_VIEW_MARKER_SECOND,	/** Indicates the "second" marker type*/
	MAP_VIEW_MARKER_FAVOURITE,	/** Indicates the favourite marker
					  type*/
	MAP_VIEW_MARKER_CONSTRUCTION,	/** Indicates the construction marker
					  type*/
	MAP_VIEW_MARKER_POI,		/** Indicates the POI marker type*/
	MAP_VIEW_MARKER_NONE,		/** Indicates the empty marker type*/
} map_view_marker_type_e;

/**
 * @brief	Enumerations of operation types over the map view object
 * @since_tizen 3.0
 */
typedef enum _map_view_object_operation_e{
	MAP_VIEW_OBJECT_ADD,	/** Indicates the add object operation */
	MAP_VIEW_OBJECT_SET_VISIBLE,	/** Indicates the change
					  object visibility operation */
	MAP_VIEW_OBJECT_MOVE,	/** Indicates the move object operation */
	MAP_VIEW_OBJECT_CHANGE,	/** Indicates the operation of changing
				 object specific properties */
	MAP_VIEW_OBJECT_REMOVE,	/** Indicates the remove object operation */
	MAP_VIEW_OBJECT_REMOVE_ALL,/** Indicates the remove all objects
				     operation */

} map_view_object_operation_e;


/**
 * @brief	Called when requesting the list of visual objects of the layer.
 * @details This callback is invoked while iterating through the list of
 * visual objects, added to the layer.
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
 * @see #map_view_object_h
 */
typedef bool(*map_view_object_layer_object_cb) (int index, int total,
					      map_view_object_h object,
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
typedef bool(*map_view_object_poly_point_cb) (int index, int total,
					      maps_coordinates_h point,
					      void *user_data);


/*----------------------------------------------------------------------------*/
/*
 * Common Visual Object Features
 */

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
 * @see map_view_add_marker()
 * @see map_view_add_polyline()
 * @see map_view_add_polygon()
 * @see map_view_add_route()
 * @see map_view_add_layer()
 * @see map_view_object_set_type()
 */
int map_view_object_get_type(const map_view_object_h object,
			      map_view_object_type_e *type);

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
 * @pre @a object is created using map_view_add_marker()
 * or map_view_add_polyline()
 * or map_view_add_polygon()
 * ot map_view_add_route()
 * or map_view_add_layer()
 *
 * @see map_view_add_marker()
 * @see map_view_add_polyline()
 * @see map_view_add_polygon()
 * @see map_view_add_route()
 * @see map_view_add_layer()
 */
int map_view_object_move(map_view_object_h object,
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
 * @pre @a object is created using map_view_add_marker()
 * or map_view_add_polyline()
 * or map_view_add_polygon()
 * ot map_view_add_route()
 * or map_view_add_layer()
 *
 * @see map_view_add_marker()
 * @see map_view_add_polyline()
 * @see map_view_add_polygon()
 * @see map_view_add_route()
 * @see map_view_add_layer()
 */
int map_view_object_set_visible(map_view_object_h object, const bool visible);

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
 * @pre @a object is created using map_view_add_marker()
 * or map_view_add_polyline()
 * or map_view_add_polygon()
 * ot map_view_add_route()
 * or map_view_add_layer()
 *
 * @see map_view_add_marker()
 * @see map_view_add_polyline()
 * @see map_view_add_polygon()
 * @see map_view_add_route()
 * @see map_view_add_layer()
 */
int map_view_object_get_visible(map_view_object_h object, bool *visible);

/*----------------------------------------------------------------------------*/
/*
 * Layer
 */

/**
 * @brief	Adds an arbitrary visual object on the layer.
 * @details This function adds an arbitrary visual object on the layer.
 * @since_tizen 3.0
 *
 * @param[in]	layer		The layer object handle
 * @param[in]	object		The object to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a layer is created using map_view_add_layer().
 *
 * @see map_view_add_layer()
 * @see #map_view_object_h
 */
int map_view_object_layer_add_object(map_view_object_h layer,
				     const map_view_object_h object);

/**
 * @brief	Sets objects to the layer.
 * @details This function sets object list to the layer.
 * @since_tizen 3.0
 *
 * @param[in]	layer		The layer object handle
 * @param[in]	objects		The objects to set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_view_add_layer().
 *
 * @see map_view_add_layer()
 * @see #map_view_object_h
 */
int map_view_object_layer_set_objects(map_view_object_h layer,
				     const maps_item_list_h objects);

/**
 * @brief	Removes a visual object from the layer.
 * @details This function removes a visual object from the layer.
 * @since_tizen 3.0
 *
 * @param[in]	layer		The layer object handle
 * @param[in]	object		The object to remove
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a layer is created using map_view_layer_create().
 *
 * @see map_view_add_layer()
 * @see #map_view_object_h
 */

int map_view_object_layer_remove_object(map_view_object_h layer,
					map_view_object_h *object);

/**
 * @brief	Retrieves all visual objects, added to the layer.
 * @details This function retrieves all visual objects, added to the layer.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_view_object_layer_object_cb().
 *
 * @param[in]	layer		The layer object handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a layer is created using map_view_add_layer().
 * @post This function invokes map_view_object_layer_object_cb() repeatedly to
 * retrieve each visual object.
 *
 * @see map_view_add_layer()
 * @see map_view_object_layer_object_cb()
 */
int map_view_object_layer_foreach_object(const map_view_object_h layer,
				   map_view_object_layer_object_cb callback,
				   void* user_data);

/*----------------------------------------------------------------------------*/
/*
 * Poly - Common for Polyline and Polygon
 */

/**
 * @brief	Adds a point to the polyline.
 * @details This function adds a point to the polyline.
 * @since_tizen 3.0
 *
 * @param[in]	poly		The polyline or polygon object handle
 * @param[in]	point		The point to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_view_add_polyline().
 *
 * @see map_view_add_polyline()
 * @see #map_view_object_h
 */
int map_view_object_poly_add_point(map_view_object_h poly,
					const maps_coordinates_h point);

/**
 * @brief	Sets points to the polyline.
 * @details This function sets point list to the polyline.
 * @since_tizen 3.0
 *
 * @param[in]	poly		The polyline or polygon object handle
 * @param[in]	points		The points to set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_view_add_polyline().
 *
 * @see map_view_add_polyline()
 * @see #map_view_object_h
 */
int map_view_object_poly_set_points(map_view_object_h poly,
				  const maps_item_list_h points);


/**
 * @brief	Retrieves all points, added to the polyline.
 * @details This function retrieves all points, added to the polyline.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_view_polyline_data_point_cb().
 *
 * @param[in]	poly		The polyline or polygon object handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a polyline is created using map_view_add_polyline().
 * @post This function invokes map_view_polyline_data_point_cb() repeatedly to
 * retrieve each point.
 *
 * @see map_view_add_polyline()
 * @see map_view_polyline_data_point_cb()
 */
int map_view_object_poly_foreach_point(map_view_object_h poly,
				       map_view_object_poly_point_cb callback,
				       void *user_data);

/*----------------------------------------------------------------------------*/
/*
 * Polyline
 */

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
 * @pre @a polyline is created using map_view_add_polyline().
 *
 * @see map_view_add_polyline()
 */
int map_view_object_polyline_set_color(map_view_object_h polyline,
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
 * @pre @a polyline is created using map_view_add_polyline().
 *
 * @see map_view_add_polyline()
 */
int map_view_object_polyline_get_color(const map_view_object_h
				       polyline,
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
 * @pre @a polyline is created using map_view_add_polyline().
 *
 * @see map_view_add_polyline()
 */
int map_view_object_polyline_set_width(map_view_object_h polyline,
				       const int width);

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
 * @pre @a polyline is created using map_view_add_polyline().
 *
 * @see map_view_add_polyline()
 */
int map_view_object_polyline_get_width(const map_view_object_h polyline,
				       int *width);


/*----------------------------------------------------------------------------*/
/*
 * Polygon
 */

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
 * @pre @a polygon is created using map_view_add_polygon().
 *
 * @see map_view_add_polygon()
 * @see map_view_polygon_get_color()
 */
int map_view_object_polygon_set_fill_color(map_view_object_h polygon,
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
 * @pre @a polygon is created using map_view_add_polygon().
 *
 * @see map_view_add_polygon()
 */
int map_view_object_polygon_get_fill_color(const map_view_object_h polygon,
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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 * @see map_view_object_marker_get_coordinates()
 * @see #maps_coordinates_h
 */
int map_view_object_marker_set_coordinates(map_view_object_h marker,
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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 * @see map_view_object_marker_get_screen_size()
 */
int map_view_object_marker_set_screen_size(map_view_object_h marker,
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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 */
int map_view_object_marker_set_file_path(map_view_object_h marker,
					const char *file_path);

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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 * @see #map_view_marker_type_e
 */
int map_view_object_marker_set_type(map_view_object_h marker,
				   const map_view_marker_type_e type);

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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 */
int map_view_object_marker_set_text(map_view_object_h marker,
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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 * @see map_view_object_marker_set_coordinates()
 * @see #maps_coordinates_h
 */
int map_view_object_marker_get_coordinates(const map_view_object_h marker,
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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 * @see map_view_object_marker_set_screen_size()
 */
int map_view_object_marker_get_screen_size(map_view_object_h marker,
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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 * @see map_view_object_marker_set_file_path()
 */
int map_view_object_marker_get_file_path(const map_view_object_h marker,
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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 * @see #map_view_marker_type_e
 */
int map_view_object_marker_get_type(const map_view_object_h marker,
				   map_view_marker_type_e *type);

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
 * @pre @a marker is created using map_view_add_marker().
 *
 * @see map_view_add_marker()
 */
int map_view_object_marker_get_text(const map_view_object_h marker,
				   char **text);

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
 * @pre @a route is created using map_view_add_route().
 *
 * @see map_view_add_route()
 */
int map_view_object_route_get_content(const map_view_object_h route,
				      maps_route_h *content);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_VIEW_OBJECT_H__*/
