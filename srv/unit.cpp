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
	this->dest = loc;
	
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
	act();
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
	if(loc != dest){//we change dest to loc once we arrive close enough
		move(pathFindDir());
	}
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
int Unit::pathFindDir(){
	double bestFitness = distance(loc, dest);
	int bestDir = -1;
	for(int dir = 0; dir < 8; dir++){
		double newFitness = dirFitness(dir);
		if(newFitness < 0) continue;
		if(newFitness < bestFitness){
			bestFitness = newFitness;
			bestDir = dir;
		}
	}
	return bestDir;
}
double Unit::dirFitness(int dir){//lower is better.
	int x = loc%mx;
	int y = loc/mx;
	if(dir > 0 && dir < 4){
		x++;
	}
	if(dir > 4){
		x--;
	}
	if(dir > 6 || dir < 2){
		y--;
	}
	if(dir > 2 && dir < 6){
		y++;
	}
	if(!validLoc(x+y*mx)){
		return -1;
	}
	return distance(dest, x+y*mx);
}
void Unit::move(int dir){
//701
//6 2
//543
	if(dir == -1) return;
	int x = loc%mx;
	int y = loc/mx;
	if(dir > 0 && dir < 4){
		x++;
	}
	if(dir > 4){
		x--;
	}
	if(dir > 6 || dir < 2){
		y--;
	}
	if(dir > 2 && dir < 6){
		y++;
	}
	loc = x+y*mx;
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

double distance(int l1, int l2){
	int x1 = l1%mx;
	int y1 = l1/mx;
	int dx = l2%mx-x1;
	int dy = l2/mx-y1;
	return sqrt(dx*dx+dy*dy);
}
