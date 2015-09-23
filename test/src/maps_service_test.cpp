/*
 * context-manager
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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
 *
 */

#include <glib.h>
#include <glib/gprintf.h>

/*----------------------------------------------------------------------------*/
/* */
/* Select between unit testing mode: */
/* */
/* */
/*  - Correctness Unit Tests */
#define MAPS_SERVICE_CORRECTNESS_TEST
/* */
/*  - Load Unit Tests */
/*#define MAPS_SERVICE_LOAD_TEST*/
/* */
/*  - Valgrind Tests */
/*#define MAPS_SERVICE_VALGRIND_TEST */
/* */
/* */
/*----------------------------------------------------------- */
/* Number of iterations Test Cases to be run */
/* It it is not a memory leak test, */
/* the value of iterations will be automatically  reduced to 1 */
static long long iterations = 250 * 1000;	/* For load test */

/* */
#ifdef MAPS_SERVICE_LOAD_TEST
	/* Sleep before next iteration */
static unsigned int sleep_microseconds = 0;	/* No delay */
		/*10*1000;*/      /* 10 milliseconds */
		/*100*1000;*/     /* 100 milliseconds */
		/*500*1000;*/     /* Half a second */
		/*1000*1000;*/    /* 1 second */

#endif				/* MAPS_SERVICE_LOAD_TEST */
/* */
/*----------------------------------------------------------------------------*/

#ifdef MAPS_SERVICE_VALGRIND_TEST

	/* This macro allows to skip tests, which generate Valgrind warning */
	/* Usually, these warnings are coming from the depth of glib */
	/*#define SUPPRESS_VALGRIND_WARNINGS */

	/* This macro turnes off g_assert */
#define G_DISABLE_ASSERT

#endif				/* MAPS_SERVICE_VALGRIND_TEST */

/* Type of each unit test */
typedef void (*test_funcvoid) (void);

#include <unistd.h>

/* Test Cases */
#include "discovery_test.h"
#include "module_test.h"

#include "command_test.h"
#include "commands_test.h"
#include "command_queue_test.h"
#include "thread_test.h"

#include "maps_address_test.h"
#include "maps_api_test.h"
#include "maps_area_test.h"
#include "maps_coordinates_test.h"
#include "maps_extra_types_test.h"

#include "maps_place_test.h"
#include "maps_place_category_test.h"
#include "maps_place_attribute_test.h"
#include "maps_place_contact_test.h"
#include "maps_place_editorial_test.h"
#include "maps_place_link_object_test.h"
#include "maps_place_media_test.h"
#include "maps_place_url_test.h"
#include "maps_place_review_test.h"
#include "maps_place_rating_test.h"
#include "maps_place_image_test.h"
#include "maps_place_filter_test.h"

#include "maps_route_test.h"
#include "maps_route_maneuver_test.h"
#include "maps_route_segment_test.h"

#include "maps_preference_test.h"

#include "map_view_test.h"
#include "map_object_test.h"
#include "map_event_data_test.h"

#include "poly_shape_hit_test_test.h"
#include "marker_constructor_test.h"
#include "polygon_constructor_test.h"
#include "polyline_constructor_test.h"
#include "route_constructor_test.h"

#include "maps_service_tutorial.h"


static void __test_add_func_load(const char *testpath, test_funcvoid func,
	const long long i)
{
	static char test_name[100] = { 0 };
	if (i > 0)
		g_sprintf(test_name, "%lld%s", i, testpath);
	else
		g_sprintf(test_name, "%s", testpath);

#ifdef MAPS_SERVICE_CORRECTNESS_TEST
	g_test_add_func(test_name, func);
#else
	g_print("%s\n", test_name);
	func();
#endif				/* MAPS_SERVICE_CORRECTNESS_TEST */
}

static void __utc_simple_test(void)
{
	g_print("Simple Test");
	g_assert(true);
}

static void __add_plugin_tests(int i)
{
	__test_add_func_load("/plugin/discovery/provider_info_construct_p",
		utc_provider_info_construct_p, i);
	/*__test_add_func_load("/plugin/discovery/operator_provider_info_h_p",
	* utc_operator_maps_provider_info_h_p, i); */
	__test_add_func_load("/plugin/discovery/provider_info_empty_p",
		utc_provider_info_empty_p, i);
	__test_add_func_load("/plugin/discovery/provider_info_empty_n_01",
		utc_provider_info_empty_n_01, i);
	__test_add_func_load("/plugin/discovery/provider_info_empty_n_02",
		utc_provider_info_empty_n_02, i);
	__test_add_func_load("/plugin/discovery/provider_info_empty_n_03",
		utc_provider_info_empty_n_03, i);

#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */
	__test_add_func_load("/plugin/discovery/get_available_list_p",
		utc_get_available_plugin_list, i);
	__test_add_func_load("/plugin/binary_extractor/init_p",
		utc_binary_extractor_init_plugin_p, i);
#endif
}

static void __add_session_tests(int i)
{
	__test_add_func_load("/session/command/get_empty_command_ptr_p",
		utc_command_empty_ptr_p, i);

#ifndef SUPPRESS_VALGRIND_WARNINGS
	__test_add_func_load("/session/command_queue/push_p",
			     utc_command_queue_push_p, i);
				/* Valgrind warnings!!! */
	__test_add_func_load("/session/command_queue/push_n",
			     utc_command_queue_push_n, i);
				/* Valgrind warnings!!! */

	__test_add_func_load("/session/glib_hashtable/use_p",
		utc_use_hash_table_p, i);

	__test_add_func_load("/session/pending_request/add_p",
		utc_pending_request_add_p, i);
	__test_add_func_load("/session/pending_request/add_n",
		utc_pending_request_add_n, i);
	__test_add_func_load("/session/pending_request/update_p",
		utc_pending_request_update_p, i);
	__test_add_func_load("/session/pending_request/update_n",
		utc_pending_request_update_n, i);
	__test_add_func_load("/session/pending_request/remove_p",
		utc_pending_request_remove_p, i);
	__test_add_func_load("/session/pending_request/remove_n",
		utc_pending_request_remove_n, i);
	__test_add_func_load("/session/pending_request/look_up_p",
		utc_pending_request_look_up_p, i);
	__test_add_func_load("/session/pending_request/look_up_n",
		utc_pending_request_look_up_n, i);
	__test_add_func_load("/session/pending_request/extract_p",
		utc_pending_request_extract_p, i);
	__test_add_func_load("/session/pending_request/extract_n",
		utc_pending_request_extract_n, i);
	__test_add_func_load("/session/pending_request/contains_p",
		utc_pending_request_contains_p, i);
	__test_add_func_load("/session/pending_request/contains_n",
		utc_pending_request_contains_n, i);
#endif
}

static void __add_extra_data_structure_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */
	if(1) {
		/* API: Extra Data Structures: List */
		__test_add_func_load("/api/list/create_p",
				     utc_maps_item_list_create_p, i);
		__test_add_func_load("/api/list/create_n",
				     utc_maps_item_list_create_n, i);
		__test_add_func_load("/api/list/append_p",
				     utc_maps_item_list_append_p, i);
		__test_add_func_load("/api/list/append_n",
				     utc_maps_item_list_append_n, i);
		__test_add_func_load("/api/list/foreach_p",
				     utc_maps_item_list_foreach_p, i);
		__test_add_func_load("/api/list/foreach_n",
				     utc_maps_item_list_foreach_n, i);
	}

	if(1) {
		/* API: Extra Data Structures: String Hash Table */
		__test_add_func_load("/api/string_hashtable/create_p",
				     utc_maps_string_hashtable_create_p, i);
		__test_add_func_load("/api/string_hashtable/create_n",
				     utc_maps_string_hashtable_create_n, i);
		__test_add_func_load("/api/string_hashtable/set_p",
				     utc_maps_string_hashtable_set_p, i);
		__test_add_func_load("/api/string_hashtable/set_n",
				     utc_maps_string_hashtable_set_n, i);
		__test_add_func_load("/api/string_hashtable/foreach_p",
				     utc_maps_string_hashtable_foreach_p, i);
		__test_add_func_load("/api/string_hashtable/foreach_n",
				     utc_maps_string_hashtable_foreach_n, i);
	}

	if(1) {
		/* API: Extra Data Structures: Integer Hash Table */
		__test_add_func_load("/api/int_hashtable/create_p",
				     utc_maps_int_hashtable_create_p, i);
		__test_add_func_load("/api/int_hashtable/create_n",
				     utc_maps_int_hashtable_create_n, i);
		__test_add_func_load("/api/int_hashtable/set_p",
				     utc_maps_int_hashtable_set_p, i);
		__test_add_func_load("/api/int_hashtable/set_n",
				     utc_maps_int_hashtable_set_n, i);
		__test_add_func_load("/api/int_hashtable/foreach_p",
				     utc_maps_int_hashtable_foreach_p, i);
		__test_add_func_load("/api/int_hashtable/foreach_n",
				     utc_maps_int_hashtable_foreach_n, i);
	}

	if(1) {
		/* API: Extra Data Structures: Hash Table */
		__test_add_func_load("/api/item_hashtable/create_p",
				     utc_maps_item_hashtable_create_p, i);
		__test_add_func_load("/api/item_hashtable/create_n",
				     utc_maps_item_hashtable_create_n, i);
		__test_add_func_load("/api/item_hashtable/set_p",
				     utc_maps_item_hashtable_set_p, i);
		__test_add_func_load("/api/item_hashtable/set_n",
				     utc_maps_item_hashtable_set_n, i);
		__test_add_func_load("/api/item_hashtable/set_macro_p",
				     utc_maps_item_hashtable_set_macro_p, i);
		__test_add_func_load("/api/item_hashtable/set_macro_n",
				     utc_maps_item_hashtable_set_macro_n, i);
		__test_add_func_load("/api/item_hashtable/set_overwrtd_macro_p",
			utc_maps_item_hashtable_set_overwrited_macro_p, i);
		__test_add_func_load("/api/item_hashtable/foreach_p",
				     utc_maps_item_hashtable_foreach_p, i);
		__test_add_func_load("/api/item_hashtable/foreach_n",
				     utc_maps_item_hashtable_foreach_n, i);
	}
#endif
}

static void __add_maps_data_structure_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */
	/* API: Data Structures: Coordinates */
	__test_add_func_load("/api/coordinates/create_p",
		utc_maps_coordinates_create_p, i);
	__test_add_func_load("/api/coordinates/create_n",
		utc_maps_coordinates_create_n, i);
	__test_add_func_load("/api/coordinates/destroy_p",
		utc_maps_coordinates_destroy_p, i);
	__test_add_func_load("/api/coordinates/destroy_n",
		utc_maps_coordinates_destroy_n, i);
	__test_add_func_load("/api/coordinates/clone_p",
		utc_maps_coordinates_clone_p, i);
	__test_add_func_load("/api/coordinates/clone_n",
		utc_maps_coordinates_clone_n, i);
	__test_add_func_load("/api/coordinates/latitude_p",
		utc_maps_coordinates_latitude_p, i);
	__test_add_func_load("/api/coordinates/latitude_n",
		utc_maps_coordinates_latitude_n, i);
	__test_add_func_load("/api/coordinates/longitude_p",
		utc_maps_coordinates_longitude_p, i);
	__test_add_func_load("/api/coordinates/longitude_n",
		utc_maps_coordinates_longitude_n, i);

#if _MAPS_COORDS_3D
	__test_add_func_load("/api/coordinates/altitude_p",
		utc_maps_coordinates_altitude_p, i);
	__test_add_func_load("/api/coordinates/altitude_n",
		utc_maps_coordinates_altitude_n, i);
#endif

	/* API: Data Structures: Area */
	__test_add_func_load("/api/area/create_rectangle_p",
		utc_maps_area_create_rectangle_p, i);
	__test_add_func_load("/api/area/create_rectangle_n",
		utc_maps_area_create_rectangle_n, i);
	__test_add_func_load("/api/area/create_circle_p",
		utc_maps_area_create_circle_p, i);
	__test_add_func_load("/api/area/create_circle_n",
		utc_maps_area_create_circle_n, i);
	__test_add_func_load("/api/area/destroy_p", utc_maps_area_destroy_p, i);
	__test_add_func_load("/api/area/destroy_n", utc_maps_area_destroy_n, i);
	__test_add_func_load("/api/area/clone_p", utc_maps_area_clone_p, i);
	__test_add_func_load("/api/area/clone_n", utc_maps_area_clone_n, i);
#endif
}

static void __add_geocode_data_structure_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */
	/* API: Data Structures: Address */
	__test_add_func_load("/api/maps_address/new_p",
		utc_maps_address_create_p, i);
	__test_add_func_load("/api/maps_address/new_n",
		utc_maps_address_create_n, i);
	__test_add_func_load("/api/maps_address/clone_p",
		utc_maps_address_clone_p, i);
	__test_add_func_load("/api/maps_address/clone_n",
		utc_maps_address_clone_n, i);
	__test_add_func_load("/api/maps_address/building_number_p",
		utc_maps_address_building_number_p, i);
	__test_add_func_load("/api/maps_address/building_number_n",
		utc_maps_address_building_number_n, i);
	__test_add_func_load("/api/maps_address/street_p",
		utc_maps_address_street_p, i);
	__test_add_func_load("/api/maps_address/street_n",
		utc_maps_address_street_n, i);
	__test_add_func_load("/api/maps_address/district_p",
		utc_maps_address_district_p, i);
	__test_add_func_load("/api/maps_address/district_n",
		utc_maps_address_district_n, i);
	__test_add_func_load("/api/maps_address/city_p",
		utc_maps_address_city_p, i);
	__test_add_func_load("/api/maps_address/city_n",
		utc_maps_address_city_n, i);
	__test_add_func_load("/api/maps_address/state_p",
		utc_maps_address_state_p, i);
	__test_add_func_load("/api/maps_address/state_n",
		utc_maps_address_state_n, i);
	__test_add_func_load("/api/maps_address/country_p",
		utc_maps_address_country_p, i);
	__test_add_func_load("/api/maps_address/country_n",
		utc_maps_address_country_n, i);
	__test_add_func_load("/api/maps_address/country_code_p",
		utc_maps_address_country_code_p, i);
	__test_add_func_load("/api/maps_address/country_code_n",
		utc_maps_address_country_code_n, i);
	__test_add_func_load("/api/maps_address/county_p",
		utc_maps_address_county_p, i);
	__test_add_func_load("/api/maps_address/county_n",
		utc_maps_address_county_n, i);
	__test_add_func_load("/api/maps_address/postal_code_p",
		utc_maps_address_postal_code_p, i);
	__test_add_func_load("/api/maps_address/postal_code_n",
		utc_maps_address_postal_code_n, i);
	__test_add_func_load("/api/maps_address/freetext_p",
		utc_maps_address_destroytext_p, i);
	__test_add_func_load("/api/maps_address/freetext_n",
		utc_maps_address_destroytext_n, i);
#endif
}

static void __add_place_data_structure_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */
	/* API: Data Structures: Place: Category */
	__test_add_func_load("/api/place/category/create_p",
		utc_maps_place_category_create_p, i);
	__test_add_func_load("/api/place/category/create_n",
		utc_maps_place_category_create_n, i);
	__test_add_func_load("/api/place/category/clone_p",
		utc_maps_place_category_clone_p, i);
	__test_add_func_load("/api/place/category/clone_n",
		utc_maps_place_category_clone_n, i);
	__test_add_func_load("/api/place/category/name_p",
		utc_maps_place_category_name_p, i);
	__test_add_func_load("/api/place/category/name_n",
		utc_maps_place_category_name_n, i);
	__test_add_func_load("/api/place/category/id_p",
		utc_maps_place_category_id_p, i);
	__test_add_func_load("/api/place/category/id_n",
		utc_maps_place_category_id_n, i);
	__test_add_func_load("/api/place/category/url_p",
		utc_maps_place_category_url_p, i);
	__test_add_func_load("/api/place/category/url_n",
		utc_maps_place_category_url_n, i);

	/* API: Data Structures: Place: Attribute */
	__test_add_func_load("/api/place/attribute/create_p",
		utc_maps_place_attribute_create_p, i);
	__test_add_func_load("/api/place/attribute/create_n",
		utc_maps_place_attribute_create_n, i);
	__test_add_func_load("/api/place/attribute/clone_p",
		utc_maps_place_attribute_clone_p, i);
	__test_add_func_load("/api/place/attribute/clone_n",
		utc_maps_place_attribute_clone_n, i);
	__test_add_func_load("/api/place/attribute/id_p",
		utc_maps_place_attribute_id_p, i);
	__test_add_func_load("/api/place/attribute/id_n",
		utc_maps_place_attribute_id_n, i);
	__test_add_func_load("/api/place/attribute/label_p",
		utc_maps_place_attribute_label_p, i);
	__test_add_func_load("/api/place/attribute/label_n",
		utc_maps_place_attribute_label_n, i);
	__test_add_func_load("/api/place/attribute/text_p",
		utc_maps_place_attribute_text_p, i);
	__test_add_func_load("/api/place/attribute/text_n",
		utc_maps_place_attribute_text_n, i);

	/* API: Data Structures: Place: Contact */
	__test_add_func_load("/api/place/contact/create_p",
		utc_maps_place_contact_create_p, i);
	__test_add_func_load("/api/place/contact/create_n",
		utc_maps_place_contact_create_n, i);
	__test_add_func_load("/api/place/contact/clone_p",
		utc_maps_place_contact_clone_p, i);
	__test_add_func_load("/api/place/contact/clone_n",
		utc_maps_place_contact_clone_n, i);
	__test_add_func_load("/api/place/contact/label_p",
		utc_maps_place_contact_label_p, i);
	__test_add_func_load("/api/place/contact/label_n",
		utc_maps_place_contact_label_n, i);
	__test_add_func_load("/api/place/contact/type_p",
		utc_maps_place_contact_type_p, i);
	__test_add_func_load("/api/place/contact/type_n",
		utc_maps_place_contact_type_n, i);
	__test_add_func_load("/api/place/contact/value_p",
		utc_maps_place_contact_value_p, i);
	__test_add_func_load("/api/place/contact/value_n",
		utc_maps_place_contact_value_n, i);

	/* API: Data Structures: Place: Editorial */
	__test_add_func_load("/api/place/editorial/create_p",
		utc_maps_place_editorial_create_p, i);
	__test_add_func_load("/api/place/editorial/create_n",
		utc_maps_place_editorial_create_n, i);
	__test_add_func_load("/api/place/editorial/clone_p",
		utc_maps_place_editorial_clone_p, i);
	__test_add_func_load("/api/place/editorial/clone_n",
		utc_maps_place_editorial_clone_n, i);
	__test_add_func_load("/api/place/editorial/description_p",
		utc_maps_place_editorial_description_p, i);
	__test_add_func_load("/api/place/editorial/description_n",
		utc_maps_place_editorial_description_n, i);
	__test_add_func_load("/api/place/editorial/language_p",
		utc_maps_place_editorial_language_p, i);
	__test_add_func_load("/api/place/editorial/language_n",
		utc_maps_place_editorial_language_n, i);
	__test_add_func_load("/api/place/editorial/media_p",
		utc_maps_place_editorial_media_p, i);
	__test_add_func_load("/api/place/editorial/media_n",
		utc_maps_place_editorial_media_n, i);

	/* API: Data Structures: Place: Link Object */
	__test_add_func_load("/api/place/link_object/create_p",
		utc_maps_place_link_object_create_p, i);
	__test_add_func_load("/api/place/link_object/create_n",
		utc_maps_place_link_object_create_n, i);
	__test_add_func_load("/api/place/link_object/clone_p",
		utc_maps_place_link_object_clone_p, i);
	__test_add_func_load("/api/place/link_object/clone_n",
		utc_maps_place_link_object_clone_n, i);
	__test_add_func_load("/api/place/link_object/id_p",
		utc_maps_place_link_object_id_p, i);
	__test_add_func_load("/api/place/link_object/id_n",
		utc_maps_place_link_object_id_n, i);
	__test_add_func_load("/api/place/link_object/name_p",
		utc_maps_place_link_object_name_p, i);
	__test_add_func_load("/api/place/link_object/name_n",
		utc_maps_place_link_object_name_n, i);
	__test_add_func_load("/api/place/link_object/type_p",
		utc_maps_place_link_object_type_p, i);
	__test_add_func_load("/api/place/link_object/type_n",
		utc_maps_place_link_object_type_n, i);
	__test_add_func_load("/api/place/link_object/string_p",
		utc_maps_place_link_object_string_p, i);
	__test_add_func_load("/api/place/link_object/string_n",
		utc_maps_place_link_object_string_n, i);

	/* API: Data Structures: Place: Media */
	__test_add_func_load("/api/place/media/create_p",
		utc_maps_place_media_create_p, i);
	__test_add_func_load("/api/place/media/create_n",
		utc_maps_place_media_create_n, i);
	__test_add_func_load("/api/place/media/clone_p",
		utc_maps_place_media_clone_p, i);
	__test_add_func_load("/api/place/media/clone_n",
		utc_maps_place_media_clone_n, i);
	__test_add_func_load("/api/place/media/attribution_p",
		utc_maps_place_media_attribution_p, i);
	__test_add_func_load("/api/place/media/attribution_n",
		utc_maps_place_media_attribution_n, i);
	__test_add_func_load("/api/place/media/supplier_p",
		utc_maps_place_media_supplier_p, i);
	__test_add_func_load("/api/place/media/supplier_n",
		utc_maps_place_media_supplier_n, i);
	__test_add_func_load("/api/place/media/via_p",
		utc_maps_place_media_via_p, i);
	__test_add_func_load("/api/place/media/via_n",
		utc_maps_place_media_via_n, i);

	/* API: Data Structures: Place: URL */
	__test_add_func_load("/api/place/URL/create_p",
		utc_maps_place_url_create_p, i);
	__test_add_func_load("/api/place/URL/create_n",
		utc_maps_place_url_create_n, i);
	__test_add_func_load("/api/place/URL/clone_p",
		utc_maps_place_url_clone_p, i);
	__test_add_func_load("/api/place/URL/clone_n",
		utc_maps_place_url_clone_n, i);
	__test_add_func_load("/api/place/URL/path_p", utc_maps_place_url_path_p,
		i);
	__test_add_func_load("/api/place/URL/path_n", utc_maps_place_url_path_n,
		i);
	__test_add_func_load("/api/place/URL/description_p",
		utc_maps_place_url_description_p, i);
	__test_add_func_load("/api/place/URL/description_n",
		utc_maps_place_url_description_n, i);

	/* API: Data Structures: Place: Review */
	__test_add_func_load("/api/place/review/create_p",
		utc_maps_place_review_create_p, i);
	__test_add_func_load("/api/place/review/create_n",
		utc_maps_place_review_create_n, i);
	__test_add_func_load("/api/place/review/clone_p",
		utc_maps_place_review_clone_p, i);
	__test_add_func_load("/api/place/review/clone_n",
		utc_maps_place_review_clone_n, i);
	__test_add_func_load("/api/place/review/date_p",
		utc_maps_place_review_date_p, i);
	__test_add_func_load("/api/place/review/date_n",
		utc_maps_place_review_date_n, i);
	__test_add_func_load("/api/place/review/title_p",
		utc_maps_place_review_title_p, i);
	__test_add_func_load("/api/place/review/title_n",
		utc_maps_place_review_title_n, i);
	__test_add_func_load("/api/place/review/rating_p",
		utc_maps_place_review_rating_p, i);
	__test_add_func_load("/api/place/review/rating_n",
		utc_maps_place_review_rating_n, i);
	__test_add_func_load("/api/place/review/description_p",
		utc_maps_place_review_description_p, i);
	__test_add_func_load("/api/place/review/description_n",
		utc_maps_place_review_description_n, i);
	__test_add_func_load("/api/place/review/language_p",
		utc_maps_place_review_language_p, i);
	__test_add_func_load("/api/place/review/language_n",
		utc_maps_place_review_language_n, i);
	__test_add_func_load("/api/place/review/media_p",
		utc_maps_place_review_media_p, i);
	__test_add_func_load("/api/place/review/media_n",
		utc_maps_place_review_media_n, i);
	__test_add_func_load("/api/place/review/user_link_p",
		utc_maps_place_review_user_link_p, i);
	__test_add_func_load("/api/place/review/user_link_n",
		utc_maps_place_review_user_link_n, i);

	/* API: Data Structures: Place: Rating */
	__test_add_func_load("/api/place/rating/create_p",
		utc_maps_place_rating_create_p, i);
	__test_add_func_load("/api/place/rating/create_n",
		utc_maps_place_rating_create_n, i);
	__test_add_func_load("/api/place/rating/clone_p",
		utc_maps_place_rating_clone_p, i);
	__test_add_func_load("/api/place/rating/clone_n",
		utc_maps_place_rating_clone_n, i);
	__test_add_func_load("/api/place/rating/count_p",
		utc_maps_place_rating_count_p, i);
	__test_add_func_load("/api/place/rating/count_n",
		utc_maps_place_rating_count_n, i);
	__test_add_func_load("/api/place/rating/average_p",
		utc_maps_place_rating_average_p, i);
	__test_add_func_load("/api/place/rating/average_n",
		utc_maps_place_rating_average_n, i);

	/* API: Data Structures: Place: Image */
	__test_add_func_load("/api/place/image/create_p",
		utc_maps_place_image_create_p, i);
	__test_add_func_load("/api/place/image/create_n",
		utc_maps_place_image_create_n, i);
	__test_add_func_load("/api/place/image/clone_p",
		utc_maps_place_image_clone_p, i);
	__test_add_func_load("/api/place/image/clone_n",
		utc_maps_place_image_clone_n, i);
	__test_add_func_load("/api/place/image/id_p", utc_maps_place_image_id_p,
		i);
	__test_add_func_load("/api/place/image/id_n", utc_maps_place_image_id_n,
		i);
	__test_add_func_load("/api/place/image/url_p",
		utc_maps_place_image_url_p, i);
	__test_add_func_load("/api/place/image/url_n",
		utc_maps_place_image_url_n, i);
	__test_add_func_load("/api/place/image/width_p",
		utc_maps_place_image_width_p, i);
	__test_add_func_load("/api/place/image/width_n",
		utc_maps_place_image_width_n, i);
	__test_add_func_load("/api/place/image/height_p",
		utc_maps_place_image_height_p, i);
	__test_add_func_load("/api/place/image/height_n",
		utc_maps_place_image_height_n, i);
	__test_add_func_load("/api/place/image/user_link_p",
		utc_maps_place_image_user_link_p, i);
	__test_add_func_load("/api/place/image/user_link_n",
		utc_maps_place_image_user_link_n, i);
	__test_add_func_load("/api/place/image/media_p",
		utc_maps_place_image_media_p, i);
	__test_add_func_load("/api/place/image/media_n",
		utc_maps_place_image_media_n, i);

	/* API: Data Structures: Maps: Place: Filter */
	__test_add_func_load("/api/place/filter/create_p",
		utc_maps_place_filter_create_p, i);
	__test_add_func_load("/api/place/filter/create_n",
		utc_maps_place_filter_create_n, i);
	__test_add_func_load("/api/place/filter/clone_p",
		utc_maps_place_filter_clone_p, i);
	__test_add_func_load("/api/place/filter/clone_n",
		utc_maps_place_filter_clone_n, i);
	__test_add_func_load("/api/place/filter/set_p",
		utc_maps_place_filter_set_p, i);
	__test_add_func_load("/api/place/filter/set_n",
		utc_maps_place_filter_set_n, i);

	/* API: Data Structures: Place */
	__test_add_func_load("/api/place/create_p", utc_maps_place_create_p, i);
	__test_add_func_load("/api/place/create_n", utc_maps_place_create_n, i);
	__test_add_func_load("/api/place/clone_p", utc_maps_place_clone_p, i);
	__test_add_func_load("/api/place/clone_n", utc_maps_place_clone_n, i);
	__test_add_func_load("/api/place/set_id_p", utc_maps_place_set_id_p, i);
	__test_add_func_load("/api/place/set_id_n", utc_maps_place_set_id_n, i);
	__test_add_func_load("/api/place/set_name_p", utc_maps_place_set_name_p,
		i);
	__test_add_func_load("/api/place/set_name_n", utc_maps_place_set_name_n,
		i);
	__test_add_func_load("/api/place/set_uri_p", utc_maps_place_set_uri_p,
		i);
	__test_add_func_load("/api/place/set_uri_n", utc_maps_place_set_uri_n,
		i);
	__test_add_func_load("/api/place/set_location_p",
		utc_maps_place_set_location_p, i);
	__test_add_func_load("/api/place/set_location_n",
		utc_maps_place_set_location_n, i);
	__test_add_func_load("/api/place/set_category_p",
		utc_maps_place_set_category_p, i);
	__test_add_func_load("/api/place/set_category_n",
		utc_maps_place_set_category_n, i);
	__test_add_func_load("/api/place/set_attribute_p",
		utc_maps_place_set_attribute_p, i);
	__test_add_func_load("/api/place/set_attribute_n",
		utc_maps_place_set_attribute_n, i);
	__test_add_func_load("/api/place/set_contact_p",
		utc_maps_place_set_contacts_p, i);
	__test_add_func_load("/api/place/set_contact_n",
		utc_maps_place_set_contacts_n, i);
	__test_add_func_load("/api/place/set_editorial_p",
		utc_maps_place_set_editorial_p, i);
	__test_add_func_load("/api/place/set_editorial_n",
		utc_maps_place_set_editorial_n, i);
	__test_add_func_load("/api/place/set_image_p",
		utc_maps_place_set_image_p, i);
	__test_add_func_load("/api/place/set_image_n",
		utc_maps_place_set_image_n, i);
	__test_add_func_load("/api/place/set_review_p",
		utc_maps_place_set_review_p, i);
	__test_add_func_load("/api/place/set_review_n",
		utc_maps_place_set_review_n, i);
	__test_add_func_load("/api/place/set_properties_p",
		utc_maps_place_set_properties_p, i);
	__test_add_func_load("/api/place/set_properties_n",
		utc_maps_place_set_properties_n, i);
	__test_add_func_load("/api/place/set_rating_p",
		utc_maps_place_set_rating_p, i);
	__test_add_func_load("/api/place/set_rating_n",
		utc_maps_place_set_rating_n, i);
	__test_add_func_load("/api/place/set_supplier_link_p",
		utc_maps_place_set_supplier_link_p, i);
	__test_add_func_load("/api/place/set_supplier_link_n",
		utc_maps_place_set_supplier_link_n, i);
	__test_add_func_load("/api/place/set_related_link_p",
		utc_maps_place_set_related_link_p, i);
	__test_add_func_load("/api/place/set_related_link_n",
		utc_maps_place_set_related_link_n, i);
	__test_add_func_load("/api/place/foreach_property_p",
		utc_maps_place_foreach_property_p, i);
	__test_add_func_load("/api/place/foreach_property_n",
		utc_maps_place_foreach_property_n, i);
#endif
}

static void __add_route_data_structure_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */
	/* API: Data Structures: Route: Maneuver */
	__test_add_func_load("/api/route/maneuver/create_p",
		utc_maps_route_maneuver_create_p, i);
	__test_add_func_load("/api/route/maneuver/create_n",
		utc_maps_route_maneuver_create_n, i);
	__test_add_func_load("/api/route/maneuver/clone_p",
		utc_maps_route_maneuver_clone_p, i);
	__test_add_func_load("/api/route/maneuver/clone_n",
		utc_maps_route_maneuver_clone_n, i);
	__test_add_func_load("/api/route/maneuver/direction_p",
		utc_maps_route_maneuver_direction_id_p, i);
	__test_add_func_load("/api/route/maneuver/direction_n",
		utc_maps_route_maneuver_direction_id_n, i);
	__test_add_func_load("/api/route/maneuver/turn_type_p",
		utc_maps_route_maneuver_turn_type_p, i);
	__test_add_func_load("/api/route/maneuver/turn_type_n",
		utc_maps_route_maneuver_turn_type_n, i);
	__test_add_func_load("/api/route/maneuver/position_p",
		utc_maps_route_maneuver_position_p, i);
	__test_add_func_load("/api/route/maneuver/position_n",
		utc_maps_route_maneuver_position_n, i);
	__test_add_func_load("/api/route/maneuver/road_name_p",
		utc_maps_route_maneuver_road_name_p, i);
	__test_add_func_load("/api/route/maneuver/road_name_n",
		utc_maps_route_maneuver_road_name_n, i);
	__test_add_func_load("/api/route/maneuver/locale_p",
		utc_maps_route_maneuver_locale_p, i);
	__test_add_func_load("/api/route/maneuver/locale_n",
		utc_maps_route_maneuver_locale_n, i);
	__test_add_func_load("/api/route/maneuver/time_to_next_instruction_p",
		utc_maps_route_maneuver_time_to_next_instruction_p, i);
	__test_add_func_load("/api/route/maneuver/time_to_next_instruction_n",
		utc_maps_route_maneuver_time_to_next_instruction_n, i);
	__test_add_func_load
		("/api/route/maneuver/distance_to_next_instruction_p",
		utc_maps_route_maneuver_distance_to_next_instruction_p, i);
	__test_add_func_load
		("/api/route/maneuver/distance_to_next_instruction_n",
		utc_maps_route_maneuver_distance_to_next_instruction_n, i);
	__test_add_func_load("/api/route/maneuver/instruction_text_p",
		utc_maps_route_maneuver_instruction_text_p, i);
	__test_add_func_load("/api/route/maneuver/instruction_text_n",
		utc_maps_route_maneuver_instruction_text_n, i);

	/* API: Data Structures: Route: Segment */
	__test_add_func_load("/api/route/segment/create_p",
		utc_maps_route_segment_create_p, i);
	__test_add_func_load("/api/route/segment/create_n",
		utc_maps_route_segment_create_n, i);
	__test_add_func_load("/api/route/segment/clone_p",
		utc_maps_route_segment_clone_p, i);
	__test_add_func_load("/api/route/segment/clone_n",
		utc_maps_route_segment_clone_n, i);
	__test_add_func_load("/api/route/segment/origin_p",
		utc_maps_route_segment_origin_p, i);
	__test_add_func_load("/api/route/segment/origin_n",
		utc_maps_route_segment_origin_n, i);
	__test_add_func_load("/api/route/segment/destination_p",
		utc_maps_route_segment_destination_p, i);
	__test_add_func_load("/api/route/segment/destination_n",
		utc_maps_route_segment_destination_n, i);
	__test_add_func_load("/api/route/segment/bounding_box_p",
		utc_maps_route_segment_bounding_box_p, i);
	__test_add_func_load("/api/route/segment/bounding_box_n",
		utc_maps_route_segment_bounding_box_n, i);
	__test_add_func_load("/api/route/segment/distance_p",
		utc_maps_route_segment_distance_p, i);
	__test_add_func_load("/api/route/segment/distance_n",
		utc_maps_route_segment_distance_n, i);
	__test_add_func_load("/api/route/segment/duration_p",
		utc_maps_route_segment_duration_p, i);
	__test_add_func_load("/api/route/segment/duration_n",
		utc_maps_route_segment_duration_n, i);
	__test_add_func_load("/api/route/segment/path_p",
		utc_maps_route_segment_path_p, i);
	__test_add_func_load("/api/route/segment/path_n",
		utc_maps_route_segment_path_n, i);
	__test_add_func_load("/api/route/segment/maneuvers_p",
		utc_maps_route_segment_maneuvers_p, i);
	__test_add_func_load("/api/route/segment/maneuvers_n",
		utc_maps_route_segment_maneuvers_n, i);

	/* API: Data Structures: Route */
	__test_add_func_load("/api/route/create_p", utc_maps_route_create_p, i);
	__test_add_func_load("/api/route/create_n", utc_maps_route_create_n, i);
	__test_add_func_load("/api/route/clone_p", utc_maps_route_clone_p, i);
	__test_add_func_load("/api/route/clone_n", utc_maps_route_clone_n, i);
	__test_add_func_load("/api/route/route_id_p", utc_maps_route_route_id_p,
		i);
	__test_add_func_load("/api/route/route_id_n", utc_maps_route_route_id_n,
		i);
	__test_add_func_load("/api/route/origin_p", utc_maps_route_origin_p, i);
	__test_add_func_load("/api/route/origin_n", utc_maps_route_origin_n, i);
	__test_add_func_load("/api/route/destination_p",
		utc_maps_route_destination_p, i);
	__test_add_func_load("/api/route/destination_n",
		utc_maps_route_destination_n, i);
	__test_add_func_load("/api/route/bounding_box_p",
		utc_maps_route_bounding_box_p, i);
	__test_add_func_load("/api/route/bounding_box_n",
		utc_maps_route_bounding_box_n, i);
	__test_add_func_load("/api/route/transport_mode_p",
		utc_maps_route_transport_mode_p, i);
	__test_add_func_load("/api/route/transport_mode_n",
		utc_maps_route_transport_mode_n, i);
	__test_add_func_load("/api/route/total_distance_p",
		utc_maps_route_total_distance_p, i);
	__test_add_func_load("/api/route/total_distance_n",
		utc_maps_route_total_distance_n, i);
	__test_add_func_load("/api/route/total_duration_p",
		utc_maps_route_total_duration_p, i);
	__test_add_func_load("/api/route/total_duration_n",
		utc_maps_route_total_duration_n, i);
	__test_add_func_load("/api/route/distance_unit_p",
		utc_maps_route_distance_unit_p, i);
	__test_add_func_load("/api/route/distance_unit_n",
		utc_maps_route_distance_unit_n, i);
	__test_add_func_load("/api/route/path_p", utc_maps_route_path_p, i);
	__test_add_func_load("/api/route/path_n", utc_maps_route_path_n, i);
	__test_add_func_load("/api/route/segments_p", utc_maps_route_segments_p,
		i);
	__test_add_func_load("/api/route/segments_n", utc_maps_route_segments_n,
		i);
	__test_add_func_load("/api/route/properties_p",
		utc_maps_route_properties_p, i);
	__test_add_func_load("/api/route/properties_n",
		utc_maps_route_properties_n, i);
#endif
}

static void __add_preference_data_structure_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */
	/* API: Data Structures: Route: Maneuver */
	__test_add_func_load("/api/preference/create_p",
		utc_maps_preference_create_p, i);
	__test_add_func_load("/api/preference/create_n",
		utc_maps_preference_create_n, i);
	__test_add_func_load("/api/preference/clone_p",
		utc_maps_preference_clone_p, i);
	__test_add_func_load("/api/preference/clone_n",
		utc_maps_preference_clone_n, i);
	__test_add_func_load("/api/preference/distance_unit_p",
		utc_maps_preference_distance_unit_p, i);
	__test_add_func_load("/api/preference/distance_unit_n",
		utc_maps_preference_distance_unit_n, i);
	__test_add_func_load("/api/preference/language_p",
		utc_maps_preference_language_p, i);
	__test_add_func_load("/api/preference/language_n",
		utc_maps_preference_language_n, i);
	__test_add_func_load("/api/preference/max_results_p",
		utc_maps_preference_max_results_p, i);
	__test_add_func_load("/api/preference/max_results_n",
		utc_maps_preference_max_results_n, i);
	__test_add_func_load("/api/preference/country_code_p",
		utc_maps_preference_country_code_p, i);
	__test_add_func_load("/api/preference/country_code_n",
		utc_maps_preference_country_code_n, i);
	__test_add_func_load("/api/preference/route_optimization_p",
		utc_maps_preference_route_optimization_p, i);
	__test_add_func_load("/api/preference/route_optimization_n",
		utc_maps_preference_route_optimization_n, i);
	__test_add_func_load("/api/preference/route_transport_mode_p",
		utc_maps_preference_route_transport_mode_p, i);
	__test_add_func_load("/api/preference/route_transport_mode_n",
		utc_maps_preference_route_transport_mode_n, i);
	__test_add_func_load("/api/preference/route_feature_weight_p",
		utc_maps_preference_route_feature_weight_p, i);
	__test_add_func_load("/api/preference/route_feature_weight_n",
		utc_maps_preference_route_feature_weight_n, i);
	__test_add_func_load("/api/preference/route_feature_p",
		utc_maps_preference_route_feature_p, i);
	__test_add_func_load("/api/preference/route_feature_n",
		utc_maps_preference_route_feature_n, i);
	__test_add_func_load("/api/preference/property_p",
		utc_maps_preference_property_p, i);
	__test_add_func_load("/api/preference/property_n",
		utc_maps_preference_property_n, i);
	__test_add_func_load("/api/preference/clone_p",
		utc_maps_preference_clone_p, i);
	__test_add_func_load("/api/preference/clone_n",
		utc_maps_preference_clone_n, i);
	__test_add_func_load("/api/preference/clone_p",
		utc_maps_preference_clone_p, i);
	__test_add_func_load("/api/preference/clone_n",
		utc_maps_preference_clone_n, i);

#endif
}

static void __add_maps_api_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */

	if (1) {		/* Create and destroy Maps Service */
		__test_add_func_load("/api/maps_service/create_destroy_p",
			utc_maps_service_create_destroy_p, i);
	}

	if (1) {		/* API: Preferences and Capabilities */
		__test_add_func_load("/api/provider_key/set_get_p",
			utc_maps_provider_key_p, i);
		__test_add_func_load("/api/provider_key/set_get_n",
			utc_maps_provider_key_n, i);
		__test_add_func_load("/api/preference/set_p",
			utc_maps_service_set_preference_p, i);
		__test_add_func_load("/api/preference/set_n",
			utc_maps_service_set_preference_n, i);
		__test_add_func_load("/api/get_provider_capabilities_p",
			utc_maps_service_provider_is_service_supported_p, i);
		__test_add_func_load("/api/get_provider_capabilities_n",
			utc_maps_service_provider_is_service_supported_n, i);
		__test_add_func_load("/api/service_support_feature_p",
			utc_maps_service_provider_is_data_supported_p, i);
		__test_add_func_load("/api/service_support_feature_n",
			utc_maps_service_provider_is_data_supported_n, i);
	}

	if (1) {		/* API: Geocode */
		__test_add_func_load("/api/geocode_p",
			utc_maps_service_geocode_p, i);
		__test_add_func_load("/api/geocode_n",
			utc_maps_service_geocode_n, i);
		__test_add_func_load("/api/geocode_inside_bounds_p",
			utc_maps_service_geocode_inside_area_p, i);
		__test_add_func_load("/api/geocode_inside_bounds_n",
			utc_maps_service_geocode_inside_area_n, i);
		__test_add_func_load("/api/geocode_by_structured_address_p",
			utc_maps_service_geocode_by_structured_address_p, i);
		__test_add_func_load("/api/geocode_by_structured_address_n",
			utc_maps_service_geocode_by_structured_address_n, i);
		__test_add_func_load("/api/reverse_geocode_p",
			utc_maps_service_reverse_geocode_p, i);
		__test_add_func_load("/api/reverse_geocode_n",
			utc_maps_service_reverse_geocode_n, i);
		__test_add_func_load("/api/cancel_geocode_p",
			utc_maps_cancel_geocode_p, i);
		__test_add_func_load("/api/cancel_geocode_p02",
			utc_maps_cancel_geocode_p02, i);
		__test_add_func_load("/api/cancel_geocode_n",
			utc_maps_cancel_geocode_n, i);
		__test_add_func_load("/api/cancel_reverse_geocode_p",
			utc_maps_cancel_reverse_geocode_p, i);
		__test_add_func_load("/api/cancel_reverse_geocode_n",
			utc_maps_cancel_reverse_geocode_n, i);
	}

	if (1) {		/* API: Place */
		__test_add_func_load("/api/place/search_p",
				     utc_maps_service_search_place_p, i);
		__test_add_func_load("/api/place/search_p",
				     utc_maps_service_search_place_p, i);
		__test_add_func_load("/api/place/search_n",
				     utc_maps_service_search_place_n, i);
		__test_add_func_load("/api/place/search_by_area_p",
				utc_maps_service_search_place_by_area_p, i);
		__test_add_func_load("/api/place/search_by_area_n",
				utc_maps_service_search_place_by_area_n, i);
		__test_add_func_load("/api/place/search_by_address_p",
				utc_maps_service_search_place_by_address_p, i);
		__test_add_func_load("/api/place/search_by_address_n",
				utc_maps_service_search_place_by_address_n, i);
		__test_add_func_load("/api/place/cancel_search_p",
				     utc_maps_cancel_place_p, i);
		__test_add_func_load("/api/place/cancel_search_n",
				     utc_maps_cancel_place_n, i);
	}

	if (1) {		/* API: Route */
		__test_add_func_load("/api/route/search_p",
			utc_maps_service_search_route_p, i);
		__test_add_func_load("/api/route/search_n",
			utc_maps_service_search_route_n, i);
		__test_add_func_load("/api/route/search_waypoints_p",
			utc_maps_service_search_route_waypoints_p, i);
		__test_add_func_load("/api/route/search_waypoints_n",
			utc_maps_service_search_route_waypoints_n, i);
		__test_add_func_load("/api/route/cancel_search_p",
			utc_maps_cancel_route_p, i);
		__test_add_func_load("/api/route/cancel_search_n",
			utc_maps_cancel_route_n, i);
	}

	if (1)
		__test_add_func_load("/api/route/search_realistic_p",
			utc_maps_service_search_route_real_p, i);

	if(1) {
		__test_add_func_load("/api/serial_p",
			utc_maps_service_serial_p, i);
	}

#endif
}

static void __add_map_view_api_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */

	if (1) {		/* Create and destroy Maps View */
		__test_add_func_load("/api/map_view/create_destroy_p",
			utc_map_view_create_destroy_p, i);
		__test_add_func_load("/api/map_view/create_destroy_n",
			utc_map_view_create_destroy_n, i);
	}


	if (1) {		/* Maps View Basic Features */
		__test_add_func_load("/api/map_view/show_location_p",
			utc_map_view_show_location_p, i);
		__test_add_func_load("/api/map_view/show_location_n",
			utc_map_view_show_location_n, i);
		__test_add_func_load("/api/map_view/screen_to_geography_p",
			utc_map_view_screen_to_geography_p, i);
		__test_add_func_load("/api/map_view/screen_to_geography_n",
			utc_map_view_screen_to_geography_n, i);
		__test_add_func_load("/api/map_view/geography_to_screen_p",
			utc_map_view_geography_to_screen_p, i);
		__test_add_func_load("/api/map_view/geography_to_screen_n",
			utc_map_view_geography_to_screen_n, i);
	}


	if (1) {		/* Maps View Handling Features */
		__test_add_func_load("/api/map_view/get_panel_p",
			utc_map_view_get_panel_p, i);
		__test_add_func_load("/api/map_view/get_panel_n",
			utc_map_view_get_panel_n, i);
		__test_add_func_load("/api/map_view/resize_p",
			utc_map_view_resize_p, i);
		__test_add_func_load("/api/map_view/resize_n",
			utc_map_view_resize_n, i);
		__test_add_func_load("/api/map_view/get_size_p",
			utc_map_view_get_size_p, i);
		__test_add_func_load("/api/map_view/get_size_n",
			utc_map_view_get_size_n, i);
		__test_add_func_load("/api/map_view/move_p",
			utc_map_view_move_p, i);
		__test_add_func_load("/api/map_view/move_n",
			utc_map_view_move_n, i);
		__test_add_func_load("/api/map_view/get_pos_p",
			utc_map_view_get_pos_p, i);
		__test_add_func_load("/api/map_view/get_pos_n",
			utc_map_view_get_pos_n, i);
		__test_add_func_load("/api/map_view/show_p",
			utc_map_view_show_p, i);
		__test_add_func_load("/api/map_view/show_n",
			utc_map_view_show_n, i);
		__test_add_func_load("/api/map_view/hide_p",
			utc_map_view_hide_p, i);
		__test_add_func_load("/api/map_view/hide_n",
			utc_map_view_hide_n, i);
		__test_add_func_load("/api/map_view/is_visible_p",
			utc_map_view_is_visible_p, i);
		__test_add_func_load("/api/map_view/is_visible_n",
			utc_map_view_is_visible_n, i);
		__test_add_func_load("/api/map_view/zoom_p",
			utc_map_view_zoom_p, i);
		__test_add_func_load("/api/map_view/zoom_n",
			utc_map_view_zoom_n, i);
		__test_add_func_load("/api/map_view/get_zoom_factor_p",
			utc_map_view_get_zoom_factor_p, i);
		__test_add_func_load("/api/map_view/get_zoom_factor_n",
			utc_map_view_get_zoom_factor_n, i);
		__test_add_func_load("/api/map_view/rotate_p",
			utc_map_view_rotate_p, i);
		__test_add_func_load("/api/map_view/rotate_n",
			utc_map_view_rotate_n, i);
		__test_add_func_load("/api/map_view/get_rotation_angle_p",
			utc_map_view_get_rotation_angle_p, i);
		__test_add_func_load("/api/map_view/get_rotation_angle_n",
			utc_map_view_get_rotation_angle_n, i);
	}


	if (1) {		/* Maps View Event Features */
		__test_add_func_load("/api/map_view/set_on_gesture_callback_p",
			utc_map_view_set_on_gesture_callback_p, i);
		__test_add_func_load("/api/map_view/set_on_gesture_callback_n",
			utc_map_view_set_on_gesture_callback_n, i);
		__test_add_func_load("/api/map_view/set_on_zoom_callback_p",
			utc_map_view_set_on_zoom_callback_p, i);
		__test_add_func_load("/api/map_view/set_on_zoom_callback_n",
			utc_map_view_set_on_zoom_callback_n, i);
		__test_add_func_load("/api/map_view/set_on_rotate_callback_p",
			utc_map_view_set_on_rotate_callback_p, i);
		__test_add_func_load("/api/map_view/set_on_rotate_callback_n",
			utc_map_view_set_on_rotate_callback_n, i);
		__test_add_func_load("/api/map_view/set_on_object_callback_p",
			utc_map_view_set_on_object_callback_p, i);
		__test_add_func_load("/api/map_view/set_on_object_callback_n",
			utc_map_view_set_on_object_callback_n, i);
		__test_add_func_load("/api/map_view/set_on_ready_callback_n",
			utc_map_view_set_on_ready_callback_n, i);
		__test_add_func_load("/api/map_view/set_gesture_action_p",
			utc_map_view_set_gesture_action_p, i);
		__test_add_func_load("/api/map_view/set_gesture_action_n",
			utc_map_view_set_gesture_action_n, i);
		__test_add_func_load("/api/map_view/disabel_gesture_p",
			utc_map_view_disable_gesture_p, i);
		__test_add_func_load("/api/map_view/disabel_gesture_n",
			utc_map_view_disable_gesture_n, i);
	}


	if (1) {		/* Maps View Visual Object Features */
		__test_add_func_load("/api/map_view/add_marker_type_p",
			utc_map_view_add_marker_type_p, i);
		__test_add_func_load("/api/map_view/add_marker_type_n",
			utc_map_view_add_marker_type_n, i);
		__test_add_func_load("/api/map_view/add_marker_image_p",
			utc_map_view_add_marker_image_p, i);
		__test_add_func_load("/api/map_view/add_marker_image_n",
			utc_map_view_add_marker_image_n, i);
		__test_add_func_load("/api/map_view/add_route_p",
			utc_map_view_add_route_p, i);
		__test_add_func_load("/api/map_view/add_route_n",
			utc_map_view_add_route_n, i);
		__test_add_func_load("/api/map_view/add_image_p",
			utc_map_view_add_image_p, i);
		__test_add_func_load("/api/map_view/add_image_n",
			utc_map_view_add_image_n, i);
		__test_add_func_load("/api/map_view/add_polyline_p",
			utc_map_view_add_polyline_p, i);
		__test_add_func_load("/api/map_view/add_polyline_n",
			utc_map_view_add_polyline_n, i);
		__test_add_func_load("/api/map_view/add_polygon_p",
			utc_map_view_add_polygon_p, i);
		__test_add_func_load("/api/map_view/add_polygon_n",
			utc_map_view_add_polygon_n, i);
		__test_add_func_load("/api/map_view/add_circle_p",
			utc_map_view_add_circle_p, i);
		__test_add_func_load("/api/map_view/add_circle_n",
			utc_map_view_add_circle_n, i);
		__test_add_func_load("/api/map_view/add_text_p",
			utc_map_view_add_text_p, i);
		__test_add_func_load("/api/map_view/add_text_n",
			utc_map_view_add_text_n, i);
		__test_add_func_load("/api/map_view/add_object_p",
			utc_map_view_add_object_p, i);
		__test_add_func_load("/api/map_view/add_object_n",
			utc_map_view_add_object_n, i);
		__test_add_func_load("/api/map_view/add_objects_p",
			utc_map_view_add_objects_p, i);
		__test_add_func_load("/api/map_view/add_objects_n",
			utc_map_view_add_objects_n, i);
		__test_add_func_load("/api/map_view/remove_object_p",
			utc_map_view_remove_object_p, i);
		__test_add_func_load("/api/map_view/remove_object_n",
			utc_map_view_remove_object_n, i);
		__test_add_func_load("/api/map_view/foreach_object_p",
			utc_map_view_foreach_object_p, i);
		__test_add_func_load("/api/map_view/foreach_object_n",
			utc_map_view_foreach_object_n, i);
	}
#endif
}

static void __add_map_object_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */

		if(1) {		/* Maps View Object */

			/* Common Features */
		__test_add_func_load("/api/map_object/create_destroy_p",
				     utc_map_object_create_p, i);
		__test_add_func_load("/api/map_object/create_destroy_n",
				     utc_map_object_create_n, i);
		__test_add_func_load("/api/map_object/get_type_p",
				     utc_map_object_get_type_p, i);
		__test_add_func_load("/api/map_object/get_type_n",
				     utc_map_object_get_type_n, i);
		__test_add_func_load("/api/map_object/move_p",
				     utc_map_object_move_p, i);
		__test_add_func_load("/api/map_object/move_n",
				     utc_map_object_move_n, i);
		__test_add_func_load("/api/map_object/set_visible_p",
				     utc_map_object_set_visible_p, i);
		__test_add_func_load("/api/map_object/set_visible_n",
				     utc_map_object_set_visible_n, i);
		__test_add_func_load("/api/map_object/get_visible_p",
				     utc_map_object_get_visible_p, i);
		__test_add_func_load("/api/map_object/get_visible_n",
				     utc_map_object_get_visible_n, i);
		__test_add_func_load("/api/map_object/hit_test_p",
				     utc_map_object_hit_test_p, i);
		__test_add_func_load("/api/map_object/hit_test_n",
				     utc_map_object_hit_test_n, i);

		/* Layer */
		__test_add_func_load("/api/map_object/group/add_object_p",
				     utc_map_object_group_add_object_p, i);
		__test_add_func_load("/api/map_object/group/add_object_n",
				     utc_map_object_group_add_object_n, i);
		__test_add_func_load("/api/map_object/group/set_objects_p",
			     utc_map_object_group_set_objects_p, i);
		__test_add_func_load("/api/map_object/group/set_objects_n",
			     utc_map_object_group_set_objects_n, i);
		__test_add_func_load("/api/map_object/group/rem_object_p",
			     utc_map_object_group_remove_object_p, i);
		__test_add_func_load("/api/map_object/group/rem_object_n",
			     utc_map_object_group_remove_object_n, i);
		__test_add_func_load("/api/map_object/group/foreach_p",
			     utc_map_object_group_foreach_object_p, i);
		__test_add_func_load("/api/map_object/group/foreach_n",
			     utc_map_object_group_foreach_object_n, i);

		/* Poly: common for polygon and polyline */
		__test_add_func_load("/api/map_object/poly/add_point_p",
				     utc_map_object_poly_add_point_p, i);
		__test_add_func_load("/api/map_object/poly/add_point_n",
				     utc_map_object_poly_add_point_n, i);
		__test_add_func_load("/api/map_object/poly/set_points_p",
				     utc_map_object_poly_set_points_p, i);
		__test_add_func_load("/api/map_object/poly/set_points_n",
				     utc_map_object_poly_set_points_n, i);
		__test_add_func_load("/api/map_object/poly/foreach_p",
				utc_map_object_poly_foreach_point_p, i);
		__test_add_func_load("/api/map_object/poly/foreach_n",
				utc_map_object_poly_foreach_point_n, i);

		/* Polyline */
		__test_add_func_load("/api/map_object/polyline/set_clr_p",
				utc_map_object_polyline_set_color_p, i);
		__test_add_func_load("/api/map_object/polyline/set_clr_n",
				utc_map_object_polyline_set_color_n, i);
		__test_add_func_load("/api/map_object/polyline/get_clr_p",
				utc_map_object_polyline_get_color_p, i);
		__test_add_func_load("/api/map_object/polyline/get_clr_n",
				utc_map_object_polyline_get_color_n, i);
		__test_add_func_load("/api/map_object/polyline/set_wdt_p",
				utc_map_object_polyline_set_width_p, i);
		__test_add_func_load("/api/map_object/polyline/set_wdt_n",
				utc_map_object_polyline_set_width_n, i);
		__test_add_func_load("/api/map_object/polyline/get_wdt_p",
				utc_map_object_polyline_get_width_p, i);
		__test_add_func_load("/api/map_object/polyline/get_wdt_n",
				utc_map_object_polyline_get_width_n, i);

		/* Polygon */
		__test_add_func_load("/api/map_object/polygon/set_clr_p",
			utc_map_object_polygon_set_fill_color_p, i);
		__test_add_func_load("/api/map_object/polygon/set_clr_n",
			utc_map_object_polygon_set_fill_color_n, i);
		__test_add_func_load("/api/map_object/polygon/get_clr_p",
			utc_map_object_polygon_get_fill_color_p, i);
		__test_add_func_load("/api/map_object/polygon/get_clr_n",
			utc_map_object_polygon_get_fill_color_n, i);

		/* Marker */
		__test_add_func_load("/api/map_object/marker/set_coords_p",
			utc_map_object_marker_set_coordinates_p, i);
		__test_add_func_load("/api/map_object/marker/set_coords_n",
			utc_map_object_marker_set_coordinates_n, i);
		__test_add_func_load("/api/map_object/marker/get_coords_p",
			utc_map_object_marker_get_coordinates_p, i);
		__test_add_func_load("/api/map_object/marker/get_coords_n",
			utc_map_object_marker_get_coordinates_n, i);
		__test_add_func_load("/api/map_object/marker/set_scrsz_p",
			utc_map_object_marker_set_screen_size_p, i);
		__test_add_func_load("/api/map_object/marker/set_scrsz_n",
			utc_map_object_marker_set_screen_size_n, i);
		__test_add_func_load("/api/map_object/marker/get_scrsz_p",
			utc_map_object_marker_get_screen_size_p, i);
		__test_add_func_load("/api/map_object/marker/get_scrsz_n",
			utc_map_object_marker_get_screen_size_n, i);
		__test_add_func_load("/api/map_object/marker/set_file_p",
			utc_map_object_marker_set_file_path_p, i);
		__test_add_func_load("/api/map_object/marker/set_file_n",
			utc_map_object_marker_set_file_path_n, i);
		__test_add_func_load("/api/map_object/marker/get_file_p",
			utc_map_object_marker_get_image_file_p, i);
		__test_add_func_load("/api/map_object/marker/get_file_n",
			utc_map_object_marker_get_image_file_n, i);
		__test_add_func_load("/api/map_object/marker/set_type_p",
				     utc_map_object_marker_set_type_p, i);
		__test_add_func_load("/api/map_object/marker/set_type_n",
				     utc_map_object_marker_set_type_n, i);
		__test_add_func_load("/api/map_object/marker/get_type_p",
				     utc_map_object_marker_get_type_p, i);
		__test_add_func_load("/api/map_object/marker/get_type_n",
				     utc_map_object_marker_get_type_n, i);
		__test_add_func_load("/api/map_object/marker/set_text_p",
				     utc_map_object_marker_set_text_p, i);
		__test_add_func_load("/api/map_object/marker/set_text_n",
				     utc_map_object_marker_set_text_n, i);
		__test_add_func_load("/api/map_object/marker/get_text_p",
				     utc_map_object_marker_get_text_p, i);
		__test_add_func_load("/api/map_object/marker/get_text_n",
				     utc_map_object_marker_get_text_n, i);
	}

#endif
}

static void __add_map_event_data_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */
	if (1) {
		/* Common Features */
		__test_add_func_load("/api/map_event_data/create_destroy_p",
				     utc_map_event_data_create_p, i);
		__test_add_func_load("/api/map_event_data/create_destroy_n",
				     utc_map_event_data_create_n, i);

		__test_add_func_load("/api/map_event_data/clone_p",
				     utc_map_event_data_clone_p, i);
		__test_add_func_load("/api/map_event_data/clone_n",
				     utc_map_event_data_clone_n, i);

		__test_add_func_load("/api/map_event_data/get_type_p",
				     utc_map_event_data_get_type_p, i);
		__test_add_func_load("/api/map_event_data/get_type_n",
				     utc_map_event_data_get_type_n, i);

		__test_add_func_load("/api/map_event_data/get_gesture_type_p",
				     utc_map_event_data_get_gesture_type_p, i);
		__test_add_func_load("/api/map_event_data/get_gesture_type_n",
				     utc_map_event_data_get_gesture_type_n, i);

		__test_add_func_load("/api/map_event_data/get_action_type_p",
				     utc_map_event_data_get_action_type_p, i);
		__test_add_func_load("/api/map_event_data/get_action_type_n",
				     utc_map_event_data_get_action_type_n, i);

		__test_add_func_load("/api/map_event_data/get_center_p",
				     utc_map_event_data_get_center_p, i);
		__test_add_func_load("/api/map_event_data/get_center_n",
				     utc_map_event_data_get_center_n, i);

		__test_add_func_load("/api/map_event_data/get_delta_p",
				     utc_map_event_data_get_delta_p, i);
		__test_add_func_load("/api/map_event_data/get_delta_n",
				     utc_map_event_data_get_delta_n, i);

		__test_add_func_load("/api/map_event_data/get_xy_p",
				     utc_map_event_data_get_xy_p, i);
		__test_add_func_load("/api/map_event_data/get_xy_n",
				     utc_map_event_data_get_xy_n, i);

		__test_add_func_load("/api/map_event_data/get_fingers_p",
				     utc_map_event_data_get_fingers_p, i);
		__test_add_func_load("/api/map_event_data/get_fingers_n",
				     utc_map_event_data_get_fingers_n, i);

		__test_add_func_load("/api/map_event_data/get_zoom_factor_p",
				     utc_map_event_data_get_zoom_factor_p, i);
		__test_add_func_load("/api/map_event_data/get_zoom_factor_n",
				     utc_map_event_data_get_zoom_factor_n, i);

		__test_add_func_load("/api/map_event_data/get_rotation_angle_p",
				     utc_map_event_data_get_rotation_angle_p, i);
		__test_add_func_load("/api/map_event_data/get_rotation_angle_n",
				     utc_map_event_data_get_rotation_angle_n, i);

		__test_add_func_load("/api/map_event_data/get_object_p",
				     utc_map_event_data_get_object_p, i);
		__test_add_func_load("/api/map_event_data/get_object_n",
				     utc_map_event_data_get_object_n, i);

	}
#endif
}

static void __add_view_tools_tests(int i)
{
#ifndef SUPPRESS_VALGRIND_WARNINGS	/* Valgrind warnings!!! */

	if (1) { /* Hit Test */
		__test_add_func_load("/api/view_tools/hit_test_bnd_box_p",
			utc_poly_shape_hit_test_hit_test_bounding_box_p, i);
		__test_add_func_load("/api/view_tools/hit_test_bnd_box_n",
			utc_poly_shape_hit_test_hit_test_bounding_box_n, i);

		__test_add_func_load("/api/view_tools/hit_test_polyline_p",
				utc_poly_shape_hit_test_hit_test_polyline_p, i);
		__test_add_func_load("/api/view_tools/hit_test_polyline_n",
				utc_poly_shape_hit_test_hit_test_polyline_n, i);

		__test_add_func_load("/api/view_tools/hit_test_segment_p",
				utc_poly_shape_hit_test_hit_test_segment_p, i);
		__test_add_func_load("/api/view_tools/hit_test_segment_n",
				utc_poly_shape_hit_test_hit_test_segment_n, i);

		__test_add_func_load("/api/view_tools/hit_test_vertices_p",
				utc_poly_shape_hit_test_hit_test_vertices_p, i);
		__test_add_func_load("/api/view_tools/hit_test_vertices_n",
				utc_poly_shape_hit_test_hit_test_vertices_n, i);

		__test_add_func_load("/api/view_tools/hit_test_pnpoly_p",
				     utc_poly_shape_hit_test_pnpoly_p, i);
		__test_add_func_load("/api/view_tools/hit_test_pnpoly_n",
				     utc_poly_shape_hit_test_pnpoly_n, i);

		__test_add_func_load("/api/view_tools/hit_test_p",
				     utc_poly_shape_hit_test_hit_test_p, i);
		__test_add_func_load("/api/view_tools/hit_test_p2",
				     utc_poly_shape_hit_test_hit_test_p2, i);
		__test_add_func_load("/api/view_tools/hit_test_n",
				     utc_poly_shape_hit_test_hit_test_n, i);
	}

	if (1) { /* Construct Marker */
		__test_add_func_load("/api/view_tools/construct_marker_p",
				     utc_construct_marker_p, i);
		__test_add_func_load("/api/view_tools/construct_marker_n",
				     utc_construct_marker_n, i);
	}

	if (1) { /* Construct Polygon */
		__test_add_func_load("/api/view_tools/construct_polygon_p",
				     utc_construct_polygon_p, i);
		__test_add_func_load("/api/view_tools/construct_polygon_n",
				     utc_construct_polygon_n, i);
	}

	if (1) { /* Construct Polyline */
		__test_add_func_load("/api/view_tools/construct_polyline_p",
				     utc_construct_polyline_p, i);
		__test_add_func_load("/api/view_tools/construct_polyline_n",
				     utc_construct_polyline_n, i);
	}

	if (1) { /* Construct Route */
#ifdef TIZEN_3_0_NEXT_MS
		__test_add_func_load("/api/view_tools/construct_route_p",
				     utc_construct_route_p, i);
		__test_add_func_load("/api/view_tools/construct_route_n",
				     utc_construct_route_n, i);

	/* !!! The old marker constructor will be deleted soom!!! */
#if 0
		__test_add_func_load("/api/view_tools/get_start_mrkr_file_p",
				     utc_get_start_marker_p, i);
		__test_add_func_load("/api/view_tools/get_finish_mrkr_file_p",
				     utc_get_finish_marker_p, i);
		__test_add_func_load("/api/view_tools/get_default_mrkr_file_p",
				     utc_get_default_marker_p, i);

		__test_add_func_load("/api/view_tools/add_marker_p",
				     utc_add_marker_p, i);
		__test_add_func_load("/api/view_tools/add_marker_n",
				     utc_add_marker_n, i);

		__test_add_func_load("/api/view_tools/add_start_marker_p",
				     utc_add_start_marker_p, i);
		__test_add_func_load("/api/view_tools/add_start_marker_n",
				     utc_add_start_marker_n, i);
		__test_add_func_load("/api/view_tools/add_finish_marker_p",
				     utc_add_finish_marker_p, i);
		__test_add_func_load("/api/view_tools/add_finish_marker_n",
				     utc_add_finish_marker_n, i);

		__test_add_func_load("/api/view_tools/add_trajectory_p",
				     utc_add_trajectory_p, i);
		__test_add_func_load("/api/view_tools/add_trajectory_n",
				     utc_add_trajectory_n, i);

		__test_add_func_load("/api/view_tools/add_trajectory_markers_p",
				     utc_add_trajectory_markers_p, i);
		__test_add_func_load("/api/view_tools/add_trajectory_markers_n",
				     utc_add_trajectory_markers_n, i);

		__test_add_func_load("/api/view_tools/add_route_path_p",
				     utc_add_route_path_p, i);
		__test_add_func_load("/api/view_tools/add_route_path_n",
				     utc_add_route_path_n, i);

		__test_add_func_load("/api/view_tools/collect_path_points_p",
				     utc_collect_path_points_p, i);
		__test_add_func_load("/api/view_tools/collect_path_points_n",
				     utc_collect_path_points_n, i);

		__test_add_func_load("/api/view_tools/add_route_segments_p",
				     utc_add_route_segments_p, i);
		__test_add_func_load("/api/view_tools/add_route_segments_n",
				     utc_add_route_segments_n, i);

		__test_add_func_load("/api/view_tools/collect_segments_p",
				     utc_collect_segments_p, i);
		__test_add_func_load("/api/view_tools/collect_segments_n",
				     utc_collect_segments_n, i);
#endif
#endif /* TIZEN_3_0_NEXT_MS */
	}

#endif
}


int main(int argc, char **argv)
{
	/* Testing the tutorial */
	_maps_service_main_tutorial();

	g_print("\n");
	g_print("***********************************************\n");
	g_print("* Maps Service Unit Test Cases\n");
	g_print("***********************************************\n");
	g_print("\n");

#ifdef MAPS_SERVICE_CORRECTNESS_TEST
	g_test_init(&argc, &argv, NULL);
#endif				/*MAPS_SERVICE_CORRECTNESS_TEST */

#ifndef MAPS_SERVICE_LOAD_TEST
	iterations = 1;		/* No needed too much iterations for correctness
				   of memory leak tests */
#endif				/* MAPS_SERVICE_LOAD_TEST */

	long long i = 0;
	while (iterations-- > 0) {

		/* Simple Test */
		if (0)
			__test_add_func_load("/techno/simple_test",
				__utc_simple_test, i);

		/* Plugin Management */
		if (0)
			__add_plugin_tests(i);

		/* Session Management */
		if (0)
			__add_session_tests(i);

		/* API: Extra Data Structures: List */
		if (0)
			__add_extra_data_structure_tests(i);

		/* API: Maps Data Structures: Coordinates, Area */
		if (0)
			__add_maps_data_structure_tests(i);

		/* API: Geocode Data Structures: Address */
		if (0)
			__add_geocode_data_structure_tests(i);

		/* API: Place Data Structures: Category, Attribute, Contact,
		*  Editorial, Link Object, Media, URL, Review, Rating, Image,
		*  Filter Place */
		if (0)
			__add_place_data_structure_tests(i);

		/* API: Route Data Structures: Maneuver, Segment, Route */
		if (0)
			__add_route_data_structure_tests(i);

		/* API: Preference Data Structure */
		if (0)
			__add_preference_data_structure_tests(i);

		/* API: Preference and Capabilities, Geocode, Place, Route */
		if (0)
			__add_maps_api_tests(i);

		/* API: Maps View */
		if (1)
			__add_map_view_api_tests(i);

		/* API: Maps View Object */
		if (1)
			__add_map_object_tests(i);

		/* API: Maps View Event Data */
		if (1)
			__add_map_event_data_tests(i);

		/* View Tools: Poly Data Hit Test, View Object Constructors */
		if (1)
			__add_view_tools_tests(i);


		i++;

#ifdef MAPS_SERVICE_LOAD_TEST
		if (sleep_microseconds > 0)
			g_usleep(sleep_microseconds);
#endif				/* MAPS_SERVICE_LOAD_TEST */
	}

#ifdef MAPS_SERVICE_CORRECTNESS_TEST
	return g_test_run();
#else
	return 0;
#endif				/* MAPS_SERVICE_CORRECTNESS_TEST */
}
