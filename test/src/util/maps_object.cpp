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

#include "maps_object.h"
#include "maps_address.h"
#include "maps_error.h"
#include <glib.h>

bool __equal_double(const double &d1, const double &d2)
{
	static double delta = 0.0000001;
	if (d1 >= (d2 + delta))
		return false;
	if (d1 <= (d2 - delta))
		return false;
	return true;
}

/*----------------------------------------------------------------------------*/
/* Simple type library, helpful for operating with Maps Objects */

/* Coordinates */
 maps::coordinates::coordinates():object(NULL)
{
}

 maps::coordinates::coordinates(const double &lat, const double &lon):object(NULL)
{
	maps_coordinates_create(lat, lon, &handle);
}

maps::coordinates::coordinates(const maps_coordinates_s & c):object(NULL)
{
	maps_coordinates_create(c.latitude, c.longitude, &handle);
}

maps::coordinates::coordinates(const coordinates & c):object(NULL)
{
	*this = c;
}

maps::coordinates & maps::coordinates::operator=(const coordinates & c)
{
	if (this != &c) {
		maps_coordinates_destroy(handle);
		maps_coordinates_clone(c.handle, &handle);
	}
	return* this;
}

maps::coordinates::~coordinates()
{
}

bool maps::coordinates::operator==(const coordinates & c) const
{
	if (this == &c)
		return true;
	if (!__equal_double(get_latitude(), c.get_latitude()))
		return false;
	if (!__equal_double(get_longitude(), c.get_longitude()))
		return false;
	return true;
}

bool maps::coordinates::operator!=(const coordinates & c) const
{
	return !(*this == c);
}

double maps::coordinates::get_latitude() const
{
	double lat = 0;
	if (handle)
		maps_coordinates_get_latitude(handle, &lat);
	return lat;
}

double maps::coordinates::get_longitude() const
{
	double lon = 0;
	if (handle)
		maps_coordinates_get_longitude(handle, &lon);
	return lon;
}

/* Area */
 maps::area::area():object(NULL)
{
}

maps::area::~area()
{
}

 maps::area::area(const coordinates & lt, const coordinates & rb):object(NULL)
{
	maps_area_create_rectangle(lt, rb, &handle);
}

maps::area::area(const double &l, const double &t, const double &r,
	const double &b):object(NULL)
{
	maps_area_create_rectangle(coordinates(l, t), coordinates(r, b),
		&handle);
}

maps::area::area(const coordinates & c, const double &r):object(NULL)
{
	maps_area_create_circle(c, r, &handle);
}

maps::area::area(const double &x, const double &y, const double &r):object(NULL)
{
	maps_area_create_circle(coordinates(x, y), r, &handle);
}

maps::area::area(const area & a):object(NULL)
{
	*this = a;
}

maps::area & maps::area::operator=(const area & a)
{
	if (this != &a) {
		maps_area_destroy(handle);
		maps_area_clone(a.handle, &handle);
	}
	return* this;
}

maps_area_type_e maps::area::get_type() const
{
	if (!handle)
		return MAPS_AREA_NONE;
	maps_area_s* a = (maps_area_s*) handle;
	return a->type;
}

maps::coordinates maps::area::get_left_top() const
{
	if (!handle)
		return coordinates(.0, .0);
	maps_area_s* a = (maps_area_s*) handle;
	return coordinates(a->rect.left_top);
}

maps::coordinates maps::area::get_right_bottom() const
{
	if (!handle)
		return coordinates(.0, .0);
	maps_area_s* a = (maps_area_s*) handle;
	return coordinates(a->rect.right_bottom);
}

maps::coordinates maps::area::get_center() const
{
	if (!handle)
		return coordinates(.0, .0);
	maps_area_s* a = (maps_area_s*) handle;
	return coordinates(a->circle.center);
}

double maps::area::get_radius() const
{
	if (!handle)
		return .0;
	maps_area_s* a = (maps_area_s*) handle;
	return a->circle.radius;
}

bool maps::area::operator==(const area & a) const
{
	if (this == &a)
		return true;
	if (get_type() != a.get_type())
		return false;
	switch (get_type()) {
	case MAPS_AREA_RECTANGLE:
		if (get_left_top() != a.get_left_top())
			return false;
		if (get_right_bottom() != a.get_right_bottom())
			return false;
		break;
	case MAPS_AREA_CIRCLE:
		if (get_center() != a.get_center())
			return false;
		if (!__equal_double(get_radius(), a.get_radius()))
			return false;
		break;
	case MAPS_AREA_NONE:
	default:
		break;
	}
	return true;
}

bool maps::area::operator!=(const area & a) const
{
	return !(*this == a);
}

/*----------------------------------------------------------------------------*/

maps::string_holder::~string_holder()
{
	g_free(str);
}

