#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netDef.h"
#include "notifications.h"
#include "map.h"


void messageParse();
void startNetwork(char* ip, int port){
	if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) {
//fix ncurses		puts("Couldn't get socket!!!");
	}

	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(6000);
	inet_aton(ip, &sendAddr.sin_addr);

	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(port);
	recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr*)&recvAddr, sizeof(recvAddr))) {
//ncurses		puts("Couldn't listen for things!");
	}
	char alert[] = "joining\0";
	sendto(sockfd, alert, sizeof(alert), 0, (struct sockaddr*)&sendAddr, sizeof(sendAddr));
}

char *data = NULL;
int maxDataLen = 0;
int datalen = 0;
char lastMsgIdx;
char lastPacketIdx;
bool bad = true;
void handleNetwork(){
	//Don't really want something potentially large on the stack, therefore static
	static char msg[MSGSIZE];
	int len = recvfrom(sockfd, msg, MSGSIZE, 0, NULL, NULL);
//	netParse(msg, len);
//}
//void netParse(char* msg, int len){
	char thisMsgIdx = msg[0];
	char thisPacketIdx = msg[1];
	if(thisMsgIdx != lastMsgIdx){//first packet
		lastMsgIdx = thisMsgIdx;
		if(thisPacketIdx != 0){//we missed the first packet :(
			bad = true;
			return;
		}
		if(!bad){//send the completed data to be processed
//			puts("sending message for processing");
			messageParse();
		}
		datalen = 0;
		bad = false;
	}else{//continuation
		if(lastPacketIdx+1 != thisPacketIdx || bad == true){//check for discontinuity
			bad = true;
			return;
		}
	}
	lastPacketIdx = thisPacketIdx;
	if(datalen+len-2 > maxDataLen){
		maxDataLen = datalen+len-2;
//		printf("Our biggest message ever! Reallocating! %d\n", maxDataLen);
		data = (char*)realloc(data, maxDataLen);
	}
	memcpy(data+datalen, msg+2, len-2);
	datalen+=len-2;
}
void messageParse(){
	char prefix[4] = {data[0], data[1], data[2], 0};
	if(!strcmp(prefix, "TXT")){
		notify(data+3);
	}else if(!strcmp(prefix, "SCN")){
		displayMap(data+3);
	}else if(!strcmp(prefix, "MAP")){
		loadMap(data+3);
	}
}
