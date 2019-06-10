/*
프로그램 실행되는 순서
main.c
parseurl.c          의 parse()                 (input: url) -> (output: output1.txt)
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
zip 파일 하나있는 링크 https://iscxdownloads.cs.unb.ca/iscxdownloads/NSL-KDD/
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parseurl.c"
#include "gumbo.cpp"
#include "makelistwithext.cpp"
#include "download.cpp"
#include <pthread.h>
#include "inotify.c"

#define BUFF_SIZE 1024

static void* threadFunc(void *arg);

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("usage : ./main ext_name url\n");
        printf("ex)  ./main .zip https://iscxdownloads.cs.unb.ca/iscxdownloads/CICAndMal2017/CSVs/");
        exit(EXIT_FAILURE);
    }
    
    char cmd[BUFF_SIZE] = "wget ";
    int depth = 1;
    char *ext = argv[1];
    const char *url = argv[2];
    int s;
    pthread_t t1;
    void *res;
    const char *filename = "output1.txt";


    //set and run thread
    //s = pthread_create(&t1, NULL, threadFunc, (void*)url);
    //s = pthread_join(t1, &res);
    printf("**finding files...\n -ext  : %s \n -url  : %s\n", ext, url);


    //start parsing and download files
    parse_url_tree(url, depth);         //parse(url) // output: output1.txt
    getHref(filename);                  //get href list      input : filename, output : hrefList.txt
    makeListwithExt(ext);               //make list with extention that want to download
    download(url, ext);                 //start download with wget

    printf("thread returned %ld\n", (long) res);
    exit(EXIT_SUCCESS);
}


static void* threadFunc(void *arg){     //we are gonna put inotify here
    //har *s = (char*) arg;
    //printf("%s", s);
    //return (void *) strlen(s);

    my_init_inotifier();
    while(1)
    { /* Read events forever */
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0){
            printf("read() from inotify fd returned 0!");
            exit(EXIT_FAILURE);
        }
            
        if (numRead == -1){
            printf("readError");
            exit(EXIT_FAILURE);
        }
        printf("Read %ld bytes from inotify fd\n", (long)numRead);

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead;)
        {
            event = (struct inotify_event *)p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }
}