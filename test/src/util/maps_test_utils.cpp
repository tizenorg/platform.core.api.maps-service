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

#include "maps_test_utils.h"
#include <glib.h>
#include <cstdlib>
#include <sys/timeb.h>

void __utc_print_error_string(int error)
{
	switch (error) {
	case MAPS_ERROR_NONE:
		g_print("MAPS_ERROR_NONE\n");
		break;
	case MAPS_ERROR_OUT_OF_MEMORY:
		g_print("MAPS_ERROR_OUT_OF_MEMORY\n");
		break;
	case MAPS_ERROR_INVALID_PARAMETER:
		g_print("MAPS_ERROR_INVALID_PARAMETER\n");
		break;
	case MAPS_ERROR_NOT_SUPPORTED:
		g_print("MAPS_ERROR_NOT_SUPPORTED\n");
		break;
	case MAPS_ERROR_CONNECTION_TIME_OUT:
		g_print("MAPS_ERROR_CONNECTION_TIME_OUT\n");
		break;
	case MAPS_ERROR_NETWORK_UNREACHABLE:
		g_print("MAPS_ERROR_NETWORK_UNREACHABLE\n");
		break;
	case MAPS_ERROR_SERVICE_NOT_AVAILABLE:
		g_print("MAPS_ERROR_SERVICE_NOT_AVAILABLE\n");
		break;
	case MAPS_ERROR_INVALID_OPERATION:
		g_print("MAPS_ERROR_INVALID_OPERATION\n");
		break;
	case MAPS_ERROR_NOT_FOUND:
		g_print("MAPS_ERROR_NOT_FOUND\n");
		break;
	case MAPS_ERROR_KEY_NOT_AVAILABLE:
		g_print("MAPS_ERROR_KEY_NOT_AVAILABLE\n");
		break;
	case MAPS_ERROR_RESOURCE_BUSY:
		g_print("MAPS_ERROR_RESOURCE_BUSY\n");
		break;
	case MAPS_ERROR_CANCELED:
		g_print("MAPS_ERROR_CANCELED\n");
		break;
	case MAPS_ERROR_UNKNOWN:
		g_print("MAPS_ERROR_UNKNOWN\n");
		break;
	default:
		g_print("Unknown Error Code\n");
		break;
	}
}

int __get_milli_count()
{
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff)* 1000;
	return nCount;
}

int __get_milli_span(int start_t)
{
	int nSpan = __get_milli_count() - start_t;
	if (nSpan < 0)
		nSpan += 0x100000* 1000;
	return nSpan;
}

