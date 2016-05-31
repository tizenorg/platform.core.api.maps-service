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

#include "command.h"
#include "empty_module.h"
#include "maps_util.h"

extern plugin::plugin_s *__extract_plugin(maps_service_h maps);

volatile int session::command::command_request_id = 1;
session::command session::command::empty_instance;

session::command::command(maps_service_h ms)
	: m(ms), my_req_id(0), error(MAPS_ERROR_NONE), is_merged(false)
{
}

session::command::command(const command &src)
{
	*this = src;
}

session::command::~command()
{
}

session::command &session::command::operator =(const command &src)
{
	if (this != (&src)) {
		m = src.m;
		my_req_id = src.my_req_id;
		error = src.error;
		is_merged = src.is_merged;
	}
	return *this;
}

int session::command::run()
{
	return MAPS_ERROR_NONE;
}

void session::command::destroy()
{
	if (this != empty_ptr())
		delete this;
}

plugin::interface_s *session::command::interface() const
{
	if (!m)
		return plugin::get_empty_interface_ptr();

	plugin::plugin_s *p = __extract_plugin(m);
	if (!p)
		return plugin::get_empty_interface_ptr();

	return &p->interface;
}

maps_plugin_h session::command::handle() const
{
	return (maps_plugin_h) plugin();
}

plugin::plugin_s *session::command::plugin() const
{
	return __extract_plugin(m);
}

session::command_type_e session::command::get_type() const
{
	/* Default command type */
	return MAPS_DATA_COMMAND;
}

int session::command::get_priority() const
{
	/* Default command priority */
	return 1;
}

void session::command::merge(const command *c)
{
	/* empty */
}

bool session::command::merged() const
{
	return is_merged;
}


void session::command::set_merged()
{
	is_merged = true;
}
/*----------------------------------------------------------------------------*/

session::command_handler::command_handler(plugin::plugin_s *p, void *ud,
					  int urid)
 : plg(p), user_data(ud), user_req_id(urid), plg_req_id(-1)
{
}

session::command_handler::~command_handler()
{
}

void session::command_handler::destroy(void *p)
{
	if (!p)
		return;
	command_handler *c = (command_handler *) p;
	delete c;
}

/*----------------------------------------------------------------------------*/

session::pending_request::pending_request(plugin::plugin_s *p) : plg(p)
{
}

bool session::pending_request::add(const int user_req_id)
{
	if (!plg || !plg->pending_request_maps || (user_req_id < 0))
		return false;
	plugin::scope_mutex(&plg->pending_request_mutex);

	MAPS_LOGD("session::pending_request::add: %d", user_req_id);

	if (contains(user_req_id))
		return false;	/* Attempt to add another instance of the id */
	g_hash_table_insert(plg->pending_request_maps, int_dup(user_req_id),
		NULL);
	return true;
}

void session::pending_request::update(int user_req_id,
				      command_handler *handler)
{
	if (!plg || !plg->pending_request_maps || (user_req_id < 0) || !handler)
		return;
	plugin::scope_mutex(&plg->pending_request_mutex);

	MAPS_LOGD("session::pending_request::update: %d, %d", user_req_id,
		handler->plg_req_id);

	if (!contains(user_req_id)) {	/* Attempt to update not existing id */
		MAPS_LOGD("\t not updated session::pending_request: %d, %d",
			user_req_id, handler->plg_req_id);
		delete handler;	/* This handler must be deleted */
		return;
	}

	g_hash_table_insert(plg->pending_request_maps, int_dup(user_req_id),
		handler);
}

void session::pending_request::remove(int user_req_id)
{
	if (!plg || !plg->pending_request_maps || (user_req_id < 0))
		return;
	plugin::scope_mutex(&plg->pending_request_mutex);

	MAPS_LOGD("session::pending_request::remove: %d", user_req_id);

	if (!contains(user_req_id))
		return;		/* Attempt to remove not existing id */
	g_hash_table_remove(plg->pending_request_maps, &user_req_id);
}

session::command_handler *session::pending_request::look_up(const int
							    user_req_id)
{
	if (!plg || !plg->pending_request_maps || (user_req_id < 0))
		return NULL;
	plugin::scope_mutex(&plg->pending_request_mutex);

	MAPS_LOGD("session::pending_request::look_up: %d", user_req_id);

	if (!contains(user_req_id))
		return NULL;	/* Attempt to query not existing id */
	return (command_handler*) g_hash_table_lookup(plg->
		pending_request_maps, &user_req_id);
}

int session::pending_request::extract_plg_id(const int user_req_id)
{
	if (!plg || !plg->pending_request_maps || (user_req_id < 0))
		return -1;
	plugin::scope_mutex(&plg->pending_request_mutex);

	command_handler* ch = look_up(user_req_id);
	if (!ch)
		return -1;
	const int plg_req_id = ch->plg_req_id;
	remove(user_req_id);
	return plg_req_id;
}

bool session::pending_request::contains(const int user_req_id)
{
	if (!plg || !plg->pending_request_maps || (user_req_id < 0))
		return false;
	plugin::scope_mutex(&plg->pending_request_mutex);

	return g_hash_table_contains(plg->pending_request_maps, &user_req_id);
}

int *session::pending_request::int_dup(const int n)
{
	int *clone = g_new0(int, 1);
	*clone = n;
	return clone;
}