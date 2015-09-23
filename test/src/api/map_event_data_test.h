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

#ifndef __MAP_EVENT_DATA_API_TEST_H__
#define __MAP_EVENT_DATA_API_TEST_H__


/*int map_event_data_destroy(map_event_data_h event);*/
void utc_map_event_data_create_p(void);
void utc_map_event_data_create_n(void);

/*int map_event_data_clone(const map_event_data_h origin,
			      map_event_data_h *cloned);*/
void utc_map_event_data_clone_p(void);
void utc_map_event_data_clone_n(void);

/*int map_event_data_get_type(const map_event_data_h event,
				 map_event_type_e *event_type);*/
void utc_map_event_data_get_type_p(void);
void utc_map_event_data_get_type_n(void);


/*int map_event_data_get_gesture_type(map_event_data_h event,
					 map_gesture_e *gesture_type);*/
void utc_map_event_data_get_gesture_type_p(void);
void utc_map_event_data_get_gesture_type_n(void);

/* int map_event_data_get_action_type(map_event_data_h event,
					map_action_e *action_type);*/
void utc_map_event_data_get_action_type_p(void);
void utc_map_event_data_get_action_type_n(void);

/*int map_event_data_get_center(map_event_data_h event,
				   maps_coordinates_h *center);*/
void utc_map_event_data_get_center_p(void);
void utc_map_event_data_get_center_n(void);

/*int map_event_data_get_delta(map_event_data_h event,
				  int *delta_x, int *delta_y);*/
void utc_map_event_data_get_delta_p(void);
void utc_map_event_data_get_delta_n(void);

/*int map_event_data_get_xy(map_event_data_h event, int *x, int* y);*/
void utc_map_event_data_get_xy_p(void);
void utc_map_event_data_get_xy_n(void);

/*int map_event_data_get_fingers(map_event_data_h event, int *fingers);*/
void utc_map_event_data_get_fingers_p(void);
void utc_map_event_data_get_fingers_n(void);

/*int map_event_data_get_zoom_factor(map_event_data_h event,
					double *zoom_factor);*/
void utc_map_event_data_get_zoom_factor_p(void);
void utc_map_event_data_get_zoom_factor_n(void);

/*int map_event_data_get_rotation_angle(map_event_data_h event,
					   double *rotation_angle);*/
void utc_map_event_data_get_rotation_angle_p(void);
void utc_map_event_data_get_rotation_angle_n(void);

/*int map_event_data_get_object(map_event_data_h event,
				   map_object_h *object);*/
void utc_map_event_data_get_object_p(void);
void utc_map_event_data_get_object_n(void);

#endif /* __MAP_EVENT_DATA_API_TEST_H__*/

