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

#ifndef __MAPS_TYPES_H__
#define __MAPS_TYPES_H__

/**
 *
 * @file maps_types.h
 * @brief This file contains the top level functions of Maps API
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAPS_SERVICE_TYPES_MODULE Primary types
 *
 * @addtogroup CAPI_MAPS_SERVICE_TYPES_MODULE
 * @{
 * @brief This provides definitions of primary types.
 *
 */


/**
 * @brief	The Maps Service handle
 * @details The Maps Service handle can be created by calling of maps_service_create().
 * \n To release the handle use maps_service_destroy().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see maps_service_create()
 * @see maps_service_destroy()
 */
typedef void *maps_service_h;


/**
 * @brief	The View handle
 * @details The handle of View instance.
 * @remarks To release the handle use maps_view_destroy().
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @see maps_view_create()
 * @see maps_view_destroy()
 */
typedef void *maps_view_h;

/**
 * @}
 */



#ifdef __cplusplus
}
#endif
#endif				/* __MAPS_TYPES_H__ */
