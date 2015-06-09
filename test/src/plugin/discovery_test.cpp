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

#include "discovery_test.h"
#include "discovery.h"
#include <glib.h>

void utc_provider_info_construct_p(void)
{
	plugin::provider_info info;
	info.construct("b", "c");
	g_assert(info.provider == "b");
	g_assert(info.file == "c");
}

/*void utc_operator_maps_provider_info_h_p(void) {
	plugin::provider_info info("b", "c");

	maps_provider_info_h h = info;
	g_assert(h);

	maps_provider_info_s* i = (maps_provider_info_s*)h;

	g_assert_cmpstr(i->provider, ==, "b");
	g_assert_cmpstr(i->file, ==, "c");

	g_slice_free(maps_provider_info_s, i);
}*/

void utc_provider_info_empty_p(void)
{
	plugin::provider_info info;
	g_assert(info.empty());
}

void utc_provider_info_empty_n_01(void)
{
	plugin::provider_info info;
	info.construct("b", "c");
	g_assert(!info.empty());
}

void utc_provider_info_empty_n_02(void)
{
	plugin::provider_info info;
	info.construct("", "c");
	g_assert(!info.empty());
}

void utc_provider_info_empty_n_03(void)
{
	plugin::provider_info info;
	info.construct("b", "");
	g_assert(!info.empty());
}

/*----------------------------------------------------------------------------*/

void utc_get_available_plugin_list(void)
{

	plugin::discovery d;

	vector < plugin::provider_info > v = d.get_available_list();

	g_assert_cmpint(int (v.size()), >=, 1);

	/*for(vector<plugin::provider_info>::iterator it = v.begin();
	* it != v.end(); ++it) { */
	for (unsigned int i = 0; i < v.size(); i++) {

		/*const plugin::provider_info pi =* it; */
		const plugin::provider_info pi = v[i];
		g_print("\nFound a \tprovider: %s\n\t\tbinary file: %s\n",
			pi.provider.c_str(), pi.file.c_str());

		if (pi.provider == "Maps Provider") {
			g_assert(pi.file == "libmaps-plugin-test.so");
		}
	}

}

