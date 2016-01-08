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

#ifndef __MAPS_SERVICE_SESSION_COMMAND_H__
#define __MAPS_SERVICE_SESSION_COMMAND_H__

#include "maps_plugin.h"
#include "module.h"

namespace session
{

/*----------------------------------------------------------------------------*/

	class command
	{
	private:
		maps_service_h m;
	protected:
		int my_req_id;
	protected:
		int error;
	public:
		static volatile int command_request_id;
	public:
		static command empty_instance;
	private:
		command()
			: m(NULL), my_req_id(-1), error(0)
		{
		}		/* Please, do not construct an empty object */
	public:
		 command(maps_service_h ms);
		 command(const command &src);
		 virtual ~command();
		 command & operator=(const command &src);
	public:
		 virtual int run();
		void destroy();
	public:
		static command *empty_ptr()
		{
			return &empty_instance;
		}
	protected:
		plugin::interface_s *interface() const;
		maps_plugin_h handle() const;
	public:
		plugin::plugin_s *plugin()const;
	};

/*----------------------------------------------------------------------------*/

	class command_handler
	{
		plugin::plugin_s *plg;
	public:
		void *user_data;
		int user_req_id;
		int plg_req_id;
	public:
		 command_handler(plugin::plugin_s *plugin, void *user_data,
				 int user_req_id);
		 virtual ~command_handler();
	private:
		 command_handler() : plg(NULL), user_data(NULL),
			user_req_id(-1), plg_req_id(-1)
		{
		};
		command_handler(const command_handler &src) : plg(NULL),
			user_data(NULL), user_req_id(-1), plg_req_id(-1)
		{
		};
		command_handler &operator=(const command_handler &src)
		{
			return *this;
		}
	public:
		plugin::plugin_s *plugin()const
		{
			return plg;
		}
	public:
		static void destroy(void *p);
	};

/*----------------------------------------------------------------------------*/

	class pending_request
	{
	private:
		plugin::plugin_s *plg;
	public:
		pending_request(plugin::plugin_s *plugin);
		virtual ~pending_request()
		{
		};
	private:
		pending_request() : plg(NULL) {
		}
		pending_request(const pending_request &src)
		{
		}
		pending_request &operator=(const pending_request &src)
		{
			return *this;
		}
	public:
		bool add(const int user_req_id);
		void update(int user_req_id, command_handler *handler);
		void remove(int user_req_id);
		command_handler *look_up(const int user_req_id);
		int extract_plg_id(const int user_req_id);
		bool contains(const int user_req_id);
	private:
		int *int_dup(const int n);
	};
}

#endif				/* __MAPS_SERVICE_SESSION_COMMAND_H__ */