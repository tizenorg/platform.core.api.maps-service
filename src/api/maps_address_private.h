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

#ifndef __MAPS_ADDRESS_PRIVATE_H__
#define __MAPS_ADDRESS_PRIVATE_H__

#include <maps_service.h>

/**
 * @file maps_address_private.h
 * @brief This file contains the functions related to Geographical Area.
 * @addtogroup CAPI_MAPS_ADDRESS_MODULE
 * @{
 * @brief This provides APIs related to Geographical Area
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @internal
 * @brief	Checks if coordinates is valid.
 * @details This function checks if coordinates is valid.
 * @since_tizen 3.0
 *
 * @param[in]	coordinates		The handle to coord
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_address_create()
 */
bool maps_address_is_valid(const maps_address_h address);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_ADDRESS_PRIVATE_H__ */
