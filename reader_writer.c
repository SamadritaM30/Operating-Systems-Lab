#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define NUM_OPERATIONS 5

int readcount = 0, writecount = 0;
sem_t wrt, readTry;
pthread_mutex_t readLock, writeLock;

void* reader(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        sem_wait(&readTry);
        pthread_mutex_lock(&readLock);
        
        readcount++;
        if (readcount == 1) {
            sem_wait(&wrt);
        }
        
        pthread_mutex_unlock(&readLock);
        sem_post(&readTry);

        printf("Reader %d is reading\n", id);
        usleep(100000);

        pthread_mutex_lock(&readLock);
        readcount--;
        if (readcount == 0) {
            sem_post(&wrt);
        }
        pthread_mutex_unlock(&readLock);

        usleep(200000);
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        pthread_mutex_lock(&writeLock);
        writecount++;
        if (writecount == 1) {
            sem_wait(&readTry);
        }
        pthread_mutex_unlock(&writeLock);

        sem_wait(&wrt);

        printf("Writer %d is writing\n", id);
        usleep(150000);

        sem_post(&wrt);

        pthread_mutex_lock(&writeLock);
        writecount--;
        if (writecount == 0) {
            sem_post(&readTry);
        }
        pthread_mutex_unlock(&writeLock);

        usleep(300000);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    sem_init(&wrt, 0, 1);
    sem_init(&readTry, 0, 1);
    pthread_mutex_init(&readLock, NULL);
    pthread_mutex_init(&writeLock, NULL);

    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&wrt);
    sem_destroy(&readTry);
    pthread_mutex_destroy(&readLock);
    pthread_mutex_destroy(&writeLock);

    return 0;
}
