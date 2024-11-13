#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define MaxItems 5  // Maximum items a producer can produce or a consumer can consume
#define BufferSize 5 // Size of the buffer

sem_t empty;  // Semaphore to track empty slots in the buffer
sem_t full;   // Semaphore to track filled slots in the buffer
sem_t mutex;  // Semaphore to provide mutual exclusion

int in = 0;
int out = 0;
int buffer[BufferSize];

void *producer(void *pno) {
    int item;
    for (int i = 0; i < MaxItems; i++) {
        item = rand(); // Produce a random item

        if (sem_trywait(&empty) != 0) {
            // Buffer is full, producer must wait
            printf("Producer %d: Buffer is full, waiting to produce...\n", *((int *)pno));
            sem_wait(&empty); // Wait until there's an empty slot
        }

        sem_wait(&mutex); // Enter critical section

        // Critical section: Insert item into buffer
        printf("Producer %d: Entering critical region\n", *((int *)pno));
        buffer[in] = item;
        printf("Producer %d: Inserted Item %d at %d\n", *((int *)pno), buffer[in], in);
        in = (in + 1) % BufferSize;

        // Exit critical section
        printf("Producer %d: Leaving critical region\n", *((int *)pno));
        sem_post(&mutex);  // Exit critical section
        sem_post(&full);   // Signal that buffer has at least one item
    }
}

void *consumer(void *cno) {
    for (int i = 0; i < MaxItems; i++) {
        if (sem_trywait(&full) != 0) {
            // Buffer is empty, consumer must wait
            printf("Consumer %d: Buffer is empty, waiting to consume...\n", *((int *)cno));
            sem_wait(&full); // Wait until there's an item in the buffer
        }

        sem_wait(&mutex); // Enter critical section

        // Critical section: Remove item from buffer
        printf("Consumer %d: Entering critical region\n", *((int *)cno));
        int item = buffer[out];
        printf("Consumer %d: Removed Item %d from %d\n", *((int *)cno), item, out);
        out = (out + 1) % BufferSize;

        // Exit critical section
        printf("Consumer %d: Leaving critical region\n", *((int *)cno));
        sem_post(&mutex);  // Exit critical section
        sem_post(&empty);  // Signal that buffer has at least one empty slot
    }
}

int main() {
    int num_producers, num_consumers;
    printf("Enter the number of producers: ");
    scanf("%d", &num_producers);
    printf("Enter the number of consumers: ");
    scanf("%d", &num_consumers);

    pthread_t *pro = malloc(num_producers * sizeof(pthread_t));
    pthread_t *con = malloc(num_consumers * sizeof(pthread_t));

    // Initialize semaphores
    sem_init(&empty, 0, BufferSize);  // Initial value is BufferSize, meaning all slots are empty
    sem_init(&full, 0, 0);            // Initial value is 0, meaning no filled slots
    sem_init(&mutex, 0, 1);           // Mutex semaphore for mutual exclusion, initial value 1

    int *a_pro = malloc(num_producers * sizeof(int));
    int *a_con = malloc(num_consumers * sizeof(int));

    // Create producer threads
    for (int i = 0; i < num_producers; i++) {
        a_pro[i] = i + 1; 
        pthread_create(&pro[i], NULL, producer, (void *)&a_pro[i]);
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; i++) {
        a_con[i] = i + 1; 
        pthread_create(&con[i], NULL, consumer, (void *)&a_con[i]);
    }

    // Join producer threads
    for (int i = 0; i < num_producers; i++) {
        pthread_join(pro[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(con[i], NULL);
    }

    // Destroy semaphores and free memory
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    free(pro);
    free(con);
    free(a_pro);
    free(a_con);

    return 0;
}
