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

#ifndef __MAPS_SERVICE_PLUGIN_DISCOVERY_H__
#define __MAPS_SERVICE_PLUGIN_DISCOVERY_H__

//#include "maps_service.h"
#include "maps_util.h"

namespace plugin
{

	class provider_info
	{
	public:
		string provider;
		string file;
	public:
		static provider_info empty_instance;
	public:
		provider_info();
		provider_info(const string &provider, const string &file);
		provider_info(const provider_info &src);
		virtual ~provider_info()
		{
		};
	public:
		 provider_info & operator=(const provider_info &src);
		/* operator maps_provider_info_h() const; */
	public:
		void construct(const string &provider, const string &file);
		bool empty() const;
	};

	class discovery
	{
	public:
		discovery();
		virtual ~discovery();
	public:
		vector<provider_info> get_available_list() const;
	private:
		 vector<string> get_module_file_list() const;
	};

	provider_info find_by_names(const string &provider);
}

#endif				/* __MAPS_SERVICE_PLUGIN_DISCOVERY_H__ */