
#include "treads.h"



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
