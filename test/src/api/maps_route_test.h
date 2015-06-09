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

#ifndef __MAPS_SERVICE_TEST_API_ROUTE_H__
#define __MAPS_SERVICE_TEST_API_ROUTE_H__

/* int maps_route_create(maps_route_h* route); */
/* int maps_route_destroy(maps_route_h route); */
void utc_maps_route_create_p(void);
void utc_maps_route_create_n(void);

/* int maps_route_clone(const maps_route_h origin, maps_route_h* cloned); */
void utc_maps_route_clone_p(void);
void utc_maps_route_clone_n(void);

/* int maps_route_set_route_id(maps_route_h route, const char* route_id); */
/* int maps_route_get_route_id(const maps_route_h route, char** route_id); */
void utc_maps_route_route_id_p(void);
void utc_maps_route_route_id_n(void);

/* int maps_route_set_origin(maps_route_h route, maps_coordinates_h origin); */
/* int maps_route_get_origin(maps_route_h route, maps_coordinates_h* origin); */
void utc_maps_route_origin_p(void);
void utc_maps_route_origin_n(void);

/* int maps_route_set_destination(maps_route_h route,
*  maps_coordinates_h destination); */
/* int maps_route_get_destination(maps_route_h route,
*  maps_coordinates_h* destination); */
void utc_maps_route_destination_p(void);
void utc_maps_route_destination_n(void);

/* int maps_route_set_bounding_box(maps_route_h route,
*  maps_area_h bounding_box); */
/* int maps_route_get_bounding_box(maps_route_h route,
*  maps_area_h* bounding_box); */
void utc_maps_route_bounding_box_p(void);
void utc_maps_route_bounding_box_n(void);

/* int maps_route_set_transport_mode(maps_route_h route,
*  const char* transport_mode); */
/* int maps_route_get_transport_mode(maps_route_h route,
*  char** transport_mode); */
void utc_maps_route_transport_mode_p(void);
void utc_maps_route_transport_mode_n(void);

/* int maps_route_set_total_distance(maps_route_h route,
*  double total_distance); */
/* int maps_route_get_total_distance(maps_route_h route,
*  double* total_distance); */
void utc_maps_route_total_distance_p(void);
void utc_maps_route_total_distance_n(void);

/* int maps_route_set_total_duration(maps_route_h route,
*  long total_duration); */
/* int maps_route_get_total_duration(maps_route_h route,
*  long* total_duration); */
void utc_maps_route_total_duration_p(void);
void utc_maps_route_total_duration_n(void);

/* int maps_route_maneuver_get_distance_unit(
*  const maps_route_maneuver_h maneuver,
*  maps_distance_unit_e* distance_unit); */
/* int maps_route_maneuver_set_distance_unit(
*  const maps_route_maneuver_h maneuver,
*  const maps_distance_unit_e distance_unit); */
void utc_maps_route_distance_unit_p(void);
void utc_maps_route_distance_unit_n(void);

/* int maps_route_foreach_path(const maps_route_h route,
*  maps_route_path_cb callback, void* user_data); */
/* int maps_route_set_path(maps_route_h route, const maps_item_list_h path); */
void utc_maps_route_path_p(void);
void utc_maps_route_path_n(void);

/* int maps_route_foreach_segment(const maps_route_h route,
*  maps_route_segment_cb callback, void* user_data); */
/* int maps_route_set_segments(maps_route_h route,
*  const maps_item_list_h segments); */
void utc_maps_route_segments_p(void);
void utc_maps_route_segments_n(void);

/* int maps_route_set_properties(maps_route_h route,
*  maps_item_hashtable_h properties); */
/* int maps_route_foreach_property(maps_route_h route,
*  maps_route_properties_cb callback, void* user_data); */
void utc_maps_route_properties_p(void);
void utc_maps_route_properties_n(void);

#endif /* __MAPS_SERVICE_TEST_API_ROUTE_H__ */

