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
#include "maps_place_review_plugin.h"
#include "maps_place_media_plugin.h"
#include "maps_place_link_object_plugin.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_review_s
{
	char *date;
	char *title;
	double rating;
	char *description;
	char *language;
	maps_place_media_h media;
	maps_place_link_object_h user;
} maps_place_review_s;

const gsize _MAPS_PLACE_REVIEW_DATE_MAX_LENGTH = 64;
const gsize _MAPS_PLACE_REVIEW_TITLE_MAX_LENGTH = 264;
const gsize _MAPS_PLACE_REVIEW_DESCRIPTION_MAX_LENGTH = 512;
const gsize _MAPS_PLACE_REVIEW_LANGUAGE_MAX_LENGTH = 32;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_review_create(maps_place_review_h *place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_review_h) g_slice_new0(maps_place_review_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_review_destroy(maps_place_review_h place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_review_s *r = (maps_place_review_s *) place;

	if (r->date)
		g_free(r->date);
	if (r->title)
		g_free(r->title);
	if (r->description)
		g_free(r->description);
	if (r->language)
		g_free(r->language);

	if (r->media)
		maps_place_media_destroy(r->media);
	if (r->user)
		maps_place_link_object_destroy(r->user);

	g_slice_free(maps_place_review_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_review_clone(const maps_place_review_h origin,
				       maps_place_review_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_review_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_review_s *r = (maps_place_review_s *) origin;

		if (r->date) {
			error = maps_place_review_set_date(*cloned, r->date);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (r->title) {
			error = maps_place_review_set_title(*cloned, r->title);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		((maps_place_review_s *) (*cloned))->rating = r->rating;

		if (r->description) {
			error = maps_place_review_set_description(*cloned,
				r->description);
			if (error != MAPS_ERROR_NONE)
				break;
		}
		if (r->language) {
			error = maps_place_review_set_language(*cloned,
				r->language);
			if (error != MAPS_ERROR_NONE)
				break;
		}
		if (r->media) {
			error = maps_place_review_set_media(*cloned, r->media);
			if (error != MAPS_ERROR_NONE)
				break;
		}
		if (r->user) {
			error = maps_place_review_set_user_link(*cloned,
				r->user);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	//LCOV_EXCL_START
	maps_place_review_destroy(*cloned);
	*cloned = NULL;
	return error;
	//LCOV_EXCL_STOP
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_review_get_date(const maps_place_review_h place,
								char **date)
{
	if (!place || !date)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_review_s *) place)->date,
		_MAPS_PLACE_REVIEW_DATE_MAX_LENGTH, date);
}

EXPORT_API int maps_place_review_get_title(const maps_place_review_h place,
								char **title)
{
	if (!place || !title)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_review_s *) place)->title,
		_MAPS_PLACE_REVIEW_TITLE_MAX_LENGTH, title);
}

EXPORT_API int maps_place_review_get_rating(const maps_place_review_h place,
								double *rating)
{
	if (!place || !rating)
		return MAPS_ERROR_INVALID_PARAMETER;
	*rating = ((maps_place_review_s *) place)->rating;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_review_get_description(const maps_place_review_h place,
								char **description)
{
	if (!place || !description)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_review_s *) place)->description,
		_MAPS_PLACE_REVIEW_DESCRIPTION_MAX_LENGTH, description);
}

EXPORT_API int maps_place_review_get_language(const maps_place_review_h place,
								char ** language)
{
	if (!place || !language)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_review_s *) place)->language,
		_MAPS_PLACE_REVIEW_LANGUAGE_MAX_LENGTH, language);
}

EXPORT_API int maps_place_review_get_media(const maps_place_review_h place,
								maps_place_media_h *media)
{
	if (!place || !media)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_place_media_clone(((maps_place_review_s *) place)->media,
		media);
}

EXPORT_API int maps_place_review_get_user_link(const maps_place_review_h place,
								maps_place_link_object_h *user)
{
	if (!place || !user)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_place_link_object_clone(((maps_place_review_s *) place)->
		user, user);
}

/*----------------------------------------------------------------------------*/
//LCOV_EXCL_START
EXPORT_API int maps_place_review_set_date(maps_place_review_h place,
								const char *date)
{
	if (!place || !date)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(date, _MAPS_PLACE_REVIEW_DATE_MAX_LENGTH,
		&((maps_place_review_s *) place)->date);
}

EXPORT_API int maps_place_review_set_title(maps_place_review_h place,
								const char *title)
{
	if (!place || !title)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(title, _MAPS_PLACE_REVIEW_TITLE_MAX_LENGTH,
		&((maps_place_review_s *) place)->title);
}

EXPORT_API int maps_place_review_set_rating(maps_place_review_h place,
								const double rating)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_place_review_s *) place)->rating = rating;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_review_set_description(maps_place_review_h place,
								const char *description)
{
	if (!place || !description)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(description,
		_MAPS_PLACE_REVIEW_DESCRIPTION_MAX_LENGTH,
		&((maps_place_review_s *) place)->description);
}

EXPORT_API int maps_place_review_set_language(maps_place_review_h place,
								const char *language)
{
	if (!place || !language)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(language, _MAPS_PLACE_REVIEW_LANGUAGE_MAX_LENGTH,
		&((maps_place_review_s *) place)->language);
}

EXPORT_API int maps_place_review_set_media(maps_place_review_h place,
								const maps_place_media_h media)
{
	if (!place || !media)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_review_s *r = (maps_place_review_s *) place;
	if (r->media)
		maps_place_media_destroy(r->media);
	return maps_place_media_clone(media, &r->media);
}

EXPORT_API int maps_place_review_set_user_link(maps_place_review_h place,
								const maps_place_link_object_h user)
{
	if (!place || !user)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_review_s *r = (maps_place_review_s *) place;
	if (r->user)
		maps_place_link_object_destroy(r->user);
	return maps_place_link_object_clone(user, &r->user);
}
//LCOV_EXCL_STOP
