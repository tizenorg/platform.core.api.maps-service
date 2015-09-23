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

#ifndef __MAPS_SERVICE_API_EXTRA_TYPES_H__
#define __MAPS_SERVICE_API_EXTRA_TYPES_H__

#include <tizen_type.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_AND_PROVIDERS_MODULE
 * @defgroup	CAPI_MAPS_CONTAINERS_MODULE Container Data Types
 *
 * @file maps_extra_types.h
 * @brief This file contains definitions and functions of useful data types.
 *
 * @addtogroup CAPI_MAPS_CONTAINERS_MODULE
 * @{
 * @brief This provides variable Maps Service data types, such as lists and hash
 * tables.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup	CAPI_MAPS_CONTAINERS_MODULE
 * @defgroup	CAPI_MAPS_CONTAINERS_MODULE_ITEM_LIST Maps Item Lists
 *
 * @file maps_extra_types.h
 * @brief This section contains API for using List of item pointers.
 *
 * @addtogroup CAPI_MAPS_CONTAINERS_MODULE_ITEM_LIST
 * @{
 * @brief This provides API for using List of item pointers.
 */

/**
 * @brief	Item List handle.
 * @details The Item List handle can be obtained via call of
 * maps_item_list_create().
 * @remarks To release the handle use maps_item_list_destroy().
 * \n To clone the handle use maps_item_list_clone().
 * \n The items of the list may be iterated using maps_item_list_foreach().
 * \n To append an item to the list use maps_item_list_append().
 * \n The items of the list may be removed maps_item_list_remove_all().
 * @since_tizen 2.4
 *
 * @see maps_item_list_create()
 * @see maps_item_list_destroy()
 * @see maps_item_list_clone()
 */
typedef void *maps_item_list_h;

/**
 * @brief	Creates a new list of item pointers.
 * @details This function creates a new instance of item pointer list, associate
 * a new handle with it and allocates all needed resources.
 * @since_tizen 2.4
 * @remarks @a list must be released using maps_item_list_destroy().
 * \n @a list may be cloned using maps_item_list_clone().
 *
 * @param[out]	list		The handle of newly created list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_item_list_destroy()
 * @see maps_item_list_clone()
 */
int maps_item_list_create(maps_item_list_h *list);

/**
 * @brief	Destroys the list handle and releases all its resources.
 * @details This function destroys the list handle and releases all its
 * resources.
 * @since_tizen 2.4
 *
 * @param[in]	list		The handle of list to be deleted
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_item_list_create().
 *
 * @see maps_item_list_create()
 * @see maps_item_list_clone()
 */
int maps_item_list_destroy(maps_item_list_h list);

/**
 * @brief	Clone function, making a copy of a list item.
 * @details This function is called once for each item of the given list while
 * the list is being cloned during the maps_item_list_clone() procedure.
 * @since_tizen 2.4
 * @remarks If this function returns the value different from MAPS_ERROR_NONE,
 * the cloned item will not be appended to the list.
 *
 * @param[in]	origin		The pointer to the item which is to be cloned
 * @param[out]	cloned		The pointer to the clone of the specified item
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_list_clone() will invoke this callback.
 *
 * @see maps_item_list_clone()
 */
typedef int (*maps_item_list_clone_cb) (void *origin, void **cloned);

/**
 * @brief	Clones the list of item pointers.
 * @details This function clones the list of item pointers @a origin and all its
 * resources.
 * \n The list handle @a origin may be created using maps_item_list_create().
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_item_list_destroy().
 *
 * @param[in]	origin		The handle of list to be cloned
 * @param[in]	clone_func	The #maps_item_list_clone_cb function used to
 * clone an element of a list
 * @param[out]	cloned		The handle of the newly cloned list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is created using maps_item_list_create().
 * @post #maps_item_list_clone_cb is used to clone each item of the list.
 *
 * @see maps_item_list_create()
 * @see maps_item_list_destroy()
 */
int maps_item_list_clone(const maps_item_list_h origin,
			 maps_item_list_clone_cb clone_func,
			 maps_item_list_h *cloned);

/**
 * @brief	Appends an item to the list.
 * @details This function appends an item @a data to the list of item pointers
 * @a list.
 * @since_tizen 2.4
 *
 * @param[in]	list		The handle of list
 * @param[in]	data		The item pointer to be pointed to the list
 * @param[in]	clone_func	The function for cloning the added list value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_item_list_create().
 *
 * @see maps_item_list_foreach()
 * @see maps_item_list_remove_all()
 * @see maps_item_list_create()
 */
int maps_item_list_append(maps_item_list_h list, const void *data,
			  maps_item_list_clone_cb clone_func);

/**
 * @brief	Called once for each item while iterating through the given
 * list.
 * @details This function is called once for each item of the given list while
 * the list is being iterated
 * during the maps_item_list_foreach() procedure.
 * @since_tizen 2.4
 * @remarks The @a data must be released by you.
 *
 * @param[in]	index		The current index of item
 * @param[in]	total		The total amount of items
 * @param[in]	data		The pointer to the list item
 * @param[in]	user_data	The user data passed from
 * maps_item_list_foreach()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_item_list_foreach() will invoke this callback.
 *
 * @see maps_item_list_foreach()
 */
typedef bool(*maps_item_list_foreach_cb) (int index, int total, void *data,
					  void *user_data);

/**
 * @brief	Gets the items of the specified list.
 * @details This function delivers items of a specified list via
 * maps_item_list_foreach_cb() callback.
 * @since_tizen 2.4
 *
 * @param[in]	list		The handle of list
 * @param[in]	clone_func	The function for cloning the list values
 * @param[in]	callback	The callback to be invoked for delivering each
 * list item
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @pre @a list is created using maps_item_list_create().
 * @post This function invokes maps_item_list_foreach_cb() to deliver list
 * items.
 *
 * @see maps_item_list_foreach_cb()
 * @see maps_item_list_append()
 * @see maps_service_create()
 */
int maps_item_list_foreach(maps_item_list_h list,
			   maps_item_list_clone_cb clone_func,
			   maps_item_list_foreach_cb callback,
			   void *user_data);

/**
 * @brief	Free function, destroying a list item.
 * @details This function is called once for each item of the given list while
 * the list is being destroyed
 * during the maps_item_list_remove_all() procedures.
 * @since_tizen 2.4
 * @remarks The return of this function is ignored.
 *
 * @param[in]	data		The pointer to the list item which is to be
 * destroyed
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_list_remove_all() will invoke this callback.
 *
 * @see maps_item_list_remove_all()
 */
typedef int (*maps_item_list_free_cb) (void *data);

/**
 * @brief	Removes and destroys list item.
 * @details This function removes the given items from the @a list and releases
 * resources, assigned with it.
 * @since_tizen 2.4
 * @remarks The @a item is destroyed with #maps_item_list_free_cb. If this
 * callback is NULL, item will not be destroyed.
 *
 * @param[in]	list		The handle of list
 * @param[in]	item		The to remove
 * @param[in]	free_func	The callback to be invoked for destroying each
 * list item
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_item_list_create().
 * @post #maps_item_list_free_cb is used to destroy the item of the list.
 *
 * @see maps_item_list_free_cb()
 * @see maps_item_list_create()
 * @see maps_item_list_remove_all()
 */
int maps_item_list_remove(maps_item_list_h list,
			  void *item,
			  maps_item_list_free_cb free_func);

/**
 * @brief	Removes and destroys all list items.
 * @details This function removes all items of the @a list and releases
 * resources, assigned with them.
 * If two elements contain the same data, only the first is removed.
 * If none of the elements contain the data, the GList is unchanged.
 * @since_tizen 3.0
 * @remarks Each item is destroyed with #maps_item_list_free_cb. If this
 * callback is NULL, items will not be destroyed.
 *
 * @param[in]	list		The handle of list
 * @param[in]	free_func	The callback to be invoked for destroying each
 * list item
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_item_list_create().
 * @post #maps_item_list_free_cb is used to destroy each item of the list.
 *
 * @see maps_item_list_free_cb()
 * @see maps_item_list_create()
 */
int maps_item_list_remove_all(maps_item_list_h list,
			      maps_item_list_free_cb free_func);

/**
 * @}
 */

/*----------------------------------------------------------------------------*/

/**
 * @ingroup	CAPI_MAPS_CONTAINERS_MODULE
 * @defgroup	CAPI_MAPS_CONTAINERS_MODULE_STRING_HASHTABLE Maps String Hash Tables
 *
 * @file maps_extra_types.h
 * @brief This section contains API for using Hash Table of strings.
 *
 * @addtogroup CAPI_MAPS_CONTAINERS_MODULE_STRING_HASHTABLE
 * @{
 * @brief This provides API for using Hash Table of strings.
 */

/**
 * @brief	The handle of Hash Table of strings.
 * @details The handle of Hash Table of strings can be obtained via call of
 * maps_string_hashtable_create().
 * @remarks To release the handle use maps_string_hashtable_destroy().
 * \n To clone the handle use maps_string_hashtable_clone().
 * \n To assign the value to a key use maps_string_hashtable_set().
 * \n The value, assigned with a specified key may be obtained using
 * maps_string_hashtable_get().
 * \n All key-value pairs of the table may be iterated using
 * maps_string_hashtable_foreach().
 * \n The key-value pair may be removed form the table using
 * maps_string_hashtable_remove().
 * \n To check if key is added to the table use
 * maps_string_hashtable_contains().
 * @since_tizen 2.4
 *
 * @see maps_string_hashtable_create()
 * @see maps_string_hashtable_destroy()
 * @see maps_string_hashtable_clone()
 */
typedef void *maps_string_hashtable_h;

/**
 * @brief	Creates a new Hash Table of strings.
 * @details This function creates a new instance of Hash Table of strings,
 * associate a new handle with it and allocates all needed resources.
 * @since_tizen 2.4
 * @remarks @a table must be released using maps_string_hashtable_destroy().
 * \n @a table may be cloned using maps_string_hashtable_clone().
 *
 * @param[out]	table		The handle of newly created table
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_string_hashtable_clone()
 * @see maps_string_hashtable_destroy()
 */
int maps_string_hashtable_create(maps_string_hashtable_h *table);

/**
 * @brief	Destroys the Hash Table of strings and releases all its
 * resources.
 * @details This function destroys the Hash Table handle and releases all its
 * resources.
 * @since_tizen 2.4
 * @remarks All strings stored in the table will be released.
 *
 * @param[in]	table		The handle of table to be destroyed
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_string_hashtable_create().
 *
 * @see maps_string_hashtable_create()
 * @see maps_string_hashtable_clone()
 */
int maps_string_hashtable_destroy(maps_string_hashtable_h table);

/**
 * @brief	Clones the Hash Table of strings.
 * @details This function clones the Hash Table of strings @a origin and all its
 * resources.
 * \n The list handle @a origin may be created using
 * maps_string_hashtable_create().
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_string_hashtable_destroy().
 *
 * @param[in]	origin		The handle of the table to be cloned
 * @param[out]	cloned		The handle of the cloned table
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_string_hashtable_create().
 *
 * @see maps_string_hashtable_create()
 * @see maps_string_hashtable_destroy()
 */
int maps_string_hashtable_clone(const maps_string_hashtable_h origin,
				maps_string_hashtable_h *cloned);

/**
 * @brief	Sets the association between string key and value in the table.
 * @details This function sets the association between string key and value in
 * the Hash Table.
 * @since_tizen 2.4
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @param[in]	value		The string value of "value"
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_string_hashtable_create().
 *
 * @see maps_string_hashtable_get()
 * @see maps_string_hashtable_remove()
 * @see maps_string_hashtable_create()
 */
int maps_string_hashtable_set(maps_string_hashtable_h table, const char *key,
			      const char *value);

/**
 * @brief	Gets the string value associated with a string key in the table.
 * @details This function gets the string value associated with a string key in
 * the Hash Table.
 * @since_tizen 2.4
 * @remark The @a value must be released using free().
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @param[out]	value		The string value of "value"
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_string_hashtable_create().
 *
 * @see maps_string_hashtable_set()
 * @see maps_string_hashtable_remove()
 * @see maps_string_hashtable_create()
 */
int maps_string_hashtable_get(maps_string_hashtable_h table, const char *key,
				char **value);

/**
 * @brief	Removes the key-value pair from the table.
 * @details This function removes the key-value pair from the Hash Table.
 * @since_tizen 2.4
 * @remarks The resources, used by item will be released automatically.
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_string_hashtable_create().
 *
 * @see maps_string_hashtable_set()
 * @see maps_string_hashtable_get()
 * @see maps_string_hashtable_create()
 */
int maps_string_hashtable_remove(maps_string_hashtable_h table,
				 const char *key);

/**
 * @brief	Called once for each key-value pair while iterating through
 * the given table.
 * @details This function is called once for each key-value pair of the given
 * table while the table is being iterated
 * during the maps_string_hashtable_foreach() procedure.
 * @since_tizen 2.4
 * @remarks The @a key and @a value must be released using free().
 *
 * @param[in]	index		The current index of item
 * @param[in]	total		The total amount of items
 * @param[in]	key		The string value of "key"
 * @param[in]	value		The string value of "value"
 * @param[in]	user_data	The user data passed from
 * maps_string_hashtable_foreach()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_string_hashtable_foreach() will invoke this callback.
 *
 * @see maps_string_hashtable_foreach()
 */
typedef bool(*maps_string_hashtable_foreach_cb) (int index, int total,
						 char *key, char *value,
						 void *user_data);

/**
 * @brief	Gets the key-value pairs of the specified table.
 * @details This function delivers key-value pairs of a specified table via
 * maps_string_hashtable_foreach_cb() callback.
 * @since_tizen 2.4
 *
 * @param[in]	table		The handle of table
 * @param[in]	callback	The callback to be invoked for delivering each
 * key-value pair
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @pre @a table is created using maps_string_hashtable_create().
 * @post This function invokes maps_string_hashtable_foreach_cb() to deliver
 * key-value pairs.
 *
 * @see maps_string_hashtable_foreach_cb()
 * @see maps_string_hashtable_set()
 * @see maps_string_hashtable_get()
 * @see maps_string_hashtable_create()
 */
int maps_string_hashtable_foreach(maps_string_hashtable_h table,
				  maps_string_hashtable_foreach_cb callback,
				  void *user_data);

/**
 * @brief	Checks if key is in hash_table.
 * @details This function checks if the specified key is in the Hash Table of
 * strings.
 * @since_tizen 2.4
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @param[out]	contains	The flag of key presence
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_string_hashtable_create().
 *
 * @see maps_string_hashtable_set()
 * @see maps_string_hashtable_get()
 * @see maps_string_hashtable_create()
 */
int maps_string_hashtable_contains(maps_string_hashtable_h table,
				   const char *key, bool *contains);

/**
 * @}
 */

/*----------------------------------------------------------------------------*/

/**
 * @ingroup	CAPI_MAPS_CONTAINERS_MODULE
 * @defgroup	CAPI_MAPS_CONTAINERS_MODULE_ITEM_HASHTABLE Maps Item Hash Tables
 *
 * @file maps_extra_types.h
 * @brief This section contains API for using Hash Table of arbitrary items.
 *
 * @addtogroup CAPI_MAPS_CONTAINERS_MODULE_ITEM_HASHTABLE
 * @{
 * @brief This provides API for using Hash Table of arbitrary items.
 */

/**
 * @brief	The handle of Hash Table of arbitrary items.
 * @details The handle of Hash Table of arbitrary items can be obtained via call
 * of maps_item_hashtable_create().
 * @remarks To release the handle use maps_item_hashtable_destroy().
 * \n To clone the handle use maps_item_hashtable_clone().
 * \n To assign the value to a key use maps_item_hashtable_set().
 * \n The value, assigned with a specified key may be obtained using
 * maps_item_hashtable_get().
 * \n All key-value pairs of the table may be iterated using
 * maps_item_hashtable_foreach().
 * \n The key-value pair may be removed form the table using
 * maps_item_hashtable_remove().
 * \n To check if key is added to the table use
 * maps_item_hashtable_contains().
 * @since_tizen 2.4
 *
 * @see maps_item_hashtable_create()
 * @see maps_item_hashtable_destroy()
 * @see maps_item_hashtable_clone()
 */
typedef void *maps_item_hashtable_h;

/**
 * @brief	Creates a new Hash Table of arbitrary items.
 * @details This function creates a new instance of Hash Table of arbitrary
 * items, associate a new handle with it and allocates all needed resources.
 * @since_tizen 2.4
 * @remarks @a table must be released using maps_item_hashtable_destroy().
 * \n @a table may be cloned using maps_item_hashtable_clone().
 *
 * @param[out]	table		The handle of newly created table
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_item_hashtable_clone()
 * @see maps_item_hashtable_destroy()
 */
int maps_item_hashtable_create(maps_item_hashtable_h *table);

/**
* @brief	Destroys the Hash Table of arbitrary items and releases all its
* resources.
 * @details This function destroys the Hash Table handle and releases all its
 * resources.
 * @since_tizen 2.4
 * @remarks All items stored in the table will be released with corresponding
 * functions, specified during maps_item_hashtable_set().
 *
 * @param[in]	table		The handle of table to be destroyed
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_create()
 * @see maps_item_hashtable_clone()
 */
int maps_item_hashtable_destroy(maps_item_hashtable_h table);

/**
 * @brief	Clones the Hash Table of arbitrary items.
 * @details This function clones the Hash Table of arbitrary items @a origin and
 * all its resources.
 * \n The list handle @a origin may be created using
 * maps_item_hashtable_create().
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_item_hashtable_destroy().
 *
 * @param[in]	origin		The handle of the table to be cloned
 * @param[out]	cloned		The handle of the cloned table
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_create()
 * @see maps_item_hashtable_destroy()
 */
int maps_item_hashtable_clone(const maps_item_hashtable_h origin,
			      maps_item_hashtable_h *cloned);

/**
 * @brief	Clone function, making a copy of a Hash Table item.
 * @details This function is called once for each item of the given Hash Table
 * while it is being cloned during the maps_item_hashtable_clone() procedure.
 * \n This function must be assigned while adding new item to the table using
 * maps_item_hashtable_set().
 * @since_tizen 2.4
 * @remarks If this function returns the value different from MAPS_ERROR_NONE,
 * the cloned item will not be appended to the list.
 *
 * @param[in]	origin		The pointer to the item which is to be cloned
 * @param[out]	cloned		The pointer to the clone of the specified item
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_hashtable_clone() will invoke this callback.
 * @pre This function must be passed as parameter of using
 * maps_item_hashtable_set().
 *
 * @see maps_item_hashtable_clone()
 * @see maps_item_hashtable_set()
 */
typedef int (*maps_item_hashtable_clone_cb) (void *origin, void **cloned);

/**
 * @brief	Free function, destroying an item of the Hash Table.
 * @details This function is called once for each item of the given Hash Table
 * while this item is to be destroyed during maps_item_hashtable_destroy().
 * \n It is also called while the item is removed with
 * maps_item_hashtable_remove()
 * or the item value is exchanged with a new one with maps_item_hashtable_set().
 * @since_tizen 2.4
 * @remarks The return of this function is ignored.
 *
 * @param[in]	data		The pointer to the table item which is to be
 * destroyed
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_hashtable_destroy(), maps_item_hashtable_remove() or
 * maps_item_hashtable_set() will invoke this callback.
 *
 * @see maps_item_hashtable_destroy()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_set()
 */
typedef int (*maps_item_hashtable_free_cb) (void *data);

/**
 * @brief	Clones the table item of string type (char*).
 * @details This is a helper function of type #maps_item_hashtable_clone_cb,
 * which is useful while adding to the table an item of string type (char*).
 * This function is intended to be passed as the @a clone_func argument in the
 * maps_item_hashtable_set().
 * @since_tizen 2.4
 * @remarks This function will be called implicitly in the
 * maps_item_hashtable_clone() procedure.
 *
 * @param[in]	origin		The pointer of the item to be cloned
 * @param[out]	cloned		The pointer of the clone
 * @return	void pointer of a copy string
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_hashtable_clone() will invoke this callback.
 *
 * @see maps_item_hashtable_clone_cb()
 * @see maps_item_hashtable_clone()
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_free_string()
 */
int maps_item_hashtable_clone_string(void *origin, void **cloned);

/**
 * @brief	Destroys the table item of string type (char*).
 * @details This is a helper function of type #maps_item_hashtable_free_cb,
 * which is useful while adding to the table an item of string type (char*).
 * This function is intended to be passed as the @a free_func argument in the
 * maps_item_hashtable_set().
 * @since_tizen 2.4
 * @remarks This function will be called implicitly in the
 * maps_item_hashtable_destroy(), maps_item_hashtable_remove(),
 * maps_item_hashtable_set() procedures.
 * \n The return of this function is ignored.
 *
 * @param[in]	data		The pointer of the item
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_hashtable_destroy(), maps_item_hashtable_remove() or
 * maps_item_hashtable_set() will invoke this callback.
 *
 * @see maps_item_hashtable_free_cb()
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_destroy()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_clone_string()
 */
int maps_item_hashtable_free_string(void *data);

/**
 * @brief	Clones the table item of integer type (int*).
 * @details This is a helper function of type #maps_item_hashtable_clone_cb,
 * which is useful while adding to the table an item of integer type (int*).
 * This function is intended to be passed as the @a clone_func argument in the
 * maps_item_hashtable_set().
 * @since_tizen 2.4
 * @remarks This function will be called implicitly in the
 * maps_item_hashtable_clone() procedure.
 *
 * @param[in]	origin		The pointer of the item to be cloned
 * @param[out]	cloned		The pointer of the clone
 * @return	void pointer of a copy integer
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_hashtable_clone() will invoke this callback.
 *
 * @see maps_item_hashtable_clone_cb()
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_free_int()
 */
int maps_item_hashtable_clone_int(void *origin, void **cloned);

/**
 * @brief	Destroys the table item of integer type (int*).
 * @details This is a helper function of type #maps_item_hashtable_free_cb,
 * which is useful while adding to the table an item of integer type (int*).
 * This function is intended to be passed as the @a free_func argument in the
 * maps_item_hashtable_set().
 * @since_tizen 2.4
 * @remarks This function will be called implicitly in the
 * maps_item_hashtable_destroy(), maps_item_hashtable_remove(),
 * maps_item_hashtable_set() procedures.
 * \n The return of this function is ignored.
 *
 * @param[in]	data		The pointer of the item
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_hashtable_destroy(), maps_item_hashtable_remove() or
 * maps_item_hashtable_set() will invoke this callback.
 *
 * @see maps_item_hashtable_free_cb()
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_destroy()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_clone_int()
 */
int maps_item_hashtable_free_int(void *data);

/**
* @brief	Clones the table item of floating point numeric type (double*).
 * @details This is a helper function of type #maps_item_hashtable_clone_cb,
 * which is useful while adding to the table an item of floating point numeric
 * type (double*). This function is intended to be passed as the @a
 * clone_func argument in the maps_item_hashtable_set().
 * @since_tizen 2.4
 * @remarks This function will be called implicitly in the
 * maps_item_hashtable_clone() procedure.
 *
 * @param[in]	origin		The pointer of the item to be cloned
 * @param[out]	cloned		The pointer of the clone
 * @return	void pointer of a copy floating point number
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_hashtable_clone() will invoke this callback.
 *
 * @see maps_item_hashtable_clone_cb()
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_free_float()
 */
int maps_item_hashtable_clone_float(void *origin, void **cloned);

/**
 * @brief	Destroys the table item of floating point numeric type
 * (double*).
 * @details This is a helper function of type #maps_item_hashtable_free_cb,
 * which is useful while adding to the table an item of floating point numeric
 * type (double*).
 * This function is intended to be passed as the @a free_func argument in the
 * maps_item_hashtable_set().
 * @since_tizen 2.4
 * @remarks This function will be called implicitly in the
 * maps_item_hashtable_destroy(), maps_item_hashtable_remove(),
 * maps_item_hashtable_set() procedures.
 * \n The return of this function is ignored.
 *
 * @param[in]	data		The pointer of the item
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_hashtable_destroy(), maps_item_hashtable_remove() or
 * maps_item_hashtable_set() will invoke this callback.
 *
 * @see maps_item_hashtable_free_cb()
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_destroy()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_clone_float()
 */
int maps_item_hashtable_free_float(void *data);

/**
 * @brief	Sets the association between string key and string (char*)
 * value in the table.
 * @details This function sets the association between string key and string
 * value in the Hash Table.
 * @since_tizen 2.4
 * @remark New instances of key and value will be put to the table. The key and
 * value must be released.
 * \n This function uses implicitly maps_item_hashtable_clone_string() and
 * maps_item_hashtable_free_string().
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key The		string value of "key"
 * @param[in]	value		The string (char*) value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_create()
 * @see maps_item_hashtable_get_string()
 * @see maps_item_hashtable_clone_string()
 * @see maps_item_hashtable_free_string()
 */
int maps_item_hashtable_set_string(maps_item_hashtable_h table,
				   const char *key, const char *value);

 /**
 * @brief	Sets the association between string key and integer value in the
 * table.
 * @details This function sets the association between string key and integer
 * value in the Hash Table.
 * @since_tizen 2.4
 * @remark New instances of key and value will be put to the table. The key and
 * value must be released.
 * \n This function uses implicitly maps_item_hashtable_clone_int() and
 * maps_item_hashtable_free_int().
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key The		string value of "key"
 * @param[in]	value		The integer value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_create()
 * @see maps_item_hashtable_get_int()
 * @see maps_item_hashtable_clone_int()
 * @see maps_item_hashtable_free_int()
 */
int maps_item_hashtable_set_int(maps_item_hashtable_h table, const char *key,
				const int value);

/**
 * @brief	Sets the association between string key and floating point
 * numeric value in the table.
 * @details This function sets the association between string key and floating
 * point numeric value in the Hash Table.
 * @since_tizen 2.4
 * @remark New instances of key and value will be put to the table. The key and
 * value must be released.
 * \n This function uses implicitly maps_item_hashtable_clone_int() and
 * maps_item_hashtable_free_int().
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @param[in]	value		The floating point numeric value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_create()
 * @see maps_item_hashtable_get_float()
 * @see maps_item_hashtable_clone_float()
 * @see maps_item_hashtable_free_float()
 */
int maps_item_hashtable_set_float(maps_item_hashtable_h table,
				  const char *key, const double value);

/**
 * @brief	Sets the association between key and value in the table.
 * @details This function sets the association between string key and
 * arbitrary typed value in the Hash Table.
 * \n To allow the Hash Table manage arbitrary types properly, @a clone_func of
 * type #maps_item_hashtable_clone_cb as well as @a free_func of type
 * #maps_item_hashtable_free_cb must be specified. Those functions will be
 * called implicitly during procedures of cloning, exchanging and removing
 * Hash Table items.
 * \n For the convenience there are defined cloning and freeing functions of
 * basic types:
 * - maps_item_hashtable_clone_string() and maps_item_hashtable_free_string()
 * for string values of type (char*)
 * - maps_item_hashtable_clone_int() and maps_item_hashtable_free_int() for
 * integer values of type (int)
 * - maps_item_hashtable_clone_float() and maps_item_hashtable_free_float() for
 * floating point numeric values of type (double)
 * .
 * However for listed basic types it is recommended to use following convenience
 * macros which applies those functions implicitly:
 * - maps_item_hashtable_set_string()
 * - maps_item_hashtable_set_int()
 * - maps_item_hashtable_set_float()
 * .
 * @since_tizen 2.4
 * @remark New instances of key and value will be put to the table. The @a key
 * and @a value must be released.
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @param[in]	value		The value
 * @param[in]	clone_func	The function for cloning the value
 * @param[in]	free_func	The function for freeing the value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_clone_string()
 * @see maps_item_hashtable_clone_int()
 * @see maps_item_hashtable_clone_float()
 * @see maps_item_hashtable_free_string()
 * @see maps_item_hashtable_free_int()
 * @see maps_item_hashtable_free_float()
 * @see maps_item_hashtable_set_string()
 * @see maps_item_hashtable_set_int()
 * @see maps_item_hashtable_set_float()
 * @see maps_item_hashtable_create()
 */
int maps_item_hashtable_set(maps_item_hashtable_h table, const char *key,
			    const void *value,
			    maps_item_hashtable_clone_cb clone_func,
			    maps_item_hashtable_free_cb free_func);

/**
 * @brief	Gets the association between string key and string (char*)
 * value in the table.
 * @details This function gets the association between string key and string
 * value in the Hash Table.
 * @since_tizen 2.4
 * @remark @a value must be released using free().
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key The		string value of "key"
 * @param[out]	value		The string (char*) value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_create()
 * @see maps_item_hashtable_set_string()
 * @see maps_item_hashtable_clone_string()
 * @see maps_item_hashtable_free_string()
 */
int maps_item_hashtable_get_string(maps_item_hashtable_h table,
				   const char *key, char **value);

/**
 * @brief	Gets the association between string key and integer value in the
 * table.
 * @details This function gets the association between string key and integer
 * value in the Hash Table.
 * @since_tizen 2.4
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key The		string value of "key"
 * @param[out]	value		The integer value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_create()
 * @see maps_item_hashtable_set_int()
 * @see maps_item_hashtable_clone_int()
 * @see maps_item_hashtable_free_int()
 */
int maps_item_hashtable_get_int(maps_item_hashtable_h table, const char *key,
				int *value);

/**
 * @brief	Gets the association between string key and floating point
 * numeric value in the table.
 * @details This function gets the association between string key and floating
 * point numeric value in the Hash Table.
 * @since_tizen 2.4
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @param[out]	value		The floating point numeric value
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_create()
 * @see maps_item_hashtable_set_float()
 * @see maps_item_hashtable_clone_float()
 * @see maps_item_hashtable_free_float()
 */
int maps_item_hashtable_get_float(maps_item_hashtable_h table,
				  const char *key, double *value);

/**
 * @brief	Gets the value associated with a string key in the table.
 * @details This function gets the value associated with a string key in the
 * Hash Table.
 * @since_tizen 2.4
 * @remark Th @a value must be released by you.
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @param[out]	value		The corresponding value of "value"
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_remove()
 * @see maps_item_hashtable_create()
 */
int maps_item_hashtable_get(maps_item_hashtable_h table, const char *key,
			    void **value);

/**
 * @brief	Removes the key-value pair from the table.
 * @details This function removes the key-value pair from the Hash Table.
 * @since_tizen 2.4
 * @remarks The resources, used by item will be released automatically.
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_create()

 */
int maps_item_hashtable_remove(maps_item_hashtable_h table, const char *key);

/**
 * @brief	Called once for each key-value pair while iterating through the
 * given table.
 * @details This function is called once for each key-value pair of the given
 * table while the table is being iterated
 * during the maps_item_hashtable_foreach() procedure.
 * @since_tizen 2.4
 * @remarks The @a key and @a value must be released by you.
 *
 * @param[in]	index		The current index of item
 * @param[in]	total		The total amount of items
 * @param[in]	key		The string value of "key"
 * @param[in]	value		The corresponding value of "value"
 * @param[in]	user_data	The user data passed from
 * maps_item_hashtable_foreach()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_item_hashtable_foreach() will invoke this callback.
 *
 * @see maps_item_hashtable_foreach()
 */
typedef bool(*maps_item_hashtable_foreach_cb) (int index, int total,
					       char *key, void *value,
					       void *user_data);

/**
 * @brief	Gets the key-value pairs of the specified table.
 * @details This function delivers key-value pairs of a specified table via
 * maps_item_hashtable_foreach_cb() callback.
 * @since_tizen 2.4
 *
 * @param[in]	table		The handle of table
 * @param[in]	callback	The callback to be invoked for delivering each
 * key-value pair
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @pre @a table is created using maps_item_hashtable_create().
 * @post This function invokes maps_item_hashtable_foreach_cb() to deliver
 * key-value pairs.
 *
 * @see maps_item_hashtable_foreach_cb()
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_create()
 */
int maps_item_hashtable_foreach(maps_item_hashtable_h table,
				maps_item_hashtable_foreach_cb callback,
				void *user_data);

/**
 * @brief	Checks if key is in hash_table.
 * @details This function checks if the specified key is in the Hash Table.
 * @since_tizen 2.4
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The string value of "key"
 * @param[out]	contains	The flag of key presence
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_item_hashtable_create().
 *
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_get()
 * @see maps_item_hashtable_create()

 */
int maps_item_hashtable_contains(maps_item_hashtable_h table, const char *key,
				 bool *contains);


/*----------------------------------------------------------------------------*/

/**
 * @ingroup	CAPI_MAPS_CONTAINERS_MODULE
 * @defgroup	CAPI_MAPS_CONTAINERS_MODULE_UTILITY Maps Container Utilities
 *
 * @file maps_extra_types.h
 * @brief This section contains API helping in usage of container types, such as
 * maps_item_list_h and maps_item_hashtable_h.
 *
 * @addtogroup CAPI_MAPS_CONTAINERS_MODULE_UTILITY
 * @{
 * @brief This provides API for helping usage of container types, such as
 * maps_item_list_h and maps_item_hashtable_h.
 */


/**
* @brief	Copies the table item of arbitrary type type without cloning.
 * @details This is a helper function of type #maps_item_no_clone,
 * which is useful while adding to the table or list an item of arbitrary type
 * without cloning it. It allows to add to the container the given object
 * instead of its clone and, consequently, reduce the memory consumption.
 * This function is intended to be passed as the @a
 * clone_func argument in functions, such as the maps_item_hashtable_set() ot
 * maps_item_list_append().
 * @since_tizen 3.0
 * @remarks This function will be called implicitly in the
 * maps_item_hashtable_clone(), maps_item_list_append() or similar procedure.
 *
 * @param[in]	origin		The pointer of the item to be cloned
 * @param[out]	cloned		The pointer of the clone
 * @return	void pointer of a copy floating point number
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre maps_item_hashtable_clone() will invoke this callback.
 * @pre maps_item_list_append() will invoke this callback.
 *
 * @see maps_item_list_append()
 * @see maps_item_hashtable_clone_cb()
 * @see maps_item_hashtable_set()
 * @see maps_item_hashtable_free_float()
 */
int maps_item_no_clone(void *origin, void **cloned);

/**
 * @}
 */

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_SERVICE_API_EXTRA_TYPES_H__ */
