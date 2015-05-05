#include "sendGetHead.h"




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
