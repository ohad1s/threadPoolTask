#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "threadpool.h"

#define MAX_DATA_SIZE 1024

typedef struct {
    int key;
    char data[MAX_DATA_SIZE];
    int length;
} EncryptedData;

void encryptData(void *arg) {
    EncryptedData *encryptedData = (EncryptedData *) arg;
    encrypt(encryptedData->data, encryptedData->key);
    printf("encrypted data: %s\n", encryptedData->data);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage: key < file \n");
        printf("!! data more than %d char will be ignored !!\n", MAX_DATA_SIZE);
        return 0;
    }

    int key = atoi(argv[1]);
    printf("key is %i \n",key);

    char c;
    int counter = 0;
    int dest_size = MAX_DATA_SIZE;
    char data[dest_size];
    ThreadPool *pool = createThreadPool(4);

    while ((c = getchar()) != EOF)
    {
        data[counter] = c;
        counter++;

        if (counter == MAX_DATA_SIZE){
            EncryptedData *encryptedData = malloc(sizeof(EncryptedData));
            encryptedData->key = key;
            strncpy(encryptedData->data, data, counter);
            encryptedData->length = counter;
            submitJob(pool, encryptData, (void *) encryptedData);
            counter = 0;
        }
    }

    if (counter > 0)
    {
        EncryptedData *encryptedData = malloc(sizeof(EncryptedData));
        encryptedData->key = key;
        strncpy(encryptedData->data, data, counter);
        encryptedData->length = counter;
        submitJob(pool, encryptData, (void *) encryptedData);
    }

    destroyThreadPool(pool);
    return 0;
}
