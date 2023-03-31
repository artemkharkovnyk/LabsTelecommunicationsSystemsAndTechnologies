#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <chrono>
char message[] = "Hello there!\n";
bool buf;

int main()
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
   
    while(1){
        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        send(sock, &time, sizeof(&time), 0);
        recv(sock, &buf, sizeof(message), 0);
        
        if (buf){
            std::cout << "Число парне    ";
        }
        else{
            std::cout << "Число не парне ";
        }
        std::cout << std::ctime(&time);
        usleep(1000000);
    }
    close(sock);
    return 0;
}