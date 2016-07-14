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
#if TIZEN_VER < 240
#include <app_manager.h>
#include <pkgmgr-info.h>
#include <privilege_checker.h>	/* privilege */
#elif TIZEN_VER < 300
#include <privilege_checker.h>	/* privilege */
#endif
#include <system_info.h>		/* system_info_get_platform_bool */
#include <maps_util.h>			/* log */
#include <maps_error.h>


#define MAPSERVICE_PRIVILEGE	"http://tizen.org/privilege/mapservice"

#define INTERNET_FEATURE		"http://tizen.org/feature/network.internet"


#if TIZEN_VER < 240
static char* __maps_condition_get_package_id(char **package_id)
{
	int ret = 0;
	pid_t pid = 0;
	char *app_id = NULL;
	char *pkg_id = NULL;
	pkgmgrinfo_appinfo_h pkgmgrinfo_appinfo;

	do {
		pid = getpid();
		ret = app_manager_get_app_id(pid, &app_id);
		if (ret) {
			MAPS_LOGE("Fail to get app_id. Err[%d]", ret);
			break;
		}

		ret = pkgmgrinfo_appinfo_get_appinfo(app_id, &pkgmgrinfo_appinfo);
		if (ret) {
			MAPS_LOGE("Fail to get appinfo for [%s]. Err[%d]", app_id, ret);
			break;
		}

		ret = pkgmgrinfo_appinfo_get_pkgname(pkgmgrinfo_appinfo, &pkg_id);
		if (ret) {
			MAPS_LOGE("Fail to get package_id for [%s]. Err[%d]", app_id, ret);
			break;
		}

		*package_id = g_strdup(pkg_id);
	} while (0);

	g_free(app_id);
	pkgmgrinfo_appinfo_destroy_appinfo(pkgmgrinfo_appinfo);

	return *package_id;
}
#endif

bool maps_condition_check_privilege(void)
{
	bool is_permitted = true;
#if TIZEN_VER < 240
	/* to check for Tizen 2.x privilege */
	char *package_id = NULL;
	if (!__maps_condition_get_package_id(&package_id))
		return is_permitted;

	int ret = privilege_checker_check_package_privilege(package_id, MAPSERVICE_PRIVILEGE);
	is_permitted = (ret == PRIV_CHECKER_ERR_NONE);
	g_free(package_id);
#elif TIZEN_VER < 300
	int ret = privilege_checker_check_privilege(MAPSERVICE_PRIVILEGE);
	is_permitted = (ret == PRIVILEGE_CHECKER_ERR_NONE);
#else
	/* to check for Tizen 3.x privilege */
	extern const char *MAPS_PLUGINS_PATH_PREFIX;
	is_permitted = (access(MAPS_PLUGINS_PATH_PREFIX, F_OK) != 0) || /* not exist */
	               (access(MAPS_PLUGINS_PATH_PREFIX, R_OK) == 0);   /* readable */
#endif
	MAPS_LOGD("mapservice privilege is%sconsented", ( is_permitted ? " " : " not "));
	return is_permitted;
}

bool maps_condition_check_feature(void)
{
	bool is_supported = true;
#ifdef TIZEN_WEARABLE
	int ret = system_info_get_platform_bool(INTERNET_FEATURE, &is_supported);
	if (ret == SYSTEM_INFO_ERROR_NONE)
		MAPS_LOGD("internet feature is%ssupported", (is_supported ? " " : " not "));
	else
		MAPS_LOGD("system info error (%d)", ret);
#endif
	return is_supported;
}
