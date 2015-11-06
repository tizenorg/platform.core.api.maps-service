/* Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __MAPS_VIEW_RUNTIME_DATA_H__
#define __MAPS_VIEW_RUNTIME_DATA_H__


#include "map_view.h"

#define MAX_FINGERS 10

namespace view
{
	/* The info about any touch point or move point, which includes
	 *  screen canvas coordinates and a time stamp */
	class touch_point {
	public:
		int _x;
		int _y;
		unsigned int _timestamp;
	public:
		touch_point();
		touch_point(const int x, const int y, const unsigned int ts);
		touch_point(const touch_point &src);
		touch_point &operator=(const touch_point &src);
		~touch_point();
	public:
		void reset();
		bool empty() const;
	};


	/* -------------------------------------------------------------------*/


	/* The info about the basic parameters of the Map View, representing
	 *  its state, including: geographical center coordinates, zoom factor,
	 *  and rotation angle */
	class map_state {
	public:
		maps_coordinates_h _center;
		double _zoom_factor;
		double _rotation_angle;
	public:
		map_state();
		map_state(const map_state &src);
		map_state &operator=(const map_state &rc);
		~map_state();
	public:
		void reset();
		void capture(map_view_h view);
	public:
		void trace();
	};


	/* -------------------------------------------------------------------*/


	/* The info about intermediate gesture state, which includes
	 *  the starting points of gesture fingers; if gesture is still lasting,
	 *  there are provided currently tapped points under user's fingers,
	 *  and if the gesture is just finished, the up points are available.
	 *  The initial state of the Map View is provided as well */
	class runtime_touch_info {
	public:
		//static const unsigned int MAX_FINGERS = 10;
		int _fingers_pressed; /* Number of fingers */

		touch_point _finger_down[MAX_FINGERS]; /* Finger down */
		touch_point _finger_move[MAX_FINGERS];

		/* Following is needed for move_center while panning */
		touch_point _prev_finger_down[MAX_FINGERS];

		touch_point _finger_up[MAX_FINGERS]; /* Finger up */


		bool _is_finger_pressed[MAX_FINGERS];

		/* Unique gestures for map view, such as
		 *  "double tap and pan" for zooming */
		map_gesture_e _cur_gesture;
		
		/*bool _going;*/ /* Do we need this? */

		map_state _start_view_state;
	public:
		runtime_touch_info();
		~runtime_touch_info();
		runtime_touch_info(const runtime_touch_info &src);
		runtime_touch_info &operator=(const runtime_touch_info &src);
	public:
		void reset();
		int calc_finger_pressed();
	};
};

#endif				/* __MAPS_VIEW_RUNTIME_DATA_H__ */
