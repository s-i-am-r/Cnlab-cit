#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#include<sys/socket.h>
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

int main(){
	int sockD;
	struct sockaddr_in server_addr;
	socklen_t server_addr_len=sizeof(struct sockaddr_in);
	memset(&server_addr,0,sizeof(struct sockaddr_in));
	struct sockaddr_in client_addr;
	socklen_t client_addr_len=sizeof(struct sockaddr_in);
	memset(&client_addr,0,sizeof(struct sockaddr_in));
	if(0>(sockD=socket(AF_INET,SOCK_DGRAM,0))) exit(1);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=INADDR_ANY;
	if(0>bind(sockD,(struct sockaddr*)&server_addr,server_addr_len)) exit(2);
	unsigned long n =0;
	recvfrom(sockD,&n,sizeof(long),0,(struct sockaddr*)&client_addr,&client_addr_len);
	close(sockD);
	if (!calculate_parity(n))
	{
		printf("no error in msg %ld\n", (n<<1)>>1);
	}else{
		printf("errors found!\n");
	}
	return 0;
}