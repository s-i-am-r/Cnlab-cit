#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include <stdlib.h>
#define PORT 3500
const char msg[]="grade policy:\n>89: A\n70-89: B\n50-69: C\n<50: F\n";
int main(){
	struct sockaddr_in students[3];
	socklen_t studl[3];
	int sockD=socket(AF_INET,SOCK_DGRAM,0);
	if (sockD<0)
	{
		perror("socket failed!");
		exit(1);
	}
	struct sockaddr_in server;
	socklen_t servl=sizeof(server);
	memset(students,0,3*sizeof(struct sockaddr_in));
	memset(&server,0,sizeof(struct sockaddr_in));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(PORT);
	if (0>bind(sockD,(struct sockaddr*)&server,servl))
	{
		perror("bind failed!");
		exit(1);
	}
	
	char buf[100];
	memset(buf,0,sizeof(buf));
	for (int i = 0; i < 3; ++i)
	{
		studl[i]=servl;
		recvfrom(sockD,buf,1,0,(struct sockaddr*)&students[i],&studl[i]);
		printf("cli revd!\n");
	}
	float mark=0;
	int tmp;
	FILE* f= fopen("grades.txt","r");
	while(!feof(f)){
		fscanf(f,"%d",&tmp);
		mark+=tmp;
	}
	fclose(f);
	mark/=3;
	printf("mark: %f\n",mark );
	strcpy(buf,msg);
	sprintf(buf+sizeof(msg)-1,"the avg mark: %f\nGrade: %c\n",mark,(mark>89)?'A':(mark>69)?'B':(mark>49)?'C':'F');
	for (int i = 0; i < 3; ++i)
	{
		sendto(sockD,buf,sizeof(buf),0,(struct sockaddr*)&students[i],studl[i]);
	}
	close(sockD);
	return 0;
}