#ifndef NETDEF_H
#define NETDEF_H
#define MSGSIZE 3000
#define MAXUSERS 8
#define SELECTIONLIMIT 1000//FIXME make dynamic
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
class Unit;
extern int sockfd;

extern void netParse(int userIdx, char* msg);
extern void sendMessage(struct sockaddr_in *addr, char* data, int datalen);

class User{//FIXME put in user.h
public:
	User(unsigned long ip);
	struct sockaddr_in addr;
	int loc = 0;
	int vx = 40, vy = 40;
	int listIdx;//FIXME go back and check for dupes.
	void sendUserData();
	void sendMapName();
	Unit* selection[SELECTIONLIMIT];
	int selectionCount = 0;
	void moveCamera(int axis, int amount);
	void getCamCoord(int* x, int* y);
	void setCamCoord(int x, int y);
	void select(int c1, int c2);
	void removeSelection(int idx);
	void clearSelection();
	void killSelectedUnits();
	void selectUnit(int idx);
	void selAttackLoc(int tloc);
	int canSee(int tloc);
	int canSeeUnit(Unit* test);
private:
};
extern User* userList[MAXUSERS];
extern int userCount;
#endif
