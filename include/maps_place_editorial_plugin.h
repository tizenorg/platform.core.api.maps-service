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

#ifndef __MAPS_PLACE_EDITORIAL_PLUGIN_H__
#define __MAPS_PLACE_EDITORIAL_PLUGIN_H__

#include <maps_place_editorial.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_EDITORIAL_MODULE Editorial
 *
 * @file maps_place_editorial_plugin.h
 * @brief This file contains the functions related to Place Editorial
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_EDITORIAL_MODULE
 * @{
 * @brief This provides APIs related to Place Editorial, used in Place
 * Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new place editorial handle.
 * @details This function creates a new place editorial handle and allocates all
 * needed resources.
 * @since_tizen 2.4
 * @remarks @a editorial must be released using maps_place_editorial_destroy().
 * \n @a editorial may be cloned using maps_place_editorial_clone().
 *
 * @param[out]	editorial	A handle of a new place editorial on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @see maps_place_editorial_destroy()
 * @see maps_place_editorial_clone()
 */
int maps_place_editorial_create(maps_place_editorial_h *editorial);

/**
 * @brief	Sets the place editorial description.
 * @details This function sets the place editorial description.
 * @since_tizen 2.4
 *
 * @param[in]	editorial		The handle of place editorial
 * @param[in]	description	The place editorial description
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a editorial is created using maps_place_editorial_create().
 *
 * @see maps_place_editorial_create()
 * @see maps_place_editorial_get_description()
 */
int maps_place_editorial_set_description(maps_place_editorial_h editorial,
					 const char *description);

/**
 * @brief	Sets the place editorial language.
 * @details This function sets the place editorial language.
 * @since_tizen 2.4
 *
 * @param[in]	editorial	The handle of place editorial
 * @param[in]	language	The place editorial language
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a editorial is created using maps_place_editorial_create().
 *
 * @see maps_place_editorial_create()
 * @see maps_place_editorial_get_language()
 */
int maps_place_editorial_set_language(maps_place_editorial_h editorial,
				      const char *language);

/**
 * @brief	Sets the place editorial media.
 * @details This function sets the place editorial media.
 * @since_tizen 2.4
 *
 * @param[in]	editorial	The handle of place editorial
 * @param[in]	media		The place editorial media
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a editorial is created using maps_place_editorial_create().
 *
 * @see maps_place_editorial_create()
 * @see maps_place_editorial_get_media()
 * @see maps_place_media_destroy
 */
int maps_place_editorial_set_media(maps_place_editorial_h editorial,
				   const maps_place_media_h media);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_EDITORIAL_PLUGIN_H__ */
