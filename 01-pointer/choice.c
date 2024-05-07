#include <stdio.h>

int main(){

    int contestants[] = {0,1,2};

    int* choice = contestants;

    /*
    C에서 배열 변수는 그 배열의 0 인덱스의 주소값으로 대체 가능하다는 점을 알고 있을 것이다.

    그런데 놀라운 점은, 
    int* choice 라고 쓰나,
    int * choice 라고 쓰나,
    int *choice 라고 쓰나

    프로그램은 정상적으로 동작했다는 것이다.

    그러니까 이제부터는
    포인터 변수의 선언에 한정해서는
    int *choice나 int * choice 같은게 나와도 전부
    int* choice와 동일하다는 것을 기억하자.
    */

    char string[4] = "wow";

    contestants[0] = 2;
    contestants[1] = contestants[2];
    contestants[2] = *choice;

    printf("나는 참가자 %i 번을 선택할거야. \n", contestants[2]);

    return 0;
}