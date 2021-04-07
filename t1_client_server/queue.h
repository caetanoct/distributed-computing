#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct node {
    struct node* next;
    int *client_socket;
} node;

void enqueue();

int * dequeue();


#endif