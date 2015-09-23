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
#include "maps_error.h"
#include "maps_util.h"
#include "map_view.h"
#include "maps_extra_types.h"
#include "marker_constructor.h"
#include "polyline_constructor.h"
#include "polygon_constructor.h"
#include "route_constructor.h"

/*
* This represents visual marker object information
 */
typedef struct _map_view_group_data_s {
	maps_item_list_h view_objects;
} map_view_group_data_s;

/*
* This represents marker visual object information
 */
typedef struct _map_view_marker_data_s {
	maps_coordinates_h coordinates;
	int screen_width;
	int screen_height;
	char *file_path;
	map_marker_type_e type;
	char *text;
} map_view_marker_data_s;

const gsize _MAP_MARKER_FILE_PATH_MAX_LENGTH = 128;
const gsize _MAP_MARKER_TEXT_MAX_LENGTH = 128;

/*
* This represents polyline visual object information
 */
typedef struct _map_view_polyline_data_s {
	/* The list of maps_coordinates_h points, comprising the polyline */
	maps_coord_list_h points;

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
	maps_coord_list_h points;

	/* The polygon backgraund color */
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} map_view_polygon_data_s;

#ifdef TIZEN_3_0_NEXT_MS
/*
* This represents visual route object information
 */
typedef struct _map_view_route_data_s {
	maps_route_h content;
} map_view_route_data_s;
#endif /* TIZEN_3_0_NEXT_MS */

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
static map_view_group_data_s *__get_group_data(const map_object_h
					       object);
static map_view_polyline_data_s *__get_polyline_data(const map_object_h
						     object);
static map_view_polygon_data_s *__get_polygon_data(const map_object_h
						   object);
static map_view_marker_data_s *__get_marker_data(const map_object_h
						 object);
#ifdef TIZEN_3_0_NEXT_MS
static map_view_route_data_s *__get_route_data(const map_object_h
					       object);
#endif /* TIZEN_3_0_NEXT_MS */

static int __map_view_group_data_create(void **group);
static int __map_view_polyline_data_create(void **polyline);
static int __map_view_polygon_data_create(void **polygon);
static int __map_view_marker_data_create(void **marker);

#ifdef TIZEN_3_0_NEXT_MS
static int __map_view_route_data_create(void **route);
#endif /* TIZEN_3_0_NEXT_MS */

static int __map_view_group_data_destroy(void *group);
static int __map_view_polyline_data_destroy(void *polyline);
static int __map_view_polygon_data_destroy(void *polygon);
static int __map_view_marker_data_destroy(void *marker);

#ifdef TIZEN_3_0_NEXT_MS
static int __map_view_route_data_destroy(void *route);
#endif /* TIZEN_3_0_NEXT_MS */


static map_view_h __get_view(const map_object_h object)
{
	if(!object)
		return NULL;
	map_object_s *o = (map_object_s *)object;
	return o->view;
}

static map_view_group_data_s *__get_group_data(const map_object_h object)
{
	map_object_s *o = (map_object_s *)object;
	if(!o || (o->type != MAP_OBJECT_GROUP))
		return NULL;
	return (map_view_group_data_s *)o->shape_data;
}

static int __map_view_group_data_create(void **group)
{
	if (!group)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	map_view_group_data_s *l = NULL;

	do {
		l = g_slice_new0(map_view_group_data_s);
		if (!l) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		error = maps_item_list_create(&l->view_objects);
		if (error != MAPS_ERROR_NONE)
			break;

		*group = (map_view_group_data_s *) l;
		return MAPS_ERROR_NONE;
	} while(false);

	__map_view_group_data_destroy(l);
	return error;
}

static int __map_view_group_data_destroy(void *group)
{
	if (!group)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_group_data_s *l = (map_view_group_data_s *)group;
	if (l->view_objects) {
		maps_item_list_remove_all(l->view_objects, map_object_destroy);
		maps_item_list_destroy(l->view_objects);
	}

	g_slice_free(map_view_group_data_s, l);

	return MAPS_ERROR_NONE;
}

#ifdef TIZEN_3_0_NEXT_MS
static int __map_view_route_data_create(void **route)
{
	if (!route)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	map_view_route_data_s *r = NULL;

	do {
		r = g_slice_new0(map_view_route_data_s);
		if (!r) {
			MAPS_LOGE("OUT_OF_MEMORY(0x%08x)",
				  MAPS_ERROR_OUT_OF_MEMORY);
			return MAPS_ERROR_OUT_OF_MEMORY;
		}

		*route = (map_view_route_data_s *) r;
		return MAPS_ERROR_NONE;
	} while(false);

	__map_view_route_data_destroy(r);
	return error;
}

static int __map_view_route_data_destroy(void *route)
{
	if (!route)
		return MAPS_ERROR_INVALID_PARAMETER;

	map_view_route_data_s *r = (map_view_route_data_s *)route;

	if(r->content)
		maps_route_destroy(r->content);

	g_slice_free(map_view_route_data_s, r);

	return MAPS_ERROR_NONE;
}
#endif /* TIZEN_3_0_NEXT_MS */

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

		error = maps_coord_list_create(&p->points);
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
		maps_coord_list_destroy(p->points);

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

		error = maps_coord_list_create(&p->points);
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
		maps_coord_list_destroy(p->points);

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

#ifdef TIZEN_3_0_NEXT_MS
static map_view_route_data_s *__get_route_data(const map_object_h object)
{
	map_object_s *o = (map_object_s *)object;
	if(!o || (o->type != MAP_OBJECT_ROUTE))
		return NULL;
	return (map_view_route_data_s *)o->shape_data;
}
#endif /* TIZEN_3_0_NEXT_MS */

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
	if (m->text)
		g_free(m->text);

	g_slice_free(map_view_marker_data_s, m);

	return MAPS_ERROR_NONE;
}

int _map_object_create(const map_object_type_e type, map_object_h *object)
{
	if (!object)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((type < MAP_OBJECT_GROUP) || (type > MAP_OBJECT_UNKNOWN))
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

		int error = MAPS_ERROR_NONE;
		switch(o->type) {
		case MAP_OBJECT_GROUP:
			error = __map_view_group_data_create(&o->shape_data);
			break;
		case MAP_OBJECT_POLYLINE:
			error = __map_view_polyline_data_create(&o->shape_data);
			break;
		case MAP_OBJECT_POLYGON:
			error = __map_view_polygon_data_create(&o->shape_data);
			break;
		case MAP_OBJECT_MARKER:
			error = __map_view_marker_data_create(&o->shape_data);
			break;
#ifdef TIZEN_3_0_NEXT_MS
		case MAP_OBJECT_ROUTE:
			error = __map_view_route_data_create(&o->shape_data);
			break;
#endif /* TIZEN_3_0_NEXT_MS */
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
	if (!object)
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
	MAPS_LOG_API;
	if (!coordinates || !marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if((type < MAP_MARKER_POI) || (type > MAP_MARKER_NONE))
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

#ifdef TIZEN_3_0_NEXT_MS
EXPORT_API int map_object_create_route(const maps_route_h route,
				       map_object_h *route_object)
{
	MAPS_LOG_API;
	if (!route|| !route_object)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Create a Route Visual Object */
	view::route_constructor rc;
	*route_object = rc.construct(route);
	if(error != MAPS_ERROR_NONE)
		return rc.get_error();

	if(rc.get_error() == MAPS_ERROR_NONE)
		return MAPS_ERROR_NONE;

	/* Route create failure */
	map_object_destroy(*route_object);
	*route_object = NULL;
	return rc.get_error();
}
#endif /* TIZEN_3_0_NEXT_MS */

EXPORT_API int map_object_create_polyline(const maps_coord_list_h coordinates,
					  const unsigned char r,
					  const unsigned char g,
					  const unsigned char b,
					  const unsigned char a,
					  const int width,
					  map_object_h *polyline)
{
	MAPS_LOG_API;
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

EXPORT_API int map_object_create_polygon(const maps_coord_list_h coordinates,
					 const unsigned char r,
					 const unsigned char g,
					 const unsigned char b,
					 const unsigned char a,
					 map_object_h *polygon)
{
	MAPS_LOG_API;
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


EXPORT_API int map_object_create_group(map_object_h *group)
{
	MAPS_LOG_API;
	if (!group)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Create a Group Visual Object */
	int error = _map_object_create(MAP_OBJECT_GROUP, group);
	if(error != MAPS_ERROR_NONE)
		return error;

	return map_object_set_visible(group, true);
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
	case MAP_OBJECT_GROUP:
		error = __map_view_group_data_destroy(o->shape_data);
		break;
	case MAP_OBJECT_POLYLINE:
		error = __map_view_polyline_data_destroy(o->shape_data);
		break;
	case MAP_OBJECT_POLYGON:
		error = __map_view_polygon_data_destroy(o->shape_data);
		break;
	case MAP_OBJECT_MARKER:
		error = __map_view_marker_data_destroy(o->shape_data);
		break;

#ifdef TIZEN_3_0_NEXT_MS
	case MAP_OBJECT_ROUTE:
		error = __map_view_route_data_destroy(o->shape_data);
		break;
#endif /* TIZEN_3_0_NEXT_MS */

	default:
		break;
	}
	g_slice_free(map_object_s, object);
	return error;
}

EXPORT_API int map_object_get_type(const map_object_h object,
				   map_object_type_e *type)
{
	MAPS_LOG_API;
	if (!object || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	*type = ((map_object_s *)object)->type;
	return MAPS_ERROR_NONE;
}


/**
* Visual Object Operations
 */

EXPORT_API int map_object_move(map_object_h object,
			       const maps_coordinates_h coordinates)
{
	MAPS_LOG_API;
	if (!object || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_object_s *o = (map_object_s *)object;

	switch(o->type) {
	case MAP_OBJECT_GROUP: {
		g_print("TODO: implement moving of group\n");
		break;
	}
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

#ifdef TIZEN_3_0_NEXT_MS
	case MAP_OBJECT_ROUTE: {
		g_print("TODO: implement moving of route\n");
		break;
	}
#endif /* TIZEN_3_0_NEXT_MS */

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

static bool __set_object_visible_cb(int index, int total,
				    map_object_h object,
				    void* user_data)
{
	if (!user_data)
		return false;
	bool *visible = (bool *)user_data;
	return (map_object_set_visible(object, *visible) ==
		MAPS_ERROR_NONE);
}

EXPORT_API int map_object_set_visible(map_object_h object,
				      const bool visible)
{
	MAPS_LOG_API;
	if (!object)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_object_s *o = (map_object_s *)object;
	o->visible = visible;

	if (o->type == MAP_OBJECT_GROUP) {
		bool visible_flag = visible;
		map_object_group_foreach_object(object,
						__set_object_visible_cb,
						&visible_flag);
	}

	/* Notify view, that the object visibility is changed */
	_map_view_on_object_operation(o->view, o, MAP_OBJECT_SET_VISIBLE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_get_visible(map_object_h object,
				      bool *visible)
{
	MAPS_LOG_API;
	if (!object || !visible)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_object_s *o = (map_object_s *)object;
	*visible = o->visible;
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/
/*
* Layer
 */
EXPORT_API int map_object_group_add_object(map_object_h group,
					   const map_object_h object)
{
	MAPS_LOG_API;
	if (!group || !object)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_group_data_s *l = __get_group_data(group);
	if (!l)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (!l->view_objects)
		return MAPS_ERROR_INVALID_PARAMETER;

	const int error = maps_item_list_append(l->view_objects,
						object,
						maps_item_no_clone);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(group),
					      group,
					      MAP_OBJECT_CHANGE);

	return error;
}

#if 0
/* TO BE REMOVED */
int map_object_group_set_objects(map_object_h group,
				 const maps_item_list_h objects)
{
	if (!group || !objects)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_group_data_s *l = __get_group_data(group);
	if (!l)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (!l->view_objects)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_item_list_remove_all(l->view_objects, map_object_destroy);
	maps_item_list_destroy(l->view_objects);

	const int error =  maps_item_list_clone(objects,
						maps_item_no_clone,
						&l->view_objects);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(group),
					      group,
					      MAP_OBJECT_CHANGE);

	return error;
}
#endif

EXPORT_API int map_object_group_remove_object(map_object_h group,
					      map_object_h object)
{
	MAPS_LOG_API;
	if (!group || !object)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_group_data_s *l = __get_group_data(group);
	if (!l)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (!l->view_objects)
		return MAPS_ERROR_INVALID_PARAMETER;

	const int error = maps_item_list_remove(l->view_objects,
						object,
						map_object_destroy);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(group),
					      group,
					      MAP_OBJECT_CHANGE);

	return error;
}


EXPORT_API int map_object_group_foreach_object(const map_object_h
					       group,
					       map_object_group_object_cb
					       callback,
					       void* user_data)
{
	MAPS_LOG_API;
	if (!group || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_group_data_s *l = __get_group_data(group);
	if (!l)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_list_foreach(l->view_objects,
				      maps_item_no_clone,
				      callback,
				      user_data);
}


/*----------------------------------------------------------------------------*/
/*
* Polyline
 */

EXPORT_API int map_object_polyline_add_point(map_object_h polyline,
					 const maps_coordinates_h point)
{
	MAPS_LOG_API;
	if (!polyline || !point)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	map_view_polyline_data_s *p = __get_polyline_data(polyline);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Append a point to the polyline */
	const int error = maps_coord_list_append(p->points, point);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(polyline),
					      polyline,
					      MAP_OBJECT_CHANGE);

	return error;
}

EXPORT_API int map_object_polyline_set_polyline(map_object_h polyline,
					  const maps_coord_list_h points)
{
	MAPS_LOG_API;
	if (!polyline || !points)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	map_view_polyline_data_s *p = __get_polyline_data(polyline);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new polyline trajectory */
	maps_coord_list_destroy(p->points);
	const int error = maps_coord_list_clone(points, &p->points);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(polyline),
					      polyline,
					      MAP_OBJECT_CHANGE);

	return error;
}

EXPORT_API int map_object_polyline_foreach_point(map_object_h polyline,
						 map_object_polyline_point_cb
						 callback,
						 void *user_data)
{
	MAPS_LOG_API;
	if (!polyline || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polyline data pointer */
	map_view_polyline_data_s *p = __get_polyline_data(polyline);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Iterate over polyline trajectory */
	return maps_coord_list_foreach(p->points, callback, user_data);

}


EXPORT_API int map_object_polyline_set_color(map_object_h polyline,
					     const unsigned char r,
					     const unsigned char g,
					     const unsigned char b,
					     const unsigned char a)
{
	MAPS_LOG_API;
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
	MAPS_LOG_API;
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


EXPORT_API int map_object_polyline_set_width(map_object_h polyline,
					     const int width)
{
	MAPS_LOG_API;
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
	MAPS_LOG_API;
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
	MAPS_LOG_API;
	if (!polygon || !point)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	map_view_polygon_data_s *p = __get_polygon_data(polygon);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Add the point to the polygon */
	const int error = maps_coord_list_append(p->points, point);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(polygon),
					      polygon,
					      MAP_OBJECT_CHANGE);

	return error;
}

EXPORT_API int map_object_polygon_set_polygon(map_object_h polygon,
					      const maps_coord_list_h points)
{
	MAPS_LOG_API;
	if (!polygon || !points)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	map_view_polygon_data_s *p = __get_polygon_data(polygon);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Set new polygon border */
	maps_coord_list_destroy(p->points);
	const int error = maps_coord_list_clone(points, &p->points);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(polygon),
					      polygon,
					      MAP_OBJECT_CHANGE);

	return error;
}

EXPORT_API int map_object_polygon_foreach_point(map_object_h polygon,
						map_object_polygon_point_cb
						callback,
						void *user_data)
{
	MAPS_LOG_API;
	if (!polygon || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Get the polygon data pointer */
	map_view_polygon_data_s *p = __get_polygon_data(polygon);
	if(!p)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Iterate over polygon border */
	return maps_coord_list_foreach(p->points, callback, user_data);
}


EXPORT_API int map_object_polygon_set_fill_color(map_object_h polygon,
						 const unsigned char r,
						 const unsigned char g,
						 const unsigned char b,
						 const unsigned char a)
{
	MAPS_LOG_API;
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
	MAPS_LOG_API;
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


EXPORT_API int map_object_marker_set_coordinates(map_object_h marker,
						 const maps_coordinates_h
						 coordinates)
{
	MAPS_LOG_API;
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

EXPORT_API int map_object_marker_resize(map_object_h
					marker,
					const int screen_width,
					const int screen_height)
{
	MAPS_LOG_API;
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((screen_width <= 0) || (screen_height <= 0))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->screen_width = screen_width;
	m->screen_height = screen_height;

	/* Notify view, that the object specific preferences is changed */
	_map_view_on_object_operation(__get_view(marker),
				      marker,
				      MAP_OBJECT_CHANGE);

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_marker_set_image_file(map_object_h
						marker,
						const char *file_path)
{
	MAPS_LOG_API;
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

EXPORT_API int map_object_marker_set_text(map_object_h marker,
					  const char *text)
{
	MAPS_LOG_API;
	if (!marker || !text)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	const int error = maps_set_string(text,
					  _MAP_MARKER_TEXT_MAX_LENGTH,
					  &m->text);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(marker),
					      marker,
					      MAP_OBJECT_CHANGE);

	return error;
}

int _map_object_marker_set_type(map_object_h marker,
				const map_marker_type_e type)
{
	/* MAPS_LOG_API; */
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	if ((type < MAP_MARKER_POI) || (type > MAP_MARKER_NONE))
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	m->type = type;

	/* Notify view, that the object specific preferences is changed */
	/*_map_view_on_object_operation(__get_view(marker),
	  marker,
	  MAP_OBJECT_CHANGE);*/

	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_marker_get_coordinates(const map_object_h marker,
						maps_coordinates_h *coordinates)
{
	MAPS_LOG_API;
	if (!marker || !coordinates)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_coordinates_clone(m->coordinates, coordinates);
}

EXPORT_API int map_object_marker_get_size(map_object_h
					  marker,
					  int *screen_width,
					  int *screen_height)
{
	MAPS_LOG_API;
	if (!marker)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	if (screen_width)
		*screen_width = m->screen_width;
	if (screen_height)
		*screen_height = m->screen_height;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_marker_get_image_file(const map_object_h marker,
						char **file_path)
{
	MAPS_LOG_API;
	if (!marker || !file_path)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(m->file_path,
			       _MAP_MARKER_FILE_PATH_MAX_LENGTH,
			       file_path);
}

EXPORT_API int map_object_marker_get_type(const map_object_h
					  marker,
					  map_marker_type_e *type)
{
	MAPS_LOG_API;
	if (!marker || !type)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	*type = m->type;
	return MAPS_ERROR_NONE;
}

EXPORT_API int map_object_marker_get_text(const map_object_h
					  marker,
					  char **text)
{
	MAPS_LOG_API;
	if (!marker || !text)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_marker_data_s *m = __get_marker_data(marker);
	if (!m)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(m->text, _MAP_MARKER_TEXT_MAX_LENGTH, text);
}


/*----------------------------------------------------------------------------*/
/*
* Route
 */

#ifdef TIZEN_3_0_NEXT_MS
int _map_object_route_set_content(map_object_h route,
				  const maps_route_h content)
{
	if (!route|| !content)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_route_data_s *r = __get_route_data(route);
	if (!r)
		return MAPS_ERROR_INVALID_PARAMETER;

	const int error = maps_route_clone(content, &r->content);

	/* Notify view, that the object specific preferences is changed */
	if(error == MAPS_ERROR_NONE)
		_map_view_on_object_operation(__get_view(route),
					      route,
					      MAP_OBJECT_CHANGE);

	return error;
}

EXPORT_API int map_object_route_get_content(const map_object_h route,
					    maps_route_h *content)
{
	if (!route|| !content)
		return MAPS_ERROR_INVALID_PARAMETER;
	map_view_route_data_s *r = __get_route_data(route);
	if (!r)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* TODO: discuss if we need cloning or is it enough to return existing
	*  handle */
	return maps_route_clone(r->content, content);
}
#endif /* TIZEN_3_0_NEXT_MS */
