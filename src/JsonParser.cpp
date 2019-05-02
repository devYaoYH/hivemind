#include "JsonParser.h"
using namespace std;
using json = nlohmann::json;

json* JsonParser::config_data = nullptr;
string JsonParser::cur_config_fname = "";

JsonParser::JsonParser() {
	if (config_data == 0) config_data = new json();
}

void JsonParser::getAttrs(map<string, string>& kvmap, vector<string>& expanded_objs, const string keySeq, const string fname) {
	if (fname.compare(cur_config_fname)) {
		//Walk keySeq and retrieve from cache
		kvmap.clear();
		expanded_objs.clear();
		//Load data from our json object
		stringstream ss(keySeq);
		string nKey;
		if (ss >> nKey) {
			auto tmp_inner = (*config_data)[nKey];
			bool exists = true;
			while (!ss.eof() && ss >> nKey) {
				//Need to figure out how to extract nested information
				if (tmp_inner.find(nKey) != tmp_inner.end()) {
					tmp_inner = tmp_inner[nKey];
				}
				else {
					exists = false;
				}
			}
			if (exists) {
				for (auto& item : tmp_inner.items()) {
					if (item.value().is_structured()) {
						expanded_objs.push_back(keySeq + " " + item.key());
					}
					else {
						kvmap.insert(pair<string, string>(item.key(), item.value()));
					}
				}
			}
		}
	}
	else {
		//File I/O Load to config_data
		if (load_file(fname)) cur_config_fname = fname;
		else return;
		//Recursively call getAttrs to walk keySeq
		//Make sure we don't dive into the rabbit hole
		return getAttrs(kvmap, expanded_objs, keySeq, cur_config_fname);
	}
}

// We use a JSON Parser Library
// Properly catch the exceptions which can arise from parsing improperly formatted .json config files
bool JsonParser::load_file(string fname) {
	ifstream fin(fname);
	if (!fin.is_open()) {
		cout << "File: " << fname << " cannot be Opened!" << endl;
		return false;
	}
	try {
		fin >> *config_data;
	}
	catch (json::parse_error& err) {
		cout << err.what() << endl;
		fin.close();
		return false;
	}
	fin.close();
	return true;
}

JsonParser::~JsonParser() {
	delete config_data;
}
