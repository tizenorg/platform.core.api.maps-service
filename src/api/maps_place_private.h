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

#ifndef __MAPS_PLACE_PRIVATE_H__
#define __MAPS_PLACE_PRIVATE_H__

#include <maps_service.h>
#include "maps_extra_types_private.h"

/**
 * @ingroup	CAPI_MAPS_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_DATA_MODULE Place
 *
 * @file maps_place_private.h
 * @brief This file contains the functions related to Place information.
 *
 * @addtogroup CAPI_MAPS_PLACE_DATA_MODULE
 * @{
 * @brief This provides APIs related to Place information, used in Place
 * Discovery and Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @internal
 * @brief	Checks if a place data feature is supported.
 * @details This function checks if a place data feature is supported.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	data		The feature to be checked
 * @param[out]	supported	Is the feature supported
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 */
int _maps_place_is_data_supported(const maps_place_h place,
				  maps_service_data_e data, bool *supported);

/**
 * @internal
 * @brief	Sets the supported place data features.
 * @details This function sets the supported place data features.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	supported_data	The supported place data features
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 */
int _maps_place_set_supported_data(maps_place_h place,
				   const maps_int_hashtable_h supported_data);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_PRIVATE_H__ */
