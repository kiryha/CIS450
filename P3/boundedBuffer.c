#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define SHARED 1

void *Producer (void *);
void *Consumer (void *);

// Semaphores
sem_t empty;
sem_t full;
pthread_mutex_t mutex;
// Shared buffer of size = 1
int data;
// Shared data of custom size (user input > buffer size)
// int *data_array;
// Number of iterations
int iterations;

// Read command line and create threads
int main(int argc, char *argv[]) {

    if (argc < 3) {
	    printf("Usage: boundedBuffer <number of iterations> <buffer size>\n");
	    printf("Example: ./boundedBuffer 100 10\n");
	    exit(0);
    }

    // Producers and consumers ids
    pthread_t producer_id_1;
    pthread_t producer_id_2;
    pthread_t producer_id_3;
    pthread_t consumer_id_1;
    pthread_t consumer_id_2;
    pthread_t consumer_id_3;

    // Get arguments
    iterations = atoi(argv[1]);  // Number of iterations
    int buffer_size = atoi(argv[2]);
    int data_array[buffer_size]; // Buffer size local variable

//    data_array = (int*)malloc(buffer_size * sizeof(int));
//    int s = sizeof(data_array) / sizeof(data_array[0]);
//    printf(">> in %d\n", buffer_size);
//    printf(">> Size %d\n", s);

    // Init semaphore
    sem_init(&empty, SHARED, buffer_size);  // Init semaphore empty = 1 (number of empty spaces in buffer)
    sem_init(&full, SHARED, 0);             // Init semaphore full = 0 (number of items in buffer)
    pthread_mutex_init(&mutex, NULL);       // Init mutex

    // Create producers and consumers
    pthread_create(&producer_id_1, NULL, Producer, NULL);
    // pthread_create(&producer_id_2, NULL, Producer, NULL);
    // pthread_create(&producer_id_3, NULL, Producer, NULL);
    pthread_create(&consumer_id_1, NULL, Consumer, NULL);
    // pthread_create(&consumer_id_2, NULL, Consumer, NULL);
    // pthread_create(&consumer_id_3, NULL, Consumer, NULL);

    pthread_join(producer_id_1, NULL);
    // pthread_join(producer_id_2, NULL);
    // pthread_join(producer_id_3, NULL);
    pthread_join(consumer_id_1, NULL);
    // pthread_join(consumer_id_2, NULL);
    // pthread_join(consumer_id_3, NULL);

    pthread_exit(0);
}

// Deposit n iterations into the data buffer
void *Producer(void *arg) {
    int produced;

    for (produced = 0; produced < iterations; produced++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        data = produced;
        printf(">> Produced %d\n", produced);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

}

// Fetch iterations items from the buffer and sum them
void *Consumer(void *arg) {
    int total = 0;
    int consumed;

    for (consumed = 0; consumed < iterations; consumed++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        total = total + data;
        printf(">> Consumed %d\n", consumed);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }

    printf("The total number is %d\n", total);
}
