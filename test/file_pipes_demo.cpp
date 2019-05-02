#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string>
#include<sys/types.h>
#include<sys/wait.h>
#include<iostream>
#include<fstream>

#define MAX_ARGS 128
#define STDIN 0
#define STDOUT 1
#define STDERR 2

using namespace std;

char* arg[MAX_ARGS];

void spawn_process(char* exec_name, char* args[]){
    arg[0] = exec_name;
    int i = 1;
    for (;;i++){
        if (args == NULL) break;
        arg[i] = *args;
        args++;
    }
    arg[i] = NULL;
    execve(exec_name, arg, NULL);
}

int main(int argc, char* argv[]){
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    if (argc < 2){
        cout << "Need executable child file name" << endl;
        return 1;
    }
    pid_t pid;
    if ((pid = fork()) == 0){
        //Child Process

        //Connect stdin to fd1[0]
        dup2(fd1[0], STDIN);
        close(fd1[0]);
        close(fd1[1]);

        //Connect stdout to fd1[1]
        dup2(fd2[1], STDOUT);
        close(fd2[0]);
        close(fd2[1]);

        //Start process
        spawn_process(argv[1], NULL);
    }
    else{
        //Parent process
        ofstream fout("log.txt");

        //Connect stdout to stdin of child
        dup2(fd1[1], STDOUT);
        close(fd1[0]);
        close(fd1[1]);

        //Connect stdin to stdout of child
        dup2(fd2[0], STDIN);
        close(fd2[0]);
        close(fd2[1]);
        
        string response;
        for(int i=0;i<5;++i){
            cout << i << endl;
            getline(cin, response);
            fout << response << endl;
        }
        cout << "quit" << endl;
        getline(cin, response);
        fout << response << endl;
        fout.close();
        waitpid(pid, NULL, 0);
    }
    return 0;
}

