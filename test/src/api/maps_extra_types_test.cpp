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

#include "maps_extra_types_test.h"
#include "maps_extra_types.h"
#include "maps_error.h"
#include <glib.h>
#include "maps_coordinates.h"
#include "maps_service.h"
#include "maps_test_utils.h"

/* int maps_item_list_create(maps_item_list_h* list); */
/* int maps_item_list_destroy(maps_item_list_h list); */
void utc_maps_item_list_create_p(void)
{
	maps_item_list_h list = NULL;
	int error = maps_item_list_create(&list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(list);

	error = maps_item_list_destroy(list);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_item_list_create_n(void)
{
	int error = maps_item_list_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_list_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

class test_env_list
{
 public:
	maps_item_list_h list;
 public:
	test_env_list():list(NULL)
	{
		const int error = maps_item_list_create(&list);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(list);
	}
	~test_env_list()
	{
		const int error = maps_item_list_destroy(list);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/* int maps_item_list_append(maps_item_list_h list, const void* data); */
/* int maps_item_list_remove(maps_item_list_h list); */
void utc_maps_item_list_append_p(void)
{
	test_env_list e;

	const char* str = "test string";
	int error =
		maps_item_list_append(e.list, str,
		maps_item_hashtable_clone_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_remove_all(e.list,
		maps_item_hashtable_free_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_item_list_append_n(void)
{
	test_env_list e;

	const char* str = "test string";

	int error =
		maps_item_list_append(NULL, str,
		maps_item_hashtable_clone_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_list_append(e.list, NULL,
		maps_item_hashtable_clone_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_list_append(e.list, str, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

}

/* typedef bool (*maps_item_list_foreach_cb)(void *data, void *user_data); */
/* int maps_item_list_foreach(maps_item_list_h list, maps_item_list_foreach_cb
*  callback, void* user_data); */
static bool __utc_maps_item_list_foreach_cb(int index, int total, void* data,
	void* user_data)
{
	char* str = (char*) data;

	g_assert_cmpint(total, ==, 3);

	if ((g_strcmp0(str, "AAA") == 0) || (g_strcmp0(str, "BBBB") == 0)
		|| (g_strcmp0(str, "CCCCC") == 0)) {
		/* OK */
	}
	else {
		g_print("\n\n%s\n\n", str);
		g_assert(false);
	}

	/*maps_service_string_release((char*)data); */
	if (data)
		g_free((char*) data);

	return true;
}

static int __utc_maps_item_list_free_cb(void* data)
{
	/*return maps_service_string_release((char*)data); */
	if (data)
		g_free((char*) data);
	return MAPS_ERROR_NONE;
}

void utc_maps_item_list_foreach_p(void)
{
	test_env_list e;

	int error =
		maps_item_list_append(e.list, "AAA",
		maps_item_hashtable_clone_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_append(e.list, "BBBB",
		maps_item_hashtable_clone_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_append(e.list, "CCCCC",
		maps_item_hashtable_clone_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_foreach(e.list, maps_item_hashtable_clone_string,
		__utc_maps_item_list_foreach_cb, NULL);

	error = maps_item_list_remove_all(e.list, __utc_maps_item_list_free_cb);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_item_list_foreach_n(void)
{
	test_env_list e;

	int error =
		maps_item_list_foreach(NULL, maps_item_hashtable_clone_string,
		__utc_maps_item_list_foreach_cb, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_list_foreach(e.list, NULL,
		__utc_maps_item_list_foreach_cb, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_list_foreach(e.list, maps_item_hashtable_clone_string,
		NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_item_list_get_data(maps_item_list_h list, void** data); */
/* int maps_item_list_get_next(maps_item_list_h list); */

/*----------------------------------------------------------------------------*/

/* int maps_string_hashtable_create(maps_string_hashtable_h* table); */
/* int maps_string_hashtable_destroy(maps_string_hashtable_h table); */
void utc_maps_string_hashtable_create_p(void)
{
	maps_string_hashtable_h table = NULL;
	int error = maps_string_hashtable_create(&table);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(table);

	error = maps_string_hashtable_destroy(table);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_string_hashtable_create_n(void)
{
	int error = maps_string_hashtable_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_string_hashtable_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

class test_env_string_hashtable
{
 public:
	maps_string_hashtable_h table;
 public:
	test_env_string_hashtable():table(NULL)
	{
		const int error = maps_string_hashtable_create(&table);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(table);
	}
	~test_env_string_hashtable()
	{
		const int error = maps_string_hashtable_destroy(table);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/* int maps_string_hashtable_set(maps_string_hashtable_h table, char* key,
*  const char* value); */
/* int maps_string_hashtable_get(maps_string_hashtable_h table, char* key,
*  char** value); */
/* int maps_string_hashtable_remove(maps_string_hashtable_h map,
*  const char* key); */
void utc_maps_string_hashtable_set_p(void)
{
	test_env_string_hashtable e;

	int error = maps_string_hashtable_set(e.table, "key1", "value1");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_string_hashtable_set(e.table, "key2", "value2");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_string_hashtable_set(e.table, "key3", "value3");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* value1 = NULL;
	error = maps_string_hashtable_get(e.table, "key1", &value1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_free(value1);

	char* value2 = NULL;
	error = maps_string_hashtable_get(e.table, "key2", &value2);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_free(value2);

	char* value3 = NULL;
	error = maps_string_hashtable_get(e.table, "key3", &value3);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_free(value3);
}

void utc_maps_string_hashtable_set_n(void)
{
	test_env_string_hashtable e;

	int error = maps_string_hashtable_set(NULL, "key1", "value1");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_string_hashtable_set(e.table, NULL, "value1");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_string_hashtable_set(e.table, "key1", NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* typedef bool (*maps_string_hashtable_foreach_cb)(char *key, char *value,
*  void* user_data); */
/* int maps_string_hashtable_foreach(maps_string_hashtable_h table,
*  maps_string_hashtable_foreach_cb callback, void* user_data); */
static bool __utc_maps_string_hashtable_foreach_cb(int index, int total,
	char* key, char* value, void* user_data)
{
	g_assert_cmpint(total, ==, 3);

	if (g_strcmp0(key, "key1") == 0) {
		g_assert_cmpstr(value, ==, "value1");

	}
	else if (g_strcmp0(key, "key2") == 0) {
		g_assert_cmpstr(value, ==, "value2");

	}
	else if (g_strcmp0(key, "key3") == 0) {
		g_assert_cmpstr(value, ==, "value3");

	}
	else {
		g_print("\n\n%s, %s\n\n", key, value);
		g_assert(false);
	}
	g_free(key);
	g_free(value);
	return true;
}

void utc_maps_string_hashtable_foreach_p(void)
{
	test_env_string_hashtable e;

	int error = maps_string_hashtable_set(e.table, "key1", "value1");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_string_hashtable_set(e.table, "key2", "value2");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_string_hashtable_set(e.table, "key3", "value3");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_string_hashtable_foreach(e.table,
		__utc_maps_string_hashtable_foreach_cb, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_string_hashtable_foreach_n(void)
{
	test_env_string_hashtable e;

	int error =
		maps_string_hashtable_foreach(NULL,
		__utc_maps_string_hashtable_foreach_cb, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_string_hashtable_foreach(e.table, NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/

/* int maps_item_hashtable_create(maps_item_hashtable_h* table); */
/* int maps_item_hashtable_destroy(maps_item_hashtable_h table); */
void utc_maps_item_hashtable_create_p(void)
{
	maps_item_hashtable_h table = NULL;
	int error = maps_item_hashtable_create(&table);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(table);

	error = maps_item_hashtable_destroy(table);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_item_hashtable_create_n(void)
{
	int error = maps_item_hashtable_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

class test_env_item_hashtable
{
 public:
	maps_item_hashtable_h table;
 public:
	test_env_item_hashtable():table(NULL)
	{
		const int error = maps_item_hashtable_create(&table);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(table);
	}
	~test_env_item_hashtable()
	{
		const int error = maps_item_hashtable_destroy(table);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/* int maps_item_hashtable_set(maps_item_hashtable_h table, char* key,
*  const char* value); */
/* int maps_item_hashtable_get(maps_item_hashtable_h table, char* key,
*  char** value); */
/* int maps_item_hashtable_remove(maps_item_hashtable_h map,
*  const char* key); */
void utc_maps_item_hashtable_set_p(void)
{
	test_env_item_hashtable e;

	{			/* Setting items to the table */

		/* First item is a string */
		int error =
			maps_item_hashtable_set(e.table, "key1", "value1",
			maps_item_hashtable_clone_string,
			maps_item_hashtable_free_string);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		/* Second item is an integer */
		int value2 = 2;
		error = maps_item_hashtable_set(e.table, "key2", &value2,
			maps_item_hashtable_clone_int,
			maps_item_hashtable_free_int);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		/* Third item is coordinates */
		maps_coordinates_h value3 = NULL;
		error = maps_coordinates_create(11.1, 22.2,
#if _MAPS_COORDS_3D_
			33.3,
#endif
			&value3);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(value3);
		error = maps_item_hashtable_set(e.table, "key3", value3,
			maps_coordinates_clone, maps_coordinates_destroy);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		error = maps_coordinates_destroy(value3);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}

	{			/* Getting items from the table */

		/* Try to get the first item: string */
		char* value1 = NULL;
		int error =
			maps_item_hashtable_get(e.table, "key1",
			(void**) &value1);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(value1, ==, "value1");
		g_free(value1);

		/* Try to get the second item: integer */
		int* value2 = NULL;
		error = maps_item_hashtable_get(e.table, "key2",
			(void**) &value2);
		if (error != MAPS_ERROR_NONE)
			__utc_print_error_string(error);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpint(*value2, ==, 2);
		maps_item_hashtable_free_int(value2);

		/* Try to get the third item: coordinates */
		maps_coordinates_h value3 = NULL;
		error = maps_item_hashtable_get(e.table, "key3",
			(void**) &value3);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		double lat = .0;
		error = maps_coordinates_get_latitude(value3, &lat);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpfloat(lat, ==, 11.1);

		double lon = .0;
		error = maps_coordinates_get_longitude(value3, &lon);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpfloat(lon, ==, 22.2);

		maps_coordinates_destroy(value3);
	}
}

void utc_maps_item_hashtable_set_n(void)
{
	test_env_item_hashtable e;

	int error =
		maps_item_hashtable_set(NULL, "key1", "value1",
		maps_item_hashtable_clone_string,
		maps_item_hashtable_free_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set(e.table, NULL, "value1",
		maps_item_hashtable_clone_string,
		maps_item_hashtable_free_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set(e.table, "key1", NULL,
		maps_item_hashtable_clone_string,
		maps_item_hashtable_free_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set(e.table, "key1", "value1", NULL,
		maps_item_hashtable_free_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set(e.table, "key1", "value1",
		maps_item_hashtable_clone_string, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* typedef bool (*maps_item_hashtable_foreach_cb)(char *key, char *value,
*  void* user_data); */
/* int maps_item_hashtable_foreach(maps_item_hashtable_h table,
*  maps_item_hashtable_foreach_cb callback, void* user_data); */
static bool __utc_maps_item_hashtable_foreach_cb(int index, int total,
	char* key, void* value, void* user_data)
{
	g_assert_cmpint(total, ==, 3);

	if (g_strcmp0(key, "key1") == 0) {
		g_assert_cmpstr((char*) value, ==, "value1");

	}
	else if (g_strcmp0(key, "key2") == 0) {
		g_assert_cmpstr((char*) value, ==, "value2");

	}
	else if (g_strcmp0(key, "key3") == 0) {
		g_assert_cmpstr((char*) value, ==, "value3");

	}
	else {
		g_print("\n\n%s, %s\n\n", key, (const char*) value);
		g_assert(false);
	}

	g_free((char*) key);
	g_free((char*) value);
	return true;
}

void utc_maps_item_hashtable_foreach_p(void)
{
	test_env_item_hashtable e;

	int error =
		maps_item_hashtable_set(e.table, "key1", "value1",
		maps_item_hashtable_clone_string,
		maps_item_hashtable_free_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_item_hashtable_set(e.table, "key2", "value2",
		maps_item_hashtable_clone_string,
		maps_item_hashtable_free_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_item_hashtable_set(e.table, "key3", "value3",
		maps_item_hashtable_clone_string,
		maps_item_hashtable_free_string);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_hashtable_foreach(e.table,
		__utc_maps_item_hashtable_foreach_cb, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_item_hashtable_foreach_n(void)
{
	test_env_item_hashtable e;

	int error =
		maps_item_hashtable_foreach(NULL,
		__utc_maps_item_hashtable_foreach_cb, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_foreach(e.table, NULL, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* #define maps_item_hashtable_set_string(table, key, value)
*  maps_item_hashtable_set(table, key, value, maps_item_hashtable_clone_string,
*  maps_item_hashtable_free_string) */
/* #define maps_item_hashtable_set_int(table, key, value)
*  maps_item_hashtable_set(table, key, value, maps_item_hashtable_clone_int,
*  maps_item_hashtable_free_int) */
/* #define maps_item_hashtable_set_float(table, key, value)
*  maps_item_hashtable_set(table, key, value, maps_item_hashtable_clone_float,
*  maps_item_hashtable_free_float) */
void utc_maps_item_hashtable_set_macro_p(void)
{
	test_env_item_hashtable e;

	{			/* Setting items to the table */
		/* First item is a string */
		int error =
			maps_item_hashtable_set_string(e.table, "key1",
			"value1");
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		/* Second item is an integer */
		error = maps_item_hashtable_set_int(e.table, "key2", 2);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		/* Third item is float */
		error = maps_item_hashtable_set_float(e.table, "key3", .3);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}

	{			/* Getting items from the table */

		/* Try to get the first item: string */
		char* value1 = NULL;
		int error =
			maps_item_hashtable_get(e.table, "key1",
			(void**) &value1);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpstr(value1, ==, "value1");
		maps_item_hashtable_free_string(value1);

		/* Try to get the second item: integer */
		int* value2 = NULL;
		error = maps_item_hashtable_get(e.table, "key2",
			(void**) &value2);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpint(*value2, ==, 2);
		maps_item_hashtable_free_int(value2);

		/* Try to get the third item: float */
		double* value3 = NULL;
		error = maps_item_hashtable_get(e.table, "key3",
			(void**) &value3);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpfloat(*value3, ==, .3);
		maps_item_hashtable_free_float(value3);
	}
}

void utc_maps_item_hashtable_set_macro_n(void)
{
	test_env_item_hashtable e;

	int error = maps_item_hashtable_set_string(NULL, "key1", "value1");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set_string(e.table, NULL, "value1");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set_string(e.table, "key1", NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set_int(NULL, "key2", 2);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set_int(e.table, NULL, 2);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set_float(NULL, "key3", .3);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_item_hashtable_set_float(e.table, NULL, .3);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

void utc_maps_item_hashtable_set_overwrited_macro_p(void)
{
	test_env_item_hashtable e;

	{			/* Setting items to the table */

		/* First item is a string: SET */
		int error =
			maps_item_hashtable_set_string(e.table, "key1",
			"value1");
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		/* Second item is an integer: OVERWRITE */
		error = maps_item_hashtable_set_int(e.table, "key1", 2);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

		/* Third item is float: OVERWRITE */
		error = maps_item_hashtable_set_float(e.table, "key1", .3);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}

	{			/* Getting item from the table */

		/* Try to get the item: float */
		double* value3 = NULL;
		int error =
			maps_item_hashtable_get(e.table, "key1",
			(void**) &value3);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert_cmpfloat(*value3, ==, .3);

		maps_item_hashtable_free_float(value3);
	}
}

