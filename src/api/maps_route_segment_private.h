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

#ifndef __MAPS_ROUTE_SEGMENT_PRIVATE_H__
#define __MAPS_ROUTE_SEGMENT_PRIVATE_H__

#include <maps_service.h>
#include <maps_extra_types.h>

/**
 * @ingroup	CAPI_MAPS_ROUTE_MODULE
 * @defgroup	CAPI_MAPS_ROUTE_SEGMENT_MODULE Segment
 *
 * @file maps_route_segment_private.h
 * @brief This file contains the functions related to Route Segment information.
 * @addtogroup CAPI_MAPS_ROUTE_SEGMENT_MODULE
 * @{
 * @brief This provides APIs related to Place Segment information, used in Route
 * Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @internal
 * @brief	Checks if a segment data feature is supported.
 * @details This function checks if a segment data feature is supported.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	segment		The handle to segment
 * @param[in]	data		The feature to be checked
 * @param[out]	supported	Is the feature supported
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a segment is created using maps_route_segment_create().
 *
 * @see maps_route_segment_create()
 */
int _maps_route_segment_is_data_supported(const maps_route_segment_h segment,
					  maps_service_data_e data,
					  bool *supported);

/**
 * @internal
 * @brief	Sets the supported segment data features.
 * @details This function sets the supported segment data features.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	segment		The handle to segment
 * @param[in]	supported_data	The supported segment data features
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a segment is created using maps_route_segment_create().
 *
 * @see maps_route_segment_create()
 */
int _maps_route_segment_set_supported_data(maps_route_segment_h segment,
					   const maps_int_hashtable_h supported_data);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_ROUTE_SEGMENT_PRIVATE_H__ */
