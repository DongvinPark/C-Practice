#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

/*
자식 프로세스들이 전부 실행 완료될 때까지 부모 프로세스가 대기하게끔
chat gpt에게 프로그래밍을 맡긴 결과다.
새로운 lib를 임포트 했고, wait() 함수를 사용하고 있다.
*/

int test_1() {
    char* my_env[] = {"FOOD=coffee", NULL};
    if (execle("./coffee", "./coffee", "donuts", NULL, my_env) == -1) {
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;  // This line will not be reached if execle is successful
}

int test_2() {
    char* my_env[] = {"FOOD=donuts", NULL};
    if (execle("./coffee", "./coffee", "cream", NULL, my_env) == -1) {
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;  // This line will not be reached if execle is successful
}

int test_3() {
    if (execl("./coffee", "coffee", NULL) == -1) {
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;  // This line will not be reached if execl is successful
}

int test_4() {
    char* my_env[] = {"FOOD=donuts", NULL};
    if (execle("./coffee", "coffee", NULL, my_env) == -1) {
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;  // This line will not be reached if execle is successful
}

int main() {
    int LOOP_CNT = 4;
    int status;

    // Array of function pointers
    int (*exec_arr[])() = {test_1, test_2, test_3, test_4};

    for (int i = 0; i < LOOP_CNT; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "cannot fork process: %s\n", strerror(errno));
            return -1;
        }
        if (pid == 0) {  // Child process
            int result = exec_arr[i]();
            _exit(result);  // Exit child process with the result
        }
    }

    // Parent process waits for all child processes to complete
    for (int i = 0; i < LOOP_CNT; i++) {
        wait(&status);
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                fprintf(stderr, "A child process exited with status %d\n", exit_status);
            }
        }
    }

    return 0;
}
