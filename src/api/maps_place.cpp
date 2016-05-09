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

#include "maps_error.h"
#include "maps_place_plugin.h"
#include "maps_extra_types.h"
#include "maps_util.h"
#include "maps_address.h"
#include "maps_place_private.h"
#include <glib.h>

static bool __is_supported(const maps_place_h place, maps_service_data_e data)
{
	bool supported = false;
	_maps_place_is_data_supported(place, data, &supported);
	return supported;
}

/*----------------------------------------------------------------------------*/

typedef struct _maps_place_s
{
	char *id;
	char *name;
	char *uri;

	maps_coordinates_h location;

	int distance;

	maps_address_h address;

	maps_place_rating_h rating;

	/* List of properties:
	 * Obtained with: maps_place_properties_cb callback
	 * Items are pairs: const char* key, const char* value */
	maps_item_hashtable_h properties;

	/* List of categories
	 * Obtained with: maps_place_categories_cb
	 * Each of type: maps_place_category_h */
	maps_item_list_h categories;

	/* List of attributes
	* Obtained with maps_place_attributes_cb
	* Each of type: maps_place_attribute_h */
	maps_item_list_h attribute;

	/* List of contacts
	 * Obtained with: maps_place_contacts_cb
	 * Each of type: maps_place_contact_h */
	maps_item_list_h contacts;

	/* List of editorials
	 * Obtained with: maps_place_editorials_cb
	 * Each of type: maps_place_editorial_h */
	maps_item_list_h editorials;

	/* List of images
	 * Obtained with: maps_place_images_cb callback
	 * Each of type: maps_place_image_h */
	maps_item_list_h images;

	/* List of reviews
	 * Obtained with: maps_place_reviews_cb callback
	 * Each of type: maps_place_review_h */
	maps_item_list_h reviews;

	maps_place_link_object_h supplier;
	maps_place_link_object_h related;

	/* The table of available data features */
	maps_int_hashtable_h supported_data;

} maps_place_s;

/* TODO: extract all such constants to the dedcated header file */
const gsize _MAPS_PLACE_ID_MAX_LENGTH = 64;
const gsize _MAPS_PLACE_NAME_MAX_LENGTH = 64;
const gsize _MAPS_PLACE_URI_MAX_LENGTH = 256;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_create(maps_place_h *place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_h) g_slice_new0(maps_place_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_destroy(maps_place_h place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_s *p = (maps_place_s *) place;

	if (p->id)
		g_free(p->id);
	if (p->name)
		g_free(p->name);
	if (p->uri)
		g_free(p->uri);

	if (p->location)
		maps_coordinates_destroy(p->location);

	if (p->address)
		maps_address_destroy(p->address);

	if (p->rating)
		maps_place_rating_destroy(p->rating);

	if (p->properties)
		maps_item_hashtable_destroy(p->properties);

	if (p->categories) {
		maps_item_list_remove_all(p->categories,
			maps_place_category_destroy);
		maps_item_list_destroy(p->categories);
	}

	if (p->attribute) {
		maps_item_list_remove_all(p->attribute,
			maps_place_attribute_destroy);
		maps_item_list_destroy(p->attribute);
	}

	if (p->contacts) {
		maps_item_list_remove_all(p->contacts,
			maps_place_contact_destroy);
		maps_item_list_destroy(p->contacts);
	}

	if (p->editorials) {
		maps_item_list_remove_all(p->editorials,
			maps_place_editorial_destroy);
		maps_item_list_destroy(p->editorials);
	}

	if (p->images) {
		maps_item_list_remove_all(p->images, maps_place_image_destroy);
		maps_item_list_destroy(p->images);
	}

	if (p->reviews) {
		maps_item_list_remove_all(p->reviews,
			maps_place_review_destroy);
		maps_item_list_destroy(p->reviews);
	}

	if (p->supplier)
		maps_place_link_object_destroy(p->supplier);
	if (p->related)
		maps_place_link_object_destroy(p->related);

	if (p->supported_data)
		maps_int_hashtable_destroy(p->supported_data);

	g_slice_free(maps_place_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_clone(const maps_place_h origin,
				maps_place_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_s *p = (maps_place_s *) origin;

		if (p->id) {
			error = maps_place_set_id(*cloned, p->id);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->name) {
			error = maps_place_set_name(*cloned, p->name);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->uri) {
			error = maps_place_set_uri(*cloned, p->uri);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->location) {
			error = maps_place_set_location(*cloned, p->location);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		error = maps_place_set_distance(*cloned, p->distance);
		if (error != MAPS_ERROR_NONE)
			break;

		if (p->address) {
			error = maps_place_set_address(*cloned, p->address);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->rating) {
			error = maps_place_set_rating(*cloned, p->rating);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->properties) {
			error = maps_place_set_properties(*cloned,
				p->properties);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->categories) {
			error = maps_place_set_categories(*cloned,
				p->categories);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->attribute) {
			error = maps_place_set_attributes(*cloned,
				p->attribute);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->contacts) {
			error = maps_place_set_contacts(*cloned, p->contacts);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->editorials) {
			error = maps_place_set_editorials(*cloned,
				p->editorials);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->images) {
			error = maps_place_set_images(*cloned, p->images);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->reviews) {
			error = maps_place_set_reviews(*cloned, p->reviews);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->supplier) {
			error = maps_place_set_supplier_link(*cloned,
				p->supplier);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->related) {
			error = maps_place_set_related_link(*cloned,
				p->related);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (p->supported_data) {
			error = _maps_place_set_supported_data(*cloned,
				p->supported_data);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	maps_place_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_get_id(const maps_place_h place, char **id)
{
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_s *) place)->id,
		_MAPS_PLACE_ID_MAX_LENGTH, id);
}

EXPORT_API int maps_place_get_name(const maps_place_h place, char **name)
{
	if (!place || !name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_s *) place)->name,
		_MAPS_PLACE_NAME_MAX_LENGTH, name);
}

EXPORT_API int maps_place_get_uri(const maps_place_h place, char **uri)
{
	if (!place || !uri)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_s *) place)->uri,
		_MAPS_PLACE_URI_MAX_LENGTH, uri);
}

EXPORT_API int maps_place_get_location(const maps_place_h place,
				       maps_coordinates_h *location)
{
	if (!place || !location)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_coordinates_clone(((maps_place_s *) place)->location,
		location);
}

EXPORT_API int maps_place_get_distance(const maps_place_h place, int *distance)
{
	if (!place || !distance)
		return MAPS_ERROR_INVALID_PARAMETER;
	*distance = ((maps_place_s *) place)->distance;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_get_address(const maps_place_h place,
				      maps_address_h *address)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_ADDRESS))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!address)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_address_clone(((maps_place_s *) place)->address, address);
}

EXPORT_API int maps_place_get_rating(const maps_place_h place,
				     maps_place_rating_h *rating)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_RATING))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!rating)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_place_rating_clone(((maps_place_s *) place)->rating,
		rating);
}

EXPORT_API int maps_place_foreach_property(const maps_place_h place,
					   maps_place_properties_cb callback,
					   void *user_data)
{
	if (!place || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_item_hashtable_foreach(((maps_place_s *) place)->properties,
		callback, user_data);
}

EXPORT_API int maps_place_foreach_category(const maps_place_h place,
					   maps_place_categories_cb callback,
					   void *user_data)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_CATEGORIES))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_place_s *) place)->categories,
		maps_place_category_clone, callback, user_data);
}

EXPORT_API int maps_place_foreach_attribute(const maps_place_h place,
					    maps_place_attributes_cb callback,
					    void * user_data)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_ATTRIBUTES))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_place_s *) place)->attribute,
		maps_place_attribute_clone, callback, user_data);
}

EXPORT_API int maps_place_foreach_contact(const maps_place_h place,
					  maps_place_contacts_cb callback,
					  void *user_data)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_CONTACTS))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_place_s *) place)->contacts,
		maps_place_contact_clone, callback, user_data);
}

EXPORT_API int maps_place_foreach_editorial(const maps_place_h place,
					    maps_place_editorials_cb callback,
					    void *user_data)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_EDITORIALS))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_place_s *) place)->editorials,
		maps_place_editorial_clone, callback, user_data);
}

EXPORT_API int maps_place_foreach_image(const maps_place_h place,
					maps_place_images_cb callback,
					void *user_data)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_IMAGE))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_place_s *) place)->images,
		maps_place_image_clone, callback, user_data);
}

EXPORT_API int maps_place_foreach_review(const maps_place_h place,
					 maps_place_reviews_cb callback,
					 void *user_data)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_REVIEWS))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach(((maps_place_s *) place)->reviews,
		maps_place_review_clone, callback, user_data);
}

EXPORT_API int maps_place_get_supplier_link(const maps_place_image_h place,
					    maps_place_link_object_h *supplier)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_SUPPLIER))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!supplier)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_place_link_object_clone(((maps_place_s *) place)->supplier,
		supplier);
}

EXPORT_API int maps_place_get_related_link(const maps_place_image_h place,
					   maps_place_link_object_h *related)
{
	/* Check if the handle of the Place is valid */
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	/* Check if this API feature available */
	if (!__is_supported(place, MAPS_PLACE_RELATED))
		return MAPS_ERROR_SERVICE_NOT_AVAILABLE;

	/* Check if parameters are valid */
	if (!related)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_place_link_object_clone(((maps_place_s *) place)->related,
		related);
}

int _maps_place_is_data_supported(const maps_place_h place,
				  maps_service_data_e data, bool *supported)
{
	if (!place || !supported)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_s *p = (maps_place_s *)place;
	if (!p->supported_data) {
		/* This is a case when the "supported" flags are not set yet */
		/* No need to limit access to fields */
		*supported = true;
		return MAPS_ERROR_NONE;
	}

	*supported = false;
	return maps_int_hashtable_contains(p->supported_data, data, supported);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_set_id(maps_place_h place, const char *id)
{
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(id, _MAPS_PLACE_ID_MAX_LENGTH,
		&((maps_place_s *) place)->id);
}

EXPORT_API int maps_place_set_name(maps_place_h place, const char *name)
{
	if (!place || !name)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(name, _MAPS_PLACE_NAME_MAX_LENGTH,
		&((maps_place_s *) place)->name);
}

EXPORT_API int maps_place_set_uri(maps_place_h place, const char *uri)
{
	if (!place || !uri)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(uri, _MAPS_PLACE_URI_MAX_LENGTH,
		&((maps_place_s *) place)->uri);
}

EXPORT_API int maps_place_set_location(maps_place_h place,
	const maps_coordinates_h location)
{
	if (!place || !location)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->location)
		maps_coordinates_destroy(p->location);
	return maps_coordinates_clone(location, &p->location);
}

EXPORT_API int maps_place_set_distance(maps_place_h place, const int distance)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	p->distance = distance;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_set_address(maps_place_h place,
				      const maps_address_h address)
{
	if (!place || !address)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->address)
		maps_address_destroy(p->address);
	return maps_address_clone(address, &p->address);
}

EXPORT_API int maps_place_set_categories(maps_place_h place,
					 const maps_item_list_h categories)
{
	if (!place || !categories)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->categories) {
		maps_item_list_remove_all(p->categories,
			maps_place_category_destroy);
		maps_item_list_destroy(p->categories);
	}
	return maps_item_list_clone(categories, maps_place_category_clone,
		&p->categories);
}

EXPORT_API int maps_place_set_attributes(maps_place_h place,
					 const maps_item_list_h attributes)
{
	if (!place || !attributes)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->attribute) {
		maps_item_list_remove_all(p->attribute,
			maps_place_attribute_destroy);
		maps_item_list_destroy(p->attribute);
	}
	return maps_item_list_clone(attributes, maps_place_attribute_clone,
		&p->attribute);
}

EXPORT_API int maps_place_set_contacts(maps_place_h place,
				       const maps_item_list_h contacts)
{
	if (!place || !contacts)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->contacts) {
		maps_item_list_remove_all(p->contacts,
			maps_place_contact_destroy);
		maps_item_list_destroy(p->contacts);
	}
	return maps_item_list_clone(contacts, maps_place_contact_clone,
		&p->contacts);
}

EXPORT_API int maps_place_set_editorials(maps_place_h place,
					 const maps_item_list_h editorials)
{
	if (!place || !editorials)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->editorials) {
		maps_item_list_remove_all(p->editorials,
			maps_place_editorial_destroy);
		maps_item_list_destroy(p->editorials);
	}
	return maps_item_list_clone(editorials, maps_place_editorial_clone,
		&p->editorials);
}

EXPORT_API int maps_place_set_images(maps_place_h place,
				     const maps_item_list_h images)
{
	if (!place || !images)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->images) {
		maps_item_list_remove_all(p->images, maps_place_image_destroy);
		maps_item_list_destroy(p->images);
	}
	return maps_item_list_clone(images, maps_place_image_clone, &p->images);
}

EXPORT_API int maps_place_set_reviews(maps_place_h place,
				      const maps_item_list_h reviews)
{
	if (!place || !reviews)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->reviews) {
		maps_item_list_remove_all(p->reviews,
			maps_place_review_destroy);
		maps_item_list_destroy(p->reviews);
	}
	return maps_item_list_clone(reviews, maps_place_review_clone,
		&p->reviews);
}

EXPORT_API int maps_place_set_properties(maps_place_h place,
					 const maps_item_hashtable_h properties)
{
	if (!place || !properties)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->properties)
		maps_item_hashtable_destroy(p->properties);
	return maps_item_hashtable_clone(properties, &p->properties);
}

EXPORT_API int maps_place_set_rating(maps_place_h place,
				     const maps_place_rating_h rating)
{
	if (!place || !rating)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->rating)
		maps_place_rating_destroy(p->rating);
	return maps_place_rating_clone(rating, &p->rating);
}

EXPORT_API int maps_place_set_supplier_link(maps_place_h place,
					    const maps_place_link_object_h
					    supplier)
{
	if (!place || !supplier)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->supplier)
		maps_place_link_object_destroy(p->supplier);
	return maps_place_link_object_clone(supplier, &p->supplier);
}

EXPORT_API int maps_place_set_related_link(maps_place_h place,
					   const maps_place_link_object_h
					   related)
{
	if (!place || !related)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->related)
		maps_place_link_object_destroy(p->related);
	return maps_place_link_object_clone(related, &p->related);
}

int _maps_place_set_supported_data(maps_place_h place,
				   const maps_int_hashtable_h supported_data)
{
	if (!place || !supported_data)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_s *p = (maps_place_s *) place;
	if (p->supported_data)
		maps_int_hashtable_destroy(p->supported_data);
	return maps_int_hashtable_clone(supported_data, &p->supported_data);
}

EXPORT_API int maps_place_list_foreach(const maps_place_list_h place_list,
				maps_place_cb callback, void *user_data)
{
	if (!place_list || !callback)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_foreach_noclone((maps_item_list_h) place_list, callback, user_data);
}

EXPORT_API int maps_place_list_create(maps_place_list_h *place_list)
{
	if (!place_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	return maps_item_list_create(place_list);
}

EXPORT_API int maps_place_list_destroy(maps_place_list_h place_list)
{
	if (!place_list)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;

	error = maps_item_list_remove_all(place_list, maps_place_destroy);
	if (error != MAPS_ERROR_NONE)
		return error;

	return maps_item_list_destroy(place_list);
}

