#ifndef QUEUE_H
#define QUEUE_H

#include "scheduler.h"

typedef struct Node {
    Process* process;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

void init_queue(Queue* q);
int is_empty(Queue* q);
void enqueue(Queue* q, Process* p);
Process* dequeue(Queue* q);
Process* peek(Queue* q);
void free_queue(Queue* q);

#endif
