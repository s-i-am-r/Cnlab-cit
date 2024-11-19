#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#define PORT 3502


int calculate_parity(unsigned long n){
	int res=0;
	for (;n;)
	{
		res^=(n)&1;
		n>>=1;
	}
	return res;
}

char* argv[]={
	"",
	"1244"
};
int main(/*int argc,char**argv*/){
	// if (argc!=2)
	// {
	// 	exit(1);
	// }
	char* end=(char*)0x744411;
	unsigned long num=strtol(argv[1],&end,10);
	if (*end!='\0')
	{
		exit(1);
	}
	if (num>>(8*sizeof(num)-1))
	{
		exit(1);
	}
	num|=(long)(calculate_parity(num))<<(8*sizeof(long)-1);
	int sockD;
	struct sockaddr_in server_addr;
	socklen_t server_addr_len=sizeof(struct sockaddr_in);
	memset(&server_addr,0,sizeof(struct sockaddr_in));
	if(0>(sockD=socket(AF_INET,SOCK_DGRAM,0))) exit(1);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	sendto(sockD,&num,sizeof(num),0,(struct sockaddr*)&server_addr,server_addr_len);
	close(sockD);
	return 0;
}