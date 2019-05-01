#pragma once
#include "ConfigParser.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

/*
 * Concrete JSON Parser implements ConfigParser
 *	-> Caches last-queried json map to speedup retrieval (less file I/O overhead)
 * JSON Parser Library from: https://github.com/nlohmann/json
 */

class JsonParser: public ConfigParser {
	public:
		JsonParser();
		~JsonParser();
		virtual void getAttrs(map<string, string>& kvmap, vector<string>& expanded_objs, const string keySeq, const string fname);
	private:
		static json* config_data;
		static string cur_config_fname;
		bool load_file(string fname);
};