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

#ifndef __MAPS_GEOAREA_H__
#define __MAPS_GEOAREA_H__

#include <maps_coordinates.h>

/**
 * @ingroup	CAPI_MAPS_SERVICE_MODULE
 * @defgroup	CAPI_MAPS_GEOAREA_MODULE Area
 *
 * @file maps_area.h
 * @brief This file contains the definitions, structures, and functions related
 * to area information.
 *
 * @addtogroup CAPI_MAPS_GEOAREA_MODULE
 * @{
 * @brief This provides APIs related to geographical area.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Handle of the Geographical Area.
 * @details The Geographical Area handle can be obtained via call of
 * maps_area_create_rectangle() or maps_area_create_circle().
 * @since_tizen 2.4
 * \n To release the handle use maps_area_destroy().
 * \n To clone the handle use maps_area_clone().
 */
typedef void *maps_area_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Creates a rectangular type of new Geographical Area with a
 * specified information.
 * @details This function creates a rectangular type of new #maps_area_h with a
 * specified left top and right bottom coordinates.
 * @since_tizen 2.4
 * @remarks @a area must be released using maps_area_destroy().
 * \n @a area may be cloned using maps_area_clone().
 * \n @a top_left and @a bottom_right must be released using
 * maps_coordinates_destroy().
 *
 * @param[in]	top_left	The left top position
 * @param[in]	bottom_right	The right bottom position
 * @param[out]	area		The area handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a top_left and @a bottom_right are created using
 * maps_coordinates_create().
 *
 * @see maps_area_clone()
 * @see maps_area_destroy()
 * @see maps_area_create_circle()
 * @see maps_coordinates_create()
 * @see maps_coordinates_destroy()
 */
int maps_area_create_rectangle(const maps_coordinates_h top_left,
			       const maps_coordinates_h bottom_right,
			       maps_area_h *area);

/**
 * @brief	Creates a circular type of new Geographical Area with a
 * specified information.
 * @details This function creates a circular type of new #maps_area_h
 * Geographical Area with a specified center coordinates and a radius.
 * @since_tizen 2.4
 * @remarks @a area must be released using maps_area_destroy().
 * \n @a top_left and @a bottom_right must be released using
 * maps_coordinates_destroy().
 *
 * @param[in]	center		The central position of the area
 * @param[in]	radius		The radius of the area
 * @param[out]	area		The area handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a center is created using maps_coordinates_create().
 *
 * @see maps_area_clone()
 * @see maps_area_destroy()
 * @see maps_area_create_rectangle()
 * @see maps_coordinates_create()
 * @see maps_coordinates_destroy()
 */
int maps_area_create_circle(const maps_coordinates_h center,
			    const double radius, maps_area_h *area);

/**
 * @brief	Destroys the Geographical Area and releases all its resources.
 * @details This function destroys the Geographical Area #maps_area_h and
 * releases all its resources.
 * @since_tizen 2.4
 *
 * @param[in]	area		The area #maps_area_h
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a area can be created using maps_area_create_rectangle() or
 * maps_area_create_circle().
 *
 * @see maps_area_create_rectangle()
 * @see maps_area_create_circle()
 */
int maps_area_destroy(maps_area_h area);

/**
 * @brief	Clones the Geographical Area.
 * @details This function makes a clone of the @a origin Geographical Area of
 * type #maps_area_h.
 * @since_tizen 2.4
 * @remarks @a cloned must be released using maps_area_destroy().
 *
 * @param[in]	origin		The area #maps_area_h to be copied
 * @param[out]	cloned		The cloned area #maps_area_h handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a origin is created using maps_area_create_rectangle() or
 * maps_area_create_circle().
 *
 * @see maps_area_create_rectangle()
 * @see maps_area_create_circle()
 * @see maps_area_destroy()
 */
int maps_area_clone(const maps_area_h origin, maps_area_h *cloned);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAPS_GOEAREA_H__ */
