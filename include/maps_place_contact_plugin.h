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

#ifndef __MAPS_PLACE_CONTACT_PLUGIN_H__
#define __MAPS_PLACE_CONTACT_PLUGIN_H__

#include <maps_place_contact.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_CONTACT_MODULE Contact
 *
 * @file maps_place_contact_plugin.h
 * @brief This file contains the functions related to Place Contact
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_CONTACT_MODULE
 * @{
 * @brief This provides APIs related to Place Contact, used in Place
 * Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new place contact handle.
 * @details This function creates a new place contact handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a place contact must be released using
 * maps_place_contact_destroy().
 * \n @a contact may be cloned using maps_place_contact_clone().
 *
 * @param[out]	contact	A handle of a new place contact on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see place_contact_destroy()
 * @see maps_place_contact_clone()
 */
int maps_place_contact_create(maps_place_contact_h *contact);

/**
 * @brief	Sets the place contact label
 * @details This function sets the place contact label.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	contact		The handle of contact contact
 * @param[in]	label		The place contact label
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a contact is created using maps_place_contact_create().
 *
 * @see maps_place_contact_create()
 * @see maps_place_contact_get_label()
 */
int maps_place_contact_set_label(maps_place_contact_h contact,
				 const char *label);

/**
 * @brief	Sets the place contact type
 * @details This function sets the place contact type.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	contact		The handle to place contact
 * @param[in]	type		The place contact type
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a contact is created using maps_place_contact_create().
 *
 * @see maps_place_contact_create()
 * @see maps_place_contact_get_type()
 */
int maps_place_contact_set_type(maps_place_contact_h contact,
				const char *type);

/**
 * @brief	Sets the place contact value
 * @details This function sets the place contact value.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	contact		The handle to place contact
 * @param[in]	value		The place contact value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a contact is created using maps_place_contact_create().
 *
 * @see maps_place_contact_create()
 * @see maps_place_contact_get_value()
 */
int maps_place_contact_set_value(maps_place_contact_h contact,
				 const char *value);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_CONTACT_PLUGIN_H__ */
