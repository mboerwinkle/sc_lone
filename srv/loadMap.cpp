#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* map = NULL;
char* hMap = NULL;
char* iMap = NULL;
int mx, my;
void loadMap(char* name){
	FILE* mapfile = fopen(name, "r");
	fscanf(mapfile, "%d %d\n", &mx, &my);
	free(map);
	map = (char*)calloc(mx*my, sizeof(char));
	hMap = (char*)calloc(mx*my, sizeof(char));
	iMap = (char*)calloc(mx*my, sizeof(char));
	fread(map, sizeof(char), mx*my, mapfile);
	for(int loc = 0; loc < mx*my; loc++){
		if(map[loc] == 'X'){
			hMap[loc] = 1;
		}
		if(map[loc] == 'W'){
			hMap[loc] = -1;
		}
	}
}
