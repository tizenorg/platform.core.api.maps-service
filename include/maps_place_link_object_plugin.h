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

#ifndef __MAPS_PLACE_LINK_OBJECT_PLUGIN_H__
#define __MAPS_PLACE_LINK_OBJECT_PLUGIN_H__

#include <maps_place_link_object.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_LINK_MODULE Link
 *
 * @file maps_place_link_object_plugin.h
 * @brief This file contains the functions related to Place Link Object
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_LINK_MODULE
 * @{
 * @brief This provides APIs related to Place Link Object, used in
 * Place Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new place link object handle.
 * @details This function creates a new place link object handle and allocates
 * all needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a link must be released using maps_place_link_object_destroy().
 * \n @a link may be cloned using maps_place_link_object_clone().
 *
 * @param[out]	link		A handle of a new place link object on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_link_object_destroy()
 * @see maps_place_link_object_clone()
 */
int maps_place_link_object_create(maps_place_link_object_h *link);

/**
 * @brief	Sets the place link object id.
 * @details This function sets the place link object id.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	link		The handle of place link object
 * @param[in]	id		The place link object id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a link is created using maps_place_link_object_create().
 *
 * @see maps_place_link_object_create()
 * @see maps_place_link_object_get_id()
 */
int maps_place_link_object_set_id(maps_place_link_object_h link,
				  const char *id);

/**
 * @brief	Sets the place link object string.
 * @details This function sets the place link object string.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	link		The handle of place link object
 * @param[in]	string		The place link object string
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a link is created using maps_place_link_object_create().
 *
 * @see maps_place_link_object_create()
 * @see maps_place_link_object_get_string()
 */
int maps_place_link_object_set_string(maps_place_link_object_h link,
				      const char *string);

/**
 * @brief	Sets the place link object type.
 * @details This function sets the place link object type.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	link		The handle of place link object
 * @param[in]	type		The place link object type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a link is created using maps_place_link_object_create().
 *
 * @see maps_place_link_object_create()
 * @see maps_place_link_object_get_type()
 */
int maps_place_link_object_set_type(maps_place_link_object_h link,
				    const char *type);

/**
 * @brief	Sets the place link object name.
 * @details This function sets the place link object name.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	link		The handle of place link object
 * @param[in]	name		The place link object name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a link is created using maps_place_link_object_create().
 *
 * @see maps_place_link_object_create()
 * @see maps_place_link_object_get_name()
 */
int maps_place_link_object_set_name(maps_place_link_object_h link,
				    const char *name);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_LINK_OBJECT_PLUGIN_H__ */
