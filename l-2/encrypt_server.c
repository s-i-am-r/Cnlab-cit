#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
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
	char serMsg[255];
	bzero(serMsg, sizeof(serMsg));
	recv(clientSocket, serMsg, sizeof(serMsg), 0);

	int i = 0, n = 0;
	char buf[255];
	sscanf(serMsg, "%s %d", buf, &n);
	// memcpy(serMsg,buf,255);
	printf("%s %d\n", buf, n);
	while (buf[i])
	{
		if (buf[i] >= 97)
		{

			buf[i] = (buf[i] - 97+n) % 26 + 97;
		}
		else
		{
			buf[i] = (buf[i] - 65+n) % 26 + 65;
		}

		i++;
	}
	// send's messages to client socket
	send(clientSocket, buf, strlen(buf) + 1, 0);

	close(servSockD);
	return 0;
}
