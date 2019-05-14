#include <fstream>
#include <iostream>
#include <string>

using namespace std;
//C++ 함수를　c에서　사용하려면　함수　앞에　extern "C"를　붙여주면된다．　
extern "C" void makeListwithExt(char* ext){
    ifstream in("hrefList.txt", ios::in | ios::binary);             //href　태그가　붙은거를　모아둔　txt파일을　읽음
    ofstream out("extList.txt", ios::out | ofstream::binary);       //원하는　확장자만　모아둔　txt에　쓸것．

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
        //문자열　안에　ext (예를들면　.zip)이　존재하는지　확인．　find("str")이
        //원래　어디에　있는지　위치를　알려주는　함수인데　단순히　bool로　정의해서　쓰면　무조건　true나와서　이런식으로　사용
        { //simplely add to extList.txt if ext inlucded
            out << line << endl;                //원하는　ext가　문자열에　있으면　파일에　써준다．
        }
    }

    in.close();
    out.close();
}
