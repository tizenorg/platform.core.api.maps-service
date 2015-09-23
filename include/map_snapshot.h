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
#include <Evas.h>

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
 * @ingroup	CAPI_MAP_SNAPSHOT_MODULE
 * @defgroup	CAPI_MAP_SNAPSHOT_DATA_MODULE Snapshot
 *
 * @addtogroup CAPI_MAP_SNAPSHOT_DATA_MODULE
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
 *
 * @see map_view_capture_snapshot()
 * @see maps_service_capture_snapshot()
 * @see map_snapshot_destroy()
 */
typedef void *map_snapshot_h;


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
 */
int map_snapshot_destroy(map_snapshot_h snapshot);

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
