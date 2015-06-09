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

#include "maps_place_review_test.h"
#include "maps_place_review_plugin.h"
#include "maps_place_media_plugin.h"
#include "maps_place_link_object_plugin.h"
#include "maps_error.h"
#include <glib.h>

/* int maps_place_review_create(maps_place_review_h* place); */
/* int maps_place_review_destroy(maps_place_review_h place); */
void utc_maps_place_review_create_p(void)
{
	maps_place_review_h h = NULL;
	int error = maps_place_review_create(&h);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(h);

	error = maps_place_review_destroy(h);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_review_create_n(void)
{
	int error = maps_place_review_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_place_review_h h;
 public:
	test_env():h(NULL)
	{
		const int error = maps_place_review_create(&h);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(h);
	}
	~test_env()
	{
		const int error = maps_place_review_destroy(h);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/
/*
* int maps_place_review_clone(const maps_place_review_h origin,
* maps_place_review_h* cloned); *
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

	maps_place_link_object_h via = __utc_prepare_place_link_object(
	"via id", "via string", "via type", "via name");
	error = maps_place_media_set_via(m, via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_place_link_object_destroy(via);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	return m;
}*/

void utc_maps_place_review_clone_p(void)
{
	test_env e;

	maps_place_review_h cloned = NULL;
	int error = maps_place_review_clone(e.h, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	error = maps_place_review_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

/*	* set test values for original *
	*------------------------------------ *
	int error = maps_place_review_set_date(e.h, "date");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_review_set_title(e.h, "title");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_review_set_rating(e.h, 6.0221);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_review_set_description(e.h, "description");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_review_set_language(e.h, "language");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_media_h media = __utc_prepare_place_media();
	error = maps_place_review_set_media(e.h, media);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_place_media_destroy(media);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_link_object_h user = __utc_prepare_place_link_object(
	"user id", "user string", "user type", "user name");
	error = maps_place_review_set_user_link(e.h, user);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_place_link_object_destroy(user);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	*------------------------------------ *

	maps_place_review_h cloned = NULL;
	error = maps_place_review_clone(e.h, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	* test if values are cloned correctly *
	*------------------------------------ *
	char* date = NULL;
	error = maps_place_review_get_date(cloned, &date);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(date, ==, "date");
	g_free(date);

	char* title = NULL;
	error = maps_place_review_get_title(cloned, &title);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(title, ==, "title");
	g_free(title);

	double rating = .0;
	error = maps_place_review_get_rating(cloned, &rating);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(rating, ==, 6.0221);

	char* description = NULL;
	error = maps_place_review_get_description(cloned, &description);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(description, ==, "description");
	g_free(description);

	char* language = NULL;
	error = maps_place_review_get_language(cloned, &language);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(language, ==, "language");
	g_free(language);

	{
		maps_place_media_h media_obtained = NULL;
		error = maps_place_review_get_media(cloned, &media_obtained);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(media_obtained);

		char* attribution = NULL;
		error = maps_place_media_get_attribution(media_obtained,
			&attribution);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(attribution, ==, "attribution");
		g_free(attribution);

		maps_place_link_object_h supplier_obtained = NULL;
		error = maps_place_media_get_supplier(media_obtained,
			&supplier_obtained);
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
			error = maps_place_link_object_get_name(
				supplier_obtained, &supplier_name);
			g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
			g_assert_cmpstr(supplier_name, ==, "supplier name");
			g_free(supplier_name);
		}
		maps_place_link_object_destroy(supplier_obtained);

		maps_place_link_object_h via_obtained = NULL;
		error = maps_place_media_get_via(media_obtained, &via_obtained);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(via_obtained);
		{ * Checking link object "via" *
			char* via_string = NULL;
			error = maps_place_link_object_get_string(
				via_obtained, &via_string);
			g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
			g_assert_cmpstr(via_string, ==, "via string");
			g_free(via_string);

			char* via_type = NULL;
			error = maps_place_link_object_get_type(
				via_obtained, &via_type);
			g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
			g_assert_cmpstr(via_type, ==, "via type");
			g_free(via_type);

			char* via_id = NULL;
			error = maps_place_link_object_get_id(
				via_obtained, &via_id);
			g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
			g_assert_cmpstr(via_id, ==, "via id");
			g_free(via_id);

			char* via_name = NULL;
			error = maps_place_link_object_get_name(
				via_obtained, &via_name);
			g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
			g_assert_cmpstr(via_name, ==, "via name");
			g_free(via_name);
		}
		maps_place_link_object_destroy(via_obtained);
		maps_place_media_destroy(media_obtained);
	}

	maps_place_link_object_h user_obtained = NULL;
	error = maps_place_review_get_user_link(cloned, &user_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(user_obtained);

	{ * Checking link object "supplier" *
		char* user_string = NULL;
		error = maps_place_link_object_get_string(
			user_obtained, &user_string);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(user_string, ==, "user string");
		g_free(user_string);

		char* user_type = NULL;
		error = maps_place_link_object_get_type(user_obtained,
			&user_type);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(user_type, ==, "user type");
		g_free(user_type);

		char* user_id = NULL;
		error = maps_place_link_object_get_id(user_obtained, &user_id);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(user_id, ==, "user id");
		g_free(user_id);

		char* user_name = NULL;
		error = maps_place_link_object_get_name(user_obtained,
			&user_name);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(user_name, ==, "user name");
		g_free(user_name);
	}
	maps_place_link_object_destroy(user_obtained);
	*------------------------------------ *

	error = maps_place_review_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);*/
}

void utc_maps_place_review_clone_n(void)
{
	test_env e;

	maps_place_review_h cloned = NULL;
	int error = maps_place_review_clone(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_review_set_date(maps_place_review_h place,
*  const char* date); */
/* int maps_place_review_get_date(maps_place_review_h place, char** date); */
void utc_maps_place_review_date_p(void)
{
	test_env e;

	int error = maps_place_review_set_date(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* str = NULL;
	error = maps_place_review_get_date(e.h, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(str, ==, "test_string");
	g_free(str);
}

void utc_maps_place_review_date_n(void)
{
	test_env e;

	int error = maps_place_review_set_date(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_set_date(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* str = NULL;
	error = maps_place_review_get_date(NULL, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_get_date(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_review_set_title(maps_place_review_h place,
*  const char* title); */
/* int maps_place_review_get_title(maps_place_review_h place, char** title); */
void utc_maps_place_review_title_p(void)
{
	test_env e;

	int error = maps_place_review_set_title(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* str = NULL;
	error = maps_place_review_get_title(e.h, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(str, ==, "test_string");
	g_free(str);
}

void utc_maps_place_review_title_n(void)
{
	test_env e;

	int error = maps_place_review_set_title(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_set_title(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* str = NULL;
	error = maps_place_review_get_title(NULL, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_get_title(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_review_set_rating(maps_place_review_h place,
*  const double rating); */
/* int maps_place_review_get_rating(maps_place_review_h place,
*  double* rating); */
void utc_maps_place_review_rating_p(void)
{
	test_env e;

	int error = maps_place_review_set_rating(e.h, 1.1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	double d = .0;
	error = maps_place_review_get_rating(e.h, &d);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(d, ==, 1.1);
}

void utc_maps_place_review_rating_n(void)
{
	int error = maps_place_review_set_rating(NULL, 1.1);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	double d = .0;
	error = maps_place_review_get_rating(NULL, &d);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_review_set_description(maps_place_review_h place,
*  const char* description); */
/* int maps_place_review_get_description(maps_place_review_h place,
*  char** description); */
void utc_maps_place_review_description_p(void)
{
	test_env e;

	int error = maps_place_review_set_description(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* str = NULL;
	error = maps_place_review_get_description(e.h, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(str, ==, "test_string");
	g_free(str);
}

void utc_maps_place_review_description_n(void)
{
	test_env e;

	int error = maps_place_review_set_description(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_set_description(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* str = NULL;
	error = maps_place_review_get_description(NULL, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_get_description(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_review_set_language(maps_place_review_h place,
*  const char* language); */
/* int maps_place_review_get_language(maps_place_review_h place,
*  char** language); */
void utc_maps_place_review_language_p(void)
{
	test_env e;

	int error = maps_place_review_set_language(e.h, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* str = NULL;
	error = maps_place_review_get_language(e.h, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(str, ==, "test_string");
	g_free(str);
}

void utc_maps_place_review_language_n(void)
{
	test_env e;

	int error = maps_place_review_set_language(NULL, "test_string");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_set_language(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* str = NULL;
	error = maps_place_review_get_language(NULL, &str);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_get_language(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_review_set_media(maps_place_review_h place,
*  maps_place_media_h media); */
/* int maps_place_review_get_media(maps_place_review_h place,
*  maps_place_media_h* media); */
void utc_maps_place_review_media_p(void)
{
	test_env e;

	maps_place_media_h obj = NULL;
	int error = maps_place_media_create(&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_review_set_media(e.h, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_media_h obj_obtained = NULL;
	error = maps_place_review_get_media(e.h, &obj_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obj_obtained);
	maps_place_media_destroy(obj_obtained);

	error = maps_place_media_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_review_media_n(void)
{
	test_env e;

	maps_place_media_h obj = NULL;
	int error = maps_place_media_create(&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_review_set_media(NULL, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_set_media(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_place_media_h obj_obtain = NULL;
	error = maps_place_review_get_media(NULL, &obj_obtain);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_get_media(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_media_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_place_review_set_user_link(maps_place_review_h place,
*  maps_place_link_object_h user); */
/* int maps_place_review_get_user_link(maps_place_review_h place,
*  maps_place_link_object_h* user); */
void utc_maps_place_review_user_link_p(void)
{
	test_env e;

	maps_place_link_object_h obj = NULL;
	int error = maps_place_link_object_create(&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_review_set_user_link(e.h, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_place_link_object_h obj_obtained = NULL;
	error = maps_place_review_get_user_link(e.h, &obj_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(obj_obtained);

	maps_place_link_object_destroy(obj_obtained);

	error = maps_place_link_object_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_review_user_link_n(void)
{
	test_env e;

	maps_place_link_object_h obj = NULL;
	int error = maps_place_link_object_create(&obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_review_set_user_link(NULL, obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_set_user_link(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_place_link_object_h obj_obtain = NULL;
	error = maps_place_review_get_user_link(NULL, &obj_obtain);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_review_get_user_link(e.h, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_link_object_destroy(obj);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

