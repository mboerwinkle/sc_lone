#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* map = NULL;
int mx, my;
short* bMap = NULL;//ground unit forbidden aream map.
void loadMap(char* name){
	FILE* mapfile = fopen(name, "r");
	fscanf(mapfile, "%d %d\n", &mx, &my);
	free(map);
	map = (char*)calloc(mx*my, sizeof(char));
	bMap = (short*)calloc(mx*my, sizeof(short));
	fread(map, sizeof(char), mx*my, mapfile);
	for(int loc = 0; loc < mx*my; loc++){
		if(map[loc] == 'X'){
			bMap[loc] = -1;
		}
	}
}
