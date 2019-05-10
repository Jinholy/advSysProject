#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>



int main(int argc, char *argv[]){
    if(argc != 3){
        printf("argument error\n");
        printf("usage : ./main thread_number url\n");
        printf("ex) ./main 3 naver.com\n");
        exit(EXIT_FAILURE);
    }

    
    exit(EXIT_SUCCESS);
}