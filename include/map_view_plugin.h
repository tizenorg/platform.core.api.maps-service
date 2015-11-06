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

#ifndef __MAP_VIEW_PLUGIN_H__
#define __MAP_VIEW_PLUGIN_H__

#include <map_view.h>


/**
 *
 * @ingroup	CAPI_MAP_VIEW_PLUGIN_MODULE
 * @defgroup	CAPI_MAP_VIEW_CONTROL_MODULE Map View
 *
 * @file map_view_plugin.h
 * @brief This file contains API for map_view plugins
 *
 * @addtogroup CAPI_MAP_VIEW_CONTROL_MODULE
 * @{
 * @brief This provides map view APIs related to operations with map view
 * @details Supported map view API features include:
 *  * Creating and destroying the map view port, moving and resizing it
 *  * Changing map zoom, and orientation.
 */


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief       Zooms the map on the view.
 * @details This function zooms the map on the Map View with real value of zoom
 * factor.
 * @since_tizen 3.0
 * @privlevel public
 *
 * @param[in]   view            The view handle
 * @param[out]  zoom_factor     The new zoom factor
 * @return      0 on success, otherwise a negative error value
 * @retval      #MAPS_ERROR_NONE Successful
 * @retval      #MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using map_view_create().
 * @post This function invokes map_view_on_event_cb() with the event type
 * #MAP_EVENT_ACTION and action type #MAP_ACTION_ZOOM.
 * \n The value of new zoom factor will be accessible via
 * map_event_data_get_scale_factor().
 *
 * @par Example
 * @code
#include <map_view.h>

// map_view_h view is created ahead

int error = MAPS_ERROR_NONE;

// Set a specified zoom factor for the Map View
error = map_view_set_scale_factor(view, 8.5);
if(error != MAPS_ERROR_NONE) {
        // Handle the error
}
 * @endcode
 *
 * @see map_view_get_scale_factor()
 * @see map_view_create()
*/
int map_view_set_scale_factor(const map_view_h view, const double scale_factor);


/**
 * @brief       Gets the map zoom factor.
 * @details This function gets the current zoom factor of the Map on the Map
 * View.
 * @since_tizen 3.0
 * @remarks zoom @a factor can be obtained also in map_view_on_event_cb()
 * callback, assigned using map_view_set_event_callback() with the event
 * type #MAP_EVENT_ACTION specified.
 *
 * @param[in]   view            The view handle
 * @param[out]  zoom_factor     The pointer to a double in which to store the
 * current zoom factor
 * @return      0 on success, otherwise a negative error value
 * @retval      #MAPS_ERROR_NONE Successful
 * @retval      #MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using map_view_create().
 *
 * @see map_view_set_scale_factor()
 * @see map_view_create()
 */
int map_view_get_scale_factor(const map_view_h view, double *scale_factor);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif				/* __MAP_VIEW_PLUGIN_H__*/
