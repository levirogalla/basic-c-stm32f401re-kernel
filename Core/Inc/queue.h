// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>  // For using 'bool' type in C

// Define the structure for queue elements
typedef struct QueueNode {
    void* data;
    struct QueueNode *next;
} QueueNode;

// Define the structure for the queue itself
typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int max;
    int len;

} Queue;

// Function prototypes
void initQueue(Queue *q, int max);
bool isEmpty(Queue *q);
bool isFull(Queue *q);
void enqueue(Queue *q, void* data);
void* dequeue(Queue *q);
void freeQueue(Queue *q);

#endif  // QUEUE_H
