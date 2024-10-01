#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
char *ip_address(const char *hostname)
{
    FILE *file = fopen("dns_record.txt", "r");
    if (!file)
    {
        return "File not found";
    }
    static char ip_address[100];
    memset(ip_address,0,100);
    char file_hostname[100];
    while (fscanf(file, "%s %s", file_hostname, ip_address) != EOF)
    {
        if (strcmp(file_hostname, hostname) == 0)
        {
            fclose(file);
            return ip_address;
        }
    }
    fclose(file);
    return "Domain not found";
}
int main()
{
    int sockfd;
    char buffer[100];
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9009);
    bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    while (1)
    {
        int n = recvfrom(sockfd, buffer, 100, 0, (struct sockaddr *)&client_addr, &len);
        // buffer[n] = '\0';
        printf("Received DNS : %s\n", buffer);
        char *response = ip_address(buffer);
        sendto(sockfd, response, strlen(response), 0, (const struct sockaddr *)&client_addr, len);
        printf("Sent response: %s\n", response);
    }
    close(sockfd);
    return 0;
}
