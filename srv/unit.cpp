#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
void Unit::act(){
//remove yourself from the blocking map.
	int x = loc%mx;
	int y = loc/mx;
	for(int dx = x; dx < x+size; dx++){
		for(int dy = y; dy < y+size; dy++){
			bMap[dx+mx*dy] = 0;
		}
	}
//move in best direction
//	if(loc != dest){//we change dest to loc once we arrive close enough
//		move(pathFindDir());
//	}
//re-add yourself to blocking map
	x = loc%mx;
	y = loc/mx;
	for(int dx = x; dx < x+size; dx++){
		for(int dy = y; dy < y+size; dy++){
			bMap[dx+mx*dy] = 1;
		}
	}

//attack if you can.
//change dest if seeing enemy.
//attacTimer decrement.
//status change
}

int Unit::validLoc(int l){
	int x = l%mx;
	int y = l/mx;
	for(int dx = x; dx < size+x; dx++){
		for(int dy = y; dy < size+y; dy++){
			if(bMap[dx+mx*dy]){
				return 0;
			}
		}
	}
	return 1;
}

int distance(int l1, int l2){
	int x1 = l1%mx;
	int y1 = l1/mx;
	int dx = l2%mx-x1;
	int dy = l2/mx-y1;
	return sqrt(dx*dx+dy*dy);
}
