#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to perform CRC check
void calculateCRC(char *message, char *crc) {
    // Divisor polynomial: x^3 + 1 -> 1001
    char divisor[] = "1001";
    int messageLength = strlen(message);
    int divisorLength = strlen(divisor);
    
    // Append zeros to the original message
    char *appendedMessage = malloc(messageLength + divisorLength - 1);
    strcpy(appendedMessage, message);
    for (int i = 0; i < divisorLength - 1; i++) {
        appendedMessage[messageLength + i] = '0';
    }
    appendedMessage[messageLength + divisorLength - 1] = '\0';

    // Binary division
    for (int i = 0; i < messageLength; i++) {
        if (appendedMessage[i] == '1') {
            for (int j = 0; j < divisorLength; j++) {
                appendedMessage[i + j] = (appendedMessage[i + j] == divisor[j]) ? '0' : '1';
            }
        }
    }

    // Extract CRC (last 3 bits)
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

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket
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

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connections...\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Read message from client
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Received message: %s\n", buffer);

    // Calculate CRC
    calculateCRC(buffer, crc);
    printf("Calculated CRC: %s\n", crc);

    // Check for errors (the received CRC should match the calculated CRC)
    if (strcmp(crc, "000") == 0) {
        printf("No errors detected.\n");
    } else {
        printf("Errors detected!\n");
    }

    // Close sockets
    close(new_socket);
    close(server_fd);
    return 0;
}
