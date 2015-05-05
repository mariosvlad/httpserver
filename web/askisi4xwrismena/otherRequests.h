#ifndef OTHERREQUESTS_H
#define OTHERREQUESTS_H

#include "webserverThreads.h"

void sendDelete(char *file, int new_socket, int typeCon);
void sendNotImpl(int new_socket, int typeCon);
void notfound(int new_socket, int typeCon);




#endif
