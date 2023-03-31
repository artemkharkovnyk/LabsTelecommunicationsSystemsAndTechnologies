#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
int main(){
    struct sockaddr_in sender, receiver;
    int buf, repeats;
    sender.sin_family = AF_INET;
    sender.sin_port = htons(5000);
    sender.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(5001);
    receiver.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t receiversize = sizeof(receiver);
    std::cout << "repeats: ";
    std::cin >> repeats;
    int socksend = socket(AF_INET, SOCK_DGRAM, 0);
    if (socksend < 0){
        perror("sender socket creating");
        return 1;
    }
    int sockrecv = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockrecv < 0){
        perror("sender socket creating");
        close(socksend);
        return 1;
    }
    if (bind(sockrecv, (struct sockaddr *)&receiver, sizeof(receiver)) < 0){
        perror("bind");
        close(sockrecv);
        close(socksend);
        return 2;
    }

    sendto(socksend, &repeats, sizeof(repeats), 0, (struct sockaddr *)&sender, sizeof(sender));
    close(socksend);
    for (int i = 0; i < repeats; i++){
        recvfrom(sockrecv, &buf, 1024, 0, (struct sockaddr *)&receiver, &receiversize);
        std::cout << "respond number " << buf << std::endl;
    }
    close(sockrecv);
    return 0;
}