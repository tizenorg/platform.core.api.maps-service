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

#ifndef __MAPS_SERVICE_TEST_COORDINATES_H__
#define __MAPS_SERVICE_TEST_COORDINATES_H__

/* int maps_coordinates_create(double latitude, double longitude,
*  double altitude, maps_coordinates_h* coordinates); */
void utc_maps_coordinates_create_p(void);
void utc_maps_coordinates_create_n(void);

/* int maps_coordinates_destroy(maps_coordinates_h coordinates); */
void utc_maps_coordinates_destroy_p(void);
void utc_maps_coordinates_destroy_n(void);

/* int maps_coordinates_clone(const maps_coordinates_h origin,
*  maps_coordinates_h* cloned); */
void utc_maps_coordinates_clone_p(void);
void utc_maps_coordinates_clone_n(void);

/* int maps_coordinates_get_latitude(maps_coordinates_h coordinates,
*  double* latitude); */
/* int maps_coordinates_set_latitude(maps_coordinates_h coordinates,
*  double latitude); */
void utc_maps_coordinates_latitude_p(void);
void utc_maps_coordinates_latitude_n(void);

/* int maps_coordinates_get_longitude(maps_coordinates_h coordinates,
*  double* longitude); */
/* int maps_coordinates_set_longitude(maps_coordinates_h coordinates,
*  double longitude); */
void utc_maps_coordinates_longitude_p(void);
void utc_maps_coordinates_longitude_n(void);

#if _MAPS_COORDS_3D_
/* int maps_coordinates_get_altitude(maps_coordinates_h coordinates,
*  double* altitude); */
/* int maps_coordinates_set_altitude(maps_coordinates_h coordinates,
*  double altitude); */
void utc_maps_coordinates_altitude_p(void);
void utc_maps_coordinates_altitude_n(void);
#endif

#endif /* __MAPS_SERVICE_TEST_COORDINATES_H__ */

