#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

/*
knock-server.c 프로그램을 2024 기준 Chat Gpt 한테 리팩토링 시켜서
만들어낸 코드다.

Chat Gpt 가 설명하는 주요 개선점은 아래와 같다.

Key Changes and Improvements:
- Use of static for listener_d: Makes it clear that listener_d is limited to the current file.
- Enhanced error handling and reporting: Improved error messages and handling mechanisms.
- Modern function signatures: Updated function signatures and types for better readability and consistency.
- Consistent and clear comments: Improved comments for clarity and better understanding.
- Memory initialization: Using memset for zero-initializing structs to ensure all fields are set correctly.
- Use of ssize_t: For consistency in socket I/O operations.
- Structured flow with early exits: Simplifies code and improves readability.

Conclustion
This refactored version adheres to modern C programming practices and should be more robust and maintainable.
*/

static int listener_d;

// Catch signal and assign handler function
int catch_signal(int sig, void (*handler)(int)){
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

// Error handling function
void error(const char* msg){
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

// Open a listener socket
int open_listener_socket(){
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        error("Cannot open socket");
    }
    return s;
}

// Handle shutdown signal
void handle_shutdown(int sig){
    if (listener_d) {
        close(listener_d);
    }
    fprintf(stderr, "Goodbye!\n");
    exit(EXIT_SUCCESS);
}

// Send a message to the client
ssize_t say(int socket, const char* s){
    ssize_t result = send(socket, s, strlen(s), 0);
    if (result == -1) {
        fprintf(stderr, "Cannot send message to client: %s\n", strerror(errno));
    }
    return result;
}

// Read input from the client
ssize_t read_in(int socket, char* buf, size_t len){
    char* s = buf;
    size_t slen = len;
    ssize_t c;

    while ((c = recv(socket, s, slen, 0)) > 0 && s[c - 1] != '\n') {
        s += c;
        slen -= c;
    }

    if (c < 0) {
        return c;
    } else if (c == 0) {
        buf[0] = '\0';
    } else {
        s[c - 1] = '\0';
    }

    return (len - slen);
}

// Bind the socket to the port
void bind_to_port(int socket, int port){
    struct sockaddr_in name;
    memset(&name, 0, sizeof(name));
    name.sin_family = PF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    int reuse = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        error("Cannot set reuse option on socket");
    }
    if (bind(socket, (struct sockaddr*)&name, sizeof(name)) == -1) {
        error("Cannot bind to socket");
    }
}

int main(int argc, char* argv[]){
    if (catch_signal(SIGINT, handle_shutdown) == -1) {
        error("Cannot set interrupt handler");
    }

    listener_d = open_listener_socket();
    bind_to_port(listener_d, 30000);
    if (listen(listener_d, 10) == -1) {
        error("Cannot listen to requests");
    }

    struct sockaddr_storage client_addr;
    socklen_t address_size = sizeof(client_addr);
    puts("... waiting for request ...");

    char buf[255];
    while (true) {
        int connect_d = accept(listener_d, (struct sockaddr*)&client_addr, &address_size);
        if (connect_d == -1) {
            error("Cannot open secondary socket");
        }

        if (fork() == 0) { // Fork a child process
            // In the child process, close the listener socket as it is not used here
            close(listener_d);

            if (say(connect_d, "Internet Knock Knock Protocol Server\r\nversion 1.0\r\nKnock Knock\r\n>") != -1) {
                read_in(connect_d, buf, sizeof(buf));
                if (strncasecmp("Who's there?", buf, 12)) {
                    say(connect_d, "You have to ask first \"Who's there?\".\r\n");
                } else {
                    if (say(connect_d, "Doctor\r\n>") != -1) {
                        read_in(connect_d, buf, sizeof(buf));
                        if (strncasecmp("Doctor who?", buf, 10)) {
                            say(connect_d, "You have to ask \"Doctor who?\" next.\r\n");
                        } else {
                            say(connect_d, "Good question, nice :D\r\n");
                        }
                    }
                }
            }
            // Close the client socket in the child process
            close(connect_d);
            // Exit the child process
            exit(EXIT_SUCCESS);
        }
        // Close the client socket in the parent process
        close(connect_d);
    }

    return 0;
}
