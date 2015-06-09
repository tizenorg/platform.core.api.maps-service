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

#ifndef __MAPS_SERVICE_TEST_API_PREFERENCE_H__
#define __MAPS_SERVICE_TEST_API_PREFERENCE_H__

/*int maps_preference_create(maps_preference_h* preference); */
/*int maps_preference_destroy(maps_preference_h preference); */
void utc_maps_preference_create_p(void);
void utc_maps_preference_create_n(void);

/*int maps_preference_clone(const maps_preference_h origin,
* maps_preference_h* cloned); */
void utc_maps_preference_clone_p(void);
void utc_maps_preference_clone_n(void);

/*int maps_preference_get_distance_unit(const maps_preference_h preference,
* maps_distance_unit_e* unit); */
/*int maps_preference_set_distance_unit(maps_preference_h preference,
* const maps_distance_unit_e unit); */
void utc_maps_preference_distance_unit_p(void);
void utc_maps_preference_distance_unit_n(void);

/*int maps_preference_get_language(const maps_preference_h preference,
* char** language); */
/*int maps_preference_set_language(maps_preference_h preference,
* const char* language); */
void utc_maps_preference_language_p(void);
void utc_maps_preference_language_n(void);

/*int maps_preference_get_max_results(const maps_preference_h preference,
* int* max_results); */
/*int maps_preference_set_max_results(maps_preference_h preference,
* const int max_results); */
void utc_maps_preference_max_results_p(void);
void utc_maps_preference_max_results_n(void);

/*int maps_preference_get_country_code(const maps_preference_h preference,
* char** country_code); */
/*int maps_preference_set_country_code(maps_preference_h preference,
* const char* country_code); */
void utc_maps_preference_country_code_p(void);
void utc_maps_preference_country_code_n(void);

/*int maps_preference_get_route_optimization(
* const maps_preference_h preference,
* maps_route_optimization_e* optimization); */
/*int maps_preference_set_route_optimization(maps_preference_h preference,
* const maps_route_optimization_e optimization); */
void utc_maps_preference_route_optimization_p(void);
void utc_maps_preference_route_optimization_n(void);

/*int maps_preference_get_route_transport_mode(
* const maps_preference_h preference,
* maps_route_transport_mode_e* transport_mode); */
/*int maps_preference_set_route_transport_mode(maps_preference_h preference,
* const maps_route_transport_mode_e transport_mode); */
void utc_maps_preference_route_transport_mode_p(void);
void utc_maps_preference_route_transport_mode_n(void);

/*int maps_preference_get_route_feature_weight(
* const maps_preference_h preference,
* maps_route_feature_weight_e* feature_weight); */
/*int maps_preference_set_route_feature_weight(maps_preference_h preference,
* const maps_route_feature_weight_e feature_weight); */
void utc_maps_preference_route_feature_weight_p(void);
void utc_maps_preference_route_feature_weight_n(void);

/*int maps_preference_get_route_feature(const maps_preference_h preference,
* maps_route_feature_weight_e* feature); */
/*int maps_preference_set_route_feature(maps_preference_h preference,
* const maps_route_feature_e feature); */
void utc_maps_preference_route_feature_p(void);
void utc_maps_preference_route_feature_n(void);

/*int maps_preference_get(const maps_preference_h preference, const char* key,
* char** value); */
/*int maps_preference_set_property(maps_preference_h preference,
* const char* key, const char* value); */
/*typedef bool (*maps_preference_properties_cb)(int index, int total, char* key,
* char* value, void* user_data); */
/*int maps_preference_foreach_property(const maps_preference_h preference,
* maps_preference_properties_cb callback , void* user_data); */
void utc_maps_preference_property_p(void);
void utc_maps_preference_property_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PREFERENCE_H__ */

