#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty_count;
sem_t full_count;
pthread_mutex_t mutex;

void* producer(void* arg) {
    int item;
    while (1) {
        item = rand() % 100;
        sem_wait(&empty_count);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer produced %d at index %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full_count);
    }
}

void* consumer(void* arg) {
    int item;
    while (1) {
        sem_wait(&full_count);
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        printf("Consumer consumed %d from index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty_count);
    }
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty_count, 0, BUFFER_SIZE);
    sem_init(&full_count, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty_count);
    sem_destroy(&full_count);
    pthread_mutex_destroy(&mutex);

    return 0;
}