#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int child_process_task() {
    char* my_env[] = {"FOOD=coffee", NULL};
    if (execle("./coffee", "./coffee", "donuts", NULL, my_env) == -1) {
        fprintf(stderr, "cannot execute process: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

/*
파이프를 만든 후, 자식 프로세스의 표준 출력을 파이프에 write하게 만들고,
부모 프로세스는 파이프에서 reading하게 만든다.
*/

int main(){

    int fd[2];
    if(pipe(fd) == -1){
        perror("cannot create pipe");
    }

    pid_t child_pid = fork();
    if(child_pid == -1 ){
        perror("cannot fork child process");
    }
    
    // 여기는 자식 프로세스가 하는 일이다.
    if(!child_pid){
        dup2(fd[1], 1);
        close(fd[0]);
        if(child_process_task()==-1){
            perror("cannot execute exe file");
        }
        // 자식 프로세스는 여기서 끝난다.
    }

    // 부모 프로세스는 이 부분을 실행한다.
    dup2(fd[0], 0);
    close(fd[1]);
    char line[255];
    while( fgets(line, 255, stdin) ){
        puts(line);
    }

    return 0;
}