#ifndef QUEUE_H
#define QUEUE_H

#include "webserverThreads.h"

int enqueue(int value, QUEUE *q);
int dequeue(QUEUE *q);

#endif
