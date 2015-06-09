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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_CONTACT_H__
#define __MAPS_SERVICE_TEST_API_PLACE_CONTACT_H__

/* int maps_place_contact_create(maps_place_contact_h* place); */
/* int maps_place_contact_destroy(maps_place_contact_h place); */
void utc_maps_place_contact_create_p(void);
void utc_maps_place_contact_create_n(void);

/* int maps_place_contact_clone(const maps_place_contact_h origin,
*  maps_place_contact_h* cloned); */
void utc_maps_place_contact_clone_p(void);
void utc_maps_place_contact_clone_n(void);

/* int maps_place_contact_set_label(maps_place_contact_h place,
*  const char* label); */
/* int maps_place_contact_get_label(maps_place_contact_h place,
*  char** label); */
void utc_maps_place_contact_label_p(void);
void utc_maps_place_contact_label_n(void);

/* int maps_place_contact_set_type(maps_place_contact_h place,
*  const char* type); */
/* int maps_place_contact_get_type(maps_place_contact_h place, char** type); */
void utc_maps_place_contact_type_p(void);
void utc_maps_place_contact_type_n(void);

/* int maps_place_contact_set_value(maps_place_contact_h place,
*  const char* value); */
/* int maps_place_contact_get_value(maps_place_contact_h place,
*  char** value); */
void utc_maps_place_contact_value_p(void);
void utc_maps_place_contact_value_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_CONTACT_H__ */

