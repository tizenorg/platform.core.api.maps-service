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

#include "discovery.h"
#include "module.h"
#include "maps_util.h"
#include <algorithm>
#include <glib.h>


const char *MAPS_PLUGINS_PATH_PREFIX = "/usr/lib/maps/plugins";

plugin::provider_info plugin::provider_info::empty_instance;

plugin::provider_info::provider_info()
{
}

plugin::provider_info::provider_info(const string &p, const string &f)
: provider(p)
, file(f)
{
}

plugin::provider_info::provider_info(const provider_info &src)
{
	*this = src;
}

plugin::provider_info &plugin::provider_info::operator=(const
							provider_info &src)
{
	if (this != &src) {
		provider = src.provider;
		file = src.file;
	}
	return* this;
}

void plugin::provider_info::construct(const string &p, const string &f)
{
	provider = p;
	file = f;
}

bool plugin::provider_info::empty() const
{
	if (!provider.empty())
		return false;
	if (!file.empty())
		return false;
	return true;
}

plugin::discovery::discovery()
{
}

plugin::discovery::~discovery()
{
}

vector<plugin::provider_info> plugin::discovery::get_available_list() const
{
	vector <provider_info> v;
	binary_extractor be;

	/* Enumerate all available plugin binaries */
	const vector <string> l = get_module_file_list();
	for (int i = 0; i < int(l.size()); i++) {
		/* Request the next plugin binary to get provider info */
		const provider_info info = be.get_plugin_info(l[i]);
		if (info.empty())
			continue;

		/* Add provider info to the resulting list */
		v.push_back(info);
	}

	return v;
}

vector<string> plugin::discovery::get_module_file_list() const
{
	vector<string> l;

	GDir *dir = NULL;
	GError *error = NULL;
	const gchar *filename = NULL;

	MAPS_LOGD("START PLUGIN FILES DISCOVERY:");
	dir = g_dir_open(MAPS_PLUGINS_PATH_PREFIX, 0, &error);
	if (dir) {

		GPatternSpec *plugin_name_pattern = g_pattern_spec_new("*?.so");

		while ((filename = g_dir_read_name(dir))) {
			MAPS_LOGD("found plugin binary: %s", filename);
			if (g_pattern_match_string(plugin_name_pattern,
					filename)) {
				MAPS_LOGD("\tadded plugin binary: %s",
					filename);
				l.push_back(string(filename));
			}
		}

		g_pattern_spec_free(plugin_name_pattern);

		g_dir_close(dir);
	}
	else if (error) {
		MAPS_LOGE("%d: Can not open directory: %s\n", error->code,
			  error->message);	/* g_error */
	}

	return l;
}

plugin::provider_info plugin::find_by_names(const string &provider)
{
	const vector<provider_info> v = discovery().get_available_list();
	for (int i = 0; i < int (v.size()); i++)
		if (v[i].provider == provider)
			return v[i];

	return provider_info::empty_instance;
}
