#include <stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char** argv){
	if(argc<4) return 1;;
	int sockD = socket(AF_INET, SOCK_DGRAM, 0);
	if (0>sockD)
	{
		perror("socket failed!");
		return 1;
	}

	char * addr = argv[1];
	int port = atoi(argv[2]);
	struct sockaddr_in serv;
	socklen_t serl=sizeof(serv);
	memset(&serv,0,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = inet_addr(addr);
	char* buf=argv[3];
	while(1){
		sendto(sockD, buf, strlen(buf),0,( const struct sockaddr *)&serv, serl);
		sleep(2);
	}
}