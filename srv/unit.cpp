#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unit.h"
int unitCount = 0;
int unitCapacity = 0;
Unit** unitList = NULL;
Unit::Unit(char type, char team, int loc, char status){
	this->type = type;
	this->team = team;
	this->loc = loc;
	this->status = status;
	if(unitCount == unitCapacity){
		unitCapacity+=10;
		printf("adding to unitCapacity %d\n", unitCapacity);
		unitList = (Unit**)realloc(unitList, sizeof(Unit*)*unitCapacity);
	}
	unitList[unitCount] = this;
	listIdx = unitCount;
	unitCount++;
	memset(userSelect, 0, MAXUSERS);
}
Unit::~Unit(){
	for(int uIdx = 0; uIdx < MAXUSERS; uIdx++){
		if(userSelect[uIdx]){
			userList[uIdx]->removeSelection(userSelIdx[uIdx]);
		}
	}
	unitList[listIdx] = unitList[unitCount-1];
	unitCount--;
	unitList[listIdx]->listIdx = listIdx;//set the moved in one's idx to its new idx;
}
