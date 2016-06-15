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

#ifndef __MAPS_UTIL_H__
#define __MAPS_UTIL_H__

#include <dlog.h>
#include <glib.h>

/*#include "stdio.h"

#ifndef EXPORT_API
#define EXPORT_API __attribute__((__visibility__("default")))
#endif*/

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "CAPI_MAPS_SERVICE"

/*
* Internal Macros
*/
#define MAPS_LOGD(fmt, args...)  LOGD(fmt, ##args)
#define MAPS_LOGW(fmt, args...)  LOGW(fmt, ##args)
#define MAPS_LOGI(fmt, args...)  LOGI(fmt, ##args)
#define MAPS_LOGE(fmt, args...)  LOGE(fmt, ##args)
#define MAPS_SECLOG(fmt, args...)  SECURE_LOGD(fmt, ##args)

#define MAPS_CHECK_CONDITION(condition, error, msg)	\
	do { \
		if (condition) { \
		} else { \
			MAPS_LOGE("%s(0x%08x)", msg, error); \
			return error; \
		} \
	} while (0)

#define MAPS_NULL_ARG_CHECK_RETURN_FALSE(arg)\
	do { \
		if (arg != NULL) { \
		} else	{ \
			MAPS_LOGE("MAPS_ERROR_INVALID_PARAMETER");  \
			return false; };	\
	} while (0)

#define MAPS_NULL_ARG_CHECK(arg)	\
	MAPS_CHECK_CONDITION(arg != NULL, MAPS_ERROR_INVALID_PARAMETER, "MAPS_ERROR_INVALID_PARAMETER")

#define MAPS_PRINT_ERROR_CODE_RETURN(code) \
	do { \
		MAPS_LOGE("%s(0x%08x)", #code, code); \
		return code;	\
	} while (0)

/*----------------------------------------------------------------------------*/

#define _S(s) #s

/*
 * @brief	Copies one string to another and releases receiver if needed.
 * @details This function copies one string to another and releases receiver if
 * needed.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[out]	dst		The destination string pointer.
 * @param[in]	src		The original string pointer.
 * @param[in]	max_length	The maximum size of bytes to be copied.
 * @return	0 on success, otherwise a negative error value.
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_set_string(const char *src, const int max_length, char **dst);

/*
 * @brief	Copies one string to another.
 * @details This function copies one string to another.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[out]	dst		The destination string pointer.
 * @param[in]	src		The original string pointer.
 * @param[in]	max_length	The maximum size of bytes to be copied.
 * @return	0 on success, otherwise a negative error value.
 * @retval	#MAPS_ERROR_NONE Successful
 * @retval	#MAPS_ERROR_INVALID_PARAMETER Invalid parameter
 */
int maps_get_string(const char *src, const int max_length, char **dst);

/* Prevent utility highlights defects in std::vector and std::string, so
*  simplified versions of that classes are implemented */
template <class T> class vector {
private:
	GArray *parray;
	unsigned int current_size;
public:
	vector() : parray(NULL), current_size(0)
	{
		parray = g_array_new(false, false, sizeof(T *));
	}
	~vector()
	{
		/*if (!parray || (current_size == 0))*/
		if (!parray)
			return;
		for (unsigned int i = 0; i < current_size; i++) {
			T *item = g_array_index(parray, T *, i);
			if (item)
				delete item;
		}
		g_array_free(parray, TRUE);
		parray = NULL;
		current_size = 0;
	}
public:
	void push_back(const T &value)
	{
		T *clone = new T(value);
		g_array_append_val(parray, clone);
		current_size++;
	}
	T &operator[](size_t idx) const
	{
		T *item = g_array_index(parray, T *, idx);
		 return *item;
	}
	unsigned int size() const
	{
		return current_size;
	}
	bool empty() const
	{
		return size() == 0;
	}
};

class string
{
private:
	char *pstring;
public:
	string() : pstring(NULL)
	{
	}
	string(const char *pstr) : pstring(NULL)
	{
		pstring = g_strdup(pstr);
	}
	string(const string &s) : pstring(NULL)
	{
		pstring = g_strdup(s.pstring);
	}
	~string()
	{
		g_free(pstring);
		pstring = NULL;
	}
public:
	string &operator=(const string &s)
	{
		if (this != &s) {
			g_free(pstring);
			pstring = g_strdup(s.pstring);
		}
		return *this;
	}
	bool operator==(const string &s) const
	{
		return g_strcmp0(pstring, s.pstring) == 0;
	}
	bool operator!=(const string &s) const
	{
		return !(*this == s);
	}
public:
	char *c_str() const
	{
		return pstring;
	}
	bool empty() const
	{
		return !pstring || (*this == string(""));
	}
};

#endif				/* __MAPS_UTIL_H__ */
