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

#include <unistd.h>
#include "thread.h"
#include "command_queue.h"
#include "command.h"

session::thread::thread()
{

}

session::thread::~thread()
{

}

/* Thread function: pops the item from the queue and performs the command */
void *session::thread::queue_thread(void *data)
{

	sleep(0);		/* Just switch the thread to accomplish previous
				   initialization routines */

	if (not data)
		return NULL;
	plugin::plugin_s *p = (plugin::plugin_s *) data;

	command_queue *cq = session::command_queue::interface();
	while (p->is_working)
		cq->process(p);	/* Continuously perform queue processing */

	return NULL;
}

void session::thread::run(plugin::plugin_s *p)
{

	if (not p)
		return;
	if (p->thread)
		return;		/* Check whether the thread is already
				   started, */
	/* Start if needed */

	p->is_working = true;

	p->thread = g_thread_new("queue_thread", &queue_thread, p);
	if(p->thread)
		g_thread_unref(p->thread);
}

void session::thread::stop(plugin::plugin_s *p)
{
	if (not p)
		return;

	p->is_working = false;

	command_queue *cq = session::command_queue::interface();
	cq->clear(p);
}
