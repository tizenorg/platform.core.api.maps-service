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

#ifndef __MAPS_COORD_LIST_H__
#define __MAPS_COORD_LIST_H__

#include <tizen_type.h>
#include <maps_coordinates.h>

/**
 * @ingroup	CAPI_MAPS_PLUGIN_AND_PROVIDERS_MODULE
 * @defgroup	CAPI_MAPS_CONTAINERS_MODULE Container Data Types
 *
 * @addtogroup CAPI_MAPS_CONTAINERS_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* TODO: Move to the Mapping API section */

/**
 * @ingroup	CAPI_MAPS_CONTAINERS_MODULE
 * @defgroup	CAPI_MAPS_CONTAINERS_MODULE_COORD_LIST Geographical Coordinates Lists
 *
 * @addtogroup CAPI_MAPS_CONTAINERS_MODULE_COORD_LIST
 * @{
 * @file maps_coord_list.h
 * @brief This provides API for using List of geographical coordinates pointers.
 */

/**
 * @brief	Item List handle.
 * @details The Item List handle can be obtained via call of
 * maps_coord_list_create().
 * @remarks To release the handle use maps_coord_list_destroy().
 * \n To clone the handle use maps_coord_list_clone().
 * \n The coords of the list may be iterated using maps_coord_list_foreach().
 * \n To append an coord to the list use maps_coord_list_append().
 * \n The coords of the list may be removed maps_coord_list_remove_all().
 * @since_tizen 2.4
 *
 * @see maps_coord_list_create()
 * @see maps_coord_list_destroy()
 * @see maps_coord_list_clone()
 */
typedef void *maps_coord_list_h;

/**
 * @brief	Creates a new list of coord pointers.
 * @details This function creates a new instance of coord pointer list, associate
 * a new handle with it and allocates all needed resources.
 * @since_tizen 2.4
 * @remarks @a list must be released using maps_coord_list_destroy().
 * \n @a list may be cloned using maps_coord_list_clone().
 *
 * @param[out]	list		The handle of newly created list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_coord_list_destroy()
 * @see maps_coord_list_clone()
 */
int maps_coord_list_create(maps_coord_list_h *list);

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
 * @pre @a list is created using maps_coord_list_create().
 *
 * @see maps_coord_list_create()
 * @see maps_coord_list_clone()
 */
int maps_coord_list_destroy(maps_coord_list_h list);

/**
 * @brief	Clones the list of coord pointers.
 * @details This function clones the list of coord pointers @a origin and all its
 * resources.
 * \n The list handle @a origin may be created using maps_coord_list_create().
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_coord_list_destroy().
 *
 * @param[in]	origin		The handle of list to be cloned
 * @param[out]	cloned		The handle of the newly cloned list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is created using maps_coord_list_create().
 *
 * @see maps_coord_list_create()
 * @see maps_coord_list_destroy()
 */
int maps_coord_list_clone(const maps_coord_list_h origin,
			  maps_coord_list_h *cloned);

/**
 * @brief	Appends an coord to the list.
 * @details This function appends a clone of @a coord to the @ list.
 * @since_tizen 2.4
 *
 * @param[in]	list		The handle of list
 * @param[in]	coord		The coord pointer to be pointed to the list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_coord_list_create().
 *
 * @see maps_coord_list_foreach()
 * @see maps_coord_list_remove_all()
 * @see maps_coord_list_create()
 */
int maps_coord_list_append(maps_coord_list_h list,
			   const maps_coordinates_h coord);

/**
 * @brief	Called once for each coord while iterating through the given
 * list.
 * @details This function is called once for each coord of the given list while
 * the list is being iterated
 * during the maps_coord_list_foreach() procedure.
 * @since_tizen 2.4
 * @remarks The @a data must NOT be released by you.
 *
 * @param[in]	index		The current index of coord
 * @param[in]	total		The total amount of coords
 * @param[in]	data		The pointer to the list coord
 * @param[in]	user_data	The user data passed from
 * maps_coord_list_foreach()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_coord_list_foreach() will invoke this callback.
 *
 * @see maps_coord_list_foreach()
 */
typedef bool(*maps_coord_list_foreach_cb) (int index, int total,
					   maps_coordinates_h coord,
					   void *user_data);

/**
 * @brief	Gets the coords of the specified list.
 * @details This function delivers coords of a specified list via
 * maps_coord_list_foreach_cb() callback.
 * @since_tizen 2.4
 *
 * @param[in]	list		The handle of list
 * @param[in]	callback	The callback to be invoked for delivering each
 * list coord
 * @param[in]	user_data	The user data to be passed to the callback
 * function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @pre @a list is created using maps_coord_list_create().
 * @post This function invokes maps_coord_list_foreach_cb() to deliver list
 * coords.
 *
 * @see maps_coord_list_foreach_cb()
 * @see maps_coord_list_append()
 * @see maps_service_create()
 */
int maps_coord_list_foreach(maps_coord_list_h list,
			   maps_coord_list_foreach_cb callback,
			   void *user_data);

/**
 * @brief	Removes and destroys list coord.
 * @details This function removes the given coords from the @a list and releases
 * resources, assigned with it.
 * @since_tizen 2.4
 *
 * @param[in]	list		The handle of list
 * @param[in]	coord		The coordinates to remove
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_coord_list_create().
 *
 * @see maps_coord_list_create()
 * @see maps_coord_list_remove_all()
 */
int maps_coord_list_remove(maps_coord_list_h list, maps_coordinates_h coord);

/**
 * @brief	Removes and destroys all list coords.
 * @details This function removes all coords of the @a list and releases
 * resources, assigned with them.
 * If two elements contain the same data, only the first is removed.
 * If none of the elements contain the data, the GList is unchanged.
 * @since_tizen 3.0
 *
 * @param[in]	list		The handle of list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_coord_list_create().
 *
 * @see maps_coord_list_create()
 */
int maps_coord_list_remove_all(maps_coord_list_h list);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_COORD_LIST_H__*/
