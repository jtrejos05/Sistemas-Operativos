/***********************************************************
Fecha: 08/10/2024
Autor: Juan Martin Trejos Vanegas
Materia: Sisteas operativos
Tema: Fork
Topico: Creacion funcion Fork()
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * Crea un nuevo proceso hijo utilizando la función fork.
 * 
 * @param linea Número de línea actual en la que se está creando el proceso.
 * 
 * Ejemplo:
 * creacion_fork(3); // Crearía un proceso hijo en la línea 3.
 */
void creacion_fork(int linea){
  pid_t proceso; 
  proceso = fork();
  if(proceso<0){
    printf("Error creación proceso\n");
    exit(1);
  }else if (proceso==0)
    printf("%d: Proceso =HIJO=: %d\n",linea, getpid());
  else
    printf("%d: Proceso =PADRE= %d\n",linea, getpid());
}

/**
 * Función principal del programa.
 * 
 * Crea un número determinado de procesos hijos utilizando la función creacion_fork.
 * 
 * @param argc Número de argumentos pasados al programa.
 * @param argv Arreglo de argumentos pasados al programa.
 * 
 * Ejemplo:
 * ./programa 5 // Crearía 5 procesos hijos.
 */
int main(int argc, char *argv[]){
  if(argc != 2){
    printf("Uso: %s <número de procesos>\n", argv[0]);
    return 1;
  }
  
  int p = (int) atoi(argv[1]);
  
  for(int i=0; i<p; i++)
    creacion_fork(i);
 
  printf("\n---\n\n");
  return 0;