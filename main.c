#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node_thread.h"
#include <sys/sysinfo.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("FORMAT: ./coder <key> <flag>\n");
        return 0;
    }
    int key = atoi(argv[1]);
    char *flag = argv[2];


    void *mission;

    if (!strcmp(flag, "-e")) {
        mission = &EncryptNode;
    } else if (!strcmp(flag, "-d")) {
        mission = &DecryptNode;
    } else {
        printf("flag must be: -e or -d\n");
        return 0;
    }

    int numOfThreads = get_nprocs_conf();
//    int numOfThreads = 1;


    threadPool *pool = createPool(mission, key, numOfThreads);
    char data[THREAD_BUFFER];
    int place = 0;
//    threadNode *node = createNode(key);
//    addNode(pool, node);
//    int pool_place = 0;
//    int node_place = 0;
    char c;
    while ((c = getchar()) != EOF) {
        data[place] = c;
        place++;
        if (place == THREAD_BUFFER - 1) {
            char *dataCopy = malloc(THREAD_BUFFER);  // Make a copy of the data
            memcpy(dataCopy, data, THREAD_BUFFER);
            dataCopy[THREAD_BUFFER - 1] = '\n';
            queueEnqueue(pool->queue, dataCopy, key);
            place = 0;
            strcpy(data, "");
//            memset(data, 0, THREAD_BUFFER);

        }
//        printf("data is: %s\n",data);
        print(pool);
    }
    queueEnqueue(pool->queue, data, key);

    for (int i = 0; i < pool->size; i++) {
        pthread_create(&(pool->pool[i]), NULL, threadPoolWorker, (void *) pool);
    }
    pool->shutdown = 1;
    for (int i = 0; i < pool->size; i++) {
        pthread_join((pool->pool[i]), NULL);
    }

    print(pool);
    free_pool(pool);
    printf("ciiii");


    return 0;
}