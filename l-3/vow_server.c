#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include<string.h>
#include<unistd.h>
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
	bind(servSockD, (struct sockaddr*)&servAddr, 
		sizeof(servAddr)); 

	// listen for connections 
	listen(servSockD, 1); 

	// integer to hold client socket. 
	int clientSocket = accept(servSockD, NULL, NULL); 
	char serMsg[255],filename[50];
	recv(clientSocket,filename,50,0);
	FILE* infile=fopen(filename,"r");
	// int i=0;
	// i=fread(serMsg,1,sizeof(serMsg),infile);
	// if (i==sizeof(serMsg))
	// {
	// 	return 1;
	// }
	

	char c=fgetc(infile);
	int count=0;
	for(;c!=EOF;c=fgetc(infile)){
		if(c!='a' &&c!='e' &&c!='i' &&c!='o' &&c!='u'&& c!='A' &&c!='E' &&c!='I' &&c!='O' &&c!='U'){
			continue;
		}
		printf("%c,",c);
		count+=1;
	}
	fclose(infile);
	FILE*outfile=fopen("output.txt","w");
	fprintf(outfile,"no of vowels found is %d.",count);
	// send's messages to client socket 
	send(clientSocket, "output.txt",11,0); 
	close(servSockD);
	return 0; 
}
