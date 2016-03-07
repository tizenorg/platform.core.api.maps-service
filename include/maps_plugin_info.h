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

#ifndef __MAPS_SERVICE_PLUGIN_INFO_H__
#define __MAPS_SERVICE_PLUGIN_INFO_H__

/**
 *
 * @ingroup	CAPI_MAPS_PLUGIN_AND_PROVIDERS_MODULE
 * @defgroup	CAPI_MAPS_PLUGIN_INFO_MODULE Plugin info
 *
 * @file maps_plugin_info.h
 * @brief This file contains the Maps Plugin Info interface
 *
 * @addtogroup CAPI_MAPS_PLUGIN_INFO_MODULE
 * @{
 * @brief This provides APIs related to Maps Plugins Info data structure
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	The Maps Plugin Info handle
 * @details The handle of Maps Plugin Info instance.
 * @remarks To create the handle use maps_plugin_info_create().
 * \n To release the handle use maps_plugin_info_destroy().
 * \n To clone the handle use maps_plugin_info_clone().
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @see maps_plugin_info_create()
 * @see maps_plugin_info_destroy()
 * @see maps_plugin_info_clone()
 */
typedef void *maps_plugin_info_h;

/*----------------------------------------------------------------------------*/

/**
 * @brief	Creates a new plugin info handle.
 * @details This function creates a new plugin info handle and allocates all
 * needed resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a info must be released using maps_plugin_info_destroy().
 * \n @a info may be cloned using maps_plugin_info_clone().
 *
 * @param[out]	info	A handle of a new plugin info on success
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see maps_plugin_info_destroy()
 * @see maps_plugin_info_clone()
 */
int maps_plugin_info_create(maps_plugin_info_h *info);

/**
 * @brief	Destroys the plugin info handle and releases all its resources.
 * @details This function destroys the plugin info handle and releases all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	info	The plugin info handle to destroy
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a info is created using maps_plugin_info_create().
 *
 * @see maps_plugin_info_create()
 * @see maps_plugin_info_clone()
 */
int maps_plugin_info_destroy(maps_plugin_info_h info);

/**
 * @brief	Clones the plugin info handle.
 * @details This function clones the plugin info handle @a origin and all its
 * resources.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a cloned must be released using maps_plugin_info_destroy().
 *
 * @param[in]	origin		The original plugin info handle
 * @param[out]	cloned		A cloned plugin info handle
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_OUT_OF_MEMORY Out of memory
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a info is created using maps_plugin_info_create().
 *
 * @see maps_plugin_info_create()
 * @see maps_plugin_info_destroy()
 */
int maps_plugin_info_clone(const maps_plugin_info_h origin,
			   maps_plugin_info_h *cloned);

/*----------------------------------------------------------------------------*/

/**
 * @brief	Gets the plugin provider name.
 * @details This function gets the plugin provider name.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @remarks @a provider_name must be released using free().
 *
 * @param[in]	info		The handle of plugin info
 * @param[out]	provider_name	The plugin info name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a info is created using maps_plugin_info_create().
 *
 * @see maps_plugin_info_create()
 */
int maps_plugin_info_get_provider_name(const maps_plugin_info_h info,
				       char **provider_name);

/**
 * @brief	Sets the plugin provider name.
 * @details This function sets the plugin provider name.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 *
 * @param[in]	info		The handle of plugin info
 * @param[in]	provider_name	The plugin info name
 * @return	0 on success, otherwise a negative error value
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre @a info is created using maps_plugin_info_create().
 *
 * @see maps_plugin_info_create()
 */
int maps_plugin_info_set_provider_name(maps_plugin_info_h info,
				       const char *provider_name);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif /*__MAPS_SERVICE_PLUGIN_INFO_H__ */
