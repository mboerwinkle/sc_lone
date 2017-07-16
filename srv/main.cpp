#include <stdio.h>
#include <stdlib.h>
#include "netDef.h"
#include "delay.h"
#include "map.h"
#include "unit.h"
#define FRAMERATE 30
extern void startNetListen();

char mapname[80] = "../dat/testMap.map";
int main(){
	loadUnitTypes();
	startNetListen();
	loadMap(mapname);//FIXME map selection done on the server...Ncurses
	for(int x = 1; x < 10; x+=3) for(int y = 1; y < 10; y+=3){
		new Unit(0, 1, x+mx*y, 0, 0);
		new Unit(1, 2, x+mx*(y+30), 0, 0);
		new Unit(2, 3, x+30+mx*(y+10), 0, 0);
	}

	while(1){
		delay(FRAMERATE);
		//calculate new state
		//FIXME only decrement top unit
		for(int unitIdx = 0; unitIdx < unitCount; unitIdx++){
			Unit* targ = unitList[unitIdx];
			if(targ->hp <= 0){
				delete targ;
				unitIdx--;
				continue;
			}
			if(targ->actTimer <= 0){
				targ->actTimer = targ->actCooldown;
				targ->act();
			}else{
				targ->actTimer--;
			}
		}
		//sendusersdata
		for(int userIdx = 0; userIdx < userCount; userIdx++){
			userList[userIdx]->sendUserData();
		}
	}
	return 0;
}
