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
 * @brief	The Mapping API provides a map view with basic interactive
 * graphical user interface.
 *
 * @section	CAPI_MAP_VIEW_MODULE_HEADER Required Header
 * \#include <map_view.h>

 * @section	CAPI_MAP_VIEW_MODULE_OVERVIEW Overview
 *
 * The Mapping API provides a developer with a set of functions, bringing basic
 * interactive visual user interface in maps applications\n
 * Mapping API comprises following features:
 *  - Map view widget
 *  - Zoom and rotation
 *  - Conversion of screen coordinates to geographical and vise versa
 *  - Map properties (size on the screen, satellite or terrain theme, language
 *  etc.)
 *  - User's gesture support
 *  .
 *
 *  When created, the instance of map view is linked with maps service API, so
 *  the developer can choose one of Map Providers, which will perform graphical
 *  rendering of the map.
 *
 * Map view provides following general features:
 *  - The map view allows drawing a map image on the map port, the specified
 *  rectangular area of the maps application GUI
 *  - It interactively and smoothly changes zoom and orientation of the map
 *  in response on user gestures, such as scrolling, tapping, zooming,
 *  rotating, etc
 *  - The map view provides interface, which allows the application
 *  centering the map on specified geographical coordinates, zooming in and out
 *  on a selected integer zoom level and rotating in a range of 0..360 degree
 *  - The map view API includes services, allowing to convert geographical
 *  coordinates into screen coordinates and vise versa
 *  - Snapshot feature allows to capture current state of image rendered on the
 *  map view
 *  - To handle the user's gestures, map view provides API allowing the
 *  application to
 *    - receive the event of the user gesture,
 *    - enable or disable the specified gesture or
 *    - re-assign the map view action, which should be taken in response on the
 *    user's gesture
 *  - The set of map view customizable properties includes:
 *    - visibility and size on the screen
 *    - theme: day, satellite or terrain
 *    - language: English, Russian, Chinese, Italian, German, Spanish, etc
 *    .
 *  .
 *
 *  @see #map_view_h
 *  @see #map_gesture_h
 *
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
 *
 * More details on featuring your application can be found from
 * <a href="https://developer.tizen.org/development/tools/native-tools/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 * @endif
 */

#endif /*__MAP_VIEW_DOC_H__ */

