#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void signal_process(int sig){
    puts("Good Bye..");
    exit(1);
}

// 함수 포인터를 인자로 받고 있음을 알 수 있다.
int catch_signal(int sig, void (*handler)(int)){
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction (sig, &action, NULL);
}

/*
signal을 이용해서 프로세스를 종료할 때 특정한 동작을 하게 만드는 에시이다.

프로그램을 실행한 후, 이름을 입력하면 프로그램이 정상 종료 되지만,
ctrl + C 를 누르면 Good Bye .. 를 출력한 후 프로그램이 종료된다.
*/
int main(){
    if( catch_signal(SIGINT, signal_process) == -1 ){
        fprintf(stderr, "cannot map signal processor");
        exit(2);
    }

    char name[30];
    printf("Enter name: ");
    fgets(name, 30, stdin);
    printf("Hi %s!!\n", name);
    return 0;
}