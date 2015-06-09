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

#ifndef __MAPS_SERVICE_TEST_API_H__
#define __MAPS_SERVICE_TEST_API_H__

void utc_maps_service_create_destroy_p(void);

void utc_maps_provider_key_p(void);
void utc_maps_provider_key_n(void);

void utc_maps_service_set_preference_p(void);
void utc_maps_service_set_preference_n(void);

void utc_maps_service_provider_is_service_supported_p(void);
void utc_maps_service_provider_is_service_supported_n(void);

void utc_maps_service_provider_is_data_supported_p(void);
void utc_maps_service_provider_is_data_supported_n(void);

void utc_maps_service_geocode_p(void);
void utc_maps_service_geocode_n(void);

void utc_maps_service_geocode_inside_area_p(void);
void utc_maps_service_geocode_inside_area_n(void);

void utc_maps_service_geocode_by_structured_address_p(void);
void utc_maps_service_geocode_by_structured_address_n(void);

void utc_maps_service_reverse_geocode_p(void);
void utc_maps_service_reverse_geocode_n(void);

void utc_maps_cancel_geocode_p(void);
void utc_maps_cancel_geocode_p02(void);
void utc_maps_cancel_geocode_n(void);

void utc_maps_cancel_reverse_geocode_p(void);
void utc_maps_cancel_reverse_geocode_n(void);

void utc_maps_service_search_place_p(void);
void utc_maps_service_search_place_n(void);

void utc_maps_service_search_place_by_area_p(void);
void utc_maps_service_search_place_by_area_n(void);

void utc_maps_service_search_place_by_address_p(void);
void utc_maps_service_search_place_by_address_n(void);

void utc_maps_cancel_place_p(void);
void utc_maps_cancel_place_n(void);

void utc_maps_service_search_route_p(void);
void utc_maps_service_search_route_n(void);

void utc_maps_service_search_route_waypoints_p(void);
void utc_maps_service_search_route_waypoints_n(void);

void utc_maps_cancel_route_p(void);
void utc_maps_cancel_route_n(void);

void utc_maps_service_search_route_real_p(void);

#endif /* __MAPS_SERVICE_TEST_API_H__ */

