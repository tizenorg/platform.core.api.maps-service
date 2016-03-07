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

#include "maps_view_test.h"
#include "maps_view.h"
#include "maps_error.h"
#include <glib.h>


/*int maps_view_create(const maps_service_h maps,
		     Evas_Object *parent,
		     maps_view_h *view);*/
void utc_maps_view_create_destroy_p(void)
{
	/* TBD */
}

void utc_maps_view_create_destroy_n(void)
{
	/* TBD */
}


/*----------------------------------------------------------------------------*/
class test_env
{
 public:
	maps_view_h h;
 public:
	test_env() : h(NULL)
	{
		/*
		 * maps_service_create
		 * const int error = maps_view_create(&v);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(v);*/
	}
	~test_env()
	{
		/*const int error = maps_view_destroy(v);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);*/
	}
};


/*int maps_view_show_location(const maps_view_h view,
			    const maps_coordinates_h coordinates);*/
void utc_maps_view_show_location_p(void)
{
	/* TBD */
}

void utc_maps_view_show_location_n(void)
{
	/* TBD */
}


/*int maps_view_screen_to_geolocation(const maps_view_h view,
				  const int x, const int y,
				  maps_coordinates_h *coordinates);*/
void utc_maps_view_screen_to_geolocation_p(void)
{
	/* TBD */
}

void utc_maps_view_screen_to_geolocation_n(void)
{
	/* TBD */
}


/*int maps_view_geolocation_to_screen(const maps_view_h view,
				  const maps_coordinates_h coordinates,
				  int *x, int *y);*/
void utc_maps_view_geolocation_to_screen_p(void)
{
	/* TBD */
}

void utc_maps_view_geolocation_to_screen_n(void)
{
	/* TBD */
}


/*int maps_view_get_panel(const maps_view_h view, Evas_Object **panel);*/
void utc_maps_view_get_panel_p(void)
{
	/* TBD */
}

void utc_maps_view_get_panel_n(void)
{
	/* TBD */
}


/*int maps_view_resize(const maps_view_h view, const int width,
		       const int height);*/
void utc_maps_view_resize_p(void)
{
	/* TBD */
}

void utc_maps_view_resize_n(void)
{
	/* TBD */
}


/*int maps_view_get_size(const maps_view_h view, int *width, int *height);*/
void utc_maps_view_get_size_p(void)
{
	/* TBD */
}

void utc_maps_view_get_size_n(void)
{
	/* TBD */
}


/*int maps_view_move(const maps_view_h view, const int x, const int y);*/
void utc_maps_view_move_p(void)
{
	/* TBD */
}

void utc_maps_view_move_n(void)
{
	/* TBD */
}


/*int maps_view_get_pos(const maps_view_h view, int *x, int *y);*/
void utc_maps_view_get_pos_p(void)
{
	/* TBD */
}

void utc_maps_view_get_pos_n(void)
{
	/* TBD */
}


/*int maps_view_show(const maps_view_h view);*/
void utc_maps_view_show_p(void)
{
	/* TBD */
}

void utc_maps_view_show_n(void)
{
	/* TBD */
}


/*int maps_view_hide(const maps_view_h view);*/
void utc_maps_view_hide_p(void)
{
	/* TBD */
}

void utc_maps_view_hide_n(void)
{
	/* TBD */
}


/*int maps_view_is_visible(const maps_view_h view, bool *visible);*/
void utc_maps_view_is_visible_p(void)
{
	/* TBD */
}

void utc_maps_view_is_visible_n(void)
{
	/* TBD */
}


/*int _maps_view_redraw(const maps_view_h view);*/
void utc_maps_view_redraw_p(void)
{
	/* TBD */
}

void utc_maps_view_redraw_n(void)
{
	/* TBD */
}


/*int maps_view_zoom(const maps_view_h view, const double factor);*/
void utc_maps_view_zoom_p(void)
{
	/* TBD */
}

void utc_maps_view_zoom_n(void)
{
	/* TBD */
}


/*int maps_view_get_zoom_factor(const maps_view_h view, double *factor);*/
void utc_maps_view_get_zoom_factor_p(void)
{
	/* TBD */
}

void utc_maps_view_get_zoom_factor_n(void)
{
	/* TBD */
}


/*int maps_view_rotate(const maps_view_h view, const double angle);*/
void utc_maps_view_rotate_p(void)
{
	/* TBD */
}

void utc_maps_view_rotate_n(void)
{
	/* TBD */
}


/*int maps_view_get_rotation_angle(const maps_view_h view, double *angle);*/
void utc_maps_view_get_rotation_angle_p(void)
{
	/* TBD */
}

void utc_maps_view_get_rotation_angle_n(void)
{
	/* TBD */
}


/*typedef void(*maps_view_on_gesture_cb) (maps_error_e result,
				     const int x, const int y,
				     const maps_view_gesture_e g,
				     void *user_data);
int maps_view_set_on_gesture_callback(maps_view_h view,
				      maps_view_on_gesture_cb callback,
				      void *user_data);*/
void utc_maps_view_set_on_gesture_callback_p(void)
{
	/* TBD */
}

void utc_maps_view_set_on_gesture_callback_n(void)
{
	/* TBD */
}


/*typedef void(*maps_view_on_zoom_cb) (maps_error_e result,
				     const int x, const int y,
				     const double factor,
				     void *user_data);
int maps_view_set_on_zoom_callback(maps_view_h view,
				   maps_view_on_zoom_cb callback,
				   void *user_data);*/
void utc_maps_view_set_on_zoom_callback_p(void)
{
	/* TBD */
}

void utc_maps_view_set_on_zoom_callback_n(void)
{
	/* TBD */
}


/*typedef void(*maps_view_on_rotate_cb) (maps_error_e result,
				       const int x, const int y,
				       const double angle,
				       void *user_data);
int maps_view_set_on_rotate_callback(maps_view_h view,
				     maps_view_on_rotate_cb callback,
				     void *user_data);*/
void utc_maps_view_set_on_rotate_callback_p(void)
{
	/* TBD */
}

void utc_maps_view_set_on_rotate_callback_n(void)
{
	/* TBD */
}


/*typedef void(*maps_view_on_object_cb) (maps_error_e result,
				       const int x, const int y,
				       const maps_view_gesture_e g,
				       const maps_view_object_h object,
				       void *user_data);
int maps_view_set_on_object_callback(maps_view_h view,
				     maps_view_on_object_cb callback,
				     void *user_data);*/
void utc_maps_view_set_on_object_callback_p(void)
{
	/* TBD */
}

void utc_maps_view_set_on_object_callback_n(void)
{
	/* TBD */
}


/*typedef void(*maps_view_on_ready_cb) (maps_error_e result, void *user_data);
int maps_view_set_on_ready_callback(maps_view_h view,
				    maps_view_on_ready_cb callback,
				    void *user_data);*/
void utc_maps_view_set_on_ready_callback_p(void)
{
	/* TBD */
}

void utc_maps_view_set_on_ready_callback_n(void)
{
	/* TBD */
}


/*int maps_view_set_gesture_action(maps_view_h view,
				 const maps_view_gesture_e gesture,
				 const maps_view_action_e action);*/
void utc_maps_view_set_gesture_action_p(void)
{
	/* TBD */
}

void utc_maps_view_set_gesture_action_n(void)
{
	/* TBD */
}


/*int maps_view_disable_gesture(maps_view_h view,
			      const maps_view_gesture_e gesture);*/
void utc_maps_view_disable_gesture_p(void)
{
	/* TBD */
}

void utc_maps_view_disable_gesture_n(void)
{
	/* TBD */
}


/*int maps_view_add_marker_type(maps_view_h view,
			      const maps_coordinates_h coordinates,
			      const maps_view_marker_type_e type,
			      maps_view_object_h *marker);*/
void utc_maps_view_add_marker_type_p(void)
{
	/* TBD */
}

void utc_maps_view_add_marker_type_n(void)
{
	/* TBD */
}


/*int maps_view_add_marker_image(maps_view_h view,
			       const maps_coordinates_h coordinates,
			       const char *image_file_name,
			       maps_view_object_h *marker);*/
void utc_maps_view_add_marker_image_p(void)
{
	/* TBD */
}

void utc_maps_view_add_marker_image_n(void)
{
	/* TBD */
}


/*int maps_view_add_route(maps_view_h view,
			const maps_route_h route,
			maps_view_object_h *route_object);*/
void utc_maps_view_add_route_p(void)
{
	/* TBD */
}

void utc_maps_view_add_route_n(void)
{
	/* TBD */
}


/*int maps_view_add_image(maps_view_h view,
			const char *image_file_name,
			const maps_item_list_h coordinates,
			const double width, const double height,
			maps_view_object_h *image);*/
void utc_maps_view_add_image_p(void)
{
	/* TBD */
}

void utc_maps_view_add_image_n(void)
{
	/* TBD */
}


/*int maps_view_add_polyline(maps_view_h view,
			   const maps_item_list_h coordinates,
			   const unsigned char r,
			   const unsigned char g,
			   const unsigned char b,
			   const unsigned char a,
			   maps_view_object_h *polyline);*/
void utc_maps_view_add_polyline_p(void)
{
	/* TBD */
}

void utc_maps_view_add_polyline_n(void)
{
	/* TBD */
}


/*int maps_view_add_polygon(maps_view_h view,
			  const maps_item_list_h coordinates,
			  const unsigned char r,
			  const unsigned char g,
			  const unsigned char b,
			  const unsigned char a,
			  maps_view_object_h *polygon);*/
void utc_maps_view_add_polygon_p(void)
{
	/* TBD */
}

void utc_maps_view_add_polygon_n(void)
{
	/* TBD */
}


/*int maps_view_add_circle(maps_view_h view,
			 const maps_coordinates_h coordinates,
			 const double radius,
			 const unsigned char r,
			 const unsigned char g,
			 const unsigned char b,
			 const unsigned char a,
			 maps_view_object_h *circle);*/
void utc_maps_view_add_circle_p(void)
{
	/* TBD */
}

void utc_maps_view_add_circle_n(void)
{
	/* TBD */
}


/*int maps_view_add_text(maps_view_h view,
		       const maps_coordinates_h coordinates,
		       const char *str,
		       const unsigned char r,
		       const unsigned char g,
		       const unsigned char b,
		       const unsigned char a,
		       maps_view_object_h *text);*/
void utc_maps_view_add_text_p(void)
{
	/* TBD */
}

void utc_maps_view_add_text_n(void)
{
	/* TBD */
}


/*int maps_view_add_object(const maps_view_h view,
			 const maps_view_object_h object);*/
void utc_maps_view_add_object_p(void)
{
	/* TBD */
}

void utc_maps_view_add_object_n(void)
{
	/* TBD */
}


/*int maps_view_add_objects(const maps_view_h view,
			  const maps_item_list_h list);*/
void utc_maps_view_add_objects_p(void)
{
	/* TBD */
}

void utc_maps_view_add_objects_n(void)
{
	/* TBD */
}


/*int maps_view_remove_object(maps_view_h view,
			    const maps_view_object_h object);*/
void utc_maps_view_remove_object_p(void)
{
	/* TBD */
}

void utc_maps_view_remove_object_n(void)
{
	/* TBD */
}


/*typedef bool(*maps_view_object_cb) (int index, int total,
				     maps_view_object_h object,
				     void *user_data);
int maps_view_foreach_object(const maps_view_h view,
			     maps_view_object_cb callback,
			     void *user_data);*/
void utc_maps_view_foreach_object_p(void)
{
	/* TBD */
}

void utc_maps_view_foreach_object_n(void)
{
	/* TBD */
}

