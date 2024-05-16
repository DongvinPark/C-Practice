#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/*
이 프로그램을 실행시키기 위해서는, coffee.c를 gcc 컴파일만 해서
coffee 라는 실행파일을 만들어줘야 한다.
이는 execle()과 execl() 함수의 특성 때문인데 자세한 내용은 main() 함수 내에서 설명한다.
*/

int test_1(){
    char* my_env[] = {"FOOD=coffee", NULL};
    if( execle("./coffee", "./coffee", "donuts", NULL, my_env) == -1 ){
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
}

int test_2(){
    char* my_env[] = {"FOOD=donuts", NULL};
    if( execle("./coffee", "./coffee", "cream", NULL, my_env) == -1 ){
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
}

int test_3(){
    if( execl("./coffee", "coffee", NULL) == -1 ){
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
}

int test_4(){
    char* my_env[] = {"FOOD=donuts", NULL};
    if( execle("./coffee", "coffee", NULL, my_env) == -1 ){
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
}

int main(){

    /*
    execl()과 execle()은 모두 현재 프로세스를 다른 프로세스 이미지로 대체하는 함수다.
    여기서 말하는 '프로세스 이미지'란, 결국 coffee.c를 컴파일 해서 만든 coffee라는 
    실행파일을 뜻한다.

    따라서, test_1() ~ test_4() 함수들 중 하나만 골라서 실행시켜야 한다.

    execl()과 execle()은 모두 현재의 프로세스를 새로운 프로세스 이미지로 대체할 때
    사용한다는 공통점이 있지만,
    전자는 현재의 환경을 그대로 유지하면서 동작시키고,
    후자는 원하는 새로운 환경을 정의해서 동작시킨다는 차이점이 있다.
    */

    //test_1();
    test_2();
    //test_3();
    //test_4();

    return 0;
}