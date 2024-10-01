#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
int main()
{
    int sockfd;
    char buffer[100];
    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr);
    char hostname[100];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9009);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    // inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    while (1)
    {
        printf("Enter host name :");
        fgets(hostname, 100, stdin);
        hostname[strcspn(hostname, "\n")] = '\0';
        sendto( sockfd, hostname, strlen(hostname)+1, 0, (const struct sockaddr *)&server_addr, len);
        int n = recvfrom(sockfd, buffer, 100, 0, (struct sockaddr *)&server_addr, &len);
        // buffer[n] = '\0';
        printf("IP address: %s\n", buffer);
        memset(buffer,0,sizeof(buffer));
    }
    close(sockfd);
    return 0;
}
