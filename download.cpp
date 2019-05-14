#include <fstream>
#include <iostream>
#include <string>

using namespace std;
//cpp함수를　c에서　사용하기위해　extern "C"　를　함수앞에　붙여준다．
extern "C" void download(const char* url){              
    //url을　전달받아서　단순히 wget url filename　이런식으로　만들어서　wget　명령어를　system　함수를　통해　실행하는　함수
    ifstream in("extList.txt", ios::in | ios::binary);              //원하는　ext만　뽑아　낸　extList.txt를　읽어들인다．
    if (!in)        //에러처리．
    {
        cout << "File " << "extList.txt" << " not found!\n";
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(in, line))       //파일에서　한줄씩　읽어들인다．
    {
        //c++에서　string과　const char*를　이어　붙이기　위한　험난한　여정
        string c_url = url;                 
        string tmp = "wget "+ c_url +line;          //wget + 실행할　때　파라메터로　넘겨준　url주소 + extList에서　읽어들인　파일이름
        const char *cmd = tmp.c_str();              //string을　c_str()로　const char*로　바꿔줌．
        system(cmd);                                //그렇게　완성된　wget 주소＋파일명　으로　wget명령어　수행．
    }

    in.close();
}
