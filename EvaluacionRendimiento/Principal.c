/**************************************************************
		Pontificia Universidad Javeriana
	Autor: J corredor 
	Fecha: Noviembre 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del 
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "Biblioteca.h" // se incluye el arvhivo que tiene las funciones de multiplicacion de matricez

// Punteros globales a las matrices A, B y C
double *mA, *mB, *mC;
// Tamaño de la matriz
static double MEM_CHUNK[DATA_SIZE];
// Mutex para sincronización de hilos
pthread_mutex_t MM_mutex;
// Variables para medir el tiempo
struct timeval start, stop;

int main(int argc, char *argv[]) {
    // Verificar el número de argumentos ingresados
    if (argc < 3) {
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;    
    }

    // Tamaño de la matriz y cantidad de hilos
    int SZ = atoi(argv[1]); 
    int n_threads = atoi(argv[2]); 

    // Declaración de los hilos y atributos de los hilos
    pthread_t p[n_threads];
    pthread_attr_t atrMM;

    // Asignación de memoria para las matrices A, B y C
    mA = MEM_CHUNK;
    mB = mA + SZ * SZ;
    mC = mB + SZ * SZ;

    // Llenar las matrices A y B con valores iniciales y mostrar
    llenar_matriz(SZ);
    print_matrix(SZ, mA);
    print_matrix(SZ, mB);

    // Inicializar el tiempo y configuraciones de los hilos
    inicial_tiempo();
    pthread_mutex_init(&MM_mutex, NULL);
    pthread_attr_init(&atrMM);
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    // Crear los hilos para la multiplicación de matrices
    for (int j = 0; j < n_threads; j++) {
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
        datos->idH = j;
        datos->nH = n_threads;
        datos->N = SZ;
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos);
    }

    // Esperar a que todos los hilos terminen
    for (int j = 0; j < n_threads; j++) {
        pthread_join(p[j], NULL);
    }

    // Calcular y mostrar el tiempo final
    final_tiempo();
    
    // Imprimir la matriz resultante
    print_matrix(SZ, mC);

    // Liberar recursos
    pthread_attr_destroy(&atrMM);
    pthread_mutex_destroy(&MM_mutex);
    pthread_exit(NULL);
}