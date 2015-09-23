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

#ifndef __MAPS_VIEW_GESTURE_PROCESSOR_H__
#define __MAPS_VIEW_GESTURE_PROCESSOR_H__


#include "map_view.h"
#include "command.h"
#include "command_queue.h"

/* Select the way of panning: shift center on delta coordinates or move to the
 * specified central coordinate.
 * If the following MACRO is defined, the shift is performed
 */
#define _MOVE_CENTER_COMMAND_DEFINED_

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
	 *  rotation angle and tilt */
	class map_state {
	public:
		maps_coordinates_h _center;
		double _zoom_factor;
		double _rotation_angle;
		double _tilt;
	public:
		map_state();
		map_state(const map_state &src);
		map_state &operator=(const map_state &rc);
		~map_state();
	public:
		void reset();
		void capture(map_view_h view);
	};


	/* -------------------------------------------------------------------*/


	/* The info about intermediate gesture state, which includes
	 *  the starting points of gesture fingers; if gesture is still lasting,
	 *  there are provided currently tapped points under user's fingers,
	 *  and if the gesture is just finished, the up points are available.
	 *  The initial state of the Map View is provided as well */
	class runtime_touch_info {
	public:
		static const unsigned int MAX_FINGERS = 10;
		int _fingers; /* Number of fingers */

		touch_point _start_touch[MAX_FINGERS]; /* Finger down */
		touch_point _cur_touch[MAX_FINGERS];

		/* Following is needed for move_center while panning */
		touch_point _prev_touch[MAX_FINGERS];

		touch_point _up_touch[MAX_FINGERS]; /* Finger up */


		bool _finger_pressed[MAX_FINGERS];

		/* This field would be useful for complicated gestures, such as
		 *  "double tap and pan" for zooming */
		map_view_gesture_e _gesture_type;

		/*bool _going;*/ /* Do we need this? */

		map_state _start_state;
	public:
		runtime_touch_info();
		~runtime_touch_info();
		runtime_touch_info(const runtime_touch_info &src);
		runtime_touch_info &operator=(const runtime_touch_info &src);
	public:
		void reset();
	};


	/* -------------------------------------------------------------------*/


	/* The service class, allowing to calculate the zoom and rotation
	 *  changes, caused bu two-finger gesture.
	 *  Briefly speaking, the zoom equals to a current distance between two
	 *  fingers, divided on distance, between two fingers in the begining of
	 *  the gesture.
	 *  The rotation angle is an angle between imaginary lines,
	 *  connecting finger positions on the Map View; first line connects
	 *  finger positions in the beginning of the gesture, while the second
	 *  line connects the current finger positions.
	 */
	class zoom_calculator {
	protected:
		touch_point _start_tp_f1;
		touch_point _cur_tp_f1;
		touch_point _start_tp_f2;
		touch_point _cur_tp_f2;
		double _new_zoom_factor;
		double _new_rotation_angle;
		bool _zoom_happend;
		bool _rotation_happend;
	public:
		zoom_calculator(const touch_point &start_tp_f1,
				const touch_point &cur_tp_f1,
				const touch_point &start_tp_f2,
				const touch_point &cur_tp_f2);
		~zoom_calculator() {}
	public:
		double get_zoom_factor() const
		{
			return _new_zoom_factor;
		}
		double get_rotation_angle() const
		{
			return _new_rotation_angle;
		}
		bool zoom_happend() const
		{
			return _zoom_happend;
		}
		bool rotation_happend() const
		{
			return _rotation_happend;
		}
	};


	/* -------------------------------------------------------------------*/


	/* This class prepares and sends session::commands, assigned with a
	 *  gestures.
	 *  Note, it doesn't know how to recognize gestures - it is the
	 *  responsibility of gesture_detector. */
	class gesture_processor {
	public:
		class gesture_detector *_gd;
	public:
		gesture_processor(class gesture_detector *gd);
		~gesture_processor();
	public:
		void on_long_press();
		void on_double_tap();
		void on_tap();
		void on_two_finger_tap();
		void on_flick();
		void on_pan();
		void on_panning_finished();
		void on_tilt();
		void on_zoom_rotate();
		void on_pinch();
	private:
		session::command_queue *q();
		void *get_maps();
		session::command *construct_gesture_command(
						map_view_gesture_e gesture,
						const maps_coordinates_h c,
						const double zoom,
						const double angle,
						const double tilt);
	protected:
		touch_point calc_center(const touch_point &tp1,
					const touch_point &tp2) const;
	protected:
		/* TODO: | DEBUG ONLY | REMOVE WHEN NO NEEDED */
		map_view_object_h __DEBUG_add_marker(
					const int x, const int y,
					const map_view_marker_type_e type);
		map_view_object_h __DEBUG_add_geo_marker(
					const maps_coordinates_h coords,
					const map_view_marker_type_e type);
		/* ----- | DEBUG ONLY | REMOVE WHEN NO NEEDED */
	};


	/* -------------------------------------------------------------------*/


	/* This class defines the automate, aimed at recognition of user
	 *  gestures on the base of gesture info, sich as a number of fingers
	 *  involved, the initial and current position of the fingers on the
	 *  Map View, the time diration of the gesture etc.
	 *  Note, it is enoug for the automate to process only each finger
	 *  the "tap", "move" and "up" events.
	 *  If the supported gesture is detected, the gesture_detector invokes
	 *  appropriate processing by gesture_processor.
	 */
	class gesture_detector {
	public:
		map_view_h _view;
	public:
		/* The current gesture info */
		runtime_touch_info _info;

		/* The history includes the touch info from previous gesture */
		runtime_touch_info _info_history;

		/* The gesture processing unit */
		gesture_processor _gp;
	public:
		gesture_detector(map_view_h v);
		~gesture_detector();
	public:
		void tap(int finger_no, const touch_point &tp);
		void move(int finger_no, const touch_point &tp);
		void up(int finger_no, const touch_point &tp);
	private:
		bool test_long_press_gesture();
		bool test_double_tap_gesture();
		bool test_tap_gesture();
		bool test_two_finger_tap_gesture();
		bool test_flick_gesture();
		bool test_pan_gesture();
		bool test_panning_gesture_finished();
		bool test_tilt_gesture();
		bool test_zoom_rotate_gesture();
		bool test_pinch_gesture();
	private:
		void reset();
	private:
		int get_trajectory_effective_length(const touch_point &p1,
						    const touch_point &p2);
	};

};

#endif				/* __MAPS_VIEW_GESTURE_PROCESSOR_H__ */
