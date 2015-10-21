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

#include <glib/gstdio.h>
#include "module.h"
#include "maps_util.h"
#include "thread.h"
#include "command.h"
#include "command_queue.h"
#include "empty_module.h"

extern const char *MAPS_PLUGINS_PATH_PREFIX;

 plugin::scope_mutex::scope_mutex(GMutex *m) : mutex(m)
{
	g_mutex_lock(mutex);
}

plugin::scope_mutex::~scope_mutex()
{
	g_mutex_unlock(mutex);
}

plugin::binary_extractor::binary_extractor()
{
}

plugin::provider_info plugin::binary_extractor::get_plugin_info(const
							string &file_name) const
{

	if (file_name.empty())
		return provider_info::empty_instance;

	/* 1.Init plugin */
	GMod *plugin = gmod_new(file_name, FALSE);
	if (!plugin)
		return provider_info::empty_instance;

	provider_info info;

	/* 2. Get plugin info */
	maps_plugin_get_info_f func =
		(maps_plugin_get_info_f) gmod_find_sym(plugin,
		"maps_plugin_get_info");
	if (func) {
		maps_plugin_info_h i;
		if (func(&i) == MAPS_ERROR_NONE) {
			char *provider_name = NULL;
			maps_plugin_info_get_provider_name(i, &provider_name);

			/* Convert plugin info to provider info */
			info.construct(string(provider_name), file_name);

			g_free(provider_name);
		}
		maps_plugin_info_destroy(i);
	}

	/* 3. shutdown plugin */
	gmod_free(plugin);

	return info;
}

maps_plugin_h plugin::binary_extractor::init(const provider_info &info)
{
	/* 1.Init plugin */
	if (info.file.empty())
		return NULL;

	GMod *plugin = gmod_new(info.file, TRUE);
	if (!plugin) {
		MAPS_LOGE("Open Module Failed: %s", info.file.c_str());
		return NULL;
	}

	/* 2.1 Create new plugin interface */
	plugin_s* new_plugin = g_slice_new0(plugin_s);

	/* 2. Perform steps to completely initialize a plugin */
	do {

		if (!new_plugin) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				MAPS_ERROR_OUT_OF_MEMORY);
			break;
		}

		/* 2.1 Set plugin module handle */
		new_plugin->module = plugin;

		/* 2.2 Set plugin interface */
		new_plugin->interface = get_empty_interface();

		/* Plugin dedicated functions */
		new_plugin->interface.maps_plugin_init =
			(maps_plugin_init_f) gmod_find_sym(plugin,
			"maps_plugin_init");
		new_plugin->interface.maps_plugin_shutdown =
			(maps_plugin_shutdown_f) gmod_find_sym(plugin,
			"maps_plugin_shutdown");
		new_plugin->interface.maps_plugin_get_info =
			(maps_plugin_get_info_f) gmod_find_sym(plugin,
			"maps_plugin_get_info");

		/* Maps Provider access key, preference and capabilities */
		new_plugin->interface.maps_plugin_set_provider_key =
			(maps_plugin_set_provider_key_f) gmod_find_sym(plugin,
			"maps_plugin_set_provider_key");
		new_plugin->interface.maps_plugin_get_provider_key =
			(maps_plugin_get_provider_key_f) gmod_find_sym(plugin,
			"maps_plugin_get_provider_key");
		new_plugin->interface.maps_plugin_set_preference =
			(maps_plugin_set_preference_f) gmod_find_sym(plugin,
			"maps_plugin_set_preference");
		new_plugin->interface.maps_plugin_get_preference =
			(maps_plugin_get_preference_f) gmod_find_sym(plugin,
			"maps_plugin_get_preference");
		new_plugin->interface.maps_plugin_is_service_supported =
			(maps_plugin_is_service_supported_f)
			gmod_find_sym(plugin,
			"maps_plugin_is_service_supported");
		new_plugin->interface.maps_plugin_is_data_supported =
			(maps_plugin_is_data_supported_f) gmod_find_sym(plugin,
			"maps_plugin_is_data_supported");

		/* Geocode */
		new_plugin->interface.maps_plugin_geocode =
			(maps_plugin_geocode_f) gmod_find_sym(plugin,
			"maps_plugin_geocode");
		new_plugin->interface.maps_plugin_geocode_inside_area =
			(maps_plugin_geocode_inside_area_f)
			gmod_find_sym(plugin,
			"maps_plugin_geocode_inside_area");
		new_plugin->interface.
			maps_plugin_geocode_by_structured_address =
			(maps_plugin_geocode_by_structured_address_f)
			gmod_find_sym(plugin,
			"maps_plugin_geocode_by_structured_address");
		new_plugin->interface.maps_plugin_reverse_geocode =
			(maps_plugin_reverse_geocode_f) gmod_find_sym(plugin,
			"maps_plugin_reverse_geocode");

		/* Place */
		new_plugin->interface.maps_plugin_search_place =
			(maps_plugin_search_place_f) gmod_find_sym(plugin,
			"maps_plugin_search_place");
		new_plugin->interface.maps_plugin_search_place_by_area =
			(maps_plugin_search_place_by_area_f)
			gmod_find_sym(plugin,
			"maps_plugin_search_place_by_area");
		new_plugin->interface.maps_plugin_search_place_by_address =
			(maps_plugin_search_place_by_address_f)
			gmod_find_sym(plugin,
			"maps_plugin_search_place_by_address");

		/* Route */
		new_plugin->interface.maps_plugin_search_route =
			(maps_plugin_search_route_f) gmod_find_sym(plugin,
			"maps_plugin_search_route");
		new_plugin->interface.maps_plugin_search_route_waypoints =
			(maps_plugin_search_route_waypoints_f)
			gmod_find_sym(plugin,
			"maps_plugin_search_route_waypoints");

		/* Cancel Request */
		new_plugin->interface.maps_plugin_cancel_request =
			(maps_plugin_cancel_request_f) gmod_find_sym(plugin,
			"maps_plugin_cancel_request");

		/* 2.3 Check whether the plugin init function is valid */
		if (!new_plugin->interface.maps_plugin_init) {
			MAPS_LOGE(
			"ERROR! Plugin initialization function is invalid");
			break;
		}

		/* 2.4 Call a plugin to initialize itself, send to the plugin
		*  its pointer */
		int ret =
			new_plugin->interface.
			maps_plugin_init((maps_plugin_h *) (&new_plugin));
		if (ret != MAPS_ERROR_NONE) {
			MAPS_LOGE(
			"ERROR! Plugin initialization function ""failed: %d",
				ret);
			break;
		}

		if (!new_plugin->interface.maps_plugin_set_provider_key) {
			MAPS_LOGE(
			"ERROR! Plugin set_provider_key function is NULL: %d",
				ret);
			break;
		}

		if (!new_plugin->interface.maps_plugin_get_provider_key) {
			MAPS_LOGE(
			"ERROR! Plugin set_provider_key function is NULL: %d",
				ret);
			break;
		}

		if (!new_plugin->interface.maps_plugin_set_preference) {
			MAPS_LOGE(
			"ERROR! Plugin set_preference function is NULL: %d",
				ret);
			break;
		}

		if (!new_plugin->interface.maps_plugin_get_preference) {
			MAPS_LOGE(
			"ERROR! Plugin get_preference function is NULL: %d",
				ret);
			break;
		}

		if (!new_plugin->interface.maps_plugin_is_data_supported) {
			MAPS_LOGE(
				"ERROR! Plugin support_is_data_supported function is NULL: %d",
				ret);
			break;
		}

		if (!new_plugin->interface.maps_plugin_is_service_supported) {
			MAPS_LOGE(
				"ERROR! Plugin support_is_service_supported function is NULL: %d",
				ret);
			break;
		}

		/* 2.7 Create a queue with asynchronous requests to plugin */
		if (session::command_queue::is_async())
			new_plugin->request_queue = g_async_queue_new();

		/* 2.8 Initialize the mutex for the map of pending requests */
		new_plugin->pending_request_maps =
			g_hash_table_new_full(g_int_hash, g_int_equal, g_free,
			session::command_handler::destroy);
		if (!new_plugin->pending_request_maps) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				MAPS_ERROR_OUT_OF_MEMORY);
			break;
		}
		g_mutex_init(&new_plugin->pending_request_mutex);

		/* DEBUG TRACE */
		trace_dbg(new_plugin);

		/* 2.5 Return newly initialized plugin */
		return new_plugin;

	} while (FALSE);

	MAPS_LOGE("Shut down the plugin becuause of error");

	/* 3. shutdown plugin in case of problem */
	shutdown(new_plugin);
	return NULL;
}

void plugin::binary_extractor::shutdown(maps_plugin_h plugin_h)
{
	if (!plugin_h)
		return;

	plugin_s *plugin = (plugin_s *) plugin_h;
	g_return_if_fail(plugin->module);

	/* 0. shutdown plugin */
	if (plugin->interface.maps_plugin_shutdown)
		plugin->interface.maps_plugin_shutdown(plugin);

	/* 1. Stop the thread, processing the request queue */
	session::thread().stop(plugin);

	/* 2. Destroy the request queue */
	if (plugin->request_queue)
		g_async_queue_unref(plugin->request_queue);

	/* 3. Destroy the map of pending requests */
	if (plugin->pending_request_maps) {
		g_hash_table_unref(plugin->pending_request_maps);

		/* Clear the mutex for the map of pending requests */
		g_mutex_clear(&plugin->pending_request_mutex);
	}

	/* 4. Unload plugin from memory */
	gmod_free((GMod *) plugin->module);

	/* 5. Destroying the table with plugin capabilities */
	/*maps_string_hashtable_destroy(plugin->capabilities); */

	/* 6. Release memory used by plugin structure */
	g_slice_free(plugin_s, plugin);
}

/* Open the binary (which contains a plugin) */
plugin::GMod *plugin::binary_extractor::gmod_new(const string &module_file,
						 gboolean is_resident) const
{

	if (!g_module_supported()) {
		MAPS_LOGE("ERROR! g_module_supported is false\n\n");
		return NULL;
	}

	if (module_file.empty())
		return NULL;

	GMod *gmod = g_new0(GMod, 1);

	gmod->name = g_strdup(module_file.c_str());
	if (!gmod->name) {
		g_free(gmod);
		return NULL;
	}

	/*gmod->path = g_module_build_path(MAPS_PLUGINS_PATH_PREFIX,
	* gmod->name); */
	gmod->path = g_strnfill(100, 0);
	g_sprintf(gmod->path, "%s/%s", MAPS_PLUGINS_PATH_PREFIX, gmod->name);
	if (!gmod->path) {
		g_free(gmod->name);
		g_free(gmod);
		return NULL;
	}

	gmod->module = g_module_open(gmod->path, G_MODULE_BIND_LAZY);
	if (!gmod->module) {
		MAPS_LOGE("module path not found: %s", gmod->path);

		const gchar *last_error = g_module_error();
		MAPS_LOGE("last module error: %s", last_error);

		g_free(gmod->name);
		g_free(gmod->path);
		g_free(gmod);
		return NULL;
	}
	MAPS_LOGD("open module");
	/*if (is_resident)
		g_module_make_resident(gmod->module);*/

	return gmod;
}

/* Close the binary (which contains a plugin) */
void plugin::binary_extractor::gmod_free(GMod *gmod) const
{
	/*g_return_if_fail(gmod); */
	if (!gmod)
		return;

	if (gmod->name)
		g_free(gmod->name);
	if (gmod->path)
		g_free(gmod->path);
	if (gmod->module)
		g_module_close(gmod->module);
	g_free(gmod);

	MAPS_LOGD("close module");
	MAPS_LOGD("last module error: %s", g_module_error());
}

/* Find the address of a function in a binary (which contains a plugin) */
gpointer plugin::binary_extractor::gmod_find_sym(GMod *gmod,
						 const gchar *func_name) const
{
	g_return_val_if_fail(gmod, NULL);
	g_return_val_if_fail(func_name, NULL);

	gpointer func_ptr = NULL;
	if (!g_module_symbol(gmod->module, func_name, &func_ptr)) {
		MAPS_LOGE("function symbol not found");
		MAPS_LOGE("%s", g_module_error());
		func_ptr = NULL;
	}
	return func_ptr;
}

void plugin::binary_extractor::trace_dbg(const plugin_s *plugin) const
{

	MAPS_LOGD("*********************************************");
	MAPS_LOGD("PLUGIN INFO");
	if (!plugin) {
		MAPS_LOGD("PLUGIN is NULL");
		MAPS_LOGD("*********************************************");
		return;
	}

	const GMod *mod = (const GMod *) plugin->module;
	if (!mod) {
		MAPS_LOGD("PLUGIN module is NULL");
	}
	else {
		MAPS_LOGD("module address:\t\t\t%p", mod->module);
		MAPS_LOGD("module name:\t\t\t%s", mod->name);
		MAPS_LOGD("module path:\t\t\t%s", mod->path);
	}

	if (!plugin->request_queue) {
		MAPS_LOGD("PLUGIN request queue is NULL");
	}
	else {
		MAPS_LOGD("plugin request queue:\t\t\t%p",
			plugin->request_queue);
	}

	const interface_s *itf = &plugin->interface;
	MAPS_LOGD("maps_plugin_init:\t\t\t%p", itf->maps_plugin_init);
	MAPS_LOGD("maps_plugin_shutdown:\t\t\t%p", itf->maps_plugin_shutdown);
	MAPS_LOGD("maps_plugin_get_info:\t\t\t%p", itf->maps_plugin_get_info);

	/* Maps Provider access key */
	MAPS_LOGD("maps_plugin_set_provider_key:\t\t%p",
		itf->maps_plugin_set_provider_key);
	MAPS_LOGD("maps_plugin_get_provider_key:\t\t%p",
		itf->maps_plugin_get_provider_key);
	MAPS_LOGD("maps_plugin_is_service_supported:\t%p",
		itf->maps_plugin_is_service_supported);
	MAPS_LOGD("maps_plugin_is_data_supported:\t\t%p",
		itf->maps_plugin_is_data_supported);

	MAPS_LOGD("maps_plugin_geocode:\t\t\t%p", itf->maps_plugin_geocode);
	MAPS_LOGD("maps_plugin_geocode_inside_area:\t%p",
		itf->maps_plugin_geocode_inside_area);
	MAPS_LOGD("maps_plugin_geocode_by_structured_address: %p",
		itf->maps_plugin_geocode_by_structured_address);
	MAPS_LOGD("maps_plugin_reverse_geocode:\t\t%p",
		itf->maps_plugin_reverse_geocode);

	MAPS_LOGD("maps_plugin_search_place:\t\t%p",
		itf->maps_plugin_search_place);
	MAPS_LOGD("maps_plugin_search_place_by_area:\t%p",
		itf->maps_plugin_search_place_by_area);
	MAPS_LOGD("maps_plugin_search_place_by_address:\t%p",
		itf->maps_plugin_search_place_by_address);

	MAPS_LOGD("maps_plugin_search_route:\t\t%p",
		itf->maps_plugin_search_route);
	MAPS_LOGD("maps_plugin_search_route_waypoints:\t%p",
		itf->maps_plugin_search_route_waypoints);

	MAPS_LOGD("maps_plugin_cancel_request:\t\t%p",
		itf->maps_plugin_cancel_request);
	MAPS_LOGD("*********************************************");
}
