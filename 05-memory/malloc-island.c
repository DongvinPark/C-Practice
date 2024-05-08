#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
이 프로그램을 실행시키기 위한 명령어는 아래와 같다.
gcc -o exe_file_name malloc-island.c && ./exe_file_name < trip.txt && rm exe_file_name
*/

/*
자기 자신에 대한 포인터를 멤버로서 가지고 있는 구조체는 typedef struct island {...} island
와 같이 구조체 이름을 전부 표기해줘야 한다.
*/
typedef struct island {
    char* name;
    char* opens;
    char* closes;
    struct island* next;
} island;

struct island* create_island(char* name) {
    island* i = malloc(sizeof(island));
    // 문자열을 힙 공간에 deep copy 한 후 시작점 포인터를 반환해주는 strdup를 써줘야
    // 나중에 연결리스트 만들었을 때 노드들의 name 필드들이 정확하게 표시된다.
    // 그냥 char* name 으로 넣어버리면 모든 노드들의 name 필드가 마지막 노드의 name
    // 필드를 정의할 때 사용한 문자열로 통일돼 버리고 만다.
    // 전부 동일한 문자열을 참조하면서 shallow copy가 되기 때문이다.
    i->name = strdup(name);
    i->opens = "09:00";
    i->closes = "17:00";
    i->next = NULL;
    return i;
}

void release(island* start){

    /*
    연결리스트는 리스트 조작에 필요한 hook들을 루프 시작 전에 먼저 만들어 놓는 것이 핵심이다.
    */
    island* i = start;
    island* next_island = NULL;
    while (1) {
        next_island = i->next;

        // 구조체 내부의 멤버를 먼저 해제해준 다음, 구조체를 해제해줘야 한다.
        // 바깥 구조체를 먼저 해제해버리면 그 구조체 내부 멤버에 접근할 수 없게 되기 때문이다.
        free(i->name);
        free(i);
        i = next_island;
        if(next_island == NULL) break;
    }
    
}

void display(struct island* start){
    if(start == NULL){
        printf("Invalid Input: input is NULL");
    } else {
        island* i = start;
        while(1){
            printf("name: %sopen: %s, close: %s \n\n", i->name, i->opens, i->closes);
            i = i->next;
            if(i == NULL) break;
        }
    }
}

int main(){

    island* start = NULL;
    island* i = NULL;
    island* next_island = NULL;
    char name[80];

    printf("\n");
    while(1){
        if(fgets(name, 80, stdin)==NULL) break;

        // 일단 '현재 노드의 다음 섬'을 무조건 먼저 만들어준다.
        next_island = create_island(name);

        // 이번이 첫 루프라면, 첫 노드를 start 라는 hook에 등록해줘야 한다.
        // 그래야 다음에 등장하는 display()와 release() 같은 함수들을 호출할 때 사용할 수 있다.
        if(start==NULL) {
            start = next_island;
        }

        // 두 번째 노드부터는 현재 노드인 i의 next 필드에다가 '다음 섬'을 등록해줘야 한다.
        if(i != NULL){
            i->next = next_island;
        }
        
        // 다음 루프를 위한 것이다.
        // 다음 루프에서 i는 '현재 노드'를 가리키는 역할을 한다.
        // 이번 루프에서 만든 '다음 노드'는 다음 루프 입장에서는 '현재 노드'가 돼야 하기 때문이다.
        i = next_island;
    }

    /*
    위의 while 루프를 for 문으로 작성해보면 아래와 같다.
    개인적으로 for 보다는 while이 루프 내의 동작을 더 명확하게 표현할 수 있어서
    while 더 선호한다.
    */
    // for( ; fgets(name, 80, stdin) != NULL; i = next_island){
    //     next_island = create_island(name);
    //     if(start == NULL){
    //         start = next_island;
    //     }
    //     if(i != NULL){
    //         i->next = next_island;
    //     }
    // }

    display(start);
    release(start);
    return 0;
}