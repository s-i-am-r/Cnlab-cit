#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <pthread.h>
#define PORT 3500
#define NO_STU 3
#define NO_SPO 3
struct form
{
	char name[20];
	char addr[20];
	char s_name[20];
	int sports;
};

struct mcast_t
{
	char addr[20];
	int port;
};
const struct mcast_t sports[] = {
	{"224.0.0.1",
	 12345},
	{"224.0.0.2",
	 12346},
	{"224.0.0.3",
	 12347}};
char *mess_arr[] = {
	"robert 5 0101000",
	"katie 3 1010000",
	"trey 4 0000101"};
void process(int sockD, FILE *stu_db)
{
	struct sockaddr_in student;
	socklen_t stu_l = sizeof(struct sockaddr_in);
	struct form sports_form;
	memset(&sports_form, 0, sizeof(struct form));
	memset(&student, 0, sizeof(struct sockaddr_in));
	recvfrom(sockD, &sports_form, sizeof(sports_form), 0, (struct sockaddr *)&student, &stu_l);
	fprintf(stu_db, "%s %s %s %d\n", sports_form.name, sports_form.addr, sports_form.s_name, sports_form.sports);
	printf("%s\n", sports[sports_form.sports].addr);
	sendto(sockD,
		   &sports[sports_form.sports],
		   sizeof(struct mcast_t),
		   0,
		   (struct sockaddr *)&student, stu_l);
	return;
}

void *send_mcast(void *arg)
{
	int id = *(int *)arg;
	int sock;
	struct sockaddr_in client_addr;
	char *message = mess_arr[id];
	// Create socket
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Set up the client's address
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(sports[id].port);
	client_addr.sin_addr.s_addr = inet_addr(sports[id].addr);

	// Send message to the client
	while (1)
	{
		if (sendto(sock, message, strlen(message), 0,
				   (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
		{
			perror("Sendto failed");
			close(sock);
			exit(EXIT_FAILURE);
		}
		printf("Sent: %s\n", message);
		sleep(2); // Wait 2 seconds between messages
	}

	close(sock);
	return NULL;
}
FILE *f;
int sockD;
void handle_c_C(int sig)
{
	fclose(f);
	close(sockD);
	exit(0);
}
int main()
{
	 if (signal(SIGINT, handle_c_C) == SIG_ERR) {
        perror("Error assigning signal handler");
        return 1;
    }
	int *tmpint;
	pthread_t spo_th_arr[NO_SPO];
	for (int i = 0; i < NO_SPO; ++i)
	{
		tmpint = malloc(sizeof(int));
		*tmpint = i;
		pthread_create(&spo_th_arr[i], NULL, send_mcast, tmpint);
	}

	sockD = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockD < 0)
	{
		perror("socket failed!");
		exit(1);
	}
	struct sockaddr_in server;
	socklen_t servl = sizeof(server);
	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	if (0 > bind(sockD, (struct sockaddr *)&server, servl))
	{
		perror("bind failed!");
		exit(1);
	}
	f = fopen("students.txt", "a");
	for (;;)
	{
		process(sockD, f);
	}

	// pthread_join(spo_th_arr[0],NULL);
	// pthread_join(spo_th_arr[1],NULL);
	// pthread_join(spo_th_arr[2],NULL);
}