#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main(void) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr = {0}, client_addr;
    server_addr.sin_family = AF_INET; server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); return 1;
    }
    printf("UDP Time Server Running...\n");
    while (1) {
        socklen_t address_length = sizeof(client_addr);
        if (recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr *)&client_addr, &address_length) < 0) continue;
        time_t current_time = time(NULL);
        snprintf(buffer, sizeof(buffer), "%s", ctime(&current_time));
        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client_addr, address_length);
    }
}