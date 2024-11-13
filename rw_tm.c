#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t wrt;       
pthread_mutex_t mutex;     
int cnt = 1;               
int numreader = 0;         

void *writer(void *wno) {
    int writer_id = *((int *)wno);
    
    pthread_mutex_lock(&wrt);
    
    cnt = cnt * 2;
    printf("Writer %d modified cnt to %d\n", writer_id, cnt);
    
    pthread_mutex_unlock(&wrt);
    
    printf("Writer %d is leaving.\n", writer_id);
    
    return NULL;
}

void *reader(void *rno) {
    int reader_id = *((int *)rno);
    
    pthread_mutex_lock(&mutex);
    
    numreader++;
    if (numreader == 1) {
        pthread_mutex_lock(&wrt);
    }
    
    pthread_mutex_unlock(&mutex);
    
    printf("Reader %d: read cnt as %d\n", reader_id, cnt);
    
    pthread_mutex_lock(&mutex);
    
    numreader--;
    if (numreader == 0) {
        pthread_mutex_unlock(&wrt);
    }
    
    pthread_mutex_unlock(&mutex);
    
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
    
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&wrt, NULL);
    
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
    
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&wrt);
    
    return 0;
}