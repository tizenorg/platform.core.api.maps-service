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

#ifndef __MAPS_SERVICE_DOC_H__
#define __MAPS_SERVICE_DOC_H__

/**
 * @ingroup	CAPI_LOCATION_FRAMEWORK
 * @defgroup	CAPI_MAPS_SERVICE_MODULE Maps Service
 *
 * @brief	The Maps Service provides an Application Developer with Maps
 * Service APIs
 *
 * @section	CAPI_MAPS_SERVICE_MODULE_HEADER Required Header
 * \#include <maps_service.h>
 *
 * @section	CAPI_MAPS_SERVICE_MODULE_OVERVIEW Overview
 *
 * Maps API provides a developer with a set of functions, helping to create Maps
 * aware applications\n
 * Maps API comprises following features:
 *  - Geocoding and reverse geocoding
 *  - Discovering Place information
 *  - Calculating Route
 *  - View
 *  .
 *
 * Maps API allows a Developer to choose one of Map Providers which are being
 * included as plugins
 *
 *
 *
 * @section CAPI_MAPS_SERVICE_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 * - http://tizen.org/feature/maps\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tools/native-tools/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 *
 * Geocoding
 * - The Maps Geocoding API allows translating an address to its geographical
 * location defined in terms of latitude and longitude; the input can be a
 * qualified and structured address or a free-formed search text with full or
 * partial address information.
 * - The example below shows a structured address:
 *   - housenumber=117,
 *   - street=Invaliden street
 *   - city=Berlin,
 *   - postalcode=10115,
 *   - country=Germany,
 *   - state=Berlin
 *   .
 * .
 *
 * Reverse Geocoding
 * - The Maps Reverse Geocoding API allows to inverse translating a geographical
 * location (longitude, latitude) to an address; it can be used to answer the
 * question "Where am I?".
 * .
 *
 * Route
 * - The Maps Route API provides ways to calculate a route that defines a path
 * between a start and a destination and may, optionally, pass through specific
 * intermediate locations.
 * - Route Preferences:
 *  - Travel Mode (car, pedestrian, public transit, truck),
 *  - Optimization (fastest, shortest),
 *  - Avoid/Prefer Preferences (toll road, motorway, ferry, public transit,
 *  tunnel, dirt road, parks, car-pool lane).
 * - Route Calculations
 *  - Way points and preferences as input values,
 *  - Route geometry and maneuver (instructions for guidance) as result values:
 *   - Geometry consists of points that visually represent the determined route,
 *   - Maneuver contains turn-by-turn instruction and position.
 *   .
 *  .
 * .
 * Places
 * - The Maps Place API allows you to find place information
 * - Place search
 *  - Depending on the location context, the number of relevant places might
 *  be large. Therefore this query may not only return places, but also
 *  suggestions for additional filter criteria that allow users to interactively
 *  refine the classes of places they are interested in.
 *
 *  - Nearby Search: search for places within a specified area.
 *    You can refine your search request by supplying keywords, Name of Points
 *    of Interests, or Proximity
 *
 *  - Category Search: search for popular places for the given location context
 *  and matching the category filter criteria.
 *    You can refine your search request by specifying the categories of place
 *    you are searching for.
 *
 * - Result item type of searching
 *  - id, name, location, distance, URI, rating, category.
 *
 * - Place information allows to fetch details about a place. The following
 * place content is supported:
 *  - Base Attribute includes name, location, categories, contacts, id, ratings,
 *  icon path, image content, review content, and editorial content.
 *  - Extended Attribute refers to opening hours, payment methods, annual
 *  closings, disabled access.
 *  .
 * .
 * View
 * - The Maps View API provides a developer with a set of functions, bringing
 * basic interactive visual user interface in maps applications.
 *
 * - View widget : Drawing a map image on the map port, the specified rectangular
 * area of the maps application GUI.
 *
 * - Zoom and rotation : Changing zoom and orientation of the view in response
 * on user gestures, such as scrolling, tapping, zooming, rotating, etc.
 *
 * - Conversion of screen coordinates to geographical and vise versa.
 *
 * - User's gesture support.
 *  - Receive the event of the user gesture.
 *  - Enable or disable the specified gesture.
 *  - Re-assign the action, which should be taken in response on the user's gesture.
 *
 * - Various Properties.
 *  - Visibility and size on the screen.
 *  - Theme : Day, satellite or terrain.
 *  - Language : English, Russian, Chinese, Italian, German, Spanish, etc.
 *  .
 * .
 * The developer is allowed to choose one of map providers by manipulating map
 * plug-ins.
 *
 */

#endif /*__MAPS_SERVICE_DOC_H__ */
