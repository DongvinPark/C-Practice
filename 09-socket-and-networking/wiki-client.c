#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

/*
영문판 위키피디아의 메인 페이지를 HTTP GET으로 가져오는 클라이언트 프로그램이다.

gcc -o exe_file_name wiki-client.c 
를 실행하고,
./exe_file_name "Main_Page"
를 실행하면 된다.

실행하면, DNS 서버의 문제인 듯 하지만

HTTP/1.1 301 Moved Permanently
content-length: 0
location: https://en.wikipedia.org/wiki/Main_Page
server: HAProxy
x-cache: cp5024 int
x-cache-status: int-tls
connection: close

와 같은 응답을 서버로부터 얻어내는 것을 확인할 수 있다.


*/

void error(char* msg){
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int open_socket(char* host, char* port){
    struct addrinfo* res;
    struct addrinfo hints;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if(getaddrinfo(host, port, &hints, &res) == -1){
        error("Cannot get address");
    }

    int d_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(d_sock == -1){
        error("Cannot open socket");
    }

    int c = connect(d_sock, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    if(c == -1){
        error("Cannot connect to remote socket");
    }

    return d_sock;
}

int say(int socket, char* s){
    int result = send(socket, s, strlen(s), 0);
    if (result == -1){
        fprintf(stderr, "%s: %s\n", "Error occurred while communicating with the server.", strerror(errno));
    }
    return result;
}

int main(int argc, char *argv[]){
    int d_sock;

    d_sock = open_socket("en.wikipedia.org", "80");
    char buf[255];

    sprintf(buf, "GET /wiki/%s HTTP/1.1\r\n", argv[1]);
    say(d_sock, buf);

    sprintf(buf, "Host: %s\r\n", "en.wikipedia.org");
    say(d_sock, buf);

    say(d_sock, "Connection: close\r\n\r\n");

    char rec[256];
    int byteRcvd = recv(d_sock, rec, 255, 0);
    while(byteRcvd){
        if(byteRcvd == -1){
            error("Cannot read from server");
        }

        rec[byteRcvd] = '\0';
        printf("%s", rec);
        byteRcvd = recv(d_sock, rec, 255, 0);
    }//wh

    close(d_sock);
    return 0;
}//main




































