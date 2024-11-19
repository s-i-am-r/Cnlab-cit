#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#define PORT 3500

struct form{
	char name[20];
	char addr[20];
	char s_name[20];
	int sports;
};

struct mcast_t
{
	char addr[20];
	int port;
};

void* recv_mcast(void* mcasr_obj){
	int sock;
    struct sockaddr_in local_addr;
    struct ip_mreq group;
    char buffer[256];
    char* addr=((struct mcast_t*)mcasr_obj)->addr;
    int port=((struct mcast_t*)mcasr_obj)->port;
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
    local_addr.sin_port = htons(port);
    group.imr_multiaddr.s_addr = inet_addr(addr);
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
    return NULL;
}

int main(){
	int sockD=socket(AF_INET,SOCK_DGRAM,0);
	if (sockD<0)
	{
		perror("socket failed!");
		exit(1);
	}
	struct sockaddr_in server;
	socklen_t servl=sizeof(server);
	memset(&server,0,sizeof(struct sockaddr_in));
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	inet_pton(AF_INET,"127.0.0.1",&server.sin_addr);
	printf("sent!\n");
	struct form myform;
	printf("enter details. name addr sports_name sport_id[0=> BASKETBALL, 1=>TENNIS, 2=>FOOTBALL ]\n");
	scanf("%s %s %s %d",
		myform.name,
		myform.addr,
		myform.s_name,
		&myform.sports
		);
	sendto(sockD,&myform,sizeof(myform),0,(struct sockaddr*)&server,servl);
	struct mcast_t mcast_grp;
	recvfrom(sockD,&mcast_grp,sizeof(struct mcast_t),0,(struct sockaddr*)&server,&servl);
	close(sockD);
	printf("adr: %s\nport: %d\n",mcast_grp.addr,mcast_grp.port);
	recv_mcast(&mcast_grp);
	return 0;	
}