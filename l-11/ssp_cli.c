#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MULTICAST_GROUP1 "224.0.0.1"  // Multicast group address
#define PORT1 12345                  // Multicast port
#define MULTICAST_GROUP2 "224.0.0.2"  // Multicast group address
#define PORT2 12346                  // Multicast port

int main(int argc,char** argv) {
    int sock;
    struct sockaddr_in local_addr;
    struct ip_mreq group;
    char buffer[256];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Allow multiple applications to use the same port
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("Setting SO_REUSEADDR failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Set up the local address
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    if (argv[1][0]=='0')
    {
        local_addr.sin_port = htons(PORT1);
        group.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP1);
    }else if(argv[1][0]=='1'){
        local_addr.sin_port = htons(PORT2);
        group.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP2);
    }
        
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Listen on any address

    // Bind the socket to the local address
    if (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("Bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Join the multicast group
    group.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) < 0) {
        perror("Joining multicast group failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Receive messages
    while (1) {
        int n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
        if (n < 0) {
            perror("Recvfrom failed");
            close(sock);
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
    }

    close(sock);
    return 0;
}
