#include <fstream>
#include <iostream>
#include <string>

using namespace std;
extern "C" void download(const char* url){
    ifstream in("extList.txt", ios::in | ios::binary);
    if (!in)
    {
        cout << "File " << "extList.txt" << " not found!\n";
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(in, line))
    {
        string c_url = url;
        string tmp = "wget "+ c_url +line;
        const char *cmd = tmp.c_str();
        system(cmd);
    }

    in.close();
}
