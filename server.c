#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    char nick_name[256];
    char message[256];

    int nick_name_length;
    int message_length;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        int message_length = read(new_socket, buffer, BUFFER_SIZE);
        
        memcpy(&nick_name_length, &buffer[0],sizeof(nick_name_length));
        memcpy(nick_name, &buffer[0 + sizeof(nick_name_length)], nick_name_length);
        nick_name[nick_name_length] = '\0';

        memcpy(&message_length, &buffer[0 + sizeof(nick_name_length) + nick_name_length], sizeof(message_length));
        memcpy(message, &buffer[0 + sizeof(nick_name_length) + nick_name_length + sizeof(message_length)], message_length);
        message[message_length] = '\0';

        printf("%s:%s\n", nick_name, message);

        close(new_socket);
    }

    return 0;
}