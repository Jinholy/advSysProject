#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/queue.h>

//WriteMemoryCallback 함수에서 쓰일 구조체
struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    //더 큰 메모리가 필요할 때 동적할당 다시하는부분
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void parse(const char *url)  //returns file_name
{
    //파싱하는 함수
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;          //구조체 선언
    chunk.memory = (char*)malloc(1);    //구조체 초기화
    chunk.size = 0;                     //구조체 초기화
    curl = curl_easy_init();            //curl 초기화
    
    /*
    //이거는 나중에 thread 돌리면 output이 많이생길거니까 
    //출력파일명 output1 ~ outputn 으로 출력하기위해 선언해 둠．
    char buf[100];                          
    sprintf(buf,"output%d.txt",1);
    char *file_name = buf;                  
    */
    if (curl)
    {
        //curl_easy_setopt함수에 curl_easy_perform을 수행하기전에 설정하는 부분
        curl_easy_setopt(curl, CURLOPT_URL, url);                     //url 설정
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);           //http 3xx status를 처리하기 위해 쓰임． 예제에 있어서 그냥 놔둠
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); 
        //curl_easy_perform은 출력이 기본적으로 fwrite라 터미널에 출력되는데 
        //메모리에 read될 때 마다 사용자가 정의해둔callback 함수를(WritememoryCallback) 불러들여서 메모리 읽기 처리 
        
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);    //callback 함수가 불러들여 질때마다 정의해둔 chunk 구조체에 읽은 데이터를 write함．
        /* Perform the request, res will get the return code */
        
        res = curl_easy_perform(curl);                                //curl 수행

        /* Check for errors */
        if (res != CURLE_OK)                                        //오류처리
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
        {
            //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
            FILE *pFile;
            pFile = fopen("output1.txt","wb");
            fwrite(chunk.memory, 1, (unsigned long)chunk.size, pFile);      
            //위에 CURLOPT_WRITEFUNCTION, CURLOPT_WRITEDATA 옵션을 통해 chunk.memory에 쓰여진 데이터를 파일로 씀
            fclose(pFile);
        }
        /* always cleanup */
        curl_easy_cleanup(curl);    //이건 그냥 해줘야되는 듯 하다．
        free(chunk.memory);         //동적할당했기 때문에 free해줌
    }
    //return file_name;
}

int parse_url_tree(const char* url, int depth){
    parse(url);
}