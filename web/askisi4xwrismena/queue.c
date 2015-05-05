#include "queue.h"

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
