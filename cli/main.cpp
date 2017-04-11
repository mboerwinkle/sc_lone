#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "netDef.h"
#include "notifications.h"
int sockfd;
struct sockaddr_in recvAddr, sendAddr;
char localhost[10] = "127.0.0.1";

extern void startNCurses();
extern void startEventListen();

int main(int argc, char** argv){
	if(argc == 1){
		startNetwork(localhost, 5999);
	}else if(argc == 2){
		startNetwork(argv[1], 5999);
	}else{
		puts("Usage: ./cli.out [server ip]");
	}
	startNCurses();
	startEventListen();
	clearNotifications();
	while(1){
		handleNetwork();
		drawNotifications();
		refresh();
	}
}
