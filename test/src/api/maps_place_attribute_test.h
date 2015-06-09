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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_ATTRIBUTE_H__
#define __MAPS_SERVICE_TEST_API_PLACE_ATTRIBUTE_H__

/* int maps_place_attribute_create(maps_place_attribute_h* place); */
/* int maps_place_attribute_destroy(maps_place_attribute_h place); */
void utc_maps_place_attribute_create_p(void);
void utc_maps_place_attribute_create_n(void);

/* int maps_place_attribute_clone(const maps_place_attribute_h origin,
*  maps_place_attribute_h* cloned); */
void utc_maps_place_attribute_clone_p(void);
void utc_maps_place_attribute_clone_n(void);

/* int maps_place_attribute_set_id(maps_place_attribute_h place,
*  const char* id); */
/* int maps_place_attribute_get_id(maps_place_attribute_h place, char** id); */
void utc_maps_place_attribute_id_p(void);
void utc_maps_place_attribute_id_n(void);

/* int maps_place_attribute_set_label(maps_place_attribute_h place,
*  const char* label); */
/* int maps_place_attribute_get_label(maps_place_attribute_h place,
*  char** label); */
void utc_maps_place_attribute_label_p(void);
void utc_maps_place_attribute_label_n(void);

/* int maps_place_attribute_set_text(maps_place_attribute_h place,
*  const char* text); */
/* int maps_place_attribute_get_text(maps_place_attribute_h place,
*  const char** text); */
void utc_maps_place_attribute_text_p(void);
void utc_maps_place_attribute_text_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_ATTRIBUTE_H__ */

