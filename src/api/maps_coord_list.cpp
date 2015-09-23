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

#include <glib.h>
#include "maps_coord_list.h"
#include "maps_error.h"
#include "maps_extra_types.h"
#include "maps_util.h"

EXPORT_API int maps_coord_list_create(maps_coord_list_h *list)
{
	MAPS_LOG_API;
	return maps_item_list_create(list);
}

EXPORT_API int maps_coord_list_destroy(maps_coord_list_h list)
{
	MAPS_LOG_API;
	maps_coord_list_remove_all(list);
	return maps_item_list_destroy(list);
}

EXPORT_API int maps_coord_list_append(maps_coord_list_h list,
				      const maps_coordinates_h data)
{
	MAPS_LOG_API;
	return maps_item_list_append(list, data, maps_coordinates_clone);
}

EXPORT_API int maps_coord_list_foreach(maps_coord_list_h list,
				       maps_coord_list_foreach_cb callback,
				       void *user_data)
{
	MAPS_LOG_API;
	return maps_item_list_foreach(list,
				      /*maps_item_no_clone,*/
				      maps_coordinates_clone,
				      callback,
				      user_data);
}

EXPORT_API int maps_coord_list_remove(maps_coord_list_h list,
				      maps_coordinates_h item)
{
	MAPS_LOG_API;
	return maps_item_list_remove(list, item, maps_coordinates_destroy);
}

EXPORT_API int maps_coord_list_remove_all(maps_coord_list_h list)
{
	MAPS_LOG_API;
	return maps_item_list_remove_all(list, maps_coordinates_destroy);
}

EXPORT_API int maps_coord_list_clone(const maps_coord_list_h origin,
				     maps_coord_list_h *cloned)
{
	MAPS_LOG_API;
	return maps_item_list_clone(origin,
				    maps_coordinates_clone,
				    cloned);
}
