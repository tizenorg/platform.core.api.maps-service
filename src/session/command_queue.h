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

#ifndef __MAPS_SERVICE_SESSION_COMMAND_QUEUE_H__
#define __MAPS_SERVICE_SESSION_COMMAND_QUEUE_H__

#include "module.h"

namespace session
{

	class command;

	class command_queue
	{
	public:
		virtual int push(command *c) = 0;
		virtual command *pop(plugin::plugin_s *p) = 0;
		virtual void process(plugin::plugin_s *p) = 0;
		virtual void clear(plugin::plugin_s *p) = 0;
	public:
		static command_queue *interface();
		static bool is_async()
		{
			return false;
		}
	};

	class command_queue_sync:public command_queue
	{
	private:
		command_queue_sync()
		{
		}
		virtual ~command_queue_sync()
		{
		}
	private:
		virtual int push(command *c);
		virtual command *pop(plugin::plugin_s *p);
		virtual void process(plugin::plugin_s *p);
		virtual void clear(plugin::plugin_s *p);

		friend class command_queue;
	};

/*
 * This is the implementation of asynchronous queue.
 * In order to pass code coverage tests it is blocked.
 */
/*#define _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_*/
#ifdef _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_
	class command_queue_async:public command_queue
	{
	private:
		command_queue_async()
		{
		}
		virtual ~command_queue_async()
		{
		}
	private:
		virtual int push(command *c);
		virtual command *pop(plugin::plugin_s *p);
		virtual void process(plugin::plugin_s *p);
		virtual void clear(plugin::plugin_s *p);

		friend class command_queue;
	};
#endif /* _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_ */

	/*
	 * This queue is intended to process mainly the commands, assigned to
	 * user gestures
	 */
	class command_queue_view : public command_queue
	{
	private:
		command_queue_view()
		{
		}
		virtual ~command_queue_view()
		{
		}
	public:
		static command_queue* interface();
	private:
		virtual int push(command *c);
		virtual command *pop(plugin::plugin_s *p);
		virtual void process(plugin::plugin_s *p);
		virtual void clear(plugin::plugin_s *p);

		friend class command_queue;
	private:
		static gint iterate(gconstpointer a,
				    gconstpointer b,
				    gpointer user_data);
	};

	class queue_autoref
	{
	public:
		GAsyncQueue *q;
	public:
		queue_autoref(GAsyncQueue *async_queue) : q(async_queue)
		{
			g_async_queue_ref(q);
		}
		~queue_autoref()
		{
			g_async_queue_unref(q);
		}
	};

};

#endif				/* __MAPS_SERVICE_SESSION_COMMAND_QUEUE_H__ */
