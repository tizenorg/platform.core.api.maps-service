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

#ifndef __MAPS_PLACE_H__
#define __MAPS_PLACE_H__

#include <tizen_type.h>
#include <maps_address.h>
#include <maps_place_category.h>
#include <maps_place_attribute.h>
#include <maps_place_contact.h>
#include <maps_place_editorial.h>
#include <maps_place_link_object.h>
#include <maps_place_image.h>
#include <maps_place_review.h>
#include <maps_place_rating.h>
#include <maps_coordinates.h>

/**
 * @ingroup	CAPI_MAPS_PLACE_MODULE
 * @defgroup	CAPI_MAPS_PLACE_DATA_MODULE Place
 *
 * @file maps_place.h
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
 * @brief	The Place handle
 * @details The handle of Place instance.
 * @remarks To release the handle use maps_place_destroy().
 * \n To clone the handle use maps_place_clone().
 * @since_tizen 2.4
 *
 * @see maps_place_destroy()
 * @see maps_place_clone()
 */
typedef void *maps_place_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Called when requesting the list of Place Properties.
 * @details This callback is invoked while iterating through the list of Place
 *
 *
 * TODO: it would be greate to add such a detailed message to all callbacks
 * /n This callback is synchronous.
 *
 *
 * Properties.
 * @since_tizen 2.4
 * @remarks @a key and @a value must be released using free() and corresponding
 * release method for property value correspondingly.
 *
 * @param[in]	index		The current index of property
 * @param[in]	total		The total amount of properties
 * @param[in]	key		The key of property
 * @param[in]	value		The value of property
 * @param[in]	uesr_data	The user data passed from
 * maps_place_foreach_property()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_place_foreach_property() will invoke this callback.
 *
 * @see maps_place_foreach_property()
 */
typedef bool(*maps_place_properties_cb) (int index, int total, char *key,
					 void *value, void *user_data);

/**
 * @brief	Called when requesting the list of Place Categories.
 * @details This callback is invoked while iterating through the list of Place
 * Categories.
 * @since_tizen 2.4
 * @remarks @a category is valid only in this function and must be released
 * using maps_place_category_destroy().
 * \n To use @a category outside this function, clone it with
 * maps_place_category_clone().
 *
 * @param[in]	index		The current index of category
 * @param[in]	total		The total amount of categories
 * @param[in]	category	The place category handle
 * @param[in]	uesr_data	The user data passed from
 * maps_place_foreach_category()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_place_foreach_category() will invoke this callback.
 *
 * @see maps_place_foreach_category()
 * @see #maps_place_category_h
 */
typedef bool(*maps_place_categories_cb) (int index, int total,
					 maps_place_category_h category,
					 void *user_data);

/**
 * @brief	Called when requesting the list of Place Attributes.
 * @details This callback is invoked while iterating through the list of Place
 * Attributes.
 * @since_tizen 2.4
 * @remarks @a attribute is valid only in this function and must be released
 * using maps_place_attribute_destroy().
 * \n To use @a attribute outside this function, clone it with
 * maps_place_attribute_clone().
 *
 * @param[in]	index		The current index of attribute
 * @param[in]	total		The total amount of attributes
 * @param[in]	attribute	The place attribute handle
 * @param[in]	uesr_data	The user data passed from the
 * maps_place_foreach_attribute()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_place_foreach_attribute() will invoke this callback.
 *
 * @see maps_place_foreach_attribute()
 * @see #maps_place_attribute_h
 */
typedef bool(*maps_place_attributes_cb) (int index, int total,
					 maps_place_attribute_h attribute,
					 void *user_data);

/**
 * @brief	Called when requesting the list of Place Contacts.
 * @details This callback is invoked while iterating through the list of Place
 * Contacts.
 * @since_tizen 2.4
 * @remarks @a contact is valid only in this function and must be released using
 * maps_place_contact_destroy().
 * \n To use @a contact outside this function, clone it with
 * maps_place_contact_clone().
 *
 * @param[in]	index		The current index of contact
 * @param[in]	total		The total amount of contacts
 * @param[in]	contact		The place contact handle
 * @param[in]	uesr_data	The user data passed from the
 * maps_place_foreach_contact()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_place_foreach_contact() will invoke this callback.
 *
 * @see maps_place_foreach_contact()
 * @see #maps_place_contact_h
 */
typedef bool(*maps_place_contacts_cb) (int index, int total,
				       maps_place_contact_h contact,
				       void *user_data);

/**
 * @brief	Called when requesting the list of Place Editorial.
 * @details This callback is invoked while iterating through the list of Place
 * Editorials.
 * @since_tizen 2.4
 * @remarks @a editorial is valid only in this function and must be released
 * using maps_place_editorial_destroy().
 * \n To use @a editorial outside this function, clone it with
 * maps_place_editorial_clone().
 *
 * @param[in]	index		The current index of editorial
 * @param[in]	total		The total amount of editorials
 * @param[in]	editorial	The place editorial handle
 * @param[in]	uesr_data	The user data passed from the
 * maps_place_foreach_editorial()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_place_foreach_editorial() will invoke this callback.
 *
 * @see maps_place_foreach_editorial()
 * @see #maps_place_editorial_h
 */

typedef bool(*maps_place_editorials_cb) (int index, int total,
					 maps_place_editorial_h editorial,
					 void *user_data);
/**
 * @brief	Called when requesting the list of Place Image.
 * @details This callback is invoked while iterating through the list of Place
 * Images.
 * @since_tizen 2.4
 * @remarks @a image is valid only in this function and must be released using
 * maps_place_image_destroy().
 * \n To use @a image outside this function, clone it with
 * maps_place_image_clone().
 *
 * @param[in]	index		The current index of image
 * @param[in]	total		The total amount of images
 * @param[in]	image		The place image handle
 * @param[in]	uesr_data	The user data passed from the
 * maps_place_foreach_image()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_place_foreach_image() will invoke this callback.
 *
 * @see maps_place_foreach_image()
 * @see #maps_place_image_h
 */
typedef bool(*maps_place_images_cb) (int index, int total,
				     maps_place_image_h image,
				     void *user_data);

/**
 * @brief	Called when requesting the list of Place Review.
 * @details This callback is invoked while iterating through the list of Place
 * Reviews.
 * @since_tizen 2.4
 * @remarks @a review is valid only in this function and must be released using
 * maps_place_review_destroy().
 * \n To use @a review outside this function, clone it with
 * maps_place_review_clone().
 *
 * @param[in]	index		The current index of review
 * @param[in]	total		The total amount of reviews
 * @param[in]	review		The place review handle
 * @param[in]	uesr_data	The user data passed from the
 * maps_place_foreach_review()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_place_foreach_review() will invoke this callback.
 *
 * @see maps_place_foreach_review()
 * @see #maps_place_image_h
 */
typedef bool(*maps_place_reviews_cb) (int index, int total,
				      maps_place_review_h review,
				      void *user_data);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Destroys the place handle and releases all its resources.
 * @details This function destroys the place handle and releases all its
 * resources.
 * @since_tizen 2.4
 *
 * @param[in]	place		The place handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_clone()
 */
int maps_place_destroy(maps_place_h place);

/**
 * @brief	Clones the place handle.
 * @details This function clones the place handle @a origin and all its
 * resources.
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_place_destroy().
 *
 * @param[in]	origin	The original place handle
 * @param[out]	cloned	A cloned place handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_place_destroy()
 */
int maps_place_clone(const maps_place_h origin, maps_place_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the place id.
 * @details This function gets the place id.
 * @since_tizen 2.4
 * @remarks @a id must be released using free().
 *
 * @param[in]	place		The place handle
 * @param[out]	id		The place id
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_get_id(const maps_place_h place, char **id);

/**
 * @brief	Gets the place name.
 * @details This function gets the place name.
 * @since_tizen 2.4
 * @remarks @a name must be released using free().
 *
 * @param[in]	place		The place handle
 * @param[out]	name		The place name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_get_name(const maps_place_h place, char **name);

/**
 * @brief	Gets the place view URI.
 * @details This function gets the place view URI.
 * @since_tizen 2.4
 * @remarks @a uri must be released using free().
 *
 * @param[in]	place		The place handle
 * @param[out]	uri		The place view URI
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_get_uri(const maps_place_h place, char **uri);

/**
 * @brief	Gets the place location.
 * @details This function gets the place location.
 * @since_tizen 2.4
 * @remarks @a location must be released using maps_coordinates_destroy().
 *
 * @param[in]	place			The place handle
 * @param[out]	location		The place location
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_get_location(const maps_place_h place,
			    maps_coordinates_h *location);

/**
 * @brief	Gets the place distance from the center of the location.
 * @details This function gets the place distance from the center of the
 * location.
 * @since_tizen 2.4
 *
 * @param[in]	place			The place handle
 * @param[out]	distance		The place distance
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_place_get_distance(const maps_place_h place, int *distance);

/**
 * @brief	Gets the place address.
 * @details This function gets the place address.
 * @since_tizen 2.4
 * @remarks @a address must be released using maps_address_destroy().
 *
 * @param[in]	place			The place handle
 * @param[out]	address			The place address
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_place_get_address(const maps_place_h place,
			   maps_address_h *address);

/**
 * @brief	Gets the place rating.
 * @details This function gets the place rating.
 * @since_tizen 2.4
 * @remarks @a rating must be released using maps_place_rating_destroy().
 *
 * @param[in]	place		The place handle
 * @param[out]	rating		The place rating handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_place_get_rating(const maps_place_h place,
			  maps_place_rating_h *rating);

/**
 * @brief	Retrieves all properties.
 * @details This function retrieves all place properties.
 * @since_tizen 2.4
 * @remarks The properties will be delivered via maps_place_properties_cb().
 *
 * @param[in]	place		The place handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @post This function invokes maps_place_properties_cb() repeatedly to retrieve
 * each property.
 *
 * @see maps_place_properties_cb()
 */
int maps_place_foreach_property(const maps_place_h place,
				maps_place_properties_cb callback,
				void *user_data);

/**
 * @brief	Retrieves all categories
 * @details This function retrieves all place categories.
 * @since_tizen 2.4
 * @remarks The categories will be delivered via maps_place_categories_cb().
 *
 * @param[in]	place		The place handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_place_categories_cb() repeatedly to retrieve
 * each category.
 *
 * @see maps_place_categories_cb()
 */
int maps_place_foreach_category(const maps_place_h place,
				maps_place_categories_cb callback,
				void *user_data);

/**
 * @brief	Retrieves all attributes.
 * @details This function retrieves all place attributes.
 * @since_tizen 2.4
 * @remarks The attributes will be delivered via maps_place_attributes_cb().
 *
 * @param[in]	place		The place handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_place_attributes_cb() repeatedly to retrieve
 * each attribute.
 *
 * @see maps_place_attributes_cb()
 */
int maps_place_foreach_attribute(const maps_place_h place,
				 maps_place_attributes_cb callback,
				 void *user_data);

/**
 * @brief	Retrieves all contacts.
 * @details This function retrieves all place contacts.
 * @since_tizen 2.4
 * @remarks The contacts will be delivered via maps_place_contacts_cb().
 *
 * @param[in]	place		The place handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_place_contacts_cb() repeatedly to retrieve
 * each contact.
 *
 * @see maps_place_contacts_cb()
 */
int maps_place_foreach_contact(const maps_place_h place,
			       maps_place_contacts_cb callback,
			       void *user_data);

/**
 * @brief	Retrieves all editorials.
 * @details This function retrieves all place editorials.
 * @since_tizen 2.4
 * @remarks The editorials will be delivered via maps_place_editorials_cb().
 *
 * @param[in]	place		The place handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_place_editorials_cb() repeatedly to retrieve
 * each editorial.
 *
 * @see maps_place_editorials_cb()
 */
int maps_place_foreach_editorial(const maps_place_h place,
				 maps_place_editorials_cb callback,
				 void *user_data);

/**
 * @brief	Retrieves all images.
 * @details This function retrieves all place images.
 * @since_tizen 2.4
 * @remarks The images will be delivered via maps_place_images_cb().
 *
 * @param[in]	place		The place handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_place_images_cb() repeatedly to retrieve
 * each image.
 *
 * @see maps_place_images_cb()
 */
int maps_place_foreach_image(const maps_place_h place,
			     maps_place_images_cb callback, void *user_data);

/**
 * @brief	Retrieves all reviews.
 * @details This function retrieves all place reviews.
 * @since_tizen 2.4
 * @remarks The reviews will be delivered via maps_place_reviews_cb().
 *
 * @param[in]	place		The place handle
 * @param[in]	callback	The callback function to invoke
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 *
 * @post This function invokes maps_place_reviews_cb() repeatedly to retrieve
 * each review.
 *
 * @see maps_place_reviews_cb()
 */
int maps_place_foreach_review(const maps_place_h place,
			      maps_place_reviews_cb callback,
			      void *user_data);

/**
 * @brief	Gets the place supplier link.
 * @details This function gets the place supplier link.
 * @since_tizen 2.4
 * @remarks @a supplier must be released using maps_place_link_object_destroy().
 *
 * @param[in]	place			The place handle image
 * @param[out]	supplier	The place supplier link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_place_get_supplier_link(const maps_place_image_h place,
				 maps_place_link_object_h *supplier);

/**
 * @brief	Gets the place related link.
 * @details This function gets the place related link.
 * @since_tizen 2.4
 * @remarks @a related must be released using maps_place_link_object_destroy().
 *
 * @param[in]	place		The place handle image
 * @param[out]	related	The place related link
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 */
int maps_place_get_related_link(const maps_place_image_h place,
				maps_place_link_object_h *related);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_PLACE_H__ */
