#include <netinet/in.h>    
#include <stdio.h>    
#include <stdlib.h>    
#include <sys/socket.h>    
#include <sys/stat.h>    
#include <sys/types.h>    
#include <unistd.h>    
#include "webserver.h"

int WORKERS;
int PORT; 
void readConfig() {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("config.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    char tmp[8];

    while ((read = getline(&line, &len, fp)) != -1) {
        if (strstr(line, "THREADS")) {
            memcpy(tmp, &line[8], 7);
            tmp[7] = '\0';
            WORKERS = atoi(tmp);
            printf("WORKERS : %d \n", WORKERS);
        }
        else if (strstr(line, "PORT")) {
            memcpy(tmp, &line[5], 7);
            tmp[7] = '\0';
            PORT = atoi(tmp);
            printf("PORT : %d \n", PORT);
        }
    }

    fclose(fp);
    if (line)
        free(line);
}

int main() {    
   readConfig();
   int create_socket, new_socket;    
   socklen_t addrlen;    
   int bufsize = 1024;    
   char *buffer = malloc(bufsize);    
   struct sockaddr_in address;    
 
   if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0){    
      printf("The socket was created\n");
   }
    
   address.sin_family = AF_INET;    
   address.sin_addr.s_addr = INADDR_ANY;    
   address.sin_port = htons(PORT);    
    
   if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == 0){    
      printf("Binding Socket\n");
   }
    
    
   while (1) {    
      if (listen(create_socket, 10) < 0) {    
         perror("server: listen");    
         exit(1);    
      }    
    
      if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {    
         perror("server: accept");    
         exit(1);    
      }    
    
      if (new_socket > 0){    
         printf("The Client is connected...\n");
      }
        
      recv(new_socket, buffer, bufsize, 0);    
      printf("%s\n", buffer);    
      write(new_socket, "hello world\n", 12);    
      close(new_socket);    
   }    
   close(create_socket);    
   return 0;    
}