/*********************************
Autor:Juan Martin Trejos
Materia:Sitemas Opertaivos
Descripcion: Funcionalidad del la funcion Calloc
***********************************/
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr;
    int n = 5;

    // Asignar memoria para 5 enteros usando calloc
    arr = (int*) calloc(n, sizeof(int));

    if (arr == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    // Imprimir el array inicializado
    printf("Array inicializado con calloc():\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Liberar la memoria asignada
    free(arr);

    return 0;
}
