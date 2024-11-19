#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#define MULTICAST_GROUP1 "224.0.0.1"  // Multicast group address
#define PORT1 12345                  // Multicast port
#define MULTICAST_GROUP2 "224.0.0.2"  // Multicast group address
#define PORT2 12346                  // Multicast port


struct mcast_t{
    
    char* addr;
    char* message;
    int port;

};
void* send_mcast(void* m_struct){
    int sock;
    struct sockaddr_in group_addr;
    char* message=((struct mcast_t*)m_struct)->message;
    // Create socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the multicast groupS address
    memset(&group_addr, 0, sizeof(group_addr));
    group_addr.sin_family = AF_INET;
    group_addr.sin_port = htons(((struct mcast_t*)m_struct)->port);
    group_addr.sin_addr.s_addr = inet_addr(((struct mcast_t*)m_struct)->addr);

    // Send message to the multicast group
    while (1) {
        if (sendto(sock, message, strlen(message), 0, 
                   (struct sockaddr*)&group_addr, sizeof(group_addr)) < 0) {
            perror("Sendto failed");
            close(sock);
            exit(EXIT_FAILURE);
        }
        printf("Sent: %s\n", message);
        sleep(2);  // Wait 2 seconds between messages
    }

    close(sock);
    return NULL;
}
int main() {
    struct mcast_t m1,m2;
    m1.addr=MULTICAST_GROUP1;
    m2.addr=MULTICAST_GROUP2;
    m1.port=PORT1;
    m2.port=PORT2;
    m1.message="apple";
    m2.message="orange";
    pthread_t t1,t2;
    pthread_create(&t1,NULL,send_mcast,&m1);
    pthread_create(&t2,NULL,send_mcast,&m2);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    

}
