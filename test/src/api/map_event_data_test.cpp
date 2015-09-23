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

#include "map_event_data_test.h"
#include "map_event_data.h"
#include "maps_error.h"
#include <glib.h>
#include "map_view.h"


extern int _map_event_data_create(map_event_data_h *event);

extern int _map_event_data_set_type(map_event_data_h event,
			     map_event_type_e event_type);

extern int _map_event_data_set_gesture_type(map_event_data_h event,
				     const map_gesture_e gesture_type);

extern int _map_event_data_set_action_type(map_event_data_h event,
				    const map_action_e action_type);

extern int _map_event_data_set_center(map_event_data_h event,
			       const maps_coordinates_h center);

extern int _map_event_data_set_delta(map_event_data_h event,
			      const int delta_x, const int delta_y);

extern int _map_event_data_set_xy(map_event_data_h event,
			   const int x, const int y);

extern int _map_event_data_set_fingers(map_event_data_h event,
				const int fingers);

extern int _map_event_data_set_zoom_factor(map_event_data_h event,
				    const double zoom_factor);

extern int _map_event_data_set_rotation_angle(map_event_data_h event,
				       const double rotation_angle);

extern int _map_event_data_set_object(map_event_data_h event,
			       map_object_h object);

extern int _map_object_create(const map_object_type_e type,
			      map_object_h *object);


/*int map_event_data_destroy(map_event_data_h event);*/
void utc_map_event_data_create_p(void)
{
	/* Create a test event data */
	map_event_data_h data = NULL;
	int error = _map_event_data_create(&data);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(data);

	/* Destroy a test view object */
	error = map_event_data_destroy(data);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_event_data_create_n(void)
{
	/* Incorrectly create a test event data */
	int error = _map_event_data_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	map_event_data_h e;
 public:
	test_env() : e(NULL)
	{
		/* Create a test event data */
		const int error = _map_event_data_create(&e);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		g_assert(e);
	}
	~test_env()
	{
		/* Destroy a test event data */
		const int error = map_event_data_destroy(e);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/


/*int map_event_data_clone(const map_event_data_h origin,
			      map_event_data_h *cloned);*/
void utc_map_event_data_clone_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_READY);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	map_event_data_h cloned = NULL;
	error = map_event_data_clone(e.e, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	map_event_type_e type = MAP_EVENT_GESTURE;
	error = map_event_data_get_type(cloned, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(type, ==, MAP_EVENT_READY);

	error = map_event_data_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_event_data_clone_n(void)
{
	test_env e;

	map_event_data_h cloned = NULL;
	int error = map_event_data_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!cloned);

	error = map_event_data_clone(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_event_data_get_type(const map_event_data_h event,
				 map_event_type_e *event_type);*/
void utc_map_event_data_get_type_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_READY);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	map_event_type_e type = MAP_EVENT_GESTURE;
	error = map_event_data_get_type(e.e, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(type, ==, MAP_EVENT_READY);
}

void utc_map_event_data_get_type_n(void)
{
	test_env e;

	map_event_type_e type = MAP_EVENT_GESTURE;
	int error = map_event_data_get_type(NULL, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_type(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_event_data_get_gesture_type(map_event_data_h event,
					 map_gesture_e *gesture_type);*/
void utc_map_event_data_get_gesture_type_p(void)
{
	test_env e;

	int error = _map_event_data_set_gesture_type(e.e, MAP_GESTURE_FLICK);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	map_gesture_e type = MAP_GESTURE_NONE;
	error = map_event_data_get_gesture_type(e.e, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(type, ==, MAP_GESTURE_FLICK);
}

void utc_map_event_data_get_gesture_type_n(void)
{
	test_env e;

	map_gesture_e type = MAP_GESTURE_NONE;
	int error = map_event_data_get_gesture_type(NULL, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_gesture_type(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/* int map_event_data_get_action_type(map_event_data_h event,
					map_action_e *action_type);*/
void utc_map_event_data_get_action_type_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_ACTION);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = _map_event_data_set_action_type(e.e, MAP_ACTION_ZOOM);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	map_action_e type = MAP_ACTION_NONE;
	error = map_event_data_get_action_type(e.e, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(type, ==, MAP_ACTION_ZOOM);
}

void utc_map_event_data_get_action_type_n(void)
{
	test_env e;

	map_action_e type = MAP_ACTION_NONE;
	int error = map_event_data_get_action_type(NULL, &type);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_action_type(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_event_data_get_center(map_event_data_h event,
				   maps_coordinates_h *center);*/
void utc_map_event_data_get_center_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_ACTION);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h coords = NULL;
	error = maps_coordinates_create(25.6, 47.8, &coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = _map_event_data_set_center(e.e, coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_coordinates_destroy(coords);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h c = NULL;
	error = map_event_data_get_center(e.e, &c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(c);

	error = maps_coordinates_destroy(c);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_event_data_get_center_n(void)
{
	test_env e;

	maps_coordinates_h c = NULL;
	int error = map_event_data_get_center(NULL, &c);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!c);

	error = map_event_data_get_center(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_event_data_get_delta(map_event_data_h event,
				  int *delta_x, int *delta_y);*/
void utc_map_event_data_get_delta_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_ACTION);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = _map_event_data_set_delta(e.e, 11, 22);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	int delta_x = 0;
	int delta_y = 0;
	error = map_event_data_get_delta(e.e, &delta_x, &delta_y);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(delta_x , ==, 11);
	g_assert_cmpint(delta_y , ==, 22);
}

void utc_map_event_data_get_delta_n(void)
{
	test_env e;

	int delta_x = 0;
	int delta_y = 0;
	int error = map_event_data_get_delta(NULL, &delta_x, &delta_y);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_delta(e.e, NULL, &delta_y);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_delta(e.e, &delta_x, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_event_data_get_xy(map_event_data_h event, int *x, int* y);*/
void utc_map_event_data_get_xy_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_GESTURE);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = _map_event_data_set_xy(e.e, 11, 22);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	int x = 0;
	int y = 0;
	error = map_event_data_get_xy(e.e, &x, &y);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(x , ==, 11);
	g_assert_cmpint(y , ==, 22);
}

void utc_map_event_data_get_xy_n(void)
{
	test_env e;

	int x = 0;
	int y = 0;
	int error = map_event_data_get_xy(NULL, &x, &y);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_xy(e.e, NULL, &y);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_xy(e.e, &x, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_event_data_get_fingers(map_event_data_h event, int *fingers);*/
void utc_map_event_data_get_fingers_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_GESTURE);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = _map_event_data_set_fingers(e.e, 3);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	int fingers = 0;
	error = map_event_data_get_fingers(e.e, &fingers);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(fingers , ==, 3);
}

void utc_map_event_data_get_fingers_n(void)
{
	test_env e;

	int fingers = 0;
	int error = map_event_data_get_fingers(NULL, &fingers);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_fingers(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_event_data_get_zoom_factor(map_event_data_h event,
					double *zoom_factor);*/
void utc_map_event_data_get_zoom_factor_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_GESTURE);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = _map_event_data_set_zoom_factor(e.e, 3.1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	double zoom_factor = 0;
	error = map_event_data_get_zoom_factor(e.e, &zoom_factor);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(zoom_factor, >=, 3);
}

void utc_map_event_data_get_zoom_factor_n(void)
{
	test_env e;

	double zoom_factor = 0;
	int error = map_event_data_get_zoom_factor(NULL, &zoom_factor);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_zoom_factor(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}


/*int map_event_data_get_rotation_angle(map_event_data_h event,
					   double *rotation_angle);*/
void utc_map_event_data_get_rotation_angle_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_GESTURE);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = _map_event_data_set_rotation_angle(e.e, 3.1);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	double rotation_angle = 0;
	error = map_event_data_get_rotation_angle(e.e, &rotation_angle);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpfloat(rotation_angle, >=, 3);
}

void utc_map_event_data_get_rotation_angle_n(void)
{
	test_env e;

	double rotation_angle = 0;
	int error = map_event_data_get_rotation_angle(NULL, &rotation_angle);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_event_data_get_rotation_angle(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*int map_event_data_get_object(map_event_data_h event,
				   map_object_h *object);*/
void utc_map_event_data_get_object_p(void)
{
	test_env e;

	int error = _map_event_data_set_type(e.e, MAP_EVENT_OBJECT);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	map_object_h object = NULL;
	error = _map_object_create(MAP_OBJECT_MARKER, &object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = _map_event_data_set_object(e.e, object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	map_object_h o = NULL;
	error = map_event_data_get_object(e.e, &o);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(o);
	g_assert(o == object);

	error = map_object_destroy(object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_map_event_data_get_object_n(void)
{
	test_env e;

	map_object_h object = NULL;
	int error = _map_object_create(MAP_OBJECT_MARKER,
					   &object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = _map_event_data_set_object(e.e, object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	map_object_h o = NULL;
	error = map_event_data_get_object(NULL, &o);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert(!o);

	error = map_event_data_get_object(e.e, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = map_object_destroy(object);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}
