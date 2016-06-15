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

#ifndef __MAPS_PLACE_MEDIA_PLUGIN_H__
#define __MAPS_PLACE_MEDIA_PLUGIN_H__

#include <maps_place_media.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_MEDIA_MODULE Media
 *
 * @file maps_place_media_plugin.h
 * @brief This file contains the functions related to Place Media
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_MEDIA_MODULE
 * @{
 * @brief This provides APIs related to Place Media, used in Place
 * Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new place media handle.
 * @details This function creates a new place media handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a media must be released using maps_place_media_destroy().
 * \n @a media may be cloned using maps_place_media_clone().
 *
 * @param[out]	media		A handle of a new place media on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_media_destroy()
 * @see maps_place_media_clone()
 */
int maps_place_media_create(maps_place_media_h *media);

/**
 * @brief	Sets the place media attribution.
 * @details This function sets the place media attribution.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	media		The handle of place media
 * @param[in]	attribution	The place media attribution
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a media is created using maps_place_media_create().
 *
 * @see maps_place_media_create()
 * @see maps_place_media_get_attribution()
 */
int maps_place_media_set_attribution(maps_place_media_h media,
				     const char *attribution);

/**
 * @brief	Sets the place media supplier link.
 * @details This function sets the place media supplier link.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	media		The handle of place media
 * @param[in]	supplier	The place media supplier link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a media is created using maps_place_media_create().
 *
 * @see maps_place_media_create()
 * @see maps_place_media_get_supplier()
 */
int maps_place_media_set_supplier(maps_place_media_h media,
				  const maps_place_link_object_h supplier);

/**
 * @brief	Sets the place media via link.
 * @details This function sets the place media via link.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	media		The handle of place media
 * @param[in]	via		The place media via link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a media is created using maps_place_media_create().
 *
 * @see maps_place_media_create()
 * @see maps_place_media_get_via()
 */
int maps_place_media_set_via(maps_place_media_h media,
			     const maps_place_link_object_h via);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_MEDIA_PLUGIN_H__ */
