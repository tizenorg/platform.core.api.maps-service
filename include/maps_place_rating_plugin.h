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

#ifndef __MAPS_PLACE_RATING_PLUGIN_H__
#define __MAPS_PLACE_RATING_PLUGIN_H__

#include <maps_place_rating.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_RATING_MODULE Rating
 *
 * @file maps_place_rating_plugin.h
 * @brief This file contains the functions related to Place Rating
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_RATING_MODULE
 * @{
 * @brief This provides APIs related to Place Rating, used in Place
 * Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new place rating handle.
 * @details This function creates a new place rating handle and allocates all
 * needed resources.
 * @since_tizen 2.4
 * @remarks @a rating must be released using maps_place_rating_destroy().
 * \n @a rating may be cloned using maps_place_rating_clone().
 *
 * @param[out]	rating		A handle of a new place rating on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_rating_destroy()
 * @see maps_place_rating_clone()
 */
int maps_place_rating_create(maps_place_rating_h *rating);

/**
 * @brief	Sets the place rating count.
 * @details This function sets the place rating count.
 * @since_tizen 2.4
 *
 * @param[in]	rating		The handle of place rating
 * @param[in]	count		The place rating count
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a rating is created using maps_place_rating_create().
 *
 * @see maps_place_rating_create()
 * @see maps_place_rating_get_count()
 */
int maps_place_rating_set_count(maps_place_rating_h rating, const int count);

/**
 * @brief	Sets the place rating average.
 * @details This function sets the place rating average.
 * @since_tizen 2.4
 *
 * @param[in]	rating		The handle of place rating
 * @param[in]	average		The place rating average
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a rating is created using maps_place_rating_create().
 *
 * @see maps_place_rating_create()
 * @see maps_place_rating_get_average()
 */
int maps_place_rating_set_average(maps_place_rating_h rating,
				  const double average);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_RATING_PLUGIN_H__ */
