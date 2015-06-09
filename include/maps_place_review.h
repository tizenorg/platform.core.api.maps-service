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

#ifndef __MAPS_PLACE_REVIEW_H__
#define __MAPS_PLACE_REVIEW_H__

#include <maps_place_media.h>
#include <maps_place_link_object.h>

/**
 * @ingroup	CAPI_MAPS_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_REVIEW_MODULE Review
 *
 * @file maps_place_review.h
 * @brief This file contains the functions related to Place Review information.
 *
 * @addtogroup CAPI_MAPS_PLACE_REVIEW_MODULE
 * @{
 * @brief This provides APIs related to Place Review information, used in Place
 * Discovery and Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Place Review handle
 * @details The handle of Place Review instance.
 * @remarks To release the handle use maps_place_review_destroy().
 * \n To clone the handle use maps_place_review_clone().
 * @since_tizen 2.4
 *
 * @see maps_place_review_destroy()
 * @see maps_place_review_clone()
 */
typedef void *maps_place_review_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the place review handle and releases all its resources.
 * @details This function destroys the place review handle and releases all its
 * resources.
 * @since_tizen 2.4
 *
 * @param[in]	review		The place review handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_review_clone()
 */
int maps_place_review_destroy(maps_place_review_h review);

/**
 * @brief	Clones the place review handle.
 * @details This function clones the place review handle @a origin and all its
 * resources.
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_place_review_destroy().
 *
 * @param[in]	origin		The original place review handle
 * @param[out]	cloned		A cloned place review handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_review_destroy()
 */
int maps_place_review_clone(const maps_place_review_h origin,
			    maps_place_review_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the place review date.
 * @details This function gets the place review date.
 * @since_tizen 2.4
 * @remarks @a date must be released using free().
 *
 * @param[in]	review		The handle to place review
 * @param[out]	date		The place review date
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_review_get_date(const maps_place_review_h review, char **date);

/**
 * @brief	Gets the place review title.
 * @details This function gets the place review title.
 * @since_tizen 2.4
 * @remarks @a title must be released using free().
 *
 * @param[in]	review		The handle to place review
 * @param[out]	title		The place review title
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_review_get_title(const maps_place_review_h review,
				char **title);

/**
 * @brief	Gets the place review rating.
 * @details This function gets the place review rating.
 * @since_tizen 2.4
 *
 * @param[in]	review		The handle to place review
 * @param[out]	rating		The place review rating
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_review_get_rating(const maps_place_review_h review,
				 double *rating);

/**
 * @brief	Gets the place review description.
 * @details This function gets the place review description.
 * @since_tizen 2.4
 * @remarks @a description must be released using free().
 *
 * @param[in]	review		The handle to place review
 * @param[out]	description	The place review description
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_review_get_description(const maps_place_review_h review,
				      char **description);

/**
 * @brief	Gets the place review language.
 * @details This function gets the place review language.
 * @since_tizen 2.4
 * @remarks @a language must be released using free().
 *
 * @param[in]	review		The handle to place review
 * @param[out]	language	The place review language
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_review_get_language(const maps_place_review_h review,
				   char **language);

/**
 * @brief	Gets the place review media.
 * @details This function gets the place review media.
 * @since_tizen 2.4
 * @remarks @a media must be released using maps_place_media_destroy().
 *
 * @param[in]	review		The handle to place review
 * @param[out]	media		The place review media
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_place_media_h
 */
int maps_place_review_get_media(const maps_place_review_h review,
				maps_place_media_h *media);

/**
 * @brief	Gets the place review user link.
 * @details This function gets the place review user link.
 * @since_tizen 2.4
 * @remarks @a user must be released using maps_place_link_object_destroy().
 *
 * @param[in]	review		The handle to place review
 * @param[out]	user		The place review user link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #maps_place_link_object_h
 */
int maps_place_review_get_user_link(const maps_place_review_h review,
				    maps_place_link_object_h *user);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_REVIEW_H__ */
