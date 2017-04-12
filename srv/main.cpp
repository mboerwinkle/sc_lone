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
	
	startNetListen();
	loadMap(mapname);
	new Unit('Z', 2, mx+3, 0);
	new Unit('Z', 2, 5*mx+7, 0);
	while(1){
		delay(FRAMERATE);
		//calculate new state
		//FIXME only decrement top unit
		for(int unitIdx = 0; unitIdx < unitCount; unitIdx++){
			Unit* targ = unitList[unitIdx];
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
