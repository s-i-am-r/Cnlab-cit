#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include<string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
int main(int argc, char const* argv[]) 
{ 
	int sockD = socket(AF_INET, SOCK_STREAM, 0); 

	struct sockaddr_in servAddr; 

	servAddr.sin_family = AF_INET; 
	servAddr.sin_port 
		= htons(9009); // use some unused port number 
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");; 

	int connectStatus 
		= connect(sockD, (struct sockaddr*)&servAddr, 
				sizeof(servAddr)); 

	if (connectStatus == -1) { 
		printf("Error...\n"); 
		return 1;
	} 
		char strData[255]; 
		bzero(strData,sizeof(strData));
		char filename[50];
		printf("file name: ");
		scanf("%[^\n]",filename);
		send(sockD,filename,strlen(filename)+1,0);
		memset(filename,0,50);
		recv(sockD, filename, 50, 0); 

		printf("saved in \"%s\"",filename); 
		close(sockD);
		return 0; 
}

