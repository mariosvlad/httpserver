#ifndef WEBSERVERTHREADS_H
#define WEBSERVERTHREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


typedef struct node {
    int current_socket;
    struct node *next;
} NODE;

typedef struct {
    NODE *head; //pointer to the first item
    NODE *tail; //pointer to the last item
    int length;
} QUEUE;
extern int WORKERS;
extern int PORT;
extern int bufsize;
extern int busy;
extern int new_socket2;
//int create_socket, new_socket;
//socklen_t addrlen;
//struct sockaddr_in address;


extern pthread_mutex_t mtx;
extern pthread_mutex_t mt;

//messages
extern char ok[];
extern char notFound[];
extern char notImpl[] ;
extern pthread_mutex_t mut;
extern pthread_cond_t con;



extern QUEUE *queue ;



#endif //HTTPSERVER_WEBSERVER_H
