#include<stdio.h>
#include<sys/inotify.h>
#include<stdlib.h>
#include<unistd.h>

#define BUF_LEN 4048
#define INOTIFY_PATH "."
#define LIST_SIZE 100

void refresh_file_list();
bool check_list(const char* str);
void extract_file(const char* file_name);

int inotifyFd, wd, j;
char buf[BUF_LEN] __attribute__((aligned(8)));
ssize_t numRead;
char *p;
struct inotify_event *event;
const char* file_list[LIST_SIZE];
int file_cnt = 0;
int  checked_file_cnt = 0;

static void displayInotifyEvent(struct inotify_event *i)
{
    //printf("    wd =%2d; ", i->wd);
    if (i->cookie > 0)
        printf("cookie =%4d; ", i->cookie);

    //printf("mask = ");
    if (i->mask & IN_ACCESS)        printf("IN_ACCESS ");
    if (i->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
    if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
    if (i->mask & IN_CLOSE_WRITE){
        refresh_file_list();
        if(check_list(i->name)){
            printf("download finished! %s\n",i->name);
            checked_file_cnt++;
            printf("Do you want to extract files %s?(y/n)\n", i->name);
            char wanna_ext[10];
            scanf("%s", wanna_ext);

            if (!strcmp(wanna_ext, "n") || !strcmp(wanna_ext, "N"))     //압축푸는걸 원치않으면 스레드 종료
                exit(EXIT_SUCCESS);
            else
                extract_file(i->name);
            
            if(checked_file_cnt == file_cnt)        //체크한 파일과 총 파일 갯수가 동일하면 스레드 종료
                exit(EXIT_SUCCESS);
        }
    }
    if (i->mask & IN_CREATE)        printf("IN_CREATE ");
    if (i->mask & IN_DELETE)        printf("IN_DELETE ");
    if (i->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
    if (i->mask & IN_IGNORED)       printf("IN_IGNORED ");
    if (i->mask & IN_ISDIR)         printf("IN_ISDIR ");
    if (i->mask & IN_MODIFY)        printf("IN_MODIFY ");
    if (i->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
    if (i->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
    if (i->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
    if (i->mask & IN_OPEN)          printf("IN_OPEN ");
    if (i->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
    if (i->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
    //printf("\n");

    //if (i->len > 0)
    //    printf("        name = %s\n", i->name);
}

int my_init_inotifier(){

    inotifyFd = inotify_init();

    if(inotifyFd == -1){
        printf("init error");
        exit(EXIT_FAILURE);
    }

    //wd = inotify_add_watch(inotifyFd, ".", IN_ALL_EVENTS);
    wd = inotify_add_watch(inotifyFd, ".", IN_CLOSE_WRITE);
    
    if(wd == -1){
        printf("inotify add watch error");
        exit(EXIT_FAILURE);
    }
    
    //printf("Watching %s using wd %d \n", ".", wd);
}

static void* threadFunc(void *arg){     //we are gonna put inotify here
    bool *wanna_download = (bool*) arg;
    //printf("%s", s);
    //return (void *) strlen(s);
    if(!wanna_download){
        exit(EXIT_FAILURE);
    }
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
        //printf("Read %ld bytes from inotify fd\n", (long)numRead);

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead;)
        {
            event = (struct inotify_event *)p;
            displayInotifyEvent(event);
            
            p += sizeof(struct inotify_event) + event->len;
        }
    }
}

void refresh_file_list(){
    char buffer[BUF_LEN];
    FILE *fp = fopen("extList.txt", "r");
    file_cnt = 0;
    while(fgets(buffer, sizeof(buffer), fp)){
        buffer[strlen(buffer) -1] = '\0';       //remove \n
        file_list[file_cnt++] = buffer;
    }
    fclose(fp);
}

bool check_list(const char* str){
    for(int i =0; i< file_cnt; i++){
        //printf("%s %s\n", file_list[i], str);
        if(!strcmp(file_list[i],str)){ //if same return 0
            return true;
        }
    }
    return false;
}

void extract_file(const char* file_name){
    char command[BUF_LEN];
    char *ext_dir;
    char *ext_type;
    char *ptr = strtok((char*)file_name, ".");
    
    ext_dir = ptr;
    ext_type = strtok(NULL, ".");
    
    printf("file type: %s\n", ext_type);
    if(strcmp(ext_type,"zip")){
        printf("sorry we can extract only .zip file\n");
        exit(EXIT_FAILURE);
    }
    
    sprintf(command, "unzip %s -d ./%s", file_name, ext_dir);
    system(command);
}