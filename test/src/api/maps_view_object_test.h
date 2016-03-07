/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __MAP_OBJECT_API_TEST_H__
#define __MAP_OBJECT_API_TEST_H__


/*int maps_view_object_create(const maps_view_object_type_e type,
			    maps_view_object_h *object);
int maps_view_object_destroy(maps_view_object_h object);*/
void utc_maps_view_object_create_p(void);
void utc_maps_view_object_create_n(void);

/*int maps_view_object_get_type(const maps_view_object_h object,
			      maps_view_object_type_e *type);*/
void utc_maps_view_object_get_type_p(void);
void utc_maps_view_object_get_type_n(void);

/*int maps_view_object_move(maps_view_object_h object,
			 const maps_coordinates_h coordinates);*/
void utc_maps_view_object_move_p(void);
void utc_maps_view_object_move_n(void);

/*int maps_view_object_set_visible(maps_view_object_h object,
				   const bool visible);*/
void utc_maps_view_object_set_visible_p(void);
void utc_maps_view_object_set_visible_n(void);

/*int maps_view_object_get_visible(maps_view_object_h object, bool *visible);*/
void utc_maps_view_object_get_visible_p(void);
void utc_maps_view_object_get_visible_n(void);

/*int maps_view_object_hit_test(maps_view_object_h text,
			      const maps_coordinates_h coordinates,
			      bool *is_hit);*/
void utc_maps_view_object_hit_test_p(void);
void utc_maps_view_object_hit_test_n(void);

/*int maps_view_object_group_add_object(maps_view_object_h group,
			       const maps_view_object_h object);*/
void utc_maps_view_object_group_add_object_p(void);
void utc_maps_view_object_group_add_object_n(void);

/*int maps_view_object_group_set_objects(maps_view_object_h group,
				     const maps_item_list_h objects);*/
void utc_maps_view_object_group_set_objects_p(void);
void utc_maps_view_object_group_set_objects_n(void);

/*int maps_view_object_group_remove_object(maps_view_object_h group,
				  const maps_view_object_h *object);*/
void utc_maps_view_object_group_remove_object_p(void);
void utc_maps_view_object_group_remove_object_n(void);

/*int maps_view_object_group_foreach_object(const maps_view_object_h group,
				   maps_view_object_group_object_cb callback,
typedef bool(*maps_view_object_group_object_cb) (int index, int total,
					      maps_view_object_h object,
					      void *user_data);*/
void utc_maps_view_object_group_foreach_object_p(void);
void utc_maps_view_object_group_foreach_object_n(void);

/*int maps_view_object_poly_add_point(maps_view_object_h poly,
					const maps_coordinates_h point);*/
void utc_maps_view_object_poly_add_point_p(void);
void utc_maps_view_object_poly_add_point_n(void);

/*int maps_view_object_poly_set_points(maps_view_object_h poly,
				  const maps_item_list_h points);*/
void utc_maps_view_object_poly_set_points_p(void);
void utc_maps_view_object_poly_set_points_n(void);

/*int maps_view_object_poly_foreach_point(maps_view_object_h poly,
				       maps_view_object_poly_point_cb callback,
typedef bool(*maps_view_object_poly_point_cb) (int index, int total,
					      maps_coordinates_h point,
					      void *user_data);*/
void utc_maps_view_object_poly_foreach_point_p(void);
void utc_maps_view_object_poly_foreach_point_n(void);

/*int maps_view_object_polyline_set_color(maps_view_object_h polyline,
				       const unsigned char r,
				       const unsigned char g,
				       const unsigned char b,
				       const unsigned char a);*/
void utc_maps_view_object_polyline_set_color_p(void);
void utc_maps_view_object_polyline_set_color_n(void);

/*int maps_view_object_polyline_get_color(const maps_view_object_h
				       polyline,
				       unsigned char *r,
				       unsigned char *g,
				       unsigned char *b,
				       unsigned char *a);*/
void utc_maps_view_object_polyline_get_color_p(void);
void utc_maps_view_object_polyline_get_color_n(void);

/*int maps_view_object_polyline_set_width(maps_view_object_h polyline,
				       const int width);*/
void utc_maps_view_object_polyline_set_width_p(void);
void utc_maps_view_object_polyline_set_width_n(void);

/*int maps_view_object_polyline_get_width(const maps_view_object_h polyline,
				       int *width);*/
void utc_maps_view_object_polyline_get_width_p(void);
void utc_maps_view_object_polyline_get_width_n(void);

/*int maps_view_object_polygon_set_fill_color(maps_view_object_h polygon,
					   const unsigned char r,
					   const unsigned char g,
					   const unsigned char b,
					   const unsigned char a);*/
void utc_maps_view_object_polygon_set_fill_color_p(void);
void utc_maps_view_object_polygon_set_fill_color_n(void);

/*int maps_view_object_polygon_get_fill_color(const maps_view_object_h polygon,
					   unsigned char *r,
					   unsigned char *g,
					   unsigned char *b,
					   unsigned char *a);*/
void utc_maps_view_object_polygon_get_fill_color_p(void);
void utc_maps_view_object_polygon_get_fill_color_n(void);

/*int maps_view_object_marker_set_coordinates(maps_view_object_h marker,
				   const maps_coordinates_h coordinates);*/
void utc_maps_view_object_marker_set_coordinates_p(void);
void utc_maps_view_object_marker_set_coordinates_n(void);

/*int maps_view_object_marker_set_screen_size(maps_view_object_h marker,
					  const int screen_width,
					  const int screen_height);*/
void utc_maps_view_object_marker_set_screen_size_p(void);
void utc_maps_view_object_marker_set_screen_size_n(void);

/*int maps_view_object_marker_set_file_path(maps_view_object_h marker,
					const char *file_path);*/
void utc_maps_view_object_marker_set_file_path_p(void);
void utc_maps_view_object_marker_set_file_path_n(void);

/*int maps_view_object_marker_set_type(maps_view_object_h marker,
				   const maps_view_marker_type_e type);*/
void utc_maps_view_object_marker_set_type_p(void);
void utc_maps_view_object_marker_set_type_n(void);

/*int maps_view_object_marker_set_text(maps_view_object_h marker,
				   const char *text);*/
void utc_maps_view_object_marker_set_text_p(void);
void utc_maps_view_object_marker_set_text_n(void);

/*int maps_view_object_marker_get_coordinates(const maps_view_object_h marker,
					  maps_coordinates_h *coordinates);*/
void utc_maps_view_object_marker_get_coordinates_p(void);
void utc_maps_view_object_marker_get_coordinates_n(void);

/*int maps_view_object_marker_get_screen_size(maps_view_object_h marker,
					  int *screen_width,
					  int *screen_height);*/
void utc_maps_view_object_marker_get_screen_size_p(void);
void utc_maps_view_object_marker_get_screen_size_n(void);

/*int maps_view_object_marker_get_image_file(const maps_view_object_h marker,
					char **file_path);*/
void utc_maps_view_object_marker_get_image_file_p(void);
void utc_maps_view_object_marker_get_image_file_n(void);

/*int maps_view_object_marker_get_type(const maps_view_object_h marker,
				   maps_view_marker_type_e *type);*/
void utc_maps_view_object_marker_get_type_p(void);
void utc_maps_view_object_marker_get_type_n(void);

/*int maps_view_object_marker_get_text(const maps_view_object_h marker,
				   char **text);*/
void utc_maps_view_object_marker_get_text_p(void);
void utc_maps_view_object_marker_get_text_n(void);

#endif /* __MAP_OBJECT_API_TEST_H__*/

