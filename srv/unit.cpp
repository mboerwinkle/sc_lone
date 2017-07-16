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
void Unit::attack(Unit* targ){
	if(attackTimer <= 0){
		attackTimer = attackCooldown;
		status = 1;
		targ->status = 2;
		targ->hp-=damage;
		inCombat = 1;
		targ->inCombat = 1;
		if(!targ->ignoreEnemies){//And destination is not on a current other enemy that I can see (which would be one of "this"'s allies)
			targ->dest = loc;
		}
	}
}
void Unit::act(){
//change dest if seeing enemy.
	if(attackTimer > 0){
		attackTimer-=actCooldown;
	}
	inCombat = 0;
	bool seenEnemy = false;
	double enemyDist = 0;//FIXME make it be the distance between destination and each enemy. that way if destination is on an enemy it will attack that enemy.
	Unit* enemy;
	if(!ignoreEnemies){//CHOOSE ENEMY
		for(int unitIdx = 0; unitIdx < unitCount; unitIdx++){//FIXME optimize
			Unit* targ = unitList[unitIdx];
			double dist = unitDist(targ);
			if(!canSee(targ->loc)) continue;//FIXME refactor//cannot see it
			if((dist > enemyDist) && seenEnemy) continue;//is not the best option
			if((attackMask & targ->team) == 0){
				if(targ->inCombat == 1 && !ignoreEnemies){
					dest = targ->loc;//FIXME should it be targ->dest???//move to an attacked friend unless you see an enemy.
				}
				continue;//is a friend
			}
			seenEnemy = true;
			enemyDist = dist;
			enemy = targ;//Finding enemy
		}
		if(seenEnemy) dest = enemy->loc;
	}
	status = 0;
	if(seenEnemy && (enemyDist <= range)){
		attack(enemy);
	}else{//MOVE
		//move in best direction
		if(loc != dest){
			move(pathFindDir());
		}
	}
}
bool Unit::canSee(int l){
	if(toPosDist(loc, l) > range) return false;
	if(visBlock(l, loc)) return false;
	return true;
}
double Unit::toPosDist(int myLoc, int t){
	int x = myLoc%mx;//FIXME contract
	int y = myLoc/mx;
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
			//FIXME get list of units near
			//see if intersects.
		}
	}
	return true;
}
double Unit::unitDist(Unit* targ){
	int x = loc%mx;
	int y = loc/mx;
	int dx = abs(targ->loc%mx-x)-targ->radius;
	int dy = abs(targ->loc/mx-y)-targ->radius;
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
		//FIXME only based on height map	if(bMap[y1-x1*mx] == -1) return true;
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
		//	if(bMap[x1+y1*mx] == -1) return true;
			err+=slope;
			if(abs(err)>=1){
				y1+=sign;
				err-=sign;
			}
		}
	}
	return false;
}
