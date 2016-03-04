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
#include <maps_coordinates.h>

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
	MAP_OBJECT_POLYLINE,	/** Indicates the polyline */
	MAP_OBJECT_POLYGON,		/** Indicates the polygon */
	MAP_OBJECT_MARKER,		/** Indicates the marker */
} map_object_type_e;

/**
 * @brief	Enumerations of map marker types
 * @since_tizen 3.0
 */
typedef enum _map_marker_type_e {
	MAP_MARKER_PIN,			/** Indicates the pin marker type*/
	MAP_MARKER_STICKER,		/** Indicates the sticker marker type*/
} map_marker_type_e;

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
 * @param[in]	user_data	The user data pointer passed from
 * map_object_polyline_foreach_point()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre map_object_polyline_foreach_point() will invoke this callback.
 *
 * @see map_object_polyline_foreach_point()
 * @see #maps_coordinates_h
 */
typedef bool(*map_object_polyline_point_cb) (int index, int total,
	maps_coordinates_h point, void *user_data);


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
 * @param[in]	user_data	The user data pointer passed from
 * map_object_polygon_foreach_point()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre map_object_polygon_foreach_point() will invoke this callback.
 *
 * @see map_object_polygon_foreach_point()
 * @see #maps_coordinates_h
 */
typedef bool(*map_object_polygon_point_cb) (int index, int total,
	maps_coordinates_h point, void *user_data);


/*----------------------------------------------------------------------------*/
/*
 * Common Visual Object Features
 */


/**
 * @brief	Creates a marker visual object.
 * @details This function creates a marker on a given geographical coordinates.
 * The marker is specified with a given image file and a type.
 * @since_tizen 3.0
 * @remarks @a marker can be released by using map_object_destroy().
 * \n If added to the Map View using map_view_add_object(), @a marker will be
 * released automatically when the Map View is destroyed.
 * \nhttp://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.
 * \nhttp://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.
 *
 * @param[in]	coordinates	The geographical coordinates to add marker on
 * @param[in]	image_file_path	The file name with the image for the marker
 * @param[in]	type		The type of the marker, one of listed in map_marker_type_e
 * @param[out]	marker		The handle of newly created marker
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a coordinates are created using map_coordinates_create().
 *
 * @see #map_object_h
 * @see #map_marker_type_e
 * @see map_object_create_polygon()
 * @see map_object_create_polyline()
 * @see map_view_add_object()
 * @see map_object_destroy()
 * @see #maps_coordinates_h
 */
int map_object_create_marker(maps_coordinates_h coordinates,
	const char *image_file_path, map_marker_type_e type, map_object_h *marker);

/**
 * @brief	Creates a polyline visual object.
 * @details This function creates a polyline visual object, specified
 * with a list of geographical coordinates, line width and color.
 * @since_tizen 3.0
 * @remarks @a polyline can be released by using map_object_destroy().
 * \n If added to the Map View using map_view_add_object(), @a polyline will be
 * released automatically when the Map View is destroyed.
 *
 * @param[in]	coordinates	The list of geographical coordinates
 * @param[in]	r		The red component of polyline color [0 ~ 255]
 * @param[in]	g		The green component of polyline color [0 ~ 255]
 * @param[in]	b		The blue component of polyline color [0 ~ 255]
 * @param[in]	a		The alpha component of polyline color [0 ~ 255]
 * @param[in]	width		The width of line [1 ~ 100] (pixels)
 * @param[out]	polyline	The handle of newly added polyline
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates is created using maps_coordinates_list_create().
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polygon()
 * @see map_view_add_object()
 * @see map_object_destroy()
 * @see #maps_coordinates_list_h
 * @see maps_coordinates_list_create()
 */
int map_object_create_polyline(maps_coordinates_list_h coordinates,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a,
	int width, map_object_h *polyline);

/**
 * @brief	Create a polygon visual object.
 * @details This function creates a polygon visual object, specified
 * with a list of geographical coordinates and fill color.
 * @since_tizen 3.0
 * @remarks @a polygon can be released by using map_object_destroy().
 * \n If added to the Map View using map_view_add_object(), @a polygon will be
 * released automatically when the Map View is destroyed.
 *
 * @param[in]	coordinates	The list of geographical coordinates
 * @param[in]	r		The red component of polygon background color [0 ~ 255]
 * @param[in]	g		The green component of polygon background color [0 ~ 255]
 * @param[in]	b		The blue component of polygon background color [0 ~ 255]
 * @param[in]	a		The alpha component of polygon background color [0 ~ 255]
 * @param[out]	polygon		The handle of newly created polygon
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates is created using maps_coordinates_list_create().
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_view_add_object()
 * @see map_object_destroy()
 * @see #maps_coordinates_list_h
 * @see maps_coordinates_list_create()
 */
int map_object_create_polygon(maps_coordinates_list_h coordinates,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a,
	map_object_h *polygon);

/**
 * @brief	Destroys the object.
 * @details This function destroys the object handle and releases all its resources.
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
 * map_object_create_polyline() or map_object_create_polygon().
 *
 * @see #map_object_h
 * @see map_object_create_marker()
 * @see map_object_create_polyline()
 * @see map_object_create_polygon()
 */
int map_object_get_type(map_object_h object, map_object_type_e *type);

/**
 * @brief	Shows the object.
 * @details This function changes the visibility of the given object on the map view.
 * @since_tizen 3.0
 *
 * @param[in]	object		The object handle
 * @param[in]	visible		The new visibility of the object
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a object is created using map_object_create_marker(),
 * map_object_create_polyline(), map_object_create_polygon().
 *
 * @see #map_object_h
 * @see map_object_get_visible()
 */
int map_object_set_visible(map_object_h object, bool visible);

/**
 * @brief	Gets the object visibility.
 * @details This function retrieves whether or not the given object is visible.
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
 * map_object_create_polyline(), map_object_create_polygon().
 *
 * @see #map_object_h
 * @see map_object_set_visible()
 */
int map_object_get_visible(const map_object_h object, bool *visible);


/*----------------------------------------------------------------------------*/
/*
 * Polyline
 */

/**
 * @brief	Sets points to the polyline.
 * @details This function sets point list to the polyline.
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
 * @post Previous points will be destroyed automatically.
 *
 * @see #map_object_h
 * @see #maps_coordinates_list_h
 * @see maps_coordinates_list_create()
 * @see map_object_create_polyline()
 * @see map_object_polyline_foreach_point()
 */
int map_object_polyline_set_polyline(map_object_h polyline, maps_coordinates_list_h points);

/**
 * @brief	Retrieves all points, added to the polyline.
 * @details This function retrieves all points, added to the polyline.
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
 *
 * @pre @a polyline is created using map_object_create_polyline().
 * @post This function invokes map_object_polyline_point_cb() repeatedly to
 * retrieve each point.
 *
 * @see #map_object_h
 * @see map_object_create_polyline()
 * @see map_object_polyline_set_polyline()
 * @see map_object_polyline_point_cb()
 */
int map_object_polyline_foreach_point(map_object_h polyline,
	maps_coordinates_cb callback, void *user_data);

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
 * @see map_object_polyline_get_color()
 * @see map_object_polyline_set_width()
 * @see map_object_create_polyline()
 */
int map_object_polyline_set_color(map_object_h polyline,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a);

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
 * @see map_object_polyline_set_color()
 * @see map_object_polyline_get_width()
 * @see map_object_create_polyline()
 */
int map_object_polyline_get_color(const map_object_h polyline,
	unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a);

/**
 * @brief	Sets the polyline width.
 * @details This function sets the polyline width on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[in]	width		The new width of line [1 ~ 100] (pixels)
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 *
 * @see map_object_polyline_get_width()
 * @see map_object_polyline_set_color()
 * @see map_object_create_polyline()
 */
int map_object_polyline_set_width(map_object_h polyline, int width);

/**
 * @brief	Gets the polyline width.
 * @details This function gets the polyline width on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polyline	The polyline object handle
 * @param[out]	width		The width of line [1 ~ 100] (pixels)
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polyline is created using map_object_create_polyline().
 * @pre @a width may be previously set using map_object_polyline_set_width().
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
 * @brief	Sets points to the polygon.
 * @details This function sets point list to the polygon.
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
 * @see map_object_create_polygon()
 * @see #map_object_h
 * @see #maps_coordinates_list_h
 * @see maps_coordinates_list_create()
 * @see map_object_polygon_foreach_point()
 */
int map_object_polygon_set_polygon(map_object_h polygon, maps_coordinates_list_h points);

/**
 * @brief	Retrieves all points, added to the polygon.
 * @details This function retrieves all points, added to the polygon.
 * @since_tizen 3.0
 * @remarks The objects will be delivered via map_object_polygon_point_cb().
 *
 * @param[in]	polygon		The polygon object handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback function
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
 * @see map_object_polygon_point_cb()
 * @see map_object_create_polygon()
 * @see #map_object_h
 * @see map_object_polygon_set_polygon()
 */
int map_object_polygon_foreach_point(map_object_h polygon,
	maps_coordinates_cb callback, void *user_data);

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
 * @see map_object_polygon_get_fill_color()
 * @see map_object_create_polygon()
 */
int map_object_polygon_set_fill_color(map_object_h polygon,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a);

/**
 * @brief	Gets polygon fill color.
 * @details This function gets the polygon fill color on canvas.
 * @since_tizen 3.0
 *
 * @param[in]	polygon		The polygon data handle
 * @param[in]	r		The unsigned char pointer in which to store the
 * red component of the background color
 * @param[in]	g		The unsigned char pointer in which to store the
 * green component of the background color
 * @param[in]	b		The unsigned char pointer in which to store the
 * blue component of the background color
 * @param[in]	a		The unsigned char pointer in which to store the
 * alpha component of the background color
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a polygon is created using map_object_create_polygon().
 * @pre fill color components may be previously set using
 * map_object_polygon_set_fill_color().
 *
 * @see map_object_polygon_set_fill_color()
 * @see map_object_create_polygon()
 */
int map_object_polygon_get_fill_color(const map_object_h polygon,
	unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a);


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
 * @param[in]	coordinates	The marker geographical coordinates handle
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see map_object_create_marker()
 * @see map_object_marker_get_coordinates()
 * @see #maps_coordinates_h
 * @see maps_coordinates_create()
 */
int map_object_marker_set_coordinates(map_object_h marker, maps_coordinates_h coordinates);

/**
 * @brief	Sets the marker screen size.
 * @details This function sets the marker screen size.
 * @since_tizen 3.0
 *
 * @param[in]	marker	The marker object handle
 * @param[in]	width	The marker pixels in width on the screen
 * @param[in]	height	The marker pixels in height on the screen
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 * @see map_object_marker_get_size()
 */
int map_object_marker_resize(map_object_h marker, int width, int height);

/**
 * @brief	Sets the marker image file path.
 * @details This function sets the marker image file path.
 * @since_tizen 3.0
 * @remarks http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.
 * \nhttp://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.
 *
 * @param[in]	marker		The marker object handle
 * @param[in]	file_path	The marker image file path
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_marker_get_image_file()
 * @see map_object_create_marker()
 */
int map_object_marker_set_image_file(map_object_h marker, const char *file_path);

/**
 * @brief	Gets the marker image file path.
 * @details This function gets the marker image file path.
 * @remarks The @a file_path should be freed using free().
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	file_path	The marker image file path
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 * @pre @a file_path may be set previously using map_object_marker_set_image_file().
 *
 * @see map_object_marker_set_image_file()
 * @see map_object_create_marker()
 */
int map_object_marker_get_image_file(const map_object_h marker, char **file_path);

/**
 * @brief	Gets the marker coordinates.
 * @details This function gets the marker geographical coordinates.
 * @remarks The @a coordinates should be freed using maps_coordinates_destroy().
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	coordinates	The pointer to #maps_coordinates_h in which to
 * store the marker geographical coordinates
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 * @pre @a coordinates may be set previously using map_object_marker_set_coordinates().
 *
 * @see map_object_marker_set_coordinates()
 * @see #maps_coordinates_h
 * @see map_object_create_marker()
 */
int map_object_marker_get_coordinates(const map_object_h marker, maps_coordinates_h *coordinates);

/**
 * @brief	Gets the marker screen size.
 * @details This function gets the marker size on the screen.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	width		The pointer to an integer in which to store the
 * marker pixels in width on the screen
 * @param[out]	height		The pointer to an integer in which to store the
 * marker pixels in height on the screen
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 * @pre @a width ind @a height may be set previously using map_object_marker_resize().
 *
 * @see map_object_marker_resize()
 * @see map_object_create_marker()
 */
int map_object_marker_get_size(const map_object_h marker, int *width, int *height);

/**
 * @brief	Gets the marker type.
 * @details This function gets the marker type.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	type		The pointer to a map_marker_type_e in which to
 * store the marker type
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see #map_marker_type_e
 * @see map_object_create_marker()
 */
int map_object_marker_get_type(const map_object_h marker, map_marker_type_e *type);

/**
 * @brief	Sets the marker z-order.
 * @details This function sets the z-order.
 * @remarks The @a z_order must be in range of [-100, 100].
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[in]	z_order		The z-order
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 * @see map_object_marker_get_z_order()
 */
int map_object_marker_set_z_order(map_object_h marker, int z_order);

/**
 * @brief	Gets the marker z-order.
 * @details This function gets the z-order.
 * @since_tizen 3.0
 *
 * @param[in]	marker		The marker object handle
 * @param[out]	z_order		The z-order
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a marker is created using map_object_create_marker().
 *
 * @see map_object_create_marker()
 * @see map_object_marker_set_z_order()
 */
int map_object_marker_get_z_order(const map_object_h marker, int *z_order);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_OBJECT_H__*/
