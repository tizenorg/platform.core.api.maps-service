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

#include "command_queue_test.h"
#include "command_queue.h"
#include "module.h"
#include "commands.h"
#include <glib.h>

extern plugin::plugin_s* __extract_plugin(maps_service_h maps);

class test_env
{
 public:
	maps_service_h m;
	plugin::plugin_s* plugin;
	GAsyncQueue* queue;
 public:
	test_env():m(NULL), plugin(NULL), queue(NULL)
	{
		const plugin::provider_info info =
			plugin::find_by_names("Maps Provider");
		if (info.empty())
			return;	/* For the case when dummy plugin is absent */

		int error = maps_service_create("Maps Provider", &m);
		/*g_assert_cmpint(error, ==, MAPS_ERROR_NONE); */
		if (error != MAPS_ERROR_NONE)
			return;	/* For the case when dummy plugin is absent */

		plugin = __extract_plugin(m);
		g_assert(plugin);
		queue = plugin->request_queue;
		if (session::command_queue::is_async())
			g_assert(queue);
		else
			g_assert(!queue);

	}
	~test_env()
	{
		if (!m)
			return;
		int error = maps_service_destroy(m);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

void utc_command_queue_push_p(void)
{
	test_env e;
	if (!e.m)
		return;		/* For the case when dummy plugin is absent */

	session::command_queue* cq = session::command_queue::interface();

	/* [OK] 2. Push a valid command: lucky path */
	cq->push(new session::command_cancel_request(e.m, -1));
	e.plugin->is_working = false;

	if (session::command_queue::is_async())
		g_assert_cmpint(g_async_queue_length(e.queue), ==, 1);

	session::command* c = cq->pop(e.plugin);
	if (session::command_queue::is_async())
		if (c)
			delete c;
}

void utc_command_queue_push_n(void)
{
	test_env e;
	if (!e.m)
		return;		/* For the case when dummy plugin is absent */
	session::command_queue* cq = session::command_queue::interface();

	/* [FAIL] 1. Push NULL instead of valid command */
	cq->push(NULL);

	/* [FAIL] 2. Push an instance of empty command: testing empty
	*  behaviour */
	cq->push(session::command::empty_ptr());

	/* [FAIL] 3. Push invalid command: plugin is NULL */
	session::command* null_plugin_command = new session::command(NULL);
	cq->push(null_plugin_command);

	/* [FAIL] 4. Push invalid command: queue in the plugin is NULL */
	plugin::plugin_s* broken_plugin = g_slice_new0(plugin::plugin_s);
	session::command* broken_plugin_command =
		new session::command(broken_plugin);
	cq->push(broken_plugin_command);

	if (e.queue)
		g_assert_cmpint(g_async_queue_length(e.queue), ==, 0);

	g_slice_free(plugin::plugin_s, broken_plugin);

	delete null_plugin_command;
	delete broken_plugin_command;
}

