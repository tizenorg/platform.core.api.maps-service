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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_IMAGE_H__
#define __MAPS_SERVICE_TEST_API_PLACE_IMAGE_H__

/* int maps_place_image_create(maps_place_image_h* place); */
/* int maps_place_image_destroy(maps_place_image_h place); */
void utc_maps_place_image_create_p(void);
void utc_maps_place_image_create_n(void);

/* int maps_place_image_clone(const maps_place_image_h origin,
*  maps_place_image_h* cloned); */
void utc_maps_place_image_clone_p(void);
void utc_maps_place_image_clone_n(void);

/* int maps_place_image_set_id(maps_place_image_h place, const char* id); */
/* int maps_place_image_get_id(maps_place_image_h place, char** id); */
void utc_maps_place_image_id_p(void);
void utc_maps_place_image_id_n(void);

/* int maps_place_image_set_url(maps_place_image_h place, const char* url); */
/* int maps_place_image_get_url(maps_place_image_h place, char** url); */
void utc_maps_place_image_url_p(void);
void utc_maps_place_image_url_n(void);

/* int maps_place_image_set_width(maps_place_image_h place, const int width); */
/* int maps_place_image_get_width(maps_place_image_h place,  int* width); */
void utc_maps_place_image_width_p(void);
void utc_maps_place_image_width_n(void);

/* int maps_place_image_set_height(maps_place_image_h place, const int height); */
/* int maps_place_image_get_height(maps_place_image_h place,  int* height); */
void utc_maps_place_image_height_p(void);
void utc_maps_place_image_height_n(void);

/* int maps_place_image_set_user_link(maps_place_image_h place,
*  maps_place_link_object_h user); */
/* int maps_place_image_get_user_link(maps_place_image_h place,
*  maps_place_link_object_h* user); */
void utc_maps_place_image_user_link_p(void);
void utc_maps_place_image_user_link_n(void);

/* int maps_place_image_set_media(maps_place_image_h place,
*  maps_place_media_h media); */
/* int maps_place_image_get_media(maps_place_image_h place,
*  maps_place_media_h* media); */
void utc_maps_place_image_media_p(void);
void utc_maps_place_image_media_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_IMAGE_H__ */

