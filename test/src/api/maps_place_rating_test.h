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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_RATING_H__
#define __MAPS_SERVICE_TEST_API_PLACE_RATING_H__

/* int maps_place_rating_create(maps_place_rating_h* place); */
/* int maps_place_rating_destroy(maps_place_rating_h place); */
void utc_maps_place_rating_create_p(void);
void utc_maps_place_rating_create_n(void);

/* int maps_place_rating_clone(const maps_place_rating_h origin,
*  maps_place_rating_h* cloned); */
void utc_maps_place_rating_clone_p(void);
void utc_maps_place_rating_clone_n(void);

/* int maps_place_rating_set_count(maps_place_rating_h place,
*  const int count); */
/* int maps_place_rating_get_count(maps_place_rating_h place, int* count); */
void utc_maps_place_rating_count_p(void);
void utc_maps_place_rating_count_n(void);

/* int maps_place_rating_set_average(maps_place_rating_h place,
*  const double average); */
/* int maps_place_rating_get_average(maps_place_rating_h place,
*  double* average); */
void utc_maps_place_rating_average_p(void);
void utc_maps_place_rating_average_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_RATING_H__ */

