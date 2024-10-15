#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024


void calculateCRC(char *message, char *crc) {
    
    char divisor[] = "1001";
    int messageLength = strlen(message);
    int divisorLength = strlen(divisor);
    
    
    char *appendedMessage = malloc(messageLength + divisorLength - 1);
    strcpy(appendedMessage, message);
    for (int i = 0; i < divisorLength - 1; i++) {
        appendedMessage[messageLength + i] = '0';
    }
    appendedMessage[messageLength + divisorLength - 1] = '\0';

    
    for (int i = 0; i < messageLength; i++) {
        if (appendedMessage[i] == '1') {
            for (int j = 0; j < divisorLength; j++) {
                appendedMessage[i + j] = (appendedMessage[i + j] == divisor[j]) ? '0' : '1';
            }
        }
    }

    
    strcpy(crc, appendedMessage + messageLength);
    free(appendedMessage);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char crc[4];

    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connections...\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Received message: %s\n", buffer);

    
    calculateCRC(buffer, crc);
    printf("Calculated CRC: %s\n", crc);

    
    if (strcmp(crc, "000") == 0) {
        printf("No errors detected.\n");
    } else {
        printf("Errors detected!\n");
    }

    
    close(new_socket);
    close(server_fd);
    return 0;
}
