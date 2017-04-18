#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "unit.h"
unsigned int teamVals[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
int unitCount = 0;
int unitCapacity = 0;
Unit** unitList = NULL;
Unit::Unit(char type, int team, int loc, char status){
	this->type = type;//FIXME config files...
	this->team = teamVals[team];
	this->loc = loc;
	this->dest = loc;
	this->status = status;
	printf("team is %u\n", team);
	this->attackMask = ~(this->team);
	printf("attackMask is %u\n", attackMask);
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
	unBlockLocation();
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
	inCombat = 0;
	bool seenEnemy = false;
	double enemyDist = 0;//FIXME make it be the distance between destination and each enemy. that way if destination is on an enemy it will attack that enemy.
	Unit* enemy;
	if(!ignoreEnemies){//CHOOSE ENEMY
		for(int unitIdx = 0; unitIdx < unitCount; unitIdx++){//FIXME optimize
			Unit* targ = unitList[unitIdx];
			if((attackMask & targ->team) == 0){
				if(targ->inCombat == 1 && !ignoreEnemies){
					dest = targ->loc;//move to an attacked friend unless you see an enemy.
				}
				continue;//is a friend
			}
			double dist = unitDist(targ);
			if(dist > visionDist) continue;//cannot see it
			if((dist > enemyDist) && seenEnemy) continue;//is not the best option
			seenEnemy = true;
			enemyDist = dist;
			enemy = targ;//Finding enemy
		}
		if(seenEnemy) dest = enemy->loc;
		if(attackTimer > 0){
			attackTimer-=actCooldown;
		}
	}
	status = 0;
	if(seenEnemy && (enemyDist <= range)){
		attack(enemy);
	}else{//MOVE
		unBlockLocation();
		//move in best direction
		if(loc != dest){
			move(pathFindDir());
		}
		blockLocation();
	}
}
void Unit::blockLocation(){
		//re-add yourself to blocking map
		int x = loc%mx;
		int y = loc/mx;
		for(int dx = x; dx < x+size; dx++){
			for(int dy = y; dy < y+size; dy++){
				bMap[dx+mx*dy] = 1;
			}
		}
}
void Unit::unBlockLocation(){
		//remove yourself from the blocking map.
		int x = loc%mx;
		int y = loc/mx;
		for(int dx = x; dx < x+size; dx++){
			for(int dy = y; dy < y+size; dy++){
				bMap[dx+mx*dy] = 0;
			}
		}
}
int Unit::pathFindDir(){
	double bestFitness = distance(loc, dest);//replace with a function member of unit one argument loc that takes into account size//FIXME
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
double Unit::unitDist(Unit* targ){
	double x = loc%mx+(size/2);
	double y = loc/mx+(size/2);
	double dx = targ->loc%mx+(targ->size/2)-x;
	double dy = targ->loc/mx+(targ->size/2)-y;
	return sqrt(dx*dx+dy*dy);
}
double distance(int l1, int l2){
	int x1 = l1%mx;
	int y1 = l1/mx;
	int dx = l2%mx-x1;
	int dy = l2/mx-y1;
	return sqrt(dx*dx+dy*dy);
}
