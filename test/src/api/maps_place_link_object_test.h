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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_LINK_OBJECT_H__
#define __MAPS_SERVICE_TEST_API_PLACE_LINK_OBJECT_H__

/* int maps_place_link_object_create(maps_place_link_object_h* place); */
/* int maps_place_link_object_destroy(maps_place_link_object_h place); */
void utc_maps_place_link_object_create_p(void);
void utc_maps_place_link_object_create_n(void);

/* int maps_place_link_object_clone(const maps_place_link_object_h origin,
*  maps_place_link_object_h* cloned); */
void utc_maps_place_link_object_clone_p(void);
void utc_maps_place_link_object_clone_n(void);

/* int maps_place_link_object_set_id(maps_place_link_object_h place,
*  const char* id); */
/* int maps_place_link_object_get_id(maps_place_link_object_h place,
*  char** id); */
void utc_maps_place_link_object_id_p(void);
void utc_maps_place_link_object_id_n(void);

/* int maps_place_link_object_set_name(maps_place_link_object_h place,
*  const char* name); */
/* int maps_place_link_object_get_name(maps_place_link_object_h place,
*  char** name); */
void utc_maps_place_link_object_name_p(void);
void utc_maps_place_link_object_name_n(void);

/* int maps_place_link_object_set_type(maps_place_link_object_h place,
*  const char* type); */
/* int maps_place_link_object_get_type(maps_place_link_object_h place,
*  char** type); */
void utc_maps_place_link_object_type_p(void);
void utc_maps_place_link_object_type_n(void);

/* int maps_place_link_object_set_string(maps_place_link_object_h place,
*  const char* string); */
/* int maps_place_link_object_get_string(maps_place_link_object_h place,
*  char** string); */
void utc_maps_place_link_object_string_p(void);
void utc_maps_place_link_object_string_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_LINK_OBJECT_H__ */

