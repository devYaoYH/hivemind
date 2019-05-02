#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int main(){
    ofstream fout("child.log");
    string query;
    while(query.compare("quit")){
        getline(cin, query);
        fout << "received: " << query << endl;
        cout << "echo: " << query << endl;
    }
    return 0;
}
