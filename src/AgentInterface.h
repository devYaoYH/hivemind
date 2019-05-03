#pragma once
#include "includes.h"
#include "Parser.h"
using namespace std;

#ifndef AGENT_INTERFACE
#define AGENT_INTERFACE
//Pre-defined ERROR strings
const string TIMEOUT = "TIMEOUT";
const string ERROR = "ERROR";
#endif

//Abstract Class to extend into Human || Robot
class AgentInterface {
    friend AgentInterface& operator<<(AgentInterface& agent, string& to_write);
    friend AgentInterface& operator>>(AgentInterface& agent, string& to_read);
	public:
		AgentInterface();								//Defaults to a Robot Agent
		virtual ~AgentInterface();						//virtual destructor
		map<string, string>* getPlayerInfo();       	//Getter for player info
		bool isAuto();			        				//Getter for is_auto?
        
        //Process control methods
        bool running();                                 //Is child process still running?
        int getChldStat();                              //Get child process running status
        void sig_callback(int status, bool state);      //Reports child process from main
        void setPid(pid_t pid);                         //Sets child process pid
        pid_t getPid();                                 //Returns child pid

        void setCmd(string cmdline);                    //Copies child init process cmdline
        string getCmd();                                //Returns child proccess cmdline

        //IO Handles
        void attach_pipes(int fd_in, int fd_out);       //Sets the Agent's read/write IO interface.
        const int* getFd();                             //Retrieves FD handles

		//Parse Meta Data
		void load_meta(const string keySeq, const string fname);

		//Overriden Methods: (Differs for Human v.s. Robot)
		virtual void move(string& output);			    //Function that our main referee calls to get a move
		virtual void update(string& input);			    //Passes current game state information to Agent
	protected:
		map<string, string>* meta_data;					//Stores player meta data information in a struct
		bool is_auto;									//Marks whether is Robot
        bool is_running;                                //Whether child process is running
        bool is_stopped;                                //Whether child process is stopped (SIGSTOP sent)
        bool has_init;                                  //First round read/write IO sets time aside for process init
        int pipes[2];                                   //Stores the STDIN/STDOUT fd pair for piping input to/from child process
        int child_status;                               //Reports any child sig status
        pid_t child_pid;                                //Stores child pid num
        string cmdline;
};

//Writes to Agent attached to this interface
AgentInterface& operator<<(AgentInterface& agent, string& to_write);
//Reads from Agent attached to this interface
AgentInterface& operator>>(AgentInterface& agent, string& to_read);
