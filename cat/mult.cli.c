#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <Multicast Address> <Port>\n", argv[0]);
        return 1;
    }

    char *multicast_addr = argv[1];
    int port = atoi(argv[2]);

    // Create a UDP socket
    int sockD = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockD < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Allow the socket to reuse the address
    int reuse = 1;
    if (setsockopt(sockD, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("Failed to set socket options");
        close(sockD);
        return 1;
    }

    // Bind the socket to the multicast port
    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockD, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("Bind failed");
        close(sockD);
        return 1;
    }

    // Join the multicast group
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = inet_addr(multicast_addr);
    group.imr_interface.s_addr = INADDR_ANY; // Use the default network interface

    if (setsockopt(sockD, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) < 0) {
        perror("Failed to join multicast group");
        close(sockD);
        return 1;
    }

    // Receive multicast messages
    char buf[1024];
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);

    printf("Listening for multicast messages on %s:%d...\n", multicast_addr, port);
    while (1) {
        memset(buf, 0, sizeof(buf));
        int len = recvfrom(sockD, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&sender_addr, &sender_len);
        if (len < 0) {
            perror("recvfrom failed");
            break;
        }
        buf[len] = '\0'; // Null-terminate the received string
        printf("Received from %s:%d >> %s\n",
               inet_ntoa(sender_addr.sin_addr),
               ntohs(sender_addr.sin_port),
               buf);
    }

    // Leave the multicast group
    if (setsockopt(sockD, IPPROTO_IP, IP_DROP_MEMBERSHIP, &group, sizeof(group)) < 0) {
        perror("Failed to leave multicast group");
    }

    close(sockD);
    return 0;
}
