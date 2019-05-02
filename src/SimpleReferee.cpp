#include "SimpleReferee.h"
using namespace std;

//Initialize with agent vector
SimpleReferee::SimpleReferee(vector<AgentInterface*> agents): GameInterface(agents){

}

void SimpleReferee::turn(){
    //Dummy Interaction with child script
    for (AgentInterface* test_agent: agents){
        cerr << "Running child: " << test_agent->getCmd();
        kill(-(test_agent->getPid()), SIGCONT);
        waitpid(test_agent->getPid(), NULL, WCONTINUED);
        string response;
        *test_agent << msg;
        *test_agent >> response;
        kill(-(test_agent->getPid()), SIGSTOP);
        waitpid(test_agent->getPid(), NULL, WUNTRACED);
        cout << response << endl;
    }
}

void SimpleReferee::quit(){
    for (AgentInterface* test_agent: agents){
        cerr << "Terminating child: " << test_agent->getCmd();
        kill(-(test_agent->getPid()), SIGCONT);
        waitpid(test_agent->getPid(), NULL, WCONTINUED);
        string response;
        *test_agent << end;
        *test_agent >> response;
        cout << response << endl;
    }
}

//Referee Script
void SimpleReferee::run(){
    for(int i=0;i<10;++i) turn();
    quit();
}

SimpleReferee::~SimpleReferee(){

}
