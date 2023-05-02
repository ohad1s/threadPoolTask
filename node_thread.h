#ifndef EX2_THREADPOOL_NODE_THREAD_H
#define EX2_THREADPOOL_NODE_THREAD_H

#define POOL_MAX_SIZE 200
#define THREAD_BUFFER 128
#include "codec.h"
#include <pthread.h>

typedef struct threadNode
{
    char buf[THREAD_BUFFER];
    pthread_t th;
    int key;
} threadNode, *PthreadNode;

typedef struct threadPool
{
    threadNode* pool[POOL_MAX_SIZE];
    int free;
    pthread_mutex_t lock;
    void (*mission)(void*);
    int key;
} threadPool, *PthreadPool;

typedef struct {
    char buf[THREAD_BUFFER];
    int key;
} ThreadArgs;


threadPool* createPool(void* mission,int key) {
    threadPool* p = malloc(sizeof(threadPool));
    p->free = 0;
    p->mission=mission;
    p->key=key;
    pthread_mutex_init(&p->lock, NULL);
    return p;
}

threadNode* createNode(int k){
    threadNode* n = malloc(sizeof(threadNode));
    n->key=k;
    return n;
}


void addNode(threadPool* pool, threadNode* node){
    int i = pool->free;
    pool->pool[i]=node;
    pool->free++;
}


void *EncryptNode(void *threadargs){
    //Get all the variables we need to do the operation
    threadNode* args = (threadNode*) threadargs;
    int key = args->key;
//    char buffer[THREAD_BUFFER];
//    strcpy(buffer, args->buf);
    encrypt(args->buf,key);
}

void *DecryptNode(void *threadargs){
    //Get all the variables we need to do the operation
    threadNode* args = (threadNode*) threadargs;
    int key = args->key;
//    char buffer[THREAD_BUFFER];
//    strcpy(buffer, args->buf);
    decrypt(args->buf,key);
}


void play(threadPool* pool){
    for (int i=0; i<pool->free;i++){
//        ThreadArgs* args = malloc(sizeof(ThreadArgs));
//        args->key=pool->key;
//        strcpy(args->buf, pool->pool[i]->buf);
        pthread_create(&(pool->pool[i]->th), NULL, (void* (*)(void*))pool->mission, pool->pool[i] );
    }
    for (int i=0; i<pool->free;i++){
        pthread_join(pool->pool[i]->th, NULL);
    }
}

void print(threadPool*pool){
    for (int i=0; i<pool->free;i++){
        printf("%s",pool->pool[i]->buf);
    }
}


#endif //EX2_THREADPOOL_NODE_THREAD_H

