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

#include "commands.h"
#include "maps_util.h"
#include "map_view_plugin.h"
#include "maps_place_private.h"
#include "maps_route_private.h"
#include "empty_module.h"

extern int _map_view_move_center(map_view_h view, const int delta_x, const int delta_y);

extern int _map_view_set_zoom_rotate(map_view_h view,
				     const bool zoom_changed, const double zoom_factor,
				     const bool rotation_changed, const double rotation_angle);

extern bool maps_address_is_valid(const maps_address_h address);
extern bool maps_area_is_valid(const maps_area_h area);
extern bool maps_coordinates_is_valid(const maps_coordinates_h coordinates);

static int __put_to_hashtable(session::command_handler *ch,
			      maps_service_data_e feature,
			      maps_int_hashtable_h t)
{
	if (!ch || !t)
		return MAPS_ERROR_INVALID_PARAMETER;
	bool supported = false;
	ch->plugin()->interface.maps_plugin_is_data_supported(feature, &supported);
	if(supported)
		return  maps_int_hashtable_set(t, feature, feature);
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_geocode_f)(maps_service_h maps, const char *address,
* maps_service_geocode_cb callback, void *user_data, int *request_id); */
session::command_geocode::command_geocode(maps_service_h ms, const string a,
					  const maps_item_hashtable_h pref,
					  maps_service_geocode_cb cb,
					  void *ud, int *request_id)
 : command(ms)
 , address(a)
 , preference(pref)
 , callback(cb)
 , user_data(ud)
 , error(0)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;
}

session::command_geocode::~command_geocode()
{
}

int session::command_geocode::run()
{
	if (error != MAPS_ERROR_NONE) return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_geocode_f func = interface()->maps_plugin_geocode;
	command_geocode_handler *handler = NULL;
	if (func) {
		/*  need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_geocode_handler(plugin(),
					     callback,
					     user_data,
					     my_req_id);

		if (handler) {
			/* Run the plugin interface function */
			error = func(address.c_str(), preference,
					 command_geocode_handler::foreach_geocode_cb, handler,
					 &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_geocode::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().maps_plugin_geocode;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

session::command_geocode_handler::command_geocode_handler(plugin::plugin_s *p,
						maps_service_geocode_cb cb,
						void *ud, int urid)
 : command_handler(p, ud, urid)
 , callback(cb)
{
}

bool session::command_geocode_handler::foreach_geocode_cb(maps_error_e error,
							  int request_id,
							  int index,
							  int total_count,
							  maps_coordinates_h
							  coordinates,
							  void *user_data)
{

	command_geocode_handler *handler = (command_geocode_handler *) user_data;

	if (request_id != handler->plg_req_id) {
		MAPS_LOGE("\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n",
			request_id, handler->plg_req_id);
	}

	/* Make a user's copy of result data */
	maps_coordinates_h cloned_result = NULL;
	if (error == MAPS_ERROR_NONE)
		error = (maps_error_e) maps_coordinates_clone(coordinates, &cloned_result);
	maps_coordinates_destroy(coordinates);

	/* Send data to user */
	const bool b = handler->callback(error, handler->user_req_id, index,
		total_count, cloned_result, handler->user_data);
	if (!b || (index >= (total_count - 1))) {
		pending_request pr(handler->plugin());
		pr.remove(handler->user_req_id);
	}

	return b;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_geocode_inside_area_f)(maps_service_h maps, const
* char *address, maps_area_h bounds, maps_service_geocode_cb callback,
* void *user_data, int *request_id); */
session::command_geocode_inside_bounds::command_geocode_inside_bounds(
					maps_service_h ms,
					const char *a,
					const maps_area_h b,
					const maps_item_hashtable_h pref,
					maps_service_geocode_cb cb,
					void *ud, int *request_id)
 : command(ms)
 , address(a)
 , bounds(NULL)
 , preference(pref)
 , callback(cb)
 , user_data(ud)
 , error(0)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_area_is_valid(b)) {
		bounds = b;
	}
	else {
		error = MAPS_ERROR_INVALID_PARAMETER;
		*request_id = -1;
	}

}

session::command_geocode_inside_bounds::~command_geocode_inside_bounds()
{
}

int session::command_geocode_inside_bounds::run()
{
	if (error != MAPS_ERROR_NONE) return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_geocode_inside_area_f func =
		interface()->maps_plugin_geocode_inside_area;
	command_geocode_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_geocode_handler(plugin(),
						      callback,
						      user_data,
						      my_req_id);

		if (handler) {
			/* Run the plugin interface function */
			error = func(address.c_str(), bounds, preference,
				     command_geocode_handler::foreach_geocode_cb,
				     handler, &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_geocode_inside_bounds::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().
			maps_plugin_geocode_inside_area;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_geocode_by_structured_address_f)(
* maps_service_h maps, maps_address_h address, maps_preference_h preference,
* maps_service_geocode_cb callback, void *user_data, int *request_id); */
session::command_geocode_by_structured_address::
	command_geocode_by_structured_address(maps_service_h ms,
					      const maps_address_h a,
					      const maps_item_hashtable_h pref,
					      maps_service_geocode_cb cb,
					      void *ud, int *request_id)
 : command(ms)
 , address(NULL)
 , preference(pref)
 , callback(cb)
 , user_data(ud)
 , error(0)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_address_is_valid(a)) {
		address = a;
	}
	else {
		error = MAPS_ERROR_INVALID_PARAMETER;
		*request_id = -1;
		MAPS_LOGD("Invalid parameter");
	}

}

session::command_geocode_by_structured_address::
	~command_geocode_by_structured_address()
{
}

int session::command_geocode_by_structured_address::run()
{
	if (error != MAPS_ERROR_NONE) return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_geocode_by_structured_address_f func =
		interface()->maps_plugin_geocode_by_structured_address;
	command_geocode_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_geocode_handler(plugin(),
						      callback,
						      user_data,
						      my_req_id);

		if (handler) {
			/* Run the plugin interface function */
			error = func(address,
						 preference, command_geocode_handler::foreach_geocode_cb,
						 handler, &handler->plg_req_id);

			pr.update(my_req_id, handler);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().
			maps_plugin_geocode_by_structured_address;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_reverse_geocode_f)(maps_service_h maps,
* double latitude, double longitude, maps_service_reverse_geocode_cb callback,
* void *user_data, int *request_id); */
session::command_reverse_geocode::command_reverse_geocode(maps_service_h ms,
					double lat, double lon,
					const maps_item_hashtable_h pref,
					maps_service_reverse_geocode_cb cb,
					void *ud, int *request_id)
 : command(ms)
 , latitude(lat)
 , longitude(lon)
 , preference(pref)
 , callback(cb)
 , user_data(ud)
 , error(0)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

}

session::command_reverse_geocode::~command_reverse_geocode()
{
}

int session::command_reverse_geocode::run()
{
	if (error != MAPS_ERROR_NONE) return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_reverse_geocode_f func = interface()->maps_plugin_reverse_geocode;
	command_reverse_geocode_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_reverse_geocode_handler(plugin(),
							      callback,
							      user_data,
							      my_req_id);

		if (handler) {
			/* Run the plugin interface function */
			error = func(latitude, longitude, preference,
						command_reverse_geocode_handler::foreach_reverse_geocode_cb,
						handler, &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_reverse_geocode::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().
			maps_plugin_reverse_geocode;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

session::command_reverse_geocode_handler::command_reverse_geocode_handler(
					plugin::plugin_s *p,
					maps_service_reverse_geocode_cb cb,
					void *ud, int urid)
 : command_handler(p, ud, urid)
 , callback(cb)
{
}

void session::command_reverse_geocode_handler::foreach_reverse_geocode_cb(
						maps_error_e error,
						int request_id, int index,
						int total,
						maps_address_h address,
						void *user_data)
{

	command_reverse_geocode_handler *handler =
		(command_reverse_geocode_handler *) user_data;

	if (request_id != handler->plg_req_id) {
		MAPS_LOGE(
"\n\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n\n",
			request_id, handler->plg_req_id);
	}

	/* Make a user's copy of result data */
	maps_address_h cloned_result = NULL;
	if (error == MAPS_ERROR_NONE)
		error = (maps_error_e) maps_address_clone(address,
			&cloned_result);
	maps_address_destroy(address);

	/* Send data to user */
	handler->callback(error, handler->user_req_id, index, total,
		cloned_result, handler->user_data);
	/*if(index>=(total-1)) */

	pending_request pr(handler->plugin());
	pr.remove(handler->user_req_id);
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_multi_reverse_geocode_f) (maps_service_h maps,
* maps_coordinates_list_h maps_list, maps_preference_h preference,
* maps_service_multi_reverse_geocode_cb callback, void * user_data, int *request_id) */
session::command_multi_reverse_geocode::command_multi_reverse_geocode(
			maps_service_h ms, const maps_coordinates_list_h list, const maps_item_hashtable_h pref,
			maps_service_multi_reverse_geocode_cb cb, void *ud, int *request_id)
 : command(ms)
 , maps_list(NULL)
 , preference(NULL)
 , callback(cb)
 , user_data(ud)
 , error(0)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	int list_size = 0;
	maps_coordinates_list_get_length(list, &list_size);
	if (list_size < 2 || list_size > 100) {
		error = MAPS_ERROR_INVALID_PARAMETER;
		*request_id = -1;
	}
	maps_list = list;
}

session::command_multi_reverse_geocode::~command_multi_reverse_geocode()
{
}

int session::command_multi_reverse_geocode::run()
{
	if (error != MAPS_ERROR_NONE) return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_multi_reverse_geocode_f func =
		interface()->maps_plugin_multi_reverse_geocode;

	command_multi_reverse_geocode_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_multi_reverse_geocode_handler(plugin(), callback, user_data, my_req_id);

		if (handler) {
			/* Run the plugin interface function */
			error = func(maps_list, preference,
						command_multi_reverse_geocode_handler::foreach_multi_reverse_geocode_cb,
						handler, &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_multi_reverse_geocode::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().
			maps_plugin_multi_reverse_geocode;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

session::command_multi_reverse_geocode_handler::command_multi_reverse_geocode_handler(
			plugin::plugin_s *p, maps_service_multi_reverse_geocode_cb cb, void *ud, int urid)
 : command_handler(p, ud, urid)
 , callback(cb)
{
}

bool session::command_multi_reverse_geocode_handler::foreach_multi_reverse_geocode_cb(
			maps_error_e error, int request_id, int total, maps_coordinates_list_h address_list, void *user_data)
{
	command_multi_reverse_geocode_handler *handler =
		(command_multi_reverse_geocode_handler *) user_data;

	if (request_id != handler->plg_req_id) {
		MAPS_LOGE("\n\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n\n",
			request_id, handler->plg_req_id);
	}

	/* Make a user's copy of result data */
	maps_coordinates_list_h cloned_result = NULL;
	cloned_result = address_list;

	/* Send data to user */
	const bool b = handler->callback(error, handler->user_req_id, total, cloned_result, handler->user_data);
	/*if(index>=(total-1)) */

	pending_request pr(handler->plugin());
	pr.remove(handler->user_req_id);

	return b;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_place_f)(maps_service_h maps,
* maps_coordinates_h position, int distance, maps_item_hashtable_h preference,
* maps_place_filter_h filter, maps_service_search_place_cb callback,
* void* user_data, int* request_id); */
session::command_search_place::command_search_place(maps_service_h ms,
					const maps_coordinates_h pos,
					int dst,
					const maps_item_hashtable_h pref,
					const maps_place_filter_h flt,
					maps_service_search_place_cb cb,
					void *ud, int *request_id)
 : command(ms)
 , position(NULL)
 , distance(dst)
 , preference(pref)
 , filter(flt)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_coordinates_is_valid(pos)) {
		position = pos;
	}
	else {
		error = MAPS_ERROR_INVALID_PARAMETER;
		*request_id = -1;
		MAPS_LOGD("Invalid parameter");
	}
}

session::command_search_place::~command_search_place()
{
}

int session::command_search_place::run()
{
	if (error != MAPS_ERROR_NONE) return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_search_place_f func = interface()->maps_plugin_search_place;
	command_search_place_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_search_place_handler(plugin(),
							   callback,
							   user_data,
							   my_req_id);
		if (handler) {
			/* Run the plugin interface function */
			error = func(position, distance, filter, preference,
					 command_search_place_handler::foreach_place_cb, handler,
					 &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_search_place::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().maps_plugin_search_place;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

session::command_search_place_handler::command_search_place_handler(
						plugin::plugin_s* p,
						maps_service_search_place_cb cb,
						void *ud, int urid)
 : command_handler(p, ud, urid)
 , callback(cb)
{
}

void session::command_search_place_handler::set_supported_data(maps_place_h place)
{
	if (!place || !plugin()) return;

	maps_int_hashtable_h data_supported = NULL;
	if (maps_int_hashtable_create(&data_supported) != MAPS_ERROR_NONE) return;

	__put_to_hashtable(this, MAPS_PLACE_ADDRESS, data_supported);
	__put_to_hashtable(this, MAPS_PLACE_RATING, data_supported);
	__put_to_hashtable(this, MAPS_PLACE_CATEGORIES, data_supported);
	__put_to_hashtable(this, MAPS_PLACE_ATTRIBUTES, data_supported);
	__put_to_hashtable(this, MAPS_PLACE_CONTACTS, data_supported);
	__put_to_hashtable(this, MAPS_PLACE_EDITORIALS, data_supported);
	__put_to_hashtable(this, MAPS_PLACE_REVIEWS, data_supported);
	__put_to_hashtable(this, MAPS_PLACE_IMAGE, data_supported);
	__put_to_hashtable(this, MAPS_PLACE_SUPPLIER, data_supported);
	__put_to_hashtable(this, MAPS_PLACE_RELATED, data_supported);

	_maps_place_set_supported_data(place, data_supported);
	maps_int_hashtable_destroy(data_supported);
}

bool session::command_search_place_handler::foreach_place_cb(maps_error_e error,
							     int request_id,
							     int index,
							     int length,
							     maps_place_h place,
							     void *user_data)
{

	command_search_place_handler *handler =
		(command_search_place_handler *) user_data;

	if (request_id != handler->plg_req_id) {
		MAPS_LOGE("\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n",
			request_id, handler->plg_req_id);
	}

	/* Make a user's copy of result data */
	maps_place_h cloned_result = NULL;
	if (error == MAPS_ERROR_NONE)
		error = (maps_error_e) maps_place_clone(place, &cloned_result);
	maps_place_destroy(place);

	/* Check which data features are supported */
	if (cloned_result)
		handler->set_supported_data(cloned_result);

	/* Send data to user */
	const bool b = handler->callback(error, handler->user_req_id, index, length,
		cloned_result, handler->user_data);
	if (!b || (index >= (length - 1))) {
		pending_request pr(handler->plugin());
		pr.remove(handler->user_req_id);
	}

	return b;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_place_by_area_f)(maps_service_h maps,
* maps_area_h boundary , maps_item_hashtable_h preference,
* maps_place_filter_h filter, maps_service_search_place_cb callback,
* void *user_data, int *request_id); */
session::command_search_by_area_place::command_search_by_area_place(
					maps_service_h ms, const maps_area_h b,
					const maps_item_hashtable_h pref,
					const maps_place_filter_h flt,
					maps_service_search_place_cb cb,
					void *ud, int *request_id)
 : command(ms)
 , boundary(NULL)
 , preference(pref)
 , filter(flt)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_area_is_valid(b)) {
		boundary = b;
	}
	else {
		error = MAPS_ERROR_INVALID_PARAMETER;
		*request_id = -1;
	}
}

session::command_search_by_area_place::~command_search_by_area_place()
{
}

int session::command_search_by_area_place::run()
{
	if (error != MAPS_ERROR_NONE) return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_search_place_by_area_f func =
		interface()->maps_plugin_search_place_by_area;
	command_search_place_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_search_place_handler(plugin(),
							   callback,
							   user_data,
							   my_req_id);
		if (handler) {

			/* Run the plugin interface function */
			error = func(boundary, filter,
				preference, command_search_place_handler::foreach_place_cb,
				handler, &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_search_by_area_place::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().
			maps_plugin_search_place_by_area;
		*/

		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_place_by_address_f)(maps_service_h maps,
* const char *address, maps_area_h boundary, maps_item_hashtable_h preference,
* maps_place_filter_h filter, maps_service_search_place_cb callback,
* void *user_data, int *request_id); */
session::command_search_by_address_place::command_search_by_address_place(
					maps_service_h ms, const char *a,
					const maps_area_h b,
					const maps_item_hashtable_h pref,
					const maps_place_filter_h flt,
					maps_service_search_place_cb cb,
					void *ud, int *request_id)
 : command(ms)
 , address(a)
 , boundary(NULL)
 , preference(pref)
 , filter(flt)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_area_is_valid(b)) {
		boundary = b;
	} else {
		error = MAPS_ERROR_INVALID_PARAMETER;
		*request_id = -1;
		MAPS_LOGD("Invalid parameter");
	}
}

session::command_search_by_address_place::~command_search_by_address_place()
{
}

int session::command_search_by_address_place::run()
{
	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_search_place_by_address_f func =
		interface()->maps_plugin_search_place_by_address;
	command_search_place_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_search_place_handler(plugin(),
							   callback,
							   user_data,
							   my_req_id);
		if (handler) {
			/* Run the plugin interface function */
			error = func(address.c_str(), boundary, filter, preference,
					 command_search_place_handler::foreach_place_cb, handler,
					 &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_search_by_address_place::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().
			maps_plugin_search_place_by_address;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_place_list_f)(maps_service_h maps,
* maps_item_hashtable_h preference, maps_place_filter_h filter,
* maps_service_search_place_list_cb callback, void* user_data, int* request_id); */
session::command_search_place_list::command_search_place_list(maps_service_h ms,
					const maps_area_h b,
					const maps_item_hashtable_h pref,
					const maps_place_filter_h flt,
					maps_service_search_place_list_cb cb,
					void *ud, int *request_id)
 : command(ms)
 , boundary(NULL)
 , preference(pref)
 , filter(flt)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_area_is_valid(b)) {
		boundary = b;
	} else {
		error = MAPS_ERROR_INVALID_PARAMETER;
		*request_id = -1;
		MAPS_LOGD("Invalid parameter");
	}
}

session::command_search_place_list::~command_search_place_list()
{
}

int session::command_search_place_list::run()
{
	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_search_place_list_f func = interface()->maps_plugin_search_place_list;
	command_search_place_list_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_search_place_list_handler(plugin(),
							   callback,
							   user_data,
							   my_req_id);
		if (handler) {
			/* Run the plugin interface function */
			error = func(boundary, filter, preference,
					 command_search_place_list_handler::foreach_place_list_cb, handler,
					 &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_search_place_list::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().maps_plugin_search_place_list;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

session::command_search_place_list_handler::command_search_place_list_handler(
						plugin::plugin_s* p,
						maps_service_search_place_list_cb cb,
						void *ud, int urid)
 : command_handler(p, ud, urid)
 , callback(cb)
{
}

void session::command_search_place_list_handler::foreach_place_list_cb(maps_error_e error,
							     int request_id,
								 int total,
							     maps_place_list_h place_list,
							     void *user_data)
{
	command_search_place_list_handler *handler =
		(command_search_place_list_handler *) user_data;

	if (request_id != handler->plg_req_id) {
		MAPS_LOGE("\n\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n\n",
			request_id, handler->plg_req_id);
	}

	/* Send data to user */
	handler->callback(error, handler->user_req_id, total, place_list, handler->user_data);

	pending_request pr(handler->plugin());
	pr.remove(handler->user_req_id);
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_get_place_details_f) */
session::command_get_place_details::command_get_place_details(
					maps_service_h ms, const char *a,
					maps_service_get_place_details_cb cb,
					void *ud, int *request_id)
 : command(ms)
 , url(a)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;
}

session::command_get_place_details::~command_get_place_details()
{
}

int session::command_get_place_details::run()
{
	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_get_place_details_f func =
		interface()->maps_plugin_get_place_details;
	command_get_place_details_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_get_place_details_handler(plugin(),
							   callback, user_data, my_req_id);
		if (handler) {
			/* Run the plugin interface function */
			error = func(url.c_str(),
					 command_get_place_details_handler::foreach_place_details_cb,
					 handler, &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_get_place_details::run: %d",
				my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().
			maps_plugin_get_place_details;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

session::command_get_place_details_handler::command_get_place_details_handler(
						plugin::plugin_s* p,
						maps_service_get_place_details_cb cb,
						void *ud, int urid)
 : command_handler(p, ud, urid)
 , callback(cb)
{
}

void session::command_get_place_details_handler::foreach_place_details_cb(maps_error_e error,
							     int request_id, maps_place_h place, void *user_data)
{

	command_get_place_details_handler *handler =
		(command_get_place_details_handler *) user_data;

	if (request_id != handler->plg_req_id) {
		MAPS_LOGE("\n\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n\n",
			request_id, handler->plg_req_id);
	}

	/* Make a user's copy of result data */
	maps_place_h cloned_result = NULL;
	if (error == MAPS_ERROR_NONE)
		error = (maps_error_e) maps_place_clone(place, &cloned_result);
	maps_place_destroy(place);

	/* Send data to user */
	handler->callback(error, handler->user_req_id, cloned_result, handler->user_data);

	pending_request pr(handler->plugin());
	pr.remove(handler->user_req_id);
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_route_f)(maps_service_h maps,
* maps_item_hashtable_h preference, maps_coordinates_h origin,
* maps_coordinates_h destination, maps_service_search_route_cb callback,
* void *user_data, int *request_id); */
session::command_search_route::command_search_route(maps_service_h ms,
					const maps_item_hashtable_h pref,
					const maps_coordinates_h orig,
					const maps_coordinates_h dest,
					maps_service_search_route_cb cb,
					void *ud, int *request_id)
 : command(ms)
 , preference(NULL)
 , origin(NULL)
 , destination(NULL)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_coordinates_is_valid(orig)) {
		origin = orig;
	}
	else {
		error = MAPS_ERROR_INVALID_PARAMETER;
	}

	if (maps_coordinates_is_valid(dest)) {
		destination = dest;
	}
	else {
		error = MAPS_ERROR_INVALID_PARAMETER;
	}

	if (error) {
		*request_id = -1;
		MAPS_LOGD("Invalid parameter");
	}
}

session::command_search_route::~command_search_route()
{
}

int session::command_search_route::run()
{
	if (error != MAPS_ERROR_NONE) return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_search_route_f func = interface()->maps_plugin_search_route;
	command_search_route_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_search_route_handler(plugin(),
							   callback,
							   user_data,
							   my_req_id);

		if (handler) {
			/* Run the plugin interface function */
			error = func(origin, destination, preference,
		     command_search_route_handler::foreach_route_cb,
		     handler, &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_search_route::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().maps_plugin_search_route;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_route_waypoints_f)(maps_service_h maps,
* maps_item_hashtable_h preference, maps_coordinates_h *waypoint_list,
* int waypoint_num, maps_service_search_route_cb callback, void *user_data,
* int *request_id); */
session::command_search_route_waypoints::command_search_route_waypoints(
				maps_service_h ms,
				const maps_item_hashtable_h pref,
				const maps_coordinates_h *list,
				int num, maps_service_search_route_cb cb,
				void *ud, int *request_id)
 : command(ms)
 , preference(pref)
 , waypoint_list(NULL)
 , waypoint_num(num)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	waypoint_list = new maps_coordinates_h[num];
	for (int index = 0; index < num; index++) {
		if (list[index] != NULL) {
			maps_coordinates_clone(list[index],
				&waypoint_list[index]);
		}
	}
}

session::command_search_route_waypoints::~command_search_route_waypoints()
{
	for (int index = 0; index < waypoint_num; index++) {
		if (waypoint_list[index] != NULL) {
			maps_coordinates_destroy(waypoint_list[index]);
		}
	}
	delete [] waypoint_list;
}

int session::command_search_route_waypoints::run()
{
	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());

	/* Get the plugin interface function */
	maps_plugin_search_route_waypoints_f func =
		interface()->maps_plugin_search_route_waypoints;

	command_search_route_handler *handler = NULL;
	if (func) {
		/* No need to create the handler when the function is NULL */
		pr.add(my_req_id);
		handler = new command_search_route_handler(plugin(),
						 callback,
						 user_data,
						 my_req_id);
		if (handler) {
			/* Run the plugin interface function */
			error = func(waypoint_list, waypoint_num, preference,
					 command_search_route_handler::foreach_route_cb, handler,
					 &handler->plg_req_id);

			pr.update(my_req_id, handler);

			MAPS_LOGD("session::command_search_place::run: %d", my_req_id);
		}
		else {
			error = MAPS_ERROR_OUT_OF_MEMORY;
		}
	}
	else {
		/* Plugin Function is NULL: use default empty function */
		/*
		func = plugin::get_empty_interface().
			maps_plugin_search_route_waypoints;
		*/
		MAPS_LOGE("MAPS_ERROR_NOT_SUPPORTED: Can't get any plugin");
		error = MAPS_ERROR_NOT_SUPPORTED;
	}

	const int ret = error;
	destroy();
	return ret;
}

session::command_search_route_handler::command_search_route_handler(
						plugin::plugin_s *p,
						maps_service_search_route_cb cb,
						void *ud, int urid)
 : command_handler(p, ud, urid)
 , callback(cb)
{
}

void session::command_search_route_handler::set_supported_data(maps_route_h route)
{
	if (!route || !plugin())
		return;

	maps_int_hashtable_h data_supported = NULL;
	if (maps_int_hashtable_create(&data_supported) != MAPS_ERROR_NONE)
		return;

	__put_to_hashtable(this, MAPS_ROUTE_PATH, data_supported);
	__put_to_hashtable(this, MAPS_ROUTE_SEGMENTS_PATH, data_supported);
	__put_to_hashtable(this, MAPS_ROUTE_SEGMENTS_MANEUVERS, data_supported);

	_maps_route_set_supported_data(route, data_supported);
	maps_int_hashtable_destroy(data_supported);
}

bool session::command_search_route_handler::foreach_route_cb(maps_error_e error,
							     int request_id,
							     int index,
							     int length,
							     maps_route_h route,
							     void *user_data)
{
	command_search_route_handler *handler =
		(command_search_route_handler *) user_data;

	if (request_id != handler->plg_req_id) {
		MAPS_LOGE("\n\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n\n",
			request_id, handler->plg_req_id);
	}

	/* Make a user's copy of result data */
	maps_route_h cloned_result = NULL;
	if (error == MAPS_ERROR_NONE)
		error = (maps_error_e) maps_route_clone(route, &cloned_result);
	maps_route_destroy(route);

	/* Check which data features are supported */
	if (cloned_result)
		handler->set_supported_data(cloned_result);

	/* Send data to user */
	const bool b =
		handler->callback(error, handler->user_req_id, index, length,
							cloned_result, handler->user_data);
	if (!b || (index >= (length - 1))) {
		pending_request pr(handler->plugin());
		pr.remove(handler->user_req_id);
	}

	return b;
}

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_cancel_request_f)(maps_service_h maps,
* int request_id); */
int session::command_cancel_request::run()
{
	pending_request pr(plugin());
	MAPS_LOGD("session::command_cancel_request::run: %d, %d", request_id, pr.look_up(request_id));
	const int error = (pr.contains(request_id)) ?
		interface()->maps_plugin_cancel_request(pr.extract_plg_id(request_id)) : MAPS_ERROR_NOT_FOUND;
	destroy();
	return error;
}


/*----------------------------------------------------------------------------*/
/*
 *		Mapping API commands
 */
/*----------------------------------------------------------------------------*/

session::command_view_set_center::command_view_set_center(maps_service_h ms,
							   map_view_h view,
						const maps_coordinates_h coords)
	: command(ms)
	, v(view)
	, c(NULL)
{
	maps_coordinates_clone(coords, &c);
}

session::command_view_set_center::~command_view_set_center()
{
	maps_coordinates_destroy(c);
}

int session::command_view_set_center::run()
{
	{ /* TODO: remove it in release */
		double lat = 0, lon = 0;
		maps_coordinates_get_latitude_longitude(c, &lat, &lon);
		MAPS_LOGD("session::command_view_set_center::run lat,lon=%f,%f",
			  lat, lon);
	}

	if(!v)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = map_view_set_center(v, c);
		if(error != MAPS_ERROR_NONE)
			break;
	} while(false);

	const int ret = error;
	destroy();
	return ret;
}

session::command_type_e session::command_view_set_center::get_type() const
{
	return MAP_VIEW_SET_CENTER_COMMAND;
}

int session::command_view_set_center::get_priority() const
{
	return 3;
}

void session::command_view_set_center::merge(const command *c)
{
	if (!c || (get_type() != c->get_type())) return;
	command_view_set_center *cmd = (command_view_set_center *)c;
	if (v == cmd->v) {
		double lat = .0;
		double lon = .0;
		maps_coordinates_get_latitude_longitude(cmd->c, &lat, &lon);
		maps_coordinates_set_latitude_longitude(this->c, lat, lon);
		cmd->set_merged();
	}
}


/*----------------------------------------------------------------------------*/


session::command_view_move_center::command_view_move_center(maps_service_h ms,
							   map_view_h view,
							   const int delta_x,
							   const int delta_y)
	: command(ms)
	, v(view)
	, _delta_x(delta_x)
	, _delta_y(delta_y)
{
}

session::command_view_move_center::~command_view_move_center()
{
}

int session::command_view_move_center::run()
{
	MAPS_LOGD("session::command_view_move_center::run "
		  "delta_x = %d, delta_y = %d",
		  _delta_x, _delta_y);

	if(!v)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = _map_view_move_center(v, _delta_x, _delta_y);
		if(error != MAPS_ERROR_NONE)
			break;
	} while(false);

	const int ret = error;
	destroy();
	return ret;
}

session::command_type_e session::command_view_move_center::get_type() const
{
	return MAP_VIEW_MOVE_CENTER_COMMAND;
}

int session::command_view_move_center::get_priority() const
{
	return 3;
}

void session::command_view_move_center::merge(const command *c)
{
	if (!c || (get_type() != c->get_type())) return;
	command_view_move_center *cmd = (command_view_move_center *)c;
	if (v == cmd->v) {
		_delta_x += cmd->_delta_x;
		_delta_y += cmd->_delta_y;
		cmd->set_merged();
	}
}

/*----------------------------------------------------------------------------*/

int session::command_view_zoom::run()
{
	MAPS_LOGD ("session::command_view_zoom::run factor = %f", zoom_factor);

	if (!v)
		return MAPS_ERROR_INVALID_PARAMETER;

	const int ret = map_view_set_zoom_factor (v, zoom_factor);

	destroy ();
	return ret;
}

session::command_type_e session::command_view_zoom::get_type() const
{
	return MAP_VIEW_ZOOM_COMMAND;
}

int session::command_view_zoom::get_priority() const
{
	return 2;
}

void session::command_view_zoom::merge(const command *c)
{
	if (!c || (get_type() != c->get_type())) return;
	command_view_zoom *cmd = (command_view_zoom *)c;
	if (v == cmd->v) {
		zoom_factor = cmd->zoom_factor;
		cmd->set_merged ();
	}
}

/*----------------------------------------------------------------------------*/
int session::command_view_rotate::run()
{
	MAPS_LOGD ("session::command_view_rotate::run angle = %f",
		  rotation_angle);

	if (!v)
		return MAPS_ERROR_INVALID_PARAMETER;

	const int ret = map_view_set_orientation(v, rotation_angle);

	destroy ();
	return ret;
}

session::command_type_e session::command_view_rotate::get_type() const
{
	return MAP_VIEW_ROTATE_COMMAND;
}

int session::command_view_rotate::get_priority() const
{
	return 2;
}

void session::command_view_rotate::merge(const command *c)
{
	if (!c || (get_type() != c->get_type())) return;
	command_view_rotate *cmd = (command_view_rotate *)c;
	if (v == cmd->v) {
		rotation_angle += cmd->rotation_angle;
		cmd->set_merged();
	}
}

/*----------------------------------------------------------------------------*/

int session::command_view_zoom_rotate::run()
{
	MAPS_LOGD ("session::command_view_zoom_rotate::run "
		   "factor = %f, angle = %f",
		   zoom_factor, rotation_angle);

	if (!v)
		return MAPS_ERROR_INVALID_PARAMETER;

	const int ret = _map_view_set_zoom_rotate(v,
						  true, zoom_factor,
						  true, rotation_angle);

	destroy ();
	return ret;
}

session::command_type_e session::command_view_zoom_rotate::get_type() const
{
	return MAP_VIEW_ZOOM_ROTATE_COMMAND;
}

int session::command_view_zoom_rotate::get_priority() const
{
	return 2;
}

void session::command_view_zoom_rotate::merge(const command *c)
{
	if (!c || (get_type() != c->get_type())) return;
	command_view_zoom_rotate *cmd = (command_view_zoom_rotate *)c;
	if (v == cmd->v) {
		zoom_factor = cmd->zoom_factor;
		rotation_angle += cmd->rotation_angle;
		cmd->set_merged();
	}
}

