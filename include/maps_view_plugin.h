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

#ifndef __MAPS_VIEW_PLUGIN_H__
#define __MAPS_VIEW_PLUGIN_H__

#include <maps_view.h>

/**
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_VIEW_MODULE Maps View
 *
 * @file map_object.h
 * @brief This file contains the functions of Map View API
 *
 * @addtogroup CAPI_MAP_OBJECTS_MODULE
 * @{
 * @brief This file contains the functions related to Map View
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Zooms the map on the view.
 * @details This function zooms the map on the View with real value of zoom
 * factor.
 * @since_tizen 3.0
 * @privlevel public
 *
 * @param[in]	view		The view handle
 * @param[out]	zoom_factor	The new zoom factor
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval	#MAPS_ERROR_PERMISSION_DENIED Permission Denied
 *
 * @pre @a view is created using maps_view_create().
 * @post This function invokes maps_view_on_event_cb() with the event type
 * #MAPS_VIEW_EVENT_ACTION and action type #MAPS_VIEW_ACTION_ZOOM.
 * \n The value of new zoom factor will be accessible via
 * maps_view_event_data_get_zoom_factor().
 *
 * @see maps_view_get_zoom_factor()
 * @see maps_view_set_zoom_level()
 * @see maps_view_get_min_zoom_level()
 * @see maps_view_get_max_zoom_level()
 * @see maps_view_on_event_cb()
 * @see maps_view_set_event_cb()
 * @see maps_view_create()
*/
int maps_view_set_zoom_factor(maps_view_h view, double zoom_factor);

/**
 * @brief	Gets the zoom factor.
 * @details This function gets the current zoom factor of View.
 * @since_tizen 3.0
 * @remarks zoom @a factor can be obtained also in maps_view_on_event_cb()
 * callback, assigned using map_view_set_event_cb() with the event
 * type #MAPS_VIEW_EVENT_ACTION specified.
 *
 * @param[in]	view		The view handle
 * @param[out]	zoom_factor	The pointer to a double in which to store the
 * current zoom factor
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a view is created using maps_view_create().
 *
 * @see maps_view_set_zoom_factor()
 * @see maps_view_set_event_cb()
 * @see maps_view_create()
 */
int maps_view_get_zoom_factor(const maps_view_h view, double *zoom_factor);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif	/* __MAPS_VIEW_PLUGIN_H__ */

