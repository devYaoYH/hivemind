#pragma once
#include "includes.h"
using namespace std;

//Struct that bundles player information -> Optional meta-data read from config file (provided)
struct Player {
	Player();								//Default constructor
	~Player();								//Destructor
	map<string, string>* meta;				//Stores player meta-data (server id, submit time etc...)
	string name;							//Labels the player
};

ostream& operator<< (ostream& o, Player p);