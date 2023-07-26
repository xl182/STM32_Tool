//
// Created by ALIEN on 2023/7/27.
//

#ifndef TOOL_QUEUE_H
#define TOOL_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Data {
    int x;
    int y;
    int color;
} Data;

// Node structure for the linked list
typedef struct Node {
    Data data;
    struct Node *next;
} Node;

// Queue structure
typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

Queue *createQueue();

int isEmpty(Queue *queue);

void enqueue(Queue *queue, Data data);

Data dequeue(Queue *queue);

void freeQueue(Queue *queue);

#endif //TOOL_QUEUE_H
