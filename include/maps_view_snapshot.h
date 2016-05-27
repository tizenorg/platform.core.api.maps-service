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

#ifndef __MAPS_VIEW_SNAPSHOT_H__
#define __MAPS_VIEW_SNAPSHOT_H__


/**
 * @ingroup	CAPI_MAPS_VIEW_MODULE
 * @defgroup	CAPI_MAPS_VIEW_SNAPSHOT_MODULE Snapshot
 *
 * @file maps_view_snapshot.h
 * @brief This file contains the functions of Map Snapshot API
 *
 * @addtogroup CAPI_MAPS_VIEW_SNAPSHOT_MODULE
 * @{
 * @brief This provides APIs related to take snapshots.
 *
 */


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Enumerations of snapshot file format type
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum _maps_view_snapshot_format_type_e
{
	MAPS_VIEW_SNAPSHOT_BMP,		/**< Indicates the BMP format type */
	MAPS_VIEW_SNAPSHOT_JPEG,	/**< Indicates the JPEG format type */
} maps_view_snapshot_format_type_e;

/**
 * @brief	Captures a snapshot of the Map View.
 * \n The request is synchronous.
 * @details This function retrieves an image of a map currently represented on
 * the Map View.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/mapservice
 * @remarks To check if Maps Provider is capable of capturing snapshots use
 * maps_service_provider_is_service_supported() with
 * #MAPS_SERVICE_VIEW_SNAPSHOT passed as @a service parameter.
 *
 * @param[in]	view	The Map View handle
 * @param[in]	type	The type of file format
 * @param[in]	quality	The quality for encoding (1~100)
 * @param[in]	path	The file path to be created
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_SERVICE_NOT_AVAILABLE Service not available
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 * @retval	#MAPS_ERROR_INVALID_OPERATION Operation is not valid
 *
 * @pre Call maps_service_create() and map_view_create() to issue Maps Service
 * and Map View handles respectively.
 * @post It invokes map_view_snapshot_cb() to deliver requested snapshot.
 *
 * @see maps_view_create()
 * @see maps_service_create()
 */
int maps_view_capture_snapshot(maps_view_h view,
								maps_view_snapshot_format_type type,
								int quality,
								const char *path);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_VIEW_SNAPSHOT_H__*/
