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

#ifndef __MAPS_PLACE_RATING_H__
#define __MAPS_PLACE_RATING_H__

#include <time.h>

/**
 * @ingroup	CAPI_MAPS_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_RATING_MODULE Rating
 *
 * @file maps_place_rating.h
 * @brief This file contains the functions related to Place Rating information.
 *
 * @addtogroup CAPI_MAPS_PLACE_RATING_MODULE
 * @{
 * @brief This provides APIs related to Place Rating information, used in Place
 * Discovery and Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Place Rating handle
 * @details The handle of Place Rating instance.
 * @remarks To release the handle use maps_place_rating_destroy().
 * \n To clone the handle use maps_place_rating_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @see maps_place_rating_destroy()
 * @see maps_place_rating_clone()
 */
typedef void *maps_place_rating_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the place rating handle and releases all its resources.
 * @details This function destroys the place rating handle and releases all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	rating		The place rating handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_rating_clone()
 */
int maps_place_rating_destroy(maps_place_rating_h rating);

/**
 * @brief	Clones the place rating handle.
 * @details This function clones the place rating handle @a origin and all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a cloned must be released using maps_place_rating_destroy().
 *
 * @param[in]	origin		The original place rating handle
 * @param[out]	cloned		A cloned place rating handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_rating_destroy()
 */
int maps_place_rating_clone(const maps_place_rating_h origin,
			    maps_place_rating_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the place rating count.
 * @details This function gets the place rating count.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	rating		The handle to place rating handle
 * @param[out]	count		The place rating count
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_rating_get_count(const maps_place_rating_h rating, int *count);

/**
 * @brief	Gets the place rating average.
 * @details This function gets the place rating average.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	rating		The handle to place rating handle
 * @param[out]	average	The place average rating
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_rating_get_average(const maps_place_rating_h rating,
				  double *average);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_RATING_H__ */
