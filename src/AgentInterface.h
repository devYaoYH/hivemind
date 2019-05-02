#pragma once
#include "includes.h"
#include "Parser.h"
using namespace std;

//Abstract Class to extend into Human || Robot
class AgentInterface {
    friend AgentInterface& operator<<(AgentInterface& agent, string& to_write);
    friend AgentInterface& operator>>(AgentInterface& agent, string& to_read);
	public:
		AgentInterface();								//Defaults to a Robot Agent
		virtual ~AgentInterface();						//virtual destructor
		map<string, string>* getPlayerInfo();       	//Getter for player info
		bool isAuto();			        				//Getter for is_auto?

        //IO Handles
        void attach_pipes(int fd_in, int fd_out);       //Sets the Agent's read/write IO interface.

		//Parse Meta Data
		void load_meta(const string keySeq, const string fname);

		//Overriden Methods: (Differs for Human v.s. Robot)
		virtual void move(string& output);			    //Function that our main referee calls to get a move
		virtual void update(string& input);			    //Passes current game state information to Agent
	protected:
		map<string, string>* meta_data;					//Stores player meta data information in a struct
		bool is_auto;									//Marks whether is Robot
        int pipes[2];                                   //Stores the STDIN/STDOUT fd pair for piping input to/from child process
};

//Writes to Agent attached to this interface
AgentInterface& operator<<(AgentInterface& agent, string& to_write);
//Reads from Agent attached to this interface
AgentInterface& operator>>(AgentInterface& agent, string& to_read);
