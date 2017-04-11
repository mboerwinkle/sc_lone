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
		//calculatenewstate
		//sendusersdata
		for(int userIdx = 0; userIdx < userCount; userIdx++){
			userList[userIdx]->sendUserData();
		}
	}
	return 0;
}
