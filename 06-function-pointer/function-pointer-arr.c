#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum response_type {
    DUMP, SECOND_CHANCE, MARRIAGE, LAW_SUIT
};

typedef struct {
    char* name;
    enum response_type type;
} response;

void dump(response r){
    printf("to %s,\n", r.name);
    puts("Game over. Fine someone else.\n");
}

void second_chance(response r){
    printf("to %s,\n", r.name);
    puts("You have one more last chance.\n");
}

void marriage(response r){
    printf("to %s,\n", r.name);
    puts("You got what you want.\n");
}

void law_suit(response r){
    printf("to %s, \n", r.name);
    puts("Employ lawyer!!");
}

int main(){

    response r[] = {
        {"Mike", DUMP}, {"Louise", SECOND_CHANCE},
        {"Matt", SECOND_CHANCE}, {"William", MARRIAGE},
        {"Kim", LAW_SUIT}
    };

    /*
    response_type 별로 서로 다른 종류의 응답을 콘솔에 출력하는 프로그램이다.
    아래와 같이 for, switch 문을 써서 작성하면 물론 잘 작동한다.
    하지만, response_type에 새로운 값이 추가되면서 새로운 응답출력 함수가 추가 된다면
    switch 문도 같이 바꿔줘야 된다는 문제점이 있다.

    함수 포인터 배열을 만들면 이러한 문제를 해결하면서 코드 양도 줄일 수 있다.

    단, 아래의 코드에서는 주의할 점이 있는데, 
    enum에서 정의된 응답타입의 순서와
    함수 포인터 배열에서 정의한 응답타입별 함수이름의 순서가 동일해야 한다는 것이다.
    만약 함수포인터의 배열이 enum과 다른 순서로 정의돼 있다면,
    전혀 엉뚱한 함수가 호출되면서 결과가 틀리게 출력된다.

    이러한 문제를 피하고 싶다면, 코드가 좀 길어지더라도 그냥 
    for문과 switch 문을 쓰는 것이 최선이기는 하다.
    */

   void(*replies[])(response) = {dump, second_chance, marriage, law_suit};
    
    for(int i=0; i<5; i++){
        replies[r[i].type](r[i]);
    }

    // for(int i=0; i<4; i++){
    //     switch (r[i].type){
    //         case DUMP: {
    //             dump(r[i]);
    //             break;
    //         }
    //         case SECOND_CHANCE: {
    //             second_chance(r[i]);
    //             break;
    //         }
    //         case MARRIAGE: {
    //             marriage(r[i]);
    //             break;
    //         }
    //     }
    // }

    return 0;
}