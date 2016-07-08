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

#include "maps_util.h"
#include "maps_error.h"
#include <glib.h>
#include <system_info.h>

#define DEFAULT_UNKNOWN_DPI 132

int maps_set_string(const char *src, const int max_length, char **dst)
{
	if (!dst || !src || (max_length <= 0))
		return MAPS_ERROR_INVALID_PARAMETER;
	if (*dst)
		g_free(*dst);
	*dst = g_strndup(src, max_length);
	return MAPS_ERROR_NONE;
}

int maps_get_string(const char *src, const int max_length, char **dst)
{
	if (!dst || !src || (max_length <= 0))
		return MAPS_ERROR_INVALID_PARAMETER;
	*dst = g_strndup(src, max_length);
	return MAPS_ERROR_NONE;
}

int maps_get_display_dpi()
{
    int dpi = DEFAULT_UNKNOWN_DPI;
    system_info_get_platform_int("http://tizen.org/feature/screen.dpi", &dpi);
    return dpi;
}
