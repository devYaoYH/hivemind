#include "Parser.h"
using namespace std;

int Parser::conut = 0;
ConfigParser* Parser::parser = nullptr;

Parser::Parser(){
    conut++;
}

void Parser::getAttrs(map<string, string>& kvmap, vector<string>& expanded_objs, const string keySeq, const string fname) {
    getParser()->getAttrs(kvmap, expanded_objs, keySeq, fname);
}

ConfigParser* Parser::getParser(){
    if (parser == nullptr){
        parser = new JsonParser();
    }
    return parser;
}

Parser::~Parser(){
    conut--;
    if (conut == 0){
        delete parser;
    }
}
