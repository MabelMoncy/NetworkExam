#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS], client_count;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void broadcast(char *message, int sender) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++)
        if (clients[i] != sender) send(clients[i], message, strlen(message), 0);
    pthread_mutex_unlock(&lock);
}
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    int length;
    while ((length = recv(client_socket, buffer, sizeof(buffer)-1, 0)) > 0) {
        buffer[length] = '\0';
        broadcast(buffer, client_socket);
    }
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++)
        if (clients[i] == client_socket) { clients[i] = clients[--client_count]; break; }
    pthread_mutex_unlock(&lock);
    close(client_socket);
    return NULL;
}
int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0), client_socket;
    struct sockaddr_in server_addr = {0}, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, MAX_CLIENTS);
    printf("Server running on port %d\n", PORT);
    while (1) {
        socklen_t address_length = sizeof(client_addr);
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &address_length);
        if (client_socket < 0) continue;
        pthread_mutex_lock(&lock);
        if (client_count == MAX_CLIENTS) {
            pthread_mutex_unlock(&lock); close(client_socket); continue;
        }
        clients[client_count++] = client_socket;
        pthread_mutex_unlock(&lock);
        int *client_argument = malloc(sizeof(int));
        *client_argument = client_socket;
        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, client_argument);
        pthread_detach(client_thread);
    }
}