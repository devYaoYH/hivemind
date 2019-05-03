#include<iostream>
#include<chrono>
#include<cstdlib>
#include<fstream>
#include<string>
using namespace std;

#define BASE 10

int main(int argc, char* argv[]){
    int millis_delay = 50;
    if (argc > 1){
        char* tmp;
        millis_delay = strtol(argv[1], &tmp, BASE);
    }
    if (millis_delay == 1337){
        int* ptr = nullptr;
        *ptr = 0;
    }
    string query;
    while(query.compare("quit")){
        getline(cin, query);
        //Pause for specified ms before printing
        auto start = chrono::steady_clock::now();
        auto end = chrono::steady_clock::now();
        while(chrono::duration_cast<chrono::milliseconds>(end - start).count() < millis_delay){
            end = chrono::steady_clock::now();
        }
        //cerr << "delay: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
        cerr << "received: " << query << endl;
        cout << "echo: " << query << endl;
    }
    return 0;
}
