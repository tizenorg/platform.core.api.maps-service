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

#include "maps_preference_test.h"
#include "maps_preference.h"
#include "maps_error.h"
#include <glib.h>

/*int maps_preference_create(maps_preference_h* preference); */
/*int maps_preference_destroy(maps_preference_h preference); */
void utc_maps_preference_create_p(void)
{
	maps_preference_h preference = NULL;
	int error = maps_preference_create(&preference);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(preference);

	error = maps_preference_destroy(preference);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_preference_create_n(void)
{
	int error = maps_preference_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
};

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_preference_h h;
	int iterations;
 public:
	 test_env():h(NULL), iterations(0)
	{
		const int error = maps_preference_create(&h);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(h);
	}
	~test_env()
	{
		const int error = maps_preference_destroy(h);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/*int maps_preference_clone(const maps_preference_h origin,
* maps_preference_h* cloned); */
void utc_maps_preference_clone_p(void)
{
	test_env e;

	/* set test values for original */
	/*------------------------------------ */
	int error =
		maps_preference_set_distance_unit(e.h, MAPS_DISTANCE_UNIT_KM);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_preference_set_language(e.h, "Russian");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_preference_set_max_results(e.h, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_preference_set_country_code(e.h, "RU");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_preference_set_route_optimization(e.h,
		MAPS_ROUTE_TYPE_FASTEST);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_preference_set_route_transport_mode(e.h,
		MAPS_ROUTE_TRANSPORT_MODE_CAR);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_preference_set_route_feature_weight(e.h,
		MAPS_ROUTE_FEATURE_WEIGHT_NORMAL);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_preference_set_route_feature(e.h, MAPS_ROUTE_FEATURE_NO);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_preference_set_property(e.h, "key", "value");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	/*------------------------------------ */

	maps_preference_h cloned = NULL;
	error = maps_preference_clone(e.h, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* test if values are cloned correctly */
	/*------------------------------------ */
	maps_distance_unit_e unit = MAPS_DISTANCE_UNIT_YD;
	error = maps_preference_get_distance_unit(cloned, &unit);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(unit, ==, MAPS_DISTANCE_UNIT_KM);

	char* language = NULL;
	error = maps_preference_get_language(cloned, &language);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(language, ==, "Russian");
	g_free(language);

	int max_results = 0;
	error = maps_preference_get_max_results(cloned, &max_results);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(max_results, ==, 42);

	char* country_code = NULL;
	error = maps_preference_get_country_code(cloned, &country_code);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(country_code, ==, "RU");
	g_free(country_code);

	maps_route_optimization_e optimization = MAPS_ROUTE_TYPE_DIRECTDRIVE;
	error = maps_preference_get_route_optimization(cloned, &optimization);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(optimization, ==, MAPS_ROUTE_TYPE_FASTEST);

	maps_route_transport_mode_e transport_mode =
		MAPS_ROUTE_TRANSPORT_MODE_TRUCK;
	error = maps_preference_get_route_transport_mode(cloned,
		&transport_mode);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(transport_mode, ==, MAPS_ROUTE_TRANSPORT_MODE_CAR);

	maps_route_feature_weight_e feature_weight =
		MAPS_ROUTE_FEATURE_WEIGHT_STRICTEXCLUDE;
	error = maps_preference_get_route_feature_weight(cloned,
		&feature_weight);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(feature_weight, ==, MAPS_ROUTE_FEATURE_WEIGHT_NORMAL);

	maps_route_feature_e feature = MAPS_ROUTE_FEATURE_STAIRS;
	error = maps_preference_get_route_feature(cloned, &feature);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(feature, ==, MAPS_ROUTE_FEATURE_NO);

	char* value = NULL;
	error = maps_preference_get(cloned, "key", &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(value, ==, "value");
	g_free(value);

	/*------------------------------------ */

	error = maps_preference_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_preference_clone_n(void)
{
	test_env e;

	maps_preference_h cloned = NULL;

	int error = maps_preference_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_clone(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int maps_preference_get_distance_unit(const maps_preference_h preference,
* maps_distance_unit_e* unit); */
/*int maps_preference_set_distance_unit(maps_preference_h preference,
* const maps_distance_unit_e unit); */
void utc_maps_preference_distance_unit_p(void)
{
	test_env e;

	int error =
		maps_preference_set_distance_unit(NULL, MAPS_DISTANCE_UNIT_M);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_distance_unit_e unit = MAPS_DISTANCE_UNIT_YD;
	error = maps_preference_get_distance_unit(e.h, &unit);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_preference_distance_unit_n(void)
{
	test_env e;

	int error =
		maps_preference_set_distance_unit(NULL, MAPS_DISTANCE_UNIT_M);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_distance_unit_e unit = MAPS_DISTANCE_UNIT_YD;
	error = maps_preference_get_distance_unit(NULL, &unit);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get_distance_unit(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int maps_p:reference_get_language(const maps_preference_h preference,
* char** language); */
/*int maps_preference_set_language(maps_preference_h preference,
* const char* language); */
void utc_maps_preference_language_p(void)
{
	test_env e;

	int error = maps_preference_set_language(e.h, "Russian");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* language = NULL;
	error = maps_preference_get_language(e.h, &language);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(language, ==, "Russian");
	g_free(language);

}

void utc_maps_preference_language_n(void)
{
	test_env e;

	int error = maps_preference_set_language(NULL, "Russian");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_set_language(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* language = NULL;
	error = maps_preference_get_language(NULL, &language);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get_language(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int maps_preference_get_max_results(const maps_preference_h preference,
* int* max_results); */
/*int maps_preference_set_max_results(maps_preference_h preference,
* const int max_results); */
void utc_maps_preference_max_results_p(void)
{
	test_env e;

	int error = maps_preference_set_max_results(e.h, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	int max_results = 0;
	error = maps_preference_get_max_results(e.h, &max_results);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(max_results, ==, 42);

}

void utc_maps_preference_max_results_n(void)
{
	test_env e;

	int error = maps_preference_set_max_results(NULL, 42);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	int max_results = 0;
	error = maps_preference_get_max_results(NULL, &max_results);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get_max_results(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int maps_preference_get_country_code(const maps_preference_h preference,
* char** country_code); */
/*int maps_preference_set_country_code(maps_preference_h preference,
* const char* country_code); */
void utc_maps_preference_country_code_p(void)
{
	test_env e;

	int error = maps_preference_set_country_code(e.h, "RU");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* country_code = NULL;
	error = maps_preference_get_country_code(e.h, &country_code);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(country_code, ==, "RU");
	g_free(country_code);

}

void utc_maps_preference_country_code_n(void)
{
	test_env e;

	int error = maps_preference_set_country_code(NULL, "RU");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_set_country_code(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* country_code = NULL;
	error = maps_preference_get_country_code(NULL, &country_code);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get_country_code(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int maps_preference_get_route_optimization(const maps_preference_h preference,
* maps_route_optimization_e* optimization); */
/*int maps_preference_set_route_optimization(maps_preference_h preference, const
* maps_route_optimization_e optimization); */
void utc_maps_preference_route_optimization_p(void)
{
	test_env e;

	int error =
		maps_preference_set_route_optimization(e.h,
		MAPS_ROUTE_TYPE_FASTEST);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_optimization_e optimization = MAPS_ROUTE_TYPE_DIRECTDRIVE;
	error = maps_preference_get_route_optimization(e.h, &optimization);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(optimization, ==, MAPS_ROUTE_TYPE_FASTEST);

}

void utc_maps_preference_route_optimization_n(void)
{
	test_env e;

	int error =
		maps_preference_set_route_optimization(NULL,
		MAPS_ROUTE_TYPE_FASTEST);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_route_optimization_e optimization = MAPS_ROUTE_TYPE_DIRECTDRIVE;
	error = maps_preference_get_route_optimization(NULL, &optimization);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get_route_optimization(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int maps_preference_get_route_transport_mode(
* const maps_preference_h preference,
* maps_route_transport_mode_e* transport_mode); */
/*int maps_preference_set_route_transport_mode(maps_preference_h preference,
* const maps_route_transport_mode_e transport_mode); */
void utc_maps_preference_route_transport_mode_p(void)
{
	test_env e;

	int error =
		maps_preference_set_route_transport_mode(e.h,
		MAPS_ROUTE_TRANSPORT_MODE_CAR);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_transport_mode_e transport_mode =
		MAPS_ROUTE_TRANSPORT_MODE_TRUCK;
	error = maps_preference_get_route_transport_mode(e.h, &transport_mode);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(transport_mode, ==, MAPS_ROUTE_TRANSPORT_MODE_CAR);

}

void utc_maps_preference_route_transport_mode_n(void)
{
	test_env e;

	int error =
		maps_preference_set_route_transport_mode(NULL,
		MAPS_ROUTE_TRANSPORT_MODE_CAR);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_route_transport_mode_e transport_mode =
		MAPS_ROUTE_TRANSPORT_MODE_TRUCK;
	error = maps_preference_get_route_transport_mode(NULL, &transport_mode);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get_route_transport_mode(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int maps_preference_get_route_feature_weight(
* const maps_preference_h preference,
* maps_route_feature_weight_e* feature_weight); */
/*int maps_preference_set_route_feature_weight(maps_preference_h preference,
* const maps_route_feature_weight_e feature_weight); */
void utc_maps_preference_route_feature_weight_p(void)
{
	test_env e;

	int error =
		maps_preference_set_route_feature_weight(e.h,
		MAPS_ROUTE_FEATURE_WEIGHT_NORMAL);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_feature_weight_e feature_weight =
		MAPS_ROUTE_FEATURE_WEIGHT_STRICTEXCLUDE;
	error = maps_preference_get_route_feature_weight(e.h, &feature_weight);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(feature_weight, ==, MAPS_ROUTE_FEATURE_WEIGHT_NORMAL);

}

void utc_maps_preference_route_feature_weight_n(void)
{
	test_env e;

	int error =
		maps_preference_set_route_feature_weight(NULL,
		MAPS_ROUTE_FEATURE_WEIGHT_NORMAL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_route_feature_weight_e feature_weight =
		MAPS_ROUTE_FEATURE_WEIGHT_STRICTEXCLUDE;
	error = maps_preference_get_route_feature_weight(NULL, &feature_weight);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get_route_feature_weight(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int maps_preference_get_route_feature(const maps_preference_h preference,
* maps_route_feature_weight_e* feature); */
/*int maps_preference_set_route_feature(maps_preference_h preference,
* const maps_route_feature_e feature); */
void utc_maps_preference_route_feature_p(void)
{
	test_env e;

	int error =
		maps_preference_set_route_feature(e.h, MAPS_ROUTE_FEATURE_NO);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_route_feature_e feature = MAPS_ROUTE_FEATURE_STAIRS;
	error = maps_preference_get_route_feature(e.h, &feature);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(feature, ==, MAPS_ROUTE_FEATURE_NO);

}

void utc_maps_preference_route_feature_n(void)
{
	test_env e;

	int error =
		maps_preference_set_route_feature(NULL, MAPS_ROUTE_FEATURE_NO);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_route_feature_e feature = MAPS_ROUTE_FEATURE_STAIRS;
	error = maps_preference_get_route_feature(NULL, &feature);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get_route_feature(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

static bool __utc_maps_preference_properties_cb(int index, int total, char* key,
	char* value, void* user_data)
{
	g_assert(index >= 0);
	g_assert(total > 0);
	g_assert(key);
	g_assert(value);
	g_free(key);
	g_free(value);
	return true;
}

/*int maps_preference_get(const maps_preference_h preference, const char* key,
* char** value); */
/*int maps_preference_set_property(maps_preference_h preference,
* const char* key, const char* value); */
/*typedef bool (*maps_preference_properties_cb)(int index, int total, char* key,
* char* value, void* user_data); */
/*int maps_preference_foreach_property(const maps_preference_h preference,
* maps_preference_properties_cb callback , void* user_data); */
void utc_maps_preference_property_p(void)
{
	test_env e;

	int error = maps_preference_set_property(e.h, "key", "value");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* value = NULL;
	error = maps_preference_get(e.h, "key", &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(value, ==, "value");
	g_free(value);

	error = maps_preference_foreach_property(e.h,
		__utc_maps_preference_properties_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_preference_property_n(void)
{
	test_env e;

	int error = maps_preference_set_property(NULL, "key", "value");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_set_property(e.h, NULL, "value");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_set_property(e.h, "key", NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* value = NULL;
	error = maps_preference_get(NULL, "key", &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get(e.h, NULL, &value);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_preference_get(e.h, "key", NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

