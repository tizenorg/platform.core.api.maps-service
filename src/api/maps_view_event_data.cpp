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
#include "maps_view_event_data.h"
#include "maps_util.h"
#include "maps_condition.h"

/*
 * This represents visual event object information
 */
typedef struct _maps_view_event_data_s {
	maps_view_event_type_e event_type;

	/* Applicable for set_center action */
	maps_coordinates_h center;

	/* Applicable for gesture */
	int x;
	int y;
 	maps_coordinates_h coordinates;

	/* Applicable for center move */
	int delta_x;
	int delta_y;

	/* Applicable for gestures */
	int fingers;

	/* Applicable for gestures, object */
	maps_view_gesture_e gesture_type;

	/* Applicable for action */
	maps_view_action_e action_type;

	/* Applicable for zoom */
	double zoom_factor;

	/* Applicable for rotation */
	double rotation_angle;

	/* Applicable for object event */
	maps_view_object_h object;
} maps_view_event_data_s;

/*----------------------------------------------------------------------------*/

int _maps_view_event_data_set_type(const maps_view_event_data_h event, maps_view_event_type_e event_type);

int _maps_view_event_data_set_gesture_type(const maps_view_event_data_h event, maps_view_gesture_e gesture_type);

int _maps_view_event_data_set_action_type(const maps_view_event_data_h event, maps_view_action_e action_type);

int _maps_view_event_data_set_center(const maps_view_event_data_h event, const maps_coordinates_h center);

int _maps_view_event_data_set_delta(const maps_view_event_data_h event, int delta_x, int delta_y);

int _maps_view_event_data_set_position(const maps_view_event_data_h event, int x, int y);

int _maps_view_event_data_set_fingers(const maps_view_event_data_h event, int fingers);

int _maps_view_event_data_set_zoom_factor(const maps_view_event_data_h event, double zoom_factor);

int _maps_view_event_data_set_rotation_angle(const maps_view_event_data_h event, double rotation_angle);

int _maps_view_event_data_set_object(const maps_view_event_data_h event, const maps_view_object_h object);


/*----------------------------------------------------------------------------*/

int _maps_view_event_data_create(maps_view_event_data_h *event)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_event_data_s *e = g_slice_new0(maps_view_event_data_s);
	if (!e) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	e->event_type = MAPS_VIEW_EVENT_GESTURE;
	e->gesture_type = MAPS_VIEW_GESTURE_SCROLL;
	e->action_type = MAPS_VIEW_ACTION_NONE;

	*event = (maps_view_event_data_h) e;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_destroy(maps_view_event_data_h event)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->center)
		maps_coordinates_destroy(e->center);
	g_slice_free(maps_view_event_data_s, e);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_clone(const maps_view_event_data_h origin, maps_view_event_data_h *cloned)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!origin || !cloned)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = _maps_view_event_data_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_view_event_data_s *e = (maps_view_event_data_s *) origin;

		error = _maps_view_event_data_set_type(*cloned, e->event_type);
		if (error != MAPS_ERROR_NONE)
			break;

		error = _maps_view_event_data_set_gesture_type(*cloned, e->gesture_type);
		error = _maps_view_event_data_set_action_type(*cloned, e->action_type);
		error = _maps_view_event_data_set_center(*cloned, e->center);
		error = _maps_view_event_data_set_delta(*cloned, e->delta_x, e->delta_y);
		error = _maps_view_event_data_set_position(*cloned, e->x, e->y);
		error = _maps_view_event_data_set_fingers(*cloned, e->fingers);
		error = _maps_view_event_data_set_zoom_factor(*cloned, e->zoom_factor);
		error = _maps_view_event_data_set_rotation_angle(*cloned, e->rotation_angle);
		error = _maps_view_event_data_set_object(*cloned, e->object);

		return MAPS_ERROR_NONE;
	} while (false);

	maps_view_event_data_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

int _maps_view_event_data_set_type(maps_view_event_data_h event, maps_view_event_type_e event_type)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	e->event_type = event_type;
	return MAPS_ERROR_NONE;
}

int _maps_view_event_data_set_center(maps_view_event_data_h event, maps_coordinates_h center)
{
	if (!event || !center)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->center)
		maps_coordinates_destroy(e->center);
	return maps_coordinates_clone(center, &e->center);
}

int _maps_view_event_data_set_delta(maps_view_event_data_h event, int delta_x, int delta_y)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	e->delta_x = delta_x;
	e->delta_y = delta_y;
	return MAPS_ERROR_NONE;
}

int _maps_view_event_data_set_position(maps_view_event_data_h event, int x, int y)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	e->x = x;
	e->y = y;
	return MAPS_ERROR_NONE;
}

int _maps_view_event_data_set_coordinates(maps_view_event_data_h event, maps_coordinates_h coordinates)
{
	if (!event || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->coordinates)
		maps_coordinates_destroy(e->coordinates);
	return maps_coordinates_clone(coordinates, &e->coordinates);
}

int _maps_view_event_data_set_fingers(maps_view_event_data_h event, int fingers)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	e->fingers = fingers;
	return MAPS_ERROR_NONE;
}

int _maps_view_event_data_set_gesture_type(maps_view_event_data_h event, maps_view_gesture_e gesture_type)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((gesture_type < MAPS_VIEW_GESTURE_NONE) || (gesture_type > MAPS_VIEW_GESTURE_LONG_PRESS))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	e->gesture_type = gesture_type;
	return MAPS_ERROR_NONE;
}

int _maps_view_event_data_set_action_type(maps_view_event_data_h event, maps_view_action_e action_type)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((action_type < MAPS_VIEW_ACTION_NONE) || (action_type > MAPS_VIEW_ACTION_ROTATE))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	e->action_type = action_type;
	return MAPS_ERROR_NONE;
}

int _maps_view_event_data_set_zoom_factor(maps_view_event_data_h event, double zoom_factor)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	e->zoom_factor = zoom_factor;
	return MAPS_ERROR_NONE;
}

int _maps_view_event_data_set_rotation_angle(maps_view_event_data_h event, double rotation_angle)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	e->rotation_angle = rotation_angle;
	return MAPS_ERROR_NONE;
}

int _maps_view_event_data_set_object(maps_view_event_data_h event, maps_view_object_h object)
{
	if (!event)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	e->object = object;
	return MAPS_ERROR_NONE;
}


/*----------------------------------------------------------------------------*/

EXPORT_API int maps_view_event_data_get_type(const maps_view_event_data_h event, maps_view_event_type_e *event_type)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !event_type)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	*event_type = e->event_type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_get_gesture_type(const maps_view_event_data_h event, maps_view_gesture_e *gesture_type)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !gesture_type)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if ((e->event_type != MAPS_VIEW_EVENT_GESTURE) && (e->event_type != MAPS_VIEW_EVENT_OBJECT))
		return MAPS_ERROR_INVALID_PARAMETER;
	*gesture_type = e->gesture_type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_get_action_type(const maps_view_event_data_h event, maps_view_action_e *action_type)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !action_type)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->event_type != MAPS_VIEW_EVENT_ACTION)
		return MAPS_ERROR_INVALID_PARAMETER;
	*action_type = e->action_type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_get_center(const maps_view_event_data_h event, maps_coordinates_h *center)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !center)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->event_type != MAPS_VIEW_EVENT_ACTION)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (!e->center)
		return MAPS_ERROR_NOT_FOUND;
	return maps_coordinates_clone(e->center, center);
}

EXPORT_API int maps_view_event_data_get_delta(const maps_view_event_data_h event, int *delta_x, int *delta_y)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !delta_x || !delta_y)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->event_type != MAPS_VIEW_EVENT_ACTION)
		return MAPS_ERROR_INVALID_PARAMETER;
	*delta_x = e->delta_x;
	*delta_y = e->delta_y;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_get_position(const maps_view_event_data_h event, int *x, int* y)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !x || !y)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->event_type != MAPS_VIEW_EVENT_GESTURE && e->event_type != MAPS_VIEW_EVENT_OBJECT)
		return MAPS_ERROR_INVALID_PARAMETER;
	*x = e->x;
	*y = e->y;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_get_coordinates(const maps_view_event_data_h event, maps_coordinates_h *coordinates)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->event_type != MAPS_VIEW_EVENT_GESTURE)
		return MAPS_ERROR_INVALID_OPERATION;
	if (!e->coordinates)
		return MAPS_ERROR_NOT_FOUND;
	return maps_coordinates_clone(e->coordinates, coordinates);
}

EXPORT_API int maps_view_event_data_get_fingers(const maps_view_event_data_h event, int *fingers)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !fingers)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->event_type != MAPS_VIEW_EVENT_GESTURE)
		return MAPS_ERROR_INVALID_PARAMETER;
	*fingers = e->fingers;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_get_zoom_factor(const maps_view_event_data_h event, double *zoom_factor)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !zoom_factor)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if ((e->event_type != MAPS_VIEW_EVENT_GESTURE) && (e->event_type != MAPS_VIEW_EVENT_ACTION))
		return MAPS_ERROR_INVALID_PARAMETER;
	*zoom_factor = e->zoom_factor;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_get_rotation_angle(const maps_view_event_data_h event, double *rotation_angle)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !rotation_angle)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if ((e->event_type != MAPS_VIEW_EVENT_GESTURE) && (e->event_type != MAPS_VIEW_EVENT_ACTION))
		return MAPS_ERROR_INVALID_PARAMETER;
	*rotation_angle = e->rotation_angle;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_event_data_get_object(const maps_view_event_data_h event, maps_view_object_h *object)
{
	if (!maps_condition_check_maps_feature())
		return MAPS_ERROR_NOT_SUPPORTED;
	if (!event || !object)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_event_data_s *e = (maps_view_event_data_s *) event;
	if (e->event_type != MAPS_VIEW_EVENT_OBJECT)
		return MAPS_ERROR_INVALID_PARAMETER;
	*object = e->object;
	return MAPS_ERROR_NONE;
}
