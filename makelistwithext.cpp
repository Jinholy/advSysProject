#include <fstream>
#include <iostream>
#include <string>

using namespace std;
extern "C" void makeListwithExt(char* ext){
    ifstream in("hrefList.txt", ios::in | ios::binary);
    ofstream out("extList.txt", ios::out | ofstream::binary);

    if (!in)
    {
        cout << "File " << "hrefList.txt" << " not found!\n";
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(in, line))
    {
        size_t exists = line.find(ext);
        if (exists!=string::npos)
        { //simplely add to extList.txt if ext inlucded
            out << line << endl;
        }
    }

    in.close();
    out.close();
}
