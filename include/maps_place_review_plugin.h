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

#ifndef __MAPS_PLACE_REVIEW_PLUGIN_H__
#define __MAPS_PLACE_REVIEW_PLUGIN_H__

#include <maps_place_media.h>
#include <maps_place_link_object.h>

#include <maps_place_review.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_REVIEW_MODULE Review
 *
 * @file maps_place_review_plugin.h
 * @brief This file contains the functions related to Place Review
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_REVIEW_MODULE
 * @{
 * @brief This provides APIs related to Place Review, used in Place
 * Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new place review handle.
 * @details This function creates a new place review handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a review must be released using maps_place_review_destroy().
 * \n @a review may be cloned using maps_place_review_clone().
 *
 * @param[out]	review		A handle of a new place review on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @see maps_place_review_destroy()
 * @see maps_place_review_clone()
 */
int maps_place_review_create(maps_place_review_h *review);

/**
 * @brief	Sets the place review date.
 * @details This function sets the place review date.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	review		The handle to place review
 * @param[in]	date		The place review date
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a review is created using maps_place_review_create().
 *
 * @see maps_place_review_create()
 * @see maps_place_review_get_date()
 */
int maps_place_review_set_date(maps_place_review_h review, const char *date);

/**
 * @brief	Sets the place review title.
 * @details This function sets the place review title.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	review		The handle to place review
 * @param[in]	title		The place review title
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a review is created using maps_place_review_create().
 *
 * @see maps_place_review_create()
 * @see maps_place_review_get_title()
 */
int maps_place_review_set_title(maps_place_review_h review,
				const char *title);

/**
 * @brief	Sets the place review rating.
 * @details This function sets the place review rating.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	review		The handle to place review
 * @param[in]	rating		The place review rating
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a review is created using maps_place_review_create().
 *
 * @see maps_place_review_create()
 * @see maps_place_review_get_rating()
 */
int maps_place_review_set_rating(maps_place_review_h review,
				 const double rating);

/**
 * @brief	Sets the place review description.
 * @details This function sets the place review description.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	review		The handle to place review
 * @param[in]	description	The place review description
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a review is created using maps_place_review_create().
 *
 * @see maps_place_review_create()
 * @see maps_place_review_get_description()
 */
int maps_place_review_set_description(maps_place_review_h review,
				      const char *description);

/**
 * @brief	Sets the place review language.
 * @details This function sets the place review language.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	review		The handle to place review
 * @param[in]	language	The place review language
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a review is created using maps_place_review_create().
 *
 * @see maps_place_review_create()
 * @see maps_place_review_get_language()
 */
int maps_place_review_set_language(maps_place_review_h review,
				   const char *language);

/**
 * @brief	Sets the place review media.
 * @details This function sets the place review media.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	review		The handle to place review
 * @param[in]	media		The place review media
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a review is created using maps_place_review_create().
 *
 * @see maps_place_review_create()
 * @see maps_place_review_get_media()
 * @see #maps_place_media_h
 */
int maps_place_review_set_media(maps_place_review_h review,
				const maps_place_media_h media);

/**
 * @brief	Sets the place review user link.
 * @details This function sets the place review user link.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	review		The handle to place review
 * @param[in]	user		The place review user link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre @a review is created using maps_place_review_create().
 *
 * @see maps_place_review_create()
 * @see maps_place_review_get_user_link()
 * @see #maps_place_link_object_h
 */
int maps_place_review_set_user_link(maps_place_review_h review,
				    const maps_place_link_object_h user);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_REVIEW_PLUGIN_H__ */
