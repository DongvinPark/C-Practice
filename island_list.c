#include <stdio.h>

/*
C에서는 typedef struct {...} island 이런 식으로 쓸 경우 구조체의 이름을
생략할 수 있는 것이 맞다.
그런데, 아래의 경우와 같이
구조체 안에 자기 자신과 같은 타입의 포인터 변수를 저장해야 할 때는

typedef struct island {...} island
와 같이 struct와 { 사이에 island라는 구조체이름을 생략해서는 안 된다.
생략하게 될 경우,

island_list.c: In function ‘display’:
island_list.c:25:21: warning: initialization of ‘island *’ {aka ‘struct <anonymous> *’} from incompatible pointer type ‘struct island *’ [-Wincompatible-pointer-types]
   25 |         island* i = start;
      |                     ^~~~~
island_list.c:28:15: warning: assignment to ‘island *’ {aka ‘struct <anonymous> *’} from incompatible pointer type ‘struct island *’ [-Wincompatible-pointer-types]
   28 |             i = i->next;
      |               ^
island_list.c: In function ‘main’:
island_list.c:40:15: warning: assignment to ‘struct island *’ from incompatible pointer type ‘island *’ {aka ‘struct <anonymous> *’} [-Wincompatible-pointer-types]
   40 |     jeju.next = &dockdo;
      |               ^
island_list.c:41:17: warning: assignment to ‘struct island *’ from incompatible pointer type ‘island *’ {aka ‘struct <anonymous> *’} [-Wincompatible-pointer-types]
   41 |     dockdo.next = &ulleung;
      |                 ^
island_list.c:42:18: warning: assignment to ‘struct island *’ from incompatible pointer type ‘island *’ {aka ‘struct <anonymous> *’} [-Wincompatible-pointer-types]
   42 |     ulleung.next = &kanghwa;
      |                  ^
island_list.c:44:13: warning: passing argument 1 of ‘display’ from incompatible pointer type [-Wincompatible-pointer-types]
   44 |     display(&jeju);
      |             ^~~~~
      |             |
      |             island * {aka struct <anonymous> *}
island_list.c:21:29: note: expected ‘struct island *’ but argument is of type ‘island *’ {aka ‘struct <anonymous> *’}
   21 | void display(struct island* start){
      |              ~~~~~~~~~~~~~~~^~~~~
name: Jeju, open: 09:00, close: 17:00
name: dockdo, open: 09:00, close: 17:00
name: ulleung, open: 09:00, close: 17:00
name: kanghwa, open: 09:00, close: 17:00

프로그램이 작동하기는 하지만,
포인터 간 타입 불일치 경고 로그가 출력된다.
*/

typedef struct  {
    char* name;
    char* opens;
    char* closes;
    struct island* next;
} island;

void display(struct island* start){
    if(start == NULL){
        printf("Invalid Input: input is NULL");
    } else {
        island* i = start;
        while(1){
            printf("name: %s, open: %s, close: %s \n", i->name, i->opens, i->closes);
            i = i->next;
            if(i == NULL) break;
        }
    }
}

int main(){
    island jeju = {"Jeju", "09:00", "17:00", NULL};
    island dockdo = {"dockdo", "09:00", "17:00", NULL};
    island ulleung = {"ulleung", "09:00", "17:00", NULL};
    island kanghwa = {"kanghwa", "09:00", "17:00", NULL};

    jeju.next = &dockdo;
    dockdo.next = &ulleung;
    ulleung.next = &kanghwa;

    display(&jeju);

    return 0;
}