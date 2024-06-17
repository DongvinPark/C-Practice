#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

/*
pthread 라이브러리를 활용하여 두 개의 스레드를 만들고 실행하는 프로그램이다.

실행 명령어를 
gcc -o exe_file_name create-pthread.c -lpthread  && ./exe_file_name && rm exe_file_name
라고 해줘야 한다. 특히, -lpthread 부분에서 pthread 라이브러리를 링크해주는 부분이 중요하다.

*/

void error(char* msg){
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

void* does_not(void* a){
    int i = 0;
    for(i=0; i<5; i++){
        sleep(1);
        puts("Does not!");
    }
    return NULL;
}

void* does_too(void* a){
    int i = 0;
    for(i=0; i<5; i++){
        sleep(1);
        puts("Does too!");
    }
    return NULL;
}

int main(){
    pthread_t t0;
    pthread_t t1;
    if ( pthread_create(&t0, NULL, does_not, NULL) == -1 ){
        error("Cannot create t0 thread");
    }
    if ( pthread_create(&t1, NULL, does_too, NULL) == -1 ){
        error("Cannot create t1 thread");
    }

    void* result;
    if ( pthread_join(t0, &result) == -1 ){
        error("Cannot terminate thread t0");
    }
    if ( pthread_join(t1, &result) == -1 ){
        error("Cannot terminate thread t1");
    }
}