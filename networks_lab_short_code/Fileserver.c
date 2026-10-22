#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0), client_socket;
    struct sockaddr_in server_addr = {0}, client_addr;
    server_addr.sin_family = AF_INET; server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);
    printf("File Server Running...\n");
    while (1) {
        socklen_t address_length = sizeof(client_addr);
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &address_length);
        if (client_socket < 0) continue;
        if (fork() == 0) {
            char filename[100], buffer[BUFFER_SIZE];
            close(server_fd);
            int length = recv(client_socket, filename, sizeof(filename)-1, 0);
            if (length <= 0) { close(client_socket); exit(0); }
            filename[length] = '\0';
            filename[strcspn(filename, "\r\n")] = '\0';
            FILE *requested_file = fopen(filename, "r");
            snprintf(buffer, sizeof(buffer), "Server PID: %d\n\n", getpid());
            send(client_socket, buffer, strlen(buffer), 0);
            if (!requested_file) {
                strcpy(buffer, "Requested file does not exist.\n");
                send(client_socket, buffer, strlen(buffer), 0);
            } else {
                while (fgets(buffer, sizeof(buffer), requested_file))
                    send(client_socket, buffer, strlen(buffer), 0);
                fclose(requested_file);
            }
            close(client_socket);
            exit(0);
        }
        close(client_socket);
        while (waitpid(-1, NULL, WNOHANG) > 0) {}
    }
}