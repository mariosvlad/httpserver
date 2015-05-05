#include "sendGetHead.h"
#include "otherRequests.h"
#include "helpers.h"
#include "queue.h"
#include "treads.h"

int WORKERS;
int PORT;
int bufsize = 1024;
int busy = 0;
int new_socket2;
//int create_socket, new_socket;
socklen_t addrlen;
struct sockaddr_in address;


pthread_mutex_t mtx;
pthread_mutex_t mt;

//messages
char ok[] = "HTTP/1.1 200 OK\r\n";
char notFound[] = "HTTP/1.1 404 NOT FOUND\r\n";
char notImpl[] = "HTTP/1.1 501 NOT IMPLEMENTED\r\n";
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t con = PTHREAD_COND_INITIALIZER;



QUEUE *queue = NULL;

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
