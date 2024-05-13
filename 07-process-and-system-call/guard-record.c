#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char* now(){
    time_t t;
    time (&t);
    return asctime(localtime (&t));
}

/*
이 프로그램을 실행하나 후, 'Test Message'를 콘솔에 입력한 다음 엔터를 누르면,
reports.log 라는 파일이 생성되고, 그 파일에

Test Message
 요일 월 일 시:분:초 연도

라는 문자열이 기록된다.

다시 실행해서 'Test Message2'를 입력하고 엔터를 누르면

Test Message2
 요일 월 일 시:분:초 연도

라는 문자열이 reports.log 라는 파일에 추가된다.

cmd라는 배열에 명령어를 기록한 후,
그 명령어를
system() 이라는 시스템 콜 API를 통해서 실행하는 것이다.
시스템 기능을 적절한 상황에 호출하면 코딩 양을 크게 줄일 수 있다.
*/

int main(){
    char comment[80];
    char cmd[120];

    fgets(comment, 80, stdin);
    sprintf(cmd, "echo '%s %s' >> reports.log", comment, now());
    system(cmd);
    return 0;
}