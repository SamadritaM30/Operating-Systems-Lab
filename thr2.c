#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000000

typedef struct {
    int thread_id;
    int *array;
    int start;
    int end;
    long long partial_sum;
} thread_data_t;

void *thread_function(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    long long sum = 0;
    
    int i=0;
    for (i = data->start; i < data->end; i++) {
        sum += data->array[i];
    }

    data->partial_sum = sum;
    pthread_exit(NULL);
}

int main() {
    int *array= (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL){
        perror("Failed to allocate memory for the array ! ");
        return 1;
    }
    int i=0;
    for ( i = 0; i < ARRAY_SIZE; i++) {
        array[i] =i + 1;
    }

    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int chunk_size= ARRAY_SIZE / NUM_THREADS;
    int remainder = ARRAY_SIZE % NUM_THREADS;

    int rc;
    long long total_sum = 0;
    i=0;
    for (i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].array = array;
        thread_data[i].start =i * chunk_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? (i + 1) * chunk_size + remainder : (i + 1) * chunk_size;
        thread_data[i].partial_sum= 0;

        printf("Creating thread... %d, start=%d, end=%d\n", i, thread_data[i].start, thread_data[i].end);

        rc = pthread_create(&threads[i], NULL, thread_function, (void *)&thread_data[i]);
        if (rc) {
            printf("ERROR : return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    i=0;
    for ( i =0; i< NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        total_sum += thread_data[i].partial_sum;
    }

    printf("Total sum = %lld\n",total_sum);

    free(array);
    pthread_exit(NULL);
    return 0;
}
