#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int read_count = 0, write_count = 0;  // Contadores de leitores e escritores
int shared_data = 0;  // Recurso compartilhado

void *reader(void *arg) {
    int id = *(int *)arg;
    pthread_mutex_lock(&mutex);
    while (write_count > 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    read_count++;
    pthread_mutex_unlock(&mutex);

    printf("Reader %d reading shared data: %d\n", id, shared_data);
    sleep(1);

    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0) pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;
    pthread_mutex_lock(&mutex);
    write_count++;
    while (read_count > 0) {
        pthread_cond_wait(&cond, &mutex);
    }

    shared_data += id;  // Escrita no recurso compartilhado
    printf("Writer %d writing shared data: %d\n", id, shared_data);
    sleep(2);

    write_count--;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t readers[5], writers[2];
    int ids[5];
    for (int i = 0; i < 5; i++) ids[i] = i + 1;

    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, &ids[i]);
        if (i < 2) pthread_create(&writers[i], NULL, writer, &ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
        if (i < 2) pthread_join(writers[i], NULL);
    }

    return 0;
}
