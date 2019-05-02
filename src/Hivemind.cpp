// Hivemind.cpp : Defines the entry point for the console application.

#include "includes.h"
//#include "AgentBuilder.h"
#include "AgentInterface.h"

char* arg[MAX_ARGS];
//char test_prog[] = "timed_child";
char test_prog[] = "/usr/bin/python";
char py_script[] = "timed_py.py";
char* test_args[] = {py_script};

void spawn_process(char* exec_name, char* args[]){
    arg[0] = exec_name;
    int i = 1;
    if (args != NULL){
        for (;;i++){
            if (*args == '\0') break;
            arg[i] = *args;
            args++;
        }
    }
    arg[i] = NULL;
    execve(exec_name, arg, NULL);
}

int main(int argc, char* argv[])
{
    //AgentBuilder builder = AgentBuilder();
    int fd_child_in[2], fd_child_out[2];
    pipe(fd_child_in);
    pipe(fd_child_out);
    AgentInterface test_agent = AgentInterface();
    test_agent.attach_pipes(fd_child_out[0], fd_child_in[1]);
    
    pid_t pid;
    if ((pid=fork()) == 0){
        //Child Process
        dup2(fd_child_in[0], STDIN);
        close(fd_child_in[0]); close(fd_child_in[1]);
        dup2(fd_child_out[1], STDOUT);
        close(fd_child_out[0]); close(fd_child_out[1]);
        
        //spawn_process(test_prog, test_args);
        //spawn_process(test_prog, NULL);
        spawn_process(argv[1], argv+2);
    }
    else{
        //Main Process
        
        //Close Child-end of pipes
        close(fd_child_in[0]);
        close(fd_child_out[1]);
        
        //Test Write to child
        string test_msg = "hey there\n";
        string end = "quit\n";
        string msg = "hello world\n";
        
        //Experiment with write()
        cout << "Write to fd: " << fd_child_in[1] << endl;
        //cout << "Write: " << write(fd_child_in[1], test_msg.c_str(), test_msg.length()) << endl;
        //cout << "Write: " << write(fd_child_in[1], end.c_str(), end.length()) << endl;

        string response;
        
        for(int i=0;i<10;++i){
            test_agent << msg;
            test_agent >> response;
            cout << response << endl;
        }
        
        test_agent << end;
        test_agent >> response;
        cout << response << endl;
       
        int child_status;
        waitpid(pid, &child_status, 0);
        cout << "Child Reports: " << child_status << endl;
    }
    return 0;
}
