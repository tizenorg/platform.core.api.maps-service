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

#include "map_view_test.h"
#include "map_view.h"
#include "maps_error.h"
#include <glib.h>


/*int map_view_create(const maps_service_h maps,
		     Evas_Object *parent,
		     map_view_h *view);*/
void utc_map_view_create_destroy_p(void)
{
	/* TBD */
}

void utc_map_view_create_destroy_n(void)
{
	/* TBD */
}


/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	map_view_h h;
 public:
	test_env() : h(NULL)
	{
		/*
		 * maps_service_create
		 * const int error = map_view_create(&v);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(v);*/
	}
	~test_env()
	{
		/*const int error = map_view_destroy(v);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);*/
	}
};


/*int map_view_show_location(const map_view_h view,
			    const maps_coordinates_h coordinates);*/
void utc_map_view_show_location_p(void)
{
	/* TBD */
}

void utc_map_view_show_location_n(void)
{
	/* TBD */
}


/*int map_view_screen_to_geolocation(const map_view_h view,
				  const int x, const int y,
				  maps_coordinates_h *coordinates);*/
void utc_map_view_screen_to_geolocation_p(void)
{
	/* TBD */
}

void utc_map_view_screen_to_geolocation_n(void)
{
	/* TBD */
}


/*int map_view_geolocation_to_screen(const map_view_h view,
				  const maps_coordinates_h coordinates,
				  int *x, int *y);*/
void utc_map_view_geolocation_to_screen_p(void)
{
	/* TBD */
}

void utc_map_view_geolocation_to_screen_n(void)
{
	/* TBD */
}


/*int map_view_get_panel(const map_view_h view, Evas_Object **panel);*/
void utc_map_view_get_panel_p(void)
{
	/* TBD */
}

void utc_map_view_get_panel_n(void)
{
	/* TBD */
}


/*int map_view_resize(const map_view_h view, const int width,
		       const int height);*/
void utc_map_view_resize_p(void)
{
	/* TBD */
}

void utc_map_view_resize_n(void)
{
	/* TBD */
}


/*int map_view_get_size(const map_view_h view, int *width, int *height);*/
void utc_map_view_get_size_p(void)
{
	/* TBD */
}

void utc_map_view_get_size_n(void)
{
	/* TBD */
}


/*int map_view_move(const map_view_h view, const int x, const int y);*/
void utc_map_view_move_p(void)
{
	/* TBD */
}

void utc_map_view_move_n(void)
{
	/* TBD */
}


/*int map_view_get_pos(const map_view_h view, int *x, int *y);*/
void utc_map_view_get_pos_p(void)
{
	/* TBD */
}

void utc_map_view_get_pos_n(void)
{
	/* TBD */
}


/*int map_view_show(const map_view_h view);*/
void utc_map_view_show_p(void)
{
	/* TBD */
}

void utc_map_view_show_n(void)
{
	/* TBD */
}


/*int map_view_hide(const map_view_h view);*/
void utc_map_view_hide_p(void)
{
	/* TBD */
}

void utc_map_view_hide_n(void)
{
	/* TBD */
}


/*int map_view_is_visible(const map_view_h view, bool *visible);*/
void utc_map_view_is_visible_p(void)
{
	/* TBD */
}

void utc_map_view_is_visible_n(void)
{
	/* TBD */
}


/*int map_view_redraw(const map_view_h view);*/
void utc_map_view_redraw_p(void)
{
	/* TBD */
}

void utc_map_view_redraw_n(void)
{
	/* TBD */
}


/*int map_view_zoom(const map_view_h view, const double factor);*/
void utc_map_view_zoom_p(void)
{
	/* TBD */
}

void utc_map_view_zoom_n(void)
{
	/* TBD */
}


/*int map_view_get_zoom_factor(const map_view_h view, double *factor);*/
void utc_map_view_get_zoom_factor_p(void)
{
	/* TBD */
}

void utc_map_view_get_zoom_factor_n(void)
{
	/* TBD */
}


/*int map_view_rotate(const map_view_h view, const double angle);*/
void utc_map_view_rotate_p(void)
{
	/* TBD */
}

void utc_map_view_rotate_n(void)
{
	/* TBD */
}


/*int map_view_get_rotation_angle(const map_view_h view, double *angle);*/
void utc_map_view_get_rotation_angle_p(void)
{
	/* TBD */
}

void utc_map_view_get_rotation_angle_n(void)
{
	/* TBD */
}


/*typedef void(*map_view_on_gesture_cb) (maps_error_e result,
				     const int x, const int y,
				     const map_view_gesture_e g,
				     void *user_data);
int map_view_set_on_gesture_callback(map_view_h view,
				      map_view_on_gesture_cb callback,
				      void *user_data);*/
void utc_map_view_set_on_gesture_callback_p(void)
{
	/* TBD */
}

void utc_map_view_set_on_gesture_callback_n(void)
{
	/* TBD */
}


/*typedef void(*map_view_on_zoom_cb) (maps_error_e result,
				     const int x, const int y,
				     const double factor,
				     void *user_data);
int map_view_set_on_zoom_callback(map_view_h view,
				   map_view_on_zoom_cb callback,
				   void *user_data);*/
void utc_map_view_set_on_zoom_callback_p(void)
{
	/* TBD */
}

void utc_map_view_set_on_zoom_callback_n(void)
{
	/* TBD */
}


/*typedef void(*map_view_on_rotate_cb) (maps_error_e result,
				       const int x, const int y,
				       const double angle,
				       void *user_data);
int map_view_set_on_rotate_callback(map_view_h view,
				     map_view_on_rotate_cb callback,
				     void *user_data);*/
void utc_map_view_set_on_rotate_callback_p(void)
{
	/* TBD */
}

void utc_map_view_set_on_rotate_callback_n(void)
{
	/* TBD */
}


/*typedef void(*map_view_on_object_cb) (maps_error_e result,
				       const int x, const int y,
				       const map_view_gesture_e g,
				       const map_object_h object,
				       void *user_data);
int map_view_set_on_object_callback(map_view_h view,
				     map_view_on_object_cb callback,
				     void *user_data);*/
void utc_map_view_set_on_object_callback_p(void)
{
	/* TBD */
}

void utc_map_view_set_on_object_callback_n(void)
{
	/* TBD */
}


/*typedef void(*map_view_on_ready_cb) (maps_error_e result, void *user_data);
int map_view_set_on_ready_callback(map_view_h view,
				    map_view_on_ready_cb callback,
				    void *user_data);*/
void utc_map_view_set_on_ready_callback_p(void)
{
	/* TBD */
}

void utc_map_view_set_on_ready_callback_n(void)
{
	/* TBD */
}


/*int map_view_set_gesture_action(map_view_h view,
				 const map_view_gesture_e gesture,
				 const map_view_action_e action);*/
void utc_map_view_set_gesture_action_p(void)
{
	/* TBD */
}

void utc_map_view_set_gesture_action_n(void)
{
	/* TBD */
}


/*int map_view_disable_gesture(map_view_h view,
			      const map_view_gesture_e gesture);*/
void utc_map_view_disable_gesture_p(void)
{
	/* TBD */
}

void utc_map_view_disable_gesture_n(void)
{
	/* TBD */
}


/*int map_view_add_marker_type(map_view_h view,
			      const maps_coordinates_h coordinates,
			      const map_marker_type_e type,
			      map_object_h *marker);*/
void utc_map_view_add_marker_type_p(void)
{
	/* TBD */
}

void utc_map_view_add_marker_type_n(void)
{
	/* TBD */
}


/*int map_view_add_marker_image(map_view_h view,
			       const maps_coordinates_h coordinates,
			       const char *image_file_name,
			       map_object_h *marker);*/
void utc_map_view_add_marker_image_p(void)
{
	/* TBD */
}

void utc_map_view_add_marker_image_n(void)
{
	/* TBD */
}


/*int map_view_add_route(map_view_h view,
			const maps_route_h route,
			map_object_h *route_object);*/
void utc_map_view_add_route_p(void)
{
	/* TBD */
}

void utc_map_view_add_route_n(void)
{
	/* TBD */
}


/*int map_view_add_image(map_view_h view,
			const char *image_file_name,
			const maps_item_list_h coordinates,
			const double width, const double height,
			map_object_h *image);*/
void utc_map_view_add_image_p(void)
{
	/* TBD */
}

void utc_map_view_add_image_n(void)
{
	/* TBD */
}


/*int map_view_add_polyline(map_view_h view,
			   const maps_item_list_h coordinates,
			   const unsigned char r,
			   const unsigned char g,
			   const unsigned char b,
			   const unsigned char a,
			   map_object_h *polyline);*/
void utc_map_view_add_polyline_p(void)
{
	/* TBD */
}

void utc_map_view_add_polyline_n(void)
{
	/* TBD */
}


/*int map_view_add_polygon(map_view_h view,
			  const maps_item_list_h coordinates,
			  const unsigned char r,
			  const unsigned char g,
			  const unsigned char b,
			  const unsigned char a,
			  map_object_h *polygon);*/
void utc_map_view_add_polygon_p(void)
{
	/* TBD */
}

void utc_map_view_add_polygon_n(void)
{
	/* TBD */
}


/*int map_view_add_circle(map_view_h view,
			 const maps_coordinates_h coordinates,
			 const double radius,
			 const unsigned char r,
			 const unsigned char g,
			 const unsigned char b,
			 const unsigned char a,
			 map_object_h *circle);*/
void utc_map_view_add_circle_p(void)
{
	/* TBD */
}

void utc_map_view_add_circle_n(void)
{
	/* TBD */
}


/*int map_view_add_text(map_view_h view,
		       const maps_coordinates_h coordinates,
		       const char *str,
		       const unsigned char r,
		       const unsigned char g,
		       const unsigned char b,
		       const unsigned char a,
		       map_object_h *text);*/
void utc_map_view_add_text_p(void)
{
	/* TBD */
}

void utc_map_view_add_text_n(void)
{
	/* TBD */
}


/*int map_view_add_object(const map_view_h view,
			 const map_object_h object);*/
void utc_map_view_add_object_p(void)
{
	/* TBD */
}

void utc_map_view_add_object_n(void)
{
	/* TBD */
}


/*int map_view_add_objects(const map_view_h view,
			  const maps_item_list_h list);*/
void utc_map_view_add_objects_p(void)
{
	/* TBD */
}

void utc_map_view_add_objects_n(void)
{
	/* TBD */
}


/*int map_view_remove_object(map_view_h view,
			    const map_object_h object);*/
void utc_map_view_remove_object_p(void)
{
	/* TBD */
}

void utc_map_view_remove_object_n(void)
{
	/* TBD */
}


/*typedef bool(*map_object_cb) (int index, int total,
				     map_object_h object,
				     void *user_data);
int map_view_foreach_object(const map_view_h view,
			     map_object_cb callback,
			     void *user_data);*/
void utc_map_view_foreach_object_p(void)
{
	/* TBD */
}

void utc_map_view_foreach_object_n(void)
{
	/* TBD */
}

