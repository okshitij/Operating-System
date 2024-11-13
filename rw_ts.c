#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

sem_t wrt;                 
sem_t reader_count_access;  
int cnt = 1;                
int numreader = 0;          

void *writer(void *wno) {
    int writer_id = *((int *)wno);
    
    sem_wait(&wrt);
    
    cnt = cnt * 2;
    printf("Writer %d modified cnt to %d\n", writer_id, cnt);
    
    sem_post(&wrt);
    
    printf("Writer %d is leaving.\n", writer_id);
    
    return NULL;
}

void *reader(void *rno) {
    int reader_id = *((int *)rno);
    
    sem_wait(&reader_count_access);
    
    numreader++;
    if (numreader == 1) {
        sem_wait(&wrt);
    }
    
    sem_post(&reader_count_access);
    
    printf("Reader %d: read cnt as %d\n", reader_id, cnt);
    
    sem_wait(&reader_count_access);
    
    numreader--;
    if (numreader == 0) {
        sem_post(&wrt);
    }
    
    sem_post(&reader_count_access);
    
    printf("Reader %d is leaving.\n", reader_id);
    
    return NULL;
}

int main() {
    int num_readers, num_writers;
    
    printf("Enter the number of readers: ");
    scanf("%d", &num_readers);
    
    printf("Enter the number of writers: ");
    scanf("%d", &num_writers);
    
    pthread_t read[num_readers], write[num_writers];   
    
    sem_init(&wrt, 0, 1);                 
    sem_init(&reader_count_access, 0, 1); 
    
    int *reader_ids = (int *)malloc(num_readers * sizeof(int)); 
    int *writer_ids = (int *)malloc(num_writers * sizeof(int)); 
    
    for (int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&read[i], NULL, reader, (void *)&reader_ids[i]);
    }
    
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&write[i], NULL, writer, (void *)&writer_ids[i]);
    }
    
    for (int i = 0; i < num_readers; i++) {
        pthread_join(read[i], NULL);
    }
    
    for (int i = 0; i < num_writers; i++) {
        pthread_join(write[i], NULL);
    }
    
    free(reader_ids);
    free(writer_ids);
    
    sem_destroy(&wrt);
    sem_destroy(&reader_count_access);
    
    return 0;
}
