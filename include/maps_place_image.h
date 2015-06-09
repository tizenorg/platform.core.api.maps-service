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

#ifndef __MAPS_PLACE_IMAGE_H__
#define __MAPS_PLACE_IMAGE_H__

#include <maps_place_link_object.h>
#include <maps_place_media.h>

/**
 * @ingroup	CAPI_MAPS_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_IMAGE_MODULE Image
 *
 * @file maps_place_image.h
 * @brief This function sets the place category id.
 *
 * @addtogroup CAPI_MAPS_PLACE_IMAGE_MODULE
 * @{
 * @brief This provides APIs related to Place Image information, used in Place
 * Discovery and Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Place Image handle
 * @details The handle of Place Image instance.
 * @remarks To release the handle use maps_place_image_destroy().
 * \n To clone the handle use maps_place_image_clone().
 * @since_tizen 2.4
 *
 * @see maps_place_image_destroy()
 * @see maps_place_image_clone()
 */
typedef void *maps_place_image_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the place image handle and releases all its resources.
 * @details This function destroys the place image handle and releases all its
 * resources.
 * @since_tizen 2.4
 *
 * @param[in]	image		The place image handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_image_clone()
 */
int maps_place_image_destroy(maps_place_image_h image);

/**
 * @brief	Clones the place image handle.
 * @details This function clones the place image handle @a origin and all its
 * resources.
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_place_image_destroy().
 *
 * @param[in]	origin		The original place image handle
 * @param[out]	cloned		A cloned place image handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_image_destroy()
 */
int maps_place_image_clone(const maps_place_image_h origin,
			   maps_place_image_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the place image id.
 * @details This function gets the place image id.
 * @since_tizen 2.4
 * @remarks @a id must be released using free().
 *
 * @param[in]	image		The handle of place image
 * @param[out]	id		The place image id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_image_get_id(const maps_place_image_h image, char **id);

/**
 * @brief	Gets the place image URL.
 * @details This function gets the place image URL.
 * @since_tizen 2.4
 * @remarks @a url must be released using free().
 *
 * @param[in]	image		The handle of place image
 * @param[out]	url		The place image url
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_image_get_url(const maps_place_image_h image, char **url);

/**
 * @brief	Gets the place image width.
 * @details This function gets the place image width.
 * @since_tizen 2.4
 *
 * @param[in]	image		The handle of place image
 * @param[out]	width		The place image width
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_image_get_width(const maps_place_image_h image, int *width);

/**
 * @brief	Gets the place image height.
 * @details This function gets the place image height.
 * @since_tizen 2.4
 *
 * @param[in]	image		The handle of place image
 * @param[out]	height	The place image height
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_image_get_height(const maps_place_image_h image, int *height);

/**
 * @brief	Gets the place image user link.
 * @details This function gets the place image user link.
 * @since_tizen 2.4
 * @remarks @a user must be released using maps_place_link_object_destroy().
 *
 * @param[in]	image		The handle of place image
 * @param[out]	user		The place image user link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_place_link_object_h
 */
int maps_place_image_get_user_link(const maps_place_image_h image,
				   maps_place_link_object_h *user);

/**
 * @brief	Gets the place image media.
 * @details This function gets the place image media.
 * @since_tizen 2.4
 * @remarks @a media must be released using maps_place_media_destroy().
 *
 * @param[in]	image		The handle of place image
 * @param[out]	media		The place image media
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_place_media_h
 */
int maps_place_image_get_media(const maps_place_image_h image,
			       maps_place_media_h *media);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_IMAGE_H__ */
