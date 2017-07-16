#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unit.h"
//FIXME this whole file needs to be cleaned
unitType* unitTypes;
int unitTypeCount;
void loadUnit(FILE* unitFile, unitType* dest);
void loadUnitTypes(){
	FILE* contents = fopen("../units/contents.txt", "r");
	if(contents == NULL) puts("error loading units.");
	fscanf(contents, "%d", &unitTypeCount);
	unitTypes = (unitType*)calloc(unitTypeCount, sizeof(unitType));
	char unitFileName[80];
	FILE* unitFile = NULL;
	char contentName[80];
	for(int unitIdx = 0; unitIdx < unitTypeCount; unitIdx++){
		if(EOF == fscanf(contents, "%s", contentName)) break;
		sprintf(unitFileName, "../units/%s", contentName);
		unitFile = fopen(unitFileName, "r");
		if(unitFile == NULL) break;
		loadUnit(unitFile, &(unitTypes[unitIdx]));
		fclose(unitFile);
		unitFile = NULL;
	}
	fclose(contents);
}
void getNextData(FILE* unitFile, char* data){
	do{
		fscanf(unitFile, "%s", data);
	}while(data[0] == '#');
}
void loadUnit(FILE* unitFile, unitType* dest){
	char line[80];
	getNextData(unitFile, line);//name
	printf("loading %s\n", line);
	strcpy(dest->name, line);
	getNextData(unitFile, line);//type
	dest->type = line[0];
	getNextData(unitFile, line);//
	dest->actCooldown = atoi(line);
	getNextData(unitFile, line);//
	dest->attackCooldown = atoi(line);
	getNextData(unitFile, line);//
	dest->damage = atoi(line);
	getNextData(unitFile, line);//
	dest->range = atoi(line);
	getNextData(unitFile, line);//
	dest->maxHp = atoi(line);
	getNextData(unitFile, line);//
	dest->visionDist = atoi(line);
	getNextData(unitFile, line);//
	dest->radius = atoi(line);	
}
