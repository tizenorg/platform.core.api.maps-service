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
#include "maps_error.h"
#include "map_event_data.h"
#include "maps_util.h"

/*
 * This represents visual event object information
 */
typedef struct _map_event_data_s {

	map_event_type_e event_type;

	/* Applicable for set_center action */
	maps_coordinates_h center;

	/* Applicable for gesture */
	int x;
	int y;

	/* Applicable for center move */
	int delta_x;
	int delta_y;

	/* Applicable for gestures */
	int fingers;

	/* Applicable for gestures, object */
	map_gesture_e gesture_type;

	/* Applicable for action */
	map_action_e action_type;

	/* Applicable for zoom */
	double zoom_factor;

	/* Applicable for rotation */
	double rotation_angle;

	/* Applicable for tilt */
	double tilt;

	/* Applicable for object event */
	map_object_h object;

} map_event_data_s;

/*----------------------------------------------------------------------------*/

int _map_event_data_set_type(map_event_data_h event,
			     map_event_type_e event_type);

int _map_event_data_set_gesture_type(map_event_data_h event,
				     const map_gesture_e gesture_type);

int _map_event_data_set_action_type(map_event_data_h event,
				    const map_action_e action_type);

int _map_event_data_set_center(map_event_data_h event,
			       const maps_coordinates_h center);

int _map_event_data_set_delta(map_event_data_h event,
			      const int delta_x, const int delta_y);

int _map_event_data_set_xy(map_event_data_h event,
			   const int x, const int y);

int _map_event_data_set_fingers(map_event_data_h event,
				const int fingers);

int _map_event_data_set_zoom_factor(map_event_data_h event,
				    const double zoom_factor);

int _map_event_data_set_rotation_angle(map_event_data_h event,
				       const double rotation_angle);

int _map_event_data_set_tilt(map_event_data_h event,
			     const double tilt);

int _map_event_data_set_object(map_event_data_h event,
			       map_object_h object);


/*----------------------------------------------------------------------------*/

int _map_event_data_create(map_event_data_h *event)
{
	MAPS_LOG_API;
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_event_data_s *e = g_slice_new0(map_event_data_s);
	if (!e) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	e->event_type = MAP_EVENT_GESTURE;
	e->gesture_type = MAP_GESTURE_PAN;
	e->action_type = MAP_ACTION_NONE;

	*event = (map_event_data_h) e;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_destroy(map_event_data_h event)
{
	MAPS_LOG_API;
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_event_data_s *e = (map_event_data_s *) event;

	if(e->center)
		maps_coordinates_destroy(e->center);

	g_slice_free(map_event_data_s, e);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_clone(const map_event_data_h origin,
					  map_event_data_h *cloned)
{
	MAPS_LOG_API;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = _map_event_data_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		map_event_data_s *e = (map_event_data_s *) origin;

		error = _map_event_data_set_type(*cloned, e->event_type);
		if (error != MAPS_ERROR_NONE)
			break;

		_map_event_data_set_gesture_type(*cloned, e->gesture_type);
		_map_event_data_set_action_type(*cloned, e->action_type);
		_map_event_data_set_center(*cloned, e->center);
		_map_event_data_set_xy(*cloned, e->x, e->y);
		_map_event_data_set_fingers(*cloned, e->fingers);
		_map_event_data_set_zoom_factor(*cloned, e->zoom_factor);
		_map_event_data_set_rotation_angle(*cloned, e->rotation_angle);
		_map_event_data_set_tilt(*cloned, e->tilt);
		_map_event_data_set_object(*cloned, e->object);

		return MAPS_ERROR_NONE;
	} while (false);

	map_event_data_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

int _map_event_data_set_type(map_event_data_h event,
				  map_event_type_e event_type)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->event_type = event_type;
	return MAPS_ERROR_NONE;
}

int _map_event_data_set_center(map_event_data_h event,
				    const maps_coordinates_h center)
{
	if (!event || !center)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	return maps_coordinates_clone(center, &e->center);
}

int _map_event_data_set_delta(map_event_data_h event,
				   const int delta_x, const int delta_y)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->delta_x = delta_x;
	e->delta_y = delta_y;
	return MAPS_ERROR_NONE;
}

int _map_event_data_set_xy(map_event_data_h event,
				const int x, const int y)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->x = x;
	e->y = y;
	return MAPS_ERROR_NONE;
}

int _map_event_data_set_fingers(map_event_data_h event,
				     const int fingers)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->fingers = fingers;
	return MAPS_ERROR_NONE;
}

int _map_event_data_set_gesture_type(map_event_data_h event,
					  const map_gesture_e gesture_type)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((gesture_type < MAP_GESTURE_PAN)
	   || (gesture_type > MAP_GESTURE_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->gesture_type = gesture_type;
	return MAPS_ERROR_NONE;
}

int _map_event_data_set_action_type(map_event_data_h event,
					 const map_action_e action_type)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((action_type < MAP_ACTION_SET_CENTER)
	   || (action_type > MAP_ACTION_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->action_type = action_type;
	return MAPS_ERROR_NONE;
}

int _map_event_data_set_zoom_factor(map_event_data_h event,
					 const double zoom_factor)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->zoom_factor = zoom_factor;
	return MAPS_ERROR_NONE;
}

int _map_event_data_set_rotation_angle(map_event_data_h event,
					    const double rotation_angle)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->rotation_angle = rotation_angle;
	return MAPS_ERROR_NONE;
}

int _map_event_data_set_tilt(map_event_data_h event,
				  const double tilt)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->tilt = tilt;
	return MAPS_ERROR_NONE;
}

int _map_event_data_set_object(map_event_data_h event,
				    map_object_h object)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	e->object = object;
	return MAPS_ERROR_NONE;
}


/*----------------------------------------------------------------------------*/


EXPORT_API int map_event_data_clone(const map_event_data_h origin,
				    map_event_data_h *cloned)
{
	MAPS_LOG_API;
	if (!origin || !cloned)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = _map_event_data_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		map_event_data_s *e = (map_event_data_s *) origin;


		error = _map_event_data_set_type(*cloned, e->event_type);
		if (error != MAPS_ERROR_NONE)
			break;

		error = _map_event_data_set_gesture_type(*cloned,
							 e->gesture_type);

		error = _map_event_data_set_action_type(*cloned,
							e->action_type);

		error = _map_event_data_set_center(*cloned, e->center);

		error = _map_event_data_set_delta(*cloned,
						  e->delta_x,
						  e->delta_y);

		error = _map_event_data_set_xy(*cloned, e->x, e->y);

		error = _map_event_data_set_fingers(*cloned, e->fingers);

		error = _map_event_data_set_zoom_factor(*cloned,
							e->zoom_factor);

		error = _map_event_data_set_rotation_angle(*cloned,
							   e->rotation_angle);

		error = _map_event_data_set_tilt(*cloned, e->tilt);

		error = _map_event_data_set_object(*cloned, e->object);

		return MAPS_ERROR_NONE;
	} while (false);

	map_event_data_destroy(*cloned);
	*cloned = NULL;
	return error;

}

EXPORT_API int map_event_data_get_type(const map_event_data_h event,
					    map_event_type_e *event_type)
{
	MAPS_LOG_API;
	if (!event || !event_type)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	*event_type = e->event_type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_get_gesture_type(map_event_data_h event,
					map_gesture_e *gesture_type)
{
	MAPS_LOG_API;
	if (!event || !gesture_type)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if((e->event_type != MAP_EVENT_GESTURE)
	   && (e->event_type != MAP_EVENT_OBJECT))
		return MAPS_ERROR_NOT_SUPPORTED;
	*gesture_type = e->gesture_type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_get_action_type(map_event_data_h event,
						map_action_e *action_type)
{
	MAPS_LOG_API;
	if (!event || !action_type)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if(e->event_type != MAP_EVENT_ACTION)
		return MAPS_ERROR_NOT_SUPPORTED;
	*action_type = e->action_type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_get_center(map_event_data_h event,
					       maps_coordinates_h *center)
{
	MAPS_LOG_API;
	if (!event || !center)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if((e->event_type != MAP_EVENT_ACTION)
	   && (e->event_type != MAP_EVENT_GESTURE))
		return MAPS_ERROR_NOT_SUPPORTED;
	return maps_coordinates_clone(e->center, center);
}

EXPORT_API int map_event_data_get_delta(map_event_data_h event,
					     int *delta_x, int *delta_y)
{
	MAPS_LOG_API;
	if (!event || !delta_x || !delta_y)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if(e->event_type != MAP_EVENT_ACTION)
		return MAPS_ERROR_NOT_SUPPORTED;
	*delta_x = e->delta_x;
	*delta_y = e->delta_y;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_get_xy(map_event_data_h event,
					  int *x, int* y)
{
	MAPS_LOG_API;
	if (!event || !x || !y)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if(e->event_type != MAP_EVENT_GESTURE)
		return MAPS_ERROR_NOT_SUPPORTED;
	*x = e->x;
	*y = e->y;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_get_fingers(map_event_data_h event,
					       int *fingers)
{
	MAPS_LOG_API;
	if (!event || !fingers)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if(e->event_type != MAP_EVENT_GESTURE)
		return MAPS_ERROR_NOT_SUPPORTED;
	*fingers = e->fingers;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_get_zoom_factor(map_event_data_h event,
						   double *zoom_factor)
{
	MAPS_LOG_API;
	if (!event || !zoom_factor)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if((e->event_type != MAP_EVENT_GESTURE)
	   && (e->event_type != MAP_EVENT_ACTION))
		return MAPS_ERROR_NOT_SUPPORTED;
	*zoom_factor = e->zoom_factor;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_get_rotation_angle(map_event_data_h
						      event,
						      double *rotation_angle)
{
	MAPS_LOG_API;
	if (!event || !rotation_angle)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if((e->event_type != MAP_EVENT_GESTURE)
	   && (e->event_type != MAP_EVENT_ACTION))
		return MAPS_ERROR_NOT_SUPPORTED;
	*rotation_angle = e->rotation_angle;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_get_tilt(map_event_data_h event,
					    double *tilt)
{
	MAPS_LOG_API;
	if (!event || !tilt)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if(e->event_type != MAP_EVENT_ACTION)
		return MAPS_ERROR_NOT_SUPPORTED;
	*tilt = e->tilt;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_event_data_get_object(map_event_data_h event,
					      map_object_h *object)
{
	MAPS_LOG_API;
	if (!event || !object)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_event_data_s *e = (map_event_data_s *) event;
	if(e->event_type != MAP_EVENT_OBJECT)
		return MAPS_ERROR_NOT_SUPPORTED;
	*object = e->object;
	return MAPS_ERROR_NONE;
}
