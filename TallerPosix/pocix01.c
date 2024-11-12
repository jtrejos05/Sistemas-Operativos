/***********************************
Autor: Juan Martin Trejos
Fecha: 15/10/2024
Tema: P_threads Posix
Profesor: Jhon Corredor Franco
************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

/*Funcion cualquiera para ejecutar el thread*/
static void *funcion(void *argumentos){
  size_t job=*(size_t *)argumentos;
 printf("\n\t Job %zu \n", job);
  return NULL;
  
}

int main(int argc, char *argv[]){
  
  int N=(int)atoi(argv[1]);
  size_t jobs[N];
  pthread_t hilos[N];
  for(int i=0;i<N;i++){
    jobs[i]=i;
    //Creacion de hilos y envio de funcion
    pthread_create(&hilos[i],NULL,funcion,jobs+i);
  }
  for(int i=0;i<N;i++){
    pthread_join(hilos[i], NULL);
  }
  return 0;
}