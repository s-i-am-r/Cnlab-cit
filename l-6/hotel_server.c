#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#define max 10
struct array
{
	int servsock;
	int count;
};
char menu[] = "idly 5\ndosa 15\npoori 17\n";
char orders[10][10];
int ordercount=0;
int fin(char *x)
{
	char *cc = (char *)69;
	char item[12];
	int xx = 0, q;
	while (cc != NULL)
	{

		sscanf(menu + xx, "%s %d", item, &q);
		cc = strchr(menu + xx, '\n');
		xx = cc - menu + 1;
		if (!strcmp(x, item))
		{
			return q;
		}
	}
	return -128;
}
void *serv_fun(void *arg)
{
	char buf[128];
	int fd = *(int *)arg;
	write(fd, menu, sizeof(menu));
	int n = read(fd, buf, 128);
	int x = 0;
	char *cc = strchr(buf + x, '\n');
	int total = 0;
	do
	{

		char item[10];
		int q, price;
		sscanf(buf + x, "%s %d", item, &q);
		x = cc - buf + 1;
		cc = strchr(buf + x, '\n');
		// printf("%d\n", x);
		price = fin(item);
		total += price * q;
	} while (cc != NULL);
	sprintf(orders[ordercount],"%d:%d",ordercount++,total);
	printf("new order:\nno %d\n%d rs\n",ordercount,total);
	
	memset(buf, 0, 128);
	sprintf(buf, "%d", total);
	write(fd, buf, 128);
}
void *acc_fun(void *arg)
{
	int servsock = *(int *)arg;
	int count = 0;
	while (count < max)
	{
		int x = accept(servsock, NULL, NULL);
		int *m = malloc(sizeof(int));
		*m = x;
		pthread_t *id = malloc(sizeof(pthread_t));
		pthread_create(id, NULL, serv_fun, m);
		count++;
	}
}

int main(int argc, char const *argv[])
{

	// create server socket similar to what was done in
	// client program
	int servSockD = socket(AF_INET, SOCK_STREAM, 0);
	pthread_t acc_thread;

	// define server address
	struct sockaddr_in servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9009);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	// bind socket to the specified IP and port
	if (0 > bind(servSockD, (struct sockaddr *)&servAddr,
				 sizeof(servAddr)))
		exit(1);

	// listen for connections
	listen(servSockD, 10);
	pthread_create(&acc_thread, NULL, acc_fun, &servSockD);
	pthread_join(acc_thread, NULL);
	// integer to hold client socket.
	close(servSockD);

	return 0;
}
