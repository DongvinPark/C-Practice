#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/*
리눅스 환경이라면, ifconfig 명령을 터미널에서 실행하고,
맥, 윈도우 환경에서는 ipconfig 명령을 실행하는 것과 같다.
*/

int main(){
    if( execl("/sbin/ifconfig", "/sbin/ifconfig", NULL) == -1 ){
        if( execlp("ipconfig", "ipconfig", NULL) == -1 ){
            fprintf(stderr, "cannot execute ipconfig: %s", strerror(errno));
            return 1;
        }
    }
    return 0;
}