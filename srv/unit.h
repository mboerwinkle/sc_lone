#ifndef UNIT_H
#define UNIT_H
#include "netDef.h"
class Unit{
public:
	Unit(char type, char team, int loc, char status);
	~Unit();
	int listIdx;
	char type;
	char team;
	int loc;
	char status;
	char userSelect[MAXUSERS];
	int userSelIdx[MAXUSERS];
};
extern int unitCount;
extern Unit** unitList;
extern int unitCapacity;
#endif
