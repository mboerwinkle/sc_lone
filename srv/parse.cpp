#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netDef.h"
void netParse(int userIdx, char* msg){
	char prefix[4] = {msg[0], msg[1], msg[2], 0};
	if(!strcmp(prefix, "CTL")){
		int type = *(int*)(msg+3);
		int dat = *(int*)(msg+3+sizeof(int));
		int coord = *(int*)(msg+3+2*sizeof(int));
		if(type == 0){//cameramovement//FIXME switch? also for client keypresses
			userList[userIdx]->moveCamera(dat, coord);
		}
		if(type == 1){//selection//FIXME two types of events. shift and reset selections
			userList[userIdx]->select(dat, coord);//here dat and coord are both coordinates for the two corners of the selection
		}
		if(type == 2){//kill all selected units
			userList[userIdx]->killSelectedUnits();
		}
	}else if(!strcmp(prefix, "TXT")){
		printf("User%d: %s\n", userIdx, msg+3);
		int len = strlen(msg)+1;
		for(int userIdx = 0; userIdx < userCount; userIdx++){
			sendMessage(&(userList[userIdx]->addr), msg, len);
		}
	}else if(!strcmp(prefix, "SPN")){
	}
	
}
