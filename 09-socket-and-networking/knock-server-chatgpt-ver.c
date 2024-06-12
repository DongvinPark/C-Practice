#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

/*
knock-serer.c 를 Chat GPT한테 2024년에 맞게 modern C standard를 반영하여
리팩토링 하라고 시켰을 때의 결과다.
아래는 무엇이 리팩토링 됐는지에 대한 GPT의 설명이다.

Key Changes and Improvements:

- Zero-initialization: Structs are zero-initialized where appropriate for clarity and safety.
- Error Handling: The error function now terminates the program with EXIT_FAILURE.
- Const-correctness: char* changed to const char* in the say and error functions to reflect that the string literals should not be modified.
- Consistent Types: Used socklen_t for the address_size variable to ensure compatibility with the accept function.
- Explicit Initializations: Added explicit zero initialization for structures like sigaction and sockaddr_in.
- These changes enhance the code's readability, safety, and adherence to modern C standards.
*/

int listener_d;

void handle_shutdown(int sig);
void error(const char* msg);

int catch_signal(int sig, void (*handler)(int)) {
    struct sigaction action = {0};
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

void error(const char* msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

int open_listener_socket(void) {
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        error("Cannot open socket");
    }
    return s;
}

void handle_shutdown(int sig) {
    if (listener_d) {
        close(listener_d);
    }

    fprintf(stderr, "Goodbye!\n");
    exit(EXIT_SUCCESS);
}

ssize_t say(int socket, const char* s) {
    ssize_t result = send(socket, s, strlen(s), 0);
    if (result == -1) {
        fprintf(stderr, "Cannot send message to client: %s\n", strerror(errno));
    }
    return result;
}

ssize_t read_in(int socket, char* buf, size_t len) {
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

void bind_to_port(int socket, int port) {
    struct sockaddr_in name = {0};
    name.sin_family = PF_INET;
    name.sin_port = htons((in_port_t)port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    int reuse = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
        error("Cannot set reuse option on socket");
    }

    if (bind(socket, (struct sockaddr*)&name, sizeof(name)) == -1) {
        error("Cannot bind to socket");
    }
}

int main(int argc, char* argv[]) {
    if (catch_signal(SIGINT, handle_shutdown) == -1) {
        error("Cannot set interrupt handler");
    }

    listener_d = open_listener_socket();
    bind_to_port(listener_d, 30000);

    if (listen(listener_d, 10) == -1) {
        error("Cannot listen for connections");
    }

    puts("...waiting for requests...");

    while (1) {
        struct sockaddr_storage client_addr;
        socklen_t address_size = sizeof(client_addr);
        int connect_d = accept(listener_d, (struct sockaddr*)&client_addr, &address_size);

        if (connect_d == -1) {
            error("Cannot open secondary socket");
        }

        if (say(connect_d, "Internet Knock Knock Protocol Server\r\nversion 1.0\r\nKnock Knock\r\n>") != -1) {
            char buf[255];
            read_in(connect_d, buf, sizeof(buf));

            if (strncasecmp("Who's there?", buf, 12)) {
                say(connect_d, "You have to ask \"Who's there?\" first.\r\n");
            } else {
                if (say(connect_d, "Oscar\r\n>") != -1) {
                    read_in(connect_d, buf, sizeof(buf));

                    if (strncasecmp("Oscar who?", buf, 10)) {
                        say(connect_d, "You have to ask \"Oscar who?\" next.\r\n");
                    } else {
                        say(connect_d, "Good question! Nice :D\r\n");
                    }
                }
            }
        }
        close(connect_d);
    }

    return 0;
}
