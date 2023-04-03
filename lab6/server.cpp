#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <thread>
#include <string.h>
#include <sys/stat.h>
#include <chrono>

void clientHandler(int sock){
    char filename[256];
    char buffer[1024];
    int bytesNumber;
    bytesNumber = recv(sock, filename, 256, 0);

    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string dirname(11, '\0');
    std::strftime(&dirname[0], dirname.size(), "%Y-%m-%d", std::localtime(&now));
    
    char path[256 + 12]= {0};
    
    strncpy(path, &dirname[0], 11);
    path[10] = '/';
    strncpy(&path[11], filename, 30);
    struct stat sb;
    if (stat(&dirname[0], &sb) != 0){
        const int dir_err = mkdir(&dirname[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (-1 == dir_err){
            printf("Error creating directory");
            close(sock);
            return;
        }
    }

    FILE *fp =fopen(path, "w");
    
    while(1){
        bytesNumber = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesNumber <= 0) break;
        fprintf(fp, "%s", buffer);
    }
    fclose(fp);
    close(sock);
}

int main(int argc, char *argv[]){
 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t addrsize = sizeof(addr);
    int buf;
    int threadsNumber = 3;


    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0){
        perror("creating sock");
        exit(1);
    }
    if (bind(listener, (struct sockaddr *)&addr , sizeof(addr)) <  0){
        perror("bind");
        exit(1);
    }
    listen(listener, 3);
    
    
    while(1){
        int sock = accept(listener, (struct sockaddr*)&addr, &addrsize);
        std::thread th(clientHandler, sock);
        th.detach();
    }

    close(listener);
    
}