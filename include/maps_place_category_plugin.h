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

#ifndef __MAPS_PLACE_CATEGORY_PLUGIN_H__
#define __MAPS_PLACE_CATEGORY_PLUGIN_H__

#include <maps_place_category.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_CATEGORY_MODULE Category
 *
 * @file maps_place_category_plugin.h
 * @brief This file contains the functions related to Place Category
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_CATEGORY_MODULE
 * @{
 * @brief This provides APIs related to place Category information,
 * used in Place Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Creates a new place category handle.
 * @details This function creates a new place category handle and allocates all
 * needed resources.
 * @since_tizen 2.4
 * @remarks @a category must be released using maps_place_category_destroy().
 * \n @a category may be cloned using maps_place_category_clone().
 *
 * @param[out]	category	A handle of a new place category on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_category_destroy()
 * @see maps_place_category_clone()
 */
int maps_place_category_create(maps_place_category_h *category);

/**
 * @brief	Sets the place category id.
 * @details This function sets the place category id.
 * @since_tizen 2.4
 *
 * @param[in]	category	The handle of place category
 * @param[in]	id		The place category id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a category is created using maps_place_category_create().
 *
 * @see maps_place_category_create()
 * @see maps_place_category_get_id()
 */
int maps_place_category_set_id(maps_place_category_h category,
			       const char *id);

/**
 * @brief	Sets the place category name.
 * @details This function sets the place category name.
 * @since_tizen 2.4
 *
 * @param[in]	category	The handle of place category
 * @param[in]	name		The place category name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a category is created using maps_place_category_create().
 *
 * @see maps_place_category_create()
 * @see maps_place_category_get_name()
 */
int maps_place_category_set_name(maps_place_category_h category,
				 const char *name);

/**
 * @brief	Sets the place category URL.
 * @details This function sets the place category URL.
 * @since_tizen 2.4
 *
 * @param[in]	category	The handle of place category
 * @param[in]	url		The place category URL
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a category is created using maps_place_category_create().
 *
 * @see maps_place_category_create()
 * @see maps_place_category_get_url()
 */
int maps_place_category_set_url(maps_place_category_h category,
				const char *url);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_CATEGORY_PLUGIN_H__ */
