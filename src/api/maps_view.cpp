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


#include <tizen.h>
#include <Ecore.h>
#include <Evas.h>
#include <unistd.h>
#include <glib.h>

#include "maps_view.h"
#include "maps_view_plugin.h"
#include "maps_util.h"
#include "module.h"
#include "empty_module.h"
#include "commands.h"
#include "command_queue.h"
#include "poly_shape_hit_test.h"
#include "gesture_processor.h"
#include "inertial_camera.h"
#include "inertial_gesture.h"
#include "gesture_detector_statemachine.h"


/* TODO: remove useless or duplicative includes */


/*
 * The structure of callbacks info, Maps View is invoking during events
 */
typedef struct _maps_view_callback_info_s {
	maps_view_on_event_cb callback;
	void *user_data;
} maps_view_callback_info_s;

/*
 * The structure of map view idle listener callbacks info
 */
typedef struct _maps_view_idle_listener_info_s {
	void (*callback)(void *user_data);
	void *user_data;
} maps_view_idle_listener_info_s;



/*
 * The structure of Maps View internal data
 */
typedef struct _maps_view_s {

	/* Map Coordinates and Area */
	maps_area_h area;
	maps_coordinates_h center;

	/* Run-time scale values */
	double zoom_factor;
	double rotation_angle; /* in graduses */
	int zoom_level;

	/* Camera inertial transition data */
	view::inertial_camera *inertial_camera;
	maps_view_idle_listener_info_s idle_listener;

	/* We'd better to store that values in order to easily limit zoom level
	 * and factor in the run time without accessing the plugin frequently */
	int min_zoom_level;
	int max_zoom_level;

	/* The handle of Maps Service, used by Map View */
	maps_service_h maps;

	/* List of Visual Objects on the View */
	maps_item_list_h view_objects;

	/* Evas basis */
	Evas *canvas;
	Evas_Object *panel;

	/* Gesture Support */
	maps_view_action_e gesture_actions[MAPS_VIEW_GESTURE_LONG_PRESS + 1];
	bool gesture_available[MAPS_VIEW_GESTURE_LONG_PRESS + 1];

	/* Continuous gesture info */
	view::finger_event_stream *finger_stream;

	/* Map View callbacks */
	maps_view_callback_info_s event_callbacks[MAPS_VIEW_EVENT_READY + 1];

	/* Evas Queue support */
	Ecore_Idler *idler;
	Ecore_Animator *animator;
	volatile bool ready_to_draw;

	/* Map View Preferences */
	maps_view_type_e type;
	char *language;
	bool buildings_enabled;
	bool traffic_enabled;
	bool public_transit_enabled;
	bool scalebar_enabled;

	void *maps_plugin_view_handle;
} maps_view_s;


/* ---------------------------------------------------------------------------*/


const gsize _MAPS_VIEW_LANGUAGE_MAX_LENGTH = 16;


/* ---------------------------------------------------------------------------*/

extern plugin::plugin_s *__extract_plugin(maps_service_h maps);
extern int _maps_view_event_data_set_type(const maps_view_event_data_h event, maps_view_event_type_e event_type);
extern int _maps_view_event_data_set_gesture_type(const maps_view_event_data_h event, maps_view_gesture_e gesture_type);
extern int _maps_view_event_data_set_action_type(const maps_view_event_data_h event, maps_view_action_e action_type);
extern int _maps_view_event_data_set_center(const maps_view_event_data_h event, const maps_coordinates_h center);
extern int _maps_view_event_data_set_delta(const maps_view_event_data_h event, int delta_x, int delta_y);
extern int _maps_view_event_data_set_position(const maps_view_event_data_h event, int x, int y);
extern int _maps_view_event_data_set_fingers(const maps_view_event_data_h event, int fingers);
extern int _maps_view_event_data_set_zoom_factor(const maps_view_event_data_h event, double zoom_factor);
extern int _maps_view_event_data_set_rotation_angle(const maps_view_event_data_h event, double rotation_angle);
extern int _maps_view_event_data_set_object(const maps_view_event_data_h event, const maps_view_object_h object);
extern int _maps_view_event_data_create(maps_view_event_data_h *event);
extern int _maps_view_object_set_view(const maps_view_object_h object, const maps_view_h view);
maps_view_event_data_h _maps_view_create_event_data(maps_view_event_type_e type);
void _maps_view_invoke_event_callback(const maps_view_h view, const maps_view_event_data_h event_data);
int _maps_view_set_inertia_enabled(const maps_view_h view, bool enabled);

/* ---------------------------------------------------------------------------*/


/* TODO: Apply this approach for other cases, particularly, in maps_service.h*/
static const plugin::interface_s *__get_plugin_interface(maps_view_h view)
{
	const plugin::plugin_s *p =
		__extract_plugin(((maps_view_s *)view)->maps);
	return (p) ? (&p->interface) : plugin::get_empty_interface_ptr();
}


/* ---------------------------------------------------------------------------*/


static void __maps_view_on_event_empty_cb(maps_view_event_type_e type,
	maps_view_event_data_h event_data, void *user_data)
{
	/* empty */
}


session::command_queue *__maps_view_select_q()
{
	/*
	 *  This is a draw-enforcing mode, when the rendering and drawing
	 *  of the map is happening as fast as it possible.
	 *  This mode is good when the Map View is a single active widget
	 *  of the App.
	 *  In this mode we are relying on the ecore main loop and the
	 *  assumption, that loop steps will apear continuously, not
	 *  simultaneously.
	 *  https://docs.enlightenment.org/efl/1.15.0/Ecore_Main_Loop_Page.html
	 */
	return session::command_queue_sync::interface();

	/*
	 * This is an analogy of previous mode, but with thread safety
	 * implemented
	 */
	/*return session::command_queue_async::interface();*/

	/*
	 *  This is a thread-friendly mode, when all heavy computations
	 *  are performed in the "idle" mode.
	 *  This mode is good for apps, which has other active widgets in
	 *  addition to Map View.
	 *  This approach allows compression of repeating commands as well
	 */
	/* return session::command_queue_view::interface(); */
}

bool _maps_view_is_gesture_available(maps_view_h view, maps_view_gesture_e gesture)
{
	if(!view)
		return false;
	maps_view_s *v = (maps_view_s *)view;
	return v->gesture_available[gesture];
}

maps_view_action_e _maps_view_get_gesture_action(maps_view_h view, maps_view_gesture_e gesture)
{
	if(!view)
		return MAPS_VIEW_ACTION_NONE;
	maps_view_s *v = (maps_view_s *)view;
	return v->gesture_actions[gesture];
}

void *_maps_view_get_maps_service_ptr(maps_view_h view)
{
	if(!view)
		return NULL;
	maps_view_s *v = (maps_view_s *)view;
	return v->maps;
}

int _maps_view_on_object_operation(maps_view_h view, maps_view_object_h object, maps_view_object_operation_e operation)
{
	if(!view)
		return  MAPS_ERROR_INVALID_PARAMETER;

	if(!__get_plugin_interface(view)->maps_plugin_on_object)
		return  MAPS_ERROR_INVALID_PARAMETER;

	return __get_plugin_interface(view)->maps_plugin_on_object(view, object, operation);
}

static void __on_canvas_tap(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	MAPS_LOGI("__on_canvas_tap");
	if(!event_info || !data)
		return;

	/* Extract the view ptr */
	maps_view_s *v = (maps_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->tap((Evas_Event_Mouse_Down *)event_info);
}

static void __on_canvas_up(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	MAPS_LOGI("__on_canvas_up");
	if(!event_info || !data)
		return;

	/* Extract the view ptr */
	maps_view_s *v = (maps_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->up((Evas_Event_Mouse_Up *)event_info);
}

static void __on_canvas_line(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	MAPS_LOGI("__on_canvas_line");
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	maps_view_s *v = (maps_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->move((Evas_Event_Mouse_Move *)event_info);
}

static void __on_canvas_multi_tap(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	MAPS_LOGI("__on_canvas_multi_tap");
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	maps_view_s *v = (maps_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->multi_tap((Evas_Event_Multi_Down *)event_info);
}


static void __on_canvas_multi_up(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	MAPS_LOGI("__on_canvas_multi_up");
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	maps_view_s *v = (maps_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->multi_up((Evas_Event_Multi_Up *)event_info);
}

static void __maps_plugin_render_map_cb(maps_error_e result, int request_id,
	maps_coordinates_h center, maps_area_h area, void *user_data)
{
	if ((result != MAPS_ERROR_NONE) || !center || !area || !user_data)
		return;

	/* Here we know that the Plugin has the rendered maps.
	*  We should mark the view as "invalid"  and request the view update.
	*  In the view update or idle handle we should do the following.
	*  There are three types of idlers: Enterers, Idlers(proper) and Exiters
	*/

	maps_view_h view = (maps_view_h)user_data;
	if (!view)
		return;

	/* Get the view pointer */
	maps_view_s *v = (maps_view_s *) view;

	/* Signal to the animator, that we are ready to draw */
	v->ready_to_draw = true;

	maps_coordinates_destroy(center);
	maps_area_destroy(area);

}

static int __maps_plugin_render_map(const maps_view_h view,
	const maps_coordinates_h coordinates, const double zoom_factor, const double rotation_angle)
{
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	int request_id = 0;
	return __get_plugin_interface(view)->maps_plugin_render_map(view,
										coordinates, zoom_factor, rotation_angle,
										__maps_plugin_render_map_cb, view, &request_id);

}

static void __on_canvas_multi_line(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	MAPS_LOGI("__on_canvas_multi_line");
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	maps_view_s *v = (maps_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->multi_move((Evas_Event_Multi_Move *)event_info);
}

void _maps_view_set_idle_listener(const maps_view_h view,
	void (*callback)(void *user_data), void *user_data)
{

	if(!view)
		return;
	maps_view_s *v = (maps_view_s *)view;
	v->idle_listener.callback = callback;
	v->idle_listener.user_data = user_data;
}

void _maps_view_halt_inertial_camera(maps_view_h view)
{
	maps_view_s *v = (maps_view_s *)view;
	if(v && v->inertial_camera && v->inertial_camera->is_transiting()) {
		v->inertial_camera->set_transiting(false);
		g_usleep(0);
	}
}

static Eina_Bool __maps_view_on_idle_cb(void *data)
{
	if (!data)
		return ECORE_CALLBACK_RENEW;

	maps_view_s *v = (maps_view_s *)data;
	if(!v)
		return ECORE_CALLBACK_RENEW; // same as EINA_TRUE

	view::inertial_camera *ic = v->inertial_camera;
	if(!ic)
		return ECORE_CALLBACK_RENEW; // same as EINA_TRUE

	if(v->idle_listener.callback)
		v->idle_listener.callback(v->idle_listener.user_data);

	if(ic->is_transiting()) {
		ic->next_transition_step();
		__maps_plugin_render_map(v,
					 ic->get_cur_center(),
					 ic->get_cur_zoom_factor(),
					 ic->get_cur_rotation_angle());
		g_usleep(10*1000);
	}

	return ECORE_CALLBACK_RENEW; // same as EINA_TRUE
}

static Eina_Bool __maps_view_animator_cb(void *data)
{
	return ECORE_CALLBACK_RENEW;
}

void __maps_view_ready(const maps_view_h view)
{
	if (!view) return;

	_maps_view_set_inertia_enabled(view, true);

	/* Invoke user registered event callback */
#if 1
	maps_view_event_data_h ed = _maps_view_create_event_data(MAPS_VIEW_EVENT_READY);
	if(ed) {
		_maps_view_invoke_event_callback(view, ed);
		maps_view_event_data_destroy(ed);
	}
#else	
	_maps_view_invoke_event_callback((maps_view_s *)view,
			_maps_view_create_event_data(MAP_EVENT_READY));
#endif
}


/* ----------------------CREATE AND DESTROY-----------------------------------*/

static void __maps_view_panel_resize_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	maps_view_s *v = (maps_view_s*)data;

	int w, h;
	evas_object_geometry_get(v->panel, NULL, NULL, &w, &h);
	maps_view_resize(v, w, h);
}

/* Create the panel and link it to the instance of Maps Service */
EXPORT_API int maps_view_create(maps_service_h maps, Evas_Image *obj, maps_view_h *view)
{
	if (!maps || !obj || !view)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = g_slice_new0(maps_view_s);
	if (!v) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	/* Initialize the list with visual objects */
	maps_item_list_create(&v->view_objects);

	v->panel = obj;

	evas_object_event_callback_add(v->panel, EVAS_CALLBACK_RESIZE, __maps_view_panel_resize_cb, v);

	v->maps_plugin_view_handle = NULL;

	/* Set up event callbacks by default */
	const int event_callback_cnt =
		sizeof(v->event_callbacks) / sizeof(v->event_callbacks[0]);
	for(int i = 0; i < event_callback_cnt; i ++) {
		v->event_callbacks[i].callback = __maps_view_on_event_empty_cb;
		v->event_callbacks[i].user_data = NULL;
	}

	/* Assign gestures to actions */
	v->gesture_actions[MAPS_VIEW_GESTURE_SCROLL] = MAPS_VIEW_ACTION_NONE;
	v->gesture_actions[MAPS_VIEW_GESTURE_ZOOM] = MAPS_VIEW_ACTION_ZOOM;
	v->gesture_actions[MAPS_VIEW_GESTURE_TAP] = MAPS_VIEW_ACTION_NONE;
	v->gesture_actions[MAPS_VIEW_GESTURE_DOUBLE_TAP] = MAPS_VIEW_ACTION_ZOOM_IN;
	v->gesture_actions[MAPS_VIEW_GESTURE_2_FINGER_TAP] = MAPS_VIEW_ACTION_ZOOM_OUT;
	v->gesture_actions[MAPS_VIEW_GESTURE_ROTATE] = MAPS_VIEW_ACTION_ROTATE;
	v->gesture_actions[MAPS_VIEW_GESTURE_LONG_PRESS] = MAPS_VIEW_ACTION_NONE;
	v->gesture_actions[MAPS_VIEW_GESTURE_NONE] = MAPS_VIEW_ACTION_NONE;

	/* Set up gesture availability */
	v->gesture_available[MAPS_VIEW_GESTURE_SCROLL] = true;
	v->gesture_available[MAPS_VIEW_GESTURE_ZOOM] = true;
	v->gesture_available[MAPS_VIEW_GESTURE_TAP] = true;
	v->gesture_available[MAPS_VIEW_GESTURE_DOUBLE_TAP] = true;
	v->gesture_available[MAPS_VIEW_GESTURE_2_FINGER_TAP] = true;
	v->gesture_available[MAPS_VIEW_GESTURE_ROTATE] = true;
	v->gesture_available[MAPS_VIEW_GESTURE_LONG_PRESS] = true;
	v->gesture_available[MAPS_VIEW_GESTURE_NONE] = false;

	/* Gesture Processing */
	v->finger_stream = new view::finger_event_stream(v);
	if (!v->finger_stream) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}
	evas_object_event_callback_add(v->panel, EVAS_CALLBACK_MOUSE_DOWN, __on_canvas_tap, v);
	evas_object_event_callback_add(v->panel, EVAS_CALLBACK_MOUSE_UP, __on_canvas_up, v);
	evas_object_event_callback_add(v->panel, EVAS_CALLBACK_MOUSE_MOVE, __on_canvas_line, v);

	evas_object_event_callback_add(v->panel, EVAS_CALLBACK_MULTI_DOWN, __on_canvas_multi_tap, v);
	evas_object_event_callback_add(v->panel, EVAS_CALLBACK_MULTI_UP, __on_canvas_multi_up, v);
	evas_object_event_callback_add(v->panel, EVAS_CALLBACK_MULTI_MOVE, __on_canvas_multi_line, v);

	/* Set up canvas and Ecore */
	v->canvas = evas_object_evas_get(v->panel);
	/*v->ee = ecore_evas_ecore_evas_get(v->canvas);*/

	/* Link with Maps Service */
	v->maps = maps;

	/* Set up canvas and Ecore */
	maps_view_set_language(v, "eng");

	/* Add an idle handler */
	v->idler = ecore_idler_add(__maps_view_on_idle_cb, v);

	/* Set up the animation frame rate */
	/*ecore_animator_frametime_set(0.1);*/

	/* Create the animator and assign the drawing task */
	v->animator = ecore_animator_add(__maps_view_animator_cb, v);

	/* Set up the output pointer to the Maps View */
	*view = (maps_view_h) v;

	/* Notify the Plugin, that the view is created */
	__get_plugin_interface(v)->maps_plugin_create_map_view(*view, __maps_view_ready);

	/* Set up zoom and rotation */
	__get_plugin_interface(v)->maps_plugin_get_min_zoom_level(v, &v->min_zoom_level);
	__get_plugin_interface(v)->maps_plugin_get_max_zoom_level(v, &v->max_zoom_level);

	if(v->min_zoom_level <= 0)
		v->min_zoom_level = 2;
	if(v->max_zoom_level <= 0)
		v->max_zoom_level = 2;

	v->zoom_level = v->min_zoom_level;
	v->zoom_factor = double(v->zoom_level);
	v->rotation_angle = 0.;

	maps_coordinates_create(.0, .0, &v->center);

	v->type = MAPS_VIEW_TYPE_NORMAL;
	v->buildings_enabled = false;
	v->traffic_enabled = false;
	v->public_transit_enabled = false;

	return MAPS_ERROR_NONE;
}

/* Destroy the panel and unlink it from the instance of Maps Service */
EXPORT_API int maps_view_destroy(maps_view_h view)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *) view;

	/* Unregister gesture processing */
	if(v->finger_stream)
		delete v->finger_stream;
	v->finger_stream = NULL;

	evas_object_event_callback_del(v->panel, EVAS_CALLBACK_MOUSE_DOWN, __on_canvas_tap);
	evas_object_event_callback_del(v->panel, EVAS_CALLBACK_MOUSE_UP, __on_canvas_up);
	evas_object_event_callback_del(v->panel, EVAS_CALLBACK_MOUSE_MOVE, __on_canvas_line);

	evas_object_event_callback_del(v->panel, EVAS_CALLBACK_MULTI_DOWN, __on_canvas_multi_tap);
	evas_object_event_callback_del(v->panel, EVAS_CALLBACK_MULTI_UP, __on_canvas_multi_up);
	evas_object_event_callback_del(v->panel, EVAS_CALLBACK_MULTI_MOVE, __on_canvas_multi_line);


	/* Unregister idle handler */
	if (v->idler)
		ecore_idler_del(v->idler);

	view::inertial_camera *inertial_camera = v->inertial_camera;
	v->inertial_camera = NULL;
	v->idle_listener.callback = NULL;

	/* Delete the animator */
	if (v->animator)
		ecore_animator_del(v->animator);

	/* Notify the Plugin, that the view is to be destroyed */
	__get_plugin_interface(view)->maps_plugin_destroy_map_view(view);

	/* Destroy a visual panel */
	if (v->panel)
		evas_object_del(v->panel);

	if (v->center)
		maps_coordinates_destroy(v->center);

	if (v->area)
		maps_area_destroy(v->area);

	/* Destroy the list with visual objects */
	maps_item_list_remove_all(v->view_objects, maps_view_object_destroy);
	maps_item_list_destroy(v->view_objects);

	if (v->language)
		g_free(v->language);

	g_slice_free(maps_view_s, v);

	if (inertial_camera)
		delete inertial_camera;

	return MAPS_ERROR_NONE;
}


int _maps_view_set_center_directly(const maps_view_h view,
								const maps_coordinates_h coordinates)
{
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *) view;
	if(v->center != coordinates) {
		if (v->center)
			maps_coordinates_destroy(v->center);
		maps_coordinates_clone(coordinates, &v->center);
	}
	return MAPS_ERROR_NONE;
}

int _maps_view_get_plugin_center(const maps_view_h view,
								maps_coordinates_h *center)
{
	if (!view || !center)
		return MAPS_ERROR_INVALID_PARAMETER;

	return __get_plugin_interface(view)->maps_plugin_get_center(view, center);
}

/*----------------------MAP ZOOM, ROTATE, SET CENTER--------------------------*/

/* Show the map with a given position centered using current zoom level and
 * rotation angle */
EXPORT_API int maps_view_set_center(maps_view_h view, maps_coordinates_h coordinates)
{
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	double zoom_factor = 1.;
	int error = maps_view_get_zoom_factor(view, &zoom_factor);
	if (error != MAPS_ERROR_NONE)
		return error;

	double rotation_angle = 0.;
	error = maps_view_get_orientation(view, &rotation_angle);
	if (error != MAPS_ERROR_NONE)
		return error;

	maps_view_s *v = (maps_view_s *) view;

	/* Set up the target for camera inertial movement */
	if(v->inertial_camera)
		v->inertial_camera->set_targets(coordinates,
						zoom_factor,
						rotation_angle);
	else
		/* Rund rendering in the plugin */
		error = __maps_plugin_render_map(view,
						 coordinates,
						 zoom_factor,
						 rotation_angle);

	if(v->center != coordinates) {
		maps_coordinates_destroy(v->center);
		maps_coordinates_clone(coordinates, &v->center);
	}

	/* Invoke user registered event callback */
	maps_view_event_data_h ed =
		_maps_view_create_event_data(MAPS_VIEW_EVENT_ACTION);
	if(ed) {
		_maps_view_event_data_set_action_type(ed, MAPS_VIEW_ACTION_SCROLL);
		_maps_view_event_data_set_center(ed, v->center);
		_maps_view_invoke_event_callback(v, ed);
		maps_view_event_data_destroy(ed);
	}

	return error;
}

int _maps_view_move_center(maps_view_h view, const int delta_x, const int delta_y)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;


	/* TODO: Implement it with command and command queue,
	*  similarly as it is done in maps service */

	int request_id = 0;
	int error = __get_plugin_interface(view)->maps_plugin_move_center(view, delta_x, delta_y,
										__maps_plugin_render_map_cb, view, &request_id);

	/* Invoke user registered event callback */
	maps_view_event_data_h ed =
		_maps_view_create_event_data(MAPS_VIEW_EVENT_ACTION);
	if(ed) {
		_maps_view_event_data_set_action_type(ed, MAPS_VIEW_ACTION_SCROLL);
		_maps_view_event_data_set_delta(ed, delta_x, delta_y);
		_maps_view_invoke_event_callback(view, ed);
		maps_view_event_data_destroy(ed);
	}

	return error;
}

EXPORT_API int maps_view_set_scalebar_enabled(const maps_view_h view, bool enable)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	return __get_plugin_interface(view)->maps_plugin_set_scalebar(view, enable);
}

EXPORT_API int maps_view_get_scalebar_enabled(const maps_view_h view, bool *enabled)
{
	if (!view || !enabled)
		return MAPS_ERROR_INVALID_PARAMETER;

	return __get_plugin_interface(view)->maps_plugin_get_scalebar(view, enabled);
}

EXPORT_API int maps_view_get_center(const maps_view_h view, maps_coordinates_h *coordinates)
{
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	return maps_coordinates_clone(v->center, coordinates);
}

EXPORT_API int maps_view_set_zoom_level(maps_view_h view, int level)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *) view;
	int new_level = level;
	if (new_level < v->min_zoom_level) new_level = v->min_zoom_level;
	if (new_level > v->max_zoom_level) new_level = v->max_zoom_level;
	
	/* Add inertia to the zoom process */
	if(v->inertial_camera)
		v->inertial_camera->set_zoom_target(double(new_level));

	v->zoom_level = new_level;
	v->zoom_factor = double(new_level); /* Update the integer  zoom level too */

	const int error = maps_view_set_center(view, v->center);

	/* Invoke user registered event callback */
	maps_view_event_data_h ed =
		_maps_view_create_event_data(MAPS_VIEW_EVENT_ACTION);
	if(ed) {
		_maps_view_event_data_set_action_type(ed, MAPS_VIEW_ACTION_ZOOM);
		_maps_view_event_data_set_zoom_factor(ed, v->zoom_factor);
		_maps_view_invoke_event_callback(v, ed);
		maps_view_event_data_destroy(ed);
	}

	return error;
}

EXPORT_API int maps_view_get_zoom_level(const maps_view_h view, int *level)
{
	if (!view || !level)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*level = v->zoom_level;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_get_min_zoom_level(const maps_view_h view, int *min_zoom_level)
{
	if (!view || !min_zoom_level)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*min_zoom_level = v->min_zoom_level;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_get_max_zoom_level(const maps_view_h view, int *max_zoom_level)
{
	if (!view || !max_zoom_level)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*max_zoom_level = v->max_zoom_level;
	return MAPS_ERROR_NONE;
}

int _maps_view_set_zoom_rotate(maps_view_h view,
								const bool zoom_changed,
								const double factor,
								const bool rotation_changed,
								const double angle)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *) view;

	/* 1. Apply new zoom and orientation values */
	if (zoom_changed) {
		double new_factor = factor;
		if (new_factor < v->min_zoom_level) new_factor = v->min_zoom_level;
		if (new_factor > v->max_zoom_level) new_factor = v->max_zoom_level;

		/* Add inertia to the zoom process */
		if(v->inertial_camera)
			v->inertial_camera->set_zoom_target(new_factor);

		/* Update Map View zoom factor */
		v->zoom_factor = new_factor;

		/* Update the integer zoom level too */
		v->zoom_level = int(new_factor);
	}

	if (rotation_changed) {
		/* Add inertia to the rotation process */
		if(v->inertial_camera)
			v->inertial_camera->set_rotation_target(angle);

		/* Update Map View rotation angle */
		v->rotation_angle = angle;
	}

	/* 2. Send update center command to the plugin */
	const int error = maps_view_set_center(view, v->center);

	/* Notify the user about changes */
	if (zoom_changed) {
		/* Invoke user registered event callback */
		maps_view_event_data_h ed =
			_maps_view_create_event_data(MAPS_VIEW_EVENT_ACTION);
		if(ed) {
			_maps_view_event_data_set_action_type(ed, MAPS_VIEW_ACTION_ZOOM);
			_maps_view_event_data_set_zoom_factor(ed, v->zoom_factor);
			_maps_view_invoke_event_callback(v, ed);
			maps_view_event_data_destroy(ed);
		}
	}

	if (rotation_changed) {
		/* Invoke user registered event callback */
		maps_view_event_data_h ed =
			_maps_view_create_event_data(MAPS_VIEW_EVENT_ACTION);
		if(ed) {
			_maps_view_event_data_set_action_type(ed, MAPS_VIEW_ACTION_ROTATE);
			_maps_view_event_data_set_rotation_angle(ed, v->rotation_angle);
			_maps_view_invoke_event_callback(v, ed);
			maps_view_event_data_destroy(ed);
		}
	}

	return error;
}

EXPORT_API int maps_view_set_zoom_factor(const maps_view_h view,
					const double factor)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *) view;
	double new_factor = factor;
	if (new_factor < v->min_zoom_level) new_factor = v->min_zoom_level;
	if (new_factor > v->max_zoom_level) new_factor = v->max_zoom_level;

	return _maps_view_set_zoom_rotate(view, true, new_factor, false, .0);
}

EXPORT_API int maps_view_get_zoom_factor(const maps_view_h view, double *factor)
{
	if (!view || !factor)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*factor = v->zoom_factor;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_set_orientation(maps_view_h view, double angle)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Add inertia to the rotation process */
	maps_view_s *v = (maps_view_s *)view;
	if(v->inertial_camera)
		v->inertial_camera->set_rotation_target(angle);

	return _maps_view_set_zoom_rotate(view, false, .0, true, angle);
}

EXPORT_API int maps_view_get_orientation(const maps_view_h view, double *angle)
{
	if (!view || !angle)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*angle = v->rotation_angle;
	return MAPS_ERROR_NONE;
}


/*----------------------SCREEN <--> GEOGRAPHY---------------------------------*/


/* Converting screen coordinates to geographical */
EXPORT_API int maps_view_screen_to_geolocation(maps_view_h view,
	int x, int y, maps_coordinates_h *coordinates)
{
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	int posx = 0;
	int posy = 0;
	maps_view_get_screen_location(view, &posx, &posy, NULL, NULL);
	return __get_plugin_interface(view)->
						maps_plugin_screen_to_geography(view, x - posx, y - posy, coordinates);
}

EXPORT_API int maps_view_geolocation_to_screen(const maps_view_h view,
	const maps_coordinates_h coordinates, int *x, int *y)
{
	if (!view || !coordinates || !x || !y)
		return MAPS_ERROR_INVALID_PARAMETER;
	int posx = 0;
	int posy = 0;
	maps_view_get_screen_location(view, &posx, &posy, NULL, NULL);
	const int error = __get_plugin_interface(view)->
						maps_plugin_geography_to_screen(view, coordinates, x, y);
	*x += posx;
	*y += posy;
	return error;
}


/* --------------------MAPS VIEW PREFERENCES----------------------------------*/


EXPORT_API int maps_view_set_type(maps_view_h view, maps_view_type_e type)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((type < MAPS_VIEW_TYPE_NORMAL) || (type > MAPS_VIEW_TYPE_HYBRID))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	v->type = type;
	return maps_view_set_center(view, v->center);
}

EXPORT_API int maps_view_get_type(const maps_view_h view, maps_view_type_e *type)
{
	if (!view || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*type = v->type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_set_buildings_enabled(maps_view_h view, bool enabled)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	v->buildings_enabled = enabled;
	return maps_view_set_center(view, v->center);
}

EXPORT_API int maps_view_get_buildings_enabled(const maps_view_h view, bool *enabled)
{
	if (!view || !enabled)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*enabled = v->buildings_enabled;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_set_traffic_enabled(maps_view_h view, bool enabled)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	v->traffic_enabled = enabled;
	return maps_view_set_center(view, v->center);
}

EXPORT_API int maps_view_get_traffic_enabled(const maps_view_h view, bool *enabled)
{
	if (!view || !enabled)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*enabled = v->traffic_enabled;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_set_public_transit_enabled(maps_view_h view, bool enabled)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	v->public_transit_enabled = enabled;
	return maps_view_set_center(view, v->center);
}

EXPORT_API int maps_view_get_public_transit_enabled(const maps_view_h view, bool *enabled)
{
	if (!view || !enabled)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*enabled = v->public_transit_enabled;
	return MAPS_ERROR_NONE;
}

int _maps_view_set_inertia_enabled(maps_view_h view, bool enabled)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;

	if(enabled) {
		if(v->inertial_camera)
			return MAPS_ERROR_NONE;

		/* Set Inertial Gesture */
		v->finger_stream->set_gesture_detector(
					new view::inertial_gesture(view));

		/* Set inertial Camera */
		v->inertial_camera = new view::inertial_camera(view);
		if (!v->inertial_camera) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}
	} else {
		if(!v->inertial_camera)
			return MAPS_ERROR_NONE;

		/* Set Non-Inertial Gesture */
		v->finger_stream->set_gesture_detector(
			new view::gesture_detector_statemachine(view));

		/* Unset Inertial Camera */
		if(v->inertial_camera->is_transiting()) {
			v->inertial_camera->set_transiting(false);
			sleep(0);
		}
		view::inertial_camera *inertial_camera = v->inertial_camera;
		v->inertial_camera = NULL;
		delete inertial_camera;
	}
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_get_inertia_enabled(maps_view_h view, bool *enabled)
{
	if (!view || !enabled)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *) view;
	*enabled = v->inertial_camera != NULL;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_set_language(maps_view_h view, const char *language)
{
	if (!view || !language)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if language is supported */
	static const char *lngs[] = {
		"ara",
		"chi",
		"cht",
		"dut",
		"eng",
		"ger",
		"gle",
		"fre",
		"ita",
		"spa",
		"rus",
		"pol",
		"gre",
		"wel"
	};
	bool supported = false;
	for(unsigned int i = 0; i < (sizeof(lngs) / sizeof(lngs[0])); i ++) {
		if(g_strcmp0(language, lngs[i]) == 0) {
			supported = true;
			break;
		}
	}
	if(!supported)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *) view;
	int error = maps_set_string(language, _MAPS_VIEW_LANGUAGE_MAX_LENGTH, &v->language);
	if (error == MAPS_ERROR_NONE)
		error = maps_view_set_center(view, v->center);
	return error;
}

EXPORT_API int maps_view_get_language(const maps_view_h view, char **language)
{
	if (!view || !language)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_view_s *) view)->language,
		_MAPS_VIEW_LANGUAGE_MAX_LENGTH, language);
}


/* --------------------MAPS PANEL MANIPULATIONS-------------------------------*/

EXPORT_API int maps_view_get_viewport(const maps_view_h view, Evas_Object **viewport)
{
	if (!view || !viewport)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*viewport = v->panel;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_set_screen_location(maps_view_h view, int x, int y, int width, int height)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	int error = maps_view_move(view, x, y);
	if (error == MAPS_ERROR_NONE)
		error = maps_view_resize(view, width, height);
	return error;
}

EXPORT_API int maps_view_get_screen_location(const maps_view_h view, int *x, int *y, int *width, int *height)
{
	if (!view || (!x && !y && !width && !height))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	evas_object_geometry_get(v->panel, x, y, width, height);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_move(maps_view_h view, int x, int y)
{
	if (!view || (x < 0) || (y < 0))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	evas_object_move(v->panel, x, y);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_resize(maps_view_h view, int width, int height)
{
	if (!view || (width <= 0) || (height <= 0))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	evas_object_resize(v->panel, width, height);
	return maps_view_set_center(view, v->center);
}

EXPORT_API int maps_view_set_visibility(maps_view_h view, bool visible)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	if (visible)
		evas_object_show(v->panel);
	else
		evas_object_hide(v->panel);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_get_visibility(const maps_view_h view, bool *visible)
{
	if (!view || !visible)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*visible = evas_object_visible_get(v->panel) == EINA_TRUE;
	return MAPS_ERROR_NONE;
}

int _maps_view_redraw(const maps_view_h view)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;

	/* Signal to the animator, that we are ready to draw */
	v->ready_to_draw = true;

	return MAPS_ERROR_NONE;
}

/* ---------------------USER CONTROL------------------------------------------*/


EXPORT_API int maps_view_set_event_cb(maps_view_h view, maps_view_event_type_e type,
	maps_view_on_event_cb callback, void *user_data)
{
	if (!view || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((type < MAPS_VIEW_EVENT_GESTURE) || (type > MAPS_VIEW_EVENT_READY))
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *) view;
	v->event_callbacks[type].callback = callback;
	v->event_callbacks[type].user_data = user_data;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_unset_event_cb(maps_view_h view, maps_view_event_type_e type)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((type < MAPS_VIEW_EVENT_GESTURE) || (type > MAPS_VIEW_EVENT_READY))
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *) view;
	v->event_callbacks[type].callback = __maps_view_on_event_empty_cb;
	v->event_callbacks[type].user_data = NULL;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_set_gesture_enabled(maps_view_h view, maps_view_gesture_e gesture, bool enabled)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((gesture < MAPS_VIEW_GESTURE_NONE) || (gesture > MAPS_VIEW_GESTURE_LONG_PRESS))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	v->gesture_available[gesture] = enabled;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_get_gesture_enabled(const maps_view_h view, maps_view_gesture_e gesture, bool *enabled)
{
	if (!view || !enabled)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((gesture < MAPS_VIEW_GESTURE_NONE) || (gesture > MAPS_VIEW_GESTURE_LONG_PRESS))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *) view;
	*enabled = v->gesture_available[gesture];
	return MAPS_ERROR_NONE;
}


/* ---------------------VISUAL OBJECTS ON THE MAP-----------------------------*/

EXPORT_API int maps_view_add_object(maps_view_h view, maps_view_object_h object)
{
	if (!view || !object)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		/* Add Visual Object to the list of View Visual Objects */
		maps_view_s *v = (maps_view_s *)view;
		error = maps_item_list_append(v->view_objects, object, NULL);
		if(error != MAPS_ERROR_NONE)
			break;

		/* Link the object with the View */
		error = _maps_view_object_set_view(object, view);
		if(error != MAPS_ERROR_NONE)
			break;

		/* Notify the plugin about added object */
		error = _maps_view_on_object_operation(view, object, MAPS_VIEW_OBJECT_ADD);
		if(error != MAPS_ERROR_NONE)
			break;

		/* Redraw the view */
		error = _maps_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while(false);

	return error;
}

EXPORT_API int maps_view_remove_object(maps_view_h view, maps_view_object_h object)
{
	if (!view || !object)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		/* Remove Visual Object from the list of View Visual Objects */
		maps_view_s *v = (maps_view_s *)view;
		error =  maps_item_list_remove(v->view_objects, object, maps_view_object_destroy);
		if(error != MAPS_ERROR_NONE)
			return error;

		/* Redraw the view */
		error = _maps_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			return error;

		return MAPS_ERROR_NONE;
	} while(false);

	return error;
}

EXPORT_API int maps_view_remove_all_objects(maps_view_h view)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		maps_view_s *v = (maps_view_s *)view;
		error = maps_item_list_remove_all(v->view_objects, maps_view_object_destroy);
		if(error != MAPS_ERROR_NONE)
			return error;

		/* Redraw the view */
		error = _maps_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			return error;

		return MAPS_ERROR_NONE;
	} while(false);

	return error;
}

EXPORT_API int maps_view_foreach_object(maps_view_h view, maps_view_object_cb callback, void *user_data)
{
	if (!view || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_s *v = (maps_view_s *)view;
	return maps_item_list_foreach(v->view_objects, NULL, callback, user_data);
}


/* ----------------------SEND MAP VIEW CALLBACK TO THE USER-------------------*/

/* TODO: Consider refactoring of send Map View Callbacks to the User */

maps_view_event_data_h _maps_view_create_event_data(maps_view_event_type_e type)
{
	maps_view_event_data_h event_data = NULL;
	const int error = _maps_view_event_data_create(&event_data);
	if(error != MAPS_ERROR_NONE) {
		maps_view_event_data_destroy(event_data);
		return NULL;
	}
	if(!event_data)
		return NULL;
	_maps_view_event_data_set_type(event_data, type);
	return event_data;
}

void _maps_view_invoke_event_callback(maps_view_h view, maps_view_event_data_h event_data)
{
	if(!view || !event_data)
		return;

	maps_view_s *v = (maps_view_s *)view;

	maps_view_event_type_e type = MAPS_VIEW_EVENT_GESTURE;
	maps_view_event_data_get_type(event_data, &type);

	v->event_callbacks[type].callback(type, event_data, v->event_callbacks[type].user_data);
}

/* ----------------------HIT TEST---------------------------------------------*/
/* TODO: consider refactoring of Hit Test features and separation as
 *  dedicated classes */

typedef struct _maps_view_hit_test_data_s {
	maps_view_s *v;
	int x;
	int y;
	maps_view_object_h object;
} maps_view_hit_test_data_s;


typedef struct _maps_view_collect_poly_object_point_s {
	maps_view_s *v;
	view::poly_shape_hit_test *pd;
} maps_view_collect_poly_object_point_s;


static bool __maps_view_object_poly_collect_points_cb(int index, maps_coordinates_h point, void *user_data)
{
	if(!point || !user_data)
		return false;

	maps_view_collect_poly_object_point_s *cpop =
		(maps_view_collect_poly_object_point_s *)user_data;

	int x = 0;
	int y = 0;
	if(maps_view_geolocation_to_screen(cpop->v, point, &x, &y) == MAPS_ERROR_NONE)
		cpop->pd->add_point(float(x), float(y));

	return true;
}

static bool __maps_view_hit_test_cb(int index, int total, maps_view_object_h object, void *user_data)
{
	if(!object || !user_data)
		return false;

	/* If it is an unvisible object, skip this hit-testing. */
	bool visible = false;
	maps_view_object_get_visible(object, &visible);
	if (!visible) return true;

	maps_view_hit_test_data_s *htd = (maps_view_hit_test_data_s *)user_data;

	maps_view_object_type_e type;
	maps_view_object_get_type(object, &type);
	switch(type) {
		case MAPS_VIEW_OBJECT_POLYLINE: {
			view::poly_shape_hit_test pd;
			maps_view_collect_poly_object_point_s cpop = {htd->v, &pd};
			int error = maps_view_object_polyline_foreach_point(object,
				__maps_view_object_poly_collect_points_cb, &cpop);
			if(error != MAPS_ERROR_NONE)
				break;

			int width = 0;
			maps_view_object_polyline_get_width(object, &width);
			if(pd.hit_test(float(htd->x), float(htd->y), false, width))
				htd->object = object;

			break;
		}

		case MAPS_VIEW_OBJECT_POLYGON: {
			view::poly_shape_hit_test pd;
			maps_view_collect_poly_object_point_s cpop = {htd->v, &pd};
			int error = maps_view_object_polygon_foreach_point(object,
				__maps_view_object_poly_collect_points_cb, &cpop);
			if(error != MAPS_ERROR_NONE)
				break;

			if(pd.hit_test(float(htd->x), float(htd->y), true))
				htd->object = object;

			break;
		}
		case MAPS_VIEW_OBJECT_MARKER: {
			maps_coordinates_h c = NULL;
			maps_view_object_marker_get_coordinates(object, &c);

			int x = 0;
			int y = 0;
			maps_view_geolocation_to_screen(htd->v, c, &x, &y);
			maps_coordinates_destroy(c);

			int w = 0;
			int h = 0;
			maps_view_object_marker_get_size(object, &w, &h);

			/* In case of PIN marker type, rearrange the hit-area. */
			maps_view_marker_type_e marker_type;
			maps_view_object_marker_get_type(object, &marker_type);
			if (marker_type == MAPS_VIEW_MARKER_PIN)
				y -= h / 2;

			/* Add some margin of the hit-area. */
			if (w < 30) w = 30;
			if (h < 30) h = 30;

			/* Check hit-area */
			if((x > (htd->x - w)) && (x < (htd->x + w))
			   && (y > (htd->y - h)) && (y < (htd->y + h))) {
				htd->object = object;
			}
			break;
		}
		default:
			break;
	}

	if(htd->object)
		return false;

	return true;
}


maps_view_object_h _maps_view_object_hit_test(maps_view_h view, int x, int y, maps_view_gesture_e gesture)
{
	if (!view)
		return NULL;

	maps_view_s *v = (maps_view_s *)view;

	/* Check if we are going to inform anyone about object events */
	if (v->event_callbacks[MAPS_VIEW_EVENT_OBJECT].callback
		== __maps_view_on_event_empty_cb)
		return NULL;

	/* 1. Iterate through the list of object and test the hit */
	maps_view_hit_test_data_s htd = {v, x, y, NULL};
	int error = maps_view_foreach_object(view, __maps_view_hit_test_cb, &htd);
	if ((error != MAPS_ERROR_NONE) || !htd.object)
		return NULL;

	/*g_print("Hit Test: FOUND OBJECT: %p\n", htd.object);*/

	/* Invoke user registered event callback */
	maps_view_event_data_h ed =
		_maps_view_create_event_data(MAPS_VIEW_EVENT_OBJECT);
	if (ed) {
		_maps_view_event_data_set_object(ed, htd.object);
		_maps_view_event_data_set_gesture_type(ed, gesture);
		_maps_view_invoke_event_callback(view, ed);
		maps_view_event_data_destroy(ed);
	}

	/* 2. Extract test result */
	return htd.object;
}

EXPORT_API int maps_view_get_maps_plugin_view_handle(maps_view_h hView, void **maps_plugin_view_handle)
{
	if (!hView || !maps_plugin_view_handle)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *)hView;

	*maps_plugin_view_handle = v->maps_plugin_view_handle;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_set_maps_plugin_view_handle(maps_view_h hView, void *maps_plugin_view_handle)
{
	if (!hView || !maps_plugin_view_handle)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_s *v = (maps_view_s *)hView;

	v->maps_plugin_view_handle = maps_plugin_view_handle;

	return MAPS_ERROR_NONE;
}

