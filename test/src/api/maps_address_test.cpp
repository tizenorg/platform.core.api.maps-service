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

#include "maps_address_test.h"
#include "maps_address.h"
#include "maps_error.h"
#include <glib.h>

/* int utc_maps_address_create (maps_address_h* address) */
/* int maps_address_destroy (maps_address_h* address); */
void utc_maps_address_create_p(void)
{
	maps_address_h address = NULL;
	int error = maps_address_create(&address);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(address);

	error = maps_address_destroy(address);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_address_create_n(void)
{
	int error = maps_address_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_address_h a;
 public:
	test_env():a(NULL)
	{
		const int error = maps_address_create(&a);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(a);
	}
	~test_env()
	{
		const int error = maps_address_destroy(a);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_address_clone (maps_address_h origin, maps_address_h* cloned); */
void utc_maps_address_clone_p()
{
	test_env e;

	/* set test values for original address */
	/*------------------------------------ */
	int error = maps_address_set_building_number(e.a, "building_number");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_address_set_street(e.a, "street");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_address_set_district(e.a, "district");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_address_set_city(e.a, "city");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_address_set_state(e.a, "state");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_address_set_country(e.a, "country");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_address_set_country_code(e.a, "country_code");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_address_set_county(e.a, "county");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_address_set_postal_code(e.a, "postal_code");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_address_set_freetext(e.a, "freetext");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_address_h cloned = NULL;
	error = maps_address_clone(e.a, &cloned);

	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	char* building_number = NULL;
	error = maps_address_get_building_number(cloned, &building_number);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(building_number, ==, "building_number");
	g_free(building_number);

	char* street = NULL;
	error = maps_address_get_street(cloned, &street);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(street, ==, "street");
	g_free(street);

	char* district = NULL;
	error = maps_address_get_district(cloned, &district);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(district, ==, "district");
	g_free(district);

	char* city = NULL;
	error = maps_address_get_city(cloned, &city);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(city, ==, "city");
	g_free(city);

	char* state = NULL;
	error = maps_address_get_state(cloned, &state);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(state, ==, "state");
	g_free(state);

	char* country = NULL;
	error = maps_address_get_country(cloned, &country);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(country, ==, "country");
	g_free(country);

	char* country_code = NULL;
	error = maps_address_get_country_code(cloned, &country_code);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(country_code, ==, "country_code");
	g_free(country_code);

	char* county = NULL;
	error = maps_address_get_county(cloned, &county);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(county, ==, "county");
	g_free(county);

	char* postal_code = NULL;
	error = maps_address_get_postal_code(cloned, &postal_code);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(postal_code, ==, "postal_code");
	g_free(postal_code);

	char* freetext = NULL;
	error = maps_address_get_freetext(cloned, &freetext);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(freetext, ==, "freetext");
	g_free(freetext);
	/*------------------------------------ */

	error = maps_address_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_address_clone_n()
{
	test_env e;

	int error = maps_address_clone(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_address_h a = NULL;
	error = maps_address_clone(NULL, &a);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!a);

	error = maps_address_clone(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_address_set_building_number(maps_address_h addr,
*  const char* building_number); */
/* int maps_address_get_building_number(maps_address_h addr,
*  char** building_number); */
void utc_maps_address_building_number_p(void)
{
	test_env e;

	int error = maps_address_set_building_number(e.a, "13");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* building_number = NULL;
	error = maps_address_get_building_number(e.a, &building_number);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(building_number, ==, "13");
	g_free(building_number);
}

void utc_maps_address_building_number_n(void)
{
	test_env e;

	int error = maps_address_set_building_number(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_building_number(NULL, "13");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_building_number(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* building_number = NULL;
	error = maps_address_get_building_number(NULL, &building_number);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!building_number);

	error = maps_address_get_building_number(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!building_number);

	error = maps_address_get_building_number(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!building_number);
}

/* int maps_address_set_street(maps_address_h addr, const char* street); */
/* int maps_address_get_street(maps_address_h addr, char** street); */
void utc_maps_address_street_p(void)
{
	test_env e;

	int error = maps_address_set_street(e.a, "Gangnam");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* street = NULL;
	error = maps_address_get_street(e.a, &street);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(street, ==, "Gangnam");
	g_free(street);
}

void utc_maps_address_street_n(void)
{
	test_env e;

	int error = maps_address_set_street(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_street(NULL, "Gangnam");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_street(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* street = NULL;
	error = maps_address_get_street(NULL, &street);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!street);

	error = maps_address_get_street(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!street);

	error = maps_address_get_street(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!street);
}

/* int maps_address_set_district(maps_address_h addr, const char* district); */
/*int maps_address_get_district(maps_address_h addr, char** district); */
void utc_maps_address_district_p(void)
{
	test_env e;

	int error = maps_address_set_district(e.a, "Gangnam");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* district = NULL;
	error = maps_address_get_district(e.a, &district);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(district, ==, "Gangnam");
	g_free(district);
}

void utc_maps_address_district_n(void)
{
	test_env e;

	int error = maps_address_set_district(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_district(NULL, "Gangnam");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_district(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* district = NULL;
	error = maps_address_get_district(NULL, &district);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!district);

	error = maps_address_get_district(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!district);

	error = maps_address_get_district(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!district);
}

/* int maps_address_set_city(maps_address_h addr, const char* city); */
/* int maps_address_get_city(maps_address_h addr, char** city); */
void utc_maps_address_city_p(void)
{
	test_env e;

	int error = maps_address_set_city(e.a, "Seoul");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* city = NULL;
	error = maps_address_get_city(e.a, &city);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(city, ==, "Seoul");
	g_free(city);
}

void utc_maps_address_city_n(void)
{
	test_env e;

	int error = maps_address_set_city(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_city(NULL, "Seoul");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_city(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* city = NULL;
	error = maps_address_get_city(NULL, &city);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!city);

	error = maps_address_get_city(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!city);

	error = maps_address_get_city(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!city);
}

/* int maps_address_set_state(maps_address_h addr, const char* state); */
/* int maps_address_get_state(maps_address_h addr, char** state); */
void utc_maps_address_state_p(void)
{
	test_env e;

	int error = maps_address_set_state(e.a, "Gyeonggi");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* state = NULL;
	error = maps_address_get_state(e.a, &state);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(state, ==, "Gyeonggi");
	g_free(state);
}

void utc_maps_address_state_n(void)
{
	test_env e;

	int error = maps_address_set_state(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_state(NULL, "Gyeonggi");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_state(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* state = NULL;
	error = maps_address_get_state(NULL, &state);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!state);

	error = maps_address_get_state(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!state);

	error = maps_address_get_state(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!state);
}

/* int maps_address_set_country(maps_address_h addr, const char* country); */
/* int maps_address_get_country(maps_address_h addr, char** country); */
void utc_maps_address_country_p(void)
{
	test_env e;

	int error = maps_address_set_country(e.a, "ROK");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* country = NULL;
	error = maps_address_get_country(e.a, &country);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(country, ==, "ROK");
	g_free(country);
}

void utc_maps_address_country_n(void)
{
	test_env e;

	int error = maps_address_set_country(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_country(NULL, "ROK");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_country(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* country = NULL;
	error = maps_address_get_country(NULL, &country);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!country);

	error = maps_address_get_country(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!country);

	error = maps_address_get_country(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!country);
}

/* int maps_address_set_country_code(maps_address_h addr,
*  const char* country_code); */
/* int maps_address_get_country_code(maps_address_h addr,
*  char** country_code); */
void utc_maps_address_country_code_p(void)
{
	test_env e;

	int error = maps_address_set_country_code(e.a, "+82");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* country_code = NULL;
	error = maps_address_get_country_code(e.a, &country_code);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(country_code, ==, "+82");
	g_free(country_code);
}

void utc_maps_address_country_code_n(void)
{
	test_env e;

	int error = maps_address_set_country_code(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_country_code(NULL, "+82");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_country_code(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* country_code = NULL;
	error = maps_address_get_country_code(NULL, &country_code);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!country_code);

	error = maps_address_get_country_code(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!country_code);

	error = maps_address_get_country_code(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!country_code);
}

/* int maps_address_set_county(maps_address_h addr, const char* county); */
/* int maps_address_get_county(maps_address_h addr, char** county); */
void utc_maps_address_county_p(void)
{
	test_env e;

	int error = maps_address_set_county(e.a, "---");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* county = NULL;
	error = maps_address_get_county(e.a, &county);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(county, ==, "---");
	g_free(county);
}

void utc_maps_address_county_n(void)
{
	test_env e;

	int error = maps_address_set_county(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_county(NULL, "---");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_county(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* county = NULL;
	error = maps_address_get_county(NULL, &county);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!county);

	error = maps_address_get_county(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!county);

	error = maps_address_get_county(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!county);
}

/* int maps_address_set_postal_code(maps_address_h addr,
*  const char* postal_code); */
/* int maps_address_get_postal_code(maps_address_h addr,
*  char** postal_code); */
void utc_maps_address_postal_code_p(void)
{
	test_env e;

	int error = maps_address_set_postal_code(e.a, "123");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* postal_code = NULL;
	error = maps_address_get_postal_code(e.a, &postal_code);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(postal_code, ==, "123");
	g_free(postal_code);
}

void utc_maps_address_postal_code_n(void)
{
	test_env e;

	int error = maps_address_set_postal_code(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_postal_code(NULL, "123");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_postal_code(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* postal_code = NULL;
	error = maps_address_get_postal_code(NULL, &postal_code);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!postal_code);

	error = maps_address_get_postal_code(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!postal_code);

	error = maps_address_get_postal_code(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!postal_code);
}

/* int maps_address_set_freetext(maps_address_h addr, const char* freetext); */
/* int maps_address_get_freetext(maps_address_h addr, char** freetext); */
void utc_maps_address_destroytext_p(void)
{
	test_env e;

	int error = maps_address_set_freetext(e.a, "---");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* freetext = NULL;
	error = maps_address_get_freetext(e.a, &freetext);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(freetext, ==, "---");
	g_free(freetext);
}

void utc_maps_address_destroytext_n(void)
{
	test_env e;

	int error = maps_address_set_freetext(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_freetext(NULL, "---");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_address_set_freetext(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* freetext = NULL;
	error = maps_address_get_freetext(NULL, &freetext);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!freetext);

	error = maps_address_get_freetext(e.a, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!freetext);

	error = maps_address_get_freetext(NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!freetext);
}

