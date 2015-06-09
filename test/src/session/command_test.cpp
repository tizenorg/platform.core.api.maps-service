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

#include "command_test.h"
#include "command.h"
#include <glib.h>

void utc_command_empty_ptr_p(void)
{
	g_assert(session::command::empty_ptr());
}

/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	plugin::plugin_s* plg;
 public:
	test_env():plg(NULL)
	{
		const plugin::provider_info info =
			plugin::find_by_names("Maps Provider");
		if (info.empty())
			 return; /* For the case when dummy plugin is absent */
		 plg = (plugin::plugin_s*) plugin::binary_extractor().
			init(info);
		 g_assert(plg);
	}
	~test_env()
	{
		if (!plg)
			return;	/* For the case when dummy plugin is absent */
		g_assert(plg);
		if (plg)
			plugin::binary_extractor().shutdown(plg);
	}
};

/*----------------------------------------------------------------------------*/

static int* __utc_int_dup(const int n)
{
	int* clone = g_new0(int, 1);
	*clone = n;
	return clone;
}

void utc_use_hash_table_p(void)
{
	test_env e;
	if (!e.plg)
		return;		/* For the case when dummy plugin is absent */

	GHashTable* t =
		g_hash_table_new_full(g_int_hash, g_int_equal, g_free,
		session::command_handler::destroy);
	g_assert(t);

	const int key = 1;
	g_hash_table_insert(t, __utc_int_dup(key),
		new session::command_handler(e.plg, NULL, 101));

	g_hash_table_remove(t, &key);

	g_hash_table_unref(t);
}

/* bool add(const int user_req_id); */
void utc_pending_request_add_p(void)
{
	test_env e;
	if (!e.plg)
		return;		/* For the case when dummy plugin is absent */

	session::pending_request pr(e.plg);
	bool b = pr.add(1);
	g_assert(b);

	b = pr.contains(1);
	g_assert(b);
}

void utc_pending_request_add_n(void)
{
	test_env e;
	if (!e.plg)
		return;		/* For the case when dummy plugin is absent */

	session::pending_request pr(e.plg);
	bool b = pr.add(-1);
	g_assert(!b);
}

/* void update(int user_req_id, int plg_req_id); */
void utc_pending_request_update_p(void)
{
	test_env e;
	if (!e.plg)
		return;		/* For the case when dummy plugin is absent */

	session::pending_request pr(e.plg);
	bool b = pr.add(1);
	g_assert(b);

	session::command_handler* ch = pr.look_up(1);
	g_assert(!ch);

	ch = new session::command_handler(e.plg, NULL, 1);
	ch->plg_req_id = 101;
	pr.update(1, ch);

	ch = pr.look_up(1);
	g_assert(ch);
	g_assert_cmpint(ch->plg_req_id, ==, 101);
}

void utc_pending_request_update_n(void)
{
	test_env e;
	if (!e.plg)
		return;		/* For the case when dummy plugin is absent */

	session::pending_request pr(e.plg);
	bool b = pr.add(1);
	g_assert(b);

	session::command_handler* ch = pr.look_up(1);
	g_assert(!ch);
}

/* void remove(int user_req_id); */
void utc_pending_request_remove_p(void)
{
	test_env e;
	if (!e.plg)
		return;		/* For the case when dummy plugin is absent */

	session::pending_request pr(e.plg);
	bool b = pr.add(1);
	g_assert(b);

	b = pr.contains(1);
	g_assert(b);

	pr.remove(1);
	g_assert(b);

	b = pr.contains(1);
	g_assert(!b);
}

void utc_pending_request_remove_n(void)
{
	/* empty */
}

/* int look_up(const int user_req_id); */
void utc_pending_request_look_up_p(void)
{
	/* empty */
}

void utc_pending_request_look_up_n(void)
{
	/* empty */
}

/* int extract_plg_id(const int user_req_id); */
void utc_pending_request_extract_p(void)
{
	test_env e;
	if (!e.plg)
		return;		/* For the case when dummy plugin is absent */

	session::pending_request pr(e.plg);
	bool b = pr.add(1);
	g_assert(b);

	session::command_handler* ch =
		new session::command_handler(e.plg, NULL, 1);
	ch->plg_req_id = 101;
	pr.update(1, ch);

	b = pr.contains(1);
	g_assert(b);

	int n = pr.extract_plg_id(1);
	g_assert_cmpint(n, ==, 101);

	b = pr.contains(1);
	g_assert(!b);
}

void utc_pending_request_extract_n(void)
{
	/* empty */
}

/* bool contains(const int user_req_id); */
void utc_pending_request_contains_p(void)
{
	/* empty */
}

void utc_pending_request_contains_n(void)
{
	/* empty */
}

