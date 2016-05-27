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
#include "maps_view_object.h"
#include "maps_view_object_plugin.h"
#include "maps_error.h"
#include "maps_util.h"
#include "maps_view.h"
#include "maps_extra_types.h"
#include "marker_constructor.h"
#include "polyline_constructor.h"
#include "polygon_constructor.h"
#include "overlay_constructor.h"
#include <cairo.h>

/*
* This represents marker visual object information
 */
typedef struct _maps_view_marker_data_s {
	maps_coordinates_h coordinates;
	int width;
	int height;
	char *file_path;
	maps_view_marker_type_e type;
	int z_order;
} maps_view_marker_data_s;

const gsize _MAPS_VIEW_MARKER_FILE_PATH_MAX_LENGTH = 128;

/*
* This represents polyline visual object information
 */
typedef struct _maps_view_polyline_data_s {
	/* The list of maps_coordinates_h points, comprising the polyline */
	maps_coordinates_list_h points;

	/* Line color */
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	/* Line width */
	int width;
} maps_view_polyline_data_s;

/*
* This represents polygon visual object information
 */
typedef struct _maps_view_polygon_data_s {
	/* The list of maps_coordinates_h points, comprising the polygon */
	maps_coordinates_list_h points;

	/* The polygon backgraund color */
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} maps_view_polygon_data_s;

typedef struct _maps_view_overlay_data_s {
	maps_coordinates_h coordinates;
	Evas_Object *object;
	int min_zoom_level;
	int max_zoom_level;
	maps_view_overlay_type_e type;
	Evas_Object *bubble;
	Evas_Object *clipper;
} maps_view_overlay_data_s;

const int _OVERLAY_BUBBLE_MARGIN = 20;
const int _OVERLAY_BUBBLE_PIN_SIZE = 20;

/*
* This represents visual object information
 */
typedef struct _maps_view_object_s {
	maps_view_object_type_e type; /* marker, polyline, polygon, group */

	void *shape_data;	/* Pointer to the visual object data, such as
				   maps_view_marker_data_s or
				   maps_view_polyline_data_s */

	bool visible;

	maps_view_object_h parent_group; /* The group, owning the object */

	maps_view_h view;	/* Link with the parent Map View */

} maps_view_object_s;

int _maps_view_on_object_operation(maps_view_h view, maps_view_object_h object, maps_view_object_operation_e operation);
int _maps_view_get_parent(const maps_view_h view, Evas_Object **parent);
int _maps_view_get_clipper(const maps_view_h view, Evas_Object **clipper);

static maps_view_h __get_view(const maps_view_object_h object);
static maps_view_polyline_data_s *__get_polyline_data(const maps_view_object_h object);
static maps_view_polygon_data_s *__get_polygon_data(const maps_view_object_h object);
static maps_view_marker_data_s *__get_marker_data(const maps_view_object_h object);
static maps_view_overlay_data_s *__get_overlay_data(const maps_view_object_h object);

static int __maps_view_polyline_data_create(void **polyline);
static int __maps_view_polygon_data_create(void **polygon);
static int __maps_view_marker_data_create(void **marker);
static int __maps_view_overlay_data_create(void **marker);

static int __maps_view_polyline_data_destroy(void *polyline);
static int __maps_view_polygon_data_destroy(void *polygon);
static int __maps_view_marker_data_destroy(void *marker);
static int __maps_view_overlay_data_destroy(void *marker);

static maps_view_h __get_view(const maps_view_object_h object)
{
	if (!object)
		return NULL;
	maps_view_object_s *o = (maps_view_object_s *)object;
	return o->view;
}

static maps_view_polyline_data_s *__get_polyline_data(const maps_view_object_h object)
{
	maps_view_object_s *o = (maps_view_object_s *)object;
	if (!o || (o->type != MAPS_VIEW_OBJECT_POLYLINE))
		return NULL;
	return (maps_view_polyline_data_s *)o->shape_data;
}

static int __maps_view_polyline_data_create(void **polyline)
{
	if (!polyline)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	maps_view_polyline_data_s *p = NULL;

	do {
		p = g_slice_new0(maps_view_polyline_data_s);
		if (!p) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		error = maps_coordinates_list_create(&p->points);
		if (error != MAPS_ERROR_NONE)
			break;

		*polyline = (maps_view_polyline_data_s *) p;
		return MAPS_ERROR_NONE;
	} while(false);

	__maps_view_polyline_data_destroy(p);
	return error;
}

static int __maps_view_polyline_data_destroy(void *polyline)
{
	if (!polyline)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_polyline_data_s *p = (maps_view_polyline_data_s *)polyline;
	if (p->points)
		maps_coordinates_list_destroy(p->points);

	g_slice_free(maps_view_polyline_data_s, p);

	return MAPS_ERROR_NONE;
}

static maps_view_polygon_data_s *__get_polygon_data(const maps_view_object_h object)
{
	maps_view_object_s *o = (maps_view_object_s *)object;
	if (!o || (o->type != MAPS_VIEW_OBJECT_POLYGON))
		return NULL;
	return (maps_view_polygon_data_s *)o->shape_data;
}

static int __maps_view_polygon_data_create(void **polygon)
{
	if (!polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	maps_view_polygon_data_s *p = NULL;

	do {
		p = g_slice_new0(maps_view_polygon_data_s);
		if (!p) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		error = maps_coordinates_list_create(&p->points);
		if (error != MAPS_ERROR_NONE)
			break;

		*polygon = (maps_view_polygon_data_s *) p;
		return MAPS_ERROR_NONE;
	} while(false);

	__maps_view_polygon_data_destroy(p);
	return error;
}

static int __maps_view_polygon_data_destroy(void *polygon)
{
	if (!polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_polygon_data_s *p = (maps_view_polygon_data_s *)polygon;
	if (p->points)
		maps_coordinates_list_destroy(p->points);

	g_slice_free(maps_view_polygon_data_s, p);

	return MAPS_ERROR_NONE;
}

static maps_view_marker_data_s *__get_marker_data(const maps_view_object_h object)
{
	maps_view_object_s *o = (maps_view_object_s *)object;
	if (!o || (o->type != MAPS_VIEW_OBJECT_MARKER))
		return NULL;
	return (maps_view_marker_data_s *)o->shape_data;
}

static int __maps_view_marker_data_create(void **marker)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;


	int error = MAPS_ERROR_NONE;
	maps_view_marker_data_s *m = NULL;

	do {
		m = g_slice_new0(maps_view_marker_data_s);
		if (!m) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		error = maps_coordinates_create(.0, .0, &m->coordinates);
		if (error != MAPS_ERROR_NONE)
			break;

		m->type = MAPS_VIEW_MARKER_PIN;
		m->width = 0;
		m->height = 0;
		m->z_order = 0;
		*marker = (maps_view_marker_data_s *) m;
		return MAPS_ERROR_NONE;
	} while(false);

	__maps_view_marker_data_destroy(m);
	return error;
}

static int __maps_view_marker_data_destroy(void *marker)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_marker_data_s *m = (maps_view_marker_data_s *)marker;
	if (m->coordinates)
		maps_coordinates_destroy(m->coordinates);

	if (m->file_path)
		g_free(m->file_path);

	g_slice_free(maps_view_marker_data_s, m);

	return MAPS_ERROR_NONE;
}

static maps_view_overlay_data_s *__get_overlay_data(const maps_view_object_h object)
{
	maps_view_object_s *o = (maps_view_object_s *)object;
	if (!o || (o->type != MAPS_VIEW_OBJECT_OVERLAY))
		return NULL;
	return (maps_view_overlay_data_s *)o->shape_data;
}

static int __maps_view_overlay_data_create(void **overlay)
{
	if (!overlay)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	maps_view_overlay_data_s *m = NULL;

	do {
		m = g_slice_new0(maps_view_overlay_data_s);
		if (!m) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		error = maps_coordinates_create(.0, .0, &m->coordinates);
		if (error != MAPS_ERROR_NONE)
			break;

		*overlay = (maps_view_overlay_data_s *) m;
		return MAPS_ERROR_NONE;
	} while(false);

	__maps_view_overlay_data_destroy(m);
	return error;
}

static int __maps_view_overlay_data_destroy(void *overlay)
{
	if (!overlay)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_overlay_data_s *m = (maps_view_overlay_data_s *)overlay;

	if (m->coordinates)
		maps_coordinates_destroy(m->coordinates);

	if (m->object)
		evas_object_del(m->object);

	if (m->bubble)
		evas_object_del(m->bubble);

	if (m->clipper) {
		const Eina_List *clipees = evas_object_clipees_get(m->clipper);
		if (!clipees || eina_list_count(clipees) == 0) {
			evas_object_hide(m->clipper);
			MAPS_LOGD("hide clipper");
		}
	}

	g_slice_free(maps_view_overlay_data_s, m);

	return MAPS_ERROR_NONE;
}

int _maps_view_object_create(maps_view_object_type_e type, maps_view_object_h *object)
{
	if (!object)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (type < MAPS_VIEW_OBJECT_POLYLINE || type > MAPS_VIEW_OBJECT_OVERLAY)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	maps_view_object_s *o = NULL;

	do {
		o = g_slice_new0(maps_view_object_s);
		if (!o) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		o->type = type;
		o->view = NULL;
		o->visible = true;

		int error = MAPS_ERROR_NONE;
		switch(o->type) {
		case MAPS_VIEW_OBJECT_POLYLINE:
			error = __maps_view_polyline_data_create(&o->shape_data);
			break;
		case MAPS_VIEW_OBJECT_POLYGON:
			error = __maps_view_polygon_data_create(&o->shape_data);
			break;
		case MAPS_VIEW_OBJECT_MARKER:
			error = __maps_view_marker_data_create(&o->shape_data);
			break;
		case MAPS_VIEW_OBJECT_OVERLAY:
			error = __maps_view_overlay_data_create(&o->shape_data);
			break;
		default:
			break;
		}

		if ((!o->shape_data) || (error != MAPS_ERROR_NONE))
			break;

		/* Notify view, that the object was constructed */
		/*if (view)
		  _maps_view_on_object_operation(view,
		  o,
		  MAPS_VIEW_OBJECT_ADD);*/

		*object = (maps_view_object_h) o;
		return MAPS_ERROR_NONE;
	} while(false);
	maps_view_object_destroy(o);
	return error;
}

int _maps_view_object_set_view(maps_view_object_h object, maps_view_h view)
{
	if (!object || !view)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_object_s *o = (maps_view_object_s *) object;
	o->view = view;

	maps_view_object_type_e type;
	maps_view_object_get_type(object, &type);
	if (type == MAPS_VIEW_OBJECT_OVERLAY)	{
		int min_zoom = 0, max_zoom = 0;
		maps_view_object_overlay_get_min_zoom_level(object, &min_zoom);
		maps_view_object_overlay_get_max_zoom_level(object, &max_zoom);
		if (min_zoom == 0) {
			maps_view_get_min_zoom_level(view, &min_zoom);
			maps_view_object_overlay_set_min_zoom_level(object, min_zoom);
		}
		if (max_zoom == 0) {
			maps_view_get_max_zoom_level(view, &max_zoom);
			maps_view_object_overlay_set_max_zoom_level(object, max_zoom);
		}
	}
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_create_marker(maps_coordinates_h coordinates,
	const char *image_file_path, maps_view_marker_type_e type, maps_view_object_h *marker)
{
	if (!coordinates || !marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (type < MAPS_VIEW_MARKER_PIN || type > MAPS_VIEW_MARKER_STICKER)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Create a Marker Visual Object */
	view::marker_constructor mc;
	*marker = mc.construct(coordinates, image_file_path, type);
	if (mc.get_error() == MAPS_ERROR_NONE)
		return mc.get_error();

	/* Marker create failure */
	maps_view_object_destroy(*marker);
	*marker = NULL;
	return mc.get_error();
}

EXPORT_API int maps_view_object_create_polyline(maps_coordinates_list_h coordinates,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a, int width,
	maps_view_object_h *polyline)
{
	if (!coordinates || !polyline || width < 1 || width > 100)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Create a Polyline Visual Object */
	view::polyline_constructor pc;
	*polyline = pc.construct(coordinates, r, g, b, a, width);
	if (pc.get_error() == MAPS_ERROR_NONE)
		return pc.get_error();

	/* Polyline create failure */
	maps_view_object_destroy(*polyline);
	*polyline = NULL;
	return pc.get_error();
}

EXPORT_API int maps_view_object_create_polygon(maps_coordinates_list_h coordinates,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a, maps_view_object_h *polygon)
{
	if (!coordinates || !polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Create a Polygon Visual Object */
	view::polygon_constructor pc;
	*polygon = pc.construct(coordinates, r, g, b, a);
	if (pc.get_error() == MAPS_ERROR_NONE)
		return pc.get_error();

	/* Polygon create failure */
	maps_view_object_destroy(*polygon);
	*polygon = NULL;
	return pc.get_error();
}

EXPORT_API int maps_view_object_create_overlay(maps_coordinates_h coordinates,
	Evas_Object *object, maps_view_overlay_type_e type, maps_view_object_h *overlay)
{
	if (!overlay || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (type < MAPS_VIEW_OVERLAY_NORMAL || type > MAPS_VIEW_OVERLAY_BOX)
		return MAPS_ERROR_INVALID_PARAMETER;

	double lat, lon;
	maps_coordinates_get_latitude_longitude(coordinates, &lat, &lon);

	/* Create a Overlay Visual Object */
	view::overlay_constructor pc;
	*overlay = pc.construct(coordinates, object, type);
	if (pc.get_error() == MAPS_ERROR_NONE)
		return pc.get_error();

	/* Overlay create failure */
	maps_view_object_destroy(*overlay);
	*overlay = NULL;
	return pc.get_error();
}

EXPORT_API int maps_view_object_destroy(maps_view_object_h object)
{
	if (!object)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_view_object_s *o = (maps_view_object_s *) object;

	/* Notify view, that the object is to be destroyed shortly */
	_maps_view_on_object_operation(o->view, o, MAPS_VIEW_OBJECT_REMOVE);

	int error = MAPS_ERROR_NONE;
	switch(o->type) {
	case MAPS_VIEW_OBJECT_POLYLINE:
		error = __maps_view_polyline_data_destroy(o->shape_data);
		break;
	case MAPS_VIEW_OBJECT_POLYGON:
		error = __maps_view_polygon_data_destroy(o->shape_data);
		break;
	case MAPS_VIEW_OBJECT_MARKER:
		error = __maps_view_marker_data_destroy(o->shape_data);
		break;
	case MAPS_VIEW_OBJECT_OVERLAY:
		error = __maps_view_overlay_data_destroy(o->shape_data);
		break;
	default:
		break;
	}
	g_slice_free(maps_view_object_s, object);
	return error;
}

EXPORT_API int maps_view_object_get_type(maps_view_object_h object, maps_view_object_type_e *type)
{
	if (!object || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	*type = ((maps_view_object_s *)object)->type;
	return MAPS_ERROR_NONE;
}


/**
* Visual Object Operations
 */

EXPORT_API int maps_view_object_set_visible(maps_view_object_h object, bool visible)
{
	if (!object)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_object_s *o = (maps_view_object_s *)object;
	o->visible = visible;

	/* Notify view, that the object visibility is changed */
	_maps_view_on_object_operation(o->view, o, MAPS_VIEW_OBJECT_SET_VISIBLE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_get_visible(const maps_view_object_h object, bool *visible)
{
	if (!object || !visible)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_object_s *o = (maps_view_object_s *)object;
	*visible = o->visible;
	return MAPS_ERROR_NONE;
}


/*----------------------------------------------------------------------------*/
/*
* Polyline
 */

EXPORT_API int maps_view_object_polyline_set_polyline(maps_view_object_h polyline, maps_coordinates_list_h points)
{
	if (!polyline || !points)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	maps_view_polyline_data_s *p = __get_polyline_data(polyline);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new polyline trajectory */
	maps_coordinates_list_destroy(p->points);
	p->points = points;

	/* Notify view, that the object specific preferences is changed */
	_maps_view_on_object_operation(__get_view(polyline),
				      polyline,
				      MAPS_VIEW_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_polyline_foreach_point(maps_view_object_h polyline,
	maps_coordinates_cb callback, void *user_data)
{
	if (!polyline || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	maps_view_polyline_data_s *p = __get_polyline_data(polyline);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Iterate over polyline trajectory */
	return maps_coordinates_list_foreach(p->points, callback, user_data);
}


EXPORT_API int maps_view_object_polyline_set_color(maps_view_object_h polyline,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if (!polyline)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	maps_view_polyline_data_s *p = __get_polyline_data(polyline);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new color */
	p->r = r;
	p->g = g;
	p->b = b;
	p->a = a;

	/* Notify view, that the object specific preferences is changed */
	_maps_view_on_object_operation(__get_view(polyline),
				      polyline,
				      MAPS_VIEW_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_polyline_get_color(const maps_view_object_h polyline,
	unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a)
{
	if (!polyline || (!r && !g && !b && !a))
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	maps_view_polyline_data_s *p = __get_polyline_data(polyline);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Retrieve the color */
	if (r)
		*r = p->r;
	if (g)
		*g = p->g;
	if (b)
		*b = p->b;
	if (a)
		*a = p->a;
	return MAPS_ERROR_NONE;
}


EXPORT_API int maps_view_object_polyline_set_width(maps_view_object_h polyline, int width)
{
	if (!polyline)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (width < 0 || width > 100)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	maps_view_polyline_data_s *p = __get_polyline_data(polyline);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Update the width of polyline */
	p->width = width;

	/* Notify view, that the object specific preferences is changed */
	_maps_view_on_object_operation(__get_view(polyline),
				      polyline,
				      MAPS_VIEW_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_polyline_get_width(const maps_view_object_h polyline, int *width)
{
	if (!polyline || !width)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	maps_view_polyline_data_s *p = __get_polyline_data(polyline);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Retrieve the width of the polyline */
	*width = p->width;
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/
/*
* Polygon
 */

EXPORT_API int maps_view_object_polygon_set_polygon(maps_view_object_h polygon, maps_coordinates_list_h points)
{
	if (!polygon || !points)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	maps_view_polygon_data_s *p = __get_polygon_data(polygon);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new polygon border */
	maps_coordinates_list_destroy(p->points);
	p->points = points;

	/* Notify view, that the object specific preferences is changed */
	_maps_view_on_object_operation(__get_view(polygon),
				      polygon,
				      MAPS_VIEW_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_polygon_foreach_point(maps_view_object_h polygon,
	maps_coordinates_cb callback, void *user_data)
{
	if (!polygon || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	maps_view_polygon_data_s *p = __get_polygon_data(polygon);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Iterate over polygon border */
	return maps_coordinates_list_foreach(p->points, callback, user_data);
}


EXPORT_API int maps_view_object_polygon_set_fill_color(maps_view_object_h polygon,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if (!polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	maps_view_polygon_data_s *p = __get_polygon_data(polygon);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new background color */
	p->r = r;
	p->g = g;
	p->b = b;
	p->a = a;

	/* Notify view, that the object specific preferences is changed */
	_maps_view_on_object_operation(__get_view(polygon),
				      polygon,
				      MAPS_VIEW_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_polygon_get_fill_color(const maps_view_object_h polygon,
	unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a)
{
	if (!polygon || (!r && !g && !b && !a))
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	maps_view_polygon_data_s *p = __get_polygon_data(polygon);
	if (!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Retrieve the background color */
	if (r)
		*r = p->r;
	if (g)
		*g = p->g;
	if (b)
		*b = p->b;
	if (a)
		*a = p->a;
	return MAPS_ERROR_NONE;
}


/*----------------------------------------------------------------------------*/
/*
* Marker
 */

EXPORT_API int maps_view_object_marker_set_coordinates(maps_view_object_h marker,
	maps_coordinates_h coordinates)
{
	if (!marker || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (m->coordinates)
		maps_coordinates_destroy(m->coordinates);
	m->coordinates = coordinates;

	/* Notify view, that the object specific preferences is changed */
	_maps_view_on_object_operation(__get_view(marker), marker, MAPS_VIEW_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_marker_resize(maps_view_object_h marker, int width, int height)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (width < 0 || height < 0)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->width = width;
	m->height = height;

	/* Notify view, that the object specific preferences is changed */
	_maps_view_on_object_operation(__get_view(marker), marker, MAPS_VIEW_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_marker_set_image_file(maps_view_object_h marker, const char *file_path)
{
	if (!marker || !file_path)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	const int error = maps_set_string(file_path, _MAPS_VIEW_MARKER_FILE_PATH_MAX_LENGTH, &m->file_path);

	/* Notify view, that the object specific preferences is changed */
	if (error == MAPS_ERROR_NONE)
		_maps_view_on_object_operation(__get_view(marker), marker, MAPS_VIEW_OBJECT_CHANGE);

	return error;
}

EXPORT_API int maps_view_object_marker_get_image_file(const maps_view_object_h marker, char **file_path)
{
	if (!marker || !file_path)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(m->file_path, _MAPS_VIEW_MARKER_FILE_PATH_MAX_LENGTH, file_path);
}

EXPORT_API int maps_view_object_marker_get_coordinates(const maps_view_object_h marker, maps_coordinates_h *coordinates)
{
	if (!marker || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_coordinates_clone(m->coordinates, coordinates);
}

EXPORT_API int maps_view_object_marker_set_size(maps_view_object_h marker, int width, int height)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->width = width;
	m->height = height;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_marker_get_size(const maps_view_object_h marker, int *width, int *height)
{
	if (!marker || (!width && !height))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (width)
		*width = m->width;
	if (height)
		*height = m->height;
	return MAPS_ERROR_NONE;
}

int _maps_view_object_marker_set_type(maps_view_object_h marker, maps_view_marker_type_e type)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((type < MAPS_VIEW_MARKER_PIN) || (type > MAPS_VIEW_MARKER_STICKER))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->type = type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_marker_get_type(const maps_view_object_h marker, maps_view_marker_type_e *type)
{
	if (!marker || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	*type = m->type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_marker_set_z_order(maps_view_object_h marker, int z_order)
{
	if (!marker || (z_order > 100 || z_order < -100))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Notify view, that the object specific preferences is changed */
	if (m->z_order != z_order) {
		m->z_order = z_order;
		_maps_view_on_object_operation(__get_view(marker), marker, MAPS_VIEW_OBJECT_CHANGE);
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_marker_get_z_order(const maps_view_object_h marker,	int *z_order)
{
	if (!marker || !z_order)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;

	*z_order = m->z_order;
	return MAPS_ERROR_NONE;
}

int _maps_view_object_overlay_set_type(maps_view_object_h overlay, maps_view_overlay_type_e type)
{
	if (!overlay || (type < MAPS_VIEW_OVERLAY_NORMAL || type > MAPS_VIEW_OVERLAY_BOX))
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->type = type;
	return MAPS_ERROR_NONE;
}

int _maps_view_object_overlay_get_bubble(maps_view_object_h overlay, Evas_Object **object)
{
	if (!overlay || !object)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	*object = m->bubble;
	return MAPS_ERROR_NONE;
}

int _maps_view_object_overlay_set_bubble(maps_view_object_h overlay)
{
	if (!overlay)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;

	int x, y, w, h;
	evas_object_geometry_get(m->object, &x, &y, &w, &h);

	w += _OVERLAY_BUBBLE_MARGIN * 2;
	h += _OVERLAY_BUBBLE_MARGIN * 2 + _OVERLAY_BUBBLE_PIN_SIZE;

	m->bubble = evas_object_image_filled_add(evas_object_evas_get(m->object));
	evas_object_stack_below(m->bubble, m->object);
	evas_object_move(m->bubble, x, y);
	evas_object_resize(m->bubble, w, h);
	evas_object_show(m->bubble);

	unsigned char *pixels;
	int row_stride;
	cairo_t *cairo;
	cairo_surface_t *surface;

	evas_object_image_alpha_set(m->bubble, EINA_TRUE);
	evas_object_image_colorspace_set(m->bubble, EVAS_COLORSPACE_ARGB8888);
	evas_object_image_size_set(m->bubble, w, h);
	evas_object_image_fill_set(m->bubble, 0, 0, w, h);

	pixels = (unsigned char *)evas_object_image_data_get(m->bubble, EINA_TRUE);
	row_stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, w);
	surface = cairo_image_surface_create_for_data(pixels,CAIRO_FORMAT_ARGB32,
												   w, h, row_stride);
	/* Create the cairo context */
	cairo = cairo_create (surface);

	/* Configuration */
	cairo_set_source_rgba(cairo, 0, 0, 0, 0);
	cairo_paint_with_alpha(cairo, 0);

	/* set the size of bubble */
	int bx = x + 2;
	int by = y + 2;
	int bw = w - bx * 2;
	int bh = h - by - _OVERLAY_BUBBLE_PIN_SIZE;

	/* build a path */
	cairo_new_path (cairo);
	if (m->type == MAPS_VIEW_OVERLAY_BUBBLE) {
		double r = bh / 8.;
		double d = M_PI / 180.;
		cairo_arc (cairo, bx + bw - r, by + r, r, -90 * d, 0 * d);
		cairo_arc (cairo, bx + bw - r, by + bh - r, r, 0 * d, 90 * d);
		cairo_line_to(cairo, bx + bw / 2 + _OVERLAY_BUBBLE_PIN_SIZE, by + bh);
		cairo_line_to(cairo, bx + bw / 2, by + bh + _OVERLAY_BUBBLE_PIN_SIZE);
		cairo_line_to(cairo, bx + bw / 2 - _OVERLAY_BUBBLE_PIN_SIZE, by + bh);
		cairo_arc (cairo, bx + r, by + bh - r, r, 90 * d, 180 * d);
		cairo_arc (cairo, bx + r, by + r, r, 180 * d, 270 * d);
	}
	else {
		cairo_move_to(cairo, 1., 1.);
		cairo_line_to(cairo, 1., bh);
		cairo_line_to(cairo, bw / 2 - _OVERLAY_BUBBLE_PIN_SIZE, bh);
		cairo_line_to(cairo, bw / 2, bh + _OVERLAY_BUBBLE_PIN_SIZE);
		cairo_line_to(cairo, bw / 2 + _OVERLAY_BUBBLE_PIN_SIZE, bh);
		cairo_line_to(cairo, bw, bh);
		cairo_line_to(cairo, bw, 1);
	}
	cairo_close_path(cairo);

	/* clipping */
	cairo_clip_preserve(cairo);
	cairo_set_source_rgb(cairo, 1., 1., 1.);
	cairo_fill_preserve(cairo);
	cairo_set_source_rgb(cairo, .1, .1, .1);
	cairo_set_line_width(cairo, 3);


	cairo_stroke(cairo);
	cairo_destroy(cairo);
	cairo_surface_destroy(surface);

	evas_object_image_data_set(m->bubble, pixels);
	evas_object_image_data_update_add(m->bubble, 0, 0, w, h);

	return MAPS_ERROR_NONE;
}

int _maps_view_object_overlay_get_object(maps_view_object_h overlay, Evas_Object **object)
{
	if (!overlay || !object)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	*object = m->object;
	return MAPS_ERROR_NONE;
}

int _maps_view_object_overlay_set_object(maps_view_object_h overlay, Evas_Object *object)
{
	if (!overlay || !object)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->object = object;
	if (m->type != MAPS_VIEW_OVERLAY_NORMAL)
		_maps_view_object_overlay_set_bubble(overlay);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_overlay_set_coordinates(maps_view_object_h overlay, maps_coordinates_h coordinates)
{
	if (!overlay || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (m->coordinates)
		maps_coordinates_destroy(m->coordinates);
	m->coordinates = coordinates;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_overlay_get_coordinates(const maps_view_object_h overlay, maps_coordinates_h *coordinates)
{
	if (!overlay || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_coordinates_clone(m->coordinates, coordinates);
}

EXPORT_API int maps_view_object_overlay_set_min_zoom_level(maps_view_object_h overlay, int zoom)
{
	if (!overlay || zoom < 0)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	int map_min_zoom_level = 0;
	maps_view_get_min_zoom_level(__get_view(overlay), &map_min_zoom_level);
	if (zoom < map_min_zoom_level)
		m->min_zoom_level = map_min_zoom_level;
	else
		m->min_zoom_level = zoom;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_overlay_get_max_zoom_level(const maps_view_object_h overlay, int *zoom)
{
	if (!overlay || !zoom)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	*zoom = m->max_zoom_level;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_overlay_set_max_zoom_level(maps_view_object_h overlay, int zoom)
{
	if (!overlay || zoom < 0)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	int map_max_zoom_level = 0;
	maps_view_get_max_zoom_level(__get_view(overlay), &map_max_zoom_level);
	if (zoom < map_max_zoom_level)
		m->max_zoom_level = map_max_zoom_level;
	else
		m->max_zoom_level = zoom;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_view_object_overlay_get_min_zoom_level(const maps_view_object_h overlay, int *zoom)
{
	if (!overlay || !zoom)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	*zoom = m->min_zoom_level;
	return MAPS_ERROR_NONE;
}

static bool __maps_view_object_overlay_set_visible(maps_view_object_h overlay)
{
	if (!overlay)
		return false;

	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	maps_view_object_s *o = (maps_view_object_s *)overlay;
	if (o->visible) {
		int zoom;
		maps_view_get_zoom_level(__get_view(overlay), &zoom);
		if (zoom < m->min_zoom_level || zoom > m->max_zoom_level) {
			evas_object_hide(m->object);
			if (m->bubble)
				evas_object_hide(m->bubble);
		}
		else {
			evas_object_show(m->object);
			if (m->bubble)
				evas_object_show(m->bubble);
		}
	}
	else {
		evas_object_hide(m->object);
		if (m->bubble)
			evas_object_hide(m->bubble);
	}
	return true;
}

static bool __maps_view_object_overlay_clip(maps_view_object_h overlay, Evas_Object *clipper)
{
	if (!overlay || !clipper)
		return false;

	int x, y, w, h;
	maps_view_get_screen_location(__get_view(overlay), &x, &y, &w, &h);
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	evas_object_color_set(clipper, 255, 255, 255, 255);
	evas_object_move(clipper, x, y);
	evas_object_resize(clipper, w, h);
	evas_object_clip_set(m->object, clipper);
	if (m->bubble)
		evas_object_clip_set(m->bubble, clipper);
	evas_object_show(clipper);

	m->clipper = clipper;
	return true;
}

static bool __maps_view_object_overlay_move(maps_view_object_h overlay, maps_coordinates_h coordinates)
{
	if (!overlay || !coordinates)
		return false;

	int x, y, w, h;
	maps_view_geolocation_to_screen(__get_view(overlay), coordinates, &x, &y);
	maps_view_overlay_data_s *m = __get_overlay_data(overlay);
	evas_object_geometry_get(m->object, NULL, NULL, &w, &h);

	x -= w / 2;
	y -= h;

	if (m->type != MAPS_VIEW_OVERLAY_NORMAL) {
		y -= _OVERLAY_BUBBLE_MARGIN + _OVERLAY_BUBBLE_PIN_SIZE;
		evas_object_move(m->object, x, y);

		if (m->bubble) {
			x -= _OVERLAY_BUBBLE_MARGIN;
			y -= _OVERLAY_BUBBLE_MARGIN ;
			evas_object_move(m->bubble, x, y);
		}
	}
	else
		evas_object_move(m->object, x, y);
	return true;
}

static bool __maps_view_object_overlay_update(maps_view_object_h overlay, Evas_Object *clipper)
{
	if (!overlay || !clipper)
		return false;

	bool ret = false;	
	maps_coordinates_h coordinates = NULL;

	do {
		ret = __maps_view_object_overlay_set_visible(overlay);
		if (!ret) break;

		ret = __maps_view_object_overlay_clip(overlay, clipper);
		if (!ret) break;

		maps_view_object_overlay_get_coordinates(overlay, &coordinates);
		ret = __maps_view_object_overlay_move(overlay, coordinates);
	} while(0);

	if (coordinates)
		maps_coordinates_destroy(coordinates);

	return ret;
}

bool _maps_view_object_overlay_cb(int index, int total, maps_view_object_h object, void *user_data)
{
	if (!object || !user_data)
		return false;

	maps_view_object_type_e type;
	maps_view_object_get_type(object, &type);
	if (type != MAPS_VIEW_OBJECT_OVERLAY)
		return true;
	return __maps_view_object_overlay_update(object, (Evas_Object*)user_data);
}

int _maps_view_object_overlay_operation(maps_view_h view, maps_view_object_h object, maps_view_object_operation_e operation)
{
	if (!view || !object)
		return MAPS_ERROR_INVALID_PARAMETER;

	Evas_Object *parent = NULL, *clipper = NULL;
	maps_view_overlay_data_s *o = __get_overlay_data(object);

	if (!o)
		return MAPS_ERROR_INVALID_PARAMETER;

	if (operation == MAPS_VIEW_OBJECT_ADD) {
		_maps_view_get_parent(view, &parent);
		evas_object_smart_member_add(o->bubble, parent);
		evas_object_smart_member_add(o->object, parent);
	}

	if (operation != MAPS_VIEW_OBJECT_REMOVE) {
		_maps_view_get_clipper(view, &clipper);
		__maps_view_object_overlay_update(object, clipper);
	}

	return MAPS_ERROR_NONE;
}
