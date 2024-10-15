#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080


void calculateCRC(char *message, char *crc) {
    
    char divisor[] = "1001";
    int messageLength = strlen(message);
    int divisorLength = strlen(divisor);
    
    
    char *appendedMessage = malloc(messageLength + divisorLength );
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


void appendCRC(char *message, char *transmittedMessage) {
    char crc[4];
    calculateCRC(message, crc);
    sprintf(transmittedMessage, "%s%s", message, crc);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "101110000"; 
    char transmittedMessage[1024] = {0};

    
    appendCRC(message, transmittedMessage);
    printf("Transmitting message: %s\n", transmittedMessage);

    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    
    send(sock, transmittedMessage, strlen(transmittedMessage), 0);
    printf("Message sent to server\n");

    
    close(sock);
    return 0;
}
