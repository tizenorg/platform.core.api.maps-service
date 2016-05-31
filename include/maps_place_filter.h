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

#ifndef __MAPS_PLACE_FILTER_H__
#define __MAPS_PLACE_FILTER_H__

#include <maps_place_category.h>

/**
 * @ingroup	CAPI_MAPS_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_FILTER_MODULE Filter
 *
 * @file maps_place_filter.h
 * @brief This file contains the functions related to Place Filter information.
 *
 * @addtogroup CAPI_MAPS_PLACE_FILTER_MODULE
 * @{
 * @brief This provides APIs related to Place Filter information, used in Place
 * Discovery and Search.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Place Filter handle
 * @details The handle of Place Filter instance.
 * @remarks To release the handle use maps_place_filter_destroy().
 * \n To clone the handle use maps_place_filter_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @see maps_place_filter_destroy()
 * @see maps_place_filter_clone()
 */
typedef void *maps_place_filter_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Called when requesting the list of Place Filter Properties.
 * @details This callback is invoked while iterating through the list of Place
 * Filter Properties.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a key and @a value must be released using free() and corresponding
 * release method for property value correspondingly.
 *
 * @param[in]	index		The current index of property
 * @param[in]	total		The total amount of properties
 * @param[in]	key		The key of property
 * @param[in]	value		The value of property
 * @param[in]	uesr_data	The user data passed from the
 * maps_place_filter_foreach_property()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_place_filter_foreach_property() will invoke this callback.
 *
 * @see maps_place_filter_foreach_property()
 */
typedef bool(*maps_place_filter_properties_cb) (int index, int total,
						char *key, void *value,
						void *user_data);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Creates a new place filter handle.
 * @details This function creates a new place filter handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a filter must be released using maps_place_filter_destroy().
 * \n @a filter may be cloned using maps_place_filter_clone().
 *
 * @param[out]	filter		A handle of a new place filter on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_filter_destroy()
 * @see maps_place_filter_clone()
 */
int maps_place_filter_create(maps_place_filter_h *filter);

/**
 * @brief	Destroys the place filter handle and releases all its resources.
 * @details This function destroys the place filter handle and releases all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	filter		The place filter handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_create()
 * @see maps_place_filter_clone()
 */
int maps_place_filter_destroy(maps_place_filter_h filter);

/**
 * @brief	Clones the place filter handle.
 * @details This function clones the place filter handle @a origin and all its
 * resources.
 * \n Place filter handle @a origin may be created using
 * maps_place_filter_create().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a cloned must be released using maps_place_filter_destroy().
 *
 * @param[in]	origin	The place filter handle to be cloned
 * @param[out]	cloned	A handle of a newly cloned place filter on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is created using maps_place_filter_create().
 *
 * @see maps_place_filter_create()
 * @see maps_place_filter_destroy()
 */
int maps_place_filter_clone(const maps_place_filter_h origin,
			    maps_place_filter_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the value of the specified key in the place filter.
 * @details This function gets the value of the specified key in the place
 * filter.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a value must be released using free().
 *
 * @param[in]	filter		The handle of the place filter
 * @param[in]	key		The key of filter
 * @param[out]	value		The filter value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_set()
 * @see maps_place_filter_create()
 */
int maps_place_filter_get(const maps_place_filter_h filter, const char *key,
			  char **value);

/**
 * @brief	Gets the value of keyword.
 * @details This function gets the value of keyword.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a keyword must be released using free().
 *
 * @param[in]	filter		The handle of the place filter
 * @param[out]	keyword		The key of filter
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_set_keyword()
 * @see maps_place_filter_create()
 */
int maps_place_filter_get_keyword(const maps_place_filter_h filter,
				  char **keyword);

/**
 * @brief	Gets the place name.
 * @details This function gets the place name.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a place_name must be released using free().
 *
 * @param[in]	filter		The handle of the place filter
 * @param[out]	place_name	The place name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_set_place_name()
 * @see maps_place_filter_create()
 */
int maps_place_filter_get_place_name(const maps_place_filter_h filter,
				     char **place_name);

/**
 * @brief	Gets the category.
 * @details This function gets the category.
 * \n For allowed values of keys, see the macros above.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a category must be released using maps_place_category_destroy().
 *
 * @param[in]	filter		The handle of the place filter
 * @param[out]	category	The category
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_set_category()
 * @see maps_place_filter_create()
 */
int maps_place_filter_get_category(const maps_place_filter_h filter,
				   maps_place_category_h *category);

/**
 * @brief Retrieves all filter properties.
 * @details This function retrieves all place filter properties.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	filter		The handle of the place filter
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @post This function invokes maps_place_filter_properties_cb() repeatedly to
 * retrieve each available filter keys.
 *
 * @see maps_place_filter_properties_cb()
 * @see maps_place_filter_get()
 * @see maps_place_filter_set()
 * @see maps_place_filter_create()
 */
int maps_place_filter_foreach_property(const maps_place_filter_h filter,
				       maps_place_filter_properties_cb
				       callback, void *user_data);

/**
 * @brief	Gets the place address.
 * @details This function gets the place address.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a place_address must be released using free().
 *
 * @param[in]	filter			The handle of the place filter
 * @param[out]	place_address	The place address
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_set_place_address()
 * @see maps_place_filter_create()
 */
int maps_place_filter_get_place_address(const maps_place_filter_h filter,
				     char **place_address);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Sets the filter value.
 * @details This function sets the filter value in an assignment with a
 * specified string key.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	filter		The handle of the place filter
 * @param[in]	key		The key of filter
 * @param[in]	value	The filter value to set, could be NULL if want remove
 * value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_get()
 * @see maps_place_filter_create()
 */
int maps_place_filter_set(maps_place_filter_h filter, const char *key,
			  const char *value);

/**
 * @brief	Sets the keyword.
 * @details This function sets the keyword.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	filter		The handle of the place filter
 * @param[in]	keyword		The keyword
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_get_keyword()
 * @see maps_place_filter_create()
 */
int maps_place_filter_set_keyword(maps_place_filter_h filter,
				  const char *keyword);

/**
 * @brief	Sets the place name.
 * @details This function sets the place name.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	filter		The handle of the place filter
 * @param[in]	place_name	The place name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_get_place_name()
 * @see maps_place_filter_create()
 */
int maps_place_filter_set_place_name(maps_place_filter_h filter,
				     const char *place_name);

/**
 * @brief	Sets the category.
 * @details This function sets the place name.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	filter		The handle of the place filter
 * @param[in]	category	The category
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_get_category()
 * @see maps_place_filter_create()
 */
int maps_place_filter_set_category(maps_place_filter_h filter,
				   const maps_place_category_h category);

/**
 * @brief	Sets the place address.
 * @details This function sets the place address.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	filter			The handle of the place filter
 * @param[in]	place_address	The place address
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a filter is created using maps_place_filter_create().
 *
 * @see maps_place_filter_get_place_address()
 * @see maps_place_filter_create()
 */
int maps_place_filter_set_place_address(maps_place_filter_h filter,
				     const char *place_address);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_FILTER_H__ */
