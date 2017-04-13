#ifndef UNIT_H
#define UNIT_H
#include "netDef.h"
#include "map.h"
class Unit{
public:
	Unit(char type, char team, int loc, char status);
	~Unit();
	void act();
	void move(int dir);
	int pathFindDir();
	int validLoc(int l);//bool return FIXME
	double dirFitness(int dir);
	int actTimer;
	int actCooldown = 3;
	char size = 3;//fixme store position as x and y for everything. only transmit loc
	//int attackTimer;
	//int attackCooldown;
//	int hp;
//	int maxHp;
//	int shield;
//	int maxShield;
	int visionDist = 10;
	int listIdx;
	char type;
	char team;
	int loc;
	int dest;
	char status;
	char attack;//as opposed to move. The only difference between passive and static entities is their resting state.//FIXME be sure all vars are init correctly.
	char attackRestingState = 0;
	char userSelect[MAXUSERS];
	int userSelIdx[MAXUSERS];
};
extern double distance(int l1, int l2);
extern int unitCount;
extern Unit** unitList;
extern int unitCapacity;
#endif
