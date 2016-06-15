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

#ifndef __MAPS_PLACE_IMAGE_PLUGIN_H__
#define __MAPS_PLACE_IMAGE_PLUGIN_H__

#include <maps_place_image.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_IMAGE_MODULE Image
 *
 * @file maps_place_image_plugin.h
 * @brief This file contains the functions related to Place Image
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_IMAGE_MODULE
 * @{
 * @brief This provides APIs related to Place Image, used in Place
 * Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new place image handle.
 * @details This function creates a new place image handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a image must be released using maps_place_image_destroy().
 * \n @a image may be cloned using maps_place_image_clone().
 *
 * @param[out]	image		A handle of a new place image on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_image_destroy()
 * @see maps_place_image_clone()
 */
int maps_place_image_create(maps_place_image_h *image);

/**
 * @brief	Sets the place image id.
 * @details This function sets the place image id.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	image		The handle of place image
 * @param[in]	id		The place image id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a image is created using maps_place_image_create().
 *
 * @see maps_place_image_create()
 * @see maps_place_image_get_id()
 */
int maps_place_image_set_id(maps_place_image_h image, const char *id);

/**
 * @brief	Sets the place image URL.
 * @details This function sets the place image URL.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	image		The handle of place image
 * @param[in]	url		The place image url
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a image is created using maps_place_image_create().
 *
 * @see maps_place_image_create()
 * @see maps_place_image_get_url()
 */
int maps_place_image_set_url(maps_place_image_h image, const char *url);

/**
 * @brief	Sets the place image width.
 * @details This function sets the place image width.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	image		The handle of place image
 * @param[in]	width		The place image width
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a image is created using maps_place_image_create().
 *
 * @see maps_place_image_create()
 * @see maps_place_image_get_width()
 */
int maps_place_image_set_width(maps_place_image_h image, const int width);

/**
 * @brief	Sets the place image height.
 * @details This function sets the place image height.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	image		The handle of place image
 * @param[in]	height		The place image height
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a image is created using maps_place_image_create().
 *
 * @see maps_place_image_create()
 * @see maps_place_image_get_height()
 */
int maps_place_image_set_height(maps_place_image_h image, const int height);

/**
 * @brief	Sets the place image user link.
 * @details This function sets the place image user link.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	image		The handle of place image
 * @param[in]	user		The place image user link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a image is created using maps_place_image_create().
 *
 * @see maps_place_image_create()
 * @see maps_place_image_get_user_link()
 * @see #maps_place_link_object_h
 */
int maps_place_image_set_user_link(maps_place_image_h image,
				   const maps_place_link_object_h user);

/**
 * @brief	Sets the place image media.
 * @details This function sets the place image media.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	image		The handle of place image
 * @param[in]	media		The place image media
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a image is created using maps_place_image_create().
 *
 * @see maps_place_image_create()
 * @see maps_place_image_get_media()
 * @see #maps_place_media_h
 */
int maps_place_image_set_media(maps_place_image_h image,
			       const maps_place_media_h media);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_IMAGE_PLUGIN_H__ */
