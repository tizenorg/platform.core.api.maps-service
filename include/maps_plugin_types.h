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

#ifndef __MAPS_PLUGIN_TYPES_H__
#define __MAPS_PLUGIN_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>

/**
 * @brief	Structure of the coordinates list.
 * @since_tizen 2.4
 * @remarks #maps_coordinates_list_h is a void pointer to the #maps_coordinates_list_s.
 */
typedef GList maps_coordinates_list_s;

/**
 * @brief	Structure of the address list.
 * @since_tizen 2.4
 * @remarks #maps_address_list_h is a void pointer to the #maps_address_list_s.
 */
typedef GList maps_address_list_s;

/**
 * @brief	Enumeration of supported types of the Geographical Area.
 * @details This enumeration represents allowed geographical type of
 * Geographical Area: rectangular and circular.
 * @since_tizen 2.4
 */
typedef enum {
	MAPS_AREA_NONE = 0, /**< Undefined geographical area type. */
	MAPS_AREA_RECTANGLE, /**< Rectangular geographical area type. */
	MAPS_AREA_CIRCLE, /**< Circle geographical area type. */
} maps_area_type_e;


/**
 * @brief	Structure of the Geographical Coordinates.
 * @details This structure represents a Geographical Coordinates, specified with
 * a @a latitude and @a longitude values.
 * \n The @a latitude must be in range of [-90.0, 90.0].
 * \n The @a longitude must be in range of [-180.0, 180.0].
 * @since_tizen 2.4
 * @remarks #maps_coordinates_h is a void pointer to the #maps_coordinates_s.
 */
typedef struct _maps_coordinates_s {
	double latitude;	/**< The latitude [-90.0 ~ 90.0] (degrees) */
	double longitude;	/**< The longitude [-180.0 ~ 180.0] (degrees) */
} maps_coordinates_s;


/**
 * @brief	Structure of the rectangular Geographical Area.
 * @details This structure represents a rectangular Geographical Area,
 * specified with left top and right bottom coordinates.
 * \n This structure is used in #maps_area_s.
 * @since_tizen 2.4
 *
 * @see maps_area_s
 */
typedef struct _maps_area_rectangle_s {
	maps_coordinates_s top_left;		/**< The top left position
						  of rectangle. */
	maps_coordinates_s bottom_right;	/**< The bottom right position
						  of rectangle. */
} maps_area_rectangle_s;

/**
 * @brief	Structure of the circular Geographical Area, specified with a
 * center coordinates and a radius.
 * @details This structure represents a circular Geographical Area.
 * \n This structure is used in #maps_area_s.
 * @since_tizen 2.4
 *
 * @see maps_area_s
 */
typedef struct _maps_area_circle_s {

	maps_coordinates_s center;	/**< The center position of a circle. */
	double radius;			/**< The radius of a circle. */
} maps_area_circle_s;

/**
 * @brief	Structure of the Geographical Area.
 * @details This structure represents a Geographical Area, specified with a
 * type, circular or rectangular, and appropriate coordinates and radius.
 * @since_tizen 2.4
 *
 * @see maps_area_type_e
 * @see maps_area_rectangle_s
 * @see maps_area_circle_s
 */
typedef struct _maps_area_s {
	maps_area_type_e type;	/**< The area type of this information. */
	union {
		maps_area_rectangle_s rect;	/**< The geographical
					information of a rectangle. */
		maps_area_circle_s circle;	/**< The geographical
					information of a circle. */
	};
} maps_area_s;


#ifdef __cplusplus
}
#endif

#endif				/* __MAPS_PLUGIN_TYPES_H__ */


