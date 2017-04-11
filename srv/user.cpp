#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netDef.h"
#include "map.h"
#include "unit.h"
User* userList[MAXUSERS];
int userCount = 0;
User::User(unsigned long ip){
	addr.sin_family=AF_INET;
	addr.sin_port=htons(5999);
	addr.sin_addr.s_addr=ip;
	userList[userCount] = this;
	listIdx = userCount;
	userCount++;
	sendMapName();
}
void User::removeSelection(int idx){
	selection[idx]->userSelect[listIdx] = 0;
	selectionCount--;
	selection[idx] = selection[selectionCount];
	selection[idx]->userSelIdx[listIdx] = idx;
}
void User::clearSelection(){
	for(int idx = 0; idx < selectionCount; idx++){
		selection[idx]->userSelect[listIdx] = 0;
	}
	selectionCount = 0;
}
void User::selectUnit(int idx){
	if(selectionCount > 900) puts("warning 1212");//FIXME descriptive
	unitList[idx]->userSelect[listIdx] = 1;
	unitList[idx]->userSelIdx[listIdx] = selectionCount;
	selection[selectionCount] = unitList[idx];
	selectionCount++;
}
void User::killSelectedUnits(){
	printf("selected: %d\n", selectionCount);
	while(selectionCount != 0){
		puts("delete");
		delete selection[0];
	}
}
void User::sendMapName(){
	int msgLen = strlen(mapname)+4;
	char msg[msgLen] = {'M', 'A', 'P'};
	strcpy(msg+3, mapname);
	msg[msgLen-1] = 0;
	sendMessage(&addr, msg, msgLen);
}
void User::sendUserData(){
	static char data[100000];//FIXME check for overflow.
	int datalen = 0;
	strcpy(data, "SCN"); datalen+=3;
	*(int*)(data+datalen) = loc; datalen+=sizeof(int);
	*(int*)(data+datalen) = vx; datalen+=sizeof(int);
	*(int*)(data+datalen) = vy; datalen+=sizeof(int);
	*(int*)(data+datalen) = selectionCount; datalen+=sizeof(int);
	int* viewUnitCount = (int*)(data+datalen);datalen+=sizeof(int);
	*viewUnitCount = 0;
	for(int unitIdx = 0; unitIdx < unitCount; unitIdx++){
		Unit* targ = unitList[unitIdx];
		if(canSee(targ->loc)){
			(*viewUnitCount)++;
			*(data+datalen) = targ->type;datalen++;
			*(data+datalen) = targ->team;datalen++;
			*(int*)(data+datalen) = targ->loc;datalen+=sizeof(int);
			*(data+datalen) = targ->userSelect[listIdx];datalen++;
			*(data+datalen) = targ->status;datalen++;
		}
	}
	strcpy(data+datalen, "END"); datalen+=3;
	sendMessage(&addr, data, datalen);
}
void User::getCamCoord(int* x, int* y){
	*x = loc%mx;
	*y = loc/mx;
}
void User::setCamCoord(int x, int y){
	loc = x+y*mx;
}
void User::moveCamera(int axis, int amount){
	int x, y, nx, ny;
	getCamCoord(&x, &y);
	nx = x;
	ny = y;
	if(axis == 0){//x
		nx = x+amount;
	}else{
		ny = y+amount;
	}
	if(nx >= 0 && ny >= 0 && nx+vx <= mx && ny+vy <= my){
		setCamCoord(nx, ny);
	}
}
void User::select(int c1, int c2){//FIXME third argument for clearing selection or shift stuff.
	clearSelection();
	int x1 = c1%mx;
	int y1 = c1/mx;
	int x2 = c2%mx;
	int y2 = c2/mx;
	for(int idx = 0; idx < unitCount; idx++){
		int tloc = unitList[idx]->loc;
		int tx = tloc%mx;//FIXME dont remake int?
		int ty = tloc/mx;//FIXME change all occurences to a macro? function?
		if(((x1 <= tx && x2 >= tx) || (x1 >= tx && x2 <= tx)) && ((y1 <= ty && y2 >= ty) || (y1 >= ty && y2 <= ty))){
			selectUnit(idx);
		}
	}
	printf("user selecting from %d %d to %d %d\n", x1, y1, x2, y2);
}
int User::canSee(int tloc){
	int myX, myY;
	getCamCoord(&myX, &myY);
	int x = tloc%mx;
	int y = tloc/mx;
	if(x >= myX && x < myX+vx && y >= myY && y < myY+vy){
		return 1;
	}
	return 0;
}
