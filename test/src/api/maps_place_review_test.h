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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_REVIEW_H__
#define __MAPS_SERVICE_TEST_API_PLACE_REVIEW_H__

/* int maps_place_review_create(maps_place_review_h* place); */
/* int maps_place_review_destroy(maps_place_review_h place); */
void utc_maps_place_review_create_p(void);
void utc_maps_place_review_create_n(void);

/* int maps_place_review_clone(const maps_place_review_h origin,
*  maps_place_review_h* cloned); */
void utc_maps_place_review_clone_p(void);
void utc_maps_place_review_clone_n(void);

/* int maps_place_review_set_date(maps_place_review_h place,
*  const char* date); */
/* int maps_place_review_get_date(maps_place_review_h place, char** date); */
void utc_maps_place_review_date_p(void);
void utc_maps_place_review_date_n(void);

/* int maps_place_review_set_title(maps_place_review_h place,
*  const char* title); */
/* int maps_place_review_get_title(maps_place_review_h place, char** title); */
void utc_maps_place_review_title_p(void);
void utc_maps_place_review_title_n(void);

/* int maps_place_review_set_rating(maps_place_review_h place,
*  const double rating); */
/* int maps_place_review_get_rating(maps_place_review_h place,
*  double* rating); */
void utc_maps_place_review_rating_p(void);
void utc_maps_place_review_rating_n(void);

/* int maps_place_review_set_description(maps_place_review_h place,
*  const char* description); */
/* int maps_place_review_get_description(maps_place_review_h place,
*  char** description); */
void utc_maps_place_review_description_p(void);
void utc_maps_place_review_description_n(void);

/* int maps_place_review_set_language(maps_place_review_h place,
*  const char* language); */
/* int maps_place_review_get_language(maps_place_review_h place,
*  char** language); */
void utc_maps_place_review_language_p(void);
void utc_maps_place_review_language_n(void);

/* int maps_place_review_set_media(maps_place_review_h place,
*  maps_place_media_h media); */
/* int maps_place_review_get_media(maps_place_review_h place,
*  maps_place_media_h* media); */
void utc_maps_place_review_media_p(void);
void utc_maps_place_review_media_n(void);

/* int maps_place_review_set_user_link(maps_place_review_h place,
*  maps_place_link_object_h user); */
/* int maps_place_review_get_user_link(maps_place_review_h place,
*  maps_place_link_object_h* user); */
void utc_maps_place_review_user_link_p(void);
void utc_maps_place_review_user_link_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_REVIEW_H__ */

