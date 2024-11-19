#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#define PORT 3500
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
	char tmp='\0';
	sendto(sockD,&tmp,1,0,(struct sockaddr*)&server,servl);
	printf("sent!\n");
	char buf[100];
	memset(buf,0,sizeof(buf));
	recvfrom(sockD,buf,sizeof(buf),0,(struct sockaddr*)&server,&servl);
	printf("%s\n",buf );
	close(sockD);

}