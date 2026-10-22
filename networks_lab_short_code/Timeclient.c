#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main(void) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buffer[BUFFER_SIZE] = "TIME";
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET; server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));
    int length = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, NULL, NULL);
    if (length > 0) { buffer[length] = '\0'; printf("Server Time: %s", buffer); }
    close(sockfd);
    return 0;
}