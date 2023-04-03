#include <sys/socket.h>
#include <unistd.h>
#include<netinet/in.h>
#include<iostream>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc < 2){
        perror("filename is required");
        exit(1);
    }
    if (argc > 2){
        perror("gound unexpected argument");
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        perror("creating sock");
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr))){
        perror("connect");
        close(sock);
        exit(1);
    }
    char filename[256];
    strncpy(filename, argv[1], 256);
    send(sock, filename, 256, 0);
    char data[1024];
    FILE *fp = fopen(filename, "r");
    while(fgets(data, 1024, fp) != NULL) {
        if (send(sock, data, 1024, 0) == -1) {
            perror("Error in sending file.");
            exit(1);
        }
        bzero(data, 1024);
    }
   
    close(sock);
    return 0;
}