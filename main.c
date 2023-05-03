#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node_thread.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("FORMAT: ./main <key> <flag>\n");
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

    threadPool *pool = createPool(mission, key);
    threadNode *node = createNode(key);
    addNode(pool, node);
    int pool_place = 0;
    int node_place = 0;
    char c;
    while ((c = getchar()) != EOF) {
        if (pool_place >= POOL_MAX_SIZE) {
            printf("Maximum 25,600 chars for this task!\n");
            return 0;
        }
        if (node_place >= THREAD_BUFFER) {
            node_place = 0;
            pool_place++;
            node = createNode(key);
            addNode(pool, node);
        }
        pool->pool[pool_place]->buf[node_place] = c;
        node_place++;
    }

    play(pool);
    print(pool);
    free_pool(pool);

    return 0;
}