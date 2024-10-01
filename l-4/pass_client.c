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
		scanf("%s",strData);
		send(sockD,strData,sizeof(strData),0);
		memset(strData,0,sizeof(strData));
		recv(sockD, strData, sizeof(strData), 0); 
		
		printf("Message from server: %s \n", strData); 
		close(sockD);

	return 0; 
}

