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