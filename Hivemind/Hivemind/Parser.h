#pragma once
#include "includes.h"
#include "ConfigParser.h"
#include "JsonParser.h"
using namespace std;

/*
 * Abstract Util Wrapper to parser configuration files
 *	->	Written as a Builder so we can switch out the format of
 *		configuration files as we go along without much impact
 *		to the rest of the program
 *  ->	Forwards method requests to the implemented parser object
 *  ->	We enforce Singleton Pattern to our implemented parsers as well
 *		save resources, don't need more than 1 parser
 */

class Parser {
	public:
		Parser() {
			conut++;
		}
		~Parser() {
			if (conut == 0) {
				delete parser;
			}
			conut--;
		}
		// Returns the innermost Map of attributes associated by walking down provided key sqeuence
		// Takes in a sequence of ' ' space separated keys
		void getAttrs(map<string, string>& kvmap, vector<string>& expanded_objs, const string keySeq, const string fname) {
			getParser()->getAttrs(kvmap, expanded_objs, keySeq, fname);
		}
	private:
		ConfigParser* getParser() {
			if (parser == 0) {
				return new JsonParser();
			}
			else return parser;
		}
		static ConfigParser* parser;
		static int conut;
};