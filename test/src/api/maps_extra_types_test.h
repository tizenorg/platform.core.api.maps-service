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

#ifndef __MAPS_SERVICE_TEST_API_EXTRA_TYPES_H__
#define __MAPS_SERVICE_TEST_API_EXTRA_TYPES_H__

void utc_maps_item_list_create_p(void);
void utc_maps_item_list_create_n(void);

void utc_maps_item_list_append_p(void);
void utc_maps_item_list_append_n(void);

void utc_maps_item_list_foreach_p(void);
void utc_maps_item_list_foreach_n(void);

/*----------------------------------------------------------------------------*/

void utc_maps_string_hashtable_create_p(void);
void utc_maps_string_hashtable_create_n(void);

void utc_maps_string_hashtable_set_p(void);
void utc_maps_string_hashtable_set_n(void);

void utc_maps_string_hashtable_foreach_p(void);
void utc_maps_string_hashtable_foreach_n(void);

/*----------------------------------------------------------------------------*/

void utc_maps_int_hashtable_create_p(void);
void utc_maps_int_hashtable_create_n(void);

void utc_maps_int_hashtable_set_p(void);
void utc_maps_int_hashtable_set_n(void);

void utc_maps_int_hashtable_foreach_p(void);
void utc_maps_int_hashtable_foreach_n(void);

/*----------------------------------------------------------------------------*/

void utc_maps_item_hashtable_create_p(void);
void utc_maps_item_hashtable_create_n(void);

void utc_maps_item_hashtable_set_p(void);
void utc_maps_item_hashtable_set_n(void);

void utc_maps_item_hashtable_set_macro_p(void);
void utc_maps_item_hashtable_set_macro_n(void);

void utc_maps_item_hashtable_set_overwrited_macro_p(void);

void utc_maps_item_hashtable_foreach_p(void);
void utc_maps_item_hashtable_foreach_n(void);

#endif /* __MAPS_SERVICE_TEST_API_EXTRA_TYPES_H__ */

