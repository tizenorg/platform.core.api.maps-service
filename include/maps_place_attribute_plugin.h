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

#ifndef __MAPS_PLACE_ATTRIBUTE_PLUGIN_H__
#define __MAPS_PLACE_ATTRIBUTE_PLUGIN_H__

#include <maps_place_attribute.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_ATTRIBUTE_MODULE Attribute
 *
 * @file maps_place_attribute_plugin.h
 * @brief This file contains the functions related to Place Attributes
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_ATTRIBUTE_MODULE
 * @{
 * @brief This provides APIs related to Place Attributes information,
 * used in Place Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new place attribute handle.
 * @details This function creates a new place attribute handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a place attribute must be released using
 * maps_place_attribute_destroy().
 * \n @a attribute may be cloned using maps_place_attribute_clone().
 *
 * @param[out]	attribute	A handle of a new place attribute on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see maps_place_attribute_destroy()
 * @see maps_place_attribute_clone()
 */
int maps_place_attribute_create(maps_place_attribute_h *attribute);

/**
 * @brief	Sets the place attribute id.
 * @details This function sets the place attribute id.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	attribute	The handle to place attribute
 * @param[in]	id		The place attribute id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a attribute is created using maps_place_attribute_create().
 *
 * @see maps_place_attribute_create()
 * @see maps_place_attribute_get_id()
 */
int maps_place_attribute_set_id(maps_place_attribute_h attribute,
				const char *id);

/**
 * @brief	Sets the place attribute label.
 * @details This function sets the place attribute label.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	attribute	The handle to place attribute
 * @param[in]	label		The place attribute label
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a attribute is created using maps_place_attribute_create().
 *
 * @see maps_place_attribute_create()
 * @see maps_place_attribute_get_label()
 */
int maps_place_attribute_set_label(maps_place_attribute_h attribute,
				   const char *label);

/**
 * @brief	Sets the place attribute text.
 * @details This function sets the place attribute text.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	attribute	The handle to place attribute
 * @param[in]	text		The place attribute text
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a attribute is created using maps_place_attribute_create().
 *
 * @see maps_place_attribute_create()
 * @see maps_place_attribute_get_text()
 */
int maps_place_attribute_set_text(maps_place_attribute_h attribute,
				  const char *text);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_ATTRIBUTE_PLUGIN_H__ */
