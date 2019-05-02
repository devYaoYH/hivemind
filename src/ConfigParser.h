#pragma once
#include "includes.h"
using namespace std;

/*
* Abstract ConfigParser to expose interface
*/

class ConfigParser {
	public:
        ConfigParser() {}
        virtual ~ConfigParser() {}
		virtual void getAttrs(map<string, string>& kvmap, vector<string>& expanded_objs, const string keySeq, const string fname) = 0;
};
