/* File: client.c */
#include <sys/types.h> /* For sockets */
#include <sys/socket.h> /* For sockets */
#include <netinet/in.h> /* For Internet sockets */
#include <netdb.h> /* For gethostbyname() */
#include <stdio.h> /* For I/O */
#include <stdlib.h>
#include <string.h>


/* Client with Internet stream sockets */
main(int argc, char *argv[]) {
    int port, sock, serverlen;
    char buf[256 * 8];
    struct sockaddr_in server;
    struct sockaddr *serverptr;
    struct hostent *rem;
/* Are server's host name and port number given? */
    if (argc < 3) {
        printf("Please give host name and port number\n");
        exit(1);
    }
/* Create TCP/IP socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    if ((rem = gethostbyname(argv[1])) == NULL) {
        herror("gethostbyname");
        exit(1);
    }
    port = atoi(argv[2]); /* Convert port number to integer */
    server.sin_family = PF_INET; /* Internet domain */
    bcopy((char *) rem->h_addr, (char *) &server.sin_addr,
          rem->h_length);
    server.sin_port = htons(port); /*Server's Internet address and port*/
    serverptr = (struct sockaddr *) &server;
    serverlen = sizeof(server);
    FILE *fp;
    if ((fp = fopen(argv[3], "r")) == NULL) {
        printf("File not found!");
        return 1;
    }
    if (connect(sock, serverptr, serverlen) < 0) { /* Request Connect */
        perror("connect");
        exit(1);
    }
    printf("Requested connection to host %s port %d\n", argv[1], port);
    bzero(buf, sizeof buf); /* Initialize buffer */
    while (!feof(fp))
        fgets(&buf[strlen(buf)], sizeof buf, fp); /* Read all the file */
    buf[strlen(buf)] = '\r';
    buf[strlen(buf) + 1] = '\n';
    buf[strlen(buf) + 2] = '\0';
    printf("Sending:\n%s\n", buf);
    if (write(sock, buf, sizeof buf) < 0) { /* Send message */
        perror("write");
        exit(1);
    }
    bzero(buf, sizeof buf); /* Initialize buffer */
    sleep(2);
    if (read(sock, buf, sizeof buf) < 0) { /* Receive message */
        perror("read");
        exit(1);
    }
    close(sock); /* Close socket */
    printf("Got:\n%s", buf);


    exit(0);
}
