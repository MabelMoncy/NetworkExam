#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    char filename[100], buffer[BUFFER_SIZE];
    server_addr.sin_family = AF_INET; server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect"); return 1;
    }
    printf("Enter filename: ");
    if (!fgets(filename, sizeof(filename), stdin)) return 1;
    filename[strcspn(filename, "\r\n")] = '\0';
    send(sockfd, filename, strlen(filename), 0);
    printf("\nResponse from Server:\n\n");
    int length;
    while ((length = recv(sockfd, buffer, sizeof(buffer), 0)) > 0)
        fwrite(buffer, 1, length, stdout);
    close(sockfd);
    return 0;
}