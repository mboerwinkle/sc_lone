#ifndef UNIT_H
#define UNIT_H
#include "netDef.h"
#include "map.h"
typedef struct unitType{
	char name[80];
	char type;
	int actCooldown;
	int attackCooldown;
	int damage;
	int range;
	int maxHp;
	int visionDist;
	int radius;
}unitType;
extern unitType* unitTypes;
extern int unitTypeCount;
extern void loadUnitTypes();
extern unsigned int teamVals[16];
class Unit{
public:
	Unit(int typeIdx, int team, int loc, char status, int ownerIdx);
	~Unit();
	void act();
	void attack(Unit* target);
	void move(int dir);
	int pathFindDir();
	bool canSee(int l);
	bool validLoc(int l);
	double dirFitness(int dir);
	double unitDist(Unit* targ);
	double toPosDist(int myLoc, int t);
	int actTimer;//FIXME make actCooldown dependent on gcf of moveCooldown and attackCooldown
	int actCooldown;
	char radius;//fixme store position as x and y for everything. only transmit loc
	int attackTimer;
	int attackCooldown;
	int damage;
	int range;
	int hp;
	int maxHp;
	bool inCombat = 0;
	bool ignoreEnemies = 0;//for moving, etc
//	bool holdingPosition = 0;//FIXME implement.
	int visionDist;
	int listIdx;
	char type;
	unsigned int team;
	unsigned int attackMask;
	int loc;
	int ownerIdx;
	int dest;//FIXME be sure of proper init of all vars
	char status;
	char userSelect[MAXUSERS];
	int userSelIdx[MAXUSERS];
};
extern double distance(int l1, int l2);
extern int unitCount;
extern Unit** unitList;
extern int unitCapacity;
#endif
