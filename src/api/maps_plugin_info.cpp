/* Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "maps_error.h"
#include "maps_plugin_info.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_plugin_info_s
{
	char *provider_name;
} maps_plugin_info_s;

const gsize _MAPS_PLUGIN_INFO_NAME_MAX_LENGTH = 64;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_plugin_info_create(maps_plugin_info_h *info)
{
	if (!info)
		return MAPS_ERROR_INVALID_PARAMETER;
	*info = g_slice_new0(maps_plugin_info_s);

	if (*info == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_info_destroy(maps_plugin_info_h info)
{
	if (!info)
		return MAPS_ERROR_INVALID_PARAMETER;
	int error = MAPS_ERROR_NONE;

	maps_plugin_info_s *c = (maps_plugin_info_s *) info;

	if (c->provider_name)
		g_free(c->provider_name);

	g_slice_free(maps_plugin_info_s, info);
	return error;
}

EXPORT_API int maps_plugin_info_clone(const maps_plugin_info_h origin,
				      maps_plugin_info_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;

	do {
		error = maps_plugin_info_create(cloned);
		if (error != MAPS_ERROR_NONE)
			break;

		maps_plugin_info_s *c = (maps_plugin_info_s *) origin;

		if (c->provider_name) {
			error = maps_plugin_info_set_provider_name(*cloned, c->provider_name);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	maps_plugin_info_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_plugin_info_get_provider_name(const maps_plugin_info_h info,
						  char **provider_name)
{
	if (!info || !provider_name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_plugin_info_s *) info)->provider_name,
		_MAPS_PLUGIN_INFO_NAME_MAX_LENGTH, provider_name);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_plugin_info_set_provider_name(maps_plugin_info_h info,
						  const char *provider_name)
{
	if (!info || !provider_name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(provider_name, _MAPS_PLUGIN_INFO_NAME_MAX_LENGTH,
		&((maps_plugin_info_s *) info)->provider_name);
}
