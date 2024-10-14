#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// CRC Functions
int finddeg(uint64_t n, uint8_t size){
    size *= 8;
    uint32_t szz = 1 << (size - 1);
    int i = 0;
    while (!(n & szz)){
        i++;
        n <<= 1;
    }
    return size - 1 - i;
}

uint8_t crc_check(uint32_t num, uint8_t divisor){
    int degree = finddeg(divisor, sizeof(divisor));
    num <<= degree;
    uint8_t rag = num >> (sizeof(num) * 8 - degree);
    uint32_t ptr = 1 << (sizeof(num) * 8 - degree - 1);
    
    while (ptr){
        rag ^= divisor;
        rag <<= 1;
        ptr >>= 1;
    }
    rag ^= divisor;
    return rag & 0xFF; // Return only 8 bits
}

// Server Code
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    const char *correct_reply = "CRC Check Passed";
    const char *wrong_reply = "CRC Check Failed";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Setting up the server address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accepting a connection from a client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Reading the message (binary message + CRC) from the client
    read(new_socket, buffer, BUFFER_SIZE);

    // Extract the CRC and message
    uint32_t received_message = *(uint32_t *)buffer; // Extracting the message
    uint8_t received_crc = *(uint8_t *)(buffer + sizeof(received_message)); // Extracting the CRC

    // Calculate the CRC on the server side
    uint8_t calculated_crc = crc_check(received_message, 0b1001);

    // Check if CRC matches
    if (calculated_crc == received_crc) {
        send(new_socket, correct_reply, strlen(correct_reply), 0);
    } else {
        send(new_socket, wrong_reply, strlen(wrong_reply), 0);
    }

    // Closing the socket
    close(new_socket);
    close(server_fd);

    return 0;
}
