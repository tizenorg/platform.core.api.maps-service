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

#include "map_view.h"
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
typedef struct _map_view_callback_info_s {
	map_view_on_event_cb callback;
	void *user_data;
} map_view_callback_info_s;

/*
 * The structure of map view idle listener callbacks info
 */
typedef struct _map_view_idle_listener_info_s {
	void (*callback)(void *user_data);
	void *user_data;
} map_view_idle_listener_info_s;



/*
 * The structure of Maps View internal data
 */
typedef struct _map_view_s {

	/* Map Coordinates and Area */
	maps_area_h area;
	maps_coordinates_h center;

	/* Run-time scale values */
	double zoom_factor;
	double rotation_angle; /* in graduses */
	int zoom_level;

	/* Camera inertial transition data */
	view::inertial_camera *inertial_camera;
	map_view_idle_listener_info_s idle_listener;

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
	map_action_e gesture_actions[MAP_GESTURE_NONE + 1];
	bool gesture_available[MAP_GESTURE_NONE + 1];

	/* Continuous gesture info */
	view::finger_event_stream *finger_stream;

	/* Map View callbacks */
	map_view_callback_info_s event_callbacks[MAP_EVENT_READY + 1];

	/* Evas Queue support */
	Ecore_Idler *idler;
	Ecore_Animator *animator;
	volatile bool ready_to_draw;

	/* Map View Preferences */
	map_view_type_e type;
	char *language;

} map_view_s;


/* ---------------------------------------------------------------------------*/


const gsize _MAPS_VIEW_LANGUAGE_MAX_LENGTH = 16;


/* ---------------------------------------------------------------------------*/


extern plugin::plugin_s *__extract_plugin(maps_service_h maps);


extern int _map_event_data_set_type(map_event_data_h event,
				    map_event_type_e event_type);

extern int _map_event_data_set_gesture_type(map_event_data_h event,
					    const map_gesture_e
					    gesture_type);

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

extern int _map_event_data_create(map_event_data_h *event);

extern int _map_object_set_view(map_object_h object, map_view_h view);

map_event_data_h _map_view_create_event_data(map_event_type_e type);

void _map_view_invoke_event_callback(map_view_h view,
				     map_event_data_h event_data);


/* ---------------------------------------------------------------------------*/


/* TODO: Apply this approach for other cases, particularly, in maps_service.h*/
static const plugin::interface_s *__get_plugin_interface(map_view_h view)
{
	const plugin::plugin_s *p =
		__extract_plugin(((map_view_s *)view)->maps);
	return (p) ? (&p->interface) : plugin::get_empty_interface_ptr();
}


/* ---------------------------------------------------------------------------*/


static void __map_view_on_event_empty_cb(maps_error_e result,
					 const map_event_type_e type,
					 map_event_data_h event_data,
					 void *user_data)
{
	/* empty */
}


session::command_queue *__map_view_select_q()
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

static session::command_queue *q()
{
	/* Obsolete approach */
	/*return session::command_queue_view::interface();*/

	return __map_view_select_q();
}

bool _map_view_is_gesture_available(map_view_h view,
				    map_gesture_e gesture)
{
	if(!view)
		return false;
	map_view_s *v = (map_view_s *)view;
	return v->gesture_available[gesture];
}

map_action_e _map_view_get_gesture_action(map_view_h view,
					       map_gesture_e gesture)
{
	if(!view)
		return MAP_ACTION_NONE;
	map_view_s *v = (map_view_s *)view;
	return v->gesture_actions[gesture];
}

void *_map_view_get_maps_service_ptr(map_view_h view)
{
	if(!view)
		return NULL;
	map_view_s *v = (map_view_s *)view;
	return v->maps;
}

int _map_view_on_object_operation(map_view_h view,
				  const map_object_h object,
				  map_object_operation_e operation)
{
	if(!view)
		return  MAPS_ERROR_INVALID_PARAMETER;

	if(!__get_plugin_interface(view)->maps_plugin_on_object)
		return  MAPS_ERROR_INVALID_PARAMETER;

	return __get_plugin_interface(view)->maps_plugin_on_object(object,
								operation);
}

#if 0
/* DEBUG: for mesuring FPS */
#include <sys/timeb.h>
static int __get_milli_count()
{
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}
#endif


static void __on_canvas_tap(void *data,
			    Evas *e,
			    Evas_Object *obj,
			    void *event_info)
{
	MAPS_LOGI("__on_canvas_tap");
	if(!event_info || !data)
		return;

	/* Extract the view ptr */
	map_view_s *v = (map_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->tap((Evas_Event_Mouse_Down *)event_info);
}

static void __on_canvas_up(void *data,
			   Evas *e,
			   Evas_Object *obj,
			   void *event_info)
{
	MAPS_LOGI("__on_canvas_up");
	if(!event_info || !data)
		return;

	/* Extract the view ptr */
	map_view_s *v = (map_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->up((Evas_Event_Mouse_Up *)event_info);
}


static void __on_canvas_line(void *data,
			     Evas *e,
			     Evas_Object *obj,
			     void *event_info)
{
	MAPS_LOGI("__on_canvas_line");
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	map_view_s *v = (map_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->move((Evas_Event_Mouse_Move *)event_info);
}

static void __on_canvas_multi_tap(void *data,
				  Evas *e,
				  Evas_Object *obj,
				  void *event_info)
{
	MAPS_LOGI("__on_canvas_multi_tap");
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	map_view_s *v = (map_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->multi_tap((Evas_Event_Multi_Down *)event_info);
}


static void __on_canvas_multi_up(void *data,
				  Evas *e,
				  Evas_Object *obj,
				  void *event_info)
{
	MAPS_LOGI("__on_canvas_multi_up");
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	map_view_s *v = (map_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->multi_up((Evas_Event_Multi_Up *)event_info);
}

static void __maps_plugin_render_map_cb(maps_error_e result, int request_id,
				 maps_coordinates_h center,
				 maps_area_h area,
				 void *user_data)
{
	if ((result != MAPS_ERROR_NONE) || !center || !area || !user_data)
		return;

	/* Here we know that the Plugin has the rendered maps.
	*  We should mark the view as "invalid"  and request the view update.
	*  In the view update or idle handle we should do the following.
	*  There are three types of idlers: Enterers, Idlers(proper) and Exiters
	*/

	map_view_h view = (map_view_h)user_data;
	if (!view)
		return;

	/* Get the view pointer */
	map_view_s *v = (map_view_s *) view;

	/* Signal to the animator, that we are ready to draw */
	v->ready_to_draw = true;

	maps_coordinates_destroy(center);
	maps_area_destroy(area);

}

static int __maps_plugin_render_map(map_view_h view,
				    const maps_coordinates_h coordinates,
				    const double zoom_factor,
				    const double rotation_angle)
{
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	int request_id = 0;
	return __get_plugin_interface(view)->maps_plugin_render_map(
					     coordinates,
					     zoom_factor,
					     rotation_angle,
					     __maps_plugin_render_map_cb,
					     view,
					     &request_id);

}

static void __on_canvas_multi_line(void *data,
				  Evas *e,
				  Evas_Object *obj,
				  void *event_info)
{
	MAPS_LOGI("__on_canvas_multi_line");
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	map_view_s *v = (map_view_s *)data;
	if(!v->finger_stream)
		return;

	/* Detect & Process the gesture */
	v->finger_stream->multi_move((Evas_Event_Multi_Move *)event_info);
}

/*gesture_detector *__map_view_gesture_detector_factory()
{
}*/

void _map_view_set_idle_listener(map_view_h view,
				 void (*callback)(void *user_data),
				 void *user_data)
{

	if(!view)
		return;
	map_view_s *v = (map_view_s *)view;
	v->idle_listener.callback = callback;
	v->idle_listener.user_data = user_data;
}

void _map_view_halt_inertial_camera(map_view_h view)
{
	map_view_s *v = (map_view_s *)view;
	if(v && v->inertial_camera && v->inertial_camera->is_transiting()) {
		v->inertial_camera->set_transiting(false);
		g_usleep(0);
	}
}

static Eina_Bool __map_view_on_idle_cb(void *data)
{
	if (!data)
		return ECORE_CALLBACK_RENEW;

	map_view_s *v = (map_view_s *)data;
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

#if 0
	/* Extract and run the on-going command */
	q()->process(__extract_plugin(v->maps));
#endif

	return ECORE_CALLBACK_RENEW; // same as EINA_TRUE
}

static Eina_Bool __map_view_animator_cb(void *data)
{
	map_view_s *v = (map_view_s *) data;
	if (!v)
		return ECORE_CALLBACK_CANCEL;

	if (!v->ready_to_draw)
		return ECORE_CALLBACK_RENEW;

	v->ready_to_draw = false;

	/* draw routin: use the plugin function */
	int width = 0;
	int height = 0;
	map_view_get_geometry(v, NULL, NULL, &width, &height);

	/* The Plugin should draw a map. */
	__get_plugin_interface(v)->
		maps_plugin_draw_map(v->canvas, 0, 0, width, height);

	q()->push(new session::command_view_ready(v->maps, v));

	return ECORE_CALLBACK_RENEW;
}

int __map_view_ready(map_view_h view)
{
	map_view_s *v = (map_view_s *) view;
	if (!v)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Invoke user registered event callback */
	_map_view_invoke_event_callback(v,
			_map_view_create_event_data(MAP_EVENT_READY));

	return MAPS_ERROR_NONE;
}




/* ----------------------CREATE AND DESTROY-----------------------------------*/

static void __map_view_panel_resize_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	map_view_s *v = (map_view_s*)data;

	int w, h;
	evas_object_geometry_get(v->panel, NULL, NULL, &w, &h);
	map_view_resize(v, w, h);
}

/* Create the panel and link it to the instance of Maps Service */
EXPORT_API int map_view_create(const maps_service_h maps,
				Evas_Image *obj,
				map_view_h *view)
{
	MAPS_LOG_API;
	if (!maps || !obj || !view)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = g_slice_new0(map_view_s);
	if (!v) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	/* Initialize the list with visual objects */
	maps_item_list_create(&v->view_objects);

	v->panel = obj;

	evas_object_event_callback_add(v->panel, EVAS_CALLBACK_RESIZE, __map_view_panel_resize_cb, v);

	/* Set up event callbacks by default */
	const int event_callback_cnt =
		sizeof(v->event_callbacks) / sizeof(v->event_callbacks[0]);
	for(int i = 0; i < event_callback_cnt; i ++) {
		v->event_callbacks[i].callback = __map_view_on_event_empty_cb;
		v->event_callbacks[i].user_data = NULL;
	}

#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	/*
	Gestures                        | Available actions
	--------------------------------+------------------------------------------------------
	MAP_GESTURE_SCROLL		| MAP_ACTION_NONE
                                        |
	MAP_GESTURE_ZOOM,		| MAP_ACTION_ZOOM
                                        |
	MAP_GESTURE_TAP,		| MAP_ACTION_SCROLL, MAP_ACTION_ZOOM_IN, MAP_ACTION_ZOOM_OUT,
                                        |
	MAP_GESTURE_DOUBLE_TAP,		| MAP_ACTION_SCROLL, MAP_ACTION_ZOOM_IN, MAP_ACTION_ZOOM_OUT,
	                                | MAP_ACTION_ZOOM_AND_SCROLL
                                        |
	MAP_GESTURE_2_FINGER_TAP,	| MAP_ACTION_SCROLL, MAP_ACTION_ZOOM_IN, MAP_ACTION_ZOOM_OUT,
	                                | MAP_ACTION_ZOOM_AND_SCROLL
                                        |
	MAP_GESTURE_SINGLE_FINGER_ZOOM,	| MAP_ACTION_ZOOM, MAP_ACTION_ZOOM_AND_SCROLL
                                        |
	MAP_GESTURE_LONG_PRESS,		| MAP_ACTION_SCROLL, MAP_ACTION_ZOOM_IN, MAP_ACTION_ZOOM_OUT
	*/

	/* Assign gestures to actions */
	v->gesture_actions[MAP_GESTURE_SCROLL] = MAP_ACTION_NONE;
	v->gesture_actions[MAP_GESTURE_ZOOM] = MAP_ACTION_ZOOM;
	v->gesture_actions[MAP_GESTURE_FLICK] = MAP_ACTION_SCROLL;
	v->gesture_actions[MAP_GESTURE_TAP] = MAP_ACTION_SCROLL;
	v->gesture_actions[MAP_GESTURE_DOUBLE_TAP] = MAP_ACTION_ZOOM_AND_SCROLL;
	v->gesture_actions[MAP_GESTURE_2_FINGER_TAP] = MAP_ACTION_ZOOM_OUT;
	v->gesture_actions[MAP_GESTURE_SINGLE_FINGER_ZOOM] = MAP_ACTION_ZOOM;
	v->gesture_actions[MAP_GESTURE_LONG_PRESS] = MAP_ACTION_SCROLL;
	v->gesture_actions[MAP_GESTURE_NONE] = MAP_ACTION_NONE;

	/* Set up gesture availability */
	v->gesture_available[MAP_GESTURE_SCROLL] = true;
	v->gesture_available[MAP_GESTURE_ZOOM] = true;
	v->gesture_available[MAP_GESTURE_FLICK] = true;
	v->gesture_available[MAP_GESTURE_TAP] = true;
	v->gesture_available[MAP_GESTURE_DOUBLE_TAP] = true;
	v->gesture_available[MAP_GESTURE_2_FINGER_TAP] = true;
	v->gesture_available[MAP_GESTURE_SINGLE_FINGER_ZOOM] = true;
	v->gesture_available[MAP_GESTURE_LONG_PRESS] = true;
	v->gesture_available[MAP_GESTURE_NONE] = false;
#else
	/* Assign gestures to actions */
	v->gesture_actions[MAP_GESTURE_SCROLL] = MAP_ACTION_SCROLL;
	v->gesture_actions[MAP_GESTURE_FLICK] = MAP_ACTION_SCROLL;
	v->gesture_actions[MAP_GESTURE_PINCH] = MAP_ACTION_SCROLL;
	v->gesture_actions[MAP_GESTURE_TAP] = MAP_ACTION_NONE;
	v->gesture_actions[MAP_GESTURE_DOUBLE_TAP] = MAP_ACTION_ZOOM;
	v->gesture_actions[MAP_GESTURE_2_FINGER_TAP] = MAP_ACTION_ZOOM;
	v->gesture_actions[MAP_GESTURE_ZOOM] = MAP_ACTION_ZOOM;
	v->gesture_actions[MAP_GESTURE_SINGLE_FINGER_ZOOM] = MAP_ACTION_ZOOM;
	v->gesture_actions[MAP_GESTURE_ROTATE] = MAP_ACTION_ROTATE;
	v->gesture_actions[MAP_GESTURE_LONG_PRESS] = MAP_ACTION_NONE;
	v->gesture_actions[MAP_GESTURE_NONE] = MAP_ACTION_NONE;

	/* Set up gesture availability */
	v->gesture_available[MAP_GESTURE_SCROLL] = true;
	v->gesture_available[MAP_GESTURE_FLICK] = true;
	v->gesture_available[MAP_GESTURE_PINCH] = true;
	v->gesture_available[MAP_GESTURE_TAP] = true;
	v->gesture_available[MAP_GESTURE_DOUBLE_TAP] = true;
	v->gesture_available[MAP_GESTURE_2_FINGER_TAP] = true;
	v->gesture_available[MAP_GESTURE_ZOOM] = true; /* pinch zoom */
	v->gesture_available[MAP_GESTURE_SINGLE_FINGER_ZOOM] = true;
	v->gesture_available[MAP_GESTURE_ROTATE] = true;
	v->gesture_available[MAP_GESTURE_LONG_PRESS] = true;
	v->gesture_available[MAP_GESTURE_NONE] = false;
#endif

	/* Gesture Processing */
	v->finger_stream = new view::finger_event_stream(v);
	if (!v->finger_stream) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}
	evas_object_event_callback_add(v->panel,
				       EVAS_CALLBACK_MOUSE_DOWN,
				       __on_canvas_tap,
				       v);
	evas_object_event_callback_add(v->panel,
				       EVAS_CALLBACK_MOUSE_UP,
				       __on_canvas_up,
				       v);
	evas_object_event_callback_add(v->panel,
				       EVAS_CALLBACK_MOUSE_MOVE,
				       __on_canvas_line,
				       v);

	evas_object_event_callback_add(v->panel,
				       EVAS_CALLBACK_MULTI_DOWN,
				       __on_canvas_multi_tap,
				       v);
	evas_object_event_callback_add(v->panel,
				       EVAS_CALLBACK_MULTI_UP,
				       __on_canvas_multi_up,
				       v);
	evas_object_event_callback_add(v->panel,
				       EVAS_CALLBACK_MULTI_MOVE,
				       __on_canvas_multi_line,
				       v);

	/* Set up canvas and Ecore */
	v->canvas = evas_object_evas_get(v->panel);
	/*v->ee = ecore_evas_ecore_evas_get(v->canvas);*/

	/* Link with Maps Service */
	v->maps = maps;

	/* Add an idle handler */
	v->idler = ecore_idler_add(__map_view_on_idle_cb, v);

	/* Set up the animation frame rate */
	/*ecore_animator_frametime_set(0.1);*/

	/* Create the animator and assign the drawing task */
	v->animator = ecore_animator_add(__map_view_animator_cb, v);

	/* Set up the output pointer to the Maps View */
	*view = (map_view_h) v;

	/* Notify the Plugin, that the view is created */
	__get_plugin_interface(*view)->maps_plugin_set_map_view(*view);

	/* Set up zoom and rotation */
	__get_plugin_interface(v)->maps_plugin_get_min_zoom_level(
							  &v->min_zoom_level);
	__get_plugin_interface(v)->maps_plugin_get_max_zoom_level(
								  &v->max_zoom_level);

	if(v->min_zoom_level <= 0)
		v->min_zoom_level = 2;
	if(v->max_zoom_level <= 0)
		v->max_zoom_level = 2;

	v->zoom_level = v->min_zoom_level;
	v->zoom_factor = double(v->zoom_level);
	v->rotation_angle = 0.;

	maps_coordinates_create(.0, .0, &v->center);

	map_view_set_inertia_enabled(v, true);

	return MAPS_ERROR_NONE;
}

/* Destroy the panel and unlink it from the instance of Maps Service */
EXPORT_API int map_view_destroy(map_view_h view)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;

	/* Unregister gesture processing */
	if(v->finger_stream)
		delete v->finger_stream;
	v->finger_stream = NULL;

	evas_object_event_callback_del(v->panel,
				       EVAS_CALLBACK_MOUSE_DOWN,
				       __on_canvas_tap);
	evas_object_event_callback_del(v->panel,
				       EVAS_CALLBACK_MOUSE_UP,
				       __on_canvas_up);
	evas_object_event_callback_del(v->panel,
				       EVAS_CALLBACK_MOUSE_MOVE,
				       __on_canvas_line);

	evas_object_event_callback_del(v->panel,
				       EVAS_CALLBACK_MULTI_DOWN,
				       __on_canvas_multi_tap);
	evas_object_event_callback_del(v->panel,
				       EVAS_CALLBACK_MULTI_UP,
				       __on_canvas_multi_up);
	evas_object_event_callback_del(v->panel,
				       EVAS_CALLBACK_MULTI_MOVE,
				       __on_canvas_multi_line);



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
	__get_plugin_interface(view)->maps_plugin_set_map_view(NULL);

	/* Destroy a visual panel */
	if (v->panel)
		evas_object_del(v->panel);

	if (v->center)
		maps_coordinates_destroy(v->center);

	if (v->area)
		maps_area_destroy(v->area);

	/* Destroy the list with visual objects */
	maps_item_list_remove_all(v->view_objects, map_object_destroy);
	maps_item_list_destroy(v->view_objects);

	if (v->language)
		g_free(v->language);

	g_slice_free(map_view_s, v);

	if(inertial_camera)
		delete inertial_camera;


	return MAPS_ERROR_NONE;
}


int _map_view_set_center_directly(const map_view_h view,
				  const maps_coordinates_h coordinates)
{
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;
	if(v->center != coordinates) {
		if (v->center)
			maps_coordinates_destroy(v->center);
		maps_coordinates_clone(coordinates, &v->center);
	}
	return MAPS_ERROR_NONE;
}

int _map_view_get_plugin_center(const map_view_h view,
				maps_coordinates_h *center)
{
	if (!view || !center)
		return MAPS_ERROR_INVALID_PARAMETER;

	return __get_plugin_interface(view)->maps_plugin_get_center(center);
}

/*----------------------MAP ZOOM, ROTATE, SET CENTER--------------------------*/

/* Show the map with a given position centered using current zoom level and
 * rotation angle */
EXPORT_API int map_view_set_center(const map_view_h view,
				   const maps_coordinates_h coordinates)
{
	MAPS_LOG_API;
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	double zoom_factor = 1.;
	int error = map_view_get_zoom_factor(view, &zoom_factor);
	if (error != MAPS_ERROR_NONE)
		return error;

	double rotation_angle = 0.;
	error = map_view_get_orientation(view, &rotation_angle);
	if (error != MAPS_ERROR_NONE)
		return error;

	#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
	map_view_s *v = (map_view_s *) view;

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
	#else
	map_view_s *v = (map_view_s *) view;
	if(v->center != coordinates) {
		if (v->center)
			maps_coordinates_destroy(v->center);
		maps_coordinates_clone(coordinates, &v->center);
	}

	/* Set up the target for camera inertial movement */
	if(v->inertial_camera)
		v->inertial_camera->set_targets(v->center,
						zoom_factor,
						rotation_angle);
	else
		/* Rund rendering in the plugin */
		error = __maps_plugin_render_map(view,
						 v->center,
						 zoom_factor,
						 rotation_angle);
	#endif

	/* Invoke user registered event callback */
	map_event_data_h ed =
		_map_view_create_event_data(MAP_EVENT_ACTION);
	if(ed) {
		_map_event_data_set_action_type(ed, MAP_ACTION_SCROLL);
		_map_event_data_set_center(ed, v->center);
		_map_view_invoke_event_callback(v, ed);
	}

	return error;
}

int _map_view_move_center(map_view_h view, const int delta_x, const int delta_y)
{
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;


	/* TODO: Implement it with command and command queue,
	*  similarly as it is done in maps service */

	int request_id = 0;
	int error = __get_plugin_interface(view)->maps_plugin_move_center(
						delta_x, delta_y,
						__maps_plugin_render_map_cb,
						view,
						&request_id);

	/* Invoke user registered event callback */
	map_event_data_h ed =
		_map_view_create_event_data(MAP_EVENT_ACTION);
	if(ed) {
		#ifdef IMPROVEMENT_OF_GESTURES_AND_ACTIONS
		_map_event_data_set_action_type(ed, MAP_ACTION_SCROLL);
		#else
		_map_event_data_set_action_type(ed, MAP_ACTION_MOVE_CENTER);
		#endif
		_map_event_data_set_delta(ed, delta_x, delta_y);
		_map_view_invoke_event_callback(view, ed);
	}

	return error;
}

EXPORT_API int map_view_get_center(const map_view_h view,
				   maps_coordinates_h *coordinates)
{
	MAPS_LOG_API;
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	return maps_coordinates_clone(v->center, coordinates);
}

EXPORT_API int map_view_set_zoom_level(map_view_h view, const int level)
{
#if 1
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;
	int new_level = level;
	if (new_level < v->min_zoom_level) new_level = v->min_zoom_level;
	if (new_level > v->max_zoom_level) new_level = v->max_zoom_level;
	
	/* Add inertia to the zoom process */
	if(v->inertial_camera)
		v->inertial_camera->set_zoom_target(double(new_level));

	v->zoom_level = new_level;
	v->zoom_factor = double(new_level); /* Update the integer  zoom level too */
#else
	MAPS_LOG_API;
	if (!view || (level <= 1))
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;


	/* TODO: consider more convenient for App Devs approach:
	 *    level = min(level, max_zoom);
	 *    level = max(level, min_zoom);
	 *    error = NONE
	 */

	/* Check if new zoom factor matches existing zoom range */
	if((level < v->min_zoom_level) || (level > v->max_zoom_level))
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Add inertia to the zoom process */
	if(v->inertial_camera)
		v->inertial_camera->set_zoom_target(double(level));

	v->zoom_level = level;
	v->zoom_factor = double(level); /* Update the integer  zoom level too */
#endif

	const int error = map_view_set_center(view, v->center);

	/* Invoke user registered event callback */
	map_event_data_h ed =
		_map_view_create_event_data(MAP_EVENT_ACTION);
	if(ed) {
		_map_event_data_set_action_type(ed, MAP_ACTION_ZOOM);
		_map_event_data_set_zoom_factor(ed, v->zoom_factor);
		_map_view_invoke_event_callback(v, ed);
	}

	return error;
}

EXPORT_API int map_view_get_zoom_level(const map_view_h view, int *level)
{
	MAPS_LOG_API;
	if (!view || !level)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*level = v->zoom_level;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_get_min_zoom_level(const map_view_h view,
					   int *min_zoom_level)
{
	MAPS_LOG_API;
	if (!view || !min_zoom_level)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*min_zoom_level = v->min_zoom_level;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_get_max_zoom_level(const map_view_h view,
					   int *max_zoom_level)
{
	MAPS_LOG_API;
	if (!view || !max_zoom_level)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*max_zoom_level = v->max_zoom_level;
	return MAPS_ERROR_NONE;
}

int _map_view_set_zoom_rotate(map_view_h view,
			      const bool zoom_changed,
			      const double factor,
			      const bool rotation_changed,
			      const double angle)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;

	/* 1. Apply new zoom and orientation values */
	if (zoom_changed) {
		/* TODO: consider more convenient for App Devs approach:
		*    factor = min(factor, max_zoom);
		*    factor = max(factor, min_zoom);
		*    error = NONE
		 */

		#if 1
		double new_factor = factor;
		if (new_factor < v->min_zoom_level) new_factor = v->min_zoom_level;
		if (new_factor > v->max_zoom_level) new_factor = v->max_zoom_level;

		/* Add inertia to the zoom process */
		if(v->inertial_camera) {
			v->inertial_camera->set_zoom_target(new_factor);
			if(rotation_changed)
				v->inertial_camera->set_rotation_target(angle);
		}

		/* Update Map View zoom factor */
		v->zoom_factor = new_factor;

		/* Update the integer  zoom level too */
		v->zoom_level = int(new_factor);
		#else
		/* Check if new zoom factor matches existing zoom range */
		if((factor < v->min_zoom_level) || (factor > v->max_zoom_level))
			return MAPS_ERROR_INVALID_PARAMETER;

		/* Add inertia to the zoom process */
		if(v->inertial_camera) {
			v->inertial_camera->set_zoom_target(factor);
			if(rotation_changed)
				v->inertial_camera->set_rotation_target(angle);
		}

		/* Update Map View zoom factor */
		v->zoom_factor = factor;

		/* Update the integer  zoom level too */
		v->zoom_level = int(factor);
		#endif
	}

	if (rotation_changed) {
		v->rotation_angle = angle;
	}

	/* 2. Send update center command to the plugin */
	const int error = map_view_set_center(view, v->center);

	/* Notify the user about changes */
	if (zoom_changed) {
		/* Invoke user registered event callback */
		map_event_data_h ed =
			_map_view_create_event_data(MAP_EVENT_ACTION);
		if(ed) {
			_map_event_data_set_action_type(ed, MAP_ACTION_ZOOM);
			_map_event_data_set_zoom_factor(ed, v->zoom_factor);
			_map_view_invoke_event_callback(v, ed);
		}
	}

	if (rotation_changed) {
		/* Invoke user registered event callback */
		map_event_data_h ed =
			_map_view_create_event_data(MAP_EVENT_ACTION);
		if(ed) {
			_map_event_data_set_action_type(ed, MAP_ACTION_ROTATE);
			_map_event_data_set_rotation_angle(ed,
							   v->rotation_angle);
			_map_view_invoke_event_callback(v, ed);
		}
	}

	return error;
}

EXPORT_API int map_view_set_zoom_factor(const map_view_h view,
					const double factor)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

#if 1
	map_view_s *v = (map_view_s *) view;
	double new_factor = factor;
	if (new_factor < v->min_zoom_level) new_factor = v->min_zoom_level;
	if (new_factor > v->max_zoom_level) new_factor = v->max_zoom_level;

	return _map_view_set_zoom_rotate(view, true, new_factor, false, .0);
#else
	return _map_view_set_zoom_rotate(view, true, factor, false, .0);
#endif
}

EXPORT_API int map_view_get_zoom_factor(const map_view_h view, double *factor)
{
	MAPS_LOG_API;
	if (!view || !factor)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*factor = v->zoom_factor;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_set_orientation(const map_view_h view,
					const double angle)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Add inertia to the rotation process */
	map_view_s *v = (map_view_s *)view;
	if(v->inertial_camera)
		v->inertial_camera->set_rotation_target(angle);

	return _map_view_set_zoom_rotate(view, false, .0, true, angle);
}

EXPORT_API int map_view_get_orientation(const map_view_h view,
					double *angle)
{
	MAPS_LOG_API;
	if (!view || !angle)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*angle = v->rotation_angle;
	return MAPS_ERROR_NONE;
}


/*----------------------SCREEN <--> GEOGRAPHY---------------------------------*/


/* Converting screen coordinates to geographical */
EXPORT_API int map_view_screen_to_geography(const map_view_h view,
				  const int x, const int y,
				  maps_coordinates_h *coordinates)
{
	MAPS_LOG_API;
	if (!view || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	int posx = 0;
	int posy = 0;
	map_view_get_geometry(view, &posx, &posy, NULL, NULL);
	return __get_plugin_interface(view)->
		maps_plugin_screen_to_geography(x - posx, y - posy,
						coordinates);
}

EXPORT_API int map_view_geography_to_screen(const map_view_h view,
				  const maps_coordinates_h coordinates,
				  int *x, int *y)
{
	MAPS_LOG_API;
	if (!view || !coordinates || !x || !y)
		return MAPS_ERROR_INVALID_PARAMETER;
	int posx = 0;
	int posy = 0;
	map_view_get_geometry(view, &posx, &posy, NULL, NULL);
	const int error = __get_plugin_interface(view)->
			maps_plugin_geography_to_screen(coordinates, x, y);
	*x += posx;
	*y += posy;
	return error;
}


/* --------------------MAPS VIEW PREFERENCES----------------------------------*/


EXPORT_API int map_view_set_type(map_view_h view, const map_view_type_e type)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((type < MAP_VIEW_TYPE_DAY) || (type > MAP_VIEW_TYPE_TERRAIN))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	v->type = type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_get_type(const map_view_h view, map_view_type_e *type)
{
	MAPS_LOG_API;
	if (!view || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*type = v->type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_set_inertia_enabled(map_view_h view, bool enabled)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;

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

EXPORT_API int map_view_get_inertia_enabled(map_view_h view, bool *enabled)
{
	MAPS_LOG_API;
	if (!view || !enabled)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;
	*enabled = v->inertial_camera != NULL;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_set_language(map_view_h view, const char *language)
{
	MAPS_LOG_API;
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

	return maps_set_string(language,
			       _MAPS_VIEW_LANGUAGE_MAX_LENGTH,
			       &((map_view_s *) view)->language);
}

EXPORT_API int map_view_get_language(const map_view_h view, char **language)
{
	MAPS_LOG_API;
	if (!view || !language)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((map_view_s *) view)->language,
		_MAPS_VIEW_LANGUAGE_MAX_LENGTH, language);
}


/* --------------------MAPS PANEL MANIPULATIONS-------------------------------*/


EXPORT_API int map_view_get_port(const map_view_h view, Evas_Object **viewport)
{
	MAPS_LOG_API;
	if (!view || !viewport)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*viewport = v->panel;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_get_geometry(const map_view_h view,
			   int *x, int *y,
			   int *width, int *height)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	evas_object_geometry_get(v->panel, x, y, width, height);
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_resize(const map_view_h view,
				const int width, const int height)
{
	MAPS_LOG_API;
	if (!view || (width <= 0) || (height <= 0))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	evas_object_resize(v->panel, width, height);
	return map_view_set_center(view, v->center);
}

EXPORT_API int map_view_set_visible(const map_view_h view, const bool visible)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	if (visible)
		evas_object_show(v->panel);
	else
		evas_object_hide(v->panel);
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_get_visible(const map_view_h view, bool *visible)
{
	MAPS_LOG_API;
	if (!view || !visible)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*visible = evas_object_visible_get(v->panel) == EINA_TRUE;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_redraw(const map_view_h view)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;

#if 0
	/* render and get the updated rectangles: */
	Eina_List *updates = evas_render_updates(v->canvas);
	evas_render_updates_free(updates);
#endif
	/* Signal to the animator, that we are ready to draw */
	v->ready_to_draw = true;

	return MAPS_ERROR_NONE;
}


/* ---------------------USER CONTROL------------------------------------------*/


EXPORT_API int map_view_set_event_callback(map_view_h view,
					   const map_event_type_e type,
					   map_view_on_event_cb callback,
					   void *user_data)
{
	MAPS_LOG_API;
	if (!view || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((type < MAP_EVENT_GESTURE) || (type > MAP_EVENT_READY))
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;
	v->event_callbacks[type].callback = callback;
	v->event_callbacks[type].user_data = user_data;

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_unset_event_callback(map_view_h view,
					     const map_event_type_e type)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((type < MAP_EVENT_GESTURE) || (type > MAP_EVENT_READY))
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;
	v->event_callbacks[type].callback = __map_view_on_event_empty_cb;
	v->event_callbacks[type].user_data = NULL;

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_set_gesture_action(map_view_h view,
					   const map_gesture_e gesture,
					   const map_action_e action)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((gesture < MAP_GESTURE_SCROLL) || (gesture > MAP_GESTURE_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	if((action < MAP_ACTION_SCROLL) || (action > MAP_ACTION_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	v->gesture_actions[gesture] = action;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_get_gesture_action(map_view_h view,
					    const map_gesture_e gesture,
					    map_action_e *action)
{
	MAPS_LOG_API;
	if (!view || !action)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((gesture < MAP_GESTURE_SCROLL) || (gesture > MAP_GESTURE_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*action = v->gesture_actions[gesture];
	return MAPS_ERROR_NONE;
}


EXPORT_API int map_view_set_gesture_enabled(map_view_h view,
					    const map_gesture_e gesture,
					    const bool enabled)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((gesture < MAP_GESTURE_SCROLL) || (gesture > MAP_GESTURE_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	v->gesture_available[gesture] = enabled;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_get_gesture_enabled(map_view_h view,
					    const map_gesture_e gesture,
					    bool *enabled)
{
	MAPS_LOG_API;
	if (!view || !enabled)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((gesture < MAP_GESTURE_SCROLL) || (gesture > MAP_GESTURE_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*enabled = v->gesture_available[gesture];
	return MAPS_ERROR_NONE;
}


/* ---------------------VISUAL OBJECTS ON THE MAP-----------------------------*/

EXPORT_API int map_view_add_object(map_view_h view, map_object_h object)
{
	MAPS_LOG_API;
	if (!view || !object)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		/* Add Visual Object to the list of View Visual Objects */
		map_view_s *v = (map_view_s *)view;
		error = maps_item_list_append(v->view_objects,
					      object,
					      maps_item_no_clone);
		if(error != MAPS_ERROR_NONE)
			break;

		/* Link the object with the View */
		error = _map_object_set_view(object, view);
		if(error != MAPS_ERROR_NONE)
			break;

		/* Notify the plugin about added object */
		error = _map_view_on_object_operation(view,
						      object,
						      MAP_OBJECT_ADD);
		if(error != MAPS_ERROR_NONE)
			break;

		/* Redraw the view */
		error = map_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while(false);

	return error;
}

EXPORT_API int map_view_remove_object(map_view_h view,
				      const map_object_h object)
{
	MAPS_LOG_API;
	if (!view || !object)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		/* Remove Visual Object from the list of View Visual Objects */
		map_view_s *v = (map_view_s *)view;
		error =  maps_item_list_remove(v->view_objects,
						   object,
						   map_object_destroy);
		if(error != MAPS_ERROR_NONE)
			return error;

#if 0
		/* Notify the plugin about added object */
		error = _map_view_on_object_operation(view,
						      object,
						      MAP_OBJECT_REMOVE);
		if(error != MAPS_ERROR_NONE)
			break;
#endif

		/* Redraw the view */
		error = map_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			return error;

		return MAPS_ERROR_NONE;
	} while(false);

	return error;
}

EXPORT_API int map_view_remove_all_objects(map_view_h view)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		map_view_s *v = (map_view_s *)view;
		error = maps_item_list_remove_all(v->view_objects,
						      map_object_destroy);
		if(error != MAPS_ERROR_NONE)
			return error;

		/* Notify the plugin view, that all objects are removed */
		_map_view_on_object_operation(view,
					      NULL,
					      MAP_OBJECT_REMOVE_ALL);
		if(error != MAPS_ERROR_NONE)
			break;

		/* Redraw the view */
		error = map_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			return error;

		return MAPS_ERROR_NONE;
	} while(false);

	return error;
}

EXPORT_API int map_view_foreach_object(const map_view_h view,
					map_object_cb callback,
					void *user_data)
{
	MAPS_LOG_API;
	if (!view || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *)view;
	return maps_item_list_foreach(v->view_objects,
				      maps_item_no_clone,
				      callback, user_data);
}


/* ----------------------SEND MAP VIEW CALLBACK TO THE USER-------------------*/

/* TODO: Consider refactoring of send Map View Callbacks to the User */

map_event_data_h _map_view_create_event_data(map_event_type_e type)
{
	map_event_data_h event_data = NULL;
	const int error = _map_event_data_create(&event_data);
	if(error != MAPS_ERROR_NONE) {
		map_event_data_destroy(event_data);
		return NULL;
	}
	if(!event_data)
		return NULL;
	_map_event_data_set_type(event_data, type);
	return event_data;
}

void _map_view_invoke_event_callback(map_view_h view,
				     map_event_data_h event_data)
{
	if(!view || !event_data)
		return;

	map_view_s *v = (map_view_s *)view;

	map_event_type_e type = MAP_EVENT_GESTURE;
	map_event_data_get_type(event_data, &type);

	v->event_callbacks[type].callback(MAPS_ERROR_NONE, type, event_data,
					  v->event_callbacks[type].user_data);
}

/* ----------------------HIT TEST---------------------------------------------*/
/* TODO: consider refactoring of Hit Test features and separation as
 *  dedicated classes */

typedef struct _map_view_hit_test_data_s {
	map_view_s *v;
	int x;
	int y;
	map_object_h object;
} map_view_hit_test_data_s;


typedef struct _map_view_collect_poly_object_point_s {
	map_view_s *v;
	view::poly_shape_hit_test *pd;
} map_view_collect_poly_object_point_s;


static bool __map_object_poly_collect_points_cb(int index,
						     maps_coordinates_h point,
						     void *user_data)
{
	if(!point || !user_data)
		return false;

	map_view_collect_poly_object_point_s *cpop =
		(map_view_collect_poly_object_point_s *)user_data;

	int x = 0;
	int y = 0;
	if(map_view_geography_to_screen(cpop->v, point, &x, &y) ==
	   MAPS_ERROR_NONE)
		cpop->pd->add_point(float(x), float(y));

	maps_coordinates_destroy(point);

	return true;
}

static bool __map_view_hit_test_cb(int index, int total,
				   map_object_h object,
				   void *user_data)
{
	if(!object || !user_data)
		return false;

	#ifdef IMPROVEMENT_OF_VISUAL_OBJECTS
	/* If it is an unvisible object, skip this hit-testing. */
	bool visible = false;
	map_object_get_visible(object, &visible);
	if (!visible) return true;
	#endif

	map_view_hit_test_data_s *htd = (map_view_hit_test_data_s *)user_data;

	map_object_type_e type = MAP_OBJECT_UNKNOWN;
	map_object_get_type(object, &type);
	switch(type) {
		case MAP_OBJECT_GROUP: {
			map_object_group_foreach_object(object,
							__map_view_hit_test_cb,
							htd);
			#ifndef IMPROVEMENT_OF_VISUAL_OBJECTS
			/* BLOCK : If one of objects in the group, the actual object should be returned. */
			if(htd->object)
				htd->object = object;
			#endif
			break;
		}
		case MAP_OBJECT_POLYLINE: {
			view::poly_shape_hit_test pd;
			map_view_collect_poly_object_point_s cpop = {htd->v,
									&pd};
			int error = map_object_polyline_foreach_point(object,
				__map_object_poly_collect_points_cb,
				&cpop);
			if(error != MAPS_ERROR_NONE)
				break;

			if(pd.hit_test(float(htd->x), float(htd->y), false))
				htd->object = object;

			break;
		}

		case MAP_OBJECT_POLYGON: {
			view::poly_shape_hit_test pd;
			map_view_collect_poly_object_point_s cpop = {htd->v,
									&pd};
			int error = map_object_polygon_foreach_point(object,
				__map_object_poly_collect_points_cb,
				&cpop);
			if(error != MAPS_ERROR_NONE)
				break;

			if(pd.hit_test(float(htd->x), float(htd->y), true))
				htd->object = object;

			break;
		}
		case MAP_OBJECT_MARKER: {
			maps_coordinates_h c = NULL;
			map_object_marker_get_coordinates(object, &c);

			int x = 0;
			int y = 0;
			map_view_geography_to_screen(htd->v, c, &x, &y);
			maps_coordinates_destroy(c);

			int w = 0;
			int h = 0;
			map_object_marker_get_size(object, &w, &h);

			#ifdef IMPROVEMENT_OF_VISUAL_OBJECTS
			/* In case of PIN marker type, rearrange the hit-area. */
			map_marker_type_e marker_type;
			map_object_marker_get_type(object, &marker_type);
			if (marker_type == MAP_MARKER_PIN)
				y -= h / 2;

			/* Add some margin of the hit-area. */
			w += 20;
			h += 20;
			#endif

			if((x > (htd->x - w)) && (x < (htd->x + w))
			   && (y > (htd->y - h)) && (y < (htd->y + h))) {
				htd->object = object;
			}
			break;
		}
#ifdef TIZEN_3_0_NEXT_MS
		case MAP_OBJECT_ROUTE: {
			/* TODO: Implement Hit Test for route */
			break;
		}
#endif /* TIZEN_3_0_NEXT_MS */
		default:
			break;
	}

	if(htd->object)
		return false;

	return true;
}


map_object_h _map_object_hit_test(map_view_h view,
					    const int x,
					    const int y,
					    map_gesture_e gesture)
{
	if (!view)
		return NULL;

	map_view_s *v = (map_view_s *)view;

	/* Check if we are going to inform anyone about object events */
	if (v->event_callbacks[MAP_EVENT_OBJECT].callback
		== __map_view_on_event_empty_cb)
		return NULL;

	/* 1. Iterate through the list of object and test the hit */
	map_view_hit_test_data_s htd = {v, x, y, NULL};
	int error = map_view_foreach_object(view, __map_view_hit_test_cb, &htd);
	if ((error != MAPS_ERROR_NONE) || !htd.object)
		return NULL;

	/*g_print("Hit Test: FOUND OBJECT: %p\n", htd.object);*/

	/* Invoke user registered event callback */
	map_event_data_h ed =
		_map_view_create_event_data(MAP_EVENT_OBJECT);
	if (ed) {
		_map_event_data_set_object(ed, htd.object);
		_map_event_data_set_gesture_type(ed, gesture);
		_map_view_invoke_event_callback(view, ed);
	}

	/* 2. Extract test result */
	return htd.object;
}
