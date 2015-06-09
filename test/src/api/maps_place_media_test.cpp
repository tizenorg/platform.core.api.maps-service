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

#include "maps_place_media_test.h"
#include "maps_place_media_plugin.h"
#include "maps_place_link_object_plugin.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_place_media_create(maps_place_media_h* place); */
/* int maps_place_media_destroy(maps_place_media_h place); */
void utc_maps_place_media_create_p(void)
{
	maps_place_media_h media = NULL;
	int error = maps_place_media_create(&media);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(media);

	error = maps_place_media_destroy(media);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_media_create_n(void)
{
	int error = maps_place_media_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_media_h m;
 public:
	test_env():m(NULL)
	{
		const int error = maps_place_media_create(&m);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(m);
	}
	~test_env()
	{
		const int error = maps_place_media_destroy(m);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/*
static maps_place_link_object_h __utc_prepare_place_link_object(const char* id,
const char* string, const char* type, const char* name) {
	maps_place_link_object_h l = NULL;

	int error = maps_place_link_object_create(&l);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_link_object_set_id(l, id);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_link_object_set_name(l, name);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_link_object_set_type(l, type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_link_object_set_string(l, string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	return l;
}*/

/* int maps_place_media_clone(const maps_place_media_h origin,
*  maps_place_media_h* cloned); */
void utc_maps_place_media_clone_p(void)
{
	test_env e;

	maps_place_media_h cloned = NULL;
	int error = maps_place_media_clone(e.m, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);
	error = maps_place_media_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

/*

	* set test values for original *
	*------------------------------------ *
	int error = maps_place_media_set_attribution(e.m, "attribution");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_link_object_h supplier = __utc_prepare_place_link_object(
	"supplier id", "supplier string", "supplier type", "supplier name");
	error = maps_place_media_set_supplier(e.m, supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_place_link_object_destroy(supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_link_object_h via = __utc_prepare_place_link_object(
	"via id", "via string", "via type", "via name");
	error = maps_place_media_set_via(e.m, via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_place_link_object_destroy(via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	*------------------------------------ *

	maps_place_media_h cloned = NULL;
	error = maps_place_media_clone(e.m, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	* test if values are cloned correctly *
	*------------------------------------ *
	char* attribution = NULL;
	error = maps_place_media_get_attribution(cloned, &attribution);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(attribution, ==, "attribution");
	g_free(attribution);

	maps_place_link_object_h supplier_obtained = NULL;
	error = maps_place_media_get_supplier(cloned, &supplier_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(supplier_obtained);

	{ * Checking link object "supplier" *
		char* supplier_string = NULL;
		error = maps_place_link_object_get_string(
		supplier_obtained, &supplier_string);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(supplier_string, ==, "supplier string");
		g_free(supplier_string);

		char* supplier_type = NULL;
		error = maps_place_link_object_get_type(
		supplier_obtained, &supplier_type);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(supplier_type, ==, "supplier type");
		g_free(supplier_type);

		char* supplier_id = NULL;
		error = maps_place_link_object_get_id(
		supplier_obtained, &supplier_id);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(supplier_id, ==, "supplier id");
		g_free(supplier_id);

		char* supplier_name = NULL;
		error = maps_place_link_object_get_name(supplier_obtained,
		&supplier_name);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(supplier_name, ==, "supplier name");
		g_free(supplier_id);
	}
	maps_place_link_object_destroy(supplier_obtained);

	maps_place_link_object_h via_obtained = NULL;
	error = maps_place_media_get_via(cloned,  &via_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(via_obtained);

	{ * Checking link object "via" *
		char* via_string = NULL;
		error = maps_place_link_object_get_string(via_obtained,
							  &via_string);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(via_string, ==, "via string");
		g_free(via_string);

		char* via_type = NULL;
		error = maps_place_link_object_get_type(via_obtained,
							&via_type);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(via_type, ==, "via type");
		g_free(via_type);

		char* via_id = NULL;
		error = maps_place_link_object_get_id(via_obtained, &via_id);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(via_id, ==, "via id");
		g_free(via_id);

		char* via_name = NULL;
		error = maps_place_link_object_get_name(via_obtained,
							&via_name);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(via_name, ==, "via name");
		g_free(via_name);
	}
	maps_place_link_object_destroy(via_obtained);
	*------------------------------------ *

	error = maps_place_media_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	*/
}

void utc_maps_place_media_clone_n(void)
{
	test_env e;

	maps_place_media_h cloned = NULL;
	int error = maps_place_media_clone(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_media_set_attribution(maps_place_media_h place,
*  const char* attribution); */
/* int maps_place_media_get_attribution(maps_place_media_h place,
*  char** attribution); */
void utc_maps_place_media_attribution_p(void)
{
	test_env e;

	int error = maps_place_media_set_attribution(e.m, "media_attribution");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* attribution = NULL;
	error = maps_place_media_get_attribution(e.m, &attribution);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(attribution, ==, "media_attribution");
	g_free(attribution);
}

void utc_maps_place_media_attribution_n(void)
{
	test_env e;

	int error = maps_place_media_set_attribution(NULL, "media_attribution");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_set_attribution(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* attribution = NULL;
	error = maps_place_media_get_attribution(NULL, &attribution);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_get_attribution(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_media_set_supplier(maps_place_media_h place,
*  maps_place_link_object_h supplier); */
/* int maps_place_media_get_supplier(maps_place_media_h place,
*  maps_place_link_object_h* supplier); */
void utc_maps_place_media_supplier_p(void)
{
	test_env e;

	maps_place_link_object_h supplier = NULL;
	int error = maps_place_link_object_create(&supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_media_set_supplier(e.m, supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_link_object_h supplier_obtained = NULL;
	error = maps_place_media_get_supplier(e.m, &supplier_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(supplier_obtained);

	maps_place_link_object_destroy(supplier_obtained);

	error = maps_place_link_object_destroy(supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_media_supplier_n(void)
{
	test_env e;

	maps_place_link_object_h supplier = NULL;
	int error = maps_place_link_object_create(&supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_media_set_supplier(NULL, supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_set_supplier(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_place_link_object_h supplier_obtained = NULL;
	error = maps_place_media_get_supplier(NULL, &supplier_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_get_supplier(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_destroy(supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_place_media_set_via(maps_place_media_h place,
*  maps_place_link_object_h via); */
/* int maps_place_media_get_via(maps_place_media_h place,
*  maps_place_link_object_h* via); */
void utc_maps_place_media_via_p(void)
{
	test_env e;

	maps_place_link_object_h via = NULL;
	int error = maps_place_link_object_create(&via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_media_set_via(e.m, via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_link_object_h via_obtained = NULL;
	error = maps_place_media_get_via(e.m, &via_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(via_obtained);

	maps_place_link_object_destroy(via_obtained);

	error = maps_place_link_object_destroy(via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_media_via_n(void)
{
	test_env e;

	maps_place_link_object_h via = NULL;
	int error = maps_place_link_object_create(&via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_media_set_via(NULL, via);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_set_via(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_place_link_object_h via_obtained = NULL;
	error = maps_place_media_get_via(NULL, &via_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_get_via(e.m, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_destroy(via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

