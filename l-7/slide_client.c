#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

char *slide_send(int fd, char *buf, int n)
{
	char c;
	char window[5], cmd[6], sent = 0, status;
	while (1)
	{
		
		scanf("%s",cmd);
		if (cmd[0] == '0')
		{
			send(fd, "xx", 2, 0);
		}
		else
		{

			if (n - sent < 5)
			{
				memcpy(window, buf + sent, n - sent);
				memset(window + n - sent, '$', 5 - n + sent);
			}
			else
			{
				memcpy(window, buf + sent, 5);
			}

			send(fd, window, 5, 0);
			
		}

		recv(fd, &status, 1, 0);
		if (status == 0)
		{
			sent += 5;
		}else if(status==2)
		{
			break;
		}
		
	}
}
int main(int argc, char const *argv[])
{
	int sockD = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9009); // use some unused port number
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int connectStatus = connect(sockD, (struct sockaddr *)&servAddr,
								sizeof(servAddr));

	if (connectStatus == -1)
	{
		printf("Error...\n");
		return 1;
	}

	slide_send(sockD,"Tuesday\n10-09-2024\n2:00 PM\n35C",31);
	close(sockD);

	return 0;
}
