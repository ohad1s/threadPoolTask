#ifndef EX2_THREADPOOL_QUEUE_H
#define EX2_THREADPOOL_QUEUE_H

#include <stdlib.h>
#include <pthread.h>

//typedef struct Node {
//    void* data;
//    struct Node* next;
//} Node;

typedef struct threadNode
{
    char* buf;
    int key;
    struct threadNode* next;
    int index;
} threadNode, *PthreadNode;

typedef struct {
    threadNode * front;
    threadNode * rear;
    int size;
} Queue;

void queueInit(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int queueIsEmpty(Queue* queue) {
    return queue->size == 0;
}

void queueEnqueue(Queue* queue, void* data,int k) {
    static int nodeId = 0;
    threadNode * newNode = (threadNode *)malloc(sizeof(threadNode));
    newNode->buf = data;
    newNode->key = k;
    newNode->index= nodeId;
    nodeId+=128;
    printf("new node with data: %s", newNode->buf);
    if (queue->rear == NULL) {
        // If the queue is empty
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        // Add the new node to the end of the queue
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    queue->size++;
}

void* queueDequeue(Queue* queue) {
    if (queueIsEmpty(queue)) {
        return NULL; // Queue is empty
    }

    threadNode * frontNode = queue->front;
//    void* data = frontNode->data;

    queue->front = frontNode->next;

    if (queue->front == NULL) {
        // If the last element is dequeued
        queue->rear = NULL;
    }

//    free(frontNode);
    queue->size--;
//    printf("dequeue: %s", frontNode->buf);
    return frontNode;
}

int queueSize(Queue* queue) {
    return queue->size;
}

void queueClear(Queue* queue) {
    while (!queueIsEmpty(queue)) {
        queueDequeue(queue);
    }
}


#endif //EX2_THREADPOOL_QUEUE_H
