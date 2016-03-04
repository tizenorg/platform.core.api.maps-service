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


#include <Ecore.h>
#include <Evas.h>
#include "maps_view.h"
#include "command.h"
#include "command_queue.h"
#include "runtime_data.h"


/* Select the way of panning: shift center on delta coordinates or move to the
 * specified central coordinate.
 * If the following MACRO is defined, the shift is performed
 */
#define _MOVE_CENTER_COMMAND_DEFINED_


namespace view
{

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
		void on_pan(int finger_no);
		void on_panning_finished(int finger_no);
		void on_zoom_rotate();
		void on_single_finger_zoom();
		void on_pinch();
	private:
		session::command_queue *q();
		void *get_maps();
		session::command *construct_gesture_command(
						maps_view_gesture_e gesture,
						const maps_coordinates_h c,
						const bool zoom_changed,
						const double zoom,
						const bool rotation_changed,
						const double angle);
	protected:
		touch_point calc_center(const touch_point &tp1,
					const touch_point &tp2) const;
	};


	/* -------------------------------------------------------------------*/


	class finger_event_stream {
	private:
		class gesture_detector *_d;
		bool _finger_pressed[MAX_FINGERS];
		bool _finger_moving[MAX_FINGERS];
		touch_point _finger_down[MAX_FINGERS];
	public:
		finger_event_stream(maps_view_h v);
		~finger_event_stream();
	public:

		void tap(Evas_Event_Mouse_Down *ev);
		void move(Evas_Event_Mouse_Move *ev);
		void up(Evas_Event_Mouse_Up *ev);
		void multi_tap(Evas_Event_Multi_Down *ev);
		void multi_move(Evas_Event_Multi_Move *ev);
		void multi_up(Evas_Event_Multi_Up *ev);
	private:
		bool finger_dragged_enough(int finger_no,
					   const touch_point &tp);
	public:
		void set_gesture_detector(class gesture_detector *d);
	};
};

#endif				/* __MAPS_VIEW_GESTURE_PROCESSOR_H__ */
