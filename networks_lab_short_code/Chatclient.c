#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int sockfd;
void *receive_msg(void *arg) {
    char message[BUFFER_SIZE];
    int length;
    while ((length = recv(sockfd, message, sizeof(message)-1, 0)) > 0) {
        message[length] = '\0'; printf("%s", message);
    }
    return NULL;
}
int main(void) {
    struct sockaddr_in server_addr = {0};
    char message[BUFFER_SIZE];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET; server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect"); return 1;
    }
    pthread_t receive_thread;
    pthread_create(&receive_thread, NULL, receive_msg, NULL);
    while (fgets(message, sizeof(message), stdin))
        if (send(sockfd, message, strlen(message), 0) <= 0) break;
    shutdown(sockfd, SHUT_RDWR); close(sockfd);
    return 0;
}