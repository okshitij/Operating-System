#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MaxItems 5  // Maximum items a producer can produce or a consumer can consume
#define BufferSize 5 // Size of the buffer

int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;
pthread_cond_t not_empty;  // Condition variable to indicate the buffer is not empty
pthread_cond_t not_full;   // Condition variable to indicate the buffer is not full

void *producer(void *pno) {   
    int item;
    for (int i = 0; i < MaxItems; i++) {
        item = rand(); // Produce a random item

        pthread_mutex_lock(&mutex);
        printf("Producer %d: Entering critical region\n", *((int *)pno));

        // Wait until there is space in the buffer (buffer is not full)
        while ((in + 1) % BufferSize == out) {
            printf("Producer %d: Buffer is full. Waiting...\n", *((int *)pno));
            pthread_cond_wait(&not_full, &mutex); // Wait for space in the buffer
        }

        // Insert the item into the buffer
        buffer[in] = item;
        printf("Producer %d: Inserted Item %d at %d\n", *((int *)pno), buffer[in], in);
        in = (in + 1) % BufferSize;

        // Signal that the buffer is not empty
        pthread_cond_signal(&not_empty);
        printf("Producer %d: Leaving critical region\n", *((int *)pno));

        pthread_mutex_unlock(&mutex);  // Unlock after accessing the buffer
    }
}

void *consumer(void *cno) {   
    for (int i = 0; i < MaxItems; i++) {
        pthread_mutex_lock(&mutex);
        printf("Consumer %d: Entering critical region\n", *((int *)cno));

        // Wait until there is something to consume (buffer is not empty)
        while (in == out) {
            printf("Consumer %d: Buffer is empty. Waiting...\n", *((int *)cno));
            pthread_cond_wait(&not_empty, &mutex); // Wait for items to consume
        }

        // Consume the item from the buffer
        int item = buffer[out];
        printf("Consumer %d: Removed Item %d from %d\n", *((int *)cno), item, out);
        out = (out + 1) % BufferSize;

        // Signal that the buffer is not full
        pthread_cond_signal(&not_full);
        printf("Consumer %d: Leaving critical region\n", *((int *)cno));

        pthread_mutex_unlock(&mutex);  // Unlock after accessing the buffer
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
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

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

    // Clean up resources
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);
    free(pro);
    free(con);
    free(a_pro);
    free(a_con);

    return 0;
}
