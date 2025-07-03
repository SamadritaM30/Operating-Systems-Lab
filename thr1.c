#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 5

typedef struct {
    int thread_id;
    char message[100];
} thread_data_t;

void *thread_function(void *arg) {
    thread_data_t *data =(thread_data_t *)arg;

    printf("Thread ID: %d, Message: %s\n", data->thread_id, data->message);

    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int i;
    int rc;

    for (i= 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        sprintf(thread_data[i].message, "This is thread number %d", i);

        printf("Creating thread... %d\n", i);
        rc= pthread_create(&threads[i], NULL, thread_function, (void *)&thread_data[i]);

        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    for (i =0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("All threads have completed ! \n");
    pthread_exit(NULL);
    return 0;
}
