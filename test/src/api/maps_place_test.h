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

#ifndef __MAPS_SERVICE_TEST_API_PLACE_H__
#define __MAPS_SERVICE_TEST_API_PLACE_H__

/* int maps_place_create(maps_place_h* place); */
/* int maps_place_destroy(maps_place_h place); */
void utc_maps_place_create_p(void);
void utc_maps_place_create_n(void);

/* int maps_place_clone(const maps_place_h origin, maps_place_h* cloned); */
void utc_maps_place_clone_p(void);
void utc_maps_place_clone_n(void);

/* int maps_place_set_id(maps_place_h place, const char* id); */
/* int maps_place_get_id(maps_place_h place, char** id); */
void utc_maps_place_set_id_p(void);
void utc_maps_place_set_id_n(void);

/* int maps_place_set_name(maps_place_h place, const char* name); */
/* int maps_place_get_name(maps_place_h place, char** name); */
void utc_maps_place_set_name_p(void);
void utc_maps_place_set_name_n(void);

/* int maps_place_set_location(maps_place_h place,
*  maps_coordinates_h location); */
/* int maps_place_get_location(maps_place_h place,
*  maps_coordinates_h* location); */
void utc_maps_place_set_location_p(void);
void utc_maps_place_set_location_n(void);

/* int maps_place_set_categories(maps_place_h place, GList* categories); */
/* int maps_place_foreach_category(maps_place_h place,
*  maps_place_categories_cb callback, void* user_data); */
void utc_maps_place_set_category_p(void);
void utc_maps_place_set_category_n(void);

/* int maps_place_set_uri(maps_place_h place, const char* uri); */
/* int maps_place_get_uri(maps_place_h place, char** uri); */
void utc_maps_place_set_uri_p(void);
void utc_maps_place_set_uri_n(void);

/* int maps_place_set_attributes(maps_place_h place, GList* attributes); */
/* int maps_place_foreach_attribute(maps_place_h place,
*  maps_place_attributes_cb callback, void* user_data); */
void utc_maps_place_set_attribute_p(void);
void utc_maps_place_set_attribute_n(void);

/* int maps_place_set_contacts(maps_place_h place, GList* contacts); */
/* int maps_place_foreach_contact(maps_place_h place,
*  maps_place_contacts_cb callback, void* user_data); */
void utc_maps_place_set_contacts_p(void);
void utc_maps_place_set_contacts_n(void);

/* int maps_place_set_editorials(maps_place_h place, GList* editorials); */
/* int maps_place_foreach_editorial(maps_place_h place,
*  maps_place_editorials_cb callback, void* user_data); */
void utc_maps_place_set_editorial_p(void);
void utc_maps_place_set_editorial_n(void);

/* int maps_place_set_images(maps_place_h place, GList* images); */
/* int maps_place_foreach_image(maps_place_h place,
*  maps_place_images_cb callback, void* user_data); */
void utc_maps_place_set_image_p(void);
void utc_maps_place_set_image_n(void);

/* int maps_place_set_reviews(maps_place_h place, GList* reviews); */
/* int maps_place_foreach_review(maps_place_h place,
*  maps_place_reviews_cb callback, void* user_data); */
void utc_maps_place_set_review_p(void);
void utc_maps_place_set_review_n(void);

/* int maps_place_set_properties(maps_place_h place,
*  maps_item_hashtable_h properties) */
/* int maps_place_foreach_property(maps_place_h place,
*  maps_place_properties_cb callback, void* user_data) { */
void utc_maps_place_set_properties_p(void);
void utc_maps_place_set_properties_n(void);

/* int maps_place_set_rating(maps_place_h place,
*  maps_place_rating_h rating); */
/* int maps_place_get_rating(maps_place_h place ,
*  maps_place_rating_h* rating); */
void utc_maps_place_set_rating_p(void);
void utc_maps_place_set_rating_n(void);

/* int maps_place_set_supplier_link(maps_place_h place,
*  maps_place_link_object_h supplier); */
/* int maps_place_get_supplier_link(maps_place_image_h place,
*  maps_place_link_object_h* supplier); */
void utc_maps_place_set_supplier_link_p(void);
void utc_maps_place_set_supplier_link_n(void);

/* int maps_place_set_related_link(maps_place_h place,
*  maps_place_link_object_h related); */
/* int maps_place_get_related_link(maps_place_image_h place,
*  maps_place_link_object_h* related); */
void utc_maps_place_set_related_link_p(void);
void utc_maps_place_set_related_link_n(void);

/* int maps_place_foreach_property(maps_place_h place,
*  maps_place_properties_cb callback, void* user_data); */
void utc_maps_place_foreach_property_p(void);
void utc_maps_place_foreach_property_n(void);

#endif /* __MAPS_SERVICE_TEST_API_PLACE_H__ */

