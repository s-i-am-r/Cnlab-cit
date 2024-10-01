#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h>
#include <unistd.h>


char* slide_recv(int fd)
{
	char buf[5];
	int sz=0,N=5,x=0;
	char status=0;
	while(1){
		
		
		x=recv(fd,buf,5,0);
		write(1,buf,x);
		//printf(";");
		
		if (x==5)
		{
			
			if (buf[4]=='$')
			{
				status=2;

			}else{

				status=0;
			}
			
		}else if(x<5)
		{
			memset(buf,0,5);
			status=1;
		}
		send(fd,&status,1,0);
		if (status==2)
		{
			break;
		}
		
	}





}

int main(int argc, char const *argv[])
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
	bind(servSockD, (struct sockaddr *)&servAddr,
		 sizeof(servAddr));

	// listen for connections
	listen(servSockD, 10);

	// integer to hold client socket.
	int clientSocket = accept(servSockD, NULL, NULL);
	slide_recv(clientSocket);
	close(servSockD);
	return 0;
}
