/*
 * queue.c
 *
 *  Created on: Nov 8, 2024
 *      Author: levirogalla
 */


// queue.c
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

// Initialize the queue
void initQueue(Queue *q, int max) {
    q->front = q->rear = NULL;
    q->max = max;
    q->len = 0;
}

// Check if the queue is empty
bool isEmpty(Queue *q) {
    return q->front == NULL;
}

bool isFull(Queue *q) {
	return q->len == q->max;
}

// Add an element to the queue
void enqueue(Queue *q, void* data) {
    QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        perror("Failed to allocate memory for queue node");
        exit(EXIT_FAILURE);
    }
    if (isFull(q)) {
    	perror("Queue is full.");
    	exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->len++;
}

// Remove an element from the queue
void* dequeue(Queue *q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Queue underflow\n");
        exit(EXIT_FAILURE);
    }

    QueueNode* temp = q->front;
    void* data = temp->data;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    q->len--;
    return data;
}

// Free all elements in the queue
void freeQueue(Queue *q) {
    while (!isEmpty(q)) {
        dequeue(q);
    }
}
