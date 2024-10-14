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

// Client Code
int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Sample message to send
    uint32_t message = 0b11010;
    uint8_t crc = crc_check(message, 0b1001);

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Setting up the server address and port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converting the IP address to binary form and storing it
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connecting to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Send the message and the CRC together
    memcpy(buffer, &message, sizeof(message)); // Copy message into buffer
    memcpy(buffer + sizeof(message), &crc, sizeof(crc)); // Copy CRC into buffer
    send(sock, buffer, sizeof(message) + sizeof(crc), 0);
    printf("Message and CRC sent to server\n");

    // Reading the server's reply
    read(sock, buffer, BUFFER_SIZE);
    printf("Message from server: %s\n", buffer);

    // Closing the socket
    close(sock);

    return 0;
}
