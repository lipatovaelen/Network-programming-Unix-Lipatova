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
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    char nick_name[256] = "Valentin";
    char message[256] = "Hello my dear";


    printf("Enter your nick name\n");
    fgets(nick_name, 256, stdin);
    nick_name[strcspn(nick_name, "\n")] = 0;
    printf("Enter your message\n");
    fgets(message, 256, stdin);
    // Удаляем символ новой строки, если он есть
    message[strcspn(message, "\n")] = 0;
    
    int nick_name_length = strlen(nick_name);
    int message_length = strlen(message);

    // first step: pack integer number - length of name 
    memcpy(&buffer[0], &nick_name_length, sizeof(nick_name_length));
    memcpy(&buffer[0 + sizeof(nick_name_length)], nick_name, nick_name_length);
    memcpy(&buffer[0 + sizeof(nick_name_length) + nick_name_length], &message_length, sizeof(message_length));
    memcpy(&buffer[0 + sizeof(nick_name_length) + nick_name_length + sizeof(message_length)], message, message_length);
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("invalid address / address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    send(sock, buffer, sizeof(nick_name_length) + nick_name_length + sizeof(message_length) + message_length, 0);

    close(sock);
    return 0;
}
