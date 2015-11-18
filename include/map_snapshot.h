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

#ifndef __MAP_SNAPSHOT_H__
#define __MAP_SNAPSHOT_H__

#include <tizen_type.h>
#include <maps_error.h>
#include <maps_area.h>
#include <maps_coordinates.h>

/**
 *
 * @file map_snapshot.h
 * @brief This file contains the Map Snapshot API
 *
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAP_SNAPSHOT_MODULE Snapshot
 *
 * @addtogroup CAPI_MAP_SNAPSHOT_MODULE
 * @{
 * @brief This provides APIs allowing to capture Map Snapshots.
 *
 */

/**
 * @brief	Handle of the Map Snapshot.
 * @details The Snapshot handle can be obtained via call of
 * map_view_capture_snapshot() or maps_service_capture_snapshot().
 * @since_tizen 3.0
 * @remarks To release the handle use map_snapshot_destroy().
 * \n To clone the handle use map_snapshot_clone().
 *
 * @see map_view_capture_snapshot()
 * @see maps_service_capture_snapshot()
 * @see map_snapshot_destroy()
 * @see map_snapshot_clone()
 */
typedef void *map_snapshot_h;


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
 * @brief	Called when snapshot requested with the
 * maps_service_capture_snapshot().
 * @details The Maps Service invokes this callback while retrieving a snapshot
 * of the map, centered on  specified geographical coordinates with specified
 * zoom level and orientation.
 * \n If snapshot capturing is failed, the value of @a snapshot is NULL.
 * @since_tizen 3.0
 * @remarks The parameter @a snapshot must be released using
 * map_snapshot_destroy().
 *
 * @param[in]	result		The result of request
 * @param[in]	request_id	The id of request
 * @param[in]	snapshot	The snapshot
 * @param[in]	user_data	The pointer to user data passed from
 * maps_service_capture_snapshot()
 *
 * @pre maps_service_capture_snapshot() will invoke this callback.
 *
 * @see maps_service_capture_snapshot()
 * @see #map_snapshot_h
 */
typedef void (*maps_service_snapshot_cb) (maps_error_e result,
					 int request_id,
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
 * @brief	Gets the snapshot of a Map.
 * \n The request is asynchronous.
 * @details This function retrieves a snapshot of a map, centered on a specified
 * geographical coordinates with specified zoom level, orientation, expected
 * snapshot size on the screen and other
 * preferences.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice \n
 *            %http://tizen.org/privilege/internet \n
 *            %http://tizen.org/privilege/network.get
 * @remarks To check if Maps Provider is capable of capturing snapshots use
 * maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_CAPTURE_SNAPSHOT passed as @a service parameter.
 *
 * @param[in]	maps		The Maps Service handle
 * @param[in]	center		The geographical coordinates of map center on
 * the snapshot
 * @param[in]	zoom_level	The zoom level of the map on the snapshot
 * @param[in]	rotation_angle	The orientation of the map on the snapshot
 * @param[in]	width		The width of the snapshot on the screen
 * @param[in]	height		The height of the snapshot on the screen
 * @param[in]	preference	The set of preferences for snapshot capturing
 * @param[in]	callback	The callback which will receive the snapshot
 * @param[in]	user_data	The pointer to user data to be passed to the
 * callback function
 * @param[out]	request_id	The id of request
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NETWORK_UNREACHABLE Network connection failed
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_CONNECTION_TIME_OUT Timeout error, no answer
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_KEY_NOT_AVAILABLE Invalid key
 * @retval	#MAPS_ERROR_UNKNOWN Unknown error
 *
 * @pre Call maps_service_create() to create Maps Service and obtain its handle.
 * @pre Optionally, call maps_preference_create() to create Map Preference
 * holder and obtain its handle.
 * @post It invokes maps_service_snapshot_cb() to deliver requested snapshot.
 *
 * @par Example
 * @code
#include <maps_service.h>

static void _snapshot_cb(maps_error_e result, int request_id,
			 map_snapshot_h snapshot, void *user_data)
{
	if (!snapshot || !user_data)
		return false;

	unsigned int *pixels = NULL;
	Evas *canvas = (Evas *)user_data;
	Evas_Object *map_snapshot = NULL;
	int width = 0;
	int height = 0;

	// Get snapshot pixels
	unsigned int *pixels = NULL;
	int error = map_snapshot_get_data(snapshot, &pixels);
	if (error == MAPS_ERROR_NONE) {

		// Create an image on the canvas
		map_snapshot = evas_object_image_add(canvas);
		map_snapshot_get_size(snapshot, &width, &height);
		evas_object_image_size_set (map_snapshot, width, height);

		// Draw snapshot on the canvas
		evas_object_image_data_set(map_snapshot, pixels);
		evas_object_show(map_snapshot);
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
	int error = MAPS_ERROR_NONE;
	int request_id = 0;
	Evas *canvas = NULL;

	do {

		// Create an instance of Maps Service
		error = maps_service_create("MapsProvider", &maps);
		if (error != MAPS_ERROR_NONE)
			break;

		// Set security key
		maps_service_set_provider_key(maps, "your-security-key");

		// Create a canvas
		//canvas = ...

		// Specify Snapshot center
		maps_coordinates_h center = NULL;
		maps_coordinates_create(center, 39.930661, 23.695172);

		// Capture snapshot
		error = maps_service_capture_snapshot(maps,
			      center,
			      12,	// zoom level
			      0,	// rotation angle
			      320,	// width on the screen
			      240,	// height on the screen
			      NULL,	// no extra preference
			      _snapshot_cb,
			      canvas,	// user_data,
			      &request_id);

		maps_coordinates_destroy(center);

		if (error != MAPS_ERROR_NONE)
			break;

	} while(false);

	// Release the Maps Service and used resources
	maps_service_destroy(maps);
	return error;
}
 * @endcode
 *
 * @see maps_service_snapshot_cb()
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 * @see map_snapshot_destroy()
 * @see maps_service_create()
 * @see maps_preference_create()
 */
int maps_service_capture_snapshot(const maps_service_h maps,
				  const maps_coordingates_h center,
				  const int zoom_level,
				  const double rotation_angle,
				  const int width,
				  const int height,
				  const maps_preference_h preference,
				  maps_service_snapshot_cb callback,
				  void *user_data,
				  int *request_id);

/**
 * @brief	Destroys the Map Snapshot and releases all its resources.
 * @details This function destroys the Map Snapshot handle and releases all its
 * resources.
 * @since_tizen 3.0
 *
 * @param[in]	snapshot	The Snapshot handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a snapshot is retrieved in map_view_capture_snapshot() or
 * maps_service_capture_snapshot()
 *
 * @see map_view_capture_snapshot()
 * @see maps_service_capture_snapshot()
 * @see map_snapshot_clone()
 */
int map_snapshot_destroy(map_snapshot_h snapshot);

/**
 * @brief	Clones the Map Snapshot.
 * @details This function makes a clone of the @a origin Snapshot.
 * @since_tizen 3.0
 * @remarks @a cloned must be released using map_snapshot_destroy().
 *
 * @param[in]	origin		The original Snapshot handle
 * @param[out]	cloned		A pointer to #map_snapshot_h in which to store
 * the cloned Snapshot handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is retrieved in map_view_capture_snapshot() or
 * maps_service_capture_snapshot()
 *
 * @see #map_snapshot_h
 * @see map_view_capture_snapshot()
 * @see maps_service_capture_snapshot()
 * @see map_snapshot_destroy()
 */
int map_snapshot_clone(const map_snapshot_h origin, map_snapshot_h *cloned);

/**
 * @brief	Gets the snapshot pixel data.
 * @details This function gets the pixel data of the snapshot.
 * @remarks Release the @a data with free() when no needed.
 * @since_tizen 3.0
 *
 * @param[in]	snapshot	The snapshot handle
 * @param[out]	data		The pointer to snapshot pixels
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a snapshot is retrieved via maps_service_capture_snapshot() or
 * map_view_capture_snapshot().
 *
 * @see maps_service_capture_snapshot()
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 */
int map_snapshot_get_data(const map_snapshot_h snapshot,
			  void **data);

/**
 * @brief	Gets the snapshot color space.
 * @details This function gets the color space of the snapshot.
 * @since_tizen 3.0
 *
 * @param[in]	snapshot	The snapshot handle
 * @param[out]	colorspace	The pointer to Evas_Colorspace in which to store
 * the color space, used while capturing snapshot
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a snapshot is retrieved via maps_service_capture_snapshot() or
 * map_view_capture_snapshot().
 *
 * @see maps_service_capture_snapshot()
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 */
int map_snapshot_get_color_space(const map_snapshot_h snapshot,
				 Evas_Colorspace *colorspace);


/**
 * @brief	Gets the snapshot center.
 * @details This function gets the geographical coordinates of snapshot center.
 * @remarks Release the @a center with maps_coordinates_destroy().
 * @since_tizen 3.0
 *
 * @param[in]	snapshot	The snapshot handle
 * @param[out]	center		The pointer to #maps_coordinates_h in which to
 * store the handle of snapshot center
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a snapshot is retrieved via maps_service_capture_snapshot() or
 * map_view_capture_snapshot().
 *
 * @see maps_service_capture_snapshot()
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 */
int map_snapshot_get_center(const map_snapshot_h snapshot,
			    maps_coordinates_h *center);

/**
 * @brief	Gets the snapshot area.
 * @details This function gets the geographical area corresponding to the
 * snapshot.
 * @remarks Release the @a area with maps_area_destroy().
 * @since_tizen 3.0
 *
 * @param[in]	snapshot	The snapshot handle
 * @param[out]	area		The pointer to #maps_area_h in which to store
 * the handle of the snapshot area
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a snapshot is retrieved via maps_service_capture_snapshot() or
 * map_view_capture_snapshot().
 *
 * @see maps_service_capture_snapshot()
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 */
int map_snapshot_get_area(const map_snapshot_h snapshot,
			  maps_area_h *area);


/**
 * @brief	Gets the snapshot zoom level.
 * @details This function gets the snapshot zoom level.
 * @since_tizen 3.0
 * @remarks Note, the snapshot is made asynchronously, so it is preferable to
 * use this function instead of map_view_get_zoom_level(): zoom level might be
 * changed already.
 *
 * @param[in]	snapshot	The snapshot handle
 * @param[out]	zoom_level	The pointer to an integer in which to store the
 * snapshot zoom level
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a snapshot is retrieved via maps_service_capture_snapshot() or
 * map_view_capture_snapshot().
 *
 * @see maps_service_capture_snapshot()
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 */
int map_snapshot_get_zoom_level(const map_snapshot_h snapshot, int *zoom_level);

/**
 * @brief	Gets the snapshot zoom factor.
 * @details This function gets the snapshot zoom factor.
 * @since_tizen 3.0
 *
 * @param[in]	snapshot	The snapshot handle
 * @param[out]	zoom_factor	The pointer to a double in which to store the
 * snapshot zoom factor
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a snapshot is retrieved via maps_service_capture_snapshot() or
 * map_view_capture_snapshot().
 *
 * @see maps_service_capture_snapshot()
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 */
int map_snapshot_get_zoom_factor(const map_snapshot_h snapshot,
				 double *zoom_factor);

/**
 * @brief	Gets the snapshot orientation.
 * @details This function gets the snapshot orientation.
 * @since_tizen 3.0
 *
 * @param[in]	snapshot	The snapshot handle
 * @param[out]	rotation_angle	The pointer to a double in which to store the
 * snapshot orientation
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a snapshot is retrieved via maps_service_capture_snapshot() or
 * map_view_capture_snapshot().
 *
 * @see maps_service_capture_snapshot()
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 */
int map_snapshot_get_orientation(const map_snapshot_h snapshot,
				 double *rotation_angle);

/**
 * @brief	Gets the snapshot screen size.
 * @details This function gets the snapshot size on the screen.
 * @since_tizen 3.0
 *
 * @param[in]	snapshot	The snapshot handle
 * @param[out]	width		The pointer to an integer in which to store the
 * snapshot width, in screen units
 * @param[out]	height		The pointer to an integer in which to store the
 * snapshot height, in screen units
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a snapshot is retrieved via maps_service_capture_snapshot() or
 * map_view_capture_snapshot().
 *
 * @see maps_service_capture_snapshot()
 * @see map_view_capture_snapshot()
 * @see #map_snapshot_h
 */
int map_snapshot_get_size(const map_snapshot_h snapshot,
			  int *width,
			  int *height);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif				/* __MAPS_SNAPSHOT_H__ */
