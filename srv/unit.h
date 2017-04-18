#ifndef UNIT_H
#define UNIT_H
#include "netDef.h"
#include "map.h"
extern unsigned int teamVals[16];
class Unit{
public:
	Unit(char type, int team, int loc, char status);
	~Unit();
	void act();
	void blockLocation();
	void unBlockLocation();
	void attack(Unit* target);
	void move(int dir);
	int pathFindDir();
	int validLoc(int l);//bool return FIXME
	double dirFitness(int dir);
	double unitDist(Unit* targ);
	int actTimer = 3;//FIXME make actCooldown dependent on gcf of moveCooldown and attackCooldown
	int actCooldown = 3;
	char size = 1;//fixme store position as x and y for everything. only transmit loc
	int attackTimer = 12;
	int attackCooldown = 12;
	int damage = 10;
	int range = 6;
	int hp = 100;
	int maxHp = 100;
	int inCombat = 0;
	int ignoreEnemies = 0;//for moving, etc
//	int holdingPosition = 0;//FIXME implement.
	int visionDist = 10;//FIXME make booleans booleans and not ints.
	int listIdx;
	char type;
	unsigned int team;
	unsigned int attackMask;
	int loc;
	User* owner;//use
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
