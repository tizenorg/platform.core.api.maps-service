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
#include <app.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <unistd.h>
#include <Elementary.h>
#include <glib.h>

#include "map_view.h"
#include "maps_util.h"
#include "module.h"
#include "empty_module.h"
#include "commands.h"
#include "command_queue.h"
#include "poly_shape_hit_test.h"
#include "marker_constructor.h"
#include "polyline_constructor.h"
#include "polygon_constructor.h"
#include "route_constructor.h"
#include "gesture_processor.h"


/* TODO: remove useless or duplicative includes */


/*
 * The structure of callbacks info, Maps View is invoking during events
 */
typedef struct _map_view_callback_info_s {
	map_view_on_event_cb callback;
	void *user_data;
} map_view_callback_info_s;

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
	double tilt; /* in graduses */
	int zoom_level;

	/* We'd better to store that values in order to easily limit zoom level
	 * and factor in the run time without accessing the plugin frequently */
	int min_zoom_level;
	int max_zoom_level;

	int min_tilt;
	int max_tilt;

	/* The handle of Maps Service, used by Map View */
	maps_service_h maps;

	/* List of Visual Objects on the View */
	maps_item_list_h view_objects;

	/* Evas basis */
	Evas *canvas;
	Evas_Object *parent;
	Evas_Object *panel;

	/* Gesture Support */
	map_view_action_e gesture_actions[MAP_GESTURE_NONE + 1];
	bool gesture_available[MAP_GESTURE_NONE + 1];

	/* Continuous gesture info */
	view::gesture_detector *gest_detect;

	/* Map View callbacks */
	map_view_callback_info_s event_callbacks[MAP_VIEW_EVENT_READY + 1];

	/* Evas Queue support */
	Ecore_Idler *idler;
	Ecore_Animator *animator;
	volatile bool ready_to_draw;

	/* Map View Preferences */
	map_view_type_e type;
	map_view_mode_e mode;
	char *language;

} map_view_s;


/* ---------------------------------------------------------------------------*/


const gsize _MAPS_VIEW_LANGUAGE_MAX_LENGTH = 16;


/* ---------------------------------------------------------------------------*/


extern plugin::plugin_s *__extract_plugin(maps_service_h maps);


extern int _map_view_event_data_set_type(map_view_event_data_h event,
					 map_view_event_type_e event_type);

extern int _map_view_event_data_set_gesture_type(map_view_event_data_h event,
					 const map_view_gesture_e gesture_type);

extern int _map_view_event_data_set_action_type(map_view_event_data_h event,
					 const map_view_action_e action_type);

extern int _map_view_event_data_set_center(map_view_event_data_h event,
					   const maps_coordinates_h center);

extern int _map_view_event_data_set_delta(map_view_event_data_h event,
					  const int delta_x, const int delta_y);

extern int _map_view_event_data_set_xy(map_view_event_data_h event,
				       const int x, const int y);

extern int _map_view_event_data_set_fingers(map_view_event_data_h event,
					    const int fingers);

extern int _map_view_event_data_set_zoom_factor(map_view_event_data_h event,
						const double zoom_factor);

extern int _map_view_event_data_set_rotation_angle(map_view_event_data_h event,
						   const double rotation_angle);

extern int _map_view_event_data_set_tilt(map_view_event_data_h event,
					 const double tilt);

extern int _map_view_event_data_set_object(map_view_event_data_h event,
					   map_view_object_h object);


extern int _map_view_object_create(map_view_h view,
				   const map_view_object_type_e type,
				   map_view_object_h *object);

extern int _map_view_object_destroy(map_view_object_h object);

extern int _map_view_event_data_create(map_view_event_data_h *event);

map_view_event_data_h _map_view_create_event_data(map_view_event_type_e type);

void _map_view_invoke_event_callback(map_view_h view,
				     map_view_event_data_h event_data);


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
					 const map_view_event_type_e type,
					 map_view_event_data_h event_data,
					 void *user_data)
{
	/* empty */
}

static session::command_queue *q()
{
	return session::command_queue_view::interface();
}

bool _map_viwe_is_gesture_available(map_view_h view,
				    map_view_gesture_e gesture)
{
	if(!view)
		return false;
	map_view_s *v = (map_view_s *)view;
	return v->gesture_available[gesture];
}

map_view_action_e _map_view_get_gesture_action(map_view_h view,
					       map_view_gesture_e gesture)
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
				  const map_view_object_h object,
				  map_view_object_operation_e operation)
{
	if(!view)
		return  MAPS_ERROR_INVALID_PARAMETER;

	if(!__get_plugin_interface(view)->maps_plugin_on_view_object)
		return  MAPS_ERROR_INVALID_PARAMETER;

	return __get_plugin_interface(view)->maps_plugin_on_view_object(object,
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
	if(!event_info || !data)
		return;

	/* Extract the view ptr */
	map_view_s *v = (map_view_s *)data;

	/* Extract gesture info */
	Evas_Event_Mouse_Down *ev = (Evas_Event_Mouse_Down *)event_info;
	const int x = ev->canvas.x;
	const int y = ev->canvas.y;

	/* Detect & Process the gesture */
	if(v->gest_detect)
		v->gest_detect->tap(0,
				     view::touch_point(x, y, ev->timestamp));
}

static void __on_canvas_up(void *data,
			   Evas *e,
			   Evas_Object *obj,
			   void *event_info)
{
	if(!event_info || !data)
		return;

	/* Extract the view ptr */
	map_view_s *v = (map_view_s *)data;

	/* Extract gesture info */
	Evas_Event_Mouse_Up *ev = (Evas_Event_Mouse_Up *)event_info;
	const int x = ev->canvas.x;
	const int y = ev->canvas.y;

	/* Detect & Process the gesture */
	if(v->gest_detect)
		v->gest_detect->up(0, view::touch_point(x, y, ev->timestamp));
}


static void __on_canvas_line(void *data,
			     Evas *e,
			     Evas_Object *obj,
			     void *event_info)
{
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	map_view_s *v = (map_view_s *)data;

	/* Extract gesture info */
	Evas_Event_Mouse_Move *ev = (Evas_Event_Mouse_Move *)event_info;
	const int x = ev->cur.canvas.x;
	const int y = ev->cur.canvas.y;

	/* Detect & Process the gesture */
	if(v->gest_detect)
		v->gest_detect->move(0,
				      view::touch_point(x, y, ev->timestamp));
}

static void __on_canvas_multi_tap(void *data,
				  Evas *e,
				  Evas_Object *obj,
				  void *event_info)
{
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	map_view_s *v = (map_view_s *)data;

	/* Extract gesture info */
	Evas_Event_Multi_Down *ev = (Evas_Event_Multi_Down *)event_info;
	const int x = ev->canvas.x;
	const int y = ev->canvas.y;

	/* Detect & Process the gesture */
	if(v->gest_detect)
		v->gest_detect->tap(ev->device,
				     view::touch_point(x, y, ev->timestamp));
}


static void __on_canvas_multi_up(void *data,
				  Evas *e,
				  Evas_Object *obj,
				  void *event_info)
{
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	map_view_s *v = (map_view_s *)data;

	/* Extract gesture info */
	Evas_Event_Multi_Up *ev = (Evas_Event_Multi_Up *)event_info;
	const int x = ev->canvas.x;
	const int y = ev->canvas.y;

	/* Detect & Process the gesture */
	if(v->gest_detect)
		v->gest_detect->up(ev->device,
				    view::touch_point(x, y, ev->timestamp));
}


static void __on_canvas_multi_line(void *data,
				  Evas *e,
				  Evas_Object *obj,
				  void *event_info)
{
	if(!event_info || !data)
		return;

	/*  Extract view ptr */
	map_view_s *v = (map_view_s *)data;

	Evas_Event_Multi_Move *ev = (Evas_Event_Multi_Move *)event_info;
	const int x = ev->cur.canvas.x;
	const int y = ev->cur.canvas.y;

	/* Detect & Process the gesture */
	if(v->gest_detect)
		v->gest_detect->move(ev->device,
				      view::touch_point(x, y, ev->timestamp));
}

static Eina_Bool __map_view_on_idle_cb(void *data)
{
	if (!data)
		return ECORE_CALLBACK_RENEW;

	map_view_s *v = (map_view_s *)data;

	/* Extract and run the on-going command */
	q()->process(__extract_plugin(v->maps));

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
			_map_view_create_event_data(MAP_VIEW_EVENT_READY));

	return MAPS_ERROR_NONE;
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


/* ----------------------CREATE AND DESTROY-----------------------------------*/


/* Create the panel and link it to the instance of Maps Service */
EXPORT_API int map_view_create(const maps_service_h maps,
				Evas_Object *parent,
				map_view_h *view)
{
	MAPS_LOG_API;
	if (!maps || !parent || !view)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Resizing the drawing area accordingly to window size */
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	evas_object_geometry_get(parent, &x, &y, &w, &h);
	/*g_print("Parents geometry: x=%d, y=%d, w=%d, h=%d\n", x, y, w, h);*/

	/* Check if the parent has valid size */
	if ((x < 0) || (y < 0) || (w <= 0) || (h <= 0))
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = g_slice_new0(map_view_s);
	if (!v) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	/* Initialize the layers with visual objects */
	maps_item_list_create(&v->view_objects);

	/* Set up parent */
	v->parent = parent;

	/* Create a visual panel to draw everything about the maps */
	Evas *parent_canvas = evas_object_evas_get(parent);

	/*g_print("Parent Canvas: %p\n", parent_canvas);*/
	if (!parent_canvas)
		return MAPS_ERROR_INVALID_PARAMETER;
	v->panel = evas_object_image_filled_add(parent_canvas);
	/*g_print("Panel: %p\n", v->panel);*/
	if (!v->panel)
		return MAPS_ERROR_INVALID_PARAMETER;

	evas_object_move(v->panel, x, y);
	evas_object_resize(v->panel, w, h);
	evas_object_image_size_set(v->panel, w, h);
	evas_object_show(v->panel);

	/* Set up event callbacks by default */
	const int event_callback_cnt =
		sizeof(v->event_callbacks) / sizeof(v->event_callbacks[0]);
	for(int i = 0; i < event_callback_cnt; i ++) {
		v->event_callbacks[i].callback = __map_view_on_event_empty_cb;
		v->event_callbacks[i].user_data = NULL;
	}


	/* Assign gestures to actions */
	v->gesture_actions[MAP_GESTURE_PAN] = MAP_ACTION_SET_CENTER;
	v->gesture_actions[MAP_GESTURE_FLICK] = MAP_ACTION_SET_CENTER;
	v->gesture_actions[MAP_GESTURE_PINCH] = MAP_ACTION_SET_CENTER;
	v->gesture_actions[MAP_GESTURE_TAP] = MAP_ACTION_SET_CENTER;
	v->gesture_actions[MAP_GESTURE_DOUBLE_TAP] = MAP_ACTION_ZOOM;
	v->gesture_actions[MAP_GESTURE_2_FINGER_TAP] = MAP_ACTION_ZOOM;
	v->gesture_actions[MAP_GESTURE_ZOOM] = MAP_ACTION_ZOOM;
	v->gesture_actions[MAP_GESTURE_ROTATE] = MAP_ACTION_ROTATE;
	v->gesture_actions[MAP_GESTURE_PRESS] = MAP_ACTION_SET_CENTER;
	v->gesture_actions[MAP_GESTURE_NONE] = MAP_ACTION_NONE;

	/* Set up gesture availability */
	v->gesture_available[MAP_GESTURE_PAN] = true;
	v->gesture_available[MAP_GESTURE_FLICK] = true;
	v->gesture_available[MAP_GESTURE_PINCH] = true;
	v->gesture_available[MAP_GESTURE_TAP] = true;
	v->gesture_available[MAP_GESTURE_DOUBLE_TAP] = true;
	v->gesture_available[MAP_GESTURE_2_FINGER_TAP] = true;
	v->gesture_available[MAP_GESTURE_ZOOM] = true;
	v->gesture_available[MAP_GESTURE_ROTATE] = true;
	v->gesture_available[MAP_GESTURE_PRESS] = true;
	v->gesture_available[MAP_GESTURE_NONE] = false;



	/* Gesture Processing */
	v->gest_detect = new view::gesture_detector(v);
	if (!v->gest_detect) {
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
	__get_plugin_interface(v)->maps_plugin_get_min_tilt(
							  &v->min_tilt);
	__get_plugin_interface(v)->maps_plugin_get_max_tilt(
							  &v->max_tilt);

	v->zoom_level = v->min_zoom_level;
	v->zoom_factor = double(v->zoom_level);
	v->rotation_angle = 0.;
	v->tilt = v->min_tilt;
	g_print("Zoom: min = %d, max = %d,  cur level = %d, cur factor = %f\n",
		v->min_zoom_level, v->max_zoom_level,
		v->zoom_level, v->zoom_factor);
	g_print("Tilt: min = %d, maz = %d, cur = %f\n",
		v->min_tilt, v->max_tilt, v->tilt);

	g_print("Maps View Created successfully\n");
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
	if(v->gest_detect)
		delete v->gest_detect;
	v->gest_detect = NULL;

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

	/* Destroy the layers with visual objects */
	maps_item_list_remove_all(v->view_objects, _map_view_object_destroy);
	maps_item_list_destroy(v->view_objects);

	if (v->language)
		g_free(v->language);

	g_slice_free(map_view_s, v);

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

	double tilt = 0.;
	error = map_view_get_tilt(view, &tilt);
	if (error != MAPS_ERROR_NONE)
		return error;

	map_view_s *v = (map_view_s *) view;
	if(v->center != coordinates) {
		if (v->center)
			maps_coordinates_destroy(v->center);
		maps_coordinates_clone(coordinates, &v->center);
	}


	/* TODO: Implement it with command and command queue,
	*  similarly as it is done in maps service */

	/* Rename to maps_plugin_render_map */
	int request_id = 0;
	error = __get_plugin_interface(view)->maps_plugin_render_map(
					v->center,
					zoom_factor,
					rotation_angle,
					tilt,
					__maps_plugin_render_map_cb,
					view,
					&request_id);

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_ACTION);
	if(ed) {
		_map_view_event_data_set_action_type(ed, MAP_ACTION_SET_CENTER);
		_map_view_event_data_set_center(ed, v->center);
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

	/* Rename to maps_plugin_render_map */
	int request_id = 0;
	int error = __get_plugin_interface(view)->maps_plugin_move_center(
						delta_x, delta_y,
						__maps_plugin_render_map_cb,
						view,
						&request_id);

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_ACTION);
	if(ed) {
		_map_view_event_data_set_action_type(ed,
						     MAP_ACTION_MOVE_CENTER);
		_map_view_event_data_set_delta(ed, delta_x, delta_y);
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

	v->zoom_level = level;
	v->zoom_factor = double(level); /* Update the integer  zoom level too */
	const int error = map_view_set_center(view, v->center);

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_ACTION);
	if(ed) {
		_map_view_event_data_set_action_type(ed, MAP_ACTION_ZOOM);
		_map_view_event_data_set_zoom_factor(ed, v->zoom_factor);
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

EXPORT_API int map_view_set_zoom_factor(const map_view_h view,
					const double factor)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;

	/* TODO: consider more convenient for App Devs approach:
	 *    factor = min(factor, max_zoom);
	 *    factor = max(factor, min_zoom);
	 *    error = NONE
	 */

	/* Check if new zoom factor matches existing zoom range */
	if((factor < v->min_zoom_level) || (factor > v->max_zoom_level))
		return MAPS_ERROR_INVALID_PARAMETER;

	v->zoom_factor = factor;
	v->zoom_level = int(factor); /* Update the integer  zoom level too */
	const int error = map_view_set_center(view, v->center);

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_ACTION);
	if(ed) {
		_map_view_event_data_set_action_type(ed, MAP_ACTION_ZOOM);
		_map_view_event_data_set_zoom_factor(ed, v->zoom_factor);
		_map_view_invoke_event_callback(v, ed);
	}

	return error;
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
	map_view_s *v = (map_view_s *) view;
	v->rotation_angle = angle;
	const int error = map_view_set_center(view, v->center);

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_ACTION);
	if(ed) {
		_map_view_event_data_set_action_type(ed, MAP_ACTION_ROTATE);
		_map_view_event_data_set_rotation_angle(ed, v->rotation_angle);
		_map_view_invoke_event_callback(v, ed);
	}
	return error;
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

EXPORT_API int map_view_set_tilt(map_view_h view, const double tilt)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	if((tilt < v->min_tilt) || (tilt > v->max_tilt))
		return MAPS_ERROR_INVALID_PARAMETER;
	v->tilt = tilt;
	const int error = map_view_set_center(view, v->center);

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_ACTION);
	if(ed) {
		_map_view_event_data_set_action_type(ed, MAP_ACTION_TILT);
		_map_view_event_data_set_tilt(ed, v->tilt);
		_map_view_invoke_event_callback(v, ed);
	}

	return error;
}

EXPORT_API int map_view_get_tilt(const map_view_h view, double *tilt)
{
	MAPS_LOG_API;
	if (!view || !tilt)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*tilt = v->tilt;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_get_min_tilt(const map_view_h view, int *min_tilt)
{
	MAPS_LOG_API;
	if (!view || !min_tilt)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*min_tilt = v->min_tilt;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_get_max_tilt(const map_view_h view, int *max_tilt)
{
	MAPS_LOG_API;
	if (!view || !max_tilt)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*max_tilt = v->max_tilt;
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
	if ((type < MAP_VIEW_TYPE_DAY) || (type > MAP_VIEW_TYPE_INDOOR))
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

EXPORT_API int map_view_set_mode(map_view_h view, const map_view_mode_e mode)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((mode < MAP_VIEW_MODE_2D) || (mode > MAP_VIEW_MODE_STREET_VIEW))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	v->mode = mode;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_get_mode(map_view_h view, map_view_mode_e* mode)
{
	MAPS_LOG_API;
	if (!view || !mode)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*mode = v->mode;
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


EXPORT_API int map_view_get_panel(const map_view_h view, Evas_Object **panel)
{
	MAPS_LOG_API;
	if (!view || !panel)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	*panel = v->panel;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_set_geometry(const map_view_h view,
			   const int x, const int y,
			   const int width, const int height)
{
	MAPS_LOG_API;
	if (!view ||(x < 0) || (y < 0) || (width <= 0) || (height <= 0))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	evas_object_geometry_set(v->panel, x, y, width, height);
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
					   const map_view_event_type_e type,
					   map_view_on_event_cb callback,
					   void *user_data)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((type < MAP_VIEW_EVENT_GESTURE) || (type > MAP_VIEW_EVENT_READY))
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_s *v = (map_view_s *) view;
	if(callback) {
		v->event_callbacks[type].callback = callback;
		v->event_callbacks[type].user_data = user_data;
	} else {
		v->event_callbacks[type].callback =
			__map_view_on_event_empty_cb;
		v->event_callbacks[type].user_data = NULL;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_set_gesture_action(map_view_h view,
					    const map_view_gesture_e gesture,
					    const map_view_action_e action)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((gesture < MAP_GESTURE_PAN) || (gesture > MAP_GESTURE_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	if((action < MAP_ACTION_SET_CENTER) || (action > MAP_ACTION_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	v->gesture_actions[gesture] = action;
	v->gesture_available[gesture] = true;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_view_disable_gesture(map_view_h view,
					 const map_view_gesture_e gesture)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((gesture < MAP_GESTURE_PAN) || (gesture > MAP_GESTURE_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *) view;
	v->gesture_available[gesture] = false;
	return MAPS_ERROR_NONE;
}


/* ---------------------VISUAL OBJECTS ON THE MAP-----------------------------*/


EXPORT_API int map_view_add_marker(map_view_h view,
			const maps_coordinates_h coordinates,
			const char *image_file_path,
			const map_view_marker_type_e type,
			map_view_object_h *marker)
{
	MAPS_LOG_API;
	if (!view || !coordinates || !marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((type < MAP_VIEW_MARKER_START) || (type > MAP_VIEW_MARKER_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		/* Create a Marker Visual Object */
		view::marker_constructor mc;
		*marker = mc.construct(view,
				       coordinates,
				       image_file_path,
				       type);
		error = mc.get_error();
		if(error != MAPS_ERROR_NONE)
			break;

		/* Add Visual Object to the list of View Visual Objects */
		map_view_s *v = (map_view_s *)view;
		error = maps_item_list_append(v->view_objects,
					      *marker,
					      maps_item_no_clone);
		if(error != MAPS_ERROR_NONE)
			break;

		error = map_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while(false);

	_map_view_object_destroy(*marker);
	*marker = NULL;
	return error;
}

EXPORT_API int map_view_add_route(map_view_h view,
		       const maps_route_h route,
		       map_view_object_h *route_object)
{
	MAPS_LOG_API;
	if (!view || !route|| !route_object)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		/* Create a Route Visual Object */
		view::route_constructor rc;
		*route_object = rc.construct(view, route);
		error = rc.get_error();
		if(error != MAPS_ERROR_NONE)
			break;

		/* Add Visual Object to the list of View Visual Objects */
		map_view_s *v = (map_view_s *)view;
		error = maps_item_list_append(v->view_objects,
					      *route_object,
					      maps_item_no_clone);
		if(error != MAPS_ERROR_NONE)
			break;

		error = map_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while(false);

	_map_view_object_destroy(*route_object);
	*route_object = NULL;
	return error;
}

EXPORT_API int map_view_add_polyline(map_view_h view,
				     const maps_item_list_h coordinates,
				     const unsigned char r,
				     const unsigned char g,
				     const unsigned char b,
				     const unsigned char a,
				     const int width,
				     map_view_object_h *polyline)
{
	MAPS_LOG_API;
	if (!view || !polyline)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		/* Create a Polyline Visual Object */
		view::polyline_constructor pc;
		*polyline = pc.construct(view, coordinates, r, g, b, a, width);
		error = pc.get_error();
		if(error != MAPS_ERROR_NONE)
			break;

		/* Add Visual Object to the list of View Visual Objects */
		map_view_s *v = (map_view_s *)view;
		error = maps_item_list_append(v->view_objects,
					      *polyline,
					      maps_item_no_clone);
		if(error != MAPS_ERROR_NONE)
			break;

		error = map_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while(false);

	_map_view_object_destroy(*polyline);
	*polyline = NULL;
	return error;
}

EXPORT_API int map_view_add_polygon(map_view_h view,
				    const maps_item_list_h coordinates,
				    const unsigned char r,
				    const unsigned char g,
				    const unsigned char b,
				    const unsigned char a,
				    map_view_object_h *polygon)
{
	MAPS_LOG_API;
	if (!view || !polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		/* Create a Polygon Visual Object */
		view::polygon_constructor pc;
		*polygon = pc.construct(view, coordinates, r, g, b, a);
		error = pc.get_error();
		if(error != MAPS_ERROR_NONE)
			break;

		/* Add Visual Object to the list of View Visual Objects */
		map_view_s *v = (map_view_s *)view;
		error = maps_item_list_append(v->view_objects,
					      *polygon,
					      maps_item_no_clone);
		if(error != MAPS_ERROR_NONE)
			break;

		error = map_view_redraw(v);
		if(error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while(false);

	_map_view_object_destroy(*polygon);
	*polygon = NULL;
	return error;
}

EXPORT_API int map_view_remove_object(map_view_h view,
				       const map_view_object_h object)
{
	MAPS_LOG_API;
	if (!view || !object)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *)view;
	int error =  maps_item_list_remove(v->view_objects,
				     object,
				     _map_view_object_destroy);
	if(error != MAPS_ERROR_NONE)
		return error;

	return map_view_redraw(v);
}

EXPORT_API int map_view_remove_all_objects(map_view_h view)
{
	MAPS_LOG_API;
	if (!view)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_s *v = (map_view_s *)view;
	int error = maps_item_list_remove_all(v->view_objects,
				     _map_view_object_destroy);
	if(error != MAPS_ERROR_NONE)
		return error;

	/* Notify view, that all objects are removed */
	_map_view_on_object_operation(view, NULL, MAP_VIEW_OBJECT_REMOVE_ALL);

	return map_view_redraw(v);
}

EXPORT_API int map_view_foreach_object(const map_view_h view,
					map_view_object_cb callback,
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

map_view_event_data_h _map_view_create_event_data(map_view_event_type_e type)
{
	map_view_event_data_h event_data = NULL;
	const int error = _map_view_event_data_create(&event_data);
	if(error != MAPS_ERROR_NONE) {
		map_view_event_data_destroy(event_data);
		return NULL;
	}
	if(!event_data)
		return NULL;
	_map_view_event_data_set_type(event_data, type);
	return event_data;
}

void _map_view_invoke_event_callback(map_view_h view,
				     map_view_event_data_h event_data)
{
	if(!view || !event_data)
		return;

	map_view_s *v = (map_view_s *)view;

	map_view_event_type_e type = MAP_VIEW_EVENT_GESTURE;
	map_view_event_data_get_type(event_data, &type);

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
	map_view_object_h object;
} map_view_hit_test_data_s;


typedef struct _map_view_collect_poly_object_point_s {
	map_view_s *v;
	view::poly_shape_hit_test *pd;
} map_view_collect_poly_object_point_s;


static bool __map_view_object_poly_collect_points_cb(int index, int total,
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
				   map_view_object_h object,
				   void *user_data)
{
	if(!object || !user_data)
		return false;

	map_view_hit_test_data_s *htd = (map_view_hit_test_data_s *)user_data;

	map_view_object_type_e type = MAP_VIEW_OBJECT_UNKNOWN;
	map_view_object_get_type(object, &type);
	switch(type) {
		case MAP_VIEW_OBJECT_LAYER: {
			map_view_object_layer_foreach_object(object,
							__map_view_hit_test_cb,
							htd);
			if(htd->object)
				htd->object = object;
			break;
		}
		case MAP_VIEW_OBJECT_POLYLINE:
		case MAP_VIEW_OBJECT_POLYGON: {
			const bool polygon = type == MAP_VIEW_OBJECT_POLYGON;

			view::poly_shape_hit_test pd;
			map_view_collect_poly_object_point_s cpop = {htd->v,
									&pd};
			int error = map_view_object_poly_foreach_point(object,
				__map_view_object_poly_collect_points_cb,
				&cpop);
			if(error != MAPS_ERROR_NONE)
				break;

			if(pd.hit_test(float(htd->x), float(htd->y), polygon))
				htd->object = object;

			break;
		}
		case MAP_VIEW_OBJECT_MARKER: {
			maps_coordinates_h c = NULL;
			map_view_object_marker_get_coordinates(object, &c);

			int x = 0;
			int y = 0;
			map_view_geography_to_screen(htd->v, c, &x, &y);
			maps_coordinates_destroy(c);

			int w = 0;
			int h = 0;
			map_view_object_marker_get_screen_size(object, &w, &h);
			if((x > (htd->x - w)) && (x < (htd->x + w))
			   && (y > (htd->y - h)) && (y < (htd->y + h))) {
				htd->object = object;
			}
			break;
		}
		case MAP_VIEW_OBJECT_ROUTE: {
			/* TODO: Implement Hit Test for route */
			break;
		}
		default:
			break;
	}

	if(htd->object)
		return false;

	return true;
}


map_view_object_h _map_view_object_hit_test(map_view_h view,
					    const int x,
					    const int y,
					    map_view_gesture_e gesture)
{
	if(!view)
		return NULL;

	map_view_s *v = (map_view_s *)view;

	/* Check if we are going to inform anyone about object events */
	if(v->event_callbacks[MAP_VIEW_EVENT_OBJECT].callback
	   == __map_view_on_event_empty_cb)
		return NULL;


	/* 1. Iterate through the list of object and test the hit */
	map_view_hit_test_data_s htd = {v, x, y, NULL};
	int error = map_view_foreach_object(view, __map_view_hit_test_cb, &htd);
	if((error != MAPS_ERROR_NONE) || !htd.object)
		return NULL;

	/*g_print("Hit Test: FOUND OBJECT: %p\n", htd.object);*/

	/* Invoke user registered event callback */
	map_view_event_data_h ed =
		_map_view_create_event_data(MAP_VIEW_EVENT_OBJECT);
	if(ed) {
		_map_view_event_data_set_object(ed, htd.object);
		_map_view_event_data_set_gesture_type(ed, gesture);
		_map_view_invoke_event_callback(view, ed);
	}

	/* 2. Extract test result */
	return htd.object;
}
