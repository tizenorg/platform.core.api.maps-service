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

#include <glib.h>
#include <unistd.h>
#include <system_info.h>
#include "maps_service.h"
#include "maps_error.h"
#include "maps_service.h"
#include "maps_util.h"

#include "command_queue.h"
#include "commands.h"
#include "discovery.h"
#include "module.h"

/*----------------------------------------------------------------------------*/
/* Structure of maps_service */
/* maps_service_s* is used as maps_service_h */
typedef struct _maps_service_s
{
	maps_plugin_h plugin;
} maps_service_s;

const gsize _MAPS_PROVIDER_KEY_MAX_LENGTH = 1024;

/* This function is used in command class */
plugin::plugin_s *__extract_plugin(maps_service_h maps)
{
	if (!maps)
		return NULL;
	maps_service_s *maps_service = (maps_service_s *) maps;
	return (plugin::plugin_s *) maps_service->plugin;
}

static bool __maps_provider_supported(maps_service_h maps, maps_service_e service)
{
	if (!maps)
		return false;
	bool supported = false;
	if (maps_service_provider_is_service_supported(maps, service, &supported)
		!= MAPS_ERROR_NONE)
		return false;
	return supported;
}

bool _is_internet_feature_supported(void)
{
	static bool __is_checked = false;
	static bool __is_supported = true;

	if (!__is_checked) {
		char *profile = NULL;
		int ret = system_info_get_platform_string("http://tizen.org/feature/profile", &profile);
		if (ret == SYSTEM_INFO_ERROR_NONE && profile && *profile) {
			MAPS_LOGD("profile : %s", profile);

			/* if wearable, check internet feature in addition */
			if (!strcmp("wearable", profile)) {
				ret = system_info_get_platform_bool("http://tizen.org/feature/network.internet", &__is_supported);
				MAPS_LOGD("internet feature supported : %d", __is_supported);
			}
			g_free(profile);

			/* set the flag which means feature checked */
			__is_checked = (ret == SYSTEM_INFO_ERROR_NONE);
		}
 	}

	return __is_supported;
/*
	if (!is_checked_profile

	bool is_supported = false;
	int retval = 0;
	retval = system_info_get_platform_bool("http://tizen.org/feature/network.internet", &is_supported);
	if (retval != SYSTEM_INFO_ERROR_NONE)
		MAPS_LOGE("SYSTEM_INFO_ERROR: network.internet (%d)", retval);
MAPS_LOGD("is_supported=%d", is_supported);
	return is_supported;
#else
MAPS_LOGD("is_supported=true");
	return true;
#endif
*/
}

static bool __has_maps_service_privilege()
{
	extern const char *MAPS_PLUGINS_PATH_PREFIX;
	return (access(MAPS_PLUGINS_PATH_PREFIX, F_OK) != 0) || /* not exist */
	       (access(MAPS_PLUGINS_PATH_PREFIX, R_OK) == 0);   /* readable */
}

/*----------------------------------------------------------------------------*/
/* */
/* Maps Service & Preference */

EXPORT_API int maps_service_foreach_provider(maps_service_provider_info_cb callback,
					     void *user_data)
{
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* The list of map provider info, obtained by enumerating available plugins */
	plugin::discovery pd;
	vector <plugin::provider_info> v = pd.get_available_list();

	/* Send obtained provider info to the user */
	const int total = int(v.size());
	for (int i = 0; i < total; i++) {
		/* Get a plugin info handle from the array */
		/* and send a callback with provider info to the user */
		char *provider = g_strdup(v[i].provider.c_str());
		if (!callback(provider, user_data))
			break;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_service_create(const char *maps_provider, maps_service_h *maps)
{
	/* Check if parameters are valid */
	if (!maps || !maps_provider)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege()) {
		MAPS_LOGD("ERROR: privilege is not included");
		return MAPS_ERROR_PERMISSION_DENIED;
	}

	int error = MAPS_ERROR_NOT_SUPPORTED;
	char *provider = NULL, *module = NULL;

	do {
		/* 0. Find the plugin, requested by the user */
		plugin::split_provider_name(maps_provider, &provider, &module);
		const plugin::provider_info info = plugin::find_by_names(provider);

		/* 1. Check whether provider info is valid */
		if (info.empty()) {
			MAPS_LOGE("ERROR! Provider info not found for name: %s", maps_provider);
			error = MAPS_ERROR_NOT_SUPPORTED;
			break;
		}

		/* 2. Setup maps service handling structure */
		maps_service_s *maps_service = g_slice_new0(maps_service_s);

		if (maps_service == NULL) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
			error = MAPS_ERROR_OUT_OF_MEMORY;
			break;
		}

		/* 3. Initialize the requested plugin */
		int init_error = MAPS_ERROR_NONE; /* Storage for init error code */
		maps_plugin_h plugin_h = plugin::binary_extractor().init(info, module, &init_error);
		if (!plugin_h) {
			error = init_error;
			MAPS_LOGE("ERROR! Plugin init failed");
			break;
		}

		maps_service->plugin = plugin_h;

		/* 4. Initialize an output pointer to maps service */
		*maps = maps_service;

		/* 5. Set status of completely correct plugin initialization */
		error = MAPS_ERROR_NONE;
	} while (false);

	if (error != MAPS_ERROR_NONE)
		maps_service_destroy(*maps);

	g_free(provider);
	g_free(module);
	return error;
}

EXPORT_API int maps_service_destroy(maps_service_h maps)
{
	/* Check if parameters are valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	maps_service_s *maps_service = (maps_service_s *) maps;

	if (maps_service->plugin)
		plugin::binary_extractor().shutdown(maps_service->plugin);

	g_slice_free(maps_service_s, maps);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_service_set_provider_key(maps_service_h maps,
					     const char *provider_key)
{
	if (!maps || !provider_key)
		return MAPS_ERROR_INVALID_PARAMETER;
	const plugin::plugin_s *p = __extract_plugin(maps);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;
	return p->interface.maps_plugin_set_provider_key(provider_key);
}

EXPORT_API int maps_service_get_provider_key(const maps_service_h maps,
					     char **provider_key)
{
	if (!maps || !provider_key)
		return MAPS_ERROR_INVALID_PARAMETER;
	const plugin::plugin_s *p = __extract_plugin(maps);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;
	return p->interface.maps_plugin_get_provider_key(provider_key);
}

EXPORT_API int maps_service_set_preference(maps_service_h maps,
					   maps_string_hashtable_h preference)
{
	if (!maps || !preference)
		return MAPS_ERROR_INVALID_PARAMETER;
	const plugin::plugin_s *p = __extract_plugin(maps);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;
	return p->interface.maps_plugin_set_preference(preference);
}

EXPORT_API int maps_service_get_preference(maps_service_h maps,
					   maps_string_hashtable_h *preference)
{
	if (!maps || !preference)
		return MAPS_ERROR_INVALID_PARAMETER;
	const plugin::plugin_s *p = __extract_plugin(maps);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;
	return p->interface.maps_plugin_get_preference(preference);
}

EXPORT_API int maps_service_provider_is_service_supported(const maps_service_h maps,
							maps_service_e service,
							bool *supported)
{
	if (!maps || !supported)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((service < MAPS_SERVICE_GEOCODE)
	    || (service > MAPS_SERVICE_SEARCH_PLACE_LIST))
		return MAPS_ERROR_INVALID_PARAMETER;
	const plugin::plugin_s *p = __extract_plugin(maps);
	if (!p)
		return MAPS_ERROR_NOT_SUPPORTED;
	return p->interface.maps_plugin_is_service_supported(service, supported);
}

EXPORT_API int maps_service_provider_is_data_supported(const maps_service_h maps,
						       maps_service_data_e data,
						       bool *supported)
{
	if (!maps || !supported)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((data < MAPS_PLACE_ADDRESS)
	    || (data > MAPS_ROUTE_SEGMENTS_MANEUVERS))
		return MAPS_ERROR_INVALID_PARAMETER;

	const plugin::plugin_s *p = __extract_plugin(maps);
	if (!p)
		return MAPS_ERROR_NOT_SUPPORTED;
	return p->interface.maps_plugin_is_data_supported(data, supported);
}

/*----------------------------------------------------------------------------*/
/* */
/* Geocoder Service */

EXPORT_API int maps_service_geocode(const maps_service_h maps,
				    const char *address,
				    const maps_preference_h preference,
				    maps_service_geocode_cb callback,
				    void *user_data,
				    int *request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps, MAPS_SERVICE_GEOCODE))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!address || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_geocode(maps, address, preference,
			callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

EXPORT_API int maps_service_geocode_inside_area(const maps_service_h maps,
						const char *address,
						const maps_area_h bounds,
						const maps_preference_h preference,
						maps_service_geocode_cb callback,
						void *user_data, int *request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps,
			MAPS_SERVICE_GEOCODE_INSIDE_AREA))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!address || !bounds || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_geocode_inside_bounds(maps,
			address, bounds, preference, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

EXPORT_API int maps_service_geocode_by_structured_address(const maps_service_h maps,
					const maps_address_h address,
					const maps_preference_h preference,
					maps_service_geocode_cb callback,
					void *user_data, int *request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps,
			MAPS_SERVICE_GEOCODE_BY_STRUCTURED_ADDRESS))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!address || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_geocode_by_structured_address(maps,
			address, preference, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

EXPORT_API int maps_service_reverse_geocode(const maps_service_h maps,
					    double latitude, double longitude,
					    const maps_preference_h preference,
					    maps_service_reverse_geocode_cb
					    callback, void * user_data,
					    int *request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps, MAPS_SERVICE_REVERSE_GEOCODE))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (latitude <= -90 || latitude >= 90)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (longitude <= -180 || longitude >= 180)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_reverse_geocode(maps, latitude,
			longitude, preference, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

/*----------------------------------------------------------------------------*/
/* */
/* Place Service */

EXPORT_API int maps_service_search_place(const maps_service_h maps,
					 const maps_coordinates_h position,
					 int distance,
					 const maps_place_filter_h filter,
					 maps_preference_h preference,
					 maps_service_search_place_cb callback,
					 void *user_data, int *request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps, MAPS_SERVICE_SEARCH_PLACE))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!position || (distance < 0) || !filter || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_search_place(maps, position,
			distance, preference, filter, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

EXPORT_API int maps_service_search_place_by_area(const maps_service_h maps,
						 const maps_area_h boundary,
						 const maps_place_filter_h filter,
						 maps_preference_h preference,
						 maps_service_search_place_cb callback,
						 void *user_data,
						 int *request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps,
			MAPS_SERVICE_SEARCH_PLACE_BY_AREA))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!boundary || !filter || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_search_by_area_place(maps,
			boundary, preference, filter, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

EXPORT_API int maps_service_search_place_by_address(const maps_service_h maps,
							const char *address,
							const maps_area_h boundary,
							const maps_place_filter_h filter,
							maps_preference_h preference,
							maps_service_search_place_cb callback,
							void *user_data,
							int *request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps,
			MAPS_SERVICE_SEARCH_PLACE_BY_ADDRESS))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!address || !boundary || !filter || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_search_by_address_place(maps,
			address, boundary, preference, filter, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

EXPORT_API int maps_service_search_place_list(const maps_service_h maps,
					const maps_area_h boundary,
					const maps_place_filter_h filter,
					maps_preference_h preference,
					maps_service_search_place_list_cb callback,
					void *user_data, int *request_id)
{
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	if (!__maps_provider_supported(maps, MAPS_SERVICE_SEARCH_PLACE_LIST))
		return MAPS_ERROR_NOT_SUPPORTED;

	if (!boundary || !filter || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_search_place_list(maps,
			boundary, preference, filter, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

EXPORT_API int maps_service_get_place_details(const maps_service_h maps,
	const char *url, maps_service_get_place_details_cb callback,
	void *user_data, int *request_id)
{
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	if (!__maps_provider_supported(maps, MAPS_SERVICE_SEARCH_PLACE_LIST))
		return MAPS_ERROR_NOT_SUPPORTED;

	if (!url || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_get_place_details(maps,
			url, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

/*----------------------------------------------------------------------------*/
/* */
/* Route Service */

EXPORT_API int maps_service_search_route(const maps_service_h maps,
					 const maps_coordinates_h origin,
					 const maps_coordinates_h destination,
					 maps_preference_h preference,
					 maps_service_search_route_cb callback,
					 void *user_data, int *request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps, MAPS_SERVICE_SEARCH_ROUTE))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!origin || !destination || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_search_route(maps, preference,
			origin, destination, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

EXPORT_API int maps_service_search_route_waypoints(const maps_service_h maps,
						   const maps_coordinates_h *waypoint_list,
						   int waypoint_num,
						   maps_preference_h preference,
						   maps_service_search_route_cb callback,
						   void *user_data,
						   int *request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps,
			MAPS_SERVICE_SEARCH_ROUTE_WAYPOINTS))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (!waypoint_list || (waypoint_num < 2) || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_search_route_waypoints(maps,
			preference, waypoint_list, waypoint_num, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

/*----------------------------------------------------------------------------*/
/* */
/* Cancel Service Request */

EXPORT_API int maps_service_cancel_request(const maps_service_h maps, int request_id)
{
	/* Check if the handle of the Maps Service is valid */
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__maps_provider_supported(maps, MAPS_SERVICE_CANCEL_REQUEST))
		return MAPS_ERROR_NOT_SUPPORTED;

	/* Check if parameters are valid */
	if (request_id < 0)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if privileges enough */
	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_cancel_request(maps, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

/*----------------------------------------------------------------------------*/
/* */
/* Multi geocoder */

EXPORT_API int maps_service_multi_reverse_geocode(const maps_service_h maps,
	const maps_coordinates_list_h coordinates_list, const maps_preference_h preference,
	maps_service_multi_reverse_geocode_cb callback, void *user_data, int *request_id)
{
	if (!maps)
		return MAPS_ERROR_INVALID_PARAMETER;

	if (!__maps_provider_supported(maps, MAPS_SERVICE_MULTI_REVERSE_GEOCODE))
		return MAPS_ERROR_NOT_SUPPORTED;

	if (!coordinates_list || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if internet feature is supported */
	if (!_is_internet_feature_supported())
		return MAPS_ERROR_NOT_SUPPORTED;

	if (!__has_maps_service_privilege())
		return MAPS_ERROR_PERMISSION_DENIED;

	session::command *cmd = new session::command_multi_reverse_geocode(maps,
			coordinates_list, preference, callback, user_data, request_id);

	int ret = (cmd && cmd->plugin()) ? cmd->run() : MAPS_ERROR_INVALID_PARAMETER;
	if (ret) MAPS_LOGE("Failed to run command.(%d)", ret);
	return ret;
}

