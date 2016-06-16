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

#include <image_util.h>

#include <maps_view_plugin.h>
#include <module.h>
#include <empty_module.h>
#include <stdlib.h>


const plugin::interface_s *__get_plugin_interface(maps_view_h view);

static void __convert_rgba_to_bgra(unsigned char *data, int width, int height)
{
	int len = width * height;
	unsigned int *p = (unsigned int *)data;
	for (int i = 0; i < len; i++)
	{
		*p = (*p & 0xFF00FF00) |
			((*p & 0x00FF0000) >> 16) |
			((*p & 0x000000FF) << 16);
		p++;
	};
}

static bool __encode_bitmap_file(const void *data, int width, int height, const char *file)
{
       struct {
              unsigned char magic[2];
       } bmpfile_magic = { {'B', 'M'} };

       struct {
              unsigned int filesz;
              unsigned short creator1;
              unsigned short creator2;
              unsigned int bmp_offset;
       } bmpfile_header = { 0, 0, 0, 0x36 };

       struct {
              unsigned int header_sz;
              unsigned int width;
              unsigned int height;
              unsigned short nplanes;
              unsigned short bitspp;
              unsigned int compress_type;
              unsigned int bmp_bytesz;
              unsigned int hres;
              unsigned int vres;
              unsigned int ncolors;
              unsigned int nimpcolors;
       } bmp_dib_v3_header_t = { 0x28, 0, 0, 1, 24, 0, 0, 0, 0, 0, 0 };

       unsigned int *blocks;
       FILE *fp = fopen(file, "w+");
       int i;

       if (fp == NULL) {
              MAPS_LOGE("fopen fail");	//LCOV_EXCL_LINE
              return false;
       }
       bmpfile_header.filesz = sizeof(bmpfile_magic) + sizeof(bmpfile_header) + sizeof(bmp_dib_v3_header_t) + (width * height * 3);
       bmp_dib_v3_header_t.header_sz = sizeof(bmp_dib_v3_header_t);
       bmp_dib_v3_header_t.width = width;
       bmp_dib_v3_header_t.height = -height;
       bmp_dib_v3_header_t.bmp_bytesz = width * height * 3;

       fwrite(&bmpfile_magic, sizeof(bmpfile_magic), 1, fp);
       fwrite(&bmpfile_header, sizeof(bmpfile_header), 1, fp);
       fwrite(&bmp_dib_v3_header_t, sizeof(bmp_dib_v3_header_t), 1, fp);
       blocks = (unsigned int *)data;

       for (i = 0; i < height * width; i++) {
              fwrite(&blocks[i], 3, 1, fp);
       }
       fclose(fp);
	   return true;
}

static image_util_colorspace_e __convert_colorspace(maps_view_colorspace_type_e cs)
{
	switch (cs) {
		case MAPS_VIEW_COLORSPACE_RGBA8888:
			return IMAGE_UTIL_COLORSPACE_RGBA8888;
		case MAPS_VIEW_COLORSPACE_BGRA8888:
			return IMAGE_UTIL_COLORSPACE_BGRA8888;
	}
	return IMAGE_UTIL_COLORSPACE_RGBA8888;
}

EXPORT_API int maps_view_capture_snapshot(maps_view_h view,
											maps_view_snapshot_format_type_e type,
											int quality,
											const char *path)
{
	if (!view || type < MAPS_VIEW_SNAPSHOT_BMP || type > MAPS_VIEW_SNAPSHOT_JPEG ||
		quality < 0 || quality > 100 || !path || (path && *path == '\0'))
		return MAPS_ERROR_INVALID_PARAMETER;

	int error = MAPS_ERROR_UNKNOWN;
	int w, h;
	unsigned char *image_buffer = NULL;
	maps_view_colorspace_type_e cs = MAPS_VIEW_COLORSPACE_RGBA8888;
	char *fname = NULL;

	do {
		if (!__get_plugin_interface(view)->maps_plugin_capture_snapshot)
			return  MAPS_ERROR_SERVICE_NOT_AVAILABLE;

		error = __get_plugin_interface(view)->maps_plugin_capture_snapshot(view,
												(void**)&image_buffer, &w, &h, &cs);
		if (error != MAPS_ERROR_NONE) break;
		if (cs != MAPS_VIEW_COLORSPACE_RGBA8888 && cs != MAPS_VIEW_COLORSPACE_BGRA8888) {
//LCOV_EXCL_START
			MAPS_LOGE("The color space is not supported yet. (%d)", cs);
			error = MAPS_ERROR_INVALID_OPERATION;
//LCOV_EXCL_STOP
			break;
		}

		int fname_len = strlen(path) + 4;
		fname = (char*)malloc(fname_len + 1);
		if (!fname)
			return MAPS_ERROR_INVALID_OPERATION;

		memset(fname, 0, fname_len + 1);
		sprintf(fname, "%s.tmp", path);

		if (type == MAPS_VIEW_SNAPSHOT_JPEG) {
			error = image_util_encode_jpeg(image_buffer, w, h,
											__convert_colorspace(cs), quality, fname);
			if (error != IMAGE_UTIL_ERROR_NONE) {
				MAPS_LOGD("image_buffer=%p, w=%d, h=%d, cs=%d, quality=%d, fname=%s",
					image_buffer, w, h, cs, quality, fname);
				MAPS_LOGE("Failed to encode it with JPEG format. error=%d", error);
				error = MAPS_ERROR_INVALID_OPERATION;
			}
		} else if (type == MAPS_VIEW_SNAPSHOT_BMP) {
			if (cs == MAPS_VIEW_COLORSPACE_RGBA8888) {
				MAPS_LOGD("RGBA -> BGRA");
				__convert_rgba_to_bgra(image_buffer, w, h);
				cs = MAPS_VIEW_COLORSPACE_BGRA8888;
			}
			if (!__encode_bitmap_file(image_buffer, w, h, fname)) {
//LCOV_EXCL_START
				MAPS_LOGE("Failed to store it to a file.");
				error = MAPS_ERROR_INVALID_OPERATION;
//LCOV_EXCL_STOP
			}
		} else {
			error = MAPS_ERROR_INVALID_PARAMETER;
		}
	} while (0);

	if (fname) {
		if (error == MAPS_ERROR_NONE) {
			remove(path);
			rename(fname, path);
		} else {
			remove(fname);	//LCOV_EXCL_LINE
		}
		free(fname);
	}
	g_free(image_buffer);
	return error;
}
