#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
void startNCurses(){
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
}
