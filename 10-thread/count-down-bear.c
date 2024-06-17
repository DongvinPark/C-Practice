#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

/*
20개의 스레드가 200만 개의 맥주를 각자 10만 번씩 마셔서 없애는 프로그램이다.

뮤텍스 락을 어떻게 사용하느냐에 따라서 화면 출력 결과가 달라진다.

실행명령어는
아래와 같다.
gcc -o exe_file_name count-down-bear.c -lpthread  && ./exe_file_name && rm exe_file_name
*/

int bears = 2000000;
pthread_mutex_t bear_lock = PTHREAD_MUTEX_INITIALIZER;

void* subtract_many_times_drink_lots(void* a){
    pthread_mutex_lock(&bear_lock);
    for(int i=0; i< 100000; i++){
        bears -= 1;
    }
    pthread_mutex_unlock(&bear_lock);

    printf("number of left bears : %i\n", bears);
    return NULL;
}

void* subtract_one_time_drink_lots(void* a){  
    for(int i=0; i< 100000; i++){
        pthread_mutex_lock(&bear_lock);
        bears -= 1;
        pthread_mutex_unlock(&bear_lock);
    }

    printf("number of left bears : %i\n", bears);
    return NULL;
}

int main(){
    pthread_t threads[20];
    int t;
    printf("Number of total bears : %i \n\n", bears);

    for(t=0; t<20; t++){
        pthread_create(&threads[t], NULL, subtract_many_times_drink_lots, NULL);
    }

    void* result;
    for(t=0; t<20; t++){
        pthread_join(threads[t], &result);
    }

    printf("\n FINAL RESULT >> Number of left bears : %i \n", bears);
    return 0;
}