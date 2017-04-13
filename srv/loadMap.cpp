#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* map = NULL;
int mx, my;
char* bMap = NULL;//ground unit forbidden aream map.
//char* hMap = NULL;//Height map FIXME
void loadMap(char* name){
	FILE* mapfile = fopen(name, "r");
	fscanf(mapfile, "%d %d\n", &mx, &my);
	free(map);
	map = (char*)calloc(mx*my, sizeof(char));
	bMap = (char*)calloc(mx*my, sizeof(char));//FIXME preadd blocked regions (water, wall, etc);
	fread(map, sizeof(char), mx*my, mapfile);
}
