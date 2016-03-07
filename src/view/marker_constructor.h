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

#ifndef __MAPS_VIEW_MARKER_CONSTRUCTOR_H__
#define __MAPS_VIEW_MARKER_CONSTRUCTOR_H__

#include "maps_error.h"
#include "maps_view_object.h"

namespace view
{
	class marker_constructor {
	private:
		int __error;
	public:
		marker_constructor()
			: __error(MAPS_ERROR_NONE)
		{
		}
	public:
		maps_view_object_h construct(maps_coordinates_h coordinates,
				       const char *image_file_path,
				       maps_view_marker_type_e type);
		int get_error() const
		{
			return __error;
		}
	};
};

#endif				/* __MAPS_VIEW_MARKER_CONSTRUCTOR_H__ */
