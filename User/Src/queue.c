//
// Created by ALIEN on 2023/7/27.
//
#include "queue.h"

// Function to create an empty queue
Queue *createQueue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    if (queue) {
        queue->front = NULL;
        queue->rear = NULL;
    }
    return queue;
}

// Function to check if the queue is empty
int isEmpty(Queue *queue) {
    return (queue->front == NULL);
}

// Function to enqueue (add) an element to the queue
void enqueue(Queue *queue, Data data) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode) {
        newNode->data = data;
        newNode->next = NULL;
        if (isEmpty(queue)) {
            queue->front = newNode;
            queue->rear = newNode;
        } else {
            queue->rear->next = newNode;
            queue->rear = newNode;
        }
    } else {
        printf("Memory allocation error while enqueueing.\n");
    }
}

// Function to dequeue (remove) an element from the queue
Data dequeue(Queue *queue) {
    Node *temp = queue->front;
    Data data = temp->data;
    queue->front = queue->front->next;
    free(temp);
    return data;
}

// Function to display the contents of the queue
void display(Queue *queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }
    printf("Queue contents: ");
    Node *current = queue->front;
    while (current) {
        printf("%d ", current->data.x);
        current = current->next;
    }
    printf("\n");
}

// Function to free the memory occupied by the queue
void freeQueue(Queue *queue) {
    Node *current = queue->front;
    while (current) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
}

//int main() {
//    Queue* queue = createQueue();
//
//    Data data_array[100];
//    data_array[0].x = 1, data_array[0].y = 1, data_array[0].color = 0xFF;
//    data_array[1].x = 2, data_array[1].y = 1, data_array[1].color = 0xFF;
//    data_array[2].x = 13, data_array[2].y = 1, data_array[2].color = 0xFF;
//    enqueue(queue, data_array[0]);
//    enqueue(queue, data_array[1]);
//    enqueue(queue, data_array[2]);
//
//    display(queue); // Output: Queue contents: 10 20 30
//
//    int item = dequeue(queue).x;
//    printf("Dequeued item: %d\n", item); // Output: Dequeued item: 10
//
//    display(queue); // Output: Queue contents: 20 30
//
//    freeQueue(queue);
//    return 0;
//}
