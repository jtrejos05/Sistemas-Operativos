/*****************************************************************************
*Nombre grupo: Time Quantum
*Fecha: 24/09/2024
*Tema: Taller Fork
*Descripcion: Aplicar los conceptos de procesos y comunicación entre procesos.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * @brief Lee un arreglo de enteros desde un archivo.
 * @param archivo Ruta del archivo que contiene el arreglo.
 * @param n Tamaño del arreglo.
 * @return Puntero al arreglo leído.
 */
int* leer_arreglo(const char* archivo, int n) {
    FILE* file = fopen(archivo, "r");  // Abre el archivo en modo de lectura.
    if (file == NULL) {  // Verifica si el archivo se abrió correctamente.
        perror("Error al abrir el archivo");  // Muestra un mensaje de error si no se pudo abrir.
        exit(EXIT_FAILURE);  // Finaliza el programa con error.
    }

    int* arreglo = (int*) malloc(n * sizeof(int));  // Reserva memoria para el arreglo.
    if (arreglo == NULL) {  // Verifica si la memoria se asignó correctamente.
        perror("Error al asignar memoria");  // Muestra un mensaje de error si falla la asignación de memoria.
        fclose(file);  // Cierra el archivo.
        exit(EXIT_FAILURE);  // Finaliza el programa con error.
    }

    // Lee los enteros del archivo y los guarda en el arreglo.
    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%d", &arreglo[i]) != 1) {  // Lee cada número y verifica si la lectura fue exitosa.
            perror("Error al leer el archivo");  // Muestra un error si la lectura falla.
            free(arreglo);  // Libera la memoria previamente asignada.
            fclose(file);  // Cierra el archivo.
            exit(EXIT_FAILURE);  // Finaliza el programa con error.
        }
    }

    fclose(file);  // Cierra el archivo después de leer los datos.
    return arreglo;  // Devuelve el puntero al arreglo leído.
}

/**
 * @brief Calcula la suma de un arreglo de enteros.
 * @param arreglo Puntero al arreglo.
 * @param n Tamaño del arreglo.
 * @return Suma del arreglo.
 */
int calcular_suma(int* arreglo, int n) {
    int suma = 0;  // Inicializa la suma en 0.
    // Recorre el arreglo y acumula la suma de sus elementos.
    for (int i = 0; i < n; i++) {
        suma += arreglo[i];  // Añade cada elemento del arreglo a la suma.
    }
    return suma;  // Retorna el valor de la suma total.
}

int main(int argc, char* argv[]) {
    // Verifica que se hayan pasado exactamente 4 argumentos (además del nombre del programa).
    if (argc != 5) {
        fprintf(stderr, "Uso: %s N1 archivo00 N2 archivo01\n", argv[0]);  // Muestra el uso correcto del programa.
        exit(EXIT_FAILURE);  // Finaliza el programa si los argumentos no son los correctos.
    }

    // Convierte los argumentos N1 y N2 a enteros.
    int N1 = atoi(argv[1]);
    int N2 = atoi(argv[3]);

    // Lee los arreglos desde los archivos proporcionados como argumentos.
    int* arregloA = leer_arreglo(argv[2], N1);
    int* arregloB = leer_arreglo(argv[4], N2);

    // Crea tres tuberías para la comunicación entre los procesos.
    int pipeA[2], pipeB[2], pipeC[2];

    // Verifica si la creación de las tuberías fue exitosa.
    if (pipe(pipeA) == -1 || pipe(pipeB) == -1 || pipe(pipeC) == -1) {
        perror("Error al crear las tuberías");  // Muestra un mensaje de error si falla la creación de las tuberías.
        exit(EXIT_FAILURE);  // Finaliza el programa con error.
    }

    pid_t pid1, pid2, pid3;

    // Crea el primer proceso hijo.
    pid1 = fork();
    if (pid1 == 0) {  // Código del primer hijo.
        // Cierra las tuberías que no va a utilizar.
        close(pipeA[0]);  // Cierra el lado de lectura de pipeA.
        close(pipeB[0]);  // Cierra el lado de lectura de pipeB.
        close(pipeB[1]);  // Cierra el lado de escritura de pipeB.
        close(pipeC[0]);  // Cierra el lado de lectura de pipeC.
        close(pipeC[1]);  // Cierra el lado de escritura de pipeC.

        // Calcula la suma de arregloA.
        int sumaA = calcular_suma(arregloA, N1);
        // Imprime el mensaje indicando que envía la suma.
        printf("Primer Hijo: [%d] Enviando Suma A: %d\n", getpid(), sumaA);

        // Escribe la suma en pipeA.
        if (write(pipeA[1], &sumaA, sizeof(int)) == -1) {
            perror("Error al escribir en pipeA");  // Muestra un error si falla la escritura en la tubería.
            exit(EXIT_FAILURE);  // Finaliza el proceso hijo con error.
        }
        close(pipeA[1]);  // Cierra el lado de escritura de pipeA.

        // Libera la memoria asignada.
        free(arregloA);
        free(arregloB);
        exit(0);  // Finaliza el primer proceso hijo.
    }

    // Crea el segundo proceso hijo.
    pid2 = fork();
    if (pid2 == 0) {  // Código del segundo hijo.
        // Cierra las tuberías que no va a utilizar.
        close(pipeA[1]);  // Cierra el lado de escritura de pipeA.
        close(pipeA[0]);  // Cierra el lado de lectura de pipeA.
        close(pipeB[0]);  // Cierra el lado de lectura de pipeB.
        close(pipeC[0]);  // Cierra el lado de lectura de pipeC.
        close(pipeC[1]);  // Cierra el lado de escritura de pipeC.

        // Calcula la suma de arregloB.
        int sumaB = calcular_suma(arregloB, N2);
        // Imprime el mensaje indicando que envía la suma.
        printf("Segundo Hijo: [%d] Enviando Suma B: %d\n", getpid(), sumaB);

        // Escribe la suma en pipeB.
        if (write(pipeB[1], &sumaB, sizeof(int)) == -1) {
            perror("Error al escribir en pipeB");  // Muestra un error si falla la escritura en la tubería.
            exit(EXIT_FAILURE);  // Finaliza el proceso hijo con error.
        }
        close(pipeB[1]);  // Cierra el lado de escritura de pipeB.

        // Libera la memoria asignada.
        free(arregloA);
        free(arregloB);
        exit(0);  // Finaliza el segundo proceso hijo.
    }

    // Crea el tercer proceso hijo (Nieto).
    pid3 = fork();
    if (pid3 == 0) {  // Código del nieto (tercer hijo).
        // Cierra las tuberías que no va a utilizar.
        close(pipeA[1]);  // Cierra el lado de escritura de pipeA.
        close(pipeB[1]);  // Cierra el lado de escritura de pipeB.
        close(pipeC[0]);  // Cierra el lado de lectura de pipeC.

        // Calcula la suma de ambos arreglos.
        int sumaA = calcular_suma(arregloA, N1);
        int sumaB = calcular_suma(arregloB, N2);
        int sumaTotal = sumaA + sumaB;  // Suma total de ambos arreglos.
        // Imprime el mensaje indicando que recibe el total.
        printf("Gran Hijo: [%d] recibiendo total: %d\n", getpid(), sumaTotal);

        // Escribe la suma total en pipeC.
        if (write(pipeC[1], &sumaTotal, sizeof(int)) == -1) {
            perror("Error al escribir en pipeC");  // Muestra un error si falla la escritura en la tubería.
            exit(EXIT_FAILURE);  // Finaliza el proceso con error.
        }

        // Cierra las tuberías abiertas.
        close(pipeA[0]);
        close(pipeB[0]);
        close(pipeC[1]);

        // Libera la memoria asignada.
        free(arregloA);
        free(arregloB);
        exit(0);  // Finaliza el proceso nieto.
    }

    // Código del proceso padre.
    // Cierra los lados de escritura de las tuberías ya que el padre solo va a leer.
    close(pipeA[1]);
    close(pipeB[1]);
    close(pipeC[1]);

    int sumaA, sumaB, sumaTotal;
    // Lee la suma enviada por el primer hijo desde pipeA.
    if (read(pipeA[0], &sumaA, sizeof(int)) == -1) {
        perror("Error al leer pipeA en el padre");  // Muestra un error si falla la lectura.
        exit(EXIT_FAILURE);  // Finaliza con error.
    }
    // Lee la suma enviada por el segundo hijo desde pipeB.
    if (read(pipeB[0], &sumaB, sizeof(int)) == -1) {
        perror("Error al leer pipeB en el padre");  // Muestra un error si falla la lectura.
        exit(EXIT_FAILURE);  // Finaliza con error.
    }
    // Lee la suma total enviada por el nieto desde pipeC.
    if (read(pipeC[0], &sumaTotal, sizeof(int)) == -1) {
        perror("Error al leer pipeC en el padre");  // Muestra un error si falla la lectura.
        exit(EXIT_FAILURE);  // Finaliza con error.
    }

    // Imprime los resultados finales.
    printf("Padre: [%d] recibiendo total: %d\n", getpid(), sumaTotal);

    // Cierra los lados de lectura de las tuberías.
    close(pipeA[0]);
    close(pipeB[0]);
    close(pipeC[0]);

    // Espera a que terminen los tres procesos hijos.
    wait(NULL);
    wait(NULL);
    wait(NULL);

    // Libera la memoria asignada.
    free(arregloA);
    free(arregloB);

    return 0;  // Finaliza el proceso padre exitosamente.
}

