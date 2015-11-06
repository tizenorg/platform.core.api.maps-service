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

#ifndef __MAPS_SERVICE_SESSION_COMMANDS_H__
#define __MAPS_SERVICE_SESSION_COMMANDS_H__

#include "command.h"

namespace session
{

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_geocode_f)(maps_service_h maps,
* const char *address, maps_service_geocode_cb callback,
* void *user_data, int* request_id); */
class command_geocode : public command
{
public:
	const string address;
	maps_item_hashtable_h preference;
	maps_service_geocode_cb callback;
	void *user_data;
	int error;
public:
	command_geocode(maps_service_h ms,
			 const string address,
			 const maps_item_hashtable_h preference,
			 maps_service_geocode_cb callback,
			 void *user_data, int *request_id);
	virtual ~command_geocode();
private:
	command_geocode() : command(NULL)
	{
	}
	command_geocode(const command_geocode &src) : command(NULL)
	{
	}
	command_geocode &operator=(const command_geocode &src)
	{
		return *this;
	}
private:
	virtual int run();
};

class command_geocode_handler : public command_handler
{
	maps_service_geocode_cb callback;
public:
	command_geocode_handler(plugin::plugin_s *plugin,
				 maps_service_geocode_cb callback,
				 void *user_data,
				 int user_req_id);
	 virtual ~command_geocode_handler()
	{
	};
private:
	command_geocode_handler() : command_handler(NULL, NULL, 0) {}
	command_geocode_handler(const command_geocode_handler &src) :
		 command_handler(NULL, NULL, 0)
	{
	}
	command_geocode_handler &operator=(
		const command_geocode_handler &src)
	{
		return *this;
	}
private:
	static bool foreach_geocode_cb(maps_error_e result,
				       int request_id, int index,
				       int total_count,
				       maps_coordinates_h coordinates,
				       void *user_data);
	friend class command_geocode;
	friend class command_geocode_inside_bounds;
	friend class command_geocode_by_structured_address;
};

/*----------------------------------------------------------------------------*/

/* TODO: rename to command_geocode_inside_area */

/*typedef int (*maps_plugin_geocode_inside_area_f)(maps_service_h maps,
* const char *address, maps_area_h bounds,
* maps_service_geocode_cb callback, void *user_data,
* int* request_id); */
class command_geocode_inside_bounds : public command
{
public:
	string address;
	maps_area_h bounds;
	maps_item_hashtable_h preference;
	maps_service_geocode_cb callback;
	void *user_data;
	int error;
public:
	command_geocode_inside_bounds(maps_service_h ms,
				       const char *address,
				       const maps_area_h bounds,
				       const maps_item_hashtable_h
				       preference,
				       maps_service_geocode_cb callback,
				       void *user_data,
				       int *request_id);
	virtual ~command_geocode_inside_bounds();
private:
	command_geocode_inside_bounds() : command(NULL)
	{
	}
	command_geocode_inside_bounds(
		const command_geocode_inside_bounds &src)
		 : command(NULL)
	{
	}
	command_geocode_inside_bounds &operator=(
		const command_geocode_inside_bounds &src)
	{
		return *this;
	}
private:
	virtual int run();
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_geocode_by_structured_address_f)(
* maps_service_h maps, maps_address_h address,
* maps_preference_h preference, maps_service_geocode_cb callback,
* void *user_data, int *request_id); */
class command_geocode_by_structured_address : public command
{
public:
	maps_address_h address;
	maps_item_hashtable_h preference;
	maps_service_geocode_cb callback;
	void *user_data;
	int error;
public:
	command_geocode_by_structured_address(maps_service_h ms,
					       const maps_address_h
					       address,
					       const
					       maps_item_hashtable_h
					       preference,
					       maps_service_geocode_cb
					       callback,
					       void *user_data,
					       int *request_id);
	virtual ~command_geocode_by_structured_address();
private:
	command_geocode_by_structured_address() : command(NULL)
	{
	}
	command_geocode_by_structured_address(
		const command_geocode_by_structured_address &src)
		 : command(NULL)
	{
	}
	command_geocode_by_structured_address &operator=(
		const command_geocode_by_structured_address &src)
	{
		return *this;
	}
private:
	virtual int run();
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_reverse_geocode_f)(maps_service_h maps,
* double latitude, double longitude,
* maps_service_reverse_geocode_cb callback, void *user_data,
* int *request_id); */
class command_reverse_geocode : public command
{
public:
	const string address;
	double latitude;
	double longitude;
	maps_item_hashtable_h preference;
	maps_service_reverse_geocode_cb callback;
	void *user_data;
	int error;
public:
	command_reverse_geocode(maps_service_h ms,
				 double latitude, double longitude,
				 const maps_item_hashtable_h preference,
				 maps_service_reverse_geocode_cb
				 callback,
				 void *user_data, int *request_id);
	virtual ~command_reverse_geocode();
private:
	command_reverse_geocode() : command(NULL)
	{
	}
	command_reverse_geocode(const command_reverse_geocode &src)
		 : command(NULL)
	{
	}
	command_reverse_geocode& operator=(
		const command_reverse_geocode &src)
	{
		return *this;
	}
private:
	virtual int run();
};

class command_reverse_geocode_handler : public command_handler
{
	maps_service_reverse_geocode_cb callback;
public:
	command_reverse_geocode_handler(plugin::plugin_s *plugin,
					 maps_service_reverse_geocode_cb
					 callback,
					 void *user_data,
					 int user_req_id);
	virtual ~command_reverse_geocode_handler()
	{
	};
private:
	command_reverse_geocode_handler()
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_reverse_geocode_handler(
		command_reverse_geocode_handler &src)
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_reverse_geocode_handler &operator=(
		const command_reverse_geocode_handler &src)
	{
		return *this;
	}
private:
	static void foreach_reverse_geocode_cb(maps_error_e result,
					       int request_id,
					       int index, int total,
					       maps_address_h address,
					       void *user_data);
	friend class command_reverse_geocode;
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_multi_reverse_geocode_f)(maps_service_h maps,
* maps_maps_h maps_list, maps_preference_h preference,
* maps_service_multi_reverse_geocode_cb callback, void *user_data, * int *request_id); */
class command_multi_reverse_geocode : public command
{
public:
	const string address;
	maps_coordinates_list_h maps_list;
	maps_item_hashtable_h preference;
	maps_service_multi_reverse_geocode_cb callback;
	void *user_data;
	int error;
public:
	command_multi_reverse_geocode(maps_service_h ms,
				 const maps_coordinates_list_h maps_list,
				 const maps_item_hashtable_h preference,
				 maps_service_multi_reverse_geocode_cb callback,
				 void *user_data, int *request_id);
	virtual ~command_multi_reverse_geocode();
private:
	command_multi_reverse_geocode() : command(NULL)
	{
	}
	command_multi_reverse_geocode(const command_multi_reverse_geocode &src)
		 : command(NULL)
	{
	}
	command_multi_reverse_geocode& operator=(
		const command_multi_reverse_geocode &src)
	{
		return *this;
	}
private:
	virtual int run();
};

class command_multi_reverse_geocode_handler : public command_handler
{
	maps_service_multi_reverse_geocode_cb callback;
public:
	command_multi_reverse_geocode_handler(plugin::plugin_s *plugin,
					 maps_service_multi_reverse_geocode_cb callback,
					 void *user_data,
					 int user_req_id);
	virtual ~command_multi_reverse_geocode_handler()
	{
	};
private:
	command_multi_reverse_geocode_handler()
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_multi_reverse_geocode_handler(
		command_multi_reverse_geocode_handler &src)
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_multi_reverse_geocode_handler &operator=(
		const command_multi_reverse_geocode_handler &src)
	{
		return *this;
	}
private:
	static bool foreach_multi_reverse_geocode_cb(maps_error_e result,
					       int request_id,
						   int total,
						   maps_coordinates_list_h address_list,
					       void *user_data);
	friend class command_multi_reverse_geocode;
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_place_f)(maps_service_h maps,
* maps_coordinates_h position, int distance,
* maps_item_hashtable_h preference, maps_place_filter_h filter,
* maps_service_search_place_cb callback, void *user_data,
* int *request_id); */
class command_search_place : public command
{
public:
	maps_coordinates_h position;
	int distance;
	maps_item_hashtable_h preference;
	maps_place_filter_h filter;
	maps_service_search_place_cb callback;
	void *user_data;

	int error;
public:
	command_search_place(maps_service_h ms,
			      const maps_coordinates_h position,
			      int distance,
			      const maps_item_hashtable_h preference,
			      const maps_place_filter_h filter,
			      maps_service_search_place_cb callback,
			      void *user_data, int *request_id);
	virtual ~command_search_place();
private:
	command_search_place() : command(NULL)
	{
	}
	command_search_place(const command_search_place &src)
		 : command(NULL)
	{
	}
	command_search_place &operator=(
		const command_search_place &src)
	{
		return *this;
	}
private:
	virtual int run();
};

class command_search_place_handler : public command_handler
{
	maps_service_search_place_cb callback;
public:
	command_search_place_handler(plugin::plugin_s *plugin,
				      maps_service_search_place_cb
				      callback,
				      void *user_data, int user_req_id);
	virtual ~command_search_place_handler()
	{
	};
private:
	command_search_place_handler()
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_search_place_handler(
		const command_search_place_handler &src)
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_search_place_handler &operator=(
		const command_search_place_handler &src)
	{ return *this;
	}
private:
	static bool foreach_place_cb(maps_error_e error, int request_id,
				     int index, int length,
				     maps_place_h place,
				     void *user_data);
	void set_supported_data(maps_place_h place);
	friend class command_search_place;
	friend class command_search_by_area_place;
	friend class command_search_by_address_place;
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_place_by_area_f)(maps_service_h maps,
* maps_area_h boundary , maps_item_hashtable_h preference,
* maps_place_filter_h filter, maps_service_search_place_cb callback,
* void *user_data, int *request_id); */
class command_search_by_area_place : public command
{
public:
	maps_area_h boundary;
	maps_item_hashtable_h preference;
	maps_place_filter_h filter;
	maps_service_search_place_cb callback;
	void *user_data;

	int error;
public:
	command_search_by_area_place(maps_service_h ms,
				      const maps_area_h boundary,
				      const maps_item_hashtable_h
				      preference,
				      const maps_place_filter_h filter,
				      maps_service_search_place_cb
				      callback,
				      void *user_data, int *request_id);
	virtual ~command_search_by_area_place();
private:
	command_search_by_area_place() : command(NULL)
	{
	}
	command_search_by_area_place(
		const command_search_by_area_place &src)
		 : command(NULL)
	{
	}
	command_search_by_area_place &operator=(
		const command_search_by_area_place &src)
	{
		return *this;
	}
private:
	virtual int run();
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_place_by_address_f)(
* maps_service_h maps, const char *address, maps_area_h boundary,
* maps_place_preference_h preference, maps_place_filter_h filter,
* maps_service_search_place_cb callback, void *user_data,
* int *request_id); */
class command_search_by_address_place : public command
{
public:
	const string address;
	maps_area_h boundary;
	maps_item_hashtable_h preference;
	maps_place_filter_h filter;
	maps_service_search_place_cb callback;
	void *user_data;

	int error;
public:
	command_search_by_address_place(maps_service_h ms,
		const char *address,
		const maps_area_h boundary,
		const maps_item_hashtable_h preference,
		const maps_place_filter_h filter,
		maps_service_search_place_cb callback,
		void *user_data, int *request_id);
	virtual ~command_search_by_address_place();
private:
	command_search_by_address_place() : command(NULL)
	{
	}
	command_search_by_address_place(
		const command_search_by_address_place &src)
		 : command(NULL)
	{
	}
	command_search_by_address_place &operator=(
		const command_search_by_address_place &src)
	{
		return *this;
	}
private:
	virtual int run();
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_place_list_f)(maps_service_h maps,
* maps_area_h boundary, maps_item_hashtable_h preference, maps_place_filter_h filter,
* maps_service_search_place_list_cb callback, void *user_data,
* int *request_id); */
class command_search_place_list : public command
{
public:
	maps_area_h boundary;
	maps_item_hashtable_h preference;
	maps_place_filter_h filter;
	maps_service_search_place_list_cb callback;
	void *user_data;

	int error;
public:
	command_search_place_list(maps_service_h ms,
				const maps_area_h boundary,
				const maps_item_hashtable_h preference,
				const maps_place_filter_h filter,
				maps_service_search_place_list_cb callback,
				void *user_data, int *request_id);
	virtual ~command_search_place_list();
private:
	command_search_place_list() : command(NULL)
	{
	}
	command_search_place_list(const command_search_place_list &src)
		 : command(NULL)
	{
	}
	command_search_place_list &operator=(
		const command_search_place_list &src)
	{
		return *this;
	}
private:
	virtual int run();
};

class command_search_place_list_handler : public command_handler
{
	maps_service_search_place_list_cb callback;
public:
	command_search_place_list_handler(plugin::plugin_s *plugin,
					maps_service_search_place_list_cb callback,
					void *user_data, int user_req_id);
	virtual ~command_search_place_list_handler()
	{
	};
private:
	command_search_place_list_handler()
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_search_place_list_handler(
		const command_search_place_list_handler &src)
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_search_place_list_handler &operator=(
		const command_search_place_list_handler &src)
	{ return *this;
	}
private:
	static void foreach_place_around_cb(maps_error_e error, int request_id,
					maps_place_list_h place, void *user_data);
	friend class command_search_place_list;
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_get_place_details_f) */
class command_get_place_details : public command
{
public:
	const string url;
	maps_service_get_place_details_cb callback;
	void *user_data;

	int error;
public:
	 command_get_place_details(maps_service_h ms, const char *url,
		maps_service_get_place_details_cb callback, void *user_data, int *request_id);
	 virtual ~command_get_place_details();
private:
	command_get_place_details() : command(NULL)
	{
	}
	command_get_place_details(
		const command_get_place_details &src)
		 : command(NULL)
	{
	}
	command_get_place_details &operator=(
		const command_get_place_details &src)
	{
		return *this;
	}
private:
	virtual int run();
};

class command_get_place_details_handler : public command_handler
{
	maps_service_get_place_details_cb callback;
public:
	command_get_place_details_handler(plugin::plugin_s *plugin,
						maps_service_get_place_details_cb callback,
						void *user_data, int user_req_id);
	virtual ~command_get_place_details_handler()
	{
	};
private:
	command_get_place_details_handler()
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_get_place_details_handler(
		const command_get_place_details_handler &src)
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_get_place_details_handler &operator=(
		const command_get_place_details_handler &src)
	{ return *this;
	}
private:
	static void foreach_place_details_cb(maps_error_e error, int request_id,
						maps_place_h place, void *user_data);
	friend class command_get_place_details;
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_route_f)(
* maps_service_h maps, maps_item_hashtable_h preference,
* maps_coordinates_h origin, maps_coordinates_h destination,
* maps_service_search_route_cb callback, void *user_data,
* int *request_id); */
class command_search_route_handler : public command_handler
{
	maps_service_search_route_cb callback;
public:
	command_search_route_handler(plugin::plugin_s *plugin,
				      maps_service_search_route_cb
				      callback,
				      void *user_data, int user_req_id);
	virtual ~command_search_route_handler()
	{
	};
private:
	command_search_route_handler()
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_search_route_handler(
		const command_search_route_handler &src)
		 : command_handler(NULL, NULL, 0)
	{
	}
	command_search_route_handler &operator=(
		const command_search_route_handler &src)
	{
		return *this;
	}
private:
	static bool foreach_route_cb(maps_error_e error, int request_id,
				     int index, int length,
				     maps_route_h route,
				     void *user_data);
	void set_supported_data(maps_route_h route);
	friend class command_search_route;
	friend class command_search_route_waypoints;
};

class command_search_route : public command
{
public:
	maps_item_hashtable_h preference;
	maps_coordinates_h origin;
	maps_coordinates_h destination;
	maps_service_search_route_cb callback;
	void *user_data;

	int error;
public:
	command_search_route(maps_service_h ms,
			      const maps_item_hashtable_h preference,
			      const maps_coordinates_h origin,
			      const maps_coordinates_h destination,
			      maps_service_search_route_cb callback,
			      void *user_data, int *request_id);
	virtual ~command_search_route();
private:
	command_search_route() : command(NULL)
	{
	}
	command_search_route(
		const command_search_route &src)
		 : command(NULL)
	{
	}
	command_search_route &operator=(
		const command_search_route &src)
	{
		return *this;
	}
private:
	virtual int run();
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_search_route_f)(maps_service_h maps,
* maps_item_hashtable_h preference, maps_coordinates_h *waypoint_list,
* int waypoint_num, maps_service_search_route_cb callback,
* void *user_data, int *request_id); */
class command_search_route_waypoints:public command
{
public:
	maps_item_hashtable_h preference;
	maps_coordinates_h *waypoint_list;
	int waypoint_num;
	maps_service_search_route_cb callback;
	void *user_data;

	int error;
public:
	command_search_route_waypoints(maps_service_h ms,
				const maps_item_hashtable_h preference,
				const maps_coordinates_h *waypoint_list,
				int waypoint_num,
				maps_service_search_route_cb callback,
				void *user_data, int *request_id);
	virtual ~command_search_route_waypoints();
private:
	command_search_route_waypoints() : command(NULL)
	{
	}
	command_search_route_waypoints(
		const command_search_route_waypoints &src)
		 : command(NULL)
	{
	}
	command_search_route_waypoints &operator=(
		const command_search_route_waypoints &src)
	{
		return *this;
	}
private:
	virtual int run();
};

/*----------------------------------------------------------------------------*/
/*typedef int (*maps_plugin_cancel_request_f)(maps_service_h maps,
* int request_id); */
class command_cancel_request : public command
{
public:
	int request_id;
public:
	command_cancel_request(maps_service_h ms, int rid)
	 : command(ms),
		request_id(rid)
	{
	}
	virtual ~command_cancel_request()
	{
	};
private:
	command_cancel_request() : command(NULL)
	{
	}
	command_cancel_request(const command_cancel_request &src)
		: command(NULL)
	{
	}
	command_cancel_request &operator=(
		const command_cancel_request &src)
	{
		return *this;
	}
private:
	virtual int run();
};


/*----------------------------------------------------------------------------*/
/*
*		Mapping API commands
*/
/*----------------------------------------------------------------------------*/

class command_view_set_center : public command {
private:
	map_view_h v;
public:
	maps_coordinates_h c;
public:
	command_view_set_center(maps_service_h ms, map_view_h view,
				const maps_coordinates_h coords);
	virtual ~command_view_set_center();
private:
	virtual int run();
private:
	virtual command_type_e get_type() const;
	virtual int get_priority() const;
	virtual void merge(const command *c);
};

class command_view_move_center : public command {
private:
	map_view_h v;
public:
	int _delta_x;
	int _delta_y;
public:
	command_view_move_center(maps_service_h ms, map_view_h view,
				 const int delta_x, const int delta_y);
	virtual ~command_view_move_center();
private:
	virtual int run();
private:
	virtual command_type_e get_type() const;
	virtual int get_priority() const;
	virtual void merge(const command *c);
};

class command_view_zoom : public command {
private:
	map_view_h v;
public:
	double zoom_factor;
public:
	command_view_zoom(maps_service_h ms, map_view_h view,
			  const double  &factor) :
		command(ms), v(view), zoom_factor(factor)
	{
	}
	virtual ~command_view_zoom()
	{
	}
private:
	virtual int run();
private:
	virtual command_type_e get_type() const;
	virtual int get_priority() const;
	virtual void merge(const command *c);
};

class command_view_rotate : public command {
private:
	map_view_h v;
public:
	double rotation_angle;
public:
	command_view_rotate(maps_service_h ms, map_view_h view,
			    const double  &angle) :
		command(ms), v(view), rotation_angle(angle)
	{
	}
	virtual ~command_view_rotate()
	{
	}
private:
	virtual int run();
private:
	virtual command_type_e get_type() const;
	virtual int get_priority() const;
	virtual void merge(const command *c);
};

class command_view_zoom_rotate : public command {
private:
	map_view_h v;
public:
	double zoom_factor;
	double rotation_angle;
public:
	command_view_zoom_rotate(maps_service_h ms, map_view_h view,
				 const double  &factor,
				 const double  &angle)
		: command(ms)
		  , v(view)
		  , zoom_factor(factor)
		  , rotation_angle(angle)
	{
	}
	virtual ~command_view_zoom_rotate()
	{
	}
private:
	virtual int run();
private:
	virtual command_type_e get_type() const;
	virtual int get_priority() const;
	virtual void merge(const command *c);
};

class command_view_ready : public command {
private:
	map_view_h v;
public:
	command_view_ready(maps_service_h ms, map_view_h view) :
		command(ms), v(view)
	{
	}
	virtual ~command_view_ready()
	{
	}
private:
	virtual int run();
private:
	virtual command_type_e get_type() const;
	virtual int get_priority() const;
	virtual void merge(const command *c);
};
}

#endif				/* __MAPS_SERVICE_SESSION_COMMANDS_H__ */
