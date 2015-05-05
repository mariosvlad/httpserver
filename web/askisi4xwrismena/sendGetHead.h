#ifndef SENDGETHEAD_H
#define SENDGETHEAD_H
#include "webserverThreads.h"




void getRequest(char *file, int new_socket, int typeCon);
void sendHead(char *file, int new_socket, int typeCon);
#endif
