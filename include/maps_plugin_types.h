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

#ifndef __MAPS_PLUGIN_TYPES_H__
#define __MAPS_PLUGIN_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup	CAPI_MAPS_CONTAINERS_MODULE
 * @defgroup	CAPI_MAPS_CONTAINERS_MODULE_OBJECT_LIST Maps Object Lists
 *
 * @file maps_plugin_types.h
 * @brief This section contains API for using list of maps objects
 *
 * @addtogroup CAPI_MAPS_CONTAINERS_MODULE_OBJECT_LIST
 * @{
 * @brief This provides API for using list of maps objects.
 */

#include <glib.h>

/**
 * @brief	Structure of the coordinates list.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 * @remarks #maps_coordinates_list_h is a void pointer to the #maps_coordinates_list_s.
 *
 * @see maps_coordinates_list_create()
 * @see maps_coordinates_list_destroy()
 */
typedef GList maps_coordinates_list_s;

/**
 * @brief	Structure of the address list.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 * @remarks #maps_address_list_h is a void pointer to the #maps_address_list_s.
 *
 * @see maps_address_list_create()
 * @see maps_address_list_destroy()
 */
typedef GList maps_address_list_s;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif				/* __MAPS_PLUGIN_TYPES_H__ */


