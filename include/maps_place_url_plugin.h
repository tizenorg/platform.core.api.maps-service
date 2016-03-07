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

#ifndef __MAPS_PLACE_URL_PLUGIN_H__
#define __MAPS_PLACE_URL_PLUGIN_H__

#include <maps_place_url.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_URL_MODULE URL
 *
 * @file maps_place_url_plugin.h
 * @brief This file contains the functions related to Place URL
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_URL_MODULE
 * @{
 * @brief This provides APIs related to Place URL, used in Place
 * Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new place URL handle.
 * @details This function creates a new place URL handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a url must be released using maps_place_url_destroy().
 * \n @a url may be cloned using maps_place_url_clone().
 *
 * @param[out]	url		A handle of a new place URL on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_url_destroy()
 * @see maps_place_url_clone()
 */
int maps_place_url_create(maps_place_url_h *url);

/**
 * @brief	Sets the place URL path.
 * @details This function sets the place URL path.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	url		The handle of place URL
 * @param[in]	path		The place URL path
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a url is created using maps_place_url_create().
 *
 * @see maps_place_url_create()
 * @see maps_place_url_get_path()
 */
int maps_place_url_set_path(maps_place_url_h url, const char *path);

/**
 * @brief	Sets the place URL description.
 * @details This function sets the place URL description.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	url		The handle of place URL
 * @param[in]	description	The place URL description
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a url is created using maps_place_url_create().
 *
 * @see maps_place_url_create()
 * @see maps_place_url_get_description()
 */
int maps_place_url_set_description(maps_place_url_h url,
				   const char *description);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_URL_PLUGIN_H__ */
