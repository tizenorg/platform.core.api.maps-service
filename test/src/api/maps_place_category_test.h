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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_CATEGORY_H__
#define __MAPS_SERVICE_TEST_API_PLACE_CATEGORY_H__

/* int maps_place_category_create(maps_place_category_h* place); */
/* int maps_place_category_destroy(maps_place_category_h place); */
void utc_maps_place_category_create_p(void);
void utc_maps_place_category_create_n(void);

/* int maps_place_category_clone(cloned, const maps_place_category_h origin,
*  maps_place_category_h*); */
void utc_maps_place_category_clone_p(void);
void utc_maps_place_category_clone_n(void);

/* int maps_place_category_get_name(maps_place_category_h place,
*  char** name); */
/* int maps_place_category_set_name(maps_place_category_h place,
*  const char* name); */
void utc_maps_place_category_name_p(void);
void utc_maps_place_category_name_n(void);

/* int maps_place_category_get_id(maps_place_category_h place, char** id); */
/* int maps_place_category_set_id(maps_place_category_h place,
*  const char* id); */
void utc_maps_place_category_id_p(void);
void utc_maps_place_category_id_n(void);

/* int maps_place_category_get_url(maps_place_category_h place,  char** url); */
/* int maps_place_category_set_url(maps_place_category_h place,
*  const char* url); */
void utc_maps_place_category_url_p(void);
void utc_maps_place_category_url_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_CATEGORY_H__ */

