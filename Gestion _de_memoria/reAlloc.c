/****************************
Funcion Malloc
Juan Martin Trejos
Fecha: 17/09/2024
Descripcion: Funcionalidad de la funcion malloc
*****************************/


#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr;
    int n = 5;

    // Asignar memoria para 5 enteros usando malloc
    arr = (int*) malloc(n * sizeof(int));

    if (arr == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    // Inicializar el array con valores
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    // Imprimir el array inicial
    printf("Array original:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Redimensionar el array para que contenga 10 enteros
    n = 10;
    arr = (int*) realloc(arr, n * sizeof(int));

    if (arr == NULL) {
        printf("Error al redimensionar la memoria.\n");
        return 1;
    }

    // Inicializar los nuevos elementos con valores
    for (int i = 5; i < n; i++) {
        arr[i] = i + 1;
    }

    // Imprimir el array redimensionado
    printf("Array redimensionado con realloc():\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Liberar la memoria asignada
    free(arr);

    return 0;
}
