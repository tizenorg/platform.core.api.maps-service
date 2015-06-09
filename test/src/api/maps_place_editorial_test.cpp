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

#include "maps_place_editorial_test.h"
#include "maps_place_editorial_plugin.h"
#include "maps_place_media_plugin.h"
#include "maps_place_link_object_plugin.h"
#include "maps_error.h"
#include <glib.h>

void utc_maps_place_editorial_create_p(void)
{
	maps_place_editorial_h editorial = NULL;
	int error = maps_place_editorial_create(&editorial);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(editorial);

	error = maps_place_editorial_destroy(editorial);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_editorial_create_n(void)
{
	int error = maps_place_editorial_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_editorial_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_editorial_h e;
 public:
	test_env():e(NULL)
	{
		const int error = maps_place_editorial_create(&e);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(e);
	}
	~test_env()
	{
		const int error = maps_place_editorial_destroy(e);
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
}

static maps_place_media_h __utc_prepare_place_media() {
	maps_place_media_h m = NULL;

	int error = maps_place_media_create(&m);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_media_set_attribution(m, "attribution");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_link_object_h supplier = __utc_prepare_place_link_object(
	"supplier id", "supplier string", "supplier type", "supplier name");
	error = maps_place_media_set_supplier(m, supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_place_link_object_destroy(supplier);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_link_object_h via = __utc_prepare_place_link_object("via id",
	"via string", "via type", "via name");
	error = maps_place_media_set_via(m, via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_place_link_object_destroy(via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	return m;
}*/

void utc_maps_place_editorial_clone_p(void)
{
	test_env e;

	maps_place_editorial_h cloned = NULL;
	int error = maps_place_editorial_clone(e.e, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	error = maps_place_editorial_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/*
	 * * set test values for original *
	 * *------------------------------------ *
	 * int error = maps_place_editorial_set_description(e.e, "description");
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 *
	 * error = maps_place_editorial_set_language(e.e, "language");
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 *
	 * maps_place_media_h media = __utc_prepare_place_media();
	 *
	 * error = maps_place_editorial_set_media(e.e, media);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 *
	 * error = maps_place_media_destroy(media);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * *------------------------------------ *
	 *
	 * maps_place_editorial_h cloned = NULL;
	 * error = maps_place_editorial_clone(e.e, &cloned);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert(cloned);
	 *
	 *
	 * * test if values are cloned correctly *
	 * *------------------------------------ *
	 * char* description = NULL;
	 * error = maps_place_editorial_get_description(cloned, &description);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(description, ==, "description");
	 * g_free(description);
	 *
	 * char* language = NULL;
	 * error = maps_place_editorial_get_language(cloned, &language);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(language, ==, "language");
	 * g_free(language);
	 *
	 * {* Checking the object "media" *
	 * maps_place_media_h media_obtained = NULL;
	 * error = maps_place_editorial_get_media(cloned,  &media_obtained);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert(media_obtained);
	 *
	 * char* attribution = NULL;
	 * error = maps_place_media_get_attribution(media_obtained,
	 * &attribution);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(attribution, ==, "attribution");
	 * g_free(attribution);
	 *
	 * maps_place_link_object_h supplier_obtained = NULL;
	 * error = maps_place_media_get_supplier(media_obtained,
	 * &supplier_obtained);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert(supplier_obtained);
	 *
	 * {* Checking link object "supplier" *
	 * char* supplier_string = NULL;
	 * error = maps_place_link_object_get_string(supplier_obtained,
	 * &supplier_string);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(supplier_string, ==, "supplier string");
	 * g_free(supplier_string);
	 *
	 * char* supplier_type = NULL;
	 * error = maps_place_link_object_get_type(supplier_obtained,
	 * &supplier_type);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(supplier_type, ==, "supplier type");
	 * g_free(supplier_type);
	 *
	 * char* supplier_id = NULL;
	 * error = maps_place_link_object_get_id(supplier_obtained,
						* &supplier_id);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(supplier_id, ==, "supplier id");
	 * g_free(supplier_id);
	 *
	 * char* supplier_name = NULL;
	 * error = maps_place_link_object_get_name(supplier_obtained,
						  * &supplier_name);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(supplier_name, ==, "supplier name");
	 * g_free(supplier_name);
	 * }
	 * maps_place_link_object_destroy(supplier_obtained);
	 *
	 * maps_place_link_object_h via_obtained = NULL;
	 * error = maps_place_media_get_via(media_obtained, &via_obtained);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert(via_obtained);
	 * {* Checking link object "via" *
	 * char* via_string = NULL;
	 * error = maps_place_link_object_get_string(via_obtained, &via_string);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(via_string, ==, "via string");
	 * g_free(via_string);
	 *
	 * char* via_type = NULL;
	 * error = maps_place_link_object_get_type(via_obtained, &via_type);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(via_type, ==, "via type");
	 * g_free(via_type);
	 *
	 * char* via_id = NULL;
	 * error = maps_place_link_object_get_id(via_obtained, &via_id);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(via_id, ==, "via id");
	 * g_free(via_id);
	 *
	 * char* via_name = NULL;
	 * error = maps_place_link_object_get_name(via_obtained, &via_name);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	 * g_assert_cmpstr(via_name, ==, "via name");
	 * g_free(via_name);
	 * }
	 * maps_place_link_object_destroy(via_obtained);
	 *
	 * maps_place_media_destroy(media_obtained);
	 * }
	 * *------------------------------------ *
	 *
	 *
	 * error = maps_place_editorial_destroy(cloned);
	 * g_assert_cmpint(error, ==, MAPS_ERROR_NONE); */
}

void utc_maps_place_editorial_clone_n(void)
{
	test_env e;

	maps_place_editorial_h cloned = NULL;
	int error = maps_place_editorial_clone(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_editorial_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

void utc_maps_place_editorial_description_p(void)
{
	test_env e;

	int error =
		maps_place_editorial_set_description(e.e,
		"editorial_description");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* description = NULL;
	error = maps_place_editorial_get_description(e.e, &description);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(description, ==, "editorial_description");
	g_free(description);
}

void utc_maps_place_editorial_description_n(void)
{
	test_env e;

	int error =
		maps_place_editorial_set_description(NULL,
		"editorial_description");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_editorial_set_description(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* description = NULL;
	error = maps_place_editorial_get_description(NULL, &description);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_editorial_get_description(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

void utc_maps_place_editorial_language_p(void)
{
	test_env e;

	int error =
		maps_place_editorial_set_language(e.e, "editorial_language");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* language = NULL;
	error = maps_place_editorial_get_language(e.e, &language);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(language, ==, "editorial_language");
	g_free(language);
}

void utc_maps_place_editorial_language_n(void)
{
	test_env e;

	int error =
		maps_place_editorial_set_language(NULL, "editorial_language");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_editorial_set_language(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* language = NULL;
	error = maps_place_editorial_get_language(NULL, &language);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_editorial_get_language(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

void utc_maps_place_editorial_media_p(void)
{
	test_env e;

	maps_place_media_h media = NULL;
	int error = maps_place_media_create(&media);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_editorial_set_media(e.e, media);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_media_h media_obtained = NULL;
	error = maps_place_editorial_get_media(e.e, &media_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(media_obtained);

	maps_place_media_destroy(media_obtained);

	error = maps_place_media_destroy(media);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_editorial_media_n(void)
{
	test_env e;

	maps_place_media_h media = NULL;
	int error = maps_place_media_create(&media);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_editorial_set_media(NULL, media);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_editorial_set_media(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_place_media_h obtain_media = NULL;
	error = maps_place_editorial_get_media(NULL, &obtain_media);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_editorial_get_media(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_destroy(media);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

