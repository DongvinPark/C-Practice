#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int test_1(){
    char* my_env[] = {"FOOD=coffee", NULL};
    if( execle("./coffee", "./coffee", "donuts", NULL, my_env) == -1 ){
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

int test_2(){
    char* my_env[] = {"FOOD=donuts", NULL};
    if( execle("./coffee", "./coffee", "cream", NULL, my_env) == -1 ){
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

int test_3(){
    if( execl("./coffee", "coffee", NULL) == -1 ){
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

int test_4(){
    char* my_env[] = {"FOOD=donuts", NULL};
    if( execle("./coffee", "coffee", NULL, my_env) == -1 ){
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

/*
coffee-tester.c의 업그레이드 버전이다.
coffee-tester.c에서는 단일 프로세스에서 exec 류의 시스템 콜을 하다보니,
test_1()~test_4() 중에서 하나만 호출해도 나머지는 exec 류의 시스템 콜의 특성상
호출할 수가 없게 돼 버리는 문제가 있었다.

하나의 main() 함수에서 test_1()~test_4()를 전부 호출하기 위해서는, fork() 함수로
현재 프로세스를 복사한 후, 복사한 프로세스에서 test_1()~test_4()를 하나씩 맡아서 호출해야 한다.
그래야 복사된 프로세스가 test_n() 함수를 호출한 후 종료돼도 main()을 실행시키는 프로세스는
종료되지 않기 때문이다.
*/

int main(){
    int LOOP_CNT = 4;
    int completed = 0;

    // 함수 포인터의 배열을 만든다.
    int(*exec_arr[])() = {test_1, test_2, test_3, test_4};
 
    for(int i=0; i<LOOP_CNT; i++){
        pid_t pid = fork();
        if (pid == -1){
            fprintf(stderr, "cannot fork process: %s\n", strerror(errno));
            return -1;
        }
        if(!pid /*pid == 0과 같다.*/){
            int result = exec_arr[i](); // 여기서 exec_arr[i] 라고만 하면 함수가 호출이 되지 않는다!!
            if(result == 0){
                fprintf(stderr, "exec call failed: %s\n", strerror(errno));
            }
            completed++;
        }
    }

    /*
    이 부분은 프로그램 실행 결과를 좀더 깔끔하게 만들기 위해서 일부러 main() 프로세스의 종료를
    늦추는 기능을 한다.
    이 부분이 없을 경우, test_1()~test_4()를 실행하는 와중에 main()을 실행하는 프로세스가 먼저
    종료 돼버려서 실행 결과가 콘솔에 이쁘게 표시되지 않고,
    프로그램 자체도 종료 되지 않는 문제가 발생한다.
    */
    int cnt = 0;
    while(cnt < 1000000000){
        cnt ++;
    }
    
    return 0;
}