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

#ifndef __MAPS_SERVICE_TEST_API_ROUTE_SEGMENT_H__
#define __MAPS_SERVICE_TEST_API_ROUTE_SEGMENT_H__

/* int maps_route_segment_create(maps_route_segment_h* segment); */
/* int maps_route_segment_destroy(maps_route_segment_h segment); */
void utc_maps_route_segment_create_p(void);
void utc_maps_route_segment_create_n(void);

/* int maps_route_segment_clone(const maps_route_segment_h origin,
*  maps_route_segment_h* cloned); */
void utc_maps_route_segment_clone_p(void);
void utc_maps_route_segment_clone_n(void);

/* int maps_route_segment_get_origin(const maps_route_segment_h segment,
*  maps_coordinates_h* origin); */
/* int maps_route_segment_set_origin(maps_route_segment_h segment,
*  const maps_coordinates_h origin); */
void utc_maps_route_segment_origin_p(void);
void utc_maps_route_segment_origin_n(void);

/* int maps_route_segment_get_destination(const maps_route_segment_h segment,
*  maps_coordinates_h* destination); */
/* int maps_route_segment_set_destination(maps_route_segment_h segment,
*  const maps_coordinates_h destination); */
void utc_maps_route_segment_destination_p(void);
void utc_maps_route_segment_destination_n(void);

/* int maps_route_segment_get_bounding_box(const maps_route_segment_h segment,
*  maps_area_h* bounding_box); */
/* int maps_route_segment_set_bounding_box(maps_route_segment_h segment,
*  maps_area_h bounding_box); */
void utc_maps_route_segment_bounding_box_p(void);
void utc_maps_route_segment_bounding_box_n(void);

/* int maps_route_segment_get_distance(const maps_route_segment_h segment,
*  double* distance); */
/*int maps_route_segment_set_distance(maps_route_segment_h segment,
* double distance); */
void utc_maps_route_segment_distance_p(void);
void utc_maps_route_segment_distance_n(void);

/* int maps_route_segment_get_duration(const maps_route_segment_h segment,
*  long* duration); */
/* int maps_route_segment_set_duration(maps_route_segment_h segment,
*  long duration); */
void utc_maps_route_segment_duration_p(void);
void utc_maps_route_segment_duration_n(void);

/* int maps_route_foreach_path(const maps_route_segment_h route,
*  maps_route_path_cb callback, void* user_data); */
/* int maps_route_segment_set_path(const maps_route_segment_h segment,
*  maps_item_list_h path); */
void utc_maps_route_segment_path_p(void);
void utc_maps_route_segment_path_n(void);

/* int maps_route_segment_foreach_maneuver(const maps_route_segment_h route,
*  maps_route_maneuver_cb callback, void* user_data); */
/* int maps_route_segment_set_maneuvers(maps_route_segment_h route,
*  const maps_item_list_h maneuvers); */
void utc_maps_route_segment_maneuvers_p(void);
void utc_maps_route_segment_maneuvers_n(void);

#endif /* __MAPS_SERVICE_TEST_API_ROUTE_SEGMENT_H__ */

