#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "map.h"
#include "notifications.h"
int vx = 20, vy = 20;
int locx = 0, locy = 0;
void getCoords(int loc, int* x, int* y);
void displayMap(char* data){
	int displayLen = (vx+1)*vy+1;
	char display[displayLen];
	display[displayLen-1] = 0;
	int pos = 0;
	int loc = *(int*)(data+pos);pos+=sizeof(int);//FIXME reduce code dupe.
	vx = *(int*)(data+pos);pos+=sizeof(int);
	vy = *(int*)(data+pos);pos+=sizeof(int);
	int selectionCount = *(int*)(data+pos); pos+=sizeof(int);
	getCoords(loc, &locx, &locy);
	for(int ty = 0; ty < vy; ty++){
		memcpy(display+(ty*(vx+1)), map+locx+((locy+ty)*mx), vx);
		display[vx+(ty*(vx+1))] = '\n';
	}
//	display[(vx+1)*vy] = 0;
	mvprintw(0, 0, display);
	int unitCount = *(int*)(data+pos);pos+=sizeof(int);
	//unit stuff (char)type(char)team(int)loc(char)selected(char)status
	for(int unitIdx = 0; unitIdx < unitCount; unitIdx++){
		char utype = *(data+pos);pos++;
		char uteam = *(data+pos);pos++;
		int uloc = *(int*)(data+pos);pos+=sizeof(int);
		int ulocx = uloc%mx-locx;
		int ulocy = uloc/mx-locy;
		char usel = *(data+pos);pos++;
		char ustat = *(data+pos);pos++;
		char usize = *(data+pos);pos++;
		for(int dx = ulocx-usize; dx <= ulocx+usize && dx < vx; dx++){
			for(int dy = ulocy-usize; dy <= ulocy+usize && dy < vy; dy++){
				mvaddch(dy, dx, utype|COLOR_PAIR(uteam)|(A_STANDOUT*usel)|(A_BOLD*ustat));
			}
		}
	}
	mvprintw(0, 26, "Selected: %d", selectionCount);
}
void getCoords(int loc, int* x, int* y){
	*x = loc%mx;
	*y = loc/mx;
}
