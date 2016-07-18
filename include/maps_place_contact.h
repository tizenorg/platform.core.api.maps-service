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

#ifndef __MAPS_PLACE_CONTACT_H__
#define __MAPS_PLACE_CONTACT_H__

/**
 * @ingroup	CAPI_MAPS_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_CONTACT_MODULE Contact
 *
 * @file maps_place_contact.h
 * @brief This file contains the functions related to Place Contact information.
 *
 * @addtogroup CAPI_MAPS_PLACE_CONTACT_MODULE
 * @{
 * @brief This provides APIs related to Place Contact, used in Place Discovery
 * and Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Place Contact handle
 * @details The handle of Place Contact instance.
 * @remarks To release the handle use maps_place_contact_destroy().
 * \n To clone the handle use maps_place_contact_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see maps_place_contact_destroy()
 * @see maps_place_contact_clone()
 */
typedef void *maps_place_contact_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the place contact handle and releases all its
 * resources.
 * @details This function destroys the place contact handle and releases all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	contact		The place contact handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see maps_place_contact_clone()
 */
int maps_place_contact_destroy(maps_place_contact_h contact);

/**
 * @brief	Clones the place contact handle.
 * @details This function clones the place contact handle @a origin and all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a cloned must be released using maps_place_contact_destroy().
 *
 * @param[in]	origin		The original place contact handle
 * @param[out]	cloned		A cloned place contact handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see maps_place_contact_destroy()
 */
int maps_place_contact_clone(const maps_place_contact_h origin,
			     maps_place_contact_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the place contact label.
 * @details This function gets the place contact label.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a label must be released using free().
 *
 * @param[in]	contact		The handle to place contact
 * @param[out]	label		The place contact label
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_place_contact_get_label(const maps_place_contact_h contact,
				 char **label);

/**
 * @brief	Gets the place contact type.
 * @details This function gets the place contact type.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a type must be released using free().
 *
 * @param[in]	contact		The handle to place contact
 * @param[out]	type		The place contact type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_place_contact_get_type(const maps_place_contact_h contact,
				char **type);

/**
 * @brief	Gets the place contact value.
 * @details This function gets the place contact value.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a value must be released using free().
 *
 * @param[in]	contact		The handle to place contact
 * @param[out]	value		The place contact value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_place_contact_get_value(const maps_place_contact_h contact,
				 char **value);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_CONTACT_H__ */
