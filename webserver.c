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
            //printf("WORKERS : %d \n", WORKERS);
        }
        else if (strstr(line, "PORT")) {
            memcpy(tmp, &line[5], 7);
            tmp[7] = '\0';
            PORT = atoi(tmp);
            //printf("PORT : %d \n", PORT);
        }
    }

    fclose(fp);
    if (line)
        free(line);
}

int main(void) {
    readConfig();
    exit(EXIT_SUCCESS);
}