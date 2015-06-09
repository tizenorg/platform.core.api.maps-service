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
#include "maps_place_private.h"
#include "maps_route_private.h"

static int __put_to_hashtable(session::command_handler *ch,
			      maps_service_data_e feature,
			      const char *feature_str,
			      maps_string_hashtable_h t)
{
	if (!ch || !feature_str || !t)
		return MAPS_ERROR_INVALID_PARAMETER;
	bool supported = false;
	ch->plugin()->interface.maps_plugin_is_data_supported(feature,
		&supported);
	return (supported) ? maps_string_hashtable_set(t, feature_str,
		feature_str) : MAPS_ERROR_NONE;
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
 , preference(NULL)
 , callback(cb)
 , user_data(ud)
 , error(0)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_item_hashtable_clone(pref, &preference) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;
}

session::command_geocode::~command_geocode()
{
	maps_item_hashtable_destroy(preference);
}

int session::command_geocode::run()
{

	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());
	pr.add(my_req_id);

	command_geocode_handler *handler =
		new command_geocode_handler(plugin(), callback, user_data,
		my_req_id);
	error = interface()->maps_plugin_geocode(address.c_str(), preference,
		command_geocode_handler::foreach_geocode_cb, handler,
		&handler->plg_req_id);

	pr.update(my_req_id, handler);

	/*MAPS_LOGD("session::command_geocode::run: %d", my_req_id,
	* handler->plg_req_id); */

	destroy();
	return error;
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

	command_geocode_handler *handler =
		(command_geocode_handler *) user_data;

	if (request_id != handler->plg_req_id) {
		MAPS_LOGE(
"\n\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n\n",
			request_id, handler->plg_req_id);
	}

	/* Make a user's copy of result data */
	maps_coordinates_h cloned_result = NULL;
	if (error == MAPS_ERROR_NONE)
		error = (maps_error_e) maps_coordinates_clone(coordinates,
			&cloned_result);
	maps_coordinates_destroy(coordinates);

	/* Send data to user */
	const bool b =
		handler->callback(error, handler->user_req_id, index,
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
 , preference(NULL)
 , callback(cb)
 , user_data(ud)
 , error(0)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_area_clone(b, &bounds) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_item_hashtable_clone(pref, &preference) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;
}

session::command_geocode_inside_bounds::~command_geocode_inside_bounds()
{
	maps_area_destroy(bounds);
	maps_item_hashtable_destroy(preference);
}

int session::command_geocode_inside_bounds::run()
{
	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());
	pr.add(my_req_id);

	command_geocode_handler *handler =
		new command_geocode_handler(plugin(), callback, user_data,
		my_req_id);
	error = interface()->maps_plugin_geocode_inside_area(address.c_str(),
		bounds, preference, command_geocode_handler::foreach_geocode_cb,
		handler, &handler->plg_req_id);

	pr.update(my_req_id, handler);

	destroy();
	return error;
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
 , preference(NULL)
 , callback(cb)
 , user_data(ud)
 , error(0)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_address_clone(a, &address) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_item_hashtable_clone(pref, &preference) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;
}

session::command_geocode_by_structured_address::
	~command_geocode_by_structured_address()
{
	maps_address_destroy(address);
	maps_item_hashtable_destroy(preference);
}

int session::command_geocode_by_structured_address::run()
{
	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());
	pr.add(my_req_id);

	command_geocode_handler *handler =
		new command_geocode_handler(plugin(), callback, user_data,
		my_req_id);
	error = interface()->maps_plugin_geocode_by_structured_address(address,
		preference, command_geocode_handler::foreach_geocode_cb,
		handler, &handler->plg_req_id);

	pr.update(my_req_id, handler);

	destroy();
	return error;
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
 , preference(NULL)
 , callback(cb)
 , user_data(ud)
 , error(0)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_item_hashtable_clone(pref, &preference) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;
}

session::command_reverse_geocode::~command_reverse_geocode()
{
	maps_item_hashtable_destroy(preference);
}

int session::command_reverse_geocode::run()
{

	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());
	pr.add(my_req_id);

	command_reverse_geocode_handler *handler =
		new command_reverse_geocode_handler(plugin(), callback,
		user_data, my_req_id);
	error = interface()->maps_plugin_reverse_geocode(latitude, longitude,
		preference,
		command_reverse_geocode_handler::foreach_reverse_geocode_cb,
		handler, &handler->plg_req_id);

	pr.update(my_req_id, handler);

	destroy();
	return error;
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
 , preference(NULL)
 , filter(NULL)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_coordinates_clone(pos, &position) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_item_hashtable_clone(pref, &preference) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_place_filter_clone(flt, &filter) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;
}

session::command_search_place::~command_search_place()
{
	maps_coordinates_destroy(position);
	maps_item_hashtable_destroy(preference);
	maps_place_filter_destroy(filter);
}

int session::command_search_place::run()
{

	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());
	pr.add(my_req_id);

	command_search_place_handler *handler =
		new command_search_place_handler(plugin(), callback, user_data,
		my_req_id);
	error = interface()->maps_plugin_search_place(position, distance,
		filter, preference,
		command_search_place_handler::foreach_place_cb, handler,
		&handler->plg_req_id);

	pr.update(my_req_id, handler);

	/*MAPS_LOGD("session::command_search_place::run: %d", my_req_id,
	* handler->plg_req_id); */

	destroy();
	return error;
}

session::command_search_place_handler::command_search_place_handler(
						plugin::plugin_s* p,
						maps_service_search_place_cb cb,
						void *ud, int urid)
 : command_handler(p, ud, urid)
 , callback(cb)
{
}

void session::command_search_place_handler::set_supported_data(maps_place_h
							       place)
{
	if (!place || !plugin())
		return;

	maps_string_hashtable_h data_supported = NULL;
	if (maps_string_hashtable_create(&data_supported) != MAPS_ERROR_NONE)
		return;

	__put_to_hashtable(this, MAPS_PLACE_ADDRESS, _S(MAPS_PLACE_ADDRESS),
		data_supported);
	__put_to_hashtable(this, MAPS_PLACE_RATING, _S(MAPS_PLACE_RATING),
		data_supported);
	__put_to_hashtable(this, MAPS_PLACE_CATEGORIES,
		_S(MAPS_PLACE_CATEGORIES), data_supported);
	__put_to_hashtable(this, MAPS_PLACE_ATTRIBUTES,
		_S(MAPS_PLACE_ATTRIBUTES), data_supported);
	__put_to_hashtable(this, MAPS_PLACE_CONTACTS, _S(MAPS_PLACE_CONTACTS),
		data_supported);
	__put_to_hashtable(this, MAPS_PLACE_EDITORIALS,
		_S(MAPS_PLACE_EDITORIALS), data_supported);
	__put_to_hashtable(this, MAPS_PLACE_REVIEWS, _S(MAPS_PLACE_REVIEWS),
		data_supported);
	__put_to_hashtable(this, MAPS_PLACE_IMAGE, _S(MAPS_PLACE_IMAGE),
		data_supported);
	__put_to_hashtable(this, MAPS_PLACE_SUPPLIER, _S(MAPS_PLACE_SUPPLIER),
		data_supported);
	__put_to_hashtable(this, MAPS_PLACE_RELATED, _S(MAPS_PLACE_RELATED),
		data_supported);

	_maps_place_set_supported_data(place, data_supported);
	maps_string_hashtable_destroy(data_supported);
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
		MAPS_LOGE(
"\n\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n\n",
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
 , preference(NULL)
 , filter(NULL)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_area_clone(b, &boundary) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_item_hashtable_clone(pref, &preference) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_place_filter_clone(flt, &filter) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;
}

session::command_search_by_area_place::~command_search_by_area_place()
{
	maps_area_destroy(boundary);
	maps_item_hashtable_destroy(preference);
	maps_place_filter_destroy(filter);
}

int session::command_search_by_area_place::run()
{

	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());
	pr.add(my_req_id);

	command_search_place_handler *handler =
		new command_search_place_handler(plugin(), callback, user_data,
		my_req_id);
	error = interface()->maps_plugin_search_place_by_area(boundary, filter,
		preference, command_search_place_handler::foreach_place_cb,
		handler, &handler->plg_req_id);

	pr.update(my_req_id, handler);

	/*MAPS_LOGD("session::command_search_by_area_place::run: %d", my_req_id,
	* handler->plg_req_id); */

	destroy();
	return error;
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
 , preference(NULL)
 , filter(NULL)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_area_clone(b, &boundary) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_item_hashtable_clone(pref, &preference) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_place_filter_clone(flt, &filter) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;
}

session::command_search_by_address_place::~command_search_by_address_place()
{
	maps_area_destroy(boundary);
	maps_item_hashtable_destroy(preference);
	maps_place_filter_destroy(filter);
}

int session::command_search_by_address_place::run()
{
	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());
	pr.add(my_req_id);

	command_search_place_handler *handler =
		new command_search_place_handler(plugin(), callback, user_data,
		my_req_id);
	error = interface()->maps_plugin_search_place_by_address(address.
		c_str(), boundary, filter, preference,
		command_search_place_handler::foreach_place_cb, handler,
		&handler->plg_req_id);

	pr.update(my_req_id, handler);

	/*MAPS_LOGD("session::command_search_by_address_place::run: %d",
	* my_req_id, handler->plg_req_id); */

	destroy();
	return error;
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

	if (maps_item_hashtable_clone(pref, &preference) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_coordinates_clone(orig, &origin) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

	if (maps_coordinates_clone(dest, &destination) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

}

session::command_search_route::~command_search_route()
{
	maps_item_hashtable_destroy(preference);
	maps_coordinates_destroy(origin);
	maps_coordinates_destroy(destination);
}

int session::command_search_route::run()
{

	if (error != MAPS_ERROR_NONE)
		return error;

	pending_request pr(plugin());
	pr.add(my_req_id);

	command_search_route_handler *handler =
		new command_search_route_handler(plugin(), callback, user_data,
		my_req_id);
	error = interface()->maps_plugin_search_route(origin, destination,
		preference, command_search_route_handler::foreach_route_cb,
		handler, &handler->plg_req_id);

	pr.update(my_req_id, handler);

	/*MAPS_LOGD("session::command_search_route::run: %d", my_req_id,
	* handler->plg_req_id); */

	destroy();
	return error;
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
 , preference(NULL)
 , waypoint_list(NULL)
 , waypoint_num(num)
 , callback(cb)
 , user_data(ud)
 , error(MAPS_ERROR_NONE)
{
	*request_id = command::command_request_id++;
	my_req_id = *request_id;

	if (maps_item_hashtable_clone(pref, &preference) != MAPS_ERROR_NONE)
		error = MAPS_ERROR_INVALID_PARAMETER;

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
	maps_item_hashtable_destroy(preference);

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
	pr.add(my_req_id);

	command_search_route_handler *handler =
		new command_search_route_handler(plugin(), callback, user_data,
		my_req_id);
	error = interface()->maps_plugin_search_route_waypoints(waypoint_list,
		waypoint_num, preference,
		command_search_route_handler::foreach_route_cb, handler,
		&handler->plg_req_id);

	pr.update(my_req_id, handler);

	/*MAPS_LOGD("session::command_search_place::run: %d", my_req_id,
	* handler->plg_req_id); */

	destroy();
	return error;
}

session::command_search_route_handler::command_search_route_handler(
						plugin::plugin_s *p,
						maps_service_search_route_cb cb,
						void *ud, int urid)
 : command_handler(p, ud, urid)
 , callback(cb)
{
}

void session::command_search_route_handler::set_supported_data(maps_route_h
							       route)
{
	if (!route || !plugin())
		return;

	maps_string_hashtable_h data_supported = NULL;
	if (maps_string_hashtable_create(&data_supported) != MAPS_ERROR_NONE)
		return;

	__put_to_hashtable(this, MAPS_ROUTE_PATH, _S(MAPS_ROUTE_PATH),
		data_supported);
	__put_to_hashtable(this, MAPS_ROUTE_SEGMENTS_PATH,
		_S(MAPS_ROUTE_SEGMENTS_PATH), data_supported);
	__put_to_hashtable(this, MAPS_ROUTE_SEGMENTS_MANEUVERS,
		_S(MAPS_ROUTE_SEGMENTS_MANEUVERS), data_supported);

	_maps_route_set_supported_data(route, data_supported);
	maps_string_hashtable_destroy(data_supported);
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
		MAPS_LOGE(
"\n\nERROR! Incorrect request id [%d] come from the plugin; expected [%d]\n\n",
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
	MAPS_LOGD("session::command_cancel_request::run: %d, %d", request_id,
		pr.look_up(request_id));
	const int error =
		(pr.contains(request_id)) ? interface()->
		maps_plugin_cancel_request(pr.
		extract_plg_id(request_id)) : MAPS_ERROR_NOT_FOUND;
	destroy();
	return error;
}