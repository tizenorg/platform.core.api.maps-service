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

#ifndef __MAPS_PLACE_LINK_OBJECT_H__
#define __MAPS_PLACE_LINK_OBJECT_H__

/**
 * @ingroup	CAPI_MAPS_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_LINK_MODULE Link
 *
 * @file maps_place_link_object.h
 * @brief This file contains the functions related to Place Link Object
 * information.
 *
 * @addtogroup CAPI_MAPS_PLACE_LINK_MODULE
 * @{
 * @brief This provides APIs related to Place Link Object information, used in
 * Place Discovery and Search.
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief	The Place Link Object handle
 * @details The handle of Place Link Object instance.
 * @remarks To release the handle use maps_place_link_object_destroy().
 * \n To clone the handle use maps_place_link_object_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see maps_place_link_object_destroy()
 * @see maps_place_link_object_clone()
 */
typedef void *maps_place_link_object_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the place link object handle and releases all its
 * resources.
 * @details This function destroys the place link object handle and releases all
 * its resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	link		The place link object handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_link_object_clone()
 */
int maps_place_link_object_destroy(maps_place_link_object_h link);

/**
 * @brief	Clones the place link object handle.
 * @details This function clones the place link object handle @a origin and all
 * its resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a cloned must be released using maps_place_link_object_destroy().
 *
 * @param[in]	origin		The original place link object handle
 * @param[out]	cloned		A cloned place link object handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_link_object_destroy()
 */
int maps_place_link_object_clone(const maps_place_link_object_h origin,
				 maps_place_link_object_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the place link object id.
 * @details This function gets the place link object id.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a id must be released using free().
 *
 * @param[in]	link		The handle of place link object
 * @param[out]	id		The place link object id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_link_object_get_id(const maps_place_link_object_h link,
				  char **id);

/**
 * @brief	Gets the place link object name.
 * @details This function gets the place link object name.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a name must be released using free().
 *
 * @param[in]	link		The handle of place link object
 * @param[out]	name		The place link object name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_link_object_get_name(const maps_place_link_object_h link,
				    char **name);

/**
 * @brief	Gets the place link object string.
 * @details This function gets the place link object string.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a string must be released using free().
 *
 * @param[in]	link		The handle of place link object
 * @param[out]	string		The place link object string
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_link_object_get_string(const maps_place_link_object_h link,
				      char **string);

/**
 * @brief	Gets the place link object type.
 * @details This function gets the place link object type.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a type must be released using free().
 *
 * @param[in]	link		The handle of place link object
 * @param[out]	type		The place link object type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_link_object_get_type(const maps_place_link_object_h link,
				    char **type);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_LINK_OBJECT_H__ */
