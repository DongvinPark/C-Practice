#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

/*
소켓을 이용하여 통신하는 서버 프로그램이다.
자바의 ServerSocket과 거의 비슷하게 동작한다.

이 프로그램을 작동시킨 후, 새로운 우분투 터미널을 열어서 텔넷 클라이언트 명령어를 실행하면
telnet 127.0.0.1 30000
서버의 응답을 확인할 수 있다.
*/

int listener_d;

int catch_signal(int sig, void (*handler)(int)){
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction (sig, &action, NULL);
}

void error(char* msg){
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int open_listener_socket(){
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if(s==-1){
        error("cannot open socket");
    }
    return s;
}

void handle_shutdown(int sig){
    if(listener_d){
        close(listener_d);
    }

    fprintf(stderr, "Good Bye! \n");
    exit(0);
}

int say(int socket, char* s){
    int result = send(socket, s, strlen(s), 0);
    if(result == -1){
        fprintf(stderr, "%s: %s\n", "cannot send message to client", strerror(errno));
    }
    return result;
}

int read_in(int socket, char* buf, int len){
    char* s = buf;
    int slen = len;
    int c = recv(socket, s, slen, 0);
    while( (c>0) && (s[c-1] != '\n') ){
        s += c;
        slen -= c;
        c = recv(socket, s, slen, 0);
    }
    if(c<0)
        return c;
    else if (c==0)
        buf[0] = '\0';
    else
        s[c-1] = '\0';
    
    return (len - slen);
}

void bind_to_port(int socket, int port){
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t)htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    int reuse = 1;
    if(setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int)) == -1){
        error("cannot set reuse option on socket");
    }
    int c = bind(socket, (struct sockaddr*)&name, sizeof(name));
    if(c == -1){
        error("cannot bind to socket");
    }
}

int main(int argc, char* argv[]){
    if(catch_signal(SIGINT, handle_shutdown) == -1){
        error("cannot set interrupt processor");
    }

    listener_d = open_listener_socket();
    bind_to_port(listener_d, 30000);
    if(listen(listener_d, 10) == -1){
        error("cannot listen to request");
    }

    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    puts("... waiting for request ...");

    char buf[255];
    while(1){
        int connect_d = accept(listener_d, (struct sockaddr*)&client_addr, &address_size);
        if(connect_d == -1){
            error("cannot open second socket");
        }

        if(
            say(
                connect_d, "Internet Knock Knock Protocol Server\r\nversion 1.0\r\nKnock Knock\r\n>"
                ) != -1
        ){
            read_in(connect_d, buf, sizeof(buf));
            if(strncasecmp("Who's there?",buf,12)){
                say(connect_d, "you have to ask first \"Who's there?\" .");
            } else {
                if(say(connect_d, "Oscar\r\n>") != -1){
                    read_in(connect_d, buf, sizeof(buf));
                    if(strncasecmp("Oscar who?", buf, 10)){
                        say(connect_d, "you have to ask \"Oscar who?\" for next.");
                    } else {
                        say(connect_d, "good question nice :D\r\n");
                    }
                }
            }
        }
        close(connect_d);
    }//wh
    return 0;
}//main