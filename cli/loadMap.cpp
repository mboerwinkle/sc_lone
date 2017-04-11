#include <stdio.h>
#include <stdlib.h>
char* map = NULL;
int mx, my;
void loadMap(char* name){
	FILE* mapfile = fopen(name, "r");
	fscanf(mapfile, "%d %d\n", &mx, &my);
	free(map);
	map = (char*)calloc(mx*my, sizeof(char));
	fread(map, sizeof(char), mx*my, mapfile);
}
