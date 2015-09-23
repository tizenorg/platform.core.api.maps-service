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
#ifdef _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_
	if (is_async()) {
		static command_queue_async async_queue;
		return &async_queue;
	} else {
		static command_queue_sync sync_queue;
		return &sync_queue;
	}
#else
	static command_queue_sync sync_queue;
	return &sync_queue;
#endif /* _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_ */
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
					/*300 * G_TIME_SPAN_MILLISECOND);*/
					/* Small timeout is better for UI */
					10 * G_TIME_SPAN_MILLISECOND);
	return (c) ? c : command::empty_ptr();
}

void session::command_queue_async::process(plugin::plugin_s* p)
{
	if (not p or not p->request_queue)
		return;

	queue_autoref(p->request_queue);
	pop(p)->run();
}

void session::command_queue_async::clear(plugin::plugin_s* p)
{
	if (not p or not p->request_queue)
		return;

	queue_autoref(p->request_queue);
	while (g_async_queue_length(p->request_queue))
		pop(p)->destroy();
}


/*----------------------------------------------------------------------------*/

session::command_queue* session::command_queue_view::interface()
{
	static command_queue_view view_queue;
	return &view_queue;
}

/*static int __dbg_queue_length = 0;*/

gint session::command_queue_view::iterate(gconstpointer a,
					  gconstpointer b,
					  gpointer user_data)
{
	/*__dbg_queue_length ++;*/

	/* Extracting command data from params */
	command *ca = (command *)a;
	command *cb = (command *)b;
	command *c_new = (command *)user_data;

	/* Attempting to update existing command with freshier data.
	 * It is decided not to store a number of commands of the same type:
	 * one command is enough */
	if(c_new) {
		if(ca->get_type() == c_new->get_type()) {
			ca->merge(c_new);
			//c_new->set_merged();
		} else if(cb->get_type() == c_new->get_type()) {
			cb->merge(c_new);
			//c_new->set_merged();
		}
	}

	/* Sorting the commands in the que with their prioirties:
	 * the lower priority, the far the command from the queue front
	 *
	 * https://developer.gnome.org/glib/stable/
	 *	glib-Asynchronous-Queues.html#g-async-queue-sort */
	const int pa = ca->get_priority();
	const int pb = cb->get_priority();
	return (pa < pb ? +1 : pa == pb ? 0 : -1);
}

int session::command_queue_view::push(command* c)
{
	/*g_print("session::command_queue_view::push "
			"pushed a command: %p\n", c);*/
	if(c == command::empty_ptr())
		return MAPS_ERROR_NONE;

	if (!c || !c->plugin() || !c->plugin()->request_queue)
		return MAPS_ERROR_INVALID_PARAMETER;


	queue_autoref(c->plugin()->request_queue);


#if 0
	/* ---------------------------------------------- */
	/* This is the routine without queue modification */
	const bool _dbg_simple_queue = false;
	if(_dbg_simple_queue) {
		g_async_queue_push (c->plugin()->request_queue, c);
		return MAPS_ERROR_NONE;
	}
	/* ---------------------------------------------- */
#endif



	/* Iterating the queue; sorting it and simultaneously attempting
	 * to update the data of the existing command of the current type.
	 * This approach allows to store only one instance of a command per
	 * given type */
	/*__dbg_queue_length = 0;*/
	g_async_queue_sort (c->plugin()->request_queue,
			    session::command_queue_view::iterate, c);
	/*g_print("Queue Length: %d\n", __dbg_queue_length);*/

	/* If the command was merged, it is not needed any more.
	 * If the command data was not merged to the existing command instance
	 * of the same type, we should:
	 *   a. add this command to the queue and
	 *   b. sort the queue once again without any attempt to update
	 *  existing commands
	 *
	 * https://developer.gnome.org/glib/stable/
	 *	glib-Asynchronous-Queues.html#g-async-queue-push-sorted */
	if(c->merged())
		delete c;
	else
		g_async_queue_push_sorted (c->plugin()->request_queue, c,
					   session::command_queue_view::iterate,
					   NULL);

	return MAPS_ERROR_NONE;
}

session::command* session::command_queue_view::pop(plugin::plugin_s* p)
{
	/*g_print("session::command_queue_view::pop\n");*/
	if (!p || !p->request_queue)
		return command::empty_ptr();

	queue_autoref(p->request_queue);

	/*
	 * Pops data from the queue.
	 * If the queue is empty, blocks for timeout microseconds, or until data
	 * becomes available.
	 * If no data is received before the timeout, NULL is returned.
	 * https://developer.gnome.org/glib/stable/glib-Asynchronous-Queues.html#g-async-queue-timeout-pop
	 */
	command* c =
		(command*) g_async_queue_timeout_pop(p->request_queue,
					/*300 * G_TIME_SPAN_MILLISECOND);*/
					/* Small timeout is better for UI */
					10 * G_TIME_SPAN_MILLISECOND);
	/*if(c)
		g_print("session::command_queue_view::pop "
			"extracted a command: %p\n", c);*/
	return (c) ? c : command::empty_ptr();
}

void session::command_queue_view::process(plugin::plugin_s *p)
{
	if (!p || !p->request_queue)
		return;

	queue_autoref(p->request_queue);
	pop(p)->run();
}

void session::command_queue_view::clear(plugin::plugin_s *p)
{
	if (!p || !p->request_queue)
		return;

	queue_autoref(p->request_queue);
	while (g_async_queue_length(p->request_queue))
		pop(p)->destroy();
}

#endif /* _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_ */
