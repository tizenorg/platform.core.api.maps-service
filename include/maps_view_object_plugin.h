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

#ifndef __MAPS_VIEW_OBJECT_PLUGIN_H__
#define __MAPS_VIEW_OBJECT_PLUGIN_H__

#include <maps_view_object.h>

/**
 * @ingroup	CAPI_MAP_VIEW_MODULE
 * @defgroup	CAPI_MAP_OBJECTS_MODULE Maps View Objects
 *
 * @file map_object.h
 * @brief This file contains the functions of Map Object API
 *
 * @addtogroup CAPI_MAP_OBJECTS_MODULE
 * @{
 * @brief This file contains the functions related to Map View Objects
 * @details Supported Object types are following:
 *  * Markers
 *  * Polygons
 *  * Polylines
 *  * Group Objects
 *  .
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Enumerations of operation types over the map view object
 * @since_tizen 3.0
 */
typedef enum _maps_view_object_operation_e {

	/** Indicates the add object operation */
	MAPS_VIEW_OBJECT_ADD,

	/** Indicates the change object visibility operation */
	MAPS_VIEW_OBJECT_SET_VISIBLE,

	/** Indicates the move object operation */
	MAPS_VIEW_OBJECT_MOVE,

	/** Indicates the operation of changing object specific properties */
	MAPS_VIEW_OBJECT_CHANGE,

	/** Indicates the remove object operation */
	MAPS_VIEW_OBJECT_REMOVE,
} maps_view_object_operation_e;

int maps_view_object_marker_set_size(const maps_view_object_h marker, int width, int height);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif	/* __MAPS_VIEW_OBJECT_PLUGIN_H__ */
