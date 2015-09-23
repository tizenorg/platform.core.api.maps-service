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

#ifndef __MAPS_SERVICE_API_EXTRA_TYPES_PRIVATE_H__
#define __MAPS_SERVICE_API_EXTRA_TYPES_PRIVATE_H__

#include <tizen_type.h>



#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------------------*/

/*
 * @internal
 * @ingroup	CAPI_MAPS_CONTAINERS_MODULE
 * @defgroup	CAPI_MAPS_CONTAINERS_MODULE_INT_HASHTABLE Maps Integer Hash Tables
 *
 * @file maps_extra_types.h
 * @brief This section contains API for using Hash Table of integers.
 *
 * @addtogroup CAPI_MAPS_CONTAINERS_MODULE_INT_HASHTABLE
 * @{
 * @brief This provides API for using Hash Table of ntegers.
 */

/*
 * @internal
 * @brief	The handle of Hash Table of ntegers.
 * @details The handle of Hash Table of integers can be obtained via call of
 * maps_int_hashtable_create().
 * @remarks To release the handle use maps_int_hashtable_destroy().
 * \n To clone the handle use maps_int_hashtable_clone().
 * \n To assign the value to a key use maps_int_hashtable_set().
 * \n The value, assigned with a specified key may be obtained using
 * maps_int_hashtable_get().
 * \n All key-value pairs of the table may be iterated using
 * maps_int_hashtable_foreach().
 * \n The key-value pair may be removed form the table using
 * maps_int_hashtable_remove().
 * \n To check if key is added to the table use
 * maps_int_hashtable_contains().
 * @since_tizen 3.0
 *
 * @see maps_int_hashtable_create()
 * @see maps_int_hashtable_destroy()
 * @see maps_int_hashtable_clone()
 */
typedef void *maps_int_hashtable_h;

/*
 * @internal
 * @brief	Creates a new Hash Table of ntegers.
 * @details This function creates a new instance of Hash Table of integers,
 * associate a new handle with it and allocates all needed resources.
 * @since_tizen 3.0
 * @remarks @a table must be released using maps_int_hashtable_destroy().
 * \n @a table may be cloned using maps_int_hashtable_clone().
 *
 * @param[out]	table		The handle of newly created table
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_int_hashtable_clone()
 * @see maps_int_hashtable_destroy()
 */
int maps_int_hashtable_create(maps_int_hashtable_h *table);

/*
 * @internal
 * @brief	Destroys the Hash Table of integers and releases all its
 * resources.
 * @details This function destroys the Hash Table handle and releases all its
 * resources.
 * @since_tizen 3.0
 * @remarks All integers stored in the table will be released.
 *
 * @param[in]	table		The handle of table to be destroyed
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_int_hashtable_create().
 *
 * @see maps_int_hashtable_create()
 * @see maps_int_hashtable_clone()
 */
int maps_int_hashtable_destroy(maps_int_hashtable_h table);

/*
 * @internal
 * @brief	Clones the Hash Table of integerss.
 * @details This function clones the Hash Table of integers @a origin and all
 * its resources.
 * \n The list handle @a origin may be created using
 * maps_int_hashtable_create().
 * @since_tizen 3.0
 * @remarks @a cloned must be released using maps_int_hashtable_destroy().
 *
 * @param[in]	origin		The handle of the table to be cloned
 * @param[out]	cloned		The handle of the cloned table
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_int_hashtable_create().
 *
 * @see maps_int_hashtable_create()
 * @see maps_int_hashtable_destroy()
 */
int maps_int_hashtable_clone(const maps_int_hashtable_h origin,
			     maps_int_hashtable_h *cloned);

/*
 * @internal
 * @brief	Sets the association between integer key and value in the table.
 * @details This function sets the association between int key and value in
 * the Hash Table.
 * @since_tizen 3.0
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The integer value of "key"
 * @param[in]	value		The integer value of "value"
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_int_hashtable_create().
 *
 * @see maps_int_hashtable_get()
 * @see maps_int_hashtable_remove()
 * @see maps_int_hashtable_create()
 */
int maps_int_hashtable_set(maps_int_hashtable_h table, const int key,
			   const int value);

/*
 * @internal
 * @brief	Gets the integer value associated with an integer key in the
 * table.
 * @details This function gets the integer value associated with a integer key
 * in the Hash Table.
 * @since_tizen 3.0
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The integer value of "key"
 * @param[out]	value		The integer value of "value"
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_int_hashtable_create().
 *
 * @see maps_int_hashtable_set()
 * @see maps_int_hashtable_remove()
 * @see maps_int_hashtable_create()
 */
int maps_int_hashtable_get(maps_int_hashtable_h table, const int key,
			   int *value);

/*
 * @internal
 * @brief	Removes the key-value pair from the table.
 * @details This function removes the key-value pair from the Hash Table.
 * @since_tizen 3.0
 * @remarks The resources, used by item will be released automatically.
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The integer value of "key"
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_int_hashtable_create().
 *
 * @see maps_int_hashtable_set()
 * @see maps_int_hashtable_get()
 * @see maps_int_hashtable_create()
 */
int maps_int_hashtable_remove(maps_int_hashtable_h table,
			      const int key);

/*
 * @internal
 * @brief	Called once for each key-value pair while iterating through
 * the given table.
 * @details This function is called once for each key-value pair of the given
 * table while the table is being iterated
 * during the maps_int_hashtable_foreach() procedure.
 * @since_tizen 3.0
 *
 * @param[in]	index		The current index of item
 * @param[in]	total		The total amount of items
 * @param[in]	key		The integer value of "key"
 * @param[in]	value		The integer value of "value"
 * @param[in]	user_data	The user data passed from
 * maps_int_hashtable_foreach()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_int_hashtable_foreach() will invoke this callback.
 *
 * @see maps_int_hashtable_foreach()
 */
typedef bool(*maps_int_hashtable_foreach_cb) (int index, int total,
					      const int key, const int value,
					      void *user_data);

/*
 * @internal
 * @brief	Gets the key-value pairs of the specified table.
 * @details This function delivers key-value pairs of a specified table via
 * maps_int_hashtable_foreach_cb() callback.
 * @since_tizen 3.0
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
 * @pre @a table is created using maps_int_hashtable_create().
 * @post This function invokes maps_int_hashtable_foreach_cb() to deliver
 * key-value pairs.
 *
 * @see maps_int_hashtable_foreach_cb()
 * @see maps_int_hashtable_set()
 * @see maps_int_hashtable_get()
 * @see maps_int_hashtable_create()
 */
int maps_int_hashtable_foreach(maps_int_hashtable_h table,
				  maps_int_hashtable_foreach_cb callback,
				  void *user_data);

/*
 * @internal
 * @brief	Checks if key is in hash_table.
 * @details This function checks if the specified key is in the Hash Table of
 * integers.
 * @since_tizen 3.0
 *
 * @param[in]	table		The handle of the table
 * @param[in]	key		The integer value of "key"
 * @param[out]	contains	The flag of key presence
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a table is created using maps_int_hashtable_create().
 *
 * @see maps_int_hashtable_set()
 * @see maps_int_hashtable_get()
 * @see maps_int_hashtable_create()
 */
int maps_int_hashtable_contains(maps_int_hashtable_h table,
				const int key, bool *contains);

/*
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif			/* __MAPS_SERVICE_API_EXTRA_TYPES_PRIVATE_H__ */
