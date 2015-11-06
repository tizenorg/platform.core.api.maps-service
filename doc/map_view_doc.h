/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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
 *
 */

#ifndef __MAP_VIEW_DOC_H__
#define __MAP_VIEW_DOC_H__
/**
 * @ingroup	CAPI_LOCATION_FRAMEWORK
 * @defgroup	CAPI_MAP_VIEW_MODULE Mapping
 *
 * @brief	The Mapping API provides a Map View with basic interactive
 * graphical user interface.
 *
 * @section	CAPI_MAP_VIEW_MODULE_HEADER Required Header
 * \#include <map_view.h>

 * @section	CAPI_MAP_VIEW_MODULE_OVERVIEW Overview
 *
 * The Mapping API provides a developer with a set of functions, bringing basic
 * interactive visual user interface in maps applications\n
 * Mapping API comprises following features:
 *  - Map View widget
 *  - Zoom and rotation
 *  - Conversion of screen coordinates to geographical and vise versa
 *  - Map properties (size on the screen, satellite or terrain theme, language etc.)
 *
 *  When created, Map View is linked with Maps Service API, so the developer can
 *  choose one of Map Providers, which will perform graphical rendering of map
 *  panel.
 *  @see map_view_create()
 *  @see maps_service_create()
 *
 *
 * @section	CAPI_MAP_VIEW_MODULE_FEATURE Features
 *
 * Map View
 *  - The Map View widget allows drawing a map image on the map panel.
 *  - It interactively and smoothly changes the zoom and orientation of the map
 *  in response on user gestures, such as panning, tapping, double tapping etc.
 *  - The Map View provides the interface to the application, which allows
 *  centering the map on a specified geographical coordinate, zooming on a
 *  selected zoom level and rotating in a range of 0..360 degree.
 *  - The Map View API includes a service, allowing to convert geographical
 *  coordinates to screen coordinates and vise versa.
 *  - The set of Map View customizable properties includes:
 *    - visibility and size on the screen
 *    - theme: day, satellite or terrain
 *    - language: English, Russian, Chinese, Italian, German, Spanish, etc.
 *    .
 *  .
 *
 * @if WEARABLE
 * @section	CAPI_MAP_VIEW_MODULE_FEATURE Related Features
 * This API is related with the following features:
 *  - http://tizen.org/feature/network.internet
 *
 * It is recommended to design feature related codes in your application for
 * reliability.\n
 *
 * You can check if a device supports the related features for this API by using
 * @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of
 * your application.\n
 *
 * To ensure your application is only running on the device with specific
 * features, please define the features in your manifest file using the manifest
 * editor in the SDK.\n
 * @endif
 *
 * More details on featuring your application can be found from
 * <a href="https://developer.tizen.org/development/tools/native-tools/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 */

#endif /*__MAP_VIEW_DOC_H__ */

