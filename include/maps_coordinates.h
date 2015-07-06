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
 * @brief	Structure of the Geographical Coordinates.
 * @details This structure represents a Geographical Coordinates, specified with
 * a @a latitude and @a longitude values.
 * \n The @a latitude must be in range of [-90.0, 90.0].
 * \n The @a longitude must be in range of [-180.0, 180.0].
 * @since_tizen 2.4
 * @remarks #maps_coordinates_h is a void pointer to the #maps_coordinates_s.
 *
 * @see maps_coordinates_h
 * @see maps_coordinates_create
 * @see maps_coordinates_clone
 * @see maps_coordinates_destroy
 */
typedef struct _maps_coordinates_s {
	double latitude;	/**< The latitude [-90.0 ~ 90.0] (degrees) */
	double longitude;	/**< The longitude [-180.0 ~ 180.0] (degrees) */
} maps_coordinates_s;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Creates a new instance of Geographical Coordinates and initiates
 * a handle with it.
 * @details This function creates @a coordinates, a new instance of Geographical
 * Coordinates of type #maps_coordinates_s
 * with a specified @a latitude and @a longitude.
 * \n New handle is assigned with this instance.
 * @since_tizen 2.4
 * @remarks @a coordinates must be released using maps_coordinates_destroy().
 * \n @a coordinates may be cloned using maps_coordinates_clone().
 *
 * @param[in]	latitude	Latitude
 * @param[in]	longitude	Longitude
 * @param[out]	coordinates	The coordinate handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
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
 * @param[in]	coordinates	The coordinate handle to destroy
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
 * @param[in]	origin		The original coordinate handle
 * @param[out]	cloned		A cloned coordinate handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is created using maps_coordinates_create().
 *
 * @see maps_coordinates_create()
 * @see maps_coordinates_destroy()
 * @see maps_coordinates_s
 */
int maps_coordinates_clone(const maps_coordinates_h origin,
			   maps_coordinates_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the latitude of the coordinates.
 * @details This function gets the @a latitude value of the coordinates handle.
 * @since_tizen 2.4
 *
 * @param[in]	coordinates	The coordinate handle
 * @param[out]	latitude	The latitude of the coordinate handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_create()
 * @see maps_coordinates_set_latitude()
 */
int maps_coordinates_get_latitude(const maps_coordinates_h coordinates,
				  double *latitude);

/**
 * @brief	Gets the longitude of the coordinates.
 * @details This function gets the @a longitude value of the coordinates handle.
 * @since_tizen 2.4
 *
 * @param[in]	coordinates	The coordinate handle
 * @param[out]	longitude	The longitude of the coordinate handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_create()
 * @see maps_coordinates_set_longitude()
 */
int maps_coordinates_get_longitude(const maps_coordinates_h coordinates,
				   double *longitude);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Sets the latitude of the coordinates.
 * @details This function sets the @a latitude value of the coordinates handle.
 * @since_tizen 2.4
 *
 * @param[in]	coordinates	The coordinate handle
 * @param[in]	latitude	The latitude of the coordinate handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_create()
 * @see maps_coordinates_get_latitude()
 */
int maps_coordinates_set_latitude(maps_coordinates_h coordinates,
				  const double latitude);

/**
 * @brief	Sets the longitude of the coordinates.
 * @details This function sets the @a longitude value of the coordinates handle.
 * @since_tizen 2.4
 *
 * @param[in]	coordinates	The coordinate handle
 * @param[out]	longitude	The longitude of the coordinate handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a coordinates are created using maps_coordinates_create().
 *
 * @see maps_coordinates_create()
 * @see maps_coordinates_get_longitude()
 */
int maps_coordinates_set_longitude(maps_coordinates_h coordinates,
				   const double longitude);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_COORDINATES_H__ */
