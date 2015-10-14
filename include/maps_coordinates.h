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

#ifndef __MAPS_COORDINATES_H__
#define __MAPS_COORDINATES_H__

/**
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAPS_COORDS_MODULE Coordinates
 *
 * @file maps_coordinates.h
 * @brief This file contains the functions related to Geographical Coordinates.
 * @addtogroup CAPI_MAPS_COORDS_MODULE
 * @{
 * @brief This provides APIs related to Geographical Coordinates
 * @details The Geographical Coordinates API includes following features:
 *  * Operations with Geographical Coordinates
 *  * Operatiosn with Lists of Geographical Coordinates.
 *  .
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Handle of the Geographical Coordinates.
 * @details The Geographical Coordinates handle can be obtained via call of
 * maps_coordinates_create().
 * @since_tizen 2.4
 * @remarks To release the handle use maps_coordinates_destroy().
 * \n To clone the handle use maps_coordinates_clone().
 *
 * @see maps_coordinates_create()
 * @see maps_coordinates_destroy()
 * @see maps_coordinates_clone()
 */
typedef void *maps_coordinates_h;

/**
 * @brief	Geographical Coordinates List handle.
 * @details The Geographical Coordinate List handle can be obtained via call of
 * maps_coordinates_list_create().
 * @since_tizen 3.0
 * @remarks To release the handle use maps_coordinates_list_destroy().
 * \n To clone the handle use maps_coordinates_list_clone().
 * \n Note: the cloning procedure is shallow, so the Geographical Coordinates
 * handles, comprising the List, will be simply copied without cloning the
 * memory.
 * \n The Geographical Coordinates, comprising the List may be iterated using
 * maps_coordinates_list_foreach_coordinates().
 * \n To append Geographical Coordinates to the List use
 * maps_coordinates_list_append().
 * \n To get the length of the List use maps_coordinates_list_get_length().
 * \n The Geographical Coordinates may be removed from the List using
 * maps_coordinates_list_remove_all().
 *
 * @see maps_coordinates_list_create()
 * @see maps_coordinates_list_destroy()
 * @see maps_coordinates_list_clone()
 * @see maps_coordinates_list_append()
 * @see maps_coordinates_list_foreach_coordinates()
 * @see maps_coordinates_list_remove_all()
 * @see #maps_coordinates_h
 */
typedef void *maps_coordinates_list_h;

/**
 * @brief	Structure of the Geographical Coordinates.
 * @details This structure represents a Geographical Coordinates, specified with
 * a @a latitude and @a longitude values.
 * \n The @a latitude must be in range of [-90.0, 90.0].
 * \n The @a longitude must be in range of [-180.0, 180.0].
 * @since_tizen 2.4
 * @remarks #maps_coordinates_h is a void pointer to the #maps_coordinates_s.
 *
 * @see #maps_coordinates_h
 * @see maps_coordinates_create
 * @see maps_coordinates_clone
 * @see maps_coordinates_destroy
 */
typedef struct _maps_coordinates_s {
	double latitude;	/**< The latitude [-90.0 ~ 90.0] (degrees) */
	double longitude;	/**< The longitude [-180.0 ~ 180.0] (degrees) */
} maps_coordinates_s;


/**
 * @brief	Called once for each Geographical Coordinates while iterating
 * through the Geographical Coordinate List.
 * @details This function is called once for each Geographical Coordinates of
 * the given Geographical Coordinate List @a list while iterated in the
 * maps_coordinates_list_foreach_coordinates() procedure.
 * @since_tizen 3.0
 * @remarks The Geographical Coordinates handle @a coordinates must be released
 * using maps_coordinates_destroy().
 *
 * @param[in]	index		The current index of Geographical Coordinates
 * @param[in]	total		The total amount of Geographical Coordinates in
 * the List
 * @param[in]	coordinates	The retrieved current Geographical Coordinates
 * handle
 * @param[in]	user_data	The user data pointer passed from
 * maps_coordinates_list_foreach_coordinates()
 * @return	@c true to continue with the next iteration of the loop, \n @c
 * false to break out of the loop
 *
 * @pre maps_coordinates_list_foreach_coordinates() will invoke this callback.
 *
 * @par Example
 * @code
#include <maps_service.h>

static bool foreach_coordinates_cb(int index, int total,
				   maps_coordinates_h coordinates,
				   void *user_data)
{
	int error = MAPS_ERROR_NONE;
	double latitude = .0;
	double longitude = .0;

	if (!coordinates)
		return false; // Error

	// Process retrieved Geographical Coordinates
	error = maps_coordinates_get_latitude_longitude(coordinates, &latitude, &longitude);
	if (error != MAPS_ERROR_NON) {
		// Handle the error...
	}

	// Release the Geographical Coordinates handle
	maps_coordinates_destroy(coordinates);

	return true;
}
 * @endcode
 *
 * @see maps_coordinates_list_foreach_coordinates()
 * @see #maps_coordinates_h
 * @see maps_coordinates_destroy()
 */
typedef bool(*maps_coordinates_list_foreach_cb) (int index, int total,
						 maps_coordinates_h coordinates,
						 void *user_data);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Creates a new instance of Geographical Coordinates and initiates
 * a handle with it.
 * @details This function creates @a coordinates, a new instance of Geographical
 * Coordinates of type #maps_coordinates_s with a specified @a latitude and
 * @a longitude.
 * \n New handle is assigned with this instance.
 * @since_tizen 2.4
 * @remarks The @a coordinates must be released using maps_coordinates_destroy().
 * \n The @a coordinates may be cloned using maps_coordinates_clone().
 * \n The @a latitude must be in range of [-90.0, 90.0].
 * \n The @a longitude must be in range of [-180.0, 180.0].
 *
 * @param[in]	latitude	Latitude
 * @param[in]	longitude	Longitude
 * @param[out]	coordinates	The pointer to #maps_coordinates_h in which to
 * store the newly created Geographical Coordinates handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @par Example
 * @code
#include <maps_service.h>

int main (int argc, char *argv[])
{
	int error = MAPS_ERROR_NONE;
	maps_coordinates_h c = NULL;
	maps_coordinates_h cloned = NULL;
	double latitude = .0;
	double longitude = .0;

	do {
		// Creates the Geographical Coordinates
		error = maps_coordinates_create(39.929386, 23.696087, &c);
		if (error != MAPS_ERROR_NON) {
			// Handle the error...
			break;
		}

		// Get latitude and longitude simultaneously
		error = maps_coordinates_get_latitude_longitude(c, &latitude, &longitude);
		if (error != MAPS_ERROR_NON) {
			// Handle the error...
		}

		// Get latitude and longitude one-by-one
		error = maps_coordinates_get_latitude(c, &latitude);
		if (error != MAPS_ERROR_NON) {
			// Handle the error...
		}
		error = maps_coordinates_get_longitude(c, &longitude);
		if (error != MAPS_ERROR_NON) {
			// Handle the error...
		}

		// Clone the Geographical Coordinates
		error = maps_coordinates_clone(c, &cloned);
		if (error != MAPS_ERROR_NON) {
			// Handle the error...
			break;
		}

		// Set latitude and longitude simultaneously
		error = maps_coordinates_set_latitude_longitude(cloned, 40.206056, 23.327389);
		if (error != MAPS_ERROR_NON) {
			// Handle the error...
		}

		// Set latitude and longitude one-by-one
		error = maps_coordinates_set_latitude(cloned, 40.140998);
		if (error != MAPS_ERROR_NON) {
			// Handle the error...
		}
		error = maps_coordinates_set_longitude(cloned, 23.363781);
		if (error != MAPS_ERROR_NON) {
			// Handle the error...
		}

	} while(false);

	// Release the original and cloned Geographical Coordinates handles
	maps_coordinates_destroy(c);
	maps_coordinates_destroy(cloned);

	return error;
}
 * @endcode
*
 * @see maps_coordinates_clone()
 * @see maps_coordinates_destroy()
 * @see maps_coordinates_s
 */
int maps_coordinates_create(const double latitude, const double longitude,
			    maps_coordinates_h *coordinates);

/**
 * @brief	Destroys the Geographical Coordinates and releases all its
 * resources.
 * @details This function destroys the Geographical Coordinates
 * #maps_coordinates_s and releases all its resources.
 * @since_tizen 2.4
 *
 * @param[in]	coordinates	The Geographical Coordinates handle to destroy
 * @return	0 on coordinates, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create_rectangle().
 *
 * @see maps_coordinates_create()
 * @see maps_coordinates_clone()
 */
int maps_coordinates_destroy(maps_coordinates_h coordinates);

/**
 * @brief	Clones the Geographical Coordinates.
 * @details This function makes a clone of the @a origin Geographical
 * Coordinates of type #maps_coordinates_s.
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_coordinates_destroy().
 *
 * @param[in]	origin		The original Geographical Coordinates handle
 * @param[out]	cloned		A pointer to #maps_coordinates_h in which to
 * store the cloned Geographical Coordinates handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is created using maps_coordinates_create().
 *
 * @see maps_coordinates_create()
 * @see maps_coordinates_destroy()
 * @see #maps_coordinates_h
 * @see maps_coordinates_s
 */
int maps_coordinates_clone(const maps_coordinates_h origin,
			   maps_coordinates_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the latitude of the Geographical Coordinates.
 * @details This function gets the @a latitude value of the Geographical
 * Coordinates handle.
 * @since_tizen 2.4
 *
 * @param[in]	coordinates	The Geographical Coordinates handle
 * @param[out]	latitude	The pointer to a double in which to store the
 * latitude of the Geographical Coordinates
 * handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_set_latitude()
 * @see maps_coordinates_get_latitude_longitude()
 * @see maps_coordinates_create()
 */
int maps_coordinates_get_latitude(const maps_coordinates_h coordinates,
				  double *latitude);

/**
 * @brief	Gets the longitude of the Geographical Coordinates.
 * @details This function gets the @a longitude value of the Geographical
 * Coordinates handle.
 * @since_tizen 2.4
 *
 * @param[in]	coordinates	The Geographical Coordinates handle
 * @param[out]	longitude	The pointer to a double in which to store the
 * longitude of the Geographical Coordinates handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_set_longitude()
 * @see maps_coordinates_get_latitude_longitude()
 * @see maps_coordinates_create()
 */
int maps_coordinates_get_longitude(const maps_coordinates_h coordinates,
				   double *longitude);


/**
 * @brief	Gets the latitude and longitude of the Geographical Coordinates.
 * @details This function gets the @a longitude and @a longitude value of the
 * Geographical Coordinates handle.
 * @since_tizen 3.0
 *
 * @param[in]	coordinates	The Geographical Coordinates handle
 * @param[out]	latitude	The pointer to a double in which to store the
 * latitude of the Geographical Coordinates handle
 * @param[out]	longitude	The pointer to a double in which to store the
 * longitude of the Geographical Coordinates handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_set_latitude_longitude()
 * @see maps_coordinates_get_latitude()
 * @see maps_coordinates_set_longitude()
 * @see maps_coordinates_create()
 */
int maps_coordinates_get_latitude_longitude(const maps_coordinates_h coordinates,
				double *latitude,
				double *longitude);


/*----------------------------------------------------------------------------*/

/**
 * @brief	Sets the latitude of the Geographical Coordinates.
 * @details This function sets the @a latitude value of the Geographical
 * Coordinates handle.
 * \n The @a latitude must be in range of [-90.0, 90.0].
 * @since_tizen 2.4
 *
 * @param[in]	coordinates	The Geographical Coordinates handle
 * @param[in]	latitude	The new latitude of the Geographical
 * Coordinates handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_get_latitude()
 * @see maps_coordinates_set_latitude_longitude()
 * @see maps_coordinates_create()
 */
int maps_coordinates_set_latitude(maps_coordinates_h coordinates,
				  const double latitude);

/**
 * @brief	Sets the longitude of the Geographical Coordinates.
 * @details This function sets the @a longitude value of the Geographical
 * Coordinates handle.
 * \n The @a longitude must be in range of [-180.0, 180.0].
 * @since_tizen 2.4
 *
 * @param[in]	coordinates	The Geographical Coordinates handle
 * @param[out]	longitude	The new longitude of the Geographical
 * Coordinates handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_get_longitude()
 * @see maps_coordinates_set_latitude_longitude()
 * @see maps_coordinates_create()
 */
int maps_coordinates_set_longitude(maps_coordinates_h coordinates,
				   const double longitude);

/**
 * @brief	Sets the latitude and longitude of the Geographical Coordinates.
 * @details This function sets the @a latitude and @a latitude value of the
 * Geographical Coordinates handle.
 * \n The @a latitude must be in range of [-90.0, 90.0].
 * \n The @a longitude must be in range of [-180.0, 180.0].
 * @since_tizen 3.0
 *
 * @param[in]	coordinates	The Geographical Coordinates handle
 * @param[in]	latitude	The new latitude of the Geographical
 * Coordinates handle
 * @param[in]	longitude	The new longitude of the Geographical
 * Coordinates handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_get_latitude_longitude()
 * @see maps_coordinates_set_latitude()
 * @see maps_coordinates_set_longitude()
 * @see maps_coordinates_create()
 */
int maps_coordinates_set_latitude_longitude(maps_coordinates_h coordinates,
				const double latitude,
				const double longitude);


/*----------------------------------------------------------------------------*/

/**
 * @brief	Creates a new list of Geographical Coordinates handles.
 * @details This function creates a new instance of Geographical Coordinate List,
 * associate a new handle with it and allocates all needed resources.
 * @since_tizen 3.0
 * @remarks The @a list must be released using maps_coordinates_list_destroy().
 * \n Note: to release Geographical Coordinates handles, comprising the @a list,
 * use maps_coordinates_list_remove_all().
 * \n The @a list may be cloned using maps_coordinates_list_clone().
 *
 * @param[out]	list		The handle of newly created Geographical
 * Coordinates List
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @par Example
 * @code
#include <maps_service.h>

int error = MAPS_ERROR_NONE;
maps_coordinates_list_h list = NULL;
maps_coordinates_h c = NULL;


// Create a Geographical Coordinates List
error = maps_coordinates_list_create(&list);
if (error != MAPS_ERROR_NON) {
	// Handle the create list error...
}

// Append a Geographical Coordinates to the List
error = maps_coordinates_create(39.929386, 23.696087, &c);
if (error != MAPS_ERROR_NON) {
	// Handle the coordinates create error...
}
error = maps_coordinates_list_append(list, c);
if (error != MAPS_ERROR_NON) {
	// Handle the error...
}
maps_coordinates_destroy(c);

// Release the List
maps_coordinates_list_remove_all(list);
maps_coordinates_list_destroy(list);
 * @endcode
 *
 * @see maps_coordinates_list_destroy()
 * @see maps_coordinates_list_remove_all()
 * @see maps_coordinates_list_clone()
 */
int maps_coordinates_list_create(maps_coordinates_list_h *list);

/**
 * @brief	Destroys the Geographical Coordinate List handle.
 * @details This function destroys the Geographical Coordinate List handle.
 * @since_tizen 3.0
 * @remarks Note: to release all Geographical Coordinates handles, comprising
 * the @a list, use maps_coordinates_list_remove_all().
 *
 * @param[in]	list		The handle of Geographical Coordinates List to
 * be destroyed
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_coordinates_list_create().
 *
 * @see maps_coordinates_list_create()
 * @see maps_coordinates_list_clone()
 * @see maps_coordinates_list_append()
 * @see maps_coordinates_list_remove_all()
 */
int maps_coordinates_list_destroy(maps_coordinates_list_h list);

/**
 * @brief	Clones the list of Geographical Coordinates List handle.
 * @details This function clones the Geographical Coordinates List handle
 * @a origin into @a cloned.
 * \n Note: the cloning procedure is shallow, so the Geographical Coordinate
 * handles, comprising the list @a origin, will be simply copied into @a cloned
 * without cloning the memory.
 * \n No new Geographical Coordinates handles will be issued.
 * \n The handle @a origin may be created using maps_coordinates_list_create().
 * @since_tizen 3.0
 * @remarks The @a cloned must be released using maps_coordinates_list_destroy()
 * when no more needed.
 * \n Note: to release all Geographical Coordinates handles, comprising the
 * @a list, use maps_coordinates_list_remove_all().
 *
 * @param[in]	origin		The handle of Geographical Coordinates List to
 * be cloned
 * @param[out]	cloned		The pointer to #maps_coordinates_list_h in which
 * to store the handle of newly cloned list
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is created using maps_coordinates_list_create().
 *
 * @par Example
 * @code
#include <maps_service.h>

int error = MAPS_ERROR_NONE;
maps_coordinates_list_h list = NULL;
maps_coordinates_list_h cloned = NULL;
maps_coordinates_h c = NULL;

// Create Geographical Coordinates List
error = maps_coordinates_list_create(&list);
if (error != MAPS_ERROR_NON) {
	// Handle the error...
}

// Append a Geographical Coordinates to the List
error = maps_coordinates_create(39.929386, 23.696087, &c);
if (error != MAPS_ERROR_NON) {
	// Handle the coordinates create error...
}
error = maps_coordinates_list_append(list, c);
if (error != MAPS_ERROR_NON) {
	// Handle the error...
}
maps_coordinates_destroy(c);

// Clone the List
error = maps_coordinates_list_clone(list, &clone);
if (error != MAPS_ERROR_NON) {
	// Handle the error...
}

// Release Geographical Coordinates handles,
// used in both original and cloned lists
error = maps_coordinates_list_remove_all(list);
if (error != MAPS_ERROR_NON) {
	// Handle the error...
}
// !!! Now the Geographical Coordinates handles are invalid
// in both original and cloned lists.

// Release the original and cloned Geographical Coordinate List handles
maps_coordinates_list_destroy(list);
maps_coordinates_list_destroy(cloned);
 * @endcode
 *
 * @see maps_coordinates_list_create()
 * @see maps_coordinates_list_destroy()
 * @see maps_coordinates_list_remove_all()
 */
int maps_coordinates_list_clone(const maps_coordinates_list_h origin,
				maps_coordinates_list_h *cloned);

/**
 * @brief	Appends Geographical Coordinates to the list.
 * @details This function appends a clone of passed Geographical Coordinates
 * to the @a list.
 * @since_tizen 3.0
 *
 * @param[in]	list		The handle of Geographical Coordinates List
 * @param[in]	coordinates	The Geographical Coordinates handle to add
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_coordinates_list_create().
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @par Example
 * @code
#include <maps_service.h>

static bool foreach_coordinates_cb(int index, int total,
				   maps_coordinates_h coordinates,
				   void *user_data)
{
	// Do something with retrieved Geographical Coordinates...

	// Release the Geographical Coordinates handle
	maps_coordinates_destroy(coordinates);

	return true;
}

int main (int argc, char *argv[])
{
	int error = MAPS_ERROR_NONE;
	maps_coordinates_list_h coordinates = NULL;
	maps_coordinates_h c = NULL;

	// Define the data for Geographical Coordinate List
	double latitudes[] = {11.990638, 11.998026, 11.981234,
		11.969983, 11.954029, 11.956045, 11.946976, 11.941098,
		11.940426, 11.965785, 11.982074};
	double longitudes[] = {121.907848, 121.925873, 121.929478,
		121.926903, 121.937717, 121.945442, 121.947330,
		121.942009, 121.934456, 121.921581, 121.908878};

	// Operate the Geographical Coordinate List
	do {
		// Issue the Geographical Coordinate List
		error = maps_coordinates_list_create(&coordinates);
		if (error != MAPS_ERROR_NON) {
			// Handle the create Geographical Coordinate List error
			break;
		}

		// Append data to Geographical Coordinate List
		for(int i = 0; i < sizeof(latitudes) / sizeof(latitudes[0])) {
			error = maps_coordinates_create(latitudes[i],
							longitudes[i],
							&c);
			if (error != MAPS_ERROR_NON) {
				// Handle create Geographical Coordinates error
				break;
			}

			maps_coordinates_list_append(coordinates, c);
			if (error != MAPS_ERROR_NON) {
				// Handle the append to Coordinate List error
				break;
			}

			maps_coordinates_destroy(c);
		}

		// Iterate through the List of Geographical Coordinates
		error = maps_coordinates_list_foreach_coordinates(coordinates,
						      foreach_coordinates_cb,
						      NULL);
		if (error != MAPS_ERROR_NON) {
			// Handle the iterate Geographical Coordinate List error
			break;
		}

		// Do something else...

		// Remove all Geographical Coordinates from the List
		error = maps_coordinates_list_remove_all(coordinates);

	} while(false);

	// Release the List when no more needed
	maps_coordinates_list_destroy(coordinates);

	return error;
}
 * @endcode
 *
 * @see maps_coordinates_list_foreach_coordinates()
 * @see maps_coordinates_list_remove()
 * @see maps_coordinates_list_remove_all()
 * @see #maps_coordinates_h
 * @see maps_coordinates_list_create()
 * @see maps_coordinates_list_destroy()
 * @see maps_coordinates_list_get_length()
 */
int maps_coordinates_list_append(maps_coordinates_list_h list,
				 const maps_coordinates_h coordinates);

/**
 * @brief	Gets the number of elements in an coordinates list.
 * @details This function gets the length of the Geographical Coordinates List.
 * @since_tizen 3.0
 *
 * @param[in]	list		The handle of Geographical Coordinates List
 * @param[out]	length		The pointer to an integer in which to store the
 * number of elements in the Geographical Coordinates List
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_NOT_SUPPORTED Not supported
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_coordinates_list_create().
 *
 * @par Example
 * @code
#include <maps_service.h>

// maps_coordinates_list_h list is created ahead

int error = MAPS_ERROR_NONE;
int length = 0;

// Get the length of the List of Geographical Coordinates
error = maps_coordinates_list_get_length(list, &length);
if (error != MAPS_ERROR_NON) {
	// Handle the error...
}
 * @endcode
 *
 * @see maps_coordinates_list_create()
 * @see maps_coordinates_list_append()
 * @see maps_coordinates_list_destroy()
 */
int maps_coordinates_list_get_length(maps_coordinates_list_h list, int *length);


/* TODO: discuss also
int maps_coordinates_list_append_latlon(maps_coordinates_list_h list,
					const double latitude,
					const double longitude,
					maps_coordinates_h *coordinates);
					*/


/**
 * @brief	Gets the Geographical Coordinates, comprising the List.
 * @details This function delivers Geographical Coordinates, comprising the
 * specified Geographical Coordinate List, via
 * maps_coordinates_list_foreach_cb() callback.
 * @since_tizen 3.0
 *
 * @param[in]	list		The handle of Geographical Coordinates List
 * @param[in]	callback	The callback to be invoked for delivering each
 * Geographical Coordinates handle, comprising the List
 * @param[in]	user_data	The user data pointer to be passed to the
 * callback function
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_NOT_FOUND Result not found
 *
 * @pre @a list is created using maps_coordinates_list_create().
 * @post This function invokes maps_coordinates_list_foreach_cb() to deliver
 * Geographical Coordinates handles, comprising the List.
 *
 * @par Example
 * @code
#include <maps_service.h>

// maps_coordinates_list_h list is created ahead

static bool foreach_coordinates_cb(int index, int total,
				   maps_coordinates_h coordinates,
				   void *user_data)
{
	int error = MAPS_ERROR_NONE;
	double latitude = .0;
	double longitude = .0;

	if (!coordinates)
		return false; // Error

	// Process retrieved Geographical Coordinates handle
	error = maps_coordinates_get_latitude_longitude(coordinates, &latitude, &longitude);
	if (error != MAPS_ERROR_NON) {
		// Handle the error...
	}

	// Release the Geographical Coordinates handle
	maps_coordinates_destroy(coordinates);

	return true;
}

static void itertare_coordinates()
{
	int error = MAPS_ERROR_NONE;

	// Iterate through the List of Geographical Coordinates
	error = maps_coordinates_list_foreach_coordinates(list,
					      foreach_coordinates_cb,
					      NULL);
	if (error != MAPS_ERROR_NON) {
		// Handle the error...
	}
}
 * @endcode
 *
 * @see maps_coordinates_list_foreach_cb()
 * @see #maps_coordinates_h
 * @see maps_coordinates_list_append()
 * @see maps_coordinates_list_create()
 * @see maps_coordinates_list_get_length()
 */
int maps_coordinates_list_foreach_coordinates(maps_coordinates_list_h list,
				  maps_coordinates_list_foreach_cb callback,
				  void *user_data);

/**
 * @brief	Removes Geographical Coordinates handle from the List.
 * @details This function removes the given Geographical Coordinates handle from
 * the @a list.
 * \n If two @a list items contain the same data, only the first is removed.
 * \n If none of the @a list items contain the data, the @a list remains
 * unchanged.
 * @since_tizen 3.0
 * @remarks The @a coordinates must be released using maps_coordinates_destroy()
 *
 * @param[in]	list		The handle of Geographical Coordinates List
 * @param[in]	coordinates	The Geographical Coordinates handle to remove
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_coordinates_list_create().
 *
 * @par Example
 * @code
#include <maps_service.h>

// maps_coordinates_list_h list is created ahead
// maps_coordinates_h c is created ahead and added to the list

int error = MAPS_ERROR_NONE;

// Remove Geographical Coordinates handle from the List
error = maps_coordinates_list_remove(list, c);
if (error != MAPS_ERROR_NON) {
	// Handle the error...
}

// Release the Geographical Coordinates handle
maps_coordinates_destroy(c);
 * @endcode
 *
 * @see maps_coordinates_list_create()
 * @see maps_coordinates_list_remove_all()
 * @see maps_coordinates_list_get_length()
 */
int maps_coordinates_list_remove(maps_coordinates_list_h list,
				 maps_coordinates_h coordinates);

/**
 * @brief	Removes and destroys all Geographical Coordinates from the List.
 * @details This function removes all Geographical Coordinates handles from the
 * @a list and releases resources, assigned with them.
 * @since_tizen 3.0
 *
 * @param[in]	list		The handle of Geographical Coordinates List
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a list is created using maps_coordinates_list_create().
 *
 * @see maps_coordinates_list_remove()
 * @see maps_coordinates_list_append()
 * @see maps_coordinates_list_create()
 * @see maps_coordinates_list_destroy()
 * @see maps_coordinates_list_get_length()
 */
int maps_coordinates_list_remove_all(maps_coordinates_list_h list);


#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_COORDINATES_H__ */
