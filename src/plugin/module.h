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

#ifndef __MAPS_SERVICE_PLUGIN_MODULE_H__
#define __MAPS_SERVICE_PLUGIN_MODULE_H__

#include "maps_plugin.h"
#include "discovery.h"
#include <glib.h>
#include <gmodule.h>

/* ------------------------------------------------------------------------- */
/*
*  Plugin dedicated functions
*/
typedef int (*maps_plugin_init_f) (maps_plugin_h *plugin);
typedef int (*maps_plugin_shutdown_f) (maps_plugin_h plugin);
typedef int (*maps_plugin_get_info_f) (maps_plugin_info_h *info);

/* Maps Provider access key, preference and capabilities */
typedef int (*maps_plugin_set_provider_key_f) (const char *provider_key);
typedef int (*maps_plugin_get_provider_key_f) (char **provider_key);
typedef int (*maps_plugin_set_preference_f) (maps_preference_h preference);
typedef int (*maps_plugin_get_preference_f) (maps_preference_h *preference);
typedef int (*maps_plugin_is_service_supported_f) (maps_service_e service,
						   bool *supported);
typedef int (*maps_plugin_is_data_supported_f) (maps_service_data_e data,
						bool *supported);

/* Geocode */
typedef int (*maps_plugin_geocode_f) (const char *address,
				      const maps_preference_h preference,
				      maps_service_geocode_cb callback,
				      void *user_data, int *request_id);
typedef int (*maps_plugin_geocode_inside_area_f) (const char *address,
						  const maps_area_h bounds,
						  const maps_preference_h
						  preference,
						  maps_service_geocode_cb
						  callback,
						  void *user_data,
						  int *request_id);
typedef int (*maps_plugin_geocode_by_structured_address_f) (const maps_address_h
							    address,
							    const
							    maps_preference_h
							    preference,
							maps_service_geocode_cb
							callback,
							void *user_data,
							int *request_id);
typedef int (*maps_plugin_reverse_geocode_f) (double latitude, double longitude,
					      const maps_preference_h
					      preference,
					      maps_service_reverse_geocode_cb
					      callback,
					      void *user_data, int *request_id);
typedef int (*maps_plugin_multi_reverse_geocode_f) (const maps_coordinates_list_h maps_list,
							const maps_preference_h preference,
							maps_service_multi_reverse_geocode_cb callback,
							void *user_data, int *request_id);

/* Place */
typedef int (*maps_plugin_search_place_f) (const maps_coordinates_h position,
					   int distance,
					   const maps_place_filter_h filter,
					   maps_preference_h preference,
					   maps_service_search_place_cb
					   callback,
					   void *user_data, int *request_id);
typedef int (*maps_plugin_search_place_by_area_f) (const maps_area_h boundary,
						   const maps_place_filter_h
						   filter,
						   maps_preference_h preference,
						   maps_service_search_place_cb
						   callback,
						   void *user_data,
						   int *request_id);
typedef int (*maps_plugin_search_place_by_address_f) (const char *address,
						      const maps_area_h
						      boundary,
						      const maps_place_filter_h
						      filter,
						      maps_preference_h
						      preference,
						maps_service_search_place_cb
						callback,
						void *user_data,
						int *request_id);

/* Route */
typedef int (*maps_plugin_search_route_f) (const maps_coordinates_h origin,
					   const maps_coordinates_h destination,
					   maps_preference_h preference,
					   maps_service_search_route_cb
					   callback,
					   void *user_data, int *request_id);
typedef int (*maps_plugin_search_route_waypoints_f) (const maps_coordinates_h *
						     waypoint_list,
						     int waypoint_num,
						     maps_preference_h
						     preference,
						maps_service_search_route_cb
						callback,
						void *user_data,
						int *request_id);

/* Cancel Request */
typedef int (*maps_plugin_cancel_request_f) (int request_id);

/* Mapping */
typedef int (*maps_plugin_set_map_view_f) (const map_view_h view);
typedef int (*maps_plugin_render_map_f) (const maps_coordinates_h coordinates,
					const double zoom_factor,
					const double rotation_angle,
					const double tilt,
					maps_plugin_render_map_cb callback,
					void* user_data,
					int* request_id);
typedef int (*maps_plugin_move_center_f) (const int delta_x,
					  const int delta_y,
					  maps_plugin_render_map_cb callback,
					  void* user_data,
					  int* request_id);
typedef int (*maps_plugin_draw_map_f) (Evas* canvas, const int x, const int y,
				       const int width, const int height);
typedef int (*maps_plugin_on_object_f) (const map_object_h object,
			       const map_object_operation_e operation);
typedef int (*maps_plugin_screen_to_geography_f) (const int x, const int y,
						  maps_coordinates_h*
						  coordinates);
typedef int (*maps_plugin_geography_to_screen_f) (const maps_coordinates_h
						  coordinates,
						  int* x, int* y);
typedef int (*maps_plugin_get_min_zoom_level_f) (int *min_zoom_level);
typedef int (*maps_plugin_get_max_zoom_level_f) (int *max_zoom_level);
typedef int (*maps_plugin_get_min_tilt_f) (int *min_tilt);
typedef int (*maps_plugin_get_max_tilt_f) (int *max_tilt);
typedef int (*maps_plugin_get_center_f) (maps_coordinates_h *coordinates);

namespace plugin {

	/* Plugin interface */
	typedef struct _interface_s {

		/* Plugin dedicated functions */
		maps_plugin_init_f maps_plugin_init;
		maps_plugin_shutdown_f maps_plugin_shutdown;
		maps_plugin_get_info_f maps_plugin_get_info;

		/* Maps Provider access key, preference and capabilities */
		maps_plugin_set_provider_key_f maps_plugin_set_provider_key;
		maps_plugin_get_provider_key_f maps_plugin_get_provider_key;
		maps_plugin_set_preference_f maps_plugin_set_preference;
		maps_plugin_get_preference_f maps_plugin_get_preference;
		maps_plugin_is_service_supported_f
			maps_plugin_is_service_supported;
		maps_plugin_is_data_supported_f maps_plugin_is_data_supported;

		/* Geocode */
		maps_plugin_geocode_f maps_plugin_geocode;
		maps_plugin_geocode_inside_area_f
			maps_plugin_geocode_inside_area;
		 maps_plugin_geocode_by_structured_address_f
			maps_plugin_geocode_by_structured_address;
		maps_plugin_reverse_geocode_f maps_plugin_reverse_geocode;
		maps_plugin_multi_reverse_geocode_f maps_plugin_multi_reverse_geocode;

		/* Place */
		maps_plugin_search_place_f maps_plugin_search_place;
		maps_plugin_search_place_by_area_f
			maps_plugin_search_place_by_area;
		maps_plugin_search_place_by_address_f
			maps_plugin_search_place_by_address;

		/* Route */
		maps_plugin_search_route_f maps_plugin_search_route;
		maps_plugin_search_route_waypoints_f
			maps_plugin_search_route_waypoints;

		/* Cancel Request */
		maps_plugin_cancel_request_f maps_plugin_cancel_request;

		/* Mapping */
		maps_plugin_set_map_view_f maps_plugin_set_map_view;
		maps_plugin_render_map_f maps_plugin_render_map;
		maps_plugin_move_center_f maps_plugin_move_center;
		maps_plugin_draw_map_f maps_plugin_draw_map;
		maps_plugin_on_object_f maps_plugin_on_object;
		maps_plugin_screen_to_geography_f
			maps_plugin_screen_to_geography;
		maps_plugin_geography_to_screen_f
			maps_plugin_geography_to_screen;
		maps_plugin_get_min_zoom_level_f maps_plugin_get_min_zoom_level;
		maps_plugin_get_max_zoom_level_f maps_plugin_get_max_zoom_level;
		maps_plugin_get_min_tilt_f maps_plugin_get_min_tilt;
		maps_plugin_get_max_tilt_f maps_plugin_get_max_tilt;
		maps_plugin_get_center_f maps_plugin_get_center;

	} interface_s;

	/* Plugin structure */
	typedef struct _plugin_s {
		interface_s interface;	/* Plugin interface function pointers */
		gpointer module;	/* Plugin module pointer, GMod */

#ifdef _MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_
		GAsyncQueue *request_queue;	/* Queue of asynchronous requests */
#endif /*_MAPS_SERVICE_SUPPORTS_ASYNC_QUEUE_*/
		GThread *thread;	/* Request queue thread: there is
					   happening delivery of request from
					   app to plugin */

		/* Flag: is the plugin in use */
		volatile bool is_working;	/* It is discouraged to use this
						   approach; */
		/* probably, it must be exchanged with event dispite of
		*  performance trade-off */

		/*maps_int_hashtable_h capabilities; // The table of plugin
		* capabilities */

		GMutex pending_request_mutex;	/* Mutex for synchronizing the
						   map of pending requests */
		GHashTable *pending_request_maps;	/* Map of pending
							   requests */
	} plugin_s;

	/* TODO: Refactor to encapsulate everithing in a single calss
	*  plugin::module */
	typedef struct _GMod {
		gchar *name;
		gchar *path;
		GModule *module;
	} GMod;

	/* RAII Class implementing the mutex helper on the base of GLib mutex */
	/* which automatically locks mutex during its creation and unlocks while
	*  exiting the scope */
	class scope_mutex {
		GMutex *mutex;
	public:
		 scope_mutex(GMutex *m);
		~scope_mutex();
	};

	class binary_extractor {
	public:
		binary_extractor();
		virtual ~binary_extractor()
		{
		}
	public:
		 provider_info get_plugin_info(const string &file_name) const;
		maps_plugin_h init(const provider_info &info);
		void shutdown(maps_plugin_h plugin_h);
	private:
		 GMod *gmod_new(const string &module_file,
				gboolean is_resident) const;
		void gmod_free(GMod *gmod) const;
		gpointer gmod_find_sym(GMod *gmod,
				       const gchar *func_name) const;
		void trace_dbg(const plugin_s *plugin) const;
	};
};

#endif				/* __MAPS_SERVICE_PLUGIN_MODULE_H__ */
