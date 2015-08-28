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

#include "command_queue.h"
#include "command.h"
#include "thread.h"

session::command_queue *session::command_queue::interface()
{
	if (is_async()) {
#ifdef _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_
		static command_queue_async async_queue;
		return &async_queue;
#else
		static command_queue_sync sync_queue;
		return &sync_queue;
#endif /* _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_ */
	}
	else {
		static command_queue_sync sync_queue;
		return &sync_queue;
	}
}

/*----------------------------------------------------------------------------*/

int session::command_queue_sync::push(command *c)
{
	return (c and c->plugin())? c->run() : MAPS_ERROR_INVALID_PARAMETER;
}

session::command* session::command_queue_sync::pop(plugin::plugin_s *p)
{
	return command::empty_ptr();
}

void session::command_queue_sync::process(plugin::plugin_s *p)
{
	/* empty */
}

void session::command_queue_sync::clear(plugin::plugin_s *p)
{
	/* empty */
}

/*----------------------------------------------------------------------------*/

/*
 * This is the implementation of asynchronous queue.
 * In order to pass code coverage tests it is blocked.
 */
#ifdef _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_
int session::command_queue_async::push(command *c)
{
	if (!c || !c->plugin() || !c->plugin()->request_queue)
		return MAPS_ERROR_INVALID_PARAMETER;

	queue_autoref(c->plugin()->request_queue);
	g_async_queue_push(c->plugin()->request_queue, c);

	/* Lazy initialization: run plugin thread if it is not started yet */
	thread().run(c->plugin());

	return MAPS_ERROR_NONE;
}

session::command *session::command_queue_async::pop(plugin::plugin_s *p)
{
	if (!p || !p->request_queue)
		return command::empty_ptr();

	queue_autoref(p->request_queue);

	/* Pops data from the queue . */
	/* If the queue is empty, blocks for timeout microseconds, or until data
	*  becomes available. */
	/* If no data is received before the timeout, NULL is returned. */
	/* https://developer.gnome.org/glib/stable/glib-Asynchronous-Queues.html#g-async-queue-timeout-pop */
	command* c =
		(command *) g_async_queue_timeout_pop(p->request_queue,
		300 * G_TIME_SPAN_MILLISECOND);
	return (c) ? c : command::empty_ptr();
}

void session::command_queue_async::process(plugin::plugin_s *p)
{
	if (!p || !p->request_queue)
		return;

	queue_autoref(p->request_queue);
	pop(p)->run();
}

void session::command_queue_async::clear(plugin::plugin_s *p)
{
	if (!p || !p->request_queue)
		return;

	queue_autoref(p->request_queue);
	while (g_async_queue_length(p->request_queue))
		pop(p)->destroy();
}
#endif /* _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_ */
