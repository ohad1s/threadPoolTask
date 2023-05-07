#ifndef EX2_THREADPOOL_NODE_THREAD_H
#define EX2_THREADPOOL_NODE_THREAD_H

#define POOL_MAX_SIZE 12000
#define THREAD_BUFFER 128

#include "codec.h"
#include <pthread.h>
#include "queue.h"

typedef struct threadPool {
    pthread_t *pool;
    int size;
    pthread_mutex_t lock;
    pthread_cond_t notify;

    void (*mission)(void *);

    int key;
    Queue *queue;
    int shutdown;
    char buffer[POOL_MAX_SIZE];
} threadPool, *PthreadPool;


void *threadPoolWorker(void *arg) {
    threadPool *pool = (threadPool *) arg;

    while (1) {
        pthread_mutex_lock(&(pool->lock));

        if (queueIsEmpty(pool->queue) && !pool->shutdown) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        if (pool->shutdown && queueIsEmpty(pool->queue)) {
            pthread_exit(NULL);
        }

//        printf("queue size: %d", pool->queue->size);

        threadNode *task = queueDequeue(pool->queue);
        printf("task data: %s", task->buf);
        pthread_mutex_unlock(&(pool->lock));

        // Execute the task
        pool->mission(task);
        pthread_mutex_unlock(&(pool->lock));
        printf("buf: %s", task->buf);
        memcpy(&pool->buffer[task->index], task->buf, THREAD_BUFFER);
        free(task);
    }
}

threadPool *createPool(void *mission, int key, int size) {
    threadPool *p = (PthreadPool) malloc(sizeof(threadPool));
    p->pool = (pthread_t *) malloc(size * sizeof(pthread_t));
    p->size = size;
    p->mission = mission;
    p->key = key;
    p->shutdown = 0;
    pthread_mutex_init(&p->lock, NULL);
    pthread_cond_init(&(p->notify), NULL);
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queueInit(queue);
    p->queue = queue;
    p->buffer[POOL_MAX_SIZE - 1] = '\n';
    return p;
}

//threadNode* createNode(int k){
//    threadNode* n = malloc(sizeof(threadNode));
//    n->key=k;
//    return n;
//}


//void addNode(threadPool* pool, threadNode* node){
//    int i = pool->free;
//    pool->pool[i]=node;
//    pool->free++;
//}


void *EncryptNode(void *threadargs) {
    printf("enc ");
    //Get all the variables we need to do the operation
    threadNode *args = (threadNode *) threadargs;
    int key = args->key;
//    char buffer[THREAD_BUFFER];
//    strcpy(buffer, args->buf);
    printf("enc2 ");
    encrypt(args->buf, key);
    printf("enc3 ");

    printf("\nenc in enc: %s\n", args->buf);
    return NULL;
}

void *DecryptNode(void *threadargs) {
    printf("dec");
    //Get all the variables we need to do the operation
    threadNode *args = (threadNode *) threadargs;
    int key = args->key;
//    char buffer[THREAD_BUFFER];
//    strcpy(buffer, args->buf);
    decrypt(args->buf, key);
    return NULL;
}


//void play(threadPool* pool){
//    for (int i=0; i<pool->free;i++){
////        ThreadArgs* args = malloc(sizeof(ThreadArgs));
////        args->key=pool->key;
////        strcpy(args->buf, pool->pool[i]->buf);
//        pthread_create(&(pool->pool[i]->th), NULL, (void* (*)(void*))pool->mission, pool->pool[i] );
//    }
//    for (int i=0; i<pool->free;i++){
//        pthread_join(pool->pool[i]->th, NULL);
//    }
//}
//
void print(threadPool *pool) {
    printf("%s", pool->buffer);
}

//
void free_pool(threadPool *pool) {
    free(pool->pool);
    free(pool->queue);
    free(pool);
}


#endif //EX2_THREADPOOL_NODE_THREAD_H

