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

#include <maps_service.h>
#include <maps_error.h>
#include <maps_plugin.h>
#include <maps_address.h>
#include <maps_place_plugin.h>
#include <maps_route_plugin.h>
#include <glib.h>

#include <maps_object.h>
#include "maps_util.h"
#include <Ecore.h>

#include <map_view.h>

/* For Route Visual Data */
#include "dummy_route_constructor.h"
#include "map_object.h"
#include <map>


static int dummy_geocode_request_id = 101;
static char __provider_key[1024] = { 0 };


/* ---------------------------------------------------------------------------*/

#ifdef TIZEN_3_0_NEXT_MS
class maps_plugin_route {
public:
	/* Handle of the corresponding Map View Object */
	map_object_h view_object;

	/* List of visual objects, comprising the route */
	map_object_h group;

	/* Route Content */
	maps_route_h content;

public:
	maps_plugin_route(const map_object_h object,
			  const maps_route_h route)
		: view_object(object)
		  , group(NULL)
		  , content(route)
	{
		view::dummy_route_constructor rc;
		group = rc.construct(route);
	}

	~maps_plugin_route()
	{
		if(group)
			map_object_destroy(group);

		if(content)
			maps_route_destroy(content);
	}
};

/* Global storage of all route objects */
std::map<void *, void *> __routes;

static void __remove_route(const map_object_h object)
{
	if(!object)
		return;

	if(__routes.count(object) <= 0)
		return;

	maps_plugin_route *route = (maps_plugin_route *)__routes[object];
	if(route)
		delete route;

	__routes.erase(object);
}

static void __remove_all_routes()
{
	for(std::map<void *, void *>::iterator it = __routes.begin();
	    it != __routes.end(); ++it) {
		if(!it->second) continue;
		maps_plugin_route *route = (maps_plugin_route *)it->second;
		delete route;
	}
	__routes.clear();
}

static void __store_route(const map_object_h object,
			  maps_plugin_route *route)
{
	if(!object || !route)
		return;
	__remove_route(object);
	__routes[object] = route;
}


static maps_plugin_route *__get_route(const map_object_h object)
{
	if(!object)
		return NULL;

	return (maps_plugin_route *)(__routes[object]);
}

#endif /* TIZEN_3_0_NEXT_MS */

/*----------------------------------------------------------------------------*/
/* Service API */

EXPORT_API int maps_plugin_init(maps_plugin_h *plugin)
{
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_shutdown(maps_plugin_h plugin)
{
#ifdef TIZEN_3_0_NEXT_MS
	__remove_all_routes();
#endif /* TIZEN_3_0_NEXT_MS */
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_get_info(maps_plugin_info_h *info)
{
	/*g_print("\n\nplugin maps_plugin_get_info\n\n"); */
	maps_plugin_info_create(info);
	maps_plugin_info_set_provider_name(*info, "Maps Provider");
	return MAPS_ERROR_NONE;
}

/*----------------------------------------------------------------------------*/
/*
 * Maps API
 */

EXPORT_API int maps_plugin_set_provider_key(const char *map_key)
{
	g_return_val_if_fail(map_key, MAPS_ERROR_INVALID_PARAMETER);
	g_snprintf(__provider_key, 1024, "%s", map_key);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_get_provider_key(char **map_key)
{
	g_return_val_if_fail(map_key, MAPS_ERROR_INVALID_PARAMETER);
	*map_key = g_strndup(__provider_key, 1024);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_get_preference(maps_item_hashtable_h *preference)
{
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_set_preference(maps_item_hashtable_h preference)
{
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_is_service_supported(maps_service_e service, bool *supported)
{
	switch (service) {
	case MAPS_SERVICE_GEOCODE:
	case MAPS_SERVICE_GEOCODE_INSIDE_AREA:
	case MAPS_SERVICE_GEOCODE_BY_STRUCTURED_ADDRESS:
	case MAPS_SERVICE_REVERSE_GEOCODE:
	case MAPS_SERVICE_SEARCH_PLACE:
	case MAPS_SERVICE_SEARCH_PLACE_BY_AREA:
	case MAPS_SERVICE_SEARCH_PLACE_BY_ADDRESS:
	case MAPS_SERVICE_SEARCH_ROUTE:
	case MAPS_SERVICE_SEARCH_ROUTE_WAYPOINTS:
	case MAPS_SERVICE_CANCEL_REQUEST:
	case MAPS_SERVICE_MULTI_REVERSE_GEOCODE:
	case MAPS_SERVICE_SEARCH_PLACE_LIST:
		*supported = true;
		return MAPS_ERROR_NONE;
	default:
		*supported = false;
		return MAPS_ERROR_NOT_SUPPORTED;
	}
}

EXPORT_API int maps_plugin_is_data_supported(maps_service_data_e data, bool *supported)
{
	switch (data) {
	case MAPS_PLACE_ADDRESS:
	case MAPS_PLACE_RATING:
	case MAPS_PLACE_CATEGORIES:
	case MAPS_PLACE_ATTRIBUTES:
	case MAPS_PLACE_CONTACTS:
	case MAPS_PLACE_EDITORIALS:
	case MAPS_PLACE_REVIEWS:
	case MAPS_PLACE_IMAGE:
	case MAPS_PLACE_SUPPLIER:
	case MAPS_PLACE_RELATED:
	case MAPS_ROUTE_PATH:
	case MAPS_ROUTE_SEGMENTS_PATH:
	case MAPS_ROUTE_SEGMENTS_MANEUVERS:
		*supported = true;
		return MAPS_ERROR_NONE;
	default:
		*supported = false;
		return MAPS_ERROR_NOT_SUPPORTED;
	}
}

EXPORT_API int maps_plugin_is_widget_supported(maps_service_widget_e service, bool *supported)
{
	if (!supported)
		return MAPS_ERROR_INVALID_PARAMETER;

	switch(service)
	{
		case MAPS_WIDGET_TRAFFIC:
		case MAPS_WIDGET_PUBLIC_TRANSIT:
		case MAPS_WIDGET_SCALEBAR:
		case MAPS_WIDGET_BUILDING:
			*supported = TRUE;
			return MAPS_ERROR_NONE;
		default:
			*supported = FALSE;
			return MAPS_ERROR_NOT_SUPPORTED;
	}
}

EXPORT_API int maps_plugin_geocode(const char *address,
				   const maps_item_hashtable_h preference,
				   maps_service_geocode_cb callback,
				   void *user_data, int *request_id)
{
	g_return_val_if_fail(address, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(callback, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(request_id, MAPS_ERROR_INVALID_PARAMETER);
	/*g_return_val_if_fail(preference, MAPS_ERROR_INVALID_PARAMETER);*/

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	maps::coordinates c(37.1, 127.2);
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, c.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_geocode_inside_area(const char *address,
					       const maps_area_h bounds,
					       const maps_item_hashtable_h
					       preference,
					       maps_service_geocode_cb callback,
					       void *user_data, int *request_id)
{
	g_return_val_if_fail(address, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(bounds, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(callback, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(request_id, MAPS_ERROR_INVALID_PARAMETER);
	/*g_return_val_if_fail(preference, MAPS_ERROR_INVALID_PARAMETER);*/

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	maps::coordinates c(37.1, 127.2);
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, c.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_geocode_by_structured_address(const maps_address_h
							 address,
							 const maps_preference_h
							 preference,
							maps_service_geocode_cb
							callback,
							void *user_data,
							int *request_id)
{
	g_return_val_if_fail(address, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(callback, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(request_id, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(preference, MAPS_ERROR_INVALID_PARAMETER);

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	maps::coordinates c(37.1, 127.2);
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, c.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_reverse_geocode(double latitude, double longitude,
					   const maps_item_hashtable_h
					   preference,
					   maps_service_reverse_geocode_cb
					   callback,
					   void *user_data,
					   int *request_id)
{
	g_return_val_if_fail(request_id, MAPS_ERROR_INVALID_PARAMETER);
	g_return_val_if_fail(preference, MAPS_ERROR_INVALID_PARAMETER);

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	maps::address a;
	maps_address_set_building_number(a, "12");
	maps_address_set_street(a, "Na Boyishti");
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, a.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_place(const maps_coordinates_h position,
					int distance,
					const maps_place_filter_h filter,
					maps_preference_h preference,
					maps_service_search_place_cb callback,
					void *user_data, int *request_id)
{

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	maps::place p;
	maps_place_set_id(p, "1722/12");
	maps_place_set_name(p, "U Kalicha");
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, p.clone(), user_data);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_place_by_area(const maps_area_h boundary,
						const maps_place_filter_h
						filter,
						maps_preference_h preference,
						maps_service_search_place_cb
						callback,
						void *user_data,
						int *request_id)
{

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	maps::place p;
	maps_place_set_id(p, "1722/12");
	maps_place_set_name(p, "U Kalicha");
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, p.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_place_by_address(const char *address,
						   const maps_area_h boundary,
						   const maps_place_filter_h
						   filter,
						   maps_preference_h preference,
						   maps_service_search_place_cb
						   callback,
						   void *user_data,
						   int *request_id)
{

	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	maps::place p;
	maps_place_set_id(p, "1722/12");
	maps_place_set_name(p, "U Kalicha");
	callback(MAPS_ERROR_NONE, *request_id, 0, 1, p.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_route(const maps_coordinates_h origin,
					const maps_coordinates_h destination,
					maps_preference_h preference,
					maps_service_search_route_cb callback,
					void *user_data,
					int *request_id)
{

	/*g_print("Maps Plugin Search Route\n\n"); */
	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	maps::route route;

	maps_route_set_origin(route, maps::coordinates(-85.0, 60.0));
	maps_route_set_destination(route, maps::coordinates(-45.0, 120.0));

	maps::item_list <maps::coordinates> path;
	path.add(maps::coordinates(-85, 80));
	path.add(maps::coordinates(-75, 110));
	path.add(maps::coordinates(-65, 100));
	path.add(maps::coordinates(-50, 90));
	path.add(maps::coordinates(-40, 70));
	path.add(maps::coordinates(-30, 80));
	path.add(maps::coordinates(-45, 140));
	maps_route_set_path(route, path);

	maps::item_list <maps::route_maneuver> maneuvers;
	maps::route_maneuver m1, m2;
	maneuvers.add(m1);
	maneuvers.add(m2);

	maps::item_list <maps::route_segment> segments;
	maps::route_segment s1, s2, s3;

	/* First segment with detailed path */
	maps_route_segment_set_origin(s1, maps::coordinates(-85.0, 60.0));
	maps_route_segment_set_destination(s1, maps::coordinates(-65.0, 80.0));
	maps::item_list <maps::coordinates> s1_path;
	s1_path.add(maps::coordinates(-85, 60));
	s1_path.add(maps::coordinates(-79, 55));
	s1_path.add(maps::coordinates(-75, 60));
	s1_path.add(maps::coordinates(-73, 85));
	s1_path.add(maps::coordinates(-69, 95));
	s1_path.add(maps::coordinates(-65, 80));
	maps_route_segment_set_path(s1, s1_path);
	maps_route_segment_set_maneuvers(s1, maneuvers);
	segments.add(s1);

	maps_route_segment_set_origin(s2, maps::coordinates(-65.0, 80.0));
	maps_route_segment_set_destination(s2, maps::coordinates(-40.0, 50.0));
	maps_route_segment_set_maneuvers(s2, maneuvers);
	segments.add(s2);

	maps_route_segment_set_origin(s3, maps::coordinates(-40.0, 50.0));
	maps_route_segment_set_destination(s3, maps::coordinates(-45.0, 120.0));
	maps_route_segment_set_maneuvers(s3, maneuvers);
	segments.add(s3);

	maps_route_set_segments(route, segments);

	callback(MAPS_ERROR_NONE, *request_id, 0, 1, route.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_search_route_waypoints(const maps_coordinates_h *
						  waypoint_list,
						  int waypoint_num,
						  maps_preference_h preference,
						  maps_service_search_route_cb
						  callback,
						  void *user_data,
						  int *request_id)
{
	*request_id = dummy_geocode_request_id++;

	/* For testing purposes */
	bool no_need_callback = false;
	maps_item_hashtable_contains(preference, "no_need_callback",
		&no_need_callback);
	if (no_need_callback)
		return MAPS_ERROR_NONE;

	maps::route route;

	maps::item_list <maps::route_maneuver> maneuvers;
	maps::route_maneuver m1, m2;
	maneuvers.add(m1);
	maneuvers.add(m2);

	maps::item_list <maps::route_segment> segments;
	maps::route_segment s1, s2;

	maps_route_segment_set_origin(s1, maps::coordinates(11.1, 12.1));
	maps_route_segment_set_destination(s1, maps::coordinates(21.1, 22.1));

	maps_route_segment_set_origin(s2, maps::coordinates(11.1, 12.1));
	maps_route_segment_set_destination(s2, maps::coordinates(21.1, 22.1));

	maps_route_segment_set_maneuvers(s1, maneuvers);
	maps_route_segment_set_maneuvers(s2, maneuvers);

	segments.add(s1);
	segments.add(s2);
	maps_route_set_segments(route, segments);

	callback(MAPS_ERROR_NONE, *request_id, 0, 1, route.clone(), user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_cancel_request(int request_id)
{
	return MAPS_ERROR_NONE;
}

/*-----------------MAPPING API------------------------------------------------*/
static map_view_h __map_view = NULL;
static double __zoom_factor = 1.;
static double __rotation_angle = 0.;
static maps_coordinates_h __center_coords = NULL;
Evas_Object *__img = NULL;
static unsigned int __view_request_no = 50;
unsigned int *__pixels = NULL;

static int __width = 0;
static int __height = 0;

static const int __palette_size = 240;
static unsigned int __demo_palette[__palette_size] = { 0 };

static void __generate_demo_palette()
{
	float n = .0;
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	unsigned int sr = 255;
	unsigned int sg = 255;
	unsigned int sb = 0;
	unsigned int er = 255;
	unsigned int eg = 0;
	unsigned int eb = 0;

	const int steps = __palette_size / 2;
	for (int i = 0; i < steps; i++)
	{
		n = (float)i / (float) (steps - 1);
		r = (float)sr * (1.0f - n) + (float)er * n;
		g = (float)sg * (1.0f - n) + (float)eg * n;
		b = (float)sb * (1.0f - n) + (float)eb * n;
		unsigned int color = (int(r) << 16) | (int(g) << 8) | b;
		__demo_palette[i] = color;
	}

	sr = 255;
	sg = 0;
	sb = 0;
	er = 0;
	eg = 0;
	eb = 255;
	for (int i = 0; i < steps; i++)
	{
		n = (float)i / (float) (steps - 1);
		r = (float)sr * (1.0f - n) + (float)er * n;
		g = (float)sg * (1.0f - n) + (float)eg * n;
		b = (float)sb * (1.0f - n) + (float)eb * n;
		unsigned int color = (int(r) << 16) | (int(g) << 8) | b;
		__demo_palette[steps + i] = color;
	}
}

static int __extract_view_geometry()
{
	if (!__map_view)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;

	do {
		error = map_view_get_screen_location(__map_view, NULL, NULL,
					       &__width, &__height);
		if (error != MAPS_ERROR_NONE)
			break;

		int __x = 0;
		int __y = 0;
		error = map_view_get_screen_location(__map_view,
					       &__x, &__y,
					       NULL, NULL);
		if (error != MAPS_ERROR_NONE)
			break;

		g_print("Extracted geometry: %d, %d, %d, %d\n", __x, __y,
			__width, __height);

		return MAPS_ERROR_NONE;
	} while(false);

	return error;
}

#if 0
#include <sys/timeb.h>
int __last_redraw_time = 0;
static int __get_milli_count()
{
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int __get_milli_span(int nTimeStart)
{
	int nSpan = __get_milli_count() - nTimeStart;
	if (nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}
#endif

EXPORT_API int maps_plugin_set_map_view(const map_view_h view)
{
	if (view) {
		__map_view = view;

		/* Default values of internal parameters */
		__zoom_factor = 1.;
		__rotation_angle = 0.;
		__center_coords = NULL;
		__img = NULL;
		__pixels = NULL;
		__width = 0;
		__height = 0;

		/* Cache the geometry of the Maps View */
		__extract_view_geometry();

		/* Generate a palette, used in immitation drawing */
		__generate_demo_palette();

	} else {
		__map_view = NULL;

		if (__center_coords)
			maps_coordinates_destroy(__center_coords);
		__center_coords = NULL;
	}
	return MAPS_ERROR_NONE;
}


static void __render_line(const int x1, const int y1,
			  const int x2, const int y2,
			  unsigned int color,
			  const int width, const int height,
			  unsigned int *pixels)
{
	if (!pixels)
		return;

	if (x1 == x2) {		/* vertical */
		for (int y = y1; y <= y2; y ++)
			pixels[y * width + x1] = color;
	} else if (y1 == y2) {	/* horizontal */
		for (int x = x1; x <= x2; x ++)
			pixels[y1 * width + x] = color;
	} else {		/* regular line: Bresenham's alg */
/* https://www.cs.unm.edu/~angel/BOOK/
 *   INTERACTIVE_COMPUTER_GRAPHICS/FOURTH_EDITION/PROGRAMS/bresenham.c */
		int dx = x2 - x1;
		if (dx < 0) dx = -dx;
		int dy = y2 - y1;
		if (dy < 0) dy = -dy;

		int incx = 1;
		if (x2 < x1) incx = -1;
		int incy = 1;
		if (y2 < y1) incy = -1;

		int x = x1;
		int y = y1;

		if (dx > dy)
		{
			pixels[y * width + x] = color;
			int e = 2*dy - dx;
			int inc1 = 2*( dy -dx);
			int inc2 = 2*dy;
			for (int i = 0; i < dx; i++)
			{
				if (e >= 0)
				{
					y += incy;
					e += inc1;
				}
				else e += inc2;
				x += incx;
				pixels[y * width + x] = color;
			}
		}
		else
		{
			pixels[y * width + x] = color;
			int e = 2*dx - dy;
			int inc1 = 2*( dx - dy);
			int inc2 = 2*dx;
			for (int i = 0; i < dy; i++)
			{
				if (e >= 0)
				{
					x += incx;
					e += inc1;
				}
				else e += inc2;
				y += incy;
				pixels[y * width + x] = color;
			}
		}
	}

}

typedef struct __collect_points_s {
	maps_coordinates_h *poly_coords;
	int poly_size;
} __collecting_poings_s;

static bool __map_view_poly_collect_points_cb(int index, int total,
					      maps_coordinates_h point,
					      void *user_data)
{
	if (!point || !user_data)
		return false;

	__collecting_poings_s *cp = (__collecting_poings_s *)user_data;

	/* On the first iteration we should initialize the buffer for coords */
	if ((index == 0) && (total > 0)) {
		cp->poly_size = total;
		cp->poly_coords = g_new0(maps_coordinates_h, total);
		if (!cp->poly_coords) {
			return false;
		}
	}
	cp->poly_coords[index] = point;
	return true;
}


static void __draw_polyline(const map_object_h object,
			    unsigned int *pixels)
{
	/*g_print("__draw_polyline\n");*/
	if (!object || !pixels)
		return;

	/* Fetch polyline points in one array */
	__collecting_poings_s cp = { 0 };
	int error =  map_object_polyline_foreach_point(object,
					    __map_view_poly_collect_points_cb,
					    &cp);
	if (error != MAPS_ERROR_NONE) {
		g_print("Error %d\n", error);
		if (error == MAPS_ERROR_INVALID_PARAMETER)
			g_print("MAPS_ERROR_INVALID_PARAMETER\n");
		else if (error == MAPS_ERROR_NOT_FOUND)
			g_print("MAPS_ERROR_NOT_FOUND\n");
		return;
	}

	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
	map_object_polyline_get_color(object, &r, &g, &b, &a);
	unsigned int color = ((1 | a) << 24)
		| ((1 | r) << 16)
		| ((1 | g) << 8)
		| b;

	for (int i = 1; i < cp.poly_size; i ++) {
		/* Start of the current polyline section */
		int x1 = 0;
		int y1 = 0;
		maps_plugin_geography_to_screen(cp.poly_coords[i - 1],
						&x1, &y1);

		/* Finish of the current polyline section */
		int x2 = 0;
		int y2 = 0;
		maps_plugin_geography_to_screen(cp.poly_coords[i], &x2, &y2);

		/* Draw the current polyline section */
		__render_line(x1, y1, x2, y2,
			      color,
			      __width, __height, pixels);
	}

	/* Release all the points and the array */
	for (int i = 0; i < cp.poly_size; i ++)
		maps_coordinates_destroy(cp.poly_coords[i]);
	g_free(cp.poly_coords);
}

static void __draw_polygon(const map_object_h object,
			   unsigned int *pixels)
{
	/* g_print("__draw_polygon\n");*/
	if (!object || !pixels)
		return;

	/* Fetch polygon points in one array */
	__collecting_poings_s cp = { 0 };
	int error =  map_object_polygon_foreach_point(object,
					    __map_view_poly_collect_points_cb,
					    &cp);
	if (error != MAPS_ERROR_NONE)
		return;

	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
	map_object_polygon_get_fill_color(object, &r, &g, &b, &a);
	unsigned int color = ((1 | a) << 24)
		| ((1 | r) << 16)
		| ((1 | g) << 8)
		| b;

	/* Draw sections of polygon */
	for (int i = 1; i < cp.poly_size; i ++) {
		/* Start of the current polygon section */
		int x1 = 0;
		int y1 = 0;
		maps_plugin_geography_to_screen(cp.poly_coords[i - 1],
						&x1, &y1);

		/* Finish of the current polygon section */
		int x2 = 0;
		int y2 = 0;
		maps_plugin_geography_to_screen(cp.poly_coords[i], &x2, &y2);

		/* Draw the current polygon section */
		__render_line(x1, y1, x2, y2,
			      color,
			      __width, __height, pixels);
	}

	/* Draw the final section, connecting the last point with the first */
	if (cp.poly_size > 1) {
		/* Start of the current polygon section */
		int x1 = 0;
		int y1 = 0;
		maps_plugin_geography_to_screen(cp.poly_coords[cp.poly_size -1],
						&x1, &y1);

		/* Finish of the current polygon section */
		int x2 = 0;
		int y2 = 0;
		maps_plugin_geography_to_screen(cp.poly_coords[0], &x2, &y2);

		/* Draw the current polygon section */
		__render_line(x1, y1, x2, y2,
			      color,
			      __width, __height, pixels);
	}

	/* Draw lines, connecting all vertices */
	for (int vertix = 0; vertix < cp.poly_size; vertix ++) {
		for (int i = 0; i < cp.poly_size; i ++) {
			if (i == vertix)
				continue;
			/* Start of the current polygon section */
			int x1 = 0;
			int y1 = 0;
			maps_plugin_geography_to_screen(
						cp.poly_coords[vertix],
						&x1, &y1);

			/* Finish of the current polygon section */
			int x2 = 0;
			int y2 = 0;
			maps_plugin_geography_to_screen(cp.poly_coords[i],
							&x2, &y2);

			/* Draw the current polygon section */
			__render_line(x1, y1, x2, y2,
				      color,
				      __width, __height, pixels);
		}
	}

	/* Release all the points and the array */
	for (int i = 0; i < cp.poly_size; i ++)
		maps_coordinates_destroy(cp.poly_coords[i]);
	g_free(cp.poly_coords);
}

static void __draw_marker(const map_object_h object, unsigned int *pixels)
{
	/*g_print("__draw_marker\n");*/
	if (!object || !pixels)
		return;

	/* Get the Marker geographical coordinate */
	maps_coordinates_h coords = NULL;
	int error = map_object_marker_get_coordinates(object, &coords);
	if (error != MAPS_ERROR_NONE)
		return;

	/* Convert geographical coordinate to screen position */
	int x = 0;
	int y = 0;
	error = maps_plugin_geography_to_screen(coords, &x, &y);
	if (error != MAPS_ERROR_NONE)
		return;
	maps_coordinates_destroy(coords);

	/* Get the Makre screen size */
	int screen_width = 0;
	int screen_height = 0;
	error = map_object_marker_get_size(object,
					   &screen_width,
					   &screen_height);
	if (error != MAPS_ERROR_NONE)
		return;

	/* Draw the marker */
	map_marker_type_e type = MAP_MARKER_NONE;
	map_object_marker_get_type(object, &type);
	switch(type) {
#ifdef TIZEN_3_0_NEXT_MS
	case MAP_MARKER_START:
	case MAP_MARKER_FINISH: {
		const int dx = screen_width / 2;
		const int dy = screen_height;

		/* TODO: use proper colors */
		__render_line(x, y, x - dx, y - dy,
			      0x0000ff00,  __width, __height, pixels);
		__render_line(x, y, x + dx, y - dy,
			      0x0000ff00,  __width, __height, pixels);
		__render_line(x - dx, y - dy, x + dx, y - dy,
			      0x0000ff00,  __width, __height, pixels);
		break;
	}
#endif /* TIZEN_3_0_NEXT_MS */
	case MAP_MARKER_POI: {
		const int dx = screen_width / 2;
		const int dy = screen_height / 2;

		/* TODO: use proper colors */
		__render_line(x - dx, y - dy, x + dx, y + dy,
			      0x00ff0000,  __width, __height, pixels);
		__render_line(x + dx, y - dy, x - dx, y + dy,
			      0x00ff0000,  __width, __height, pixels);
		break;
	}
#ifdef TIZEN_3_0_NEXT_MS
	case MAP_MARKER_FIRST:
	case MAP_MARKER_SECOND:
	case MAP_MARKER_FAVOURITE:
	case MAP_MARKER_CONSTRUCTION:
#endif /* TIZEN_3_0_NEXT_MS */
	case MAP_MARKER_NONE:
	default: {
		const int dx = screen_width / 2;
		const int dy = screen_height / 2;

		/* TODO: use proper colors */
		__render_line(x, y - dy, x, y + dy,
			      0x000000ff,  __width, __height, pixels);
		__render_line(x - dx, y, x + dx, y,
			      0x000000ff,  __width, __height, pixels);
		  break;
	}
	}


}


static bool __for_each_map_object_cb(int index, int total,
					  map_object_h object,
					  void *user_data)
{
	/*g_print("__for_each_map_object_cb\n");*/

	if (!object)
		return false;

	bool visible = false;
	int error = map_object_get_visible(object, &visible);
	if (error != MAPS_ERROR_NONE)
		return false;

	if (!visible)
		return true; /* This object should not be drawn */

	/* Extract the type of the object */
	map_object_type_e type;
	error = map_object_get_type(object, &type);
	if (error != MAPS_ERROR_NONE)
		return false;

	unsigned int *pixels = (unsigned int *)user_data;
	switch(type) {
	case MAP_OBJECT_POLYLINE:
		__draw_polyline(object, pixels);
		break;
	case MAP_OBJECT_POLYGON:
		__draw_polygon(object, pixels);
		break;
	case MAP_OBJECT_MARKER:
		__draw_marker(object, pixels);
		break;
#ifdef TIZEN_3_0_NEXT_MS
	case MAP_OBJECT_ROUTE: {
		/* The Maps Plugin is in charge of drawing the route */


		maps_plugin_route *route = __get_route(object);
		if(!route)
			break;

		__draw_group(route->group, pixels);

		break;
	}
#endif /* TIZEN_3_0_NEXT_MS */
	default:
		g_print("ERROR! Unsupported Maps View Object Type!\n");
		break;
	}
	/*map_object_destroy(object);*/
	return true;
}

static bool __perform_render_map()
{
	__pixels = NULL;

	const int width = __width;
	const int height = __height;

	/* Apply the image file to the maps pannel */
	if (!__img) {
		map_view_get_panel(__map_view, &__img);
		evas_object_image_file_set(__img, "/tmp/maps/tizen.jpg", NULL);
		int err = evas_object_image_load_error_get(__img);
		if (err != EVAS_LOAD_ERROR_NONE) {
			g_print("could not load image %s: %s\n",
				"/tmp/maps/tizen.jpg",
				evas_load_error_str((Evas_Load_Error)err));
			return false;
		}
	}


	/* Perform some drawing to immitate the computationally complex maps
	 * drawing */
	const int total_pixels = width * height;
	unsigned int *pixels =
		(unsigned int *)evas_object_image_data_get(__img, EINA_TRUE);
	for (int i = 0; i < total_pixels; i ++)
		pixels[i] = 0xff11AADD;
		/*pixels[i] =
		* double(__view_request_no) / width * int(0xff11AADD);*/

	/* Draw the border */
	__render_line(0, 0, width - 1, 0,
		      0x000000ff, width, height, pixels);
	__render_line(0, height - 1, width - 1, height - 1,
		      0x000000ff, width, height, pixels);
	__render_line(0, 0, 0, height - 1,
		      0x000000ff, width, height, pixels);
	__render_line(width - 1, 0, width - 1, height - 1,
		      0x000000ff, width, height, pixels);
	__render_line(0, 0, width - 1, height - 1,
		      0x000000ff, width, height, pixels);
	__render_line(width - 1, 0, 0, height - 1,
		      0x000000ff, width, height, pixels);

#if 0
	int center_x = width / 2;
	int center_y = height / 2;
	/* Draw something */
	int cnt = int(__view_request_no % (width / 2 - 1));
	for (int i = 0; i < cnt; i ++) {
		int cur_left = center_x - i;
		int cur_right = center_x + i;
		int cur_top = center_y - i;
		int cur_bottom = center_y + i;

		unsigned int color = __demo_palette[i % __palette_size];

		/* Apply zoom to change the palette */
		unsigned char r = __zoom_factor * ((color & 0x00FF0000) >> 16);
		unsigned char g = __zoom_factor * ((color & 0x0000FF00) >> 8);
		unsigned char b = __zoom_factor * (color & 0x0000FF);
		color = (int(r) << 16) | (int(g) << 8) | b;

		for (int j = cur_left; j <= cur_right; j ++) {
			pixels[cur_top * width + j] = color;
			pixels[cur_bottom * width + j] = color;
		}
		for (int j = cur_top; j <= cur_bottom; j ++) {
			pixels[j * width + cur_left] = color;
			pixels[j * width + cur_right] = color;
		}
	}
#endif

	/* Draw the visual objects */
	map_view_foreach_object(__map_view,
				 __for_each_map_object_cb,
				 pixels);

	/* Draw the current center position */
	if (__center_coords) {
		int map_center_x = 0;
		int map_center_y = 0;

		maps_coordinates_h cur_center = NULL;
		maps_coordinates_clone(__center_coords, &cur_center);
		maps_plugin_geography_to_screen(cur_center,
						&map_center_x, &map_center_y);
		maps_coordinates_destroy(cur_center);

		__render_line(0, map_center_y, width - 1, map_center_y,
			      0x00ff0080, width, height, pixels);
		__render_line(map_center_x, 0, map_center_x, height - 1,
			      0x00ff0080, width, height, pixels);
	}

	__pixels = pixels;


	return true;
}

EXPORT_API int maps_plugin_render_map(const maps_coordinates_h coords,
				     const double zoom_factor,
				     const double rotation_angle,
				     maps_plugin_render_map_cb callback,
				     void* user_data,
				     int* request_id)
{
	/*g_print("Render Map:  %d\n", __get_milli_count());*/

	if (!coords || !callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	*request_id = dummy_geocode_request_id++;

	/* Store the map request params (uncomment when supported) */
	__zoom_factor = zoom_factor;
	__rotation_angle = rotation_angle;

	/* Store current center coordinates */
	if (__center_coords)
		maps_coordinates_destroy(__center_coords);
	maps_coordinates_clone(coords, &__center_coords);

	/* Fetch map on the Provider */
	__perform_render_map();

	__view_request_no += 10;


	callback(MAPS_ERROR_NONE, *request_id,
		 maps::coordinates(40.0, 30.0).clone(), /* TODO: it must be a
							   center point */
		 maps::area(60.0, 10.0, 20.0, 50.0).clone(),
		 user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_move_center(const int delta_x,
				       const int delta_y,
				       maps_plugin_render_map_cb callback,
				       void* user_data,
				       int* request_id)
{
	/*g_print("Render Map:  %d\n", __get_milli_count());*/

	if (!callback || !request_id)
		return MAPS_ERROR_INVALID_PARAMETER;

	*request_id = dummy_geocode_request_id++;

	/* Get previous center coordinates */
	int center_x = 0;
	int center_y = 0;
	maps_plugin_geography_to_screen(__center_coords,
					  &center_x,
					  &center_y);
	center_x += delta_x;
	center_y += delta_y;

	/* Release previous center coordinates */
	if (__center_coords) {
		maps_coordinates_destroy(__center_coords);
		__center_coords = NULL;
	}

	/* Update center coordinates */
	maps_coordinates_h center_coords = NULL;
	maps_plugin_screen_to_geography(center_x, center_y, &center_coords);
	maps_coordinates_clone(center_coords, &__center_coords);
	maps_coordinates_destroy(center_coords);

	/* Fetch map on the Provider */
	__perform_render_map();

	__view_request_no += 10;


	callback(MAPS_ERROR_NONE, *request_id,
		 maps::coordinates(40.0, 30.0).clone(), /* TODO: it must be a
							   center point */
		 maps::area(60.0, 10.0, 20.0, 50.0).clone(),
		 user_data);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_draw_map(Evas* canvas, const int x, const int y,
				    const int width, const int height)
{
	/*g_print("Draw Map:    %d\n", __get_milli_count());*/

	if (!__map_view || !canvas)
		return MAPS_ERROR_INVALID_PARAMETER;

	/*g_print("maps_plugin_draw_map:\n"
		"\tx,y: %d,%d  w,h: %d,%d\n"
		"\tzoom: %.2f, rotation: %.2f\n",
		x, y, width, height, __zoom_factor, __rotation_angle);*/

	if (__img && __pixels) {
		evas_object_image_data_set(__img, __pixels);
		evas_object_image_data_update_add(__img, 0, 0, width, height);
	}

#if 0
	/* Analyze the frame rate */
	if (__last_redraw_time == 0)
		__last_redraw_time = __get_milli_count();
	else {
		int milsec = __get_milli_span(__last_redraw_time);
		__last_redraw_time = __get_milli_count();
		/*g_print("\t\t\t...redraw %d...\n", __view_request_no);*/
		g_print("\t\t\t...redraw after %d ms...\n", milsec);
	}
#endif


	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_on_object(const map_object_h object,
			       const map_object_operation_e operation)
{
	/* Construct and store internally a Route visual representation */
	if(!object)
		return MAPS_ERROR_INVALID_PARAMETER;

	switch(operation) {

	case MAP_OBJECT_ADD: {

		map_object_type_e type;
		map_object_get_type(object, &type);
#ifdef TIZEN_3_0_NEXT_MS
		if(type != MAP_OBJECT_ROUTE)
			break; /* We are interested only in Route */
#endif /* TIZEN_3_0_NEXT_MS */

		break;
	}

	case MAP_OBJECT_SET_VISIBLE:
		/* Ignore */
		break;

	case MAP_OBJECT_MOVE:

		/* TODO: */

		break;

	case MAP_OBJECT_CHANGE: {

#ifdef TIZEN_3_0_NEXT_MS
		map_object_type_e type = MAP_OBJECT_UNKNOWN;
		map_object_get_type(object, &type);

		if(type != MAP_OBJECT_ROUTE)
			break; /* We are interested only in Route */

		/* Extract the Route Content from the Map View object */
		maps_route_h route_content = NULL;
		int error = map_object_route_get_content(object,
							       &route_content);
		if(error != MAPS_ERROR_NONE)
			return error;

		/* Construct the plugin representation of the route and store it
		 *  internally */
		__store_route(object,
			      new maps_plugin_route(object, route_content));
#endif /* TIZEN_3_0_NEXT_MS */

		break;
	}

	case MAP_OBJECT_REMOVE:
#ifdef TIZEN_3_0_NEXT_MS
		__remove_route(object);
#endif /* TIZEN_3_0_NEXT_MS */
		break;

	default:
		/* Unknown operation */
		return MAPS_ERROR_INVALID_PARAMETER;

	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_screen_to_geography(const int x, const int y,
					       maps_coordinates_h* coords)
{
	if (!coords || (x < 0) || (y < 0 ))
		return MAPS_ERROR_INVALID_PARAMETER;

	if (!__map_view)
		return MAPS_ERROR_INVALID_PARAMETER;

	if ((__width <= 0) || (__height <= 0))
		__extract_view_geometry();

	int error = MAPS_ERROR_NONE;

	do {
		double lon = double(x) / __width * 360. - 180.;
		double lat = double(y) / __height * 180. - 90;

		if (lat <= -90) {
			g_print("latitude is out of range: %f\n", lat);
			lat = -90;
		}
		if (lat >= 90) {
			g_print("latitude is out of range: %f\n", lat);
			lat = 90;
		}
		if (lon <= -180) {
			g_print("longitude is out of range: %f\n", lon);
			lon = -180;
		}
		if (lon >= 180) {
			g_print("longitude is out of range: %f\n", lon);
			lon = 180;
		}

		error = maps_coordinates_create(lat, lon, coords);
		if (error != MAPS_ERROR_NONE)
			break;

		return MAPS_ERROR_NONE;
	} while(false);

	return error;
}

EXPORT_API int maps_plugin_geography_to_screen(const maps_coordinates_h
					       coords,
					       int* x, int* y)
{
	if (!coords || !x || !y)
		return MAPS_ERROR_INVALID_PARAMETER;

	if (!__map_view)
		return MAPS_ERROR_INVALID_PARAMETER;

	if ((__width <= 0) || (__height <= 0))
		__extract_view_geometry();

	int error = MAPS_ERROR_NONE;

	do {
		double lat = .0;
		error = maps_coordinates_get_latitude(coords, &lat);
		if (error != MAPS_ERROR_NONE)
			break;

		double lon = .0;
		error = maps_coordinates_get_longitude(coords, &lon);
		if (error != MAPS_ERROR_NONE)
			break;

		/*error = maps_coordinates_destroy(coords);
		if (error != MAPS_ERROR_NONE)
			break;*/

		*x = __width * (lon + 180.) / 360.;
		*y = __height * (lat + 90.) / 180.;

		if (*x < 0) {
			g_print("x is out of range %d\n", *x);
			*x = 0;
		}
		if (*x >  __width) {
			g_print("x is out of range %d\n", *x);
			*x = __width - 1;
		}
		if (*y < 0) {
			g_print("y is out of range %d\n", *y);
			*y = 0;
		}
		if (*y > (__height)) {
			g_print("y is out of range %d\n", *y);
			*y = __height - 1;
		}

		return MAPS_ERROR_NONE;
	} while(false);

	return error;
}

EXPORT_API int maps_plugin_get_min_zoom_level(int *min_zoom_level)
{
	if(!min_zoom_level)
		return MAPS_ERROR_INVALID_PARAMETER;
	*min_zoom_level = 2;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_plugin_get_max_zoom_level(int *max_zoom_level)
{
	if(!max_zoom_level)
		return MAPS_ERROR_INVALID_PARAMETER;
	*max_zoom_level = 18;
	return MAPS_ERROR_NONE;
}
