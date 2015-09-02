/* Copyright (c) 2010-2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
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

#include "maps_api_test.h"
#include "maps_service.h"
#include <glib.h>
#include "maps_address.h"
#include "maps_test_utils.h"
#include "maps_object.h"

class test_env
{
	public:
	maps_service_h m;
	maps_preference_h p;
	int rid;
	bool async;
	int iterations;

	enum TEST_PLUGIN
	{
		DUMMY,
		MAPQUEST,
		HERE,
	};
	struct test_plugin_info_s
	{
		/* Maps Provider name, returned by the Plugin */
		const char* provider;
		/* Maps Provider key */
		const char* key;
		/* Does the Plugin have asynchronous requests */
		bool async;
	};
	int test_plugin_type;
	test_plugin_info_s i;
	GMainLoop* mainloop;

	int last_time;

	private:
	 test_plugin_info_s get_plugin_info(int idx) const
	{
		static test_plugin_info_s info[] = {
			/* DUMMY */
			{ "Maps Provider",
				"test_key",
				false },

			/* MAPQUEST */
			{ "Open Street Maps",
				"test_key"
				true },

			/* HERE */
			{ "HERE",
				"test_app_id/test_app_code"
				true }
		};
		 return info[idx];
	}
	public:
	 test_env():m(NULL), p(NULL), rid(-1), async(false), iterations(0),
		mainloop(NULL), last_time(0)
	{

		/* Chose the Plugin for testing */
		test_plugin_type =
			/*DUMMY*/	/* Dummy plugin */
			/*MAPQUEST*/	/* MapQuest plugin */
			HERE	/* Nokia Here Maps */
			;

		i = get_plugin_info(test_plugin_type);

		/* Create an instance of Maps Service */
		int error = maps_service_create(i.provider, &m);
		async = i.async;
		if (error != MAPS_ERROR_NONE)
			__utc_print_error_string(error);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_preference_create(&p);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		/* Set up general preferenes for real plugins */
		error = maps_service_set_provider_key(m, i.key);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_preference_set_max_results(p, 4);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
	~test_env() {
		int error = maps_service_destroy(m);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_item_hashtable_destroy(p);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}

	public:
	void finish_request()
	{
		delete this;
	}
	void finish_response()
	{
		/*g_print("\nfinishing response...\n");*/
		if (async && mainloop)
			test_env::stop_waiting_for_response(this);
		/*g_print("\nfinished response...\n");*/
	}
	private:
	static gboolean stop_waiting_for_response(gpointer data)
	{
		/*g_print("\nquite main loop\n");*/
		/*int milsec = -1;*/
		if (data) {
			test_env* e = (test_env*) data;
			if (e->mainloop) {
				/*milsec = __get_milli_span(e->last_time);*/
				g_main_loop_unref(e->mainloop);
				g_main_loop_quit(e->mainloop);
				e->mainloop = NULL;
			}
		}
		/*g_print("return from main loop: %d msec\n", milsec);*/
		return FALSE;
	}
	public:
	void wait_for_response(void)
	{
		if (!async)
			return;
		last_time = __get_milli_count();
		/*g_print("\nenter main loop\n"); */
		mainloop = g_main_loop_new(NULL, FALSE);
		const int timeout_id =
			g_timeout_add_seconds(15, stop_waiting_for_response,
			this);
		g_main_loop_run(mainloop);
		g_source_remove(timeout_id);
	}
};

/*----------------------------------------------------------------------------*/
/* Create and Destroy */
void utc_maps_service_create_destroy_p(void)
{
	test_env e;
}

/*----------------------------------------------------------------------------*/
/* Keys and properties */
/* */

/*int maps_service_set_provider_key(maps_service_h maps,
* const char* map_key); */
/*int maps_service_get_provider_key(const maps_service_h maps,
* char** map_key); */
void utc_maps_provider_key_p(void)
{
	test_env e;

	int error = maps_service_set_provider_key(e.m, e.i.key);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* obtained_key = NULL;
	error = maps_service_get_provider_key(e.m, &obtained_key);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obtained_key);
	g_assert_cmpstr(obtained_key, ==, e.i.key);
	g_free(obtained_key);
}

void utc_maps_provider_key_n(void)
{
	test_env e;

	int error = maps_service_set_provider_key(NULL, "TEST_KEY");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_set_provider_key(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* obtained_key = NULL;
	error = maps_service_get_provider_key(NULL, &obtained_key);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!obtained_key);

	error = maps_service_get_provider_key(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!obtained_key);
}

/*int maps_service_get_preference(maps_service_h maps,
* maps_preference_h* preference); */
/*int maps_service_set_preference(maps_service_h maps,
* maps_preference_h preference); */
void utc_maps_service_set_preference_p(void)
{
	test_env e;

	int error =
		maps_item_hashtable_set_string(e.p, "MAP_SERVICE_PREF_LANGUAGE",
		"value");

	/* test start --------------------------------- */
	error = maps_service_set_preference(e.m, e.p);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/* test finish --------------------------------- */
}

void utc_maps_service_set_preference_n(void)
{
	test_env e;

	int error =
		maps_item_hashtable_set_string(e.p, "MAP_SERVICE_PREF_LANGUAGE",
		"value");

	/* test start --------------------------------- */
	error = maps_service_set_preference(NULL, e.p);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_set_preference(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */
}

/*int maps_service_provider_is_service_supported(const maps_service_h maps, */
void utc_maps_service_provider_is_service_supported_p(void)
{
	test_env e;

	bool supported = false;
	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_GEOCODE, &supported), ==, MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_GEOCODE_INSIDE_AREA, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_GEOCODE_BY_STRUCTURED_ADDRESS, &supported),
		==, MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_REVERSE_GEOCODE, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_SEARCH_PLACE, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_SEARCH_PLACE_BY_AREA, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_SEARCH_PLACE_BY_ADDRESS, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_SEARCH_ROUTE, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_SEARCH_ROUTE_WAYPOINTS, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_CANCEL_REQUEST, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);
}

void utc_maps_service_provider_is_service_supported_n(void)
{
	test_env e;

	bool supported = false;
	g_assert_cmpint(maps_service_provider_is_service_supported(NULL,
			MAPS_SERVICE_GEOCODE, &supported), ==,
		MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!supported);

	/*g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			(maps_service_e) (-1), &supported), ==,
		MAPS_ERROR_NOT_SUPPORTED);*/
	int error = maps_service_provider_is_service_supported(e.m,
			(maps_service_e) (-1), &supported);
	if (error != MAPS_ERROR_INVALID_PARAMETER)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!supported);

	g_assert_cmpint(maps_service_provider_is_service_supported(e.m,
			MAPS_SERVICE_GEOCODE, NULL), ==,
		MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!supported);
}

void utc_maps_service_provider_is_data_supported_p(void)
{
	test_env e;

	bool supported = false;

	g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
			MAPS_PLACE_ADDRESS, &supported), ==, MAPS_ERROR_NONE);
	g_assert(supported);

	if (e.test_plugin_type != test_env::MAPQUEST) {
		g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
				MAPS_PLACE_RATING, &supported), ==,
			MAPS_ERROR_NONE);
		g_assert(supported);

		g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
				MAPS_PLACE_CATEGORIES, &supported), ==,
			MAPS_ERROR_NONE);
		g_assert(supported);

		g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
				MAPS_PLACE_ATTRIBUTES, &supported), ==,
			MAPS_ERROR_NONE);
		g_assert(supported);

		g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
				MAPS_PLACE_CONTACTS, &supported), ==,
			MAPS_ERROR_NONE);
		g_assert(supported);

		g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
				MAPS_PLACE_EDITORIALS, &supported), ==,
			MAPS_ERROR_NONE);
		g_assert(supported);

		g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
				MAPS_PLACE_REVIEWS, &supported), ==,
			MAPS_ERROR_NONE);
		g_assert(supported);

		g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
				MAPS_PLACE_IMAGE, &supported), ==,
			MAPS_ERROR_NONE);
		g_assert(supported);

		g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
				MAPS_PLACE_SUPPLIER, &supported), ==,
			MAPS_ERROR_NONE);
		g_assert(supported);

		g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
				MAPS_PLACE_RELATED, &supported), ==,
			MAPS_ERROR_NONE);
		g_assert(supported);
	}

	g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
			MAPS_ROUTE_PATH, &supported), ==, MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
			MAPS_ROUTE_SEGMENTS_PATH, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);

	g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
			MAPS_ROUTE_SEGMENTS_MANEUVERS, &supported), ==,
		MAPS_ERROR_NONE);
	g_assert(supported);
}

void utc_maps_service_provider_is_data_supported_n(void)
{
	test_env e;

	bool supported = false;
	g_assert_cmpint(maps_service_provider_is_data_supported(NULL,
			MAPS_PLACE_ADDRESS, &supported), ==,
		MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!supported);

	g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
			(maps_service_data_e) (-1), &supported), ==,
		MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!supported);

	g_assert_cmpint(maps_service_provider_is_data_supported(e.m,
			MAPS_PLACE_ADDRESS, NULL), ==,
		MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!supported);
}

/*----------------------------------------------------------------------------*/
/* Geocode */
/* */
/* int maps_service_geocode(maps_service_h maps, const char* address,
*  maps_service_geocode_cb callback, void* user_data, int* request_id); */
/* typedef bool (*maps_service_geocode_cb)(maps_error_e result, int request_id,
*  int index, int total_count, maps_coordinates_h coordinates,
*  void* user_data); */

static bool __utc_maps_service_geocode_cb(maps_error_e result, int request_id,
	int index, int total_count, maps_coordinates_h coordinates,
	void* user_data)
{
/*g_print("\n\n__utc_maps_service_geocode_cb [%d of %d]\n\n", index, total_count);*/

	if (result != MAPS_ERROR_NONE)
		__utc_print_error_string(result);
	g_assert_cmpint(result, ==, MAPS_ERROR_NONE);

	test_env* e = (test_env*) user_data;
	g_assert(e);

	e->iterations++;

	if (e->rid > 0) {
		g_assert_cmpint(e->rid, ==, request_id);
	}

	g_assert(index >= 0);
	g_assert(total_count > 0);

	g_assert(coordinates);

	double latitude = .0, longitude = .0, altitude = .0;
	int error = maps_coordinates_get_latitude(coordinates, &latitude);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_coordinates_get_longitude(coordinates, &longitude);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	g_assert(latitude > -90 || latitude < 90);
	g_assert(longitude > -180 || longitude < 180);
	g_assert(altitude > -100 || altitude < 100);

	error = maps_coordinates_destroy(coordinates);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	if (index == (total_count - 1)) {
		e->finish_response();
	}
	return true;
}

void utc_maps_service_geocode_p(void)
{
	test_env* e = new test_env;

	/* test start --------------------------------- */
	int error =
		maps_service_geocode(e->m, "Seoul", e->p,
		__utc_maps_service_geocode_cb, (void*) e, &e->rid);
	/*int error = maps_service_geocode(e->m, "Berlin", e->p,
	* __utc_maps_service_geocode_cb, (void*)e, &e->rid); */
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	e->wait_for_response();

	g_assert(e->rid > 0);
	g_assert(e->iterations > 0);
	/* test finish --------------------------------- */

	/*if(!e->async) delete e; */
	/*else e->wait_for_response(); */
	e->finish_request();
}

void utc_maps_service_geocode_n(void)
{
	test_env e;

	/* test start --------------------------------- */
	int error =
		maps_service_geocode(NULL, "Seoul", e.p,
		__utc_maps_service_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_geocode(e.m, NULL, e.p,
		__utc_maps_service_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/*error = maps_service_geocode(e.m, "Seoul", NULL,
		__utc_maps_service_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);*/

	error = maps_service_geocode(e.m, "Seoul", e.p, NULL, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_geocode(e.m, "Seoul", e.p,
		__utc_maps_service_geocode_cb, NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */
}

/*int maps_service_geocode_inside_area(maps_service_h maps,
* const char* address, maps_area_h bounds, maps_service_geocode_cb callback,
* void* user_data, int* request_id); */
void utc_maps_service_geocode_inside_area_p(void)
{
	test_env* e = new test_env;

	maps_coordinates_h c1 = NULL;
	/*int error = maps_coordinates_create(38.0, 127.0, &c1); */
	int error = maps_coordinates_create(15.665354, 74.311523, &c1);
	g_assert(c1);

	maps_coordinates_h c2 = NULL;
	/*error = maps_coordinates_create(37.0, 128.0, &c2); */
	error = maps_coordinates_create(10.617418, 79.145508, &c2);
	g_assert(c2);

	maps_area_h bounds = NULL;
	error = maps_area_create_rectangle(c1, c2, &bounds);
	g_assert(bounds);

	/* test start --------------------------------- */
	error = maps_service_geocode_inside_area(e->m,
		/*"Seoul", */
		"Berlin",
		bounds,
		e->p, __utc_maps_service_geocode_cb, (void*) e, &e->rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	e->wait_for_response();

	g_assert(e->rid > 0);
	/* test finish --------------------------------- */

	error = maps_area_destroy(bounds);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_coordinates_destroy(c1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_coordinates_destroy(c2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/*if(!e->async) delete e; */
	e->finish_request();
}

void utc_maps_service_geocode_inside_area_n(void)
{
	test_env e;

	maps_coordinates_h center = NULL;
	int error = maps_coordinates_create(11.1, 22.2, &center);
	g_assert(center);

	maps_area_h bounds = NULL;
	error = maps_area_create_circle(center, 200, &bounds);
	g_assert(bounds);

	/* test start --------------------------------- */
	error = maps_service_geocode_inside_area(NULL, "Seoul", bounds, e.p,
		__utc_maps_service_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_geocode_inside_area(e.m, NULL, bounds, e.p,
		__utc_maps_service_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/*error = maps_service_geocode_inside_area(e.m, "Seoul", bounds, NULL,
		__utc_maps_service_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);*/

	error = maps_service_geocode_inside_area(e.m, "Seoul", bounds, e.p,
		NULL, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_geocode_inside_area(e.m, "Seoul", bounds, e.p,
		__utc_maps_service_geocode_cb, NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */

	error = maps_area_destroy(bounds);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_coordinates_destroy(center);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_service_geocode_by_structured_address_p(void)
{
	test_env* e = new test_env;

	/* TODO: check if MapQuest supports geocoding by structured address */
	if (e->test_plugin_type == test_env::MAPQUEST) {
		delete e;
		return;
	}

	maps_address_h address = NULL;
	int error = maps_address_create(&address);
	g_assert(address);
	error = maps_address_set_city(address, "Prague");
	error = maps_address_set_street(address, "Na Bojisti");
	error = maps_address_set_building_number(address, "1733/12");

	/* test start --------------------------------- */
	error = maps_service_geocode_by_structured_address(e->m,
		address,
		e->p, __utc_maps_service_geocode_cb, (void*) e, &e->rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	e->wait_for_response();

	g_assert(e->rid > 0);
	/* test finish --------------------------------- */

	error = maps_address_destroy(address);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/*if(!e->async) delete e; */
	e->finish_request();
}

void utc_maps_service_geocode_by_structured_address_n(void)
{
	test_env e;

	maps_address_h address = NULL;
	int error = maps_address_create(&address);
	g_assert(address);

	/* test start --------------------------------- */
	error = maps_service_geocode_by_structured_address(NULL, address, e.p,
		__utc_maps_service_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_geocode_by_structured_address(e.m, NULL, e.p,
		__utc_maps_service_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/*error = maps_service_geocode_by_structured_address(e.m, address, NULL,
		__utc_maps_service_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);*/

	error = maps_service_geocode_by_structured_address(e.m, address, e.p,
		NULL, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_geocode_by_structured_address(e.m, address, e.p,
		__utc_maps_service_geocode_cb, NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */

	error = maps_address_destroy(address);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

static void __utc_maps_service_reverse_geocode_cb(maps_error_e result,
	int request_id, int index, int total, maps_address_h address,
	void* user_data)
{

	g_assert_cmpint(result, ==, MAPS_ERROR_NONE);

	g_assert(address);

	int error = 0;

	if (address) {
		g_print("[%s] address\n", __FUNCTION__);

		char *building_number = NULL;
		if (maps_address_get_building_number(address,
				&building_number) == MAPS_ERROR_NONE)
			g_print("\tbuilding number\t: %s\n", building_number);
		g_free(building_number);

		char *street = NULL;
		if (maps_address_get_street(address, &street) == MAPS_ERROR_NONE)
			g_print("\tstreet\t: %s\n", street);
		g_free(street);

		char *district = NULL;
		if (maps_address_get_district(address,
				&district) == MAPS_ERROR_NONE)
			g_print("\tdistrict\t: %s\n", district);
		g_free(district);

		char *city = NULL;
		if (maps_address_get_city(address, &city) == MAPS_ERROR_NONE)
			g_print("\tcity\t: %s\n", city);
		g_free(city);

		char *state = NULL;
		if (maps_address_get_state(address, &state) == MAPS_ERROR_NONE)
			g_print("\tstate\t: %s\n", state);
		g_free(state);

		char *country = NULL;
		if (maps_address_get_country(address,
				&country) == MAPS_ERROR_NONE)
			g_print("\tcountry\t: %s\n", country);
		g_free(country);

		char *country_code = NULL;
		if (maps_address_get_country_code(address,
				&country_code) == MAPS_ERROR_NONE)
			g_print("\tcountry code\t: %s\n", country_code);
		g_free(country_code);

		char *county = NULL;
		if (maps_address_get_county(address, &county) == MAPS_ERROR_NONE)
			g_print("\tcounty\t: %s\n", county);
		g_free(county);

		char *postal_code = NULL;
		if (maps_address_get_postal_code(address,
				&postal_code) == MAPS_ERROR_NONE)
			g_print("\tpostal code\t: %s\n", postal_code);
		g_free(postal_code);

		char *free_text = NULL;
		if (maps_address_get_freetext(address,
				&free_text) == MAPS_ERROR_NONE)
			g_print("\tfreetext\t: %s\n", free_text);
		g_free(free_text);

		error = maps_address_destroy(address);
	}

	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	test_env* e = (test_env*) user_data;
	g_assert(e);

	if (e->rid > 0) {
		g_assert_cmpint(e->rid, ==, request_id);
	}

	g_assert_cmpint(index, ==, 0);
	g_assert_cmpint(total, ==, 1);

	/*if(e->async) delete e; */
	e->finish_response();
}

/*int maps_service_reverse_geocode(maps_service_h maps, double latitude,
* double longitude, maps_service_reverse_geocode_cb callback, void* user_data,
* int* request_id); */
void utc_maps_service_reverse_geocode_p(void)
{
	test_env* e = new test_env;

	/* test start --------------------------------- */
	int error =
		maps_service_reverse_geocode(e->m, 12.944594, 77.554303, e->p,
		__utc_maps_service_reverse_geocode_cb, (void*) e, &e->rid);
	if (error != MAPS_ERROR_NONE)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	e->wait_for_response();

	g_assert(e->rid > 0);
	/* test finish --------------------------------- */

	/*if(!e->async) delete e; */
	e->finish_request();
}

void utc_maps_service_reverse_geocode_n(void)
{
	test_env e;

	/* test start --------------------------------- */
	int error =
		maps_service_reverse_geocode(NULL, 11.1, 22.2, e.p,
		__utc_maps_service_reverse_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/*error = maps_service_reverse_geocode(e.m, 11.1, 22.2, NULL,
		__utc_maps_service_reverse_geocode_cb, NULL, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);*/

	error = maps_service_reverse_geocode(e.m, 11.1, 22.2, e.p,
		__utc_maps_service_reverse_geocode_cb, NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */
}

/*int maps_cancel_geocode(maps_service_h maps, int request_id); */
static bool __utc_maps_cancel_geocode_cb(maps_error_e result, int request_id,
	int index, int total_count, maps_coordinates_h coordinates,
	void* user_data)
{
	return true;
}

void utc_maps_cancel_geocode_p(void)
{
	test_env e;

	maps_item_hashtable_set_string(e.p, "no_need_callback",
		"no_need_callback");

	int error =
		maps_service_geocode(e.m, "Seoul", e.p,
		__utc_maps_cancel_geocode_cb, (void*) &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(e.rid > 0);

	/*error = maps_cancel_geocode(e.m, e.rid); */
	error = maps_service_cancel_request(e.m, e.rid);
	if (error != MAPS_ERROR_NONE)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_cancel_geocode_p02(void)
{
	/* TODO: figure out, why it is failed with MapQuest Plugin */
	test_env e;

	maps_item_hashtable_set_string(e.p, "no_need_callback",
		"no_need_callback");

	for (int i = 0; i < 5; i++) {
		int error =
			maps_service_geocode(e.m, "Seoul", e.p,
			__utc_maps_cancel_geocode_cb, (void*) &e, &e.rid);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(e.rid > 0);

		error = maps_service_cancel_request(e.m, e.rid);
		if (error != MAPS_ERROR_NONE)
			__utc_print_error_string(error);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
}

void utc_maps_cancel_geocode_n(void)
{
	const int error = maps_service_cancel_request(NULL, 1);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int maps_cancel_reverse_geocode(maps_service_h maps, int request_id); */
static void __utc_maps_cancel_reverse_geocode_cb(maps_error_e result,
	int request_id, int index, int total, maps_address_h address,
	void* user_data)
{
	return;
}

void utc_maps_cancel_reverse_geocode_p(void)
{
	test_env e;

	maps_item_hashtable_set_string(e.p, "no_need_callback",
		"no_need_callback");

	int error =
		maps_service_reverse_geocode(e.m, 11.1, 22.2, e.p,
		__utc_maps_cancel_reverse_geocode_cb, (void*) &e, &e.rid);;
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(e.rid > 0);

	error = maps_service_cancel_request(e.m, e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_cancel_reverse_geocode_n(void)
{
	const int error = maps_service_cancel_request(NULL, 1);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
/* Place */
static bool __utc_place_contact_cb(int index, int total,
	maps_place_contact_h contact_h, void* user_data)
{
	if (!contact_h)
		return FALSE;

	maps::place_contact contact(contact_h);

	g_print("contact [%d/%d]\n", index + 1, total);

	maps::string_holder label;
	if (maps_place_contact_get_label(contact, label) == MAPS_ERROR_NONE)
		g_print("\tlabel\t: %s\n", label.str);

	maps::string_holder type;
	if (maps_place_contact_get_type(contact, type) == MAPS_ERROR_NONE)
		g_print("\ttype\t: %s\n", type.str);

	maps::string_holder value;
	if (maps_place_contact_get_value(contact, value) == MAPS_ERROR_NONE)
		g_print("\tvalue\t: %s\n", value.str);

	return TRUE;
}

static bool __utc_place_category_cb(int index, int total,
	maps_place_category_h category_h, void* user_data)
{
	if (!category_h)
		return FALSE;

	maps::place_category category(category_h);

	g_print("category [%d/%d]\n", index + 1, total);

	maps::string_holder name;
	if (maps_place_category_get_name(category, name) == MAPS_ERROR_NONE)
		g_print("\tname\t: %s\n", name.str);

	maps::string_holder id;
	if (maps_place_category_get_id(category, id) == MAPS_ERROR_NONE)
		g_print("\tid\t: %s\n", id.str);

	maps::string_holder url;
	if (maps_place_category_get_url(category, url) == MAPS_ERROR_NONE)
		g_print("\turl\t: %s\n", url.str);

	return TRUE;
}

static bool __utc_place_link_object(const char* tag,
	maps_place_link_object_h link_object_h)
{
	if (!link_object_h)
		return FALSE;

	maps::place_link_object link_object(link_object_h);

	maps::string_holder id;
	if (maps_place_link_object_get_id(link_object, id) == MAPS_ERROR_NONE)
		g_print("%sid\t: %s\n", tag, id.str);

	maps::string_holder name;
	if (maps_place_link_object_get_name(link_object,
			name) == MAPS_ERROR_NONE)
		g_print("%sname\t: %s\n", tag, name.str);

	maps::string_holder string;
	if (maps_place_link_object_get_string(link_object,
			string) == MAPS_ERROR_NONE)
		g_print("%sstring\t: %s\n", tag, string.str);

	maps::string_holder type;
	if (maps_place_link_object_get_type(link_object,
			type) == MAPS_ERROR_NONE)
		g_print("%stype\t: %s\n", tag, type.str);

	return TRUE;
}

static bool __utc_place_image_cb(int index, int total,
	maps_place_image_h image_h, void* user_data)
{
	if (!image_h)
		return FALSE;

	g_print("image [%d/%d]\n", index + 1, total);

	maps::place_image image(image_h);

	maps::string_holder url;
	if (maps_place_image_get_url(image, url) == MAPS_ERROR_NONE)
		g_print("\turl\t: %s\n", url.str);

	maps::string_holder id;
	if (maps_place_image_get_id(image, id) == MAPS_ERROR_NONE)
		g_print("\tid\t: %s\n", id.str);

	int nval = 0;
	if (maps_place_image_get_width(image, &nval) == MAPS_ERROR_NONE
		&& nval > 0)
		g_print("\twidth\t: %d\n", nval);

	if (maps_place_image_get_height(image, &nval) == MAPS_ERROR_NONE
		&& nval > 0)
		g_print("\theight\t: %d\n", nval);

	maps::place_link_object user_link(NULL);
	if (maps_place_image_get_user_link(image, user_link) ==
	    MAPS_ERROR_NONE) {
		g_print("\tuserlink\n");
		__utc_place_link_object("\t\t", user_link.clone());
	}

	maps::place_media media(NULL);
	if (maps_place_image_get_media(image, media) == MAPS_ERROR_NONE) {
		g_print("\tmedia\n");
		maps::string_holder attribution;
		if (maps_place_media_get_attribution(media,
				attribution) == MAPS_ERROR_NONE)
			g_print("\t\tattribution\t: %s\n", attribution.str);

		maps::place_link_object supplier(NULL);
		if (maps_place_media_get_supplier(media,
				supplier) == MAPS_ERROR_NONE) {
			g_print("\t\tsupplier link\n");
			__utc_place_link_object("\t\t\t", supplier.clone());
		}

		maps::place_link_object via(NULL);
		if (maps_place_media_get_via(media, via) == MAPS_ERROR_NONE) {
			g_print("\t\tvia\n");
			__utc_place_link_object("\t\t\t", via.clone());
		}
	}

	return TRUE;
}

static bool __utc_place_editorial_cb(int index, int total,
	maps_place_editorial_h editorial_h, void* user_data)
{
	/* TODO: */
	maps::place_editorial editorial(editorial_h);
	return TRUE;
}

static bool __utc_place_review_cb(int index, int total,
	maps_place_review_h review_h, void* user_data)
{
	/* TODO: */
	maps::place_review review(review_h);
	return TRUE;
}

static bool __utc_maps_service_search_place_cb(maps_error_e error,
	int request_id, int index, int length, maps_place_h place_h,
	void* user_data)
{

	if((error != MAPS_ERROR_NONE) && (error != MAPS_ERROR_NOT_FOUND)) {
		__utc_print_error_string(error);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}

	test_env* e = (test_env*) user_data;
	g_assert(e);

	if (e->rid > 0) {
		g_assert_cmpint(e->rid, ==, request_id);
	}

	g_assert(index >= 0);
	g_assert(length > 0);

	if(error == MAPS_ERROR_NOT_FOUND) {
		g_assert(index == 0);
		g_assert(length == 1);
		e->finish_response();
		return true;
	}


	g_assert(place_h);

	maps::place place(place_h);

	/* Parsing Result Data ------------------------------------- */
	/*g_print("Search Place [%d/%d]\n", index + 1, length);*/

	maps::coordinates coord(NULL);
	if (maps_place_get_location(place, coord) == MAPS_ERROR_NONE) {
		g_print("\tposition\t: %f, %f\n", coord.get_latitude(),
			coord.get_longitude());
	}

	maps::string_holder name;
	if (maps_place_get_name(place, name) == MAPS_ERROR_NONE)
		g_print("\ttitle\t: %s\n", name.str);

	maps::string_holder uri;
	if (maps_place_get_uri(place, uri) == MAPS_ERROR_NONE)
		g_print("\thref\t: %s\n", uri.str);

	maps::string_holder id;
	if (maps_place_get_id(place, id) == MAPS_ERROR_NONE)
		g_print("\tid\t: %s\n", id.str);

	maps::place_rating rating(NULL);
	if (maps_place_get_rating(place, rating) == MAPS_ERROR_NONE) {
		int count = 0;
		double value = .0;
		maps_place_rating_get_average(rating, &value);
		maps_place_rating_get_count(rating, &count);
		g_print("\trating\t: %f\n", value);
		g_print("\tcount\t: %d\n", count);
	}

	maps::address address(NULL);
	if (maps_place_get_address(place, address) == MAPS_ERROR_NONE) {
		g_print("address\n");

		maps::string_holder building_number;
		if (maps_address_get_building_number(address,
				building_number) == MAPS_ERROR_NONE)
			g_print("\tbuilding number\t: %s\n",
				building_number.str);

		maps::string_holder street;
		if (maps_address_get_street(address, street) == MAPS_ERROR_NONE)
			g_print("\tstreet\t: %s\n", street.str);

		maps::string_holder district;
		if (maps_address_get_district(address,
				district) == MAPS_ERROR_NONE)
			g_print("\tdistrict\t: %s\n", district.str);

		maps::string_holder city;
		if (maps_address_get_city(address, city) == MAPS_ERROR_NONE)
			g_print("\tcity\t: %s\n", city.str);

		maps::string_holder state;
		if (maps_address_get_state(address, state) == MAPS_ERROR_NONE)
			g_print("\tstate\t: %s\n", state.str);

		maps::string_holder country;
		if (maps_address_get_country(address,
				country) == MAPS_ERROR_NONE)
			g_print("\tcountry\t: %s\n", country.str);

		maps::string_holder country_code;
		if (maps_address_get_country_code(address,
				country_code) == MAPS_ERROR_NONE)
			g_print("\tcountry code\t: %s\n", country_code.str);

		maps::string_holder county;
		if (maps_address_get_county(address, county) == MAPS_ERROR_NONE)
			g_print("\tcounty\t: %s\n", county.str);

		maps::string_holder postal_code;
		if (maps_address_get_postal_code(address,
				postal_code) == MAPS_ERROR_NONE)
			g_print("\tpostal code\t: %s\n", postal_code.str);

		maps::string_holder free_text;
		if (maps_address_get_freetext(address,
				free_text) == MAPS_ERROR_NONE)
			g_print("\tfreetext\t: %s\n", free_text.str);
	}

	maps_place_foreach_contact(place, __utc_place_contact_cb, user_data);
	maps_place_foreach_category(place, __utc_place_category_cb, user_data);
	maps_place_foreach_image(place, __utc_place_image_cb, user_data);
	maps_place_foreach_editorial(place, __utc_place_editorial_cb,
		user_data);
	maps_place_foreach_review(place, __utc_place_review_cb, user_data);

	/*g_print("\n");*/

	/* End Parsing Result Data --------------------------------- */

	if (index == (length - 1)) {
		/*if(e->async) delete e; */
		e->finish_response();
	}

	return true;
}

/*int maps_service_search_place(maps_service_h maps,
* maps_coordinates_h position, int distance, maps_place_preference_h preference,
* maps_place_filter_h filter, maps_service_search_place_cb callback,
* void* user_data, int* request_id); */
void utc_maps_service_search_place_p(void)
{
	test_env* e = new test_env;

	/*maps::coordinates position(37.7942, -122.4070);*/
	maps::coordinates position(37.7555302, 127.002253);

	maps::place_filter filter;
	int error = maps_place_filter_set_place_name(filter, "Seoul");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	/* shlg, 20150716, as recommended by HERE Plugin developer, the distance
	 * is extended in 10 times to 50000 */
	error = maps_service_search_place(e->m, position, 50000, filter, e->p,
		__utc_maps_service_search_place_cb, (void*) e, &e->rid);
	if (error != MAPS_ERROR_NONE)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(e->rid > 0);
	/* test finish --------------------------------- */

	e->wait_for_response();
	e->finish_request();
}

void utc_maps_service_search_place_n(void)
{
	test_env e;
	/*maps::coordinates position(37.7942, -122.4070);*/
	maps::coordinates position(37.7555302, 127.002253);
	maps::place_filter filter;

	/* test start --------------------------------- */
	int error =
		maps_service_search_place(NULL, position, 5000, filter, e.p,
		__utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_place(e.m, NULL, 5000, filter, e.p,
		__utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/*error = maps_service_search_place(e.m, position, 5000, filter, NULL,
		__utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);*/

	error = maps_service_search_place(e.m, position, 5000, NULL, e.p,
		__utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_place(e.m, position, 5000, filter, e.p, NULL,
		&e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */
}

/*int maps_service_search_place_by_area(maps_service_h maps,
* maps_area_h boundary , maps_place_preference_h preference,
* maps_place_filter_h filter, maps_service_search_place_cb callback,
* void* user_data, int* request_id); */
void utc_maps_service_search_place_by_area_p(void)
{
	test_env* e = new test_env;

	maps::place_filter filter;
	int error = maps_place_filter_set_place_name(filter, "Seoul");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps::place_category category;
	//error = maps_place_category_set_id(category, "eat-drink");
	error = maps_place_category_set_id(category, "cafe");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_place_filter_set_category(filter, category);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/*maps::area area(37.7942 + 0.2, -122.407 - 0.2,
			37.7942 - 0.2, -122.407 + 0.2);*/
	maps::area area(37.7555302 + 0.2, 127.002253 - 0.2,
			37.7555302 - 0.2, 127.002253 + 0.2);

	/* test start --------------------------------- */
	error = maps_service_search_place_by_area(e->m, area, filter, e->p,
		__utc_maps_service_search_place_cb, (void*) e, &e->rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(e->rid > 0);
	/* test finish --------------------------------- */

	e->wait_for_response();
	e->finish_request();
}

void utc_maps_service_search_place_by_area_n(void)
{
	test_env e;
	maps::place_filter filter;
	/*maps::area area(37.7942 + 0.2, -122.407 - 0.2, 37.7942 - 0.2,
		-122.407 + 0.2);*/
	maps::area area(37.7555302 + 0.2, 127.002253 - 0.2,
			37.7555302 - 0.2, 127.002253 + 0.2);

	/* test start --------------------------------- */
	int error =
		maps_service_search_place_by_area(NULL, area, filter, e.p,
		__utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_place_by_area(e.m, NULL, filter, e.p,
		__utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/*error = maps_service_search_place_by_area(e.m, area, filter, NULL,
		__utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);*/

	error = maps_service_search_place_by_area(e.m, area, NULL, e.p,
		__utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_place_by_area(e.m, area, filter, e.p, NULL,
		&e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */
}

/*int maps_service_search_place_by_address(maps_service_h maps,
* const char* address, maps_area_h boundary, maps_place_preference_h preference,
* maps_place_filter_h filter, maps_service_search_place_cb callback,
* void* user_data, int* request_id); */
void utc_maps_service_search_place_by_address_p(void)
{
	test_env* e = new test_env;

	maps::place_filter filter;
	int error = maps_place_filter_set_place_name(filter, "Seoul");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps::area area(37.7942 + 0.2, -122.407 - 0.2,
			37.7942 - 0.2, -122.407 + 0.2);
	/*maps::area area(37.7555302 + 0.2, 127.002253 - 0.2,
			37.7555302 - 0.2, 127.002253 + 0.2);*/

	/* test start --------------------------------- */
	error = maps_service_search_place_by_address(e->m, "Jackson", area,
		filter, e->p, __utc_maps_service_search_place_cb, (void*) e,
		&e->rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(e->rid > 0);
	/* test finish --------------------------------- */

	e->wait_for_response();
	e->finish_request();
}

void utc_maps_service_search_place_by_address_n(void)
{
	test_env e;

	maps::place_filter filter;
	int error = maps_place_filter_set_place_name(filter, "Seoul");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/*maps::area area(37.7942 + 0.2, -122.407 - 0.2, 37.7942 - 0.2,
		-122.407 + 0.2);*/
	maps::area area(37.7555302 + 0.2, 127.002253 - 0.2,
			37.7555302 - 0.2, 127.002253 + 0.2);

	/* test start --------------------------------- */
	error = maps_service_search_place_by_address(NULL, "Seoul", area,
		filter, e.p, __utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_place_by_address(e.m, NULL, area, filter,
		e.p, __utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/*error = maps_service_search_place_by_address(e.m, "Seoul", filter, NULL,
		filter, __utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);*/

	error = maps_service_search_place_by_address(e.m, "Seoul", area, NULL,
		e.p, __utc_maps_service_search_place_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_place_by_address(e.m, "Seoul", area, filter,
		e.p, NULL, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */
}

/*int maps_cancel_place(maps_service_h maps, int request_id); */
static bool __utc_maps_cancel_place_cb(maps_error_e error, int request_id,
	int index, int length, maps_place_h place, void* user_data)
{
	return true;
}

void utc_maps_cancel_place_p(void)
{
	test_env e;

	maps_item_hashtable_set_string(e.p, "no_need_callback",
		"no_need_callback");

	maps::coordinates position(37.34, 126.58);

	maps::place_filter filter;
	int error = maps_place_filter_set_place_name(filter, "Seoul");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_service_search_place(e.m, position, 5000, filter, e.p,
		__utc_maps_cancel_place_cb, (void*) &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(e.rid > 0);

	error = maps_service_cancel_request(e.m, e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* TODO: add tests for canceling maps_service_search_place_by_area,
	*  maps_service_search_place_by_address APIs */
}

void utc_maps_cancel_place_n(void)
{
	const int error = maps_service_cancel_request(NULL, 1);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
/* Route */
/*typedef bool(*maps_service_search_route_cb)(maps_error_e error,
* int request_id, int index, int total, maps_route_h route, void* user_data); */
static bool __utc_maps_service_search_route_cb(maps_error_e error,
	int request_id, int index, int total, maps_route_h route,
	void* user_data)
{
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	test_env* e = (test_env*) user_data;
	g_assert(e);

	if (e->rid > 0) {
		g_assert_cmpint(e->rid, ==, request_id);
	}

	g_assert(index >= 0);
	g_assert(total > 0);

	g_assert(route);
	int err = maps_route_destroy(route);
	g_assert_cmpint(err, ==, MAPS_ERROR_NONE);

	if (index == (total - 1)) {
		e->finish_response();
	}

	return true;
}

/*int maps_service_search_route(maps_service_h maps,
* maps_item_hashtable_h preference, maps_coordinates_h origin,
* maps_coordinates_h destination, maps_service_search_route_cb callback,
* void* user_data, int* request_id); */
void utc_maps_service_search_route_p(void)
{
	test_env* e = new test_env;

	maps_coordinates_h origin = NULL;
	int error = maps_coordinates_create(37.34, 126.58, &origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h destination = NULL;
	error = maps_coordinates_create(37.34, 126.58, &destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	error = maps_service_search_route(e->m, origin, destination, e->p,
		__utc_maps_service_search_route_cb, (void*) e, &e->rid);
	if (error != MAPS_ERROR_NONE)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(e->rid > 0);
	/* test finish --------------------------------- */

	e->wait_for_response();

	error = maps_coordinates_destroy(origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	e->finish_request();
}

void utc_maps_service_search_route_n(void)
{
	test_env e;

	maps_coordinates_h origin = NULL;
	int error = maps_coordinates_create(37.34, 126.58, &origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h destination = NULL;
	error = maps_coordinates_create(37.34, 126.58, &destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	error = maps_service_search_route(NULL, origin, destination, e.p,
		__utc_maps_service_search_route_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/*error = maps_service_search_route(e.m, origin, destination, NULL,
		__utc_maps_service_search_route_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);*/

	error = maps_service_search_route(e.m, NULL, destination, e.p,
		__utc_maps_service_search_route_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_route(e.m, origin, NULL, e.p,
		__utc_maps_service_search_route_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_route(e.m, origin, destination, e.p, NULL,
		&e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_route(e.m, origin, destination, e.p,
		__utc_maps_service_search_route_cb, &e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */

	error = maps_coordinates_destroy(origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/*int maps_service_search_route_waypoints(maps_service_h maps,
* maps_item_hashtable_h preference, maps_coordinates_h* waypoint_list,
* int waypoint_num, maps_service_search_route_cb callback, void* user_data,
* int* request_id); */
void utc_maps_service_search_route_waypoints_p(void)
{
	test_env* e = new test_env;

	maps_coordinates_h origin = NULL;
	int error = maps_coordinates_create(37.34, 126.58, &origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h destination = NULL;
	error = maps_coordinates_create(37.34, 126.58, &destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h waypoint_list[2] = { origin, destination };

	/* test start --------------------------------- */
	error = maps_service_search_route_waypoints(e->m, waypoint_list, 2,
		e->p, __utc_maps_service_search_route_cb, (void*) e, &e->rid);
	if (error != MAPS_ERROR_NONE)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(e->rid > 0);
	/* test finish --------------------------------- */

	e->wait_for_response();

	error = maps_coordinates_destroy(origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/*if(!e->async) delete e; */
	e->finish_request();
}

void utc_maps_service_search_route_waypoints_n(void)
{
	test_env e;

	maps_coordinates_h origin = NULL;
	int error = maps_coordinates_create(37.34, 126.58, &origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h destination = NULL;
	error = maps_coordinates_create(37.34, 126.58, &destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h waypoint_list[2] = { origin, destination };

	/* test start --------------------------------- */
	error = maps_service_search_route_waypoints(NULL, waypoint_list, 2, e.p,
		__utc_maps_service_search_route_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	/*error = maps_service_search_route_waypoints(e.m, waypoint_list, 2, NULL,
		__utc_maps_service_search_route_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);*/

	error = maps_service_search_route_waypoints(e.m, NULL, 2, e.p,
		__utc_maps_service_search_route_cb, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_route_waypoints(e.m, waypoint_list, 2, e.p,
		NULL, &e, &e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_service_search_route_waypoints(e.m, waypoint_list, 2, e.p,
		__utc_maps_service_search_route_cb, &e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	/* test finish --------------------------------- */

	error = maps_coordinates_destroy(origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

static bool __utc_maps_cancel_search_route_cb(maps_error_e error,
	int request_id, int index, int total, maps_route_h route,
	void* user_data)
{
	return true;
}

/*int maps_cancel_route(maps_service_h maps, int request_id); */
void utc_maps_cancel_route_p(void)
{
	test_env e;

	maps_item_hashtable_set_string(e.p, "no_need_callback",
		"no_need_callback");

	maps_coordinates_h origin = NULL;
	int error = maps_coordinates_create(37.34, 126.58, &origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h destination = NULL;
	error = maps_coordinates_create(37.34, 126.58, &destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	error = maps_service_search_route(e.m, origin, destination, e.p,
		__utc_maps_cancel_search_route_cb, &e, &e.rid);
	if (error != MAPS_ERROR_NONE)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(e.rid > 0);

	/*error = maps_cancel_route(e.m, e.rid); */
	error = maps_service_cancel_request(e.m, e.rid);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* TODO: add tests for canceling
	*  maps_service_search_route_waypoints API */
	/* test finish --------------------------------- */

	error = maps_coordinates_destroy(origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_cancel_route_n(void)
{
	/*const int error = maps_cancel_route(NULL, 1); */
	const int error = maps_service_cancel_request(NULL, 1);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
/* Route Search with realistic input */
static bool __utc_maps_route_segment_maneuver_cb(int index, int total,
	maps_route_maneuver_h maneuver, void* user_data)
{
	/*g_print(" Maneuver >> %d\n", (index+1)); */

	int error = maps_route_maneuver_destroy(maneuver);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	return true;
}

static bool __utc_maps_route_segment_cb(int index, int total,
	maps_route_segment_h segment, void* user_data)
{
	if (!segment) {
		g_print("critical error : FAILED\n");
		return false;
	}

	maps_coordinates_h _origin = NULL, _destination = NULL;
	double lat = 0.0, lon = 0.0;

	int error = maps_route_segment_get_origin(segment, &_origin);
	if (error != MAPS_ERROR_NONE)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	if (_origin) {
		error = maps_coordinates_get_latitude(_origin, &lat);
		if (error != MAPS_ERROR_NONE)
			__utc_print_error_string(error);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_coordinates_get_longitude(_origin, &lon);
		if (error != MAPS_ERROR_NONE)
			__utc_print_error_string(error);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		/*g_print("Segment Origin Lat : %f, Lon : %f\n", lat, lon); */

		error = maps_coordinates_destroy(_origin);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}

	error = maps_route_segment_get_destination(segment, &_destination);
	if (error != MAPS_ERROR_NONE)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	if (_destination) {
		error = maps_coordinates_get_latitude(_destination, &lat);
		if (error != MAPS_ERROR_NONE)
			__utc_print_error_string(error);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		error = maps_coordinates_get_longitude(_destination, &lon);
		if (error != MAPS_ERROR_NONE)
			__utc_print_error_string(error);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		/*g_print("Segment Destination Lat : %f, Lon : %f\n", lat,
		* lon); */

		error = maps_coordinates_destroy(_destination);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}

	/*g_print(">>>>>> Parsing segments <<<<<<<<<<\n"); */
	error = maps_route_segment_foreach_maneuver(segment,
		__utc_maps_route_segment_maneuver_cb, user_data);
	if (error != MAPS_ERROR_NONE)
		__utc_print_error_string(error);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/*g_print("maneuver is supported, error code :: %d\n", error); */

	error = maps_route_segment_destroy(segment);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	return true;
}

/*typedef bool(*maps_service_search_route_cb)(maps_error_e error,
* int request_id, int index, int total, maps_route_h route, void* user_data); */
static bool __utc_maps_service_search_route_real_cb(maps_error_e error,
	int request_id, int index, int total, maps_route_h route,
	void* user_data)
{
	/*g_print("Received route callback [%d of %d]\n\n", index, total);*/
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	test_env* e = (test_env*) user_data;
	g_assert(e);

	g_assert(index >= 0);
	g_assert(total > 0);

	/*------------------------------------------------------ */
	if (error == MAPS_ERROR_NONE) {
		/*g_print("\nDisplaying route result..\n"); */

		double dist = 0.0;
		long duration = 0;
		int err = maps_route_get_total_distance(route, &dist);
		g_assert_cmpint(err, ==, MAPS_ERROR_NONE);

		err = maps_route_get_total_duration(route, &duration);
		g_assert_cmpint(err, ==, MAPS_ERROR_NONE);

		/*g_print("Distance: %f\tDuration: %ld secs\n", dist,
		* duration); */

		/*g_print("Route ptr: %p\n", route); */
		err = maps_route_foreach_segment(route,
			__utc_maps_route_segment_cb, NULL);
		if (err != MAPS_ERROR_NONE)
			__utc_print_error_string(err);
		g_assert_cmpint(err, ==, MAPS_ERROR_NONE);
	}
	/*------------------------------------------------------ */

	g_assert(route);
	int err = maps_route_destroy(route);
	g_assert_cmpint(err, ==, MAPS_ERROR_NONE);

	if (index == (total - 1)) {
		e->finish_response();
	}

	return true;
}

/*int maps_service_search_route(maps_service_h maps,
* maps_item_hashtable_h preference, maps_coordinates_h origin,
* maps_coordinates_h destination, maps_service_search_route_cb callback,
* void* user_data, int* request_id); */
void utc_maps_service_search_route_real_p(void)
{
	test_env* e = new test_env;

	/*utc_main_loop_master mlm(&__utc_main_loop); */

	double srcLat = 12.733027;
	double srcLon = 77.83015999;

	double destLat = 12.9165167;
	double destLon = 79.1324985999;

	/* Creating route preference */
	maps_preference_h preference;
	int error = maps_preference_create(&preference);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* Call Route Search API */
	maps_coordinates_h origin = NULL;
	maps_coordinates_h destination = NULL;
	int request_id = 0;

	/* Setting Origin and Destination coordinates */
	maps_coordinates_create(srcLat, srcLon, &origin);
	maps_coordinates_create(destLat, destLon, &destination);

	error = maps_preference_set_route_transport_mode(preference,
		MAPS_ROUTE_TRANSPORT_MODE_CAR);
	/*MAPS_ROUTE_TRANSPORT_MODE_PEDESTRIAN); */
	/*MAPS_ROUTE_TRANSPORT_MODE_PUBLICTRANSIT); */
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* Set Route Optimization */
	error = maps_preference_set_route_optimization(preference,
		MAPS_ROUTE_TYPE_FASTEST);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/*g_print("Run Routing Service!\n"); */
	error = maps_service_search_route(e->m, origin, destination, preference,
		__utc_maps_service_search_route_real_cb, e, &request_id);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	e->wait_for_response();

	error = maps_coordinates_destroy(origin);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(destination);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_preference_destroy(preference);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/*if(!e->async) delete e; */
	e->finish_request();
}

