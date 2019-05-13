#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

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

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

char* parse(char *url, int file_count)  //returns file_name
{
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;
    curl = curl_easy_init();
    char buf[100];
    sprintf(buf,"output%d.txt",file_count);
    char *file_name = buf;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
        {
            printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
            FILE *pFile;
            pFile = fopen(file_name,"wb");
            fwrite(chunk.memory, 1, (unsigned long)chunk.size, pFile);
            fclose(pFile);
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        free(chunk.memory);
    }
    return file_name;
}