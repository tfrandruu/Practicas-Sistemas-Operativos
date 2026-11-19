#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define COMIDAS 3

pthread_mutex_t tenedores[NUM_FILOSOFOS];

void* filosofo(void* arg) {
    int id = *(int*)arg;
    int izq = id;
    int der = (id + 1) % NUM_FILOSOFOS;

    for (int i = 1; i <= COMIDAS; i++) {
        printf("[Filosofo %d] Esta pensando...\n", id);
        usleep(100000);

        if (id % 2 == 0) {
            pthread_mutex_lock(&tenedores[izq]);
            printf("[Filosofo %d] Tomo tenedor izquierdo (%d)\n", id, izq);
            pthread_mutex_lock(&tenedores[der]);
            printf("[Filosofo %d] Tomo tenedor derecho (%d)\n", id, der);
        } else {
            pthread_mutex_lock(&tenedores[der]);
            printf("[Filosofo %d] Tomo tenedor derecho (%d)\n", id, der);
            pthread_mutex_lock(&tenedores[izq]);
            printf("[Filosofo %d] Tomo tenedor izquierdo (%d)\n", id, izq);
        }

        printf("  --> [Filosofo %d] COMIENDO (comida %d de %d)\n", id, i, COMIDAS);
        usleep(150000);

        pthread_mutex_unlock(&tenedores[izq]);
        pthread_mutex_unlock(&tenedores[der]);
        printf("[Filosofo %d] Termino de comer y solto tenedores (%d y %d)\n", id, izq, der);
    }

    printf("*** [Filosofo %d] Satisfecho, se retira de la mesa ***\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&tenedores[i], NULL);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        if (pthread_create(&hilos[i], NULL, filosofo, &ids[i]) != 0) {
            perror("Error al crear el hilo del filosofo");
            return 1;
        }
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&tenedores[i]);
    }

    printf("\nTodos los filosofos terminaron de cenar sin interbloqueo.\n");
    return 0;
}
