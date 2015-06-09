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

/*--------------------------------------------------------------------------- */
/* 1.1 */
#include <maps_service.h>
/*--------------------------------------------------------------------------- */

/*--------------------------------------------------------------------------- */
/* 1.2 */
#include <maps_service.h>

static bool __my_maps_service_provider_info_cb(char* maps_provider,
	void* user_data)
{
	/* Operate provider name, stored in maps_provider parameter */
	return true;
}

void get_available_providers()
{
	const int error =
		maps_service_foreach_provider
		(__my_maps_service_provider_info_cb, NULL);
	if (error == MAPS_ERROR_NONE) {
		/* Chose one of providers from the available_providers list */
	}
	else {
		/* Handle error */
	}
}

/*--------------------------------------------------------------------------- */

/*--------------------------------------------------------------------------- */
/* 1.3, 1.4 */

#include <maps_service.h>

void create_maps_service(const char* maps_provider_name)
{
	maps_service_h maps = NULL;
	int error = maps_service_create("Maps Provider", &maps);

	error = maps_service_set_provider_key(maps, "XXXYYYZZZ");

	bool supported = false;

	/* Check if Routing is available */
	error = maps_service_provider_is_service_supported(maps,
		MAPS_SERVICE_SEARCH_ROUTE, &supported);
	const bool is_routing_supported =
		(error == MAPS_ERROR_NONE) ? supported : false;

	/* Check if Routing via specified waypoints is available */
	error = maps_service_provider_is_service_supported(maps,
		MAPS_SERVICE_SEARCH_ROUTE_WAYPOINTS, &supported);
	const bool is_routing_waypoints_supported =
		(error == MAPS_ERROR_NONE) ? supported : false;

	/* etc for other services, enumerated in maps_service_e */

	if (is_routing_supported || is_routing_waypoints_supported) {
		/* Check which features of place data structure are available */

		/* Check if route path data is supported */
		error = maps_service_provider_is_data_supported(maps,
			MAPS_ROUTE_PATH, &supported);
		const bool is_route_path_supported =
			(error == MAPS_ERROR_NONE) ? supported : false;
		if (is_route_path_supported) {
			/* Allow route path usage */
		}

		/* Check if maneuver path data is supported */
		error = maps_service_provider_is_data_supported(maps,
			MAPS_ROUTE_SEGMENTS_PATH, &supported);
		const bool is_route_segment_path_supported =
			(error == MAPS_ERROR_NONE) ? supported : false;
		if (is_route_segment_path_supported) {
			/* Allow segment path usage */
		}

		/* Check if maneuver sements data is supported */
		error = maps_service_provider_is_data_supported(maps,
			MAPS_ROUTE_SEGMENTS_MANEUVERS, &supported);
		const bool is_route_segment_maneuvers_supported =
			(error == MAPS_ERROR_NONE) ? supported : false;
		if (is_route_segment_maneuvers_supported) {
			/* Allow segment maneuvers usage */
		}

		/* etc for other data features,
		*  enumerated in maps_service_data_e */
	}

}

/*--------------------------------------------------------------------------- */

/*--------------------------------------------------------------------------- */
/* 1.6 */
#include <maps_service.h>
void set_general_preference()
{
	maps_service_h maps = NULL;

	/* Create an instance of a preference set */
	maps_preference_h preference = NULL;
	int error = maps_preference_create(&preference);
	if (error != MAPS_ERROR_NONE) {
		/* Handle error */
	}

	/* Set the distance units preference */
	error = maps_preference_set_distance_unit(preference,
		MAPS_DISTANCE_UNIT_M);
	if (error != MAPS_ERROR_NONE) {
		/* Handle error */
	}

	/* Set the language preference */
	error = maps_preference_set_language(preference, "en-US");
	if (error != MAPS_ERROR_NONE) {
		/* Handle error */
	}

	/* Apply the set of preferences for Maps Service */
	error = maps_service_set_preference(maps, preference);
	if (error != MAPS_ERROR_NONE) {
		/* Handle error */
	}

	/* Destroy the instance of the preference set */
	error = maps_preference_destroy(preference);
	if (error != MAPS_ERROR_NONE) {
		/* Handle error */
	}
}

/*--------------------------------------------------------------------------- */
/* 1.7 */
#include <maps_service.h>

static bool __maps_service_geocode_cb(maps_error_e result, int request_id,
	int index, int total, maps_coordinates_h coordinates, void* user_data)
{

	/* Handle obtained coordinates data... */

	/* Release results */
	maps_coordinates_destroy(coordinates);
	return true;
}

void geocode()
{
	maps_service_h maps = NULL;
	maps_preference_h preference = NULL;
	void* user_data = NULL;
	int error = MAPS_ERROR_NONE;
	int request_id = 0;

	/* Searching for geocode of the */
	/* Samsung'c campus "Digital City" in Suwon */
	error = maps_service_geocode(maps,
		"Suwon, Digital City",
		preference, __maps_service_geocode_cb, user_data, &request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	maps_area_h bounds = NULL;
	/* Use maps_area_create_rectangle() or maps_area_create_circle() */
	/* to create a geographical bounds of Geocoding */

	/* Searching for geocode of the */
	/* Samsung's campus Digital City */
	/* within a specified geographical area */
	error = maps_service_geocode_inside_area(maps,
		"Digital City",
		bounds,
		preference, __maps_service_geocode_cb, user_data, &request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	maps_address_h address = NULL;
	/* Use maps_address_create() to create an instance of an address */
	/* Than use maps_address_set_xxx to initialize the address with */
	/* desired values */

	/* Searching for a geocode of a place, */
	/* specified with a structured address */
	error = maps_service_geocode_by_structured_address(maps,
		address,
		preference, __maps_service_geocode_cb, user_data, &request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
}

static void __maps_service_reverse_geocode_cb(maps_error_e result,
	int request_id, int index, int total, maps_address_h address,
	void* user_data)
{

	/* Handle obtained address... */

	/* Release results */
	maps_address_destroy(address);
}

/*----------------------------------------------------------------------------*/
/* */
/* Geocoder Service */

void reverse_geocode()
{
	maps_service_h maps = NULL;
	maps_preference_h preference = NULL;
	void* user_data = NULL;
	int error = MAPS_ERROR_NONE;
	int request_id = 0;

	/* Obtaining the reverse geocode with a specified coordinates */
	error = maps_service_reverse_geocode(maps,
		37.257865,
		127.053659,
		preference,
		__maps_service_reverse_geocode_cb, user_data, &request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
}

#include <stdlib.h>

void parse_address_data()
{
	maps_address_h address = NULL;
	int error = 0;

	/* Obtain building number */
	char* building_number = NULL;
	error = maps_address_get_building_number(address, &building_number);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/* Use building_number ... */

	free(building_number);

	/* Obtain street name */
	char* street = NULL;
	error = maps_address_get_street(address, &street);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/* Use street */

	free(street);
}

/*----------------------------------------------------------------------------*/
/* */
/* Place Service */

static bool __maps_service_search_place_cb(maps_error_e error, int request_id,
	int index, int total, maps_place_h place, void* user_data)
{

	/* Handle obtained place data... */

	/* Release results */
	maps_place_destroy(place);
	return true;
}

void place()
{
	maps_service_h maps = NULL;
	maps_preference_h preference = NULL;
	void* user_data = NULL;
	int error = MAPS_ERROR_NONE;
	maps_place_filter_h filter = NULL;
	int request_id = 0;

	/* Create extra preferences for Place Search Service */
	error = maps_preference_create(&preference);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
	error = maps_preference_set_property(preference, MAPS_PLACE_FILTER_TYPE,
		"restaurant");
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	maps_coordinates_h position = NULL;
	/* Create coordinates with maps_coordinates_create() */

	int distance = 500;
	error = maps_service_search_place(maps,
		position,
		distance,
		filter,
		preference,
		__maps_service_search_place_cb, user_data, &request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	maps_preference_destroy(preference);

	maps_area_h boundary = NULL;
	/* Create boundary with one of: */
	/* - maps_area_create_rectangle() */
	/* - maps_area_create_circle() */

	error = maps_service_search_place_by_area(maps,
		boundary,
		filter,
		preference,
		__maps_service_search_place_cb, user_data, &request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/*maps_area_h boundary = NULL; */
	/* Create boundary with one of: */
	/* - maps_area_create_rectangle() */
	/* - maps_area_create_circle() */

	error = maps_service_search_place_by_address(maps,
		"Digital City",
		boundary,
		filter,
		preference,
		__maps_service_search_place_cb, user_data, &request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
}

static bool __maps_place_categories_cb(int index, int total,
	maps_place_category_h category, void* user_data)
{

	/* Handle obtained place category data... */

	/* Release results */
	maps_place_category_destroy(category);
	return true;
}

static bool __maps_place_properties_cb(int index, int total,
	char* key, void* value, void* user_data)
{

	/* Handle obtained a property: */
	/* property_name = key */
	/* property_value = value */

	/* Release property name and value */
	free(key);
	free(value);
	return true;
}

void parse_place_data()
{
	maps_service_h maps = NULL;
	maps_place_h place = NULL;
	int error = MAPS_ERROR_NONE;
	void* user_data = NULL;

	/* Obtain place name */
	char* name = NULL;
	error = maps_place_get_name(place, &name);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/* Use place name */

	free(name);

	/* Obtain place location */
	maps_coordinates_h location = NULL;
	error = maps_place_get_location(place, &location);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/* Use route location */

	maps_coordinates_destroy(location);

	/* Check if route path data is supported */
	bool is_place_rating_supported = false;
	error = maps_service_provider_is_data_supported(maps, MAPS_PLACE_RATING,
		&is_place_rating_supported);

	if (error == MAPS_ERROR_NONE) {
		if (is_place_rating_supported) {

			/* Obtain place rating */
			maps_place_rating_h rating = NULL;
			error = maps_place_get_rating(place, &rating);
			if (error != MAPS_ERROR_NONE) {
				/* Handle Error */
			}

			/* Use route name */

			maps_place_rating_destroy(rating);
		}
	}
	else {
		/* Handle Error */
	}

	/* Obtain a list of place categories */
	error = maps_place_foreach_category(place, __maps_place_categories_cb,
		user_data);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/* Obtain Maps Provider specific place data properties */
	error = maps_place_foreach_property(place, __maps_place_properties_cb,
		user_data);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
}

/*----------------------------------------------------------------------------*/
/* */
/* Route Service */

static bool __maps_service_search_route_cb(maps_error_e error, int request_id,
	int index, int total, maps_route_h route, void* user_data)
{

	/* Handle obtained route data... */

	/* Release results */
	maps_route_destroy(route);
	return true;
}

void route()
{
	maps_service_h maps = NULL;
	maps_preference_h preference = NULL;
	void* user_data = NULL;
	int error = MAPS_ERROR_NONE;
	int request_id = 0;

	/* Create extra preferences for Place Search Service */
	error = maps_preference_create(&preference);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
	error = maps_preference_set_property(preference,
		MAPS_ROUTE_FREEFORM_ADDR_TO_AVOID, "Suwon, Digital City");
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
	error = maps_preference_set_route_optimization(preference,
		MAPS_ROUTE_TYPE_SHORTEST);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	maps_coordinates_h origin = NULL, destination = NULL;
	/* Create coordinates with maps_coordinates_create() */

	error = maps_service_search_route(maps,
		origin,
		destination,
		preference,
		__maps_service_search_route_cb, user_data, &request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	maps_preference_destroy(preference);

	/* Specify amount of way points */
	const int waypoint_num = 5;

	/* Create array with coordinates of way points */
	maps_coordinates_h* waypoint_list = NULL;

	error = maps_service_search_route_waypoints(maps,
		waypoint_list,
		waypoint_num,
		preference,
		__maps_service_search_route_cb, user_data, &request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
}

static bool __maps_route_path_cb(int index, int total,
	maps_coordinates_h coordinates, void* user_data)
{

	/* Handle obtained coordinates of route path... */

	/* Release results */
	maps_coordinates_destroy(coordinates);
	return true;
}

static bool __maps_route_segment_cb(int index, int total,
	maps_route_segment_h segment, void* user_data)
{

	/* Handle obtained segment of route... */

	/* Release results */
	maps_route_segment_destroy(segment);
	return true;
}

static bool __maps_route_properties_cb(int index, int total,
	char* key, void* value, void* user_data)
{

	/* Handle obtained a property: */
	/* property_name = key */
	/* property_value = value */

	/* Release property name and value */
	free(key);
	free(value);
	return true;
}

void parse_route_data()
{
	maps_service_h maps = NULL;
	maps_route_h route = NULL;
	int error = MAPS_ERROR_NONE;
	void* user_data = NULL;

	/* Obtain route id */
	char* id = NULL;
	error = maps_route_get_route_id(route, &id);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/* Use route id */

	free(id);

	/* Obtain route origin and destination */
	maps_coordinates_h origin = NULL, destination = NULL;
	error = maps_route_get_origin(route, &origin);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
	error = maps_route_get_destination(route, &destination);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/* Use route origin and destination */

	maps_coordinates_destroy(origin);
	maps_coordinates_destroy(destination);

	/* Obtain route total distance */
	double total_distance = .0;
	error = maps_route_get_total_distance(route, &total_distance);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/* Use route total distance */

	error = maps_route_foreach_path(route, __maps_route_path_cb, user_data);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	error = maps_route_foreach_segment(route, __maps_route_segment_cb,
		user_data);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}

	/* Check if route path data is supported */
	bool is_route_path_supported = false;
	error = maps_service_provider_is_data_supported(maps, MAPS_ROUTE_PATH,
		&is_route_path_supported);

	if (error == MAPS_ERROR_NONE) {
		if (is_route_path_supported) {

			/* Obtain route path */
			error = maps_route_foreach_path(route,
				__maps_route_path_cb, user_data);
			if (error != MAPS_ERROR_NONE) {
				/* Handle Error */
			}
		}
	}
	else {
		/* Handle Error */
	}

	/* Check if route segment data is supported */

	bool is_route_segment_path_supported = false;
	error = maps_service_provider_is_data_supported(maps,
		MAPS_ROUTE_SEGMENTS_PATH, &is_route_segment_path_supported);

	bool is_route_segment_maneuvers_supported = false;
	error = maps_service_provider_is_data_supported(maps,
		MAPS_ROUTE_SEGMENTS_MANEUVERS,
		&is_route_segment_maneuvers_supported);

	if (error == MAPS_ERROR_NONE) {
		if (is_route_segment_path_supported
			|| is_route_segment_maneuvers_supported) {

			/* Obtain route segments */
			error = maps_route_foreach_segment(route,
				__maps_route_segment_cb, user_data);
			if (error != MAPS_ERROR_NONE) {
				/* Handle Error */
			}
		}
	}
	else {
		/* Handle Error */
	}

	/* Obtain Maps Provider specific route data properties */
	error = maps_route_foreach_property(route, __maps_route_properties_cb,
		user_data);
	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
}

/*----------------------------------------------------------------------------*/
/* */
/* Cancel Request */

void cancel()
{
	maps_service_h maps = NULL;
	int error = MAPS_ERROR_NONE;
	int request_id = 0;

	/* Cancel the request with a specified id */
	error = maps_service_cancel_request(maps, request_id);

	if (error != MAPS_ERROR_NONE) {
		/* Handle Error */
	}
}

int _maps_service_main_tutorial(void)
{

	/* 1.2 */
	if (0)
		get_available_providers();

	/* 1.3, 1.4 */
	if (0)
		create_maps_service("Maps Provider");

	/* 1.6 */
	if (0)
		set_general_preference();

	/* 1.7 */
	if (0)
		geocode();
	if (0)
		parse_address_data();

	/* 1.7 */
	if (0)
		place();
	if (0)
		parse_place_data();

	/* 1.7 */
	if (0)
		route();
	if (0)
		parse_route_data();

	/* 1.7 */
	if (0)
		cancel();

	return 0;
}

