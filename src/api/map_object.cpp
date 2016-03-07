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
#include "map_object.h"
#include "map_object_plugin.h"
#include "maps_error.h"
#include "maps_util.h"
#include "map_view.h"
#include "maps_extra_types.h"
#include "marker_constructor.h"
#include "polyline_constructor.h"
#include "polygon_constructor.h"

/*
* This represents marker visual object information
 */
typedef struct _map_view_marker_data_s {
	maps_coordinates_h coordinates;
	int width;
	int height;
	char *file_path;
	map_marker_type_e type;
	int z_order;
} map_view_marker_data_s;

const gsize _MAP_MARKER_FILE_PATH_MAX_LENGTH = 128;

/*
* This represents polyline visual object information
 */
typedef struct _map_view_polyline_data_s {
	/* The list of maps_coordinates_h points, comprising the polyline */
	maps_coordinates_list_h points;

	/* Line color */
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	/* Line width */
	int width;
} map_view_polyline_data_s;

/*
* This represents polygon visual object information
 */
typedef struct _map_view_polygon_data_s {
	/* The list of maps_coordinates_h points, comprising the polygon */
	maps_coordinates_list_h points;

	/* The polygon backgraund color */
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} map_view_polygon_data_s;

/*
* This represents visual object information
 */
typedef struct _map_object_s {
	map_object_type_e type; /* marker, polyline, polygon, group */

	void *shape_data;	/* Pointer to the visual object data, such as
				   map_view_marker_data_s or
				   map_view_polyline_data_s */

	bool visible;

	map_object_h parent_group; /* The group, owning the object */

	map_view_h view;	/* Link with the parent Map View */

} map_object_s;


extern int  _map_view_on_object_operation(map_view_h view,
					  const map_object_h object,
					  map_object_operation_e operation);

static map_view_h __get_view(const map_object_h object);
static map_view_polyline_data_s *__get_polyline_data(const map_object_h
						     object);
static map_view_polygon_data_s *__get_polygon_data(const map_object_h
						   object);
static map_view_marker_data_s *__get_marker_data(const map_object_h
						 object);

static int __map_view_polyline_data_create(void **polyline);
static int __map_view_polygon_data_create(void **polygon);
static int __map_view_marker_data_create(void **marker);

static int __map_view_polyline_data_destroy(void *polyline);
static int __map_view_polygon_data_destroy(void *polygon);
static int __map_view_marker_data_destroy(void *marker);

static map_view_h __get_view(const map_object_h object)
{
	if(!object)
		return NULL;
	map_object_s *o = (map_object_s *)object;
	return o->view;
}

static map_view_polyline_data_s *__get_polyline_data(const map_object_h
						     object)
{
	map_object_s *o = (map_object_s *)object;
	if(!o || (o->type != MAP_OBJECT_POLYLINE))
		return NULL;
	return (map_view_polyline_data_s *)o->shape_data;
}

static int __map_view_polyline_data_create(void **polyline)
{
	if (!polyline)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	map_view_polyline_data_s *p = NULL;

	do {
		p = g_slice_new0(map_view_polyline_data_s);
		if (!p) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		error = maps_coordinates_list_create(&p->points);
		if (error != MAPS_ERROR_NONE)
			break;

		*polyline = (map_view_polyline_data_s *) p;
		return MAPS_ERROR_NONE;
	} while(false);

	__map_view_polyline_data_destroy(p);
	return error;
}

static int __map_view_polyline_data_destroy(void *polyline)
{
	if (!polyline)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_polyline_data_s *p = (map_view_polyline_data_s *)polyline;
	if (p->points)
		maps_coordinates_list_destroy(p->points);

	g_slice_free(map_view_polyline_data_s, p);

	return MAPS_ERROR_NONE;
}

static map_view_polygon_data_s *__get_polygon_data(const map_object_h
						   object)
{
	map_object_s *o = (map_object_s *)object;
	if(!o || (o->type != MAP_OBJECT_POLYGON))
		return NULL;
	return (map_view_polygon_data_s *)o->shape_data;
}

static int __map_view_polygon_data_create(void **polygon)
{
	if (!polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	map_view_polygon_data_s *p = NULL;

	do {
		p = g_slice_new0(map_view_polygon_data_s);
		if (!p) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		error = maps_coordinates_list_create(&p->points);
		if (error != MAPS_ERROR_NONE)
			break;

		*polygon = (map_view_polygon_data_s *) p;
		return MAPS_ERROR_NONE;
	} while(false);

	__map_view_polygon_data_destroy(p);
	return error;
}

static int __map_view_polygon_data_destroy(void *polygon)
{
	if (!polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_polygon_data_s *p = (map_view_polygon_data_s *)polygon;
	if (p->points)
		maps_coordinates_list_destroy(p->points);

	g_slice_free(map_view_polygon_data_s, p);

	return MAPS_ERROR_NONE;
}

static map_view_marker_data_s *__get_marker_data(const map_object_h object)
{
	map_object_s *o = (map_object_s *)object;
	if(!o || (o->type != MAP_OBJECT_MARKER))
		return NULL;
	return (map_view_marker_data_s *)o->shape_data;
}

static int __map_view_marker_data_create(void **marker)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;


	int error = MAPS_ERROR_NONE;
	map_view_marker_data_s *m = NULL;

	do {
		m = g_slice_new0(map_view_marker_data_s);
		if (!m) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		error = maps_coordinates_create(.0, .0, &m->coordinates);
		if (error != MAPS_ERROR_NONE)
			break;

		m->type = MAP_MARKER_NONE;
		m->width = 0;
		m->height = 0;
		m->z_order = 0;
		*marker = (map_view_marker_data_s *) m;
		return MAPS_ERROR_NONE;
	} while(false);

	__map_view_marker_data_destroy(m);
	return error;
}

static int __map_view_marker_data_destroy(void *marker)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_marker_data_s *m = (map_view_marker_data_s *)marker;
	if (m->coordinates)
		maps_coordinates_destroy(m->coordinates);

	if (m->file_path)
		g_free(m->file_path);

	g_slice_free(map_view_marker_data_s, m);

	return MAPS_ERROR_NONE;
}

int _map_object_create(const map_object_type_e type, map_object_h *object)
{
	if (!object)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((type < MAP_OBJECT_POLYLINE) || (type > MAP_OBJECT_UNKNOWN))
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	map_object_s *o = NULL;

	do {
		o = g_slice_new0(map_object_s);
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
		case MAP_OBJECT_POLYLINE:
			error = __map_view_polyline_data_create(&o->shape_data);
			break;
		case MAP_OBJECT_POLYGON:
			error = __map_view_polygon_data_create(&o->shape_data);
			break;
		case MAP_OBJECT_MARKER:
			error = __map_view_marker_data_create(&o->shape_data);
			break;
		default:
			break;
		}

		if((!o->shape_data) || (error != MAPS_ERROR_NONE))
			break;

		/* Notify view, that the object was constructed */
		/*if(view)
		  _map_view_on_object_operation(view,
		  o,
		  MAP_OBJECT_ADD);*/

		*object = (map_object_h) o;
		return MAPS_ERROR_NONE;
	} while(false);
	map_object_destroy(o);
	return error;
}

int _map_object_set_view(map_object_h object, map_view_h view)
{
	if (!object || !view)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_object_s *o = (map_object_s *) object;
	o->view = view;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_create_marker(const maps_coordinates_h coordinates,
					const char *image_file_path,
					const map_marker_type_e type,
					map_object_h *marker)
{
	if (!coordinates || !marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((type < MAP_MARKER_PIN) || (type > MAP_MARKER_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Create a Marker Visual Object */
	view::marker_constructor mc;
	*marker = mc.construct(coordinates, image_file_path, type);
	if(mc.get_error() == MAPS_ERROR_NONE)
		return mc.get_error();

	/* Marker create failure */
	map_object_destroy(*marker);
	*marker = NULL;
	return mc.get_error();
}

EXPORT_API int map_object_create_polyline(const maps_coordinates_list_h coordinates,
					  const unsigned char r,
					  const unsigned char g,
					  const unsigned char b,
					  const unsigned char a,
					  const int width,
					  map_object_h *polyline)
{
	if (!coordinates || !polyline)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Create a Polyline Visual Object */
	view::polyline_constructor pc;
	*polyline = pc.construct(coordinates, r, g, b, a, width);
	if(pc.get_error() == MAPS_ERROR_NONE)
		return pc.get_error();

	/* Polyline create failure */
	map_object_destroy(*polyline);
	*polyline = NULL;
	return pc.get_error();
}

EXPORT_API int map_object_create_polygon(const maps_coordinates_list_h coordinates,
					 const unsigned char r,
					 const unsigned char g,
					 const unsigned char b,
					 const unsigned char a,
					 map_object_h *polygon)
{
	if (!coordinates || !polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Create a Polygon Visual Object */
	view::polygon_constructor pc;
	*polygon = pc.construct(coordinates, r, g, b, a);
	if(pc.get_error() == MAPS_ERROR_NONE)
		return pc.get_error();

	/* Polygon create failure */
	map_object_destroy(*polygon);
	*polygon = NULL;
	return pc.get_error();
}

EXPORT_API int map_object_destroy(map_object_h object)
{
	if (!object)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_object_s *o = (map_object_s *) object;

	/* Notify view, that the object is to be destroyed shortly */
	_map_view_on_object_operation(o->view, o, MAP_OBJECT_REMOVE);

	int error = MAPS_ERROR_NONE;
	switch(o->type) {
	case MAP_OBJECT_POLYLINE:
		error = __map_view_polyline_data_destroy(o->shape_data);
		break;
	case MAP_OBJECT_POLYGON:
		error = __map_view_polygon_data_destroy(o->shape_data);
		break;
	case MAP_OBJECT_MARKER:
		error = __map_view_marker_data_destroy(o->shape_data);
		break;
	default:
		break;
	}
	g_slice_free(map_object_s, object);
	return error;
}

EXPORT_API int map_object_get_type(const map_object_h object,
				   map_object_type_e *type)
{
	if (!object || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	*type = ((map_object_s *)object)->type;
	return MAPS_ERROR_NONE;
}


/**
* Visual Object Operations
 */

EXPORT_API int map_object_move(const map_object_h object,
			       const maps_coordinates_h coordinates)
{
	if (!object || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_object_s *o = (map_object_s *)object;

	switch(o->type) {
	case MAP_OBJECT_POLYLINE: {
		g_print("TODO: implement moving of polyline\n");
		break;
	}
	case MAP_OBJECT_POLYGON: {
		g_print("TODO: implement moving of polygon\n");
		break;
	}
	case MAP_OBJECT_MARKER: {
		g_print("TODO: implement moving of polygon\n");
		break;
		/*map_object_h marker_data =
		  (map_object_h)o->shape_data;
		  return map_object_marker_set_coordinates(marker_data,
		  coordinates);*/
	}
	case MAP_OBJECT_UNKNOWN:
	default:
		return MAPS_ERROR_INVALID_PARAMETER;
	}

	/* Notify view, that the object is to be moved */
	_map_view_on_object_operation(o->view, o, MAP_OBJECT_MOVE);

	/* TODO: if the object has parent group, it is neede to check
	* if its geometry was affected by this object movement */

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_set_visible(const map_object_h object, const bool visible)
{
	if (!object)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_object_s *o = (map_object_s *)object;
	o->visible = visible;

	/* Notify view, that the object visibility is changed */
	_map_view_on_object_operation(o->view, o, MAP_OBJECT_SET_VISIBLE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_get_visible(const map_object_h object, bool *visible)
{
	if (!object || !visible)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_object_s *o = (map_object_s *)object;
	*visible = o->visible;
	return MAPS_ERROR_NONE;
}


/*----------------------------------------------------------------------------*/
/*
* Polyline
 */

EXPORT_API int map_object_polyline_set_polyline(const map_object_h polyline,
						const maps_coordinates_list_h points)
{
	if (!polyline || !points)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	map_view_polyline_data_s *p = __get_polyline_data(polyline);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new polyline trajectory */
	maps_coordinates_list_destroy(p->points);
	p->points = points;

	/* Notify view, that the object specific preferences is changed */
	_map_view_on_object_operation(__get_view(polyline),
				      polyline,
				      MAP_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_polyline_foreach_point(const map_object_h polyline,
						 maps_coordinates_cb callback,
						 void *user_data)
{
	if (!polyline || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	map_view_polyline_data_s *p = __get_polyline_data(polyline);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Iterate over polyline trajectory */
	return maps_coordinates_list_foreach(p->points, callback, user_data);
}


EXPORT_API int map_object_polyline_set_color(const map_object_h polyline,
					     const unsigned char r,
					     const unsigned char g,
					     const unsigned char b,
					     const unsigned char a)
{
	if (!polyline)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	map_view_polyline_data_s *p = __get_polyline_data(polyline);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new color */
	p->r = r;
	p->g = g;
	p->b = b;
	p->a = a;

	/* Notify view, that the object specific preferences is changed */
	_map_view_on_object_operation(__get_view(polyline),
				      polyline,
				      MAP_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_polyline_get_color(const map_object_h polyline,
					     unsigned char *r,
					     unsigned char *g,
					     unsigned char *b,
					     unsigned char *a)
{
	if (!polyline || !r || !g || !b || !a)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	map_view_polyline_data_s *p = __get_polyline_data(polyline);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Retrieve the color */
	*r = p->r;
	*g = p->g;
	*b = p->b;
	*a = p->a;
	return MAPS_ERROR_NONE;
}


EXPORT_API int map_object_polyline_set_width(const map_object_h polyline,
					     const int width)
{
	if (!polyline)
		return MAPS_ERROR_INVALID_PARAMETER;
	if(width < 0)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	map_view_polyline_data_s *p = __get_polyline_data(polyline);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Update the width of polyline */
	p->width = width;

	/* Notify view, that the object specific preferences is changed */
	_map_view_on_object_operation(__get_view(polyline),
				      polyline,
				      MAP_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_polyline_get_width(const map_object_h polyline,
					     int *width)
{
	if (!polyline || !width)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	map_view_polyline_data_s *p = __get_polyline_data(polyline);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Retrieve the width of the polyline */
	*width = p->width;
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/
/*
* Polygon
 */

EXPORT_API int map_object_polygon_add_point(map_object_h polygon,
					    const maps_coordinates_h point)
{
	if (!polygon || !point)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	map_view_polygon_data_s *p = __get_polygon_data(polygon);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Add the point to the polygon */
	const int error = maps_coordinates_list_append(p->points, point);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(polygon),
					      polygon,
					      MAP_OBJECT_CHANGE);

	return error;
}

EXPORT_API int map_object_polygon_set_polygon(const map_object_h polygon,
					      const maps_coordinates_list_h points)
{
	if (!polygon || !points)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	map_view_polygon_data_s *p = __get_polygon_data(polygon);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new polygon border */
	maps_coordinates_list_destroy(p->points);
	p->points = points;

	/* Notify view, that the object specific preferences is changed */
	_map_view_on_object_operation(__get_view(polygon),
				      polygon,
				      MAP_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_polygon_foreach_point(const map_object_h polygon,
						maps_coordinates_cb callback,
						void *user_data)
{
	if (!polygon || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	map_view_polygon_data_s *p = __get_polygon_data(polygon);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Iterate over polygon border */
	return maps_coordinates_list_foreach(p->points, callback, user_data);
}


EXPORT_API int map_object_polygon_set_fill_color(const map_object_h polygon,
						 const unsigned char r,
						 const unsigned char g,
						 const unsigned char b,
						 const unsigned char a)
{
	if (!polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	map_view_polygon_data_s *p = __get_polygon_data(polygon);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new background color */
	p->r = r;
	p->g = g;
	p->b = b;
	p->a = a;

	/* Notify view, that the object specific preferences is changed */
	_map_view_on_object_operation(__get_view(polygon),
				      polygon,
				      MAP_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_polygon_get_fill_color(const map_object_h polygon,
						 unsigned char *r,
						 unsigned char *g,
						 unsigned char *b,
						 unsigned char *a)
{
	if (!polygon)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	map_view_polygon_data_s *p = __get_polygon_data(polygon);
	if(!p)
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

EXPORT_API int map_object_marker_set_coordinates(const map_object_h marker,
						 const maps_coordinates_h coordinates)
{
	if (!marker || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (m->coordinates)
		maps_coordinates_destroy(m->coordinates);
	const int error = maps_coordinates_clone(coordinates, &m->coordinates);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(marker),
					      marker,
					      MAP_OBJECT_CHANGE);

	return error;

}

EXPORT_API int map_object_marker_resize(const map_object_h marker,
					const int width, const int height)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (width < 0 || height < 0)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->width = width;
	m->height = height;

	/* Notify view, that the object specific preferences is changed */
	_map_view_on_object_operation(__get_view(marker),
				      marker,
				      MAP_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_marker_set_image_file(const map_object_h marker,
						const char *file_path)
{
	if (!marker || !file_path)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	const int error = maps_set_string(file_path,
					  _MAP_MARKER_FILE_PATH_MAX_LENGTH,
					  &m->file_path);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(marker),
					      marker,
					      MAP_OBJECT_CHANGE);

	return error;
}

EXPORT_API int map_object_marker_set_z_order(const map_object_h marker, const int z_order)
{
	if (!marker || (z_order > 100 || z_order < -100))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Notify view, that the object specific preferences is changed */
	if (m->z_order != z_order)
	{
		m->z_order = z_order;
		_map_view_on_object_operation(__get_view(marker), marker, MAP_OBJECT_CHANGE);
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_marker_get_z_order(const map_object_h marker,	int *z_order)
{
	if (!marker || !z_order)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;

	*z_order = m->z_order;
	return MAPS_ERROR_NONE;
}

int _map_object_marker_set_type(const map_object_h marker,
				const map_marker_type_e type)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((type < MAP_MARKER_PIN) || (type > MAP_MARKER_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->type = type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_marker_get_coordinates(const map_object_h marker,
						maps_coordinates_h *coordinates)
{
	if (!marker || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_coordinates_clone(m->coordinates, coordinates);
}

EXPORT_API int map_object_marker_set_size(const map_object_h marker, int width, int height)
{
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->width = width;
	m->height = height;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_marker_get_size(const map_object_h marker, int *width, int *height)
{
	if (!marker || (!width && !height))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (width)
		*width = m->width;
	if (height)
		*height = m->height;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_marker_get_image_file(const map_object_h marker,
						char **file_path)
{
	if (!marker || !file_path)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(m->file_path,
			       _MAP_MARKER_FILE_PATH_MAX_LENGTH,
			       file_path);
}

EXPORT_API int map_object_marker_get_type(const map_object_h marker,
					  map_marker_type_e *type)
{
	if (!marker || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	*type = m->type;
	return MAPS_ERROR_NONE;
}
