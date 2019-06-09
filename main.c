/*
프로그램 실행되는 순서
main.c
parseurl.c          의 parse()                 (input: url, thread_count) -> (output: output1.txt)
gumbo.cpp           의 getHref()               (input: filename)          -> (output: hrefList.txt)
makelistwithext.cpp 의 makeListwithExt()       (input : hrefList.txt)     -> (output: extList.txt)
download.cpp        의 download()              (input : extList.txt)      -> (output: wget 명령어를 통해 다운로드된 파일)
순으로 진행됨．

지금상태 요약(마지막에 개선사항)：
1.thread미구현
2.href태그만 뽑아낸다．(image 등은 안됨)
3.root에 대한것만 처리(재귀적으로 실행되지않음) 
지금 해놓은게 gumboparser가 네트워크에서 바로바로 재귀적으로 하는게 아니고 파일로 읽어들여서 수행하는거라 
parseurl.c가 에서 파싱한 페이지가 재귀적으로 읽어들여서 하지 않아 그런게 가장 유력함．
4.thread미구현의 연장선인데 wget 명령어가 하나씩 수행되서 효율떨어짐． 당장에는 wget명령어를 background에서 수행하게 옵션 집어넣으면 될듯하다．
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parseurl.c"
#include "gumbo.cpp"
#include "makelistwithext.cpp"
#include "download.cpp"

#define BUFF_SIZE 1024

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("usage : ./main ext_name url\n");
        printf("ex)  ./main .zip https://iscxdownloads.cs.unb.ca/iscxdownloads/CICAndMal2017/CSVs/");
        exit(EXIT_FAILURE);
    }
    
    char cmd[BUFF_SIZE] = "wget ";
    char *url = argv[2];

    char* filename = parse(url,1);      //parse(url, thread_count) thread has not been implemented yet  // output: output1.txt
    getHref(filename);                  //get href list      input : filename, output : hrefList.txt
    makeListwithExt(argv[1]);           //make list with extention that want to download
    download(url);                      //start download with wget

    exit(EXIT_SUCCESS);
}