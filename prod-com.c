#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int head = 0, tail = 0;

sem_t s_free, s_used;
pthread_mutex_t mutex;

void insert_item(int item) {
    sem_wait(&s_free);
    pthread_mutex_lock(&mutex);

    buffer[head] = item;
    printf("Inserted: '%d' Used: %d Free: %d\n", item, (head - tail + BUFFER_SIZE) % BUFFER_SIZE, (BUFFER_SIZE - (head - tail + BUFFER_SIZE) % BUFFER_SIZE));
    head = (head + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&s_used);
}

void remove_item() {
    sem_wait(&s_used);
    pthread_mutex_lock(&mutex);

    int item = buffer[tail];
    printf("Removed: '%d' Used: %d Free: %d\n", item, (head - tail + BUFFER_SIZE) % BUFFER_SIZE, (BUFFER_SIZE - (head - tail + BUFFER_SIZE) % BUFFER_SIZE));
    tail = (tail + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&s_free);
}

void *producer(void *param) {
    int item;
    
    while (1) {
        sleep(rand() % 3); // Simulate production time
        item = rand() % 100; // Generate random item
        insert_item(item);
    }
}

void *consumer(void *param) {
    while (1) {
        sleep(rand() % 3); // Simulate consumption time
        remove_item();
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Syntax: prod-com <sleeptime> <producers> <consumers>\n");
        exit(0);
    }
    
    int sleeptime, numP, numC;
    int numcreated = 0;

    sleeptime = atoi(argv[1]); 
    numP = atoi(argv[2]); 
    numC = atoi(argv[3]);
    pthread_t threads[numP + numC];

    printf("Producers: %d  Consumers: %d  Will sleep for: %ds\n", numP, numC, sleeptime);

    /* initialise buffer */
    for(int c = 0; c < BUFFER_SIZE; c++) buffer[c] = 0;

    /* initialise the mutual-exclusion object mutex */
    pthread_mutex_init(&mutex,NULL);

    /* Initialise the free and used semaphores */
    sem_init(&s_free, 0, BUFFER_SIZE);
    sem_init(&s_used, 0, 0);

    /* create producer thread(s) */
    for(int c = 0; c < numP; c++) {
        if (pthread_create(&threads[numcreated++], NULL, producer, NULL) != 0) {
            fprintf(stderr, "Error creating producer thread.\n");
            exit(EXIT_FAILURE);
        } else {
            printf("Producer thread %d created.\n", numcreated);
        }
    }

    /* create consumer thread(s) */
    for(int c = 0; c < numC; c++) {
        if (pthread_create(&threads[numcreated++], NULL, consumer, NULL) != 0) {
            fprintf(stderr, "Error creating consumer thread.\n");
            exit(EXIT_FAILURE);
        } else {
            printf("Consumer thread %d created.\n", numcreated);
        }
    }

    /* sleep */
    printf("Main thread sleeping for %d seconds...\n", sleeptime); // Corrected sleep time
    sleep(sleeptime); // Original sleep time
    printf("Main thread done.\n");

    /* join all threads */
    for (int i = 0; i < numP + numC; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d.\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
