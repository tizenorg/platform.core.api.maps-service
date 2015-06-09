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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_MEDIA_H__
#define __MAPS_SERVICE_TEST_API_PLACE_MEDIA_H__

/* int maps_place_media_create(maps_place_media_h* place); */
/* int maps_place_media_destroy(maps_place_media_h place); */
void utc_maps_place_media_create_p(void);
void utc_maps_place_media_create_n(void);

/* int maps_place_media_clone(const maps_place_media_h origin,
*  maps_place_media_h* cloned); */
void utc_maps_place_media_clone_p(void);
void utc_maps_place_media_clone_n(void);

/* int maps_place_media_set_attribution(maps_place_media_h place,
*  const char* attribution); */
/* int maps_place_media_get_attribution(maps_place_media_h place,
*  char** attribution); */
void utc_maps_place_media_attribution_p(void);
void utc_maps_place_media_attribution_n(void);

/* int maps_place_media_set_supplier(maps_place_media_h place,
*  maps_place_link_object_h supplier); */
/* int maps_place_media_get_supplier(maps_place_media_h place,
*  maps_place_link_object_h* supplier); */
void utc_maps_place_media_supplier_p(void);
void utc_maps_place_media_supplier_n(void);

/* int maps_place_media_set_via(maps_place_media_h place,
*  maps_place_link_object_h via); */
/* int maps_place_media_get_via(maps_place_media_h place,
*  aps_place_link_object_h* via); */
void utc_maps_place_media_via_p(void);
void utc_maps_place_media_via_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_MEDIA_H__ */

