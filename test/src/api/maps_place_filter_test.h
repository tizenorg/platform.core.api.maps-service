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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_FILTER_H__
#define __MAPS_SERVICE_TEST_API_PLACE_FILTER_H__

/* int maps_place_filter_create(maps_place_filter_h* filter); */
/* int maps_place_filter_destroy(maps_place_filter_h filter); */
void utc_maps_place_filter_create_p(void);
void utc_maps_place_filter_create_n(void);

/* int maps_place_filter_clone(const maps_place_filter_h origin,
*  maps_place_filter_h* cloned); */
void utc_maps_place_filter_clone_p(void);
void utc_maps_place_filter_clone_n(void);

/* typedef bool (*maps_place_filter_properties_cb)(const char* key ,
*  const char* value , void *user_data); */
/* int maps_place_filter_set(maps_place_filter_h filter, const char* key,
*  const char* value); */
/* int maps_place_filter_get(maps_place_filter_h filter, const char* key,
*  char** value); */
/* int maps_place_filter_foreach_property(maps_place_filter_h filter,
*  maps_place_filter_properties_cb callback , void* user_data); */
void utc_maps_place_filter_set_p(void);
void utc_maps_place_filter_set_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_FILTER_H__ */

