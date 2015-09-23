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

#include <glib.h>
#include "maps_error.h"
#include "maps_extra_types.h"
#include "maps_extra_types_private.h"
#include "maps_util.h"

typedef struct _maps_item_list_s
{
	GList *l;
} maps_item_list_s;

EXPORT_API int maps_item_list_create(maps_item_list_h *list)
{
	MAPS_LOG_API;
	if (!list)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_list_s *l = g_slice_new0(maps_item_list_s);

	if (!l) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	l->l = NULL;

	*list = (maps_item_list_h *) l;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_list_destroy(maps_item_list_h list)
{
	MAPS_LOG_API;
	if (!list)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_list_s *l = (maps_item_list_s *) list;
	if (l->l)
		g_list_free(l->l);
	g_slice_free(maps_item_list_s, l);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_list_append(maps_item_list_h list, const void *data,
				     maps_item_list_clone_cb clone_func)
{
	MAPS_LOG_API;
	if (!list || !data || !clone_func)
		return MAPS_ERROR_INVALID_PARAMETER;
	void *p = NULL;
	const int error = clone_func((void *) data, &p);
	if (error != MAPS_ERROR_NONE)
		return error;
	maps_item_list_s *l = (maps_item_list_s *) list;
	l->l = g_list_append(l->l, (gpointer) p);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_list_foreach(maps_item_list_h list,
				      maps_item_list_clone_cb clone_func,
				      maps_item_list_foreach_cb callback,
	void *user_data)
{
	MAPS_LOG_API;
	if (!list || !clone_func || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_list_s *l = (maps_item_list_s *) list;

	if (!l->l)
		return MAPS_ERROR_NOT_FOUND;

	const int total = g_list_length(l->l);
	GList *head = g_list_first(l->l);
	int index = 0;
	while (head) {
		void *data = head->data;
		head = head->next;
		void *clone = NULL;
		if (clone_func(data, &clone) != MAPS_ERROR_NONE)
			continue;
		if (!callback(index++, total, clone, user_data))
			break;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_list_remove(maps_item_list_h list,
				     void *item,
				     maps_item_list_free_cb free_func)
{
	MAPS_LOG_API;
	if (!list)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_item_list_s *l = (maps_item_list_s *) list;
	if (l->l) {
		l->l = g_list_remove(l->l, item);
		if (free_func)
			free_func(item);
	}
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_list_remove_all(maps_item_list_h list,
					 maps_item_list_free_cb free_func)
{
	MAPS_LOG_API;
	if (!list)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_list_s *l = (maps_item_list_s *) list;
	if (l->l) {
		GList *head = g_list_first(l->l);
		while (head) {
			if (free_func)
				free_func(head->data);
			head = head->next;
		}

		g_list_free(l->l);
		l->l = NULL;
	}
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_list_clone(const maps_item_list_h origin,
				    maps_item_list_clone_cb clone_func,
				    maps_item_list_h *cloned)
{
	MAPS_LOG_API;
	if (!cloned || !origin || !clone_func)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {

		error = maps_item_list_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_item_list_s *l = (maps_item_list_s *) origin;
		if (!l->l)
			return MAPS_ERROR_NONE;

		maps_item_list_s *l_cloned = (maps_item_list_s *) (*cloned);

		/* It would be cool when we will have glib v 2.34 */
		/*l_cloned->l = g_list_copy_deep(l->l, clone_func, NULL); */
		/* Now we iterate */
		GList *head = g_list_first(l->l);
		while (head) {
			void *cloned_item = NULL;
			const int error = clone_func(head->data, &cloned_item);
			if (error == MAPS_ERROR_NONE)
				l_cloned->l =
					g_list_append(l_cloned->l,
					(gpointer) cloned_item);
			head = head->next;
		}
		return MAPS_ERROR_NONE;

	} while (false);

	maps_item_list_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

typedef struct _maps_string_hashtable_s
{
	GHashTable *t;
} maps_string_hashtable_s;

EXPORT_API int maps_string_hashtable_create(maps_string_hashtable_h *table)
{
	MAPS_LOG_API;
	if (!table)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_string_hashtable_s *t = g_slice_new0(maps_string_hashtable_s);

	if (!t) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	t->t = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
	*table = t;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_string_hashtable_destroy(maps_string_hashtable_h table)
{
	MAPS_LOG_API;
	if (!table)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_string_hashtable_s *t = (maps_string_hashtable_s *) table;
	if (t->t)
		g_hash_table_unref(t->t);
	g_slice_free(maps_string_hashtable_s, t);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_string_hashtable_set(maps_string_hashtable_h table,
					 const char *key, const char *value)
{
	MAPS_LOG_API;
	if (!table || !key || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_string_hashtable_s *t = (maps_string_hashtable_s *) table;
	if (t->t)
		g_hash_table_insert(t->t, (gpointer) g_strdup(key),
			(gpointer) g_strdup(value));
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_string_hashtable_get(maps_string_hashtable_h table,
					 const char *key, char **value)
{
	MAPS_LOG_API;
	if (!table || !key || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_string_hashtable_s *t = (maps_string_hashtable_s *) table;
	if (!t->t)
		return MAPS_ERROR_NOT_FOUND;
	char *table_value = (char *) g_hash_table_lookup(t->t, key);
	if (!table_value)
		return MAPS_ERROR_NONE;
	return maps_get_string(table_value, strlen(table_value), value);
}

EXPORT_API int maps_string_hashtable_remove(maps_string_hashtable_h table,
					    const char *key)
{
	MAPS_LOG_API;
	if (!table || !key)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_string_hashtable_s *t = (maps_string_hashtable_s *) table;
	if (t->t)
		g_hash_table_remove(t->t, (gpointer) key);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_string_hashtable_foreach(maps_string_hashtable_h table,
					     maps_string_hashtable_foreach_cb
					     callback,
					     void *user_data)
{
	MAPS_LOG_API;
	if (!table || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_string_hashtable_s *t = (maps_string_hashtable_s *) table;

	if (!t->t)
		return MAPS_ERROR_NOT_FOUND;

	GHashTableIter iter;
	gpointer key, value;
	const int total = g_hash_table_size(t->t);
	int index = 0;
	g_hash_table_iter_init(&iter, t->t);
	while (g_hash_table_iter_next(&iter, &key, &value)) {

		char *key_clone = NULL;
		if (maps_item_hashtable_clone_string(key,
				(void **) &key_clone) != MAPS_ERROR_NONE)
			continue;
		char *value_clone = NULL;
		if (maps_item_hashtable_clone_string(value,
				(void **) &value_clone) != MAPS_ERROR_NONE)
			continue;

		if (!callback(index++, total, key_clone, value_clone,
				user_data))
			break;

	}
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_string_hashtable_clone(const maps_string_hashtable_h origin,
					   maps_string_hashtable_h *cloned)
{
	MAPS_LOG_API;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {

		error = maps_string_hashtable_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_string_hashtable_s *t = (maps_string_hashtable_s *) origin;

		if (!t->t)
			return MAPS_ERROR_NONE;

		maps_string_hashtable_s *t_cloned =
			(maps_string_hashtable_s *) (*cloned);

		GHashTableIter iter;
		gpointer key, value;
		g_hash_table_iter_init(&iter, t->t);
		while (g_hash_table_iter_next(&iter, &key, &value)) {
			g_hash_table_insert(t_cloned->t,
				(gpointer) g_strdup((const gchar *) key),
				(gpointer) g_strdup((const gchar *) value));
		}

		return MAPS_ERROR_NONE;

	} while (false);

	maps_string_hashtable_destroy(*cloned);
	*cloned = NULL;
	return error;
}

EXPORT_API int maps_string_hashtable_contains(maps_string_hashtable_h table,
					      const char * key, bool * contains)
{
	MAPS_LOG_API;
	if (!table || !key || !contains)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_string_hashtable_s *t = (maps_string_hashtable_s *) table;
	if (t->t)
		*contains = g_hash_table_contains(t->t, (gpointer) key);
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/

typedef struct _maps_int_hashtable_s
{
	GHashTable *t;
} maps_int_hashtable_s;

EXPORT_API int maps_int_hashtable_create(maps_int_hashtable_h *table)
{
	MAPS_LOG_API;
	if (!table)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_int_hashtable_s *t = g_slice_new0(maps_int_hashtable_s);

	if (!t) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	t->t = g_hash_table_new_full(g_int_hash, g_int_equal, g_free, g_free);
	*table = t;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_int_hashtable_destroy(maps_int_hashtable_h table)
{
	MAPS_LOG_API;
	if (!table)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_int_hashtable_s *t = (maps_int_hashtable_s *) table;
	if (t->t)
		g_hash_table_unref(t->t);
	g_slice_free(maps_int_hashtable_s, t);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_int_hashtable_set(maps_int_hashtable_h table,
				      const int key, const int value)
{
	MAPS_LOG_API;
	if (!table)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_int_hashtable_s *t = (maps_int_hashtable_s *) table;
	if (t->t) {
		int *new_key = g_new0(int, 1);
		*new_key = key;
		int *new_value = g_new0(int, 1);
		*new_value = value;
		g_hash_table_insert(t->t,
				    (gpointer)new_key,
				    (gpointer)new_value);
	}
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_int_hashtable_get(maps_int_hashtable_h table,
				      const int key, int *value)
{
	MAPS_LOG_API;
	if (!table || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_int_hashtable_s *t = (maps_int_hashtable_s *) table;
	if (!t->t)
		return MAPS_ERROR_NOT_FOUND;
	const int *table_value = (const int *) g_hash_table_lookup(t->t, &key);
	if (!table_value)
		return MAPS_ERROR_NONE;
	*value = *table_value;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_int_hashtable_remove(maps_int_hashtable_h table,
					 const int key)
{
	MAPS_LOG_API;
	if (!table)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_int_hashtable_s *t = (maps_int_hashtable_s *) table;
	if (t->t)
		g_hash_table_remove(t->t, (gpointer) &key);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_int_hashtable_foreach(maps_int_hashtable_h table,
					  maps_int_hashtable_foreach_cb
					  callback,
					  void *user_data)
{
	MAPS_LOG_API;
	if (!table || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_int_hashtable_s *t = (maps_int_hashtable_s *) table;

	if (!t->t)
		return MAPS_ERROR_NOT_FOUND;

	GHashTableIter iter;
	gpointer key, value;
	const int total = g_hash_table_size(t->t);
	int index = 0;
	g_hash_table_iter_init(&iter, t->t);
	while (g_hash_table_iter_next(&iter, &key, &value)) {

		if(!key || !value)
			continue;

		int *key_ptr = (int *)key;
		int *value_ptr = (int *)value;
		if (!callback(index++, total, *key_ptr, *value_ptr, user_data))
			break;

	}
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_int_hashtable_clone(const maps_int_hashtable_h origin,
					maps_int_hashtable_h *cloned)
{
	MAPS_LOG_API;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {

		error = maps_int_hashtable_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_int_hashtable_s *t = (maps_int_hashtable_s *) origin;

		if (!t->t)
			return MAPS_ERROR_NONE;

		maps_int_hashtable_s *t_cloned =
			(maps_int_hashtable_s *) (*cloned);

		GHashTableIter iter;
		gpointer key, value;
		g_hash_table_iter_init(&iter, t->t);
		while (g_hash_table_iter_next(&iter, &key, &value)) {
			int *new_key = g_new0(int, 1);
			*new_key = *((int *)key);
			int *new_value = g_new0(int, 1);
			*new_value = *((int *)value);
			g_hash_table_insert(t_cloned->t,
				(gpointer) new_key,
				(gpointer) new_value);
		}

		return MAPS_ERROR_NONE;

	} while (false);

	maps_int_hashtable_destroy(*cloned);
	*cloned = NULL;
	return error;
}

EXPORT_API int maps_int_hashtable_contains(maps_int_hashtable_h table,
					   const int key, bool *contains)
{
	MAPS_LOG_API;
	if (!table || !contains)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_int_hashtable_s *t = (maps_int_hashtable_s *) table;
	if (t->t)
		*contains = g_hash_table_contains(t->t, (gpointer)&key);
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/

typedef struct _maps_hashtable_item_s
{
	void *value;
	maps_item_hashtable_clone_cb clone_func;
	maps_item_hashtable_free_cb free_func;
} maps_hashtable_item_s;

void _maps_hashtable_item_create(maps_hashtable_item_s **item)
{
	*item = g_slice_new0(maps_hashtable_item_s);

	if (*item == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
	}
}

void _maps_hashtable_item_destroy(gpointer data)
{
	if (!data)
		return;

	maps_hashtable_item_s *item = (maps_hashtable_item_s *) data;
	if (item && item->value && item->free_func)
		item->free_func(item->value);

	g_slice_free(maps_hashtable_item_s, item);
}

/*----------------------------------------------------------------------------*/

typedef struct _maps_item_hashtable_s
{
	GHashTable *t;
} maps_item_hashtable_s;

EXPORT_API int maps_item_hashtable_create(maps_item_hashtable_h *table)
{
	MAPS_LOG_API;
	if (!table)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_hashtable_s *t = g_slice_new0(maps_item_hashtable_s);

	if (!t) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	t->t = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
		_maps_hashtable_item_destroy);
	*table = t;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_destroy(maps_item_hashtable_h table)
{
	MAPS_LOG_API;
	if (!table)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_hashtable_s *t = (maps_item_hashtable_s *) table;
	if (t->t)
		g_hash_table_unref(t->t);
	g_slice_free(maps_item_hashtable_s, t);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_clone_string(void *origin, void **cloned)
{
	MAPS_LOG_API;
	if (!origin || !cloned)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string((char *) origin, strlen((char *) origin),
		(char **) cloned);
}

EXPORT_API int maps_item_hashtable_free_string(void *data)
{
	MAPS_LOG_API;
	g_free(data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_clone_int(void *origin, void **cloned)
{
	MAPS_LOG_API;
	if (!origin || !cloned)
		return MAPS_ERROR_INVALID_PARAMETER;
	int *n_cloned = g_new0(int, 1);

	if (n_cloned == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	int *n_origin = (int *) origin;
	*n_cloned = *n_origin;
	*cloned = n_cloned;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_free_int(void *data)
{
	MAPS_LOG_API;
	g_free(data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_clone_float(void *origin, void **cloned)
{
	MAPS_LOG_API;
	if (!origin || !cloned)
		return MAPS_ERROR_INVALID_PARAMETER;
	double *n_cloned = g_new0(double, 1);

	if (n_cloned == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	double *n_origin = (double *) origin;
	*n_cloned = *n_origin;
	*cloned = n_cloned;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_free_float(void *data)
{
	MAPS_LOG_API;
	g_free(data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_set_string(maps_item_hashtable_h table,
					      const char *key,
					      const char *value)
{
	MAPS_LOG_API;
	return maps_item_hashtable_set(table, key, value,
		maps_item_hashtable_clone_string,
		maps_item_hashtable_free_string);
}

EXPORT_API int maps_item_hashtable_set_int(maps_item_hashtable_h table,
					   const char *key, const int value)
{
	MAPS_LOG_API;
	return maps_item_hashtable_set(table, key, &value,
		maps_item_hashtable_clone_int, maps_item_hashtable_free_int);
}

EXPORT_API int maps_item_hashtable_set_float(maps_item_hashtable_h table,
					     const char *key,
					     const double value)
{
	MAPS_LOG_API;
	return maps_item_hashtable_set(table, key, &value,
		maps_item_hashtable_clone_float,
		maps_item_hashtable_free_float);
}

EXPORT_API int maps_item_hashtable_set(maps_item_hashtable_h table,
				       const char *key, const void *value,
	maps_item_hashtable_clone_cb clone_func,
	maps_item_hashtable_free_cb free_func)
{
	MAPS_LOG_API;
	if (!table || !key || !value || !clone_func || !free_func)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_hashtable_s *t = (maps_item_hashtable_s *) table;

	if (t->t) {
		maps_hashtable_item_s *item = NULL;
		_maps_hashtable_item_create(&item);

		const int error = clone_func((void *) value, &item->value);
		if (error != MAPS_ERROR_NONE) {
			_maps_hashtable_item_destroy(item);
			return error;
		}
		item->clone_func = clone_func;
		item->free_func = free_func;
		if (t->t)
			g_hash_table_insert(t->t, (gpointer) g_strdup(key),
				(gpointer) item);
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_get_string(maps_item_hashtable_h table,
					      const char *key, char **value)
{
	MAPS_LOG_API;
	return maps_item_hashtable_get(table, key, (void **) value);
}

EXPORT_API int maps_item_hashtable_get_int(maps_item_hashtable_h table,
					   const char *key, int *value)
{
	MAPS_LOG_API;
	if (!table || !key || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	int *data = NULL;
	const int error = maps_item_hashtable_get(table, key, (void **) &data);
	if (error == MAPS_ERROR_NONE) {
		*value = *data;
		maps_item_hashtable_free_int(data);
	}
	return error;
}

EXPORT_API int maps_item_hashtable_get_float(maps_item_hashtable_h table,
					     const char *key, double *value)
{
	MAPS_LOG_API;
	if (!table || !key || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	double *data = NULL;
	const int error = maps_item_hashtable_get(table, key, (void **) &data);
	if (error == MAPS_ERROR_NONE) {
		*value = *data;
		maps_item_hashtable_free_float(data);
	}
	return error;
}

EXPORT_API int maps_item_hashtable_get(maps_item_hashtable_h table,
				       const char *key, void **value)
{
	MAPS_LOG_API;
	if (!table || !key || !value)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_hashtable_s *t = (maps_item_hashtable_s *) table;
	if (!t->t)
		return MAPS_ERROR_NOT_FOUND;
	maps_hashtable_item_s *item =
		(maps_hashtable_item_s *) g_hash_table_lookup(t->t, key);
	if (!item || !item->clone_func)
		return MAPS_ERROR_NOT_FOUND;
	return item->clone_func(item->value, value);
}

EXPORT_API int maps_item_hashtable_remove(maps_item_hashtable_h table,
					  const char *key)
{
	MAPS_LOG_API;
	if (!table || !key)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_hashtable_s *t = (maps_item_hashtable_s *) table;
	if (t->t)
		g_hash_table_remove(t->t, (gpointer) key);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_foreach(maps_item_hashtable_h table,
					   maps_item_hashtable_foreach_cb
					   callback,
					   void *user_data)
{
	MAPS_LOG_API;
	if (!table || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_hashtable_s *t = (maps_item_hashtable_s *) table;

	if (!t->t)
		return MAPS_ERROR_NOT_FOUND;

	GHashTableIter iter;
	gpointer key, value;
	const int total = g_hash_table_size(t->t);
	int index = 0;
	g_hash_table_iter_init(&iter, t->t);
	while (g_hash_table_iter_next(&iter, &key, &value)) {
		maps_hashtable_item_s *item = (maps_hashtable_item_s *) value;
		if (!item || !item->clone_func)
			continue;

		char *key_clone = NULL;
		if (maps_item_hashtable_clone_string(key,
				(void **) &key_clone) != MAPS_ERROR_NONE)
			continue;
		void *value_clone = NULL;
		if (item->clone_func(item->value,
				&value_clone) != MAPS_ERROR_NONE)
			continue;

		if (!callback(index++, total, key_clone, value_clone,
				user_data))
			break;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_item_hashtable_clone(const maps_item_hashtable_h origin,
					 maps_item_hashtable_h* cloned)
{
	MAPS_LOG_API;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {

		error = maps_item_hashtable_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_item_hashtable_s *t = (maps_item_hashtable_s *) origin;

		if (!t->t)
			return MAPS_ERROR_NONE;

		GHashTableIter iter;
		gpointer key, value;
		g_hash_table_iter_init(&iter, t->t);
		while (g_hash_table_iter_next(&iter, &key, &value)) {
			maps_hashtable_item_s *item =
				(maps_hashtable_item_s *) value;
			if (!item)
				continue;
			maps_item_hashtable_set(*cloned, (const char *) key,
				(const void *) item->value, item->clone_func,
				item->free_func);
		}

		return MAPS_ERROR_NONE;

	} while (false);

	maps_item_hashtable_destroy(*cloned);
	*cloned = NULL;
	return error;
}

EXPORT_API int maps_item_hashtable_contains(maps_item_hashtable_h table,
					    const char *key, bool *contains)
{
	MAPS_LOG_API;
	if (!table || !key || !contains)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_hashtable_s *t = (maps_item_hashtable_s *) table;
	if (t->t)
		*contains = g_hash_table_contains(t->t, (gpointer) key);
	return MAPS_ERROR_NONE;
}

/**
 * This is the function, needed by maps_item_list_append.
 * It allows not cloning the object while appending it to the list,
 * but directly add the object pointer.
 * We should use this function to reduce the memory consumption,
 * caused by multyple clones
 */
EXPORT_API int maps_item_no_clone(void *origin, void **cloned)
{
	MAPS_LOG_API;
	if(!cloned)
		return MAPS_ERROR_INVALID_PARAMETER;
	*cloned = origin;
	return MAPS_ERROR_NONE;
}
