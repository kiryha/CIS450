#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define SHARED 1

void *Producer (void *); // the two threads
void *Consumer (void *);

// Semaphores
sem_t empty;
sem_t full;
sem_t mutex;
// Shared buffer of size = 1
int data;
// Number of iterations
int iterations;

// Read command line and create threads
int main(int argc, char *argv[]) {

    // Producers and consumers ids
    pthread_t producer_id_1;
    pthread_t producer_id_2;
    pthread_t producer_id_3;
    pthread_t consumer_id_1;
    pthread_t consumer_id_2;
    pthread_t consumer_id_3;

    sem_init(&empty, SHARED, 1);  // sem empty = 1
    sem_init(&full, SHARED, 0);   // sem full = 0
    sem_init(&mutex, SHARED, 1);  // sem mutex = 1

    if (argc < 3) {
	    printf("Usage: boundedBuffer <number of iterations> <buffer size>\n");
	    printf("Example: ./boundedBuffer 100 10\n");
	    exit(0);
    }

    iterations = atoi(argv[1]);
    int buffer_size = atoi(argv[2]);
    int data_array[buffer_size];

    // Create producers and consumers
    pthread_create(&producer_id_1, NULL, Producer, NULL);
//    pthread_create(&producer_id_2, NULL, Producer, NULL);
//    pthread_create(&producer_id_3, NULL, Producer, NULL);
    pthread_create(&consumer_id_1, NULL, Consumer, NULL);
//    pthread_create(&consumer_id_2, NULL, Consumer, NULL);
//    pthread_create(&consumer_id_3, NULL, Consumer, NULL);

    pthread_join(producer_id_1, NULL);
    pthread_join(consumer_id_1, NULL);
    pthread_exit(0);
}

// Deposit n iterations into the data buffer
void *Producer(void *arg) {
    int produced;

    for (produced = 0; produced < iterations; produced++) {
        sem_wait(&empty);
        data = produced;
        printf(">> Produced %d\n", produced);
        sem_post(&full);
    }

}

// Fetch iterations items from the buffer and sum them
void *Consumer(void *arg) {
    int total = 0;
    int consumed;

    for (consumed = 0; consumed < iterations; consumed++) {
        sem_wait(&full);
        total = total + data;
        printf(">> Consumed %d\n", consumed);
        sem_post(&empty);
    }

    printf("The total number is %d\n", total);
}
