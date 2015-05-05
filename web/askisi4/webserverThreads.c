#include "webserverThreads.h"

int WORKERS;
int PORT;
int bufsize = 1024;
int busy = 0;
int new_socket2;
//int create_socket, new_socket;
socklen_t addrlen;
struct sockaddr_in address;

void *takeThisRequest(char *, int, int);

void *handle();
pthread_mutex_t mtx;
pthread_mutex_t mt;

//messages
char ok[] = "HTTP/1.1 200 OK\r\n";
char notFound[] = "HTTP/1.1 404 NOT FOUND\r\n";
char notImpl[] = "HTTP/1.1 501 NOT IMPLEMENTED\r\n";
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t con = PTHREAD_COND_INITIALIZER;

typedef struct node {
    int current_socket;
    struct node *next;
} NODE;

typedef struct {
    NODE *head; //pointer to the first item
    NODE *tail; //pointer to the last item
    int length;
} QUEUE;
QUEUE *queue = NULL;


char *giveContentType(char *file) {
    char temp[18];
    char *split;
    char *word;
    split = strtok(file, ".");
    while (split != NULL) {
        word = split;
        split = strtok(NULL, ".");
    }

    if ((strcmp(word, "txt") == 0) || (strcmp(word, "sed") == 0) || (strcmp(word, "awk") == 0)) {
        strcpy(temp, "text/plain\0");

    }
    if ((strcmp(word, "h") == 0) || (strcmp(word, "c") == 0)) {
        strcpy(temp, "text/plain\0");

    }

    if ((strcmp(word, "html") == 0) || (strcmp(word, "htm") == 0)) {

        temp[0] = '\0';
        strcpy(temp, "text/html\0");

    }
    if ((strcmp(word, "jpeg") == 0) || (strcmp(word, "jpg") == 0)||(strcmp(word, "png") == 0)) {
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

void getRequest(char *file, int new_socket, int typeCon) {
    struct stat file_stat;
    char *contents_chopped = file + 1;
    //printf("irthe na parei to file\n");
    FILE *fp;
    long lSize;
    char *buffer;
    int fd;
    int sent_bytes = 0;
#ifdef DEBUG
    printf("%s\n", contents_chopped);
#endif
    fp = fopen(contents_chopped, "rb");
    if (!fp) {
        printf("den vrike file");
            notfound(new_socket, typeCon);
            return;

    }
    else {

        fseek(fp, 0L, SEEK_END);
        lSize = ftell(fp);
        rewind(fp);

        buffer = calloc(1, lSize + 1);
        if (!buffer) {
            fclose(fp), fputs("memory alloc fails", stderr), exit(1);
        }


        if (1 != fread(buffer, lSize, 1, fp)) {
            fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);
        }
        fclose(fp);

        char len[11];
        sprintf(len, "%ld", lSize);
        if (write(new_socket, ok, 17) < 0) {
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
        char cnc[25];
        printf("Epestrepse to arxeio: %s\n" ,contents_chopped );

        if (typeCon == 1) {
            strcpy(cnc, "Connection: keep-alive\r\n");

        } else {
            strcpy(cnc, "Connection: close\r\n");
        }

        if (write(new_socket, cnc, strlen(cnc)) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

            
        char *tipos = giveContentType(file);
#ifdef DEBUG
        printf("Hello Pire Tipo\n");
        printf("%s\n", tipos);
        printf("%s\n", len);
#endif
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

        if (write(new_socket, buffer, lSize) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        free(buffer);
    }
    return;
}
void sendHead(char *file, int new_socket, int typeCon){
 struct stat file_stat;
    char *contents_chopped = file + 1;

    FILE *fp;
    long lSize;
    char *buffer;
    int fd;
    int sent_bytes = 0;
#ifdef DEBUG
    printf("%s\n", contents_chopped);
#endif
    fp = fopen(contents_chopped, "rb");
    if (!fp) {
        printf("den vrike file");
            notfound(new_socket, typeCon);
            return;
    }
    else {

        fseek(fp, 0L, SEEK_END);
        lSize = ftell(fp);
        rewind(fp);

        buffer = calloc(1, lSize + 1);
        if (!buffer) {
            fclose(fp), fputs("memory alloc fails", stderr), exit(1);
        }


        if (1 != fread(buffer, lSize, 1, fp)) {
            fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);
        }
        fclose(fp);

        char len[11];
        sprintf(len, "%ld", lSize);
        if (write(new_socket, ok, 17) < 0) {
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
        char cnc[25];
        printf("Epestrepse to arxeio: %s\n" ,contents_chopped );

        if (typeCon == 1) {
            strcpy(cnc, "Connection: keep-alive\r\n");

        } else {
            strcpy(cnc, "Connection: close\r\n");
        }

        if (write(new_socket, cnc, strlen(cnc)) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        //to evala etsi proswrina :)
            
        char *tipos = giveContentType(file);
#ifdef DEBUG
        printf("Hello Pire Tipo\n");
        printf("%s\n", tipos);
        printf("%s\n", len);
#endif
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
        free(buffer);
    }
    return;
}

void sendDelete(char *file, int new_socket, int typeCon){
   struct stat file_stat;
    char *contents_chopped = file + 1;

    int ret;
    int fd;
    int sent_bytes = 0;
#ifdef DEBUG
    printf("%s\n", contents_chopped);
#endif

        ret = remove(contents_chopped);
        if (ret!=0){
            printf("Den vrethike arxeio. \n");
            notfound(new_socket, typeCon);
            return;
        }
        else{
        char head[13] = "Delete OK\r\n";
        if (write(new_socket, ok, 17) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "Server: Marios and Evanthia Server\r\n", 36) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        if (write(new_socket, "Content-Length: ", 16) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "11", 2) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "\r\n", 2) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        char cnc[25];
        printf("Epestrepse to arxeio: %s\n" ,contents_chopped );

        if (typeCon == 1) {
            strcpy(cnc, "Connection: keep-alive\r\n");

        } else {
            strcpy(cnc, "Connection: close\r\n");
        }

        if (write(new_socket, cnc, strlen(cnc)) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

#ifdef DEBUG
        printf("Hello Pire Tipo\n");
        printf("%s\n", tipos);
        printf("%s\n", len);
#endif
        if (write(new_socket, "Content-Type: text/plain", 24) < 0) {
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

        if (write(new_socket, head, strlen(head)) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
      }
    return;
}

void sendNotImpl(int new_socket, int typeCon){

    int ret;
    int fd;
    int sent_bytes = 0;
#ifdef DEBUG
    printf("%s\n", contents_chopped);
#endif

            printf("Den vrethike arxeio. \n");
        char notim[26]="Method not implemented.\r\n";

        if (write(new_socket, notImpl, 30) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "Server: Marios and Evanthia Server\r\n", 36) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        if (write(new_socket, "Content-Length: ", 16) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "25",2) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "\r\n", 2) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        char cnc[25];

        if (typeCon == 1) {
            strcpy(cnc, "Connection: keep-alive\r\n");

        } else {
            strcpy(cnc, "Connection: close\r\n");
        }

        if (write(new_socket, cnc, strlen(cnc)) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };   
#ifdef DEBUG
        printf("Hello Pire Tipo\n");
        printf("%s\n", tipos);
        printf("%s\n", len);
#endif
        if (write(new_socket, "Content-Type: text/plain", 24) < 0) {
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
        if (write(new_socket, notim, strlen(notim)) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
    return;
}

void notfound(int new_socket, int typeCon){


            printf("Den vrethike arxeio. \n");
        char notim[23]="Document not Found.\r\n";

        if (write(new_socket, notFound, 24) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "Server: Marios and Evanthia Server\r\n", 36) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        if (write(new_socket, "Content-Length: ", 16) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "19", 2) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

        if (write(new_socket, "\r\n", 2) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
        char cnc[25];

        if (typeCon == 1) {
            strcpy(cnc, "Connection: keep-alive\r\n");

        } else {
            strcpy(cnc, "Connection: close\r\n");
        }

        if (write(new_socket, cnc, strlen(cnc)) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };

       
#ifdef DEBUG
        printf("Hello Pire Tipo\n");
        printf("%s\n", tipos);
        printf("%s\n", len);
#endif
        if (write(new_socket, "Content-Type: text/plain", 24) < 0) {
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

        if (write(new_socket, "Document not Found.", 19) < 0) {
            printf("ERROR: couldn't write to socket\n");
            exit(EXIT_FAILURE);
        };
    return;
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

void *takeThisRequest(char *buffer, int new_socket, int typeCon) {

    char request[1024];
    strcpy(request, buffer);
    char *split;
    char *word;
    char *requestFile;
    char *connectionType;
    split = strtok(request, " ");
    char connection[10];
    int flag=0;
    while (split != NULL) {
        word = split;
        if ((strcmp(split, "GET") == 0) || (strcmp(split, "HEAD") == 0) || (strcmp(split, "DELETE") == 0)) {
            split = strtok(NULL, " ");
            flag=1;
            requestFile = split;
            printf("Requested File: %s\n", requestFile);
            if (strcmp(word, "GET") == 0){
                   getRequest(requestFile, new_socket, typeCon);
                    break;     
            }else{
                if (strcmp(word, "HEAD") == 0){
                   printf("paei head\n");
                   sendHead(requestFile, new_socket, typeCon);
                    break;     
                 }else{
                    if (strcmp(word, "DELETE") == 0){
                   sendDelete(requestFile, new_socket, typeCon); 
                    break;    
                 }
                }
            }
        }
        split = strtok(NULL, " ");
    }
    if (flag==0){
        sendNotImpl(new_socket, typeCon);
    }



#ifdef DEBUG
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
#endif
    return;

}


int handleThreads(int new_socket) {

    char *buffer = malloc(bufsize);
    recv(new_socket, buffer, bufsize, 0);
    while (buffer == NULL) {
        recv(new_socket, buffer, bufsize, 0);
    }
#ifdef DEBUG
    printf("%s\n", buffer);
    printf("%s\n", "Tipwsa ti pira");
#endif
    char request[1024];
    int typeCon = -1;
    char *split;
    char *word;
    char *requestFile;
    strcpy(request, buffer);

    char findCon[50];
    char connectionType[11];
    char *sub;
    sub = strstr(request, "Connection:");

    char *sub2 = strstr(sub, "keep-alive");
    char *sub3 = strstr(sub, "close");


    if (sub2 != NULL) {
        typeCon = 1;
    }
    if (sub3 != NULL) {
        typeCon = 0;
    }
    takeThisRequest(buffer, new_socket, typeCon);
#ifdef DEBUG
    printf("meta tin take this request");
    printf("type con: %d\n", typeCon);
#endif
    if (typeCon == 1) {
        printf("Epistrefei keep-alive\n");
        return 1;
    } else {
        printf("kleinei i sindesi\n");
        close(new_socket);
        return 0;

    }
    return 0;
}

void *handle() {
    pthread_mutex_lock(&mut);
    int now_socket = dequeue(queue);
    pthread_mutex_unlock(&mut);
    int goOn = handleThreads(now_socket);
    while (goOn == 1) {
        printf("apo handle\n");
        goOn = handleThreads(now_socket);
    }


    pthread_mutex_lock(&mut);
    busy = busy - 1;
    pthread_cond_wait(&con, &mut);
    handle();
    return;
}




int enqueue(int value, QUEUE *q) {
    NODE *p = NULL;
    p = (NODE *) malloc(sizeof(NODE));
    if (p == NULL) {
        printf("System out of memory\n");
        return 0;
    }
    p->current_socket = value;
    p->next = NULL;
    if (q->length == 0)
        q->head = q->tail = p;
    else {
        q->tail->next = p;
        q->tail = p;
    }
    (q->length)++;
    return 1;
}

int dequeue(QUEUE *q) {
    NODE *p = NULL;
    if ((q == NULL) || (q->head == NULL)) {
        printf("Sorry queue is empty\n");
        return -1;
    }
    p = q->head;

    int epistrofi = q->head->current_socket;
    q->head = q->head->next;
    free(p);
    --(q->length);
    if (q->length == 0) {
        q->tail = NULL;
    }

    return epistrofi;
}


int main() {
    readConfig();
    int create_socket, new_socket;
    queue = (QUEUE *) malloc(sizeof(QUEUE));
    queue->head = queue->tail = NULL;
    queue->length = 0;
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
    pthread_t tid[WORKERS];

    int times = 0;
    while (1) {
        if (listen(create_socket, WORKERS) < 0) {
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

        if (busy >= WORKERS) {
            close(new_socket);
        }
        else {
            printf("apasxolimenoi: %d\n", busy);
            if (times <= WORKERS) {
                printf("new socket: %d\n", new_socket);
                pthread_mutex_lock(&mut);
                busy = busy + 1;
                enqueue(new_socket, queue);
                pthread_mutex_unlock(&mut);
                times++;
                if (pthread_create(&tid[times], NULL, &handle, NULL) != 0) {
                    printf("pthread_create");
                }


            }
            else {

                printf("new socket: %d\n", new_socket);
                printf("Perimenei");
                pthread_mutex_lock(&mut);
                enqueue(new_socket, queue);
                busy = busy + 1;
                times++;
                pthread_cond_signal(&con);
                pthread_mutex_unlock(&mut);
            }


        }

    }
    close(create_socket);
    return 0;
}
