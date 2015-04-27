#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "webserver.h"
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>

int WORKERS;
int PORT;
//int create_socket, new_socket;
socklen_t addrlen;
struct sockaddr_in address;
//messages
char ok[] = "HTTP/1.1 200 OK\r\n";
char notFound[] = "HTTP/1.1 404 NOT FOUND\r\n";
char notImpl[] = "HTTP/1.1 501 NOT IMPLEMENTED\r\n";

char *giveContentType(char *file) {
    char temp[18];
    char *split;
    char *word;
    split = strtok(file, ".");
    while (split != NULL) {
        word = split;
        split = strtok(NULL, ".");
    }

    printf("Type: %s\n", word);
    if ((strcmp(word, "txt") == 0) || (strcmp(word, "sed") == 0) || (strcmp(word, "awk") == 0)) {
        strcpy(temp, "text/plain\0");
        printf("First if");
    }

    if ((strcmp(word, "h") == 0) || (strcmp(word, "c") == 0)) {
        strcpy(temp, "text/plain\0");
        printf(".h");
    }

    printf("After first if");
    if ((strcmp(word, "html") == 0) || (strcmp(word, "htm") == 0)) {
        printf("Vrike ton tipo\n");
        temp[0] = '\0';
        strcpy(temp, "text/html\0");
    }

    if ((strcmp(word, "jpeg") == 0) || (strcmp(word, "jpg") == 0)) {
        strcpy(temp, "image/jpeg\0");
    }

    if (strcmp(word, "gif") == 0) {
        strcpy(temp, "image/gif\0");
    }

    if (strcmp(word, "pdf") == 0) {
        strcpy(temp, "application/pdf\0");
    }

    char *str = malloc(18);
    strcpy(str, temp);
    return str;
}

void getRequest(char *file, int new_socket) {
    //struct stat file_stat;
    char *contents_chopped = file + 1;
    printf("irthe na parei to file\n");
    FILE *fp;
    long lSize;
    char *buffer;
    //int fd;
    //int sent_bytes = 0;
    printf("%s\n", contents_chopped);
    fp = fopen(contents_chopped, "rb");

    if (!fp) {
        printf("den vrike file");
        if (write(new_socket, notFound, 23) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        return;
    }
    else {
        fseek(fp, 0L, SEEK_END);
        lSize = ftell(fp);
        rewind(fp);
        // allocate memory for entire content
        buffer = calloc(1, lSize + 1);
        if (!buffer) {
            fclose(fp);
            fputs("memory alloc fails", stderr);
            exit(1);
        }

        // copy the file into the buffer
        if (1 != fread(buffer, lSize, 1, fp)) {
            fclose(fp);
            free(buffer);
            fputs("entire read fails", stderr), exit(1);
        }
        fclose(fp);
        char len[11];

        sprintf(len, "%ld", lSize);
        if (write(new_socket, ok, 16) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "Server: Marios and Evanthia Server\r\n", 36) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "Content-Length:", 15) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, len, strlen(len)) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "\r\n", 2) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        if (write(new_socket, "Connection: keep-alive\r\n", 24) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        }; //to evala etsi proswrina :)
        char *tipos = giveContentType(file);
        printf("Hello Pire Tipo\n");
        printf("%s\n", tipos);
        printf("%s\n", len);
        if (write(new_socket, "Content-Type: ", 14) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        if (write(new_socket, tipos, strlen(tipos)) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        if (write(new_socket, "\r\n", 2) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        if (write(new_socket, "\r\n", 2) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        printf("tipos ok\n");
        if (write(new_socket, buffer, lSize) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        free(buffer);
    }
}

void readConfig() {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen("config.txt", "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

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
    if (line) {
        free(line);
    }

}

int main() {
    readConfig();
    int create_socket, new_socket;
    //socklen_t addrlen;
    int bufsize = 1024;
    char *buffer = malloc(bufsize);
    //struct sockaddr_in address;
    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0) {
        printf("The socket was created\n");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    while (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) != 0) {
        printf("try bind\n");
    }

    printf("Binding Socket\n");
    while (1) {
        if (listen(create_socket, 10) < 0) {
            perror("server: listen");
            exit(1);
        }

        if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {
            perror("server: accept");
            exit(1);
        }

        if (new_socket > 0) {
            printf("The Client is connected...\n");
        }

        recv(new_socket, buffer, bufsize, 0);
        printf("%s\n", buffer);
        printf("%s\n", "Tipwsa ti pira");
        char request[1024];
        strcpy(request, buffer);
        char *split;
        //char *word;
        char *requestFile = NULL;
        //char *connectionType;
        split = strtok(request, " ");

        while (split != NULL) {
            //word = split;
            //printf("%s\n", word);
            if ((strcmp(split, "GET") == 0) || (strcmp(split, "HEAD") == 0) || (strcmp(split, "DELETE") == 0)) {
                split = strtok(NULL, " ");
                requestFile = split;
                printf("Requested File: %s\n", requestFile);
            }
            split = strtok(NULL, " ");
        }
        getRequest(requestFile, new_socket);
        printf("%s\n", request);

        if (strcmp(request, "GET") == 0) {
            printf("The Request was GET\n");
        }

        if (strcmp(request, "HEAD") == 0) {
            printf("The Request was HEAD\n");
        }

        if (strcmp(request, "DELETE") == 0) {
            printf("The Request was DELETE\n");
        }

        printf("%s\n", "Tipwsa ti pira");
        close(new_socket);
    }
    close(create_socket);
    return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> db089619e9211f6eeffeef5a38ffbf18e39be712
