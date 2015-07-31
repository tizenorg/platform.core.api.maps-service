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

#ifndef __MAPS_SERVICE_TEST_API_OBJECT_H__
#define __MAPS_SERVICE_TEST_API_OBJECT_H__

#include <maps_service.h>
#include <maps_plugin.h>
#include <maps_address.h>
#include <maps_place_plugin.h>
#include <maps_place_rating_plugin.h>
#include <maps_place_editorial_plugin.h>
#include <maps_place_review_plugin.h>
#include <maps_place_image_plugin.h>
#include <maps_place_media_plugin.h>
#include <maps_place_link_object_plugin.h>
#include <maps_place_contact_plugin.h>
#include <maps_place_category.h>
#include <maps_route_plugin.h>
#include <maps_route_segment_plugin.h>
#include <maps_route_maneuver_plugin.h>

bool __equal_double(const double &d1, const double &d2);

/*----------------------------------------------------------------------------*/
/* Simple type library, helpful for operating with Maps Objects */
/* */
/* Implement also a gabage collector */
namespace maps
{

	/* Basic functions of each Object */
	typedef int (*create_f) (void **data);
	typedef int (*destroy_f) (void *data);
	typedef int (*clone_f) (void *origin, void **cloned);

	/* The basic class of each Object */
	template <create_f CRF, destroy_f DF, clone_f CLF> class object
	{
	protected:
		void *handle;
	public:
		object() : handle(NULL)
		{
			CRF(&handle);
		}
		object(void *h) : handle(h)
		{
		}
		object(const object &o)
		{
			CLF(o.handle, &handle);
		}
		virtual ~object()
		{
			DF(handle);
		}
	public:
		object &operator=(const object &o)
		{
			if (this != &o) {
				DF(handle);
				CLF(o.handle, handle);
			}
			return *this;
		}
	public:
		destroy_f destroy_func() const
		{
			return DF;
		}
		clone_f clone_func() const
		{
			return CLF;
		}
	public:
		 operator void *()
		{
			return handle;
		}
		operator void *() const
		{
			return handle;
		}
		operator void **()
		{
			return &handle;
		}
	public:
		void *clone() const
		{
			void *c = NULL;
			 CLF(handle, &c);
			 return c;
		}
	};

	/* Empty Object */
	class empty_object
	{
	public:
		virtual ~empty_object();
		static int create_empty(void **obj)
		{
			return 0;
		}
		static int destroy_empty(void *obj)
		{
			return 0;
		}
		static int clone_empty(void *origin, void **cloned)
		{
			return 0;
		}
	};

	/* The list of objects */
	template <class T> class item_list :
		public object <maps_item_list_create,
			maps_item_list_destroy,
			empty_object::clone_empty>
	{
	public:
		virtual ~item_list()
		{
			clear();
		}
		void add(const T &o)
		{
			maps_item_list_append(handle, o, o.clone_func());
		}
		void clear()
		{
			static T empty;
			maps_item_list_remove_all(handle, empty.destroy_func());
		}
	};

	/* Object Classes: */

	/* Coordinates */
	class coordinates : public object <empty_object::create_empty,
				maps_coordinates_destroy,
				maps_coordinates_clone>
	{
	public:
		coordinates();
		coordinates(const double &lat, const double &lon);
		coordinates(const maps_coordinates_s &c);
		coordinates(void *h):object(h)
		{
		}
		coordinates(const coordinates &c);
		virtual ~coordinates();
	public:
		coordinates &operator=(const coordinates &c);
		bool operator==(const coordinates &c) const;
		bool operator!=(const coordinates &c) const;
	public:
		double get_latitude() const;
		double get_longitude() const;
	};

	/* Area */
	class area : public object <empty_object::create_empty,
			maps_area_destroy,
			maps_area_clone >
	{
	public:
		area();

		/* Rectangular area */
		area(const coordinates &lt, const coordinates &rb);
		area(const double &l, const double &t,
		     const double &r, const double &b);

		/* Circular area */
		area(const coordinates &c, const double &r);
		area(const double &x, const double &y, const double &r);

		area(void *h) : object(h)
		{
		}
		area(const area &a);
		virtual ~area();
	public:
		area &operator=(const area &a);
		bool operator==(const area &a) const;
		bool operator!=(const area &a) const;
	public:
		maps_area_type_e get_type() const;
		coordinates get_top_left() const;
		coordinates get_bottom_right() const;
		coordinates get_center() const;
		double get_radius() const;
	};

/*----------------------------------------------------------------------------*/
	/* Address */
	typedef object <maps_address_create, maps_address_destroy,
		maps_address_clone> address;

/*----------------------------------------------------------------------------*/
	/* Place */
	typedef object <maps_place_create,
		maps_place_destroy,
		maps_place_clone> place;

	typedef object <maps_place_filter_create,
		maps_place_filter_destroy,
		maps_place_filter_clone> place_filter;

	typedef object <maps_place_rating_create,
		maps_place_rating_destroy,
		maps_place_rating_clone> place_rating;

	typedef object <maps_place_editorial_create,
		maps_place_editorial_destroy,
		maps_place_editorial_clone> place_editorial;

	typedef object <maps_place_review_create,
		maps_place_review_destroy,
		maps_place_review_clone> place_review;

	typedef object <maps_place_image_create,
		maps_place_image_destroy,
		maps_place_image_clone> place_image;

	typedef object <maps_place_media_create,
		maps_place_media_destroy,
		maps_place_media_clone> place_media;

	typedef object <maps_place_link_object_create,
		maps_place_link_object_destroy,
		maps_place_link_object_clone> place_link_object;

	typedef object <maps_place_contact_create,
		maps_place_contact_destroy,
		maps_place_contact_clone> place_contact;

	typedef object <maps_place_category_create,
		maps_place_category_destroy,
		maps_place_category_clone> place_category;

/*----------------------------------------------------------------------------*/
	/* Route */
	typedef object <maps_route_create,
		maps_route_destroy,
		maps_route_clone> route;

	typedef object <maps_route_segment_create,
		maps_route_segment_destroy,
		maps_route_segment_clone> route_segment;

	typedef object <maps_route_maneuver_create,
		maps_route_maneuver_destroy,
		maps_route_maneuver_clone> route_maneuver;

/*----------------------------------------------------------------------------*/
	/* String holder with auto-free of memory */
	class string_holder
	{
	public:
		char *str;
	public:
		string_holder(char *string = NULL) : str(string) {
		}
		~string_holder();
		operator char *() const
		{
			return str;
		}
		operator char **()
		{
			return &str;
		}
	};
};

/*----------------------------------------------------------------------------*/

#endif				/* __MAPS_SERVICE_TEST_API_OBJECT_H__ */

