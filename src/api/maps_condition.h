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

#ifndef __MAPS_SERVICE_CHECK_H__
#define __MAPS_SERVICE_CHECK_H__

/**
 * @brief	Gets if privilege is consented
 */
bool maps_condition_check_privilege(void);

/**
 * @brief	Gets if network.internet feature is supported
 * @remarks	If target is not a wearable gear, it returns true always.
 */
bool maps_condition_check_feature(void);

#endif	/* __MAPS_SERVICE_CHECK_H__ */
