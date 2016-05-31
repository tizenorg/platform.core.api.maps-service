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

#ifndef __MAPS_ERROR_H__
#define __MAPS_ERROR_H__

#include <tizen_type.h>
#include <tizen_error.h>

/**
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 *
 * @file maps_error.h
 * @brief This file contains the list of errors of Maps API
 *
 * @addtogroup CAPI_MAPS_SERVICE_AND_PREFERENCE_MODULE
 * @{
 * @brief This provides a list of Maps API errors
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumerations of error codes for Maps Service and Plug-ins
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum _maps_error_e {

	MAPS_ERROR_NONE = TIZEN_ERROR_NONE,	/**< Successful */

	MAPS_ERROR_PERMISSION_DENIED =
		TIZEN_ERROR_PERMISSION_DENIED,	/**< Permission Denied */

	MAPS_ERROR_OUT_OF_MEMORY =
		TIZEN_ERROR_OUT_OF_MEMORY,	/**< Out of memory */

	MAPS_ERROR_INVALID_PARAMETER =
		TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid parameter */

	MAPS_ERROR_NOT_SUPPORTED =
		TIZEN_ERROR_NOT_SUPPORTED,	/**< Not supported */

	MAPS_ERROR_CONNECTION_TIME_OUT =
		TIZEN_ERROR_CONNECTION_TIME_OUT,/**< Timeout error, no answer */

	MAPS_ERROR_NETWORK_UNREACHABLE =
		TIZEN_ERROR_NETWORK_UNREACHABLE,/**< Network unavailable*/

	MAPS_ERROR_INVALID_OPERATION =
		TIZEN_ERROR_INVALID_OPERATION,	/**< Operation is not valid */

	MAPS_ERROR_KEY_NOT_AVAILABLE =
		TIZEN_ERROR_KEY_NOT_AVAILABLE,	/**< Invalid key */

	MAPS_ERROR_RESOURCE_BUSY =
		TIZEN_ERROR_RESOURCE_BUSY,	/**< Maps Service busy */

	MAPS_ERROR_CANCELED =
		TIZEN_ERROR_CANCELED,	/**< Maps Service request aborted */

	MAPS_ERROR_UNKNOWN =
		TIZEN_ERROR_UNKNOWN,		/**< Unknown error */

	MAPS_ERROR_SERVICE_NOT_AVAILABLE =
		TIZEN_ERROR_MAPS_SERVICE | 0x01,/**< Service unavailable */

	MAPS_ERROR_NOT_FOUND =
		TIZEN_ERROR_MAPS_SERVICE | 0x02/**< Result not found */

} maps_error_e;

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_ERROR_H__ */
