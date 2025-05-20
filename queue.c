#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Initializes an empty queue by resetting all its pointers and size.
void init_queue(Queue* q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

// Returns 1 if the queue is empty, 0 otherwise.
int is_empty(Queue* q) {
    return q->size == 0;
}

// Adds a new process to the rear of the queue.
void enqueue(Queue* q, Process* p) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->process = p;
    new_node->next = NULL;

    if (q->rear == NULL) {
        // Queue is empty; new node becomes both front and rear.
        q->front = q->rear = new_node;
    } else {
        // Attach the new node to the end of the queue.
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
}

// Removes and returns the process at the front of the queue.
// Returns NULL if the queue is empty.
Process* dequeue(Queue* q) {
    if (is_empty(q)) return NULL;

    Node* temp = q->front;
    Process* p = temp->process;
    q->front = q->front->next;

    if (q->front == NULL) {
        // Queue is now empty; reset rear pointer as well.
        q->rear = NULL;
    }

    free(temp);
    q->size--;
    return p;
}

// Returns the process at the front of the queue without removing it.
// Returns NULL if the queue is empty.
Process* peek(Queue* q) {
    if (is_empty(q)) return NULL;
    return q->front->process;
}

// Frees all nodes in the queue and resets its state.
void free_queue(Queue* q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
}
