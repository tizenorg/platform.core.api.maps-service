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

#ifndef __MAPS_PLACE_ATTRIBUTE_H__
#define __MAPS_PLACE_ATTRIBUTE_H__

/**
 * @ingroup	CAPI_MAPS_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_ATTRIBUTE_MODULE Attribute
 *
 * @file maps_place_attribute.h
 * @brief This file contains the functions related to Place Attributes
 * information.
 *
 * @addtogroup CAPI_MAPS_PLACE_ATTRIBUTE_MODULE
 * @{
 * @brief This provides APIs related to Place Attributes information, used in
 * Place Discovery and Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Place Attribute handle
 * @details The handle of Place Attribute instance.
 * @remarks To release the handle use maps_place_attribute_destroy().
 * \n To clone the handle use maps_place_attribute_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @see maps_place_attribute_destroy()
 * @see maps_place_attribute_clone()
 */
typedef void *maps_place_attribute_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the place attribute handle and releases all its
 * resources.
 * @details This function destroys the place attribute handle and releases all
 * its resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	attribute	The place attribute handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_attribute_clone()
 */
int maps_place_attribute_destroy(maps_place_attribute_h attribute);

/**
 * @brief	Clones the place attribute handle.
 * @details This function clones the place attribute handle @a origin and all
 * its resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a cloned must be released using maps_place_attribute_destroy().
 *
 * @param[in]	origin		The original place attribute handle
 * @param[out]	cloned		A cloned place attribute handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_attribute_destroy()
 */
int maps_place_attribute_clone(const maps_place_attribute_h origin,
			       maps_place_attribute_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the place attribute id.
 * @details This function gets the place attribute id.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a id must be released using free().
 *
 * @param[in]	attribute	The handle to place attribute
 * @param[out]	id		The place attribute id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_attribute_get_id(const maps_place_attribute_h attribute,
				char **id);

/**
 * @brief	Gets the place link object label.
 * @details This function gets the place link object label.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a label must be released using free().
 *
 * @param[in]	attribute	The handle to place attribute
 * @param[out]	label		The place attribute label
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_attribute_get_label(const maps_place_attribute_h attribute,
				   char **label);

/**
 * @brief	Gets the place attribute text.
 * @details This function gets the place attribute text.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a text must be released using free().
 *
 * @param[in]	attribute	The handle to place attribute
 * @param[out]	text		The place attribute text
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_attribute_get_text(const maps_place_attribute_h attribute,
				  char **text);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_ATTRIBUTE_H__ */
