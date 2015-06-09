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

#include "module_test.h"
#include "module.h"
#include <glib.h>

void utc_binary_extractor_get_plugin_info_p(void)
{
	plugin::binary_extractor e;

	/*get_plugin_info("libmaps_plugin_test.so"); */
}

void utc_binary_extractor_get_plugin_info_n(void)
{

}

void utc_binary_extractor_init_plugin_p(void)
{
	const plugin::provider_info info =
		plugin::find_by_names("Maps Provider");
	if (info.empty())
		return;

	plugin::plugin_s* plg =
		(plugin::plugin_s*) plugin::binary_extractor().init(info);
	g_assert(plg);

	if (plg)
		plugin::binary_extractor().shutdown(plg);
}

void utc_binary_extractor_init_plugin_n(void)
{

}

