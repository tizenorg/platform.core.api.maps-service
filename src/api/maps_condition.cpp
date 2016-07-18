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

#include <glib.h>
#include <unistd.h>
#include <system_info.h>		/* system_info_get_platform_bool */
#include <maps_util.h>			/* log */
#include <maps_error.h>
#if (TIZEN_VER < VERSION(3, 0, 0))
#include <privacy_checker_client.h>
#include <sys/smack.h>

#define MAPSERVICE_PRIVILEGE	"privilege::tizen::mapservice"
#define MAPSERVICE_PERMISSION	"rw"
#endif
#define MAPS_FEATURE			"http://tizen.org/feature/maps"
#define INTERNET_FEATURE		"http://tizen.org/feature/network.internet"


#if (TIZEN_VER < VERSION(3, 0, 0))
static int __maps_condition_check_privilege(const char *privilege_name, const char *permission)
{
	if (!privilege_name || !permission)
		return MAPS_ERROR_INVALID_PARAMETER;

	int ret;
	char *label = NULL;

	if (smack_new_label_from_self(&label) == -1) {
		MAPS_LOGE("smack_new_label_from_self error");
		return MAPS_ERROR_INVALID_OPERATION;
	}
	ret = smack_have_access(label, privilege_name, permission);
	if (ret == 1) {
		ret = MAPS_ERROR_NONE;
	} else if (ret == 0) {
		ret = MAPS_ERROR_PERMISSION_DENIED;
	} else {
		ret = MAPS_ERROR_INVALID_OPERATION;
	}
	g_free(label);
	return ret;
}
#endif

bool maps_condition_check_privilege(void)
{
	static bool is_permitted = true;
	static bool is_read = false;

	if (!is_read) {
#if (TIZEN_VER < VERSION(3, 0, 0))
		int ret = __maps_condition_check_privilege(MAPSERVICE_PRIVILEGE, MAPSERVICE_PERMISSION);
		is_permitted = (ret == MAPS_ERROR_NONE);
#else
		/* to check for Tizen 3.x privilege */
		extern const char *MAPS_PLUGINS_PATH_PREFIX;
		is_permitted = (access(MAPS_PLUGINS_PATH_PREFIX, F_OK) != 0) || /* not exist */
		               (access(MAPS_PLUGINS_PATH_PREFIX, R_OK) == 0);   /* readable */
#endif
		is_read = true;
		MAPS_LOGD("mapservice privilege is%sconsented", (is_permitted ? " " : " not "));
	}
	return is_permitted;
}

bool maps_condition_check_maps_feature(void)
{
	static bool is_supported = true;
	static bool is_read = false;

	if (!is_read) {
		int ret = system_info_get_platform_bool(MAPS_FEATURE, &is_supported);
		if (ret == SYSTEM_INFO_ERROR_NONE) {
			MAPS_LOGD("maps feature is%ssupported", (is_supported ? " " : " not "));
			is_read = true;
		}
	}
	return is_supported;
}

bool maps_condition_check_internet_feature(void)
{
	static bool is_supported = true;
#ifdef TIZEN_WEARABLE
	static bool is_read = false;

	if (!is_read) {
		int ret = system_info_get_platform_bool(INTERNET_FEATURE, &is_supported);
		if (ret == SYSTEM_INFO_ERROR_NONE) {
			MAPS_LOGD("internet feature is%ssupported", (is_supported ? " " : " not "));
			is_read = true;
		}
	}
#endif
	return is_supported;
}
