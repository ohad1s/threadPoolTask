#ifndef EX2_THREADPOOL_NODE_THREAD_H
#define EX2_THREADPOOL_NODE_THREAD_H

#define POOL_MAX_SIZE 100
#define THREAD_BUFFER 80


typedef struct threadNode
{
    char buf[THREAD_BUFFER];
    pthread_t th;
} threadNode, *PthreadNode;

typedef struct threadPool
{
    ThreadNode* pool[POOL_MAX_SIZE];
    int free;
    pthread_mutex_t lock;
    void* mission;
    int key;
} threadPool, *PthreadPool;

typedef struct {
    char buf[THREAD_BUFFER];
    int key;
    ThreadNode* pool;
} ThreadArgs;


threadPool* createPool(void* mission,int key) {
    threadPool* p = malloc(sizeof(threadPool));
    p->free = 0;
    p->mission=mission;
    p->key=key;
    pthread_mutex_init(&p->lock, NULL);
    return q;
}

threadNode* createNode(){
    threadNode* n = malloc(sizeof(threadNode));
    return n;
}


void addNode(threadPool* pool, threadNode* node){
    pool->pool[i]=node;
}


// ################ STOPPED HERE !!!!! ###########################################
void *EncryptCover(void *threadargs){
    //Get all the variables we need to do the operation
    struct thread_args* args = (struct thread_args*) threadargs;
    int key = args->key;
    Queue* q = args->q;
    //Get the list we want to put our output in
    char** output = args->listOfOutput;
    while(!isEmpty(q)){
        Pair node = dequeue(q);
        char* encryptedData = node.string;
        encrypt(encryptedData,key);
        memcpy(output[node.index], encryptedData, 1024);
    }
    pthread_exit(NULL);
}


void play(threadPool* pool){
    for (int i=0; i<pool->free;i++){
        pthread_create(&(pool->pool[i]), NULL, pool->mission, pool->pool[i].buf, pool->key);
    }
}












#endif //EX2_THREADPOOL_NODE_THREAD_H

