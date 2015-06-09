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
#include "maps_place_image_plugin.h"
#include <glib.h>
#include "maps_place_media_plugin.h"
#include "maps_place_link_object_plugin.h"
#include <glib.h>
#include "maps_util.h"

typedef struct _maps_place_image_s
{
	char *id;
	char *url;
	int width;
	int height;
	maps_place_media_h media;
	maps_place_link_object_h user;
} maps_place_image_s;

const gsize _MAPS_PLACE_IMAGE_ID_MAX_LENGTH = 32;
const gsize _MAPS_PLACE_IMAGE_URL_MAX_LENGTH = 128;

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_image_create(maps_place_image_h *place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	*place = (maps_place_image_h) g_slice_new0(maps_place_image_s);

	if (*place == NULL) {
		MAPS_LOGE("OUT_OF_MEMORY(0x%08x)", MAPS_ERROR_OUT_OF_MEMORY);
		return MAPS_ERROR_OUT_OF_MEMORY;
	}

	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_image_destroy(maps_place_image_h place)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;

	maps_place_image_s *i = (maps_place_image_s *) place;

	if (i->id)
		g_free(i->id);
	if (i->url)
		g_free(i->url);

	if (i->media)
		maps_place_media_destroy(i->media);
	if (i->user)
		maps_place_link_object_destroy(i->user);

	g_slice_free(maps_place_image_s, place);
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_image_clone(const maps_place_image_h origin,
				      maps_place_image_h *cloned)
{
	if (!cloned || !origin)
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_NONE;
	do {
		error = maps_place_image_create(cloned);
		if (!(*cloned) || (error != MAPS_ERROR_NONE))
			break;

		maps_place_image_s *i = (maps_place_image_s *) origin;

		if (i->id) {
			error = maps_place_image_set_id(*cloned, i->id);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (i->url) {
			error = maps_place_image_set_url(*cloned, i->url);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		error = maps_place_image_set_width(*cloned, i->width);
		if (error != MAPS_ERROR_NONE)
			break;

		error = maps_place_image_set_height(*cloned, i->height);
		if (error != MAPS_ERROR_NONE)
			break;

		if (i->media) {
			error = maps_place_image_set_media(*cloned, i->media);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		if (i->user) {
			error = maps_place_image_set_user_link(*cloned,
				i->user);
			if (error != MAPS_ERROR_NONE)
				break;
		}

		return MAPS_ERROR_NONE;
	} while (false);

	maps_place_image_destroy(*cloned);
	*cloned = NULL;
	return error;
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_image_get_url(const maps_place_image_h place,
					char **url)
{
	if (!place || !url)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_image_s *) place)->url,
		_MAPS_PLACE_IMAGE_URL_MAX_LENGTH, url);
}

EXPORT_API int maps_place_image_get_id(const maps_place_image_h place,
				       char **id)
{
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_get_string(((maps_place_image_s *) place)->id,
		_MAPS_PLACE_IMAGE_ID_MAX_LENGTH, id);
}

EXPORT_API int maps_place_image_get_width(const maps_place_image_h place,
					  int *width)
{
	if (!place || !width)
		return MAPS_ERROR_INVALID_PARAMETER;
	*width = ((maps_place_image_s *) place)->width;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_image_get_height(const maps_place_image_h place,
					   int *height)
{
	if (!place || !height)
		return MAPS_ERROR_INVALID_PARAMETER;
	*height = ((maps_place_image_s *) place)->height;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_image_get_user_link(const maps_place_image_h place,
					      maps_place_link_object_h *user)
{
	if (!place || !user)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_place_link_object_clone(((maps_place_image_s *) place)->
		user, user);
}

EXPORT_API int maps_place_image_get_media(const maps_place_image_h place,
					  maps_place_media_h *media)
{
	if (!place || !media)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_place_media_clone(((maps_place_image_s *) place)->media,
		media);
}

/*----------------------------------------------------------------------------*/

EXPORT_API int maps_place_image_set_id(maps_place_image_h place, const char *id)
{
	if (!place || !id)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(id, _MAPS_PLACE_IMAGE_ID_MAX_LENGTH,
		&((maps_place_image_s *) place)->id);
}

EXPORT_API int maps_place_image_set_url(maps_place_image_h place,
					const char *url)
{
	if (!place || !url)
		return MAPS_ERROR_INVALID_PARAMETER;
	return maps_set_string(url, _MAPS_PLACE_IMAGE_URL_MAX_LENGTH,
		&((maps_place_image_s *) place)->url);
}

EXPORT_API int maps_place_image_set_width(maps_place_image_h place,
					  const int width)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_place_image_s *) place)->width = width;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_image_set_height(maps_place_image_h place,
					   const int height)
{
	if (!place)
		return MAPS_ERROR_INVALID_PARAMETER;
	((maps_place_image_s *) place)->height = height;
	return MAPS_ERROR_NONE;
}

EXPORT_API int maps_place_image_set_user_link(maps_place_image_h place,
					      const maps_place_link_object_h
					      user)
{
	if (!place || !user)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_image_s *i = (maps_place_image_s *) place;
	if (i->user)
		maps_place_link_object_destroy(i->user);
	return maps_place_link_object_clone(user, &i->user);
}

EXPORT_API int maps_place_image_set_media(maps_place_image_h place,
					  const maps_place_media_h media)
{
	if (!place || !media)
		return MAPS_ERROR_INVALID_PARAMETER;
	maps_place_image_s *i = (maps_place_image_s *) place;
	if (i->media)
		maps_place_media_destroy(i->media);
	return maps_place_media_clone(media, &i->media);
}
