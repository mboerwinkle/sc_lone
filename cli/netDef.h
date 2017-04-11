#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MSGSIZE 3000
extern int sockfd;
//extern void netParse(char* msg, int len);
extern void handleNetwork();
extern void startNetwork(char* ip, int port);
extern struct sockaddr_in recvAddr, sendAddr;
