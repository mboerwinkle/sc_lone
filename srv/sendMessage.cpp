#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netDef.h"
char packet[MSGSIZE];
const int usableLength = MSGSIZE-2;
char* msgIdx = packet;
char* packetIdx = packet+1;
void sendMessage(struct sockaddr_in *addr, char* data, int datalen){
	(*msgIdx)++;
	(*packetIdx) = 0;
	int packetSize;
	while(datalen > 0){
		if(datalen > usableLength){
			packetSize = usableLength;
		}else{
			packetSize = datalen;
		}
		memcpy(packet+2, data, packetSize);
		sendto(sockfd, packet, packetSize+2, 0, (struct sockaddr*)(addr), sizeof(*addr));
		datalen-=packetSize;
		data+=packetSize;
		(*packetIdx)++;
	}
}
