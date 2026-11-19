#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TAM_BUFFER 5
#define ITEMS_POR_PRODUCTOR 10

int buffer[TAM_BUFFER];
int in = 0;
int out = 0;

pthread_mutex_t mutex;
sem_t vacios;
sem_t llenos;

void* productor(void* arg) {
    int id = *(int*)arg;

    for (int i = 1; i <= ITEMS_POR_PRODUCTOR; i++) {
        int item = (id * 100) + i;

        sem_wait(&vacios);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("[Productor %d] Produjo: %d en posicion %d\n", id, item, in);
        in = (in + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&llenos);
        usleep(100000);
    }

    pthread_exit(NULL);
}

void* consumidor(void* arg) {
    int id = *(int*)arg;

    for (int i = 1; i <= ITEMS_POR_PRODUCTOR; i++) {
        sem_wait(&llenos);
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("  --> [Consumidor %d] Consumio: %d de posicion %d\n", id, item, out);
        out = (out + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&vacios);

        usleep(150000);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t prod[2], cons[2];
    int ids[2] = {1, 2};

    pthread_mutex_init(&mutex, NULL);
    sem_init(&vacios, 0, TAM_BUFFER);
    sem_init(&llenos, 0, 0);       

    for (int i = 0; i < 2; i++) {
        pthread_create(&prod[i], NULL, productor, &ids[i]);
        pthread_create(&cons[i], NULL, consumidor, &ids[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&vacios);
    sem_destroy(&llenos);

    printf("\nEjecucion finalizada correctamente (20 items producidos y 20 consumidos).\n");
    return 0;
}
