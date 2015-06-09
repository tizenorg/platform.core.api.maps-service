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
		static void foreach_reverse_geocode_cb(maps_error_e result,
						       int request_id,
						       int index, int total,
						       maps_address_h address,
						       void *user_data);
		friend class command_reverse_geocode;
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
		 virtual int run();
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
		virtual int run();
	};
}

#endif				/* __MAPS_SERVICE_SESSION_COMMANDS_H__ */