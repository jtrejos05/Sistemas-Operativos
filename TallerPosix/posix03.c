#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

int counter = 0;
int max = 5; // Cambiado a 5 para que coincida con el número de hilos
int limit = 10; // Limite de producción

void* productor();
void* consumidor();

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVarProd = PTHREAD_COND_INITIALIZER;
pthread_cond_t condVarCons = PTHREAD_COND_INITIALIZER;

int main() {
    pthread_t proHilo[max], conHilo[max];
    int i;
    for (i = 0; i < max; i++) {
        pthread_create(&proHilo[i], NULL, productor, NULL);
        pthread_create(&conHilo[i], NULL, consumidor, NULL);
    }
    for (i = 0; i < max; i++) {
        pthread_join(proHilo[i], NULL);
        pthread_join(conHilo[i], NULL);
    }
    return 0;
}

void* productor() {
    while (1) {
        pthread_mutex_lock(&counter_mutex);
        while (counter >= limit) {
            pthread_cond_wait(&condVarProd, &counter_mutex);
        }

        counter++;
        printf("Soy el productor %ld valor contador = %d \n", pthread_self(), counter);
        pthread_cond_signal(&condVarCons);
        pthread_mutex_unlock(&counter_mutex);

        if (counter == 5) {
            sleep(1);
        }
    }
}

void* consumidor() {
    while (1) {
        sleep(1);
        pthread_mutex_lock(&counter_mutex);
        while (counter <= 0) {
            pthread_cond_wait(&condVarCons, &counter_mutex);
        }

        printf("Soy el consumidor %ld valor contador = %d \n", pthread_self(), counter);
        counter--;
        pthread_cond_signal(&condVarProd);
        pthread_mutex_unlock(&counter_mutex);
    }
}
