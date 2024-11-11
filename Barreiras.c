#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 4
pthread_barrier_t barrier;

void *task(void *arg) {
    int id = *(int *)arg;

    printf("Thread %d: Executando a etapa 1\n", id);
    sleep(1);
    pthread_barrier_wait(&barrier);  // Sincronização

    printf("Thread %d: Executando a etapa 2\n", id);
    pthread_barrier_wait(&barrier);  // Sincronização

    printf("Thread %d: Finalizou\n", id);
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int ids[THREAD_COUNT];
    pthread_barrier_init(&barrier, NULL, THREAD_COUNT);

    for (int i = 0; i < THREAD_COUNT; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, task, &ids[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}
