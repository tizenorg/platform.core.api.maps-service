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

#include "maps_place_test.h"
#include "maps_place_plugin.h"
#include "maps_place_category.h"
#include "maps_place_attribute_plugin.h"
#include "maps_error.h"
#include <glib.h>
#include "maps_place_private.h"
#include "maps_util.h"

 /* int maps_place_create(maps_place_h* place); */
/* int maps_place_destroy(maps_place_h place); */
void utc_maps_place_create_p(void)
{
	maps_place_h place = NULL;
	int error = maps_place_create(&place);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(place);

	error = maps_place_destroy(place);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_create_n(void)
{
	int error = maps_place_create(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_destroy(NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/*----------------------------------------------------------------------------*/
static int __utc_put_to_hashtable(const char* feature_str,
	maps_string_hashtable_h t)
{
	if (!feature_str || !t)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_string_hashtable_set(t, feature_str, feature_str);
}

class test_env
{
 public:
	maps_place_h p;
	int iteration;
 public:
	 test_env():p(NULL), iteration(0)
	{
		const int error = maps_place_create(&p);
		 g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
		 g_assert(p);

		maps_string_hashtable_h data_supported = NULL;
		if (maps_string_hashtable_create(&data_supported) !=
			MAPS_ERROR_NONE)
			 return;

		 __utc_put_to_hashtable(_S(MAPS_PLACE_ADDRESS), data_supported);
		 __utc_put_to_hashtable(_S(MAPS_PLACE_RATING), data_supported);
		 __utc_put_to_hashtable(_S(MAPS_PLACE_CATEGORIES),
			data_supported);
		 __utc_put_to_hashtable(_S(MAPS_PLACE_ATTRIBUTES),
			data_supported);
		 __utc_put_to_hashtable(_S(MAPS_PLACE_CONTACTS),
			data_supported);
		 __utc_put_to_hashtable(_S(MAPS_PLACE_EDITORIALS),
			data_supported);
		 __utc_put_to_hashtable(_S(MAPS_PLACE_REVIEWS), data_supported);
		 __utc_put_to_hashtable(_S(MAPS_PLACE_IMAGE), data_supported);
		 __utc_put_to_hashtable(_S(MAPS_PLACE_SUPPLIER),
			data_supported);
		 __utc_put_to_hashtable(_S(MAPS_PLACE_RELATED), data_supported);

		 _maps_place_set_supported_data(p, data_supported);
		 maps_string_hashtable_destroy(data_supported);

	}
	~test_env()
	{
		const int error = maps_place_destroy(p);
		g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	}
};

/*----------------------------------------------------------------------------*/

/* int maps_place_clone(const maps_place_h origin, maps_place_h* cloned); */
void utc_maps_place_clone_p(void)
{
	test_env e;

	maps_place_h cloned = NULL;
	int error = maps_place_clone(e.p, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(cloned);

	/* TODO: it is better to add meaningful values to the origin and check
	*  if they are clonned correctly */

	error = maps_place_destroy(cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_clone_n(void)
{
	test_env e;

	maps_place_h cloned = NULL;
	int error = maps_place_clone(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_clone(NULL, &cloned);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_set_id(maps_place_h place, const char* id); */
/* int maps_place_get_id(maps_place_h place, char** id); */
void utc_maps_place_set_id_p(void)
{
	test_env e;

	int error = maps_place_set_id(e.p, "place_id_1");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* id = NULL;
	error = maps_place_get_id(e.p, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(id, ==, "place_id_1");
	g_free(id);
}

void utc_maps_place_set_id_n(void)
{
	test_env e;

	int error = maps_place_set_id(NULL, "place_id_1");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_set_id(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* id = NULL;
	error = maps_place_get_id(NULL, &id);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_get_id(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_set_name(maps_place_h place, const char* name); */
/* int maps_place_get_name(maps_place_h place, char** name); */
void utc_maps_place_set_name_p(void)
{
	test_env e;

	int error = maps_place_set_name(e.p, "place_name");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* name = NULL;
	error = maps_place_get_name(e.p, &name);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(name, ==, "place_name");
	g_free(name);
}

void utc_maps_place_set_name_n(void)
{
	test_env e;

	int error = maps_place_set_name(NULL, "place_name");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_set_name(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* name = NULL;
	error = maps_place_get_name(NULL, &name);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_get_name(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_set_uri(maps_place_h place, const char* uri); */
/* int maps_place_get_uri(maps_place_h place, char** uri); */
void utc_maps_place_set_uri_p(void)
{
	test_env e;

	int error = maps_place_set_uri(e.p, "place_uri");
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	char* uri = NULL;
	error = maps_place_get_uri(e.p, &uri);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpstr(uri, ==, "place_uri");
	g_free(uri);
}

void utc_maps_place_set_uri_n(void)
{
	test_env e;

	int error = maps_place_set_uri(NULL, "place_uri");
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_set_uri(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	char* uri = NULL;
	error = maps_place_get_uri(NULL, &uri);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_get_uri(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
}

/* int maps_place_set_location(maps_place_h place,
*  maps_coordinates_h location); */
/* int maps_place_get_location(maps_place_h place,
*  maps_coordinates_h* location); */
void utc_maps_place_set_location_p(void)
{
	test_env e;

	maps_coordinates_h location = NULL;
	int error = maps_coordinates_create(44.4, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&location);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(location);

	error = maps_place_set_location(e.p, location);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_h location_obtained = NULL;
	error = maps_place_get_location(e.p, &location_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(location_obtained);

	double lat = .0, lon = .0;
#if _MAPS_COORDS_3D_
	double alt = .0;
#endif

	error = maps_coordinates_get_latitude(location_obtained, &lat);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	error = maps_coordinates_get_longitude(location_obtained, &lon);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

#if _MAPS_COORDS_3D
	error = maps_coordinates_get_altitude(location_obtained, &alt);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
#endif

	g_assert_cmpfloat(44.4, ==, lat);
	g_assert_cmpfloat(22.2, ==, lon);
#if _MAPS_COORDS_3D_
	g_assert_cmpfloat(33.3, ==, alt);
#endif

	error = maps_coordinates_destroy(location);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	maps_coordinates_destroy(location_obtained);
}

void utc_maps_place_set_location_n(void)
{
	test_env e;

	maps_coordinates_h location = NULL;
	int error = maps_coordinates_create(44.4, 22.2,
#if _MAPS_COORDS_3D_
		33.3,
#endif
		&location);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(location);

	error = maps_place_set_location(NULL, location);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_set_location(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	maps_coordinates_h location_obtained = NULL;
	error = maps_place_get_location(NULL, &location_obtained);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_get_location(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_coordinates_destroy(location);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_place_set_categories(maps_place_h place,
*  maps_item_list_h* categories); */
/* int maps_place_foreach_category(maps_place_h place,
*  maps_place_categories_cb callback, void* user_data); */
/* typedef bool (*maps_place_categories_cb)(
*  const maps_place_category_h category, void* user_data); */
static bool __utc_maps_place_categories_cb(int index, int total,
	const maps_place_category_h category, void* user_data)
{
	g_assert(category);
	test_env* e = (test_env*) user_data;
	g_assert(e);

	e->iteration++;

	maps_place_category_destroy(category);

	return true;
}

void utc_maps_place_set_category_p(void)
{
	test_env e;

	maps_item_list_h categories = NULL;
	int error = maps_item_list_create(&categories);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(categories);

	maps_place_category_h category = NULL;
	error = maps_place_category_create(&category);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(category);

	error = maps_item_list_append(categories, category,
		maps_place_category_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	error = maps_place_set_categories(e.p, categories);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_foreach_category(e.p, __utc_maps_place_categories_cb,
		&e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iteration, ==, 1);
	/* test finish --------------------------------- */

	error = maps_place_category_destroy(category);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_remove_all(categories,
		maps_place_category_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(categories);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_set_category_n(void)
{
	test_env e;

	maps_item_list_h categories = NULL;
	int error = maps_item_list_create(&categories);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(categories);

	maps_place_category_h category = NULL;
	error = maps_place_category_create(&category);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(category);

	error = maps_item_list_append(categories, category,
		maps_place_category_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	error = maps_place_set_categories(NULL, categories);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_set_categories(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_foreach_category(NULL,
		__utc_maps_place_categories_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iteration, ==, 0);

	error = maps_place_foreach_category(e.p, NULL, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iteration, ==, 0);
	/* test finish --------------------------------- */

	error = maps_place_category_destroy(category);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_remove_all(categories,
		maps_place_category_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(categories);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_place_set_attributes(maps_place_h place, GList* attributes); */
/* int maps_place_foreach_attribute(maps_place_h place,
*  maps_place_attributes_cb callback, void* user_data); */
static bool __utc_maps_place_attributes_cb(int index, int total,
	const maps_place_attribute_h attribute, void* user_data)
{
	g_assert(attribute);
	test_env* e = (test_env*) user_data;
	g_assert(e);

	e->iteration++;

	maps_place_attribute_destroy(attribute);

	return true;
}

void utc_maps_place_set_attribute_p(void)
{
	test_env e;

	maps_item_list_h attributes = NULL;
	int error = maps_item_list_create(&attributes);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(attributes);

	maps_place_attribute_h attribute = NULL;
	error = maps_place_attribute_create(&attribute);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(attribute);

	error = maps_item_list_append(attributes, attribute,
		maps_place_attribute_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	error = maps_place_set_attributes(e.p, attributes);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_place_foreach_attribute(e.p,
		__utc_maps_place_attributes_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert_cmpint(e.iteration, ==, 1);
	/* test finish --------------------------------- */

	error = maps_place_attribute_destroy(attribute);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_remove_all(attributes,
		maps_place_attribute_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(attributes);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

void utc_maps_place_set_attribute_n(void)
{
	test_env e;

	maps_item_list_h attributes = NULL;
	int error = maps_item_list_create(&attributes);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(attributes);

	maps_place_attribute_h attribute = NULL;
	error = maps_place_attribute_create(&attribute);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
	g_assert(attribute);

	error = maps_item_list_append(attributes, attribute,
		maps_place_attribute_clone);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	/* test start --------------------------------- */
	error = maps_place_set_attributes(NULL, attributes);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_set_attributes(e.p, NULL);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);

	error = maps_place_foreach_attribute(NULL,
		__utc_maps_place_attributes_cb, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iteration, ==, 0);

	error = maps_place_foreach_attribute(e.p, NULL, &e);
	g_assert_cmpint(error, ==, MAPS_ERROR_INVALID_PARAMETER);
	g_assert_cmpint(e.iteration, ==, 0);
	/* test finish --------------------------------- */

	error = maps_place_attribute_destroy(attribute);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_remove_all(attributes,
		maps_place_attribute_destroy);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);

	error = maps_item_list_destroy(attributes);
	g_assert_cmpint(error, ==, MAPS_ERROR_NONE);
}

/* int maps_place_set_contact(maps_place_h place, GList* contacts); */
/* int maps_place_foreach_contact(maps_place_h place,
*  maps_place_contacts_cb callback, void* user_data); */
void utc_maps_place_set_contacts_p(void)
{

}

void utc_maps_place_set_contacts_n(void)
{

}

/* int maps_place_set_editorials(maps_place_h place, GList* editorials); */
/* int maps_place_foreach_editorial(maps_place_h place,
*  maps_place_editorials_cb callback, void* user_data); */
void utc_maps_place_set_editorial_p(void)
{

}

void utc_maps_place_set_editorial_n(void)
{

}

/* int maps_place_set_images(maps_place_h place, GList* images); */
/* int maps_place_foreach_image(maps_place_h place,
*  maps_place_images_cb callback, void* user_data); */
void utc_maps_place_set_image_p(void)
{

}

void utc_maps_place_set_image_n(void)
{

}

/* int maps_place_set_reviews(maps_place_h place, GList* reviews); */
/* int maps_place_foreach_review(maps_place_h place,
*  maps_place_reviews_cb callback, void* user_data); */
void utc_maps_place_set_review_p(void)
{

}

void utc_maps_place_set_review_n(void)
{

}

/* int maps_place_set_properties(maps_place_h place,
*  maps_item_hashtable_h properties) */
/* int maps_place_foreach_property(maps_place_h place,
*  maps_place_properties_cb callback, void* user_data) { */
void utc_maps_place_set_properties_p(void)
{

}

void utc_maps_place_set_properties_n(void)
{

}

/* int maps_place_set_rating(maps_place_h place,
*  maps_place_rating_h rating); */
/* int maps_place_get_rating(maps_place_h place ,
*  maps_place_rating_h* rating); */
void utc_maps_place_set_rating_p(void)
{

}

void utc_maps_place_set_rating_n(void)
{

}

/* int maps_place_set_supplier_link(maps_place_h place,
*  maps_place_link_object_h supplier); */
/* int maps_place_get_supplier_link(maps_place_image_h place,
*  maps_place_link_object_h* supplier); */
void utc_maps_place_set_supplier_link_p(void)
{

}

void utc_maps_place_set_supplier_link_n(void)
{

}

/* int maps_place_set_related_link(maps_place_h place,
*  maps_place_link_object_h related); */
/* int maps_place_get_related_link(maps_place_image_h place,
*  maps_place_link_object_h* related); */
void utc_maps_place_set_related_link_p(void)
{

}

void utc_maps_place_set_related_link_n(void)
{

}

/* int maps_place_foreach_property(maps_place_h place,
*  maps_place_properties_cb callback, void* user_data); */
void utc_maps_place_foreach_property_p(void)
{

}

void utc_maps_place_foreach_property_n(void)
{

}

