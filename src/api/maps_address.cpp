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

#include <glib.h>
#include "maps_error.h"
#include "maps_address.h"
#include "maps_util.h"

/*
 * This represents address information such as building number,
 * street name, etc.
 */
typedef struct _maps_address_s
{
	char *building_number;		/**< Building number. */
	char *street;			/**< Full street name. */
	char *district;			/**< Municipal district name. */
	char *city;			/**< City name. */
	char *state;			/**< State or province region
					  of a nation. */
	char *country_code;		/**< Country name. */
	char *postal_code;		/**< Postal delivery code. */
	char *freetext;			/**<freeform address input */
	char *country;			/**<country name */
	char *county;			/**<county name */
} maps_address_s;

const gsize _MAPS_ADDRESS_BUILDING_NUMBER_MAX_LENGTH = 32;
const gsize _MAPS_ADDRESS_STREET_MAX_LENGTH = 256;
const gsize _MAPS_ADDRESS_MAX_LENGTH = 128;
const gsize _MAPS_ADDRESS_DISTRICT_MAX_LENGTH = 128;
const gsize _MAPS_ADDRESS_CITY_MAX_LENGTH = 256;
const gsize _MAPS_ADDRESS_STATE_MAX_LENGTH = 256;
const gsize _MAPS_ADDRESS_COUNTRY_CODE_MAX_LENGTH = 32;
const gsize _MAPS_ADDRESS_POSTAL_CODE_MAX_LENGTH = 32;
const gsize _MAPS_ADDRESS_FREE_TEXT_MAX_LENGTH = 1024;
const gsize _MAPS_ADDRESS_COUNTRY_MAX_LENGTH = 128;
const gsize _MAPS_ADDRESS_COUNTY_MAX_LENGTH = 128;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_address_create(maps_address_h *address)
{
	MAPS_LOG_API;
	if (!address)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_address_s *a = g_slice_new0(maps_address_s);
	if (!a) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	*address = (maps_address_h) a;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_address_destroy(maps_address_h address)
{
	MAPS_LOG_API;
	if (!address)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_address_s *a = (maps_address_s *) address;

	if (a->building_number)
		g_free(a->building_number);
	if (a->street)
		g_free(a->street);
	if (a->district)
		g_free(a->district);
	if (a->city)
		g_free(a->city);
	if (a->state)
		g_free(a->state);
	if (a->country_code)
		g_free(a->country_code);
	if (a->postal_code)
		g_free(a->postal_code);
	if (a->freetext)
		g_free(a->freetext);
	if (a->country)
		g_free(a->country);
	if (a->county)
		g_free(a->county);

	g_slice_free(maps_address_s, a);
	address = NULL;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_address_clone(const maps_address_h origin,
				  maps_address_h *cloned)
{
	MAPS_LOG_API;
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_address_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_address_s *a = (maps_address_s *) origin;

		if (a->building_number) {
			error = maps_address_set_building_number(*cloned,
				a->building_number);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->street) {
			error = maps_address_set_street(*cloned, a->street);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->district) {
			error = maps_address_set_district(*cloned, a->district);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->city) {
			error = maps_address_set_city(*cloned, a->city);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->state) {
			error = maps_address_set_state(*cloned, a->state);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->country_code) {
			error = maps_address_set_country_code(*cloned,
				a->country_code);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->postal_code) {
			error = maps_address_set_postal_code(*cloned,
				a->postal_code);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->freetext) {
			error = maps_address_set_freetext(*cloned, a->freetext);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->country) {
			error = maps_address_set_country(*cloned, a->country);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (a->county) {
			error = maps_address_set_county(*cloned, a->county);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	maps_address_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_address_get_building_number(const maps_address_h address,
						char **building_number)
{
	MAPS_LOG_API;
	if (!address || !building_number)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->building_number,
		_MAPS_ADDRESS_BUILDING_NUMBER_MAX_LENGTH, building_number);
}

EXPORT_API int maps_address_get_street(const maps_address_h address,
				       char **street)
{
	MAPS_LOG_API;
	if (!address || !street)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->street,
		_MAPS_ADDRESS_STREET_MAX_LENGTH, street);
}

EXPORT_API int maps_address_get_district(const maps_address_h address,
					 char **district)
{
	MAPS_LOG_API;
	if (!address || !district)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->district,
		_MAPS_ADDRESS_DISTRICT_MAX_LENGTH, district);
}

EXPORT_API int maps_address_get_city(const maps_address_h address, char **city)
{
	MAPS_LOG_API;
	if (!address || !city)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->city,
		_MAPS_ADDRESS_CITY_MAX_LENGTH, city);
}

EXPORT_API int maps_address_get_state(const maps_address_h address,
				      char **state)
{
	MAPS_LOG_API;
	if (!address || !state)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->state,
		_MAPS_ADDRESS_STATE_MAX_LENGTH, state);
}

EXPORT_API int maps_address_get_country(const maps_address_h address,
					char **country)
{
	MAPS_LOG_API;
	if (!address || !country)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->country,
		_MAPS_ADDRESS_COUNTRY_MAX_LENGTH, country);
}

EXPORT_API int maps_address_get_country_code(const maps_address_h address,
					     char **country_code)
{
	MAPS_LOG_API;
	if (!address || !country_code)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->country_code,
		_MAPS_ADDRESS_COUNTRY_CODE_MAX_LENGTH, country_code);
}

EXPORT_API int maps_address_get_county(const maps_address_h address,
				       char **county)
{
	MAPS_LOG_API;
	if (!address || !county)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->county,
		_MAPS_ADDRESS_COUNTY_MAX_LENGTH, county);
}

EXPORT_API int maps_address_get_postal_code(const maps_address_h address,
					    char **postal_code)
{
	MAPS_LOG_API;
	if (!address || !postal_code)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->postal_code,
		_MAPS_ADDRESS_POSTAL_CODE_MAX_LENGTH, postal_code);
}

EXPORT_API int maps_address_get_freetext(const maps_address_h address,
					 char **freetext)
{
	MAPS_LOG_API;
	if (!address || !freetext)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_address_s *) address)->freetext,
		_MAPS_ADDRESS_FREE_TEXT_MAX_LENGTH, freetext);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_address_set_building_number(maps_address_h address,
						const char *building_number)
{
	MAPS_LOG_API;
	if (!address || !building_number)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(building_number,
		_MAPS_ADDRESS_BUILDING_NUMBER_MAX_LENGTH,
		&((maps_address_s *) address)->building_number);
}

EXPORT_API int maps_address_set_street(maps_address_h address,
				       const char *street)
{
	MAPS_LOG_API;
	if (!address || !street)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(street, _MAPS_ADDRESS_STREET_MAX_LENGTH,
		&((maps_address_s *) address)->street);
}

EXPORT_API int maps_address_set_district(maps_address_h address,
					 const char *district)
{
	MAPS_LOG_API;
	if (!address || !district)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(district, _MAPS_ADDRESS_DISTRICT_MAX_LENGTH,
		&((maps_address_s *) address)->district);
}

EXPORT_API int maps_address_set_city(maps_address_h address, const char *city)
{
	MAPS_LOG_API;
	if (!address || !city)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(city, _MAPS_ADDRESS_CITY_MAX_LENGTH,
		&((maps_address_s *) address)->city);
}

EXPORT_API int maps_address_set_state(maps_address_h address, const char *state)
{
	MAPS_LOG_API;
	if (!address || !state)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(state, _MAPS_ADDRESS_STATE_MAX_LENGTH,
		&((maps_address_s *) address)->state);
}

EXPORT_API int maps_address_set_country(maps_address_h address,
					const char *country)
{
	MAPS_LOG_API;
	if (!address || !country)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(country, _MAPS_ADDRESS_COUNTRY_MAX_LENGTH,
		&((maps_address_s *) address)->country);
}

EXPORT_API int maps_address_set_country_code(maps_address_h address,
					     const char *country_code)
{
	MAPS_LOG_API;
	if (!address || !country_code)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(country_code,
		_MAPS_ADDRESS_COUNTRY_CODE_MAX_LENGTH,
		&((maps_address_s *) address)->country_code);
}

EXPORT_API int maps_address_set_county(maps_address_h address,
				       const char *county)
{
	MAPS_LOG_API;
	if (!address || !county)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(county, _MAPS_ADDRESS_COUNTY_MAX_LENGTH,
		&((maps_address_s *) address)->county);
}

EXPORT_API int maps_address_set_postal_code(maps_address_h address,
					    const char *postal_code)
{
	MAPS_LOG_API;
	if (!address || !postal_code)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(postal_code,
		_MAPS_ADDRESS_POSTAL_CODE_MAX_LENGTH,
		&((maps_address_s *) address)->postal_code);
}

EXPORT_API int maps_address_set_freetext(maps_address_h address,
					 const char *freetext)
{
	MAPS_LOG_API;
	if (!address || !freetext)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(freetext, _MAPS_ADDRESS_FREE_TEXT_MAX_LENGTH,
		&((maps_address_s *) address)->freetext);
}

/*
 * Tizen 3.0
 */

EXPORT_API int maps_address_list_create(maps_address_list_h *address_list)
{
	if (!address_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_alloc();
	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");
	*address_list = (void *) list;
	MAPS_LOGD("address_list: %p", list);

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_address_list_append(maps_address_list_h address_list, maps_address_h address)
{
	if (!address_list || !address)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_append((GList *)address_list, (gpointer) address);
	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");
	address_list = list;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_address_list_remove(maps_address_list_h address_list, maps_address_h address)
{
	if (!address_list || !address)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = g_list_remove((GList *) address_list, (gpointer) address);
	MAPS_CHECK_CONDITION(list, MAPS_ERROR_OUT_OF_MEMORY, "MAPS_ERROR_OUT_OF_MEMORY");
	address_list = list;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_address_list_get_length(maps_address_list_h address_list, int *length)
{
	if (!address_list || !length)
		return MAPS_ERROR_INVALID_PARAMETER;

	*length = g_list_length((GList *)address_list) - 1;

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_address_list_foreach(maps_address_list_h address_list, maps_address_cb callback, void *user_data)
{
	if (!address_list || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	bool ret = true;
	int index = 0;

	GList *l = (GList *)address_list;
	l = g_list_first(l);
	while (l != NULL)
	{
		GList *next = l->next;
		maps_address_s *address = (maps_address_s *)l->data;
		if (address) {
			/*
			int ret = callback(index++, address->country_code, address->country, addres->county, address->state,
							address->city, address->district, addrss->street, address->building_number, address->freetext, user_data);
			*/
			ret = callback(index++, address, user_data);
		}
		if (ret)
			l = next;
		else
			break;
	}

	return MAPS_ERROR_NONE;
}

static void _free_address(gpointer data, gpointer user_data)
{
	maps_address_h address = (maps_address_h) data;
	maps_address_h address_list = (maps_address_list_h) user_data;
	int ret;

	if (address) {
		MAPS_LOGD("%p", address);
		ret = maps_address_destroy(address);
		if (ret) {
			MAPS_LOGI("Failed to maps_address_destroy!!!");
		}
	}

	if (address_list) {
		ret = maps_address_list_remove(address_list, address);
		if (ret) {
			MAPS_LOGI("Failed to maps_address_list_remove!!!");
		}
	}
	address = NULL;
}

EXPORT_API int maps_address_list_destroy(maps_address_list_h address_list)
{
	if (!address_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	GList *list = (GList *) address_list;
	MAPS_LOGD("address_list:%p, length = %d", list, g_list_length(list));

	list = g_list_first(list);
	g_list_foreach(list, _free_address, list);
	g_list_free(list);
	address_list = NULL;

	return MAPS_ERROR_NONE;
}

