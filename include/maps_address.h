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

#ifndef __MAPS_ADDRESS_H__
#define __MAPS_ADDRESS_H__

/**
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAPS_ADDRESS_MODULE Address
 *
 * @file maps_address.h
 * @brief This file contains the functions related to address information.
 *
 * @addtogroup CAPI_MAPS_ADDRESS_MODULE
 * @{
 * @brief This provides APIs related to Address information used in Geocoding
 * and Reverse Geocoding
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The address handle.
 * @details The address handle can be obtained via calling of maps_address_create()
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 */
typedef void *maps_address_h;

/**
 * @brief The address list handle
 * @details The address list handle can be obtained via calling of maps_address_list_create().
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef void *maps_address_list_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Creates a new address handle.
 * @details This function creates a new address handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a address must be released using maps_address_destroy().
 * \n @a address may be cloned using maps_address_clone().
 *
 * @param[out]	address		The address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_address_destroy()
 * @see maps_address_clone()
 */
int maps_address_create(maps_address_h *address);

/**
 * @brief	Destroys the address handle and releases all its resources.
 * @details This function destroys the address handle and releases all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_clone()
 */
int maps_address_destroy(maps_address_h address);

/**
 * @brief	Clones the address handle.
 * @details This function clones the address handle @a origin and all its
 * resources.
 * \n Address handle @a origin may be created using maps_address_create().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a cloned must be released using maps_address_destroy().
 *
 * @param[in]	origin		The original address handle
 * @param[out]	cloned		A cloned address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_destroy()
 */
int maps_address_clone(const maps_address_h origin, maps_address_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the building number of the address handle.
 * @details This function gets the building number of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a building_number must be released using free().
 *
 * @param[in]	address		The address handle
 * @param[out]	building_number	The building number of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_building_number()
 * @see maps_address_create()
 */
int maps_address_get_building_number(const maps_address_h address,
								char **building_number);

/**
 * @brief	Gets the street name of the address handle.
 * @details This function gets the street name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a street must be released using free().
 *
 * @param[in]	address	The address handle
 * @param[out]	street	The street name of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_street()
 * @see maps_address_create()
 */
int maps_address_get_street(const maps_address_h address, char **street);

/**
 * @brief	Gets the district name of the address handle.
 * @details This function gets the district name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a district must be released using free().
 *
 * @param[in]	address		The address handle
 * @param[out]	district	The district name of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_district()
 * @see maps_address_create()
 */
int maps_address_get_district(const maps_address_h address, char **district);

/**
 * @brief	Gets the city name of the address handle.
 * @details This function gets the city name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a city must be released using free().
 *
 * @param[in]	address		The address handle
 * @param[out]	city		The city name of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_city()
 * @see maps_address_create()
 */
int maps_address_get_city(const maps_address_h address, char **city);

/**
 * @brief	Gets the state name of the address handle
 * @details This function gets the state name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a state must be released using free().
 *
 * @param[in]	address		The address handle
 * @param[out]	state		The state name of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_state()
 * @see maps_address_create()
 */
int maps_address_get_state(const maps_address_h address, char **state);

/**
 * @brief	Gets the country name of the address handle.
 * @details This function gets the country name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a country must be released using free().
 *
 * @param[in]	address		The address handle
 * @param[out]	country		The country name of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_country()
 * @see maps_address_create()
 */
int maps_address_get_country(const maps_address_h address, char **country);

/**
 * @brief	Gets the country code of the address handle.
 * @details This function gets the country code of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a country_code must be released using free().
 *
 * @param[in]	address		The address handle
 * @param[out]	country_code	The country code of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_country_code()
 * @see maps_address_create()
 */
int maps_address_get_country_code(const maps_address_h address,
								char **country_code);

/**
 * @brief	Gets the county of the address handle.
 * @details This function gets the county of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a county must be released using free().
 *
 * @param[in]	address		The address handle
 * @param[out]	county		The county of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_county()
 * @see maps_address_create()
 */
int maps_address_get_county(const maps_address_h address, char **county);

/**
 * @brief	Gets the postal code of the address handle.
 * @details This function gets the postal code of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a postal_code must be released using free().
 *
 * @param[in]	address		The address handle
 * @param[out]	postal_code	The postal code of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_postal_code()
 * @see maps_address_create()
 */
int maps_address_get_postal_code(const maps_address_h address,
				 char **postal_code);

/**
 * @brief	Gets the free text of the address handle.
 * @details This function gets the free text of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 * @remarks @a freetext must be released using free().
 *
 * @param[in]	address		The address handle
 * @param[out]	freetext	The free text of the address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_set_freetext()
 * @see maps_address_create()
 */
int maps_address_get_freetext(const maps_address_h address, char **freetext);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Sets the building number of the address handle.
 * @details This function sets the building number of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	building_number	The building number to be set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_number()
 */
int maps_address_set_building_number(maps_address_h address,
				     const char *building_number);

/**
 * @brief	Sets the street name of the address handle.
 * @details This function sets the street name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	street		The street name to be set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_street()
 */
int maps_address_set_street(maps_address_h address, const char *street);

/**
 * @brief	Sets the district name of the address handle.
 * @details This function sets the district name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	district	The district name to be set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_district()
 */
int maps_address_set_district(maps_address_h address, const char *district);

/**
 * @brief	Sets the city name of the address handle.
 * @details This function sets the city name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	city		The city name to be set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_city()
 */
int maps_address_set_city(maps_address_h address, const char *city);

/**
 * @brief	Sets the state name of the address handle.
 * @details This function sets the state name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	state		The state name to be set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_state()
 */
int maps_address_set_state(maps_address_h address, const char *state);

/**
 * @brief	Sets the country name of the address handle.
 * @details This function sets the country name of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	country		The country name to be set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_country()
 */
int maps_address_set_country(maps_address_h address, const char *country);

/**
 * @brief	Sets the country code of the address handle.
 * @details This function sets the country code of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	country_code	The country code to be set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_country_code()
 */
int maps_address_set_country_code(maps_address_h address,
				  const char *country_code);

/**
 * @brief	Sets the county of the address handle.
 * @details This function sets the county of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	county		The county to be set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_county()
 */
int maps_address_set_county(maps_address_h address, const char *county);

/**
 * @brief	Sets the postal code of the address handle.
 * @details This function sets the postal code of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	postal_code	The postal code to be set
 * @return	0, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_postal_code()
 */
int maps_address_set_postal_code(maps_address_h address,
				 const char *postal_code);

/**
 * @brief	Sets the free text of the address handle.
 * @details This function sets the free text of the address handle.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address		The address handle
 * @param[in]	freetext	The free text to be set
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a address is created using maps_address_create().
 *
 * @see maps_address_create()
 * @see maps_address_get_freetext()
 */
int maps_address_set_freetext(maps_address_h address, const char *freetext);

/**
 * @brief	Creates a address list having a set of addresses.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[out]	address_list	The address list handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see maps_address_list_destroy()
 */
int maps_address_list_create(maps_address_list_h *address_list);

/**
 * @brief	Appends an address to an address list.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 * @remarks	Don't release @a address before removed from the list using maps_address_list_remove().
 *
 * @param[in]	address_list	The address list handle
 * @param[in]	address	The address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see maps_address_list_create()
 * @see maps_address_list_destroy()
 * @see maps_address_create()
 */
int maps_address_list_append(maps_address_list_h address_list, maps_address_h address);

/**
 * @brief	Removes an address from an address list.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 * @remarks	@a address is just excepted from the list.\n
 * It must be released using maps_address_destroy().
 *
 * @param[in]	address_list	The address list handle
 * @param[in]	address	The address handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_address_list_create()
 * @see maps_address_list_destroy()
 * @see maps_address_destroy()
 */
int maps_address_list_remove(maps_address_list_h address_list, maps_address_h address);

/**
 * @brief	Gets the number of elements in an address list.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address_list	The address list handle
 * @param[out]	length	The number of elements in the address list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_address_list_create()
 * @see maps_address_list_destroy()
 */
int maps_address_list_get_length(maps_address_list_h address_list, int *length);

/**
 * @brief	Called iteratively to get address information.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	index	The index of iteration
 * @param[in]	address	The address handle
 * @param[in]	user_data	The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop, \n
 *         @c false to break out of the loop
 *
 * @see maps_address_list_foreach_address()
 * @see maps_address_list_destroy()
 */
typedef bool (*maps_address_cb)(int index, maps_address_h address, void *user_data);

/**
 * @brief Retrieves all addresses by invoking a specific callback for each address of address list.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address_list	The address list handle
 * @param[in]	callback	The iteration callback
 * @param[in]	user_data	The user data to be passed to the callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_address_list_create()
 * @see maps_address_list_destroy()
 * @see maps_address_list_get_length()
 */
int maps_address_list_foreach(maps_address_list_h address_list,
								maps_address_cb callback, void *user_data);

/**
 * @brief	Frees all of the memory used by a address list.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]	address_list	The address list handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_address_list_create()
 */
int maps_address_list_destroy(maps_address_list_h address_list);


#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_ADDRESS_H__ */
