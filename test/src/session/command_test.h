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

#ifndef __MAPS_SERVICE_TEST_SESSION_COMMAND_H__
#define __MAPS_SERVICE_TEST_SESSION_COMMAND_H__

void utc_command_empty_ptr_p(void);

void utc_use_hash_table_p(void);

/* bool add(const int user_req_id); */
void utc_pending_request_add_p(void);
void utc_pending_request_add_n(void);

/* void update(int user_req_id, int plg_req_id); */
void utc_pending_request_update_p(void);
void utc_pending_request_update_n(void);

/* void remove(int user_req_id); */
void utc_pending_request_remove_p(void);
void utc_pending_request_remove_n(void);

/* int look_up(const int user_req_id); */
void utc_pending_request_look_up_p(void);
void utc_pending_request_look_up_n(void);

/* int extract_plg_id(const int user_req_id); */
void utc_pending_request_extract_p(void);
void utc_pending_request_extract_n(void);

/* bool contains(const int user_req_id); */
void utc_pending_request_contains_p(void);
void utc_pending_request_contains_n(void);

#endif /* __MAPS_SERVICE_TEST_SESSION_COMMAND_H__ */

