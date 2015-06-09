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

#ifndef __MAPS_SERVICE_TEST_API_ROUTE_MANEUVER_H__
#define __MAPS_SERVICE_TEST_API_ROUTE_MANEUVER_H__

/* int maps_route_maneuver_create(maps_route_maneuver_h* maneuver); */
/* int maps_route_maneuver_destroy(maps_route_maneuver_h maneuver); */
void utc_maps_route_maneuver_create_p(void);
void utc_maps_route_maneuver_create_n(void);

/* int maps_route_maneuver_clone(const maps_route_maneuver_h origin,
*  maps_route_maneuver_h* cloned); */
void utc_maps_route_maneuver_clone_p(void);
void utc_maps_route_maneuver_clone_n(void);

/* int maps_route_maneuver_get_direction_id(
*  const maps_route_maneuver_h maneuver,
*  maps_route_direction_e* direction_id); */
/* int maps_route_maneuver_set_direction_id(maps_route_maneuver_h maneuver,
*  const maps_route_direction_e direction_id); */
void utc_maps_route_maneuver_direction_id_p(void);
void utc_maps_route_maneuver_direction_id_n(void);

/* int maps_route_maneuver_get_turn_type(const maps_route_maneuver_h maneuver,
*  maps_route_turn_type_e* turn_type); */
/* int maps_route_maneuver_set_turn_type(maps_route_maneuver_h maneuver,
*  const maps_route_turn_type_e turn_type); */
void utc_maps_route_maneuver_turn_type_p(void);
void utc_maps_route_maneuver_turn_type_n(void);

/* int maps_route_maneuver_get_position(maps_route_maneuver_h maneuver,
*  maps_coordinates_h* position); */
/* int maps_route_maneuver_set_position(maps_route_maneuver_h maneuver,
*  const maps_coordinates_h position); */
void utc_maps_route_maneuver_position_p(void);
void utc_maps_route_maneuver_position_n(void);

/* int maps_route_maneuver_get_road_name(maps_route_maneuver_h maneuver,
*  char** road_name); */
/* int maps_route_maneuver_set_road_name(maps_route_maneuver_h maneuver,
*  const char* road_name); */
void utc_maps_route_maneuver_road_name_p(void);
void utc_maps_route_maneuver_road_name_n(void);

/* int maps_route_maneuver_get_locale(const maps_route_maneuver_h maneuver,
*  char** locale); */
/* int maps_route_maneuver_set_locale(maps_route_maneuver_h maneuver,
*  const char* locale); */
void utc_maps_route_maneuver_locale_p(void);
void utc_maps_route_maneuver_locale_n(void);

/* int maps_route_maneuver_get_time_to_next_instruction(
*  const maps_route_maneuver_h maneuver, int* time_to_next_instruction); */
/* int maps_route_maneuver_set_time_to_next_instruction(
*  maps_route_maneuver_h maneuver, int time_to_next_instruction); */
void utc_maps_route_maneuver_time_to_next_instruction_p(void);
void utc_maps_route_maneuver_time_to_next_instruction_n(void);

/* int maps_route_maneuver_get_distance_to_next_instruction(
*  const maps_route_maneuver_h maneuver,
*  double* distance_to_next_instruction); */
/* int maps_route_maneuver_set_distance_to_next_instruction(
*  maps_route_maneuver_h maneuver, double distance_to_next_instruction); */
void utc_maps_route_maneuver_distance_to_next_instruction_p(void);
void utc_maps_route_maneuver_distance_to_next_instruction_n(void);

/* int maps_route_maneuver_get_instruction_text(maps_route_maneuver_h maneuver,
*  char** instruction_text); */
/* int maps_route_maneuver_set_instruction_text(maps_route_maneuver_h maneuver,
*  const char* instruction_text); */
void utc_maps_route_maneuver_instruction_text_p(void);
void utc_maps_route_maneuver_instruction_text_n(void);

#endif /* __MAPS_SERVICE_TEST_API_ROUTE_MANEUVER_H__ */

