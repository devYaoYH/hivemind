#pragma once
#include "includes.h"
#include "Player.h"
#include "Parser.h"
using namespace std;

//Abstract Class to extend into Human || Robot
class AgentInterface {
	public:
		AgentInterface();							//Defaults to a Robot Agent
		virtual ~AgentInterface();					//virtual destructor
		virtual map<string, string>* getPlayer();	//Getter for player info
		virtual bool isAuto();						//Getter for is_auto?

		//Parse Meta Data
		virtual void load_meta(const string keySeq, const string fname);

		//Overriden Methods: (Differs for Human v.s. Robot)
		virtual string move() = 0;					//Function that our main referee calls to get a move
		virtual void update() = 0;					//Passes current game state information to Agent
	protected:
		map<string, string>* meta_data;				//Stores player meta data information in a struct
		bool is_auto;								//Marks whether is Robot
};