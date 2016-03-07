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

#ifndef __MAPS_VIEW_TEST_ROUTE_CONSTRUCTOR_H__
#define __MAPS_VIEW_TEST_ROUTE_CONSTRUCTOR_H__


#ifdef TIZEN_3_0_NEXT_MS
void utc_construct_route_p(void);
void utc_construct_route_n(void);

	/* !!! The old marker constructor will be deleted soom!!! */
#if 0
/*void static const char *get_start_marker_image();*/
void utc_get_start_marker_p(void);

/*static const char *get_finish_marker_image();*/
void utc_get_finish_marker_p(void);

/*static const char *get_default_marker_image();*/
void utc_get_default_marker_p(void);

/*static int add_marker(maps_view_object_h route_object,
				       const maps_coordinates_h coords,
				       maps_view_marker_type_e type);*/
void utc_add_marker_p(void);
void utc_add_marker_n(void);

/*void add_start_marker(maps_view_object_h route_object,
		      maps_route_h route);*/
void utc_add_start_marker_p(void);
void utc_add_start_marker_n(void);

/*void add_finish_marker(maps_view_object_h route_object,
		       maps_route_h route);*/
void utc_add_finish_marker_p(void);
void utc_add_finish_marker_n(void);

/*static int add_trajectory(maps_view_object_h route_object,
			  maps_item_list_h route_points);*/
void utc_add_trajectory_p(void);
void utc_add_trajectory_n(void);

/*static bool add_trajectory_markers(int index,
				   int total,
				   void *data,
				   void *user_data);*/
void utc_add_trajectory_markers_p(void);
void utc_add_trajectory_markers_n(void);

/*void add_route_path(maps_view_object_h route_object,
		    maps_route_h route);*/
void utc_add_route_path_p(void);
void utc_add_route_path_n(void);

/*static bool collect_path_points(int index, int total,
				maps_coordinates_h coordinates,
				void *user_data);*/
void utc_collect_path_points_p(void);
void utc_collect_path_points_n(void);

/*void add_route_segments(maps_view_object_h route_object,
			maps_route_h route);*/
void utc_add_route_segments_p(void);
void utc_add_route_segments_n(void);

/*static bool collect_segments(int index, int total,
			     maps_route_segment_h segment,
			     void *user_data);*/
void  utc_collect_segments_p(void);
void  utc_collect_segments_n(void);
#endif

#endif /* TIZEN_3_0_NEXT_MS */

#endif /* __MAPS_VIEW_TEST_ROUTE_CONSTRUCTOR_H__ */
