#ifndef TREADS_H
#define TREADS_H

#include "webserverThreads.h"

void *takeThisRequest(char *buffer, int new_socket, int typeCon);
int handleThreads(int new_socket);
void *handle();


#endif
