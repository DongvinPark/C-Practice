#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

int score = 0;
int life_cnt = 3;

/*
곱셈 문제가 나올 때마다 5초 이내로 답을 맞춰야 하고,
틀릴 때마다 라이프 개수가 1개씩 깎여 나간다.

5초 이내로 답을 입력하지 못하거나, 라이프 개수가 0이 되면
게임오버 된다.
*/

void end_game(int sig){
    printf("\nFinal Score : %i\n", score);
    exit(0);
}

int catch_signal(int sig, void (*handler)(int)){
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction (sig, &action, NULL);
}

void times_up(int sig){
    puts("\nConsumed All Time!!");
    raise(SIGINT);
}

void error(char* msg){
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
}

int main(){
    catch_signal(SIGALRM, times_up);
    catch_signal(SIGINT, end_game);
    srandom (time(0)); // 프로그램 실행 때마다 난수가 달라지게 만들기 위한 코드

    while(1){
        int a = random() % 11;
        int b = random() % 11;
        char txt[4];
        alarm(5); // 매 while 루프마다 5초의 제한 시간을 적용한다.

        printf("\n %i X %i == ?? ", a, b);
        fgets(txt, 4, stdin);
        int answer = atoi(txt);
        if(answer == (a*b)){
            score++;
        } else {
            life_cnt--;
            printf("Wrong!! Current Score : %i, your life : %i \n", score, life_cnt);
            if(life_cnt == 0){
                printf("\nConsumed all of your life count. Game Over!!\n");
                end_game(SIGINT);
                break;
            }
        }
    }

    return 0;
}