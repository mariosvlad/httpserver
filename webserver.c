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
//messages
char ok[16] = "HTTP/1.1 200 OK\r\n";
char notFound[23] = "HTTP/1.1 404 NOT FOUND\r\n";
char notImpl[29] = "HTTP/1.1 501 NOT IMPLEMENTED\r\n";

char* giveContentType(char *arxeio){
	char temp[18];
	char* split;
	char* word;
	split=strtok(arxeio,".");
	while(split != NULL){
		word=split;
		split=strtok(NULL,".");
	}
	printf("Type: %s\n" word);
	if ((strcmp(word,"txt")==0)||(strcmp(word,"sed")==0)||(strcmp(word,"awk")==0)||(strcmp(word,'c')==0)||(strcmp(word,'h')==0)){
		temp="text/plain";
	}else{
	if ((strcmp(word,"html")==0)||(strcmp(word,"htm")==0)){
		temp="text/html";
	}else{
		if ((strcmp(word,"jpeg")==0)||(strcmp(word,"jpg")==0)){
		temp= "image/jpeg";
		}else{
			if (strcmp(word,"gif")==0){
			temp="image/gif";
			}else{
				if (strcmp(word,"pdf")==0){
				temp="application/pdf";
				}
			}	

		}
	}
	}
	char* str=malloc(18);
	strcpy(str, temp);
}
void getRequest(char* arxeio){
	FILE *fp;
	char *line = NULL;
	fp = fopen(arxeio, "r");
	if (fp==NULL){
		write("%s\n", notFound);
	}else{
		size_t len = 0;
		ssize_t read;
		while ((read = getline(&line, &len, fp)) != -1) {
		write(read);
		}
		write("Server: Marios and Evanthia Server\r\n");
		fclose(output_file);
		fp = fopen(arxeio, "r");
		char nextChar = getc(fp);
		int numCharacters = 0;

		while (nextChar != EOF) {
		    //Do something else, like collect statistics
		    numCharacters++;
		    nextChar = getc(fp);
		}
		write("Content Length:");
		write(numCharacters);
		write("\r\n");
		write("Content Type:");
		char* tipos= giveContentType(arxeio);
		write(tipos);
		write("\r\n");
	}
}
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
	while (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) != 0){
		printf("try bind\n");
	}
	printf("Binding Socket\n");
	while (1) {
		if (listen(create_socket, 10) < 0) {
			perror("server: listen");
			exit(1);
		}
		if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0){
			perror("server: accept");
			exit(1);
		}
		if (new_socket > 0){
			printf("The Client is connected...\n");
		}
		recv(new_socket, buffer, bufsize, 0);
		printf("%s\n", buffer);
		printf("%s\n","Tipwsa ti pira");
		char request[1024];
		strcpy(request, buffer);
		char* split;
		char* word;
		char* requestFile;
		char* connectionType;
		split=strtok(request," ");
		while(split != NULL){
			word=split;
			printf("%s\n", word);
		if ((strcmp(split,"GET")==0)||(strcmp(split,"HEAD")==0)||(strcmp(split,"DELETE")==0)){
			split=strtok(NULL," ");
			requestFile=split;
			printf("Requested File: %s\n", requestFile);
			}
			split=strtok(NULL," ");
		}
		printf("%s\n", request);
		if (strcmp(request,"GET")==0){
			printf("The Request was GET\n");
		}
		if (strcmp(request,"HEAD")==0){
			printf("The Request was HEAD\n");
		}
		if (strcmp(request,"DELETE")==0){
			printf("The Request was DELETE\n");
		}
		write(new_socket, "hello world\n", 12);
		sleep(2);
		printf("%s\n","Tipwsa ti pira");
		close(new_socket);
	}
	close(create_socket);
	return 0;
}
