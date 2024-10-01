#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h>
#include <sys/types.h> 
#include<string.h>
#include<ctype.h>
#include<unistd.h>
uint8_t check(char* pass){
	uint8_t result=0;
	result|=(strlen(pass)==6 && isalpha(pass[0]))? 0b10000000 : 0;
	if (!result)
	{
		printf("inv\n");
		return 0;
	}
	
	for(int i=1;i<6;i++){
		result|=(isdigit(pass[i]))? 0b01000000 : (!isalpha(pass[i])) ?0b00100000: 0;
	}
	return result;
}
int main(int argc, char const* argv[]) 
{ 

	// create server socket similar to what was done in 
	// client program 
	int servSockD = socket(AF_INET, SOCK_STREAM, 0); 


	// define server address 
	struct sockaddr_in servAddr; 

	servAddr.sin_family = AF_INET; 
	servAddr.sin_port = htons(9009); 
	servAddr.sin_addr.s_addr = INADDR_ANY; 

	// bind socket to the specified IP and port 
	if(0>bind(servSockD, (struct sockaddr*)&servAddr, 
		sizeof(servAddr))) exit(1); 
	
	// listen for connections 
	listen(servSockD, 10); 

	// integer to hold client socket. 
	int clientSocket = accept(servSockD, NULL, NULL); 
	char serMsg[255];
	bzero(serMsg,sizeof(serMsg));
	recv(clientSocket,serMsg,sizeof(serMsg),0);
	
	printf("%s \n",serMsg);
	uint8_t x= check(serMsg);
	printf("\"%d\"",x);	
	bzero(serMsg,sizeof(serMsg));
	switch (x)
	{
	case 0b10000000:
		strcpy(serMsg,"weak password");
		break;
	case 0b11000000:
		strcpy(serMsg,"medium password");
		break;
	case 0b11100000:
		strcpy(serMsg,"strong password");
		break;
	default:
		strcpy(serMsg,"invalid password!");
		break;
	}
	
	send(clientSocket, serMsg,strlen(serMsg),0); 
	close(servSockD);
	// close(clientSocket);


	return 0; 
}
