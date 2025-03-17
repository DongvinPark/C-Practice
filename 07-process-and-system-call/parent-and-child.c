#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int x = 100;  // Variable in memory

    pid_t pid = fork();  // Create child process

    if (pid == 0) {  // Child process
        printf("Child: x = %d (before modification)\n", x);
        x = 200;  // Modify x in child
        printf("Child: x = %d (after modification)\n", x);
    } else {  // Parent process
        sleep(1);  // Ensure child executes first (not necessary but helps clarity)
        printf("Parent: x = %d (after fork)\n", x);
    }

    return 0;
}