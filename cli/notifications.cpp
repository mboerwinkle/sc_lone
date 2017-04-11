#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "notifications.h"
#define MESSAGECOUNT 5
#define MESSAGELENGTH 50
char messages[MESSAGECOUNT][MESSAGELENGTH+1];
void clearNotifications(){
	
	for(int idx = 0; idx < MESSAGECOUNT; idx++){
		memset(messages[MESSAGECOUNT], 0, MESSAGELENGTH+1);
	}
}
void notify(char* message){
	for(int idx = MESSAGECOUNT; idx >0; idx--){
		strcpy(messages[idx], messages[idx-1]);
	}
	strncpy(messages[0], message, MESSAGELENGTH);
}
void drawNotifications(){
	for(int idx = 0; idx < MESSAGECOUNT; idx++){
		mvprintw(20+idx, 0, messages[idx]);
	}
}
