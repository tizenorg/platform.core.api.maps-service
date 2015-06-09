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

#ifndef __MAPS_PLACE_PLUGIN_H__
#define __MAPS_PLACE_PLUGIN_H__

#include <maps_place.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_MODULE Place
 */

/**
 * @ingroup	CAPI_MAPS_PLUGIN_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_PLACE_DATA_MODULE Place
 *
 * @file maps_place_plugin.h
 * @brief This file contains the functions related to Place
 * information, needed in plug-in development.
 *
 * @addtogroup CAPI_MAPS_PLUGIN_PLACE_DATA_MODULE
 * @{
 * @brief This provides APIs related to Place information, used in
 * Place Discovery and Search, needed in plug-in development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Creates a new place handle.
 * @details This function creates a new place handle and allocates all needed
 * resources.
 * @since_tizen 2.4
 * @remarks @a place must be released using maps_place_destroy().
 * \n @a place may be cloned using maps_place_clone().
 *
 * @param[in]	place		A handle of a new place on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_destroy()
 * @see maps_place_clone()
 */
int maps_place_create(maps_place_h *place);

/**
 * @brief	Sets the place id.
 * @details This function sets the place id.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	id		The place id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_get_related_link()
 */
int maps_place_set_id(maps_place_h place, const char *id);

/**
 * @brief	Sets the place name.
 * @details This function sets the place name.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	name		The place name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_get_name()
 */
int maps_place_set_name(maps_place_h place, const char *name);

/**
 * @brief	Sets the place location.
 * @details This function sets the place location.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	location	The place location
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_get_location()
 */
int maps_place_set_location(maps_place_h place,
			    const maps_coordinates_h location);

/**
 * @brief	Sets the place distance from the center of the location.
 * @details This function sets the place distance from the center of the
 * location.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	distance	The place distance
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_get_distance()
 */
int maps_place_set_distance(maps_place_h place, const int distance);

/**
 * @brief	Sets the place address.
 * @details This function sets the place address.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	address		The place address
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_get_address()
 */
int maps_place_set_address(maps_place_h place, const maps_address_h address);

/**
 * @brief	Sets the place categories.
 * @details This function sets the place categories.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	categories	The place category list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_foreach_category()
 */
int maps_place_set_categories(maps_place_h place,
				const maps_item_list_h categories);

/**
 * @brief	Sets the place view URI.
 * @details This function sets the place URI.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	uri		The place view uri
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_get_uri()
 */
int maps_place_set_uri(maps_place_h place, const char *uri);

/**
 * @brief	Sets the place attributes.
 * @details This function sets the place attributes.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	attributes	The place attribute list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_foreach_attribute()
 */
int maps_place_set_attributes(maps_place_h place,
			      const maps_item_list_h attributes);

/**
 * @brief	Sets the place contacts.
 * @details This function sets the place contacts.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	contacts	The place contact list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_foreach_contact()
 */
int maps_place_set_contacts(maps_place_h place,
			    const maps_item_list_h contacts);

/**
 * @brief	Sets the place editorials.
 * @details This function sets the place editorials.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	editorials	The place editorial list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_foreach_editorial()
 */
int maps_place_set_editorials(maps_place_h place,
			      const maps_item_list_h editorials);

/**
 * @brief	Sets the place images.
 * @details This function sets the place images.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	images		The place image list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_foreach_image()
 */
int maps_place_set_images(maps_place_h place, const maps_item_list_h images);

/**
 * @brief	Sets the place review.
 * @details This function sets the place reviews.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	reviews		The place review list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_foreach_review()
 */
int maps_place_set_reviews(maps_place_h place,
			   const maps_item_list_h reviews);

/**
 * @brief	Sets the place properties.
 * @details This function sets the place properties.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place
 * @param[in]	properties	The place properties list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_foreach_property()
 */
int maps_place_set_properties(maps_place_h place,
			      const maps_item_hashtable_h properties);

/**
 * @brief	Sets the place rating.
 * @details This function sets the place rating.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place image
 * @param[in]	rating		The place rating
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_get_rating()
 */
int maps_place_set_rating(maps_place_h place,
			  const maps_place_rating_h rating);

/**
 * @brief	Sets the place supplier link.
 * @details This function sets the place supplier link.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place image
 * @param[in]	supplier	The place supplier link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_get_supplier_link()
 */
int maps_place_set_supplier_link(maps_place_h place,
				 const maps_place_link_object_h supplier);

/**
 * @brief	Sets the place related link.
 * @details This function sets the place related link.
 * @since_tizen 2.4
 *
 * @param[in]	place		The handle to place image
 * @param[in]	related		The place related link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a place is created using maps_place_create().
 *
 * @see maps_place_create()
 * @see maps_place_get_related_link()
 */
int maps_place_set_related_link(maps_place_h place,
				const maps_place_link_object_h related);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_PLUGIN_H__ */
