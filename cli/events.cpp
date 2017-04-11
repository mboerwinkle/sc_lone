#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ncurses.h>
#include "notifications.h"
#include "netDef.h"
#include "map.h"

void* textListen(void *null);

void sendEvent(int type, int dat, int coord){
	char msg[3+sizeof(int)*3] = "CTL";
	*(int*)(msg+3) = type;
	*(int*)(msg+3+sizeof(int)) = dat;
	*(int*)(msg+3+sizeof(int)*2) = coord;
	sendto(sockfd, msg, 3+3*sizeof(int), 0, (struct sockaddr*)&sendAddr, sizeof(sendAddr));
}
/*
void* textListen(void *null){
	char dat[160] = {'T','X','T'};
	int len;
	while(1){
		scanf("%s", dat+3);
		len = strlen(dat)+1;
		sendto(sockfd, dat, len, 0, (struct sockaddr*)&sendAddr, sizeof(sendAddr));
	}
}
*/
void* eventListen(void *null){
	MEVENT mpress, mrelease;
	while(1){
		int ch = getch();
		if(ch == KEY_LEFT){
			sendEvent(0/*move*/, 0/*x*/, -1/*one space*/);
		}
		if(ch == KEY_UP){
			sendEvent(0, 1, -1);
		}
		if(ch == KEY_RIGHT){
			sendEvent(0, 0, 1);
		}
		if(ch == KEY_DOWN){
			sendEvent(0, 1, 1);
		}
		if(ch == 'z'){
			while(getch() != KEY_MOUSE){}
			getmouse(&mpress);
			while(getch() != KEY_MOUSE){}
			getmouse(&mrelease);
			sendEvent(1, (locx+mpress.x)+(locy+mpress.y)*mx, (locx+mrelease.x)+(locy+mrelease.y)*mx);
		}
		if(ch == KEY_MOUSE){
			getmouse(&mpress);
			int arg = (locx+mpress.x)+(locy+mpress.y)*mx;
			sendEvent(1, arg, arg);
		}
		if(ch == 'q'){
			char m[80] = "Press CTL+c to quit";
			notify(m);
		}
		if(ch == 'k'){
			sendEvent(2, 0, 0);
		}
	}
}

void startEventListen(){
	mousemask(BUTTON1_PRESSED, NULL);//FIXME what do I actually need?
	pthread_t eventThread;
	pthread_create(&eventThread, NULL, eventListen, NULL);
}
