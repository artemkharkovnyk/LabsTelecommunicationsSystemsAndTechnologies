#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
    struct sockaddr_in receiver, sender;
    int repeats;
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(5000);
    receiver.sin_addr.s_addr = htonl(INADDR_ANY);
    sender.sin_family = AF_INET;
    sender.sin_port = htons(5001);
    sender.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    socklen_t receiversize = sizeof(receiver);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0){
        perror("socket creating");
        return 1;
    }

    if (bind(sock, (struct sockaddr *)&receiver, sizeof(receiver)) < 0){
        perror("bind");
        close(sock);
        return 2;
    }

    while(1){
        
        recvfrom(sock, &repeats, 1024, 0, (struct sockaddr *)&receiver, &receiversize);
        std::cout << "repeats: " << repeats << std::endl;
        for (int i = 1; i != repeats; i++){
            std::cout << "send: " << i <<std::endl;
            sendto(sock, &i, sizeof(repeats), 0, (struct sockaddr *)&sender, sizeof(sender));
        }


    }


    return 0;
}