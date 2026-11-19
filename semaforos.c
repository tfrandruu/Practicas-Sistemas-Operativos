#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaforo;

void* contar(void* arg) {
    int id = *(int*)arg;

    for (int i = 1; i <= 10000; i++) {
        if (i == 4000) {
            sem_wait(&semaforo);
            printf("\n--- [Hilo %d] ENTRA a sección crítica (4000 - 6000) ---\n", id);       }

        printf("[Hilo %d] %d\n", id, i);
        if (i == 6000) {
            printf("--- [Hilo %d] SALE de sección crítica ---\n\n", id);
            sem_post(&semaforo);
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilo1, hilo2;
    int id1 = 1, id2 = 2;
    if (sem_init(&semaforo, 0, 1) != 0) {
        perror("Error al inicializar el semáforo");
        return 1;
    }
    if (pthread_create(&hilo1, NULL, contar, &id1) != 0 ||
        pthread_create(&hilo2, NULL, contar, &id2) != 0) {
        perror("Error al crear los hilos");
        return 1;
    }
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    sem_destroy(&semaforo);

    return 0;
}

