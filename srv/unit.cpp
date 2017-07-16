#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "unit.h"
unsigned int teamVals[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
int unitCount = 0;
int unitCapacity = 0;
Unit** unitList = NULL;
Unit::Unit(int typeIdx, int team, int loc, char status, int ownerIdx){
	unitType* p = &(unitTypes[typeIdx]);
	this->type = p->type;//FIXME make this an independent function
	this->team = teamVals[team];
	this->loc = loc;
	this->dest = loc;
	this->status = status;
	this->ownerIdx = ownerIdx;
	this->attackMask = ~(this->team);
	this->actCooldown = p->actCooldown;
	this->actTimer = 0;
	this->attackCooldown = p->attackCooldown;
	this->attackTimer = 0;
	this->damage = p->damage;
	this->range = p->range;
	this->maxHp = p->maxHp;
	hp = maxHp;
	this->visionDist = p->visionDist;
	this->radius = p->radius;
	if(unitCount == unitCapacity){
		unitCapacity+=10;
		printf("adding to unitCapacity %u\n", unitCapacity);
		unitList = (Unit**)realloc(unitList, sizeof(Unit*)*unitCapacity);
	}
	unitList[unitCount] = this;
	listIdx = unitCount;
	unitCount++;
	memset(userSelect, 0, MAXUSERS);
	reserveIMap();
	act();
}
Unit::~Unit(){
	freeIMap();
	for(int uIdx = 0; uIdx < MAXUSERS; uIdx++){
		if(userSelect[uIdx]){
			userList[uIdx]->removeSelection(userSelIdx[uIdx]);
		}
	}
	unitList[listIdx] = unitList[unitCount-1];
	unitCount--;
	unitList[listIdx]->listIdx = listIdx;//set the moved in one's idx to its new idx;
}
void Unit::attack(Unit* targ){
	if(attackTimer <= 0){
		attackTimer = attackCooldown;
		status = 1;
		targ->status = 2;
		targ->hp-=damage;
		if(inCombat < attackCooldown+1) inCombat = attackCooldown+1;
		if(targ->inCombat < attackCooldown+1) targ->inCombat = attackCooldown+1;
	}
}
void Unit::act(){
	if(attackTimer > 0){
		attackTimer-=actCooldown;
	}
	status = 0;
	int bestDist;
	Unit* enemy;
	bool seenEnemy = false;
	if(!ignoreEnemies){//CHOOSE ENEMY
		int priority = 0;//at level 0 any event takes priority
		for(int unitIdx = 0; unitIdx < unitCount; unitIdx++){//FIXME optimize
			Unit* targ = unitList[unitIdx];
			int dist = unitDist(targ);
			if(!canSee(targ->loc)) continue;//cannot see it
			if(attackMask & targ->team){//if its an enemy
				if(priority < 2){
					priority = 2;
					bestDist = dist;
					seenEnemy = true;
					dest = targ->loc;
					enemy = targ;
				}else if(dist < bestDist){
					bestDist = dist;
					dest = targ->loc;
					enemy = targ;
				}	
			}else if((team ^ targ->team)== 0 && targ->inCombat){//if its your team//Note is not a simple else bc we dont want to do anything if its just a neutral
				if(priority > 1) continue;
				if(priority < 1){
					priority = 1;
					bestDist = dist;
					dest = targ->loc;
				}else if(dist < bestDist){ 
					bestDist = dist;
					dest = targ->loc;
				}
			}
		}
	}
	if(seenEnemy &&  range >= bestDist){
		attack(enemy);
	}else{//MOVE
		//move in best direction
		if(loc != dest){
			freeIMap();//FIXME should be more dynamic to save memory time
			move(pathFindDir());
			reserveIMap();
		}
	}
}
void Unit::reserveIMap(){
	int x = loc%mx;
	int y = loc/mx;
	for(int dy = y-radius; dy <= y+radius; dy++){
		for(int dx = x-radius; dx <= x+radius; dx++){
			iMap[dx+dy*mx] = 1;
		}
	}
}
void Unit::freeIMap(){
	int x = loc%mx;
	int y = loc/mx;
	for(int dy = y-radius; dy <= y+radius; dy++){
		for(int dx = x-radius; dx <= x+radius; dx++){
			iMap[dx+dy*mx] = 0;
		}
	}
}
bool Unit::canSee(int l){
	if(toPosDist(l) > visionDist) return false;
	if(visBlock(l, loc)) return false;
	return true;
}
int Unit::toPosDist(int t){
	int x = loc%mx;//FIXME contract
	int y = loc/mx;
	int dx = abs(t%mx-x);
	int dy = abs(t/mx-y);
	if(dx > dy) return dx;
	return dy;
}
int Unit::pathFindDir(){
	double bestFitness = distance(loc, dest);//FIXME use distance() here and below in dirFitness
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
	if(x < 0 || y < 0 || x >= mx || y >= my){
		return -1;
	}
	if(!validLoc(x+y*mx)){
		return -1;
	}
	return distance(x+y*mx, dest);
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
	//FIXME change nodes
}

bool Unit::validLoc(int l){
	int x = l%mx;
	int y = l/mx;
	for(int dx = x-radius; dx <= x+radius; dx++){
		for(int dy = y-radius; dy <= y+radius; dy++){
			if(hMap[dx+dy*mx] != 0 || iMap[dx+dy*mx] != 0) return false;
		}
	}
	return true;
}
int Unit::unitDist(Unit* targ){
	int x = loc%mx;
	int y = loc/mx;
	int dx = abs(targ->loc%mx-x)-targ->radius-radius;
	int dy = abs(targ->loc/mx-y)-targ->radius-radius;
	if(dx < 0 && dy < 0) return 0;//return 0 for overlapping flying units
	if(dx > dy) return dx;
	return dy;
}
double distance(int l1, int l2){
	int x1 = l1%mx;
	int y1 = l1/mx;
	int dx = l2%mx-x1;
	int dy = l2/mx-y1;
	return sqrt(dx*dx+dy*dy);
}
bool visBlock(int l1, int l2){
	int x1 = l1%mx;
	int y1 = l1/mx;
	int x2 = l2%mx;
	int y2 = l2/mx;
		if(abs(x2-x1) < abs(y2-y1)){//rotate plane
		int temp = x1;
		x1 = -y1;
		y1 = temp;
		
		temp = x2;
		x2 = -y2;
		y2 = temp;

		if(x1 > x2){//line is going backwards; switch points
			temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		double slope = (double)(y2-y1)/(double)(x2-x1);
		int sign = 1;
		if(slope < 0){
			sign = -1;
		}
		double err = 0;
		for(;x1 <= x2; x1++){
		if(hMap[y1-x1*mx] == 1) return true;
			err+=slope;
			if(abs(err)>=1){
				y1+=sign;
				err-=sign;
			}
		}
	}else{
		if(x1 > x2){//line is going backwards; switch points
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		double slope = (double)(y2-y1)/(double)(x2-x1);
		int sign = 1;
		if(slope < 0){
			sign = -1;
		}
		double err = 0;
		for(;x1 <= x2; x1++){
			if(hMap[x1+y1*mx] == 1) return true;
			err+=slope;
			if(abs(err)>=1){
				y1+=sign;
				err-=sign;
			}
		}
	}
	return false;
}
