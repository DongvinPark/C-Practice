#include <stdio.h>

int main(){

    char *cards = "JQK";
    //char cards[] = "JQK";

    //cards[0] = 'A';

    printf("%c\n", *(cards + 1));

    return 0;

    /*
    짧은 프로그램이지만, C에서 문자열과 포인터 간에 어떤 관계를 가지고 있는지 파악할 수 있는 좋은 예제다.

    char *cards_pointer_ver = "JQK";
    char cards_arr_ver[] = "JQK";

    이 둘에 대해서 'read' 접근을 하는 것은 범위만 벗어나지 않으면 둘이 똑같이 작동한다.
    예를 들면 아래와 같다.

    cards_pointer_ver[1]
    cards_pointer_ver[1]
    *(cards_arr_ver + 1)
    *(cards_arr_ver + 1)

    이것들은 전부 똑같이 Q 라는 char를 반환한다.

    그러나, char *cards_pointer_ver 는 '문자열 상수'이기 때문에
    cards_pointer_ver[0] = 'A'; 같이 내용물을 바꾸는 코드는 segmentation fault 같은 fatal error를 발생시킨다.
    문자열의 내용물을 변경하고 싶다면, '상수'가 아니라
    char cards_arr_ver[] = "JQK"; 처럼 '배열'로 선언하고 초기화 해야 한다.
    
    */
}