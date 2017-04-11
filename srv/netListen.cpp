#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "netDef.h"
struct sockaddr_in recvAddr;
int sockfd;
void* netListen(void *null);
void startNetListen(){
	recvAddr.sin_family=AF_INET;
	recvAddr.sin_port=htons(6000);
	recvAddr.sin_addr={.s_addr = htonl(INADDR_ANY)};
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind(sockfd, (struct sockaddr*)&recvAddr, sizeof(recvAddr));
	pthread_t networkThread;
	pthread_create(&networkThread, NULL, netListen, NULL);

}
void* netListen(void *null){
	char msg[MSGSIZE];
	struct sockaddr_in incomingAddr;
	incomingAddr.sin_family=AF_INET;
	incomingAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	incomingAddr.sin_port = htons(6000);
	bind(sockfd, (struct sockaddr*)&incomingAddr, sizeof(incomingAddr));
	socklen_t len;
	while(1){
		int found = 0;
		len = sizeof(incomingAddr);
		recvfrom(sockfd, msg, MSGSIZE, 0, (struct sockaddr*)&incomingAddr, &len);
		for(int x = 0; x < userCount; x++){
			if(userList[x]->addr.sin_addr.s_addr == incomingAddr.sin_addr.s_addr){
				found = 1;
				netParse(x, msg);
			}
		}
		if(!found){
			if(strncmp("joining", msg, 7)) continue;
			new User(incomingAddr.sin_addr.s_addr);
		}
	}
}
