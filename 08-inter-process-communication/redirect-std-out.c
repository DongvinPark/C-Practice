#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

/*
자식 프로세스들의 표준 출력을 파일스트림으로 보내도록 수정한 프로그램이다.
이 프로그램을 실행하면, orders.txt 라는 파일이 민들어지며 그 안에는
자식 프로세스들이 출력한 텍스트들이 저장된다.
*/

int test_1() {
    char* my_env[] = {"FOOD=coffee", NULL};
    if (execle("./coffee", "./coffee", "donuts", NULL, my_env) == -1) {
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

int test_2() {
    char* my_env[] = {"FOOD=donuts", NULL};
    if (execle("./coffee", "./coffee", "cream", NULL, my_env) == -1) {
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

int test_3() {
    if (execl("./coffee", "coffee", NULL) == -1) {
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

int test_4() {
    char* my_env[] = {"FOOD=donuts", NULL};
    if (execle("./coffee", "coffee", NULL, my_env) == -1) {
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

int main() {
    int LOOP_CNT = 4;
    int status;

    FILE* f = fopen("orders.txt", "w");
    if(!f /*이건 f==0과 같다. C에서 0은 false의 역할도 한다.*/){
        perror("cannot open file stream orders.txt");
    }

    int (*exec_arr[])() = {test_1, test_2, test_3, test_4};

    // 자식프로세스 4개를 만들어서 각각 test_1 ~ test_4를 실행하게 만든다.
    for (int i = 0; i < LOOP_CNT; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "cannot fork process: %s\n", strerror(errno));
            return -1;
        }
        if (pid == 0 /*이건 !pid 와 같다.*/ ) {
            // 이 부분에서 표준 출력을 파일 스트림으로 리다이렉트한다.
            if( dup2(fileno(f),1) == -1 ){
                perror("cannot redirect std out");
            }
            int result = exec_arr[i]();
            _exit(result);
        }
    }

    for (int i = 0; i < LOOP_CNT; i++) {
        wait(&status);
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                fprintf(stderr, "A child process exited with status %d\n", exit_status);
            }
        }
    }

    fclose(f);
    return 0;
}