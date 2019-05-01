#include "stdafx.h"
#include "AgentInterface.h"
using namespace std;

AgentInterface::AgentInterface() : is_auto(true) {}

map<string, string>* AgentInterface::getPlayer() {
	return meta_data;
}

bool AgentInterface::isAuto() {
	return is_auto;
}

void AgentInterface::load_meta(const string keySeq, const string fname) {
	Parser* config_parser = new Parser();
	vector<string> tmp_objs;
	config_parser->getAttrs(*meta_data, tmp_objs, keySeq, fname);
}

AgentInterface::~AgentInterface() {
	delete meta_data;
}