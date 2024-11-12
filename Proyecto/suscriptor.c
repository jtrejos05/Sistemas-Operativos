/***************************************
Nombres: Time Quantum 
Fecha: 11/11/2024
Tema: Proyecto
Descripcion: Archivo publicador
***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_LARGO 100
#define MAX_TEMAS 5

// Función para crear un pipe si no existe
void crear_pipe_si_no_existe(const char *nombrePipe) {
    if (mkfifo(nombrePipe, 0666) == -1) {
        if (errno != EEXIST) { // Ignorar error si el pipe ya existe
            perror("Error al crear el pipe");
            exit(EXIT_FAILURE);
        }
    }
}

// Función para suscribirse a temas de interés
void suscribirse_a_temas(const char *pipeSSC) {
    int fd_pipe = open(pipeSSC, O_WRONLY);
    if (fd_pipe == -1) {
        perror("Error al abrir el pipe para enviar suscripciones");
        exit(EXIT_FAILURE);
    }

    char temas[MAX_TEMAS]; // Almacena los temas (un carácter por tema)
    int num_temas;

    // Solicitar al usuario los temas a los que quiere suscribirse
    printf("Ingrese el número de temas a suscribirse (1-%d): ", MAX_TEMAS);
    scanf("%d", &num_temas);
    getchar();  // Limpiar el buffer de entrada

    if (num_temas < 1 || num_temas > MAX_TEMAS) {
        fprintf(stderr, "Número de temas inválido. Debe ser entre 1 y %d.\n", MAX_TEMAS);
        close(fd_pipe);
        exit(EXIT_FAILURE);
    }

    // Leer los temas del usuario
    for (int i = 0; i < num_temas; i++) {
        printf("Ingrese el tema %d (A, E, C, P, S): ", i + 1);
        char tema;
        scanf(" %c", &tema);  // Leer un solo carácter

        // Validar que el tema sea uno de los permitidos
        if (tema == 'A' || tema == 'E' || tema == 'C' || tema == 'P' || tema == 'S') {
            temas[i] = tema;
        } else {
            fprintf(stderr, "Tema inválido '%c'. Debe ser A, E, C, P, o S.\n", tema);
            i--;  // Repetir la solicitud para este tema
            continue;
        }
    }

    // Enviar los temas seleccionados al Sistema de Comunicación
    write(fd_pipe, temas, num_temas);
    close(fd_pipe);
}

// Función para recibir y mostrar noticias del Sistema de Comunicación
void recibir_noticias(const char *pipeSSC) {
    int fd = open(pipeSSC, O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el pipe para recibir noticias");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_LARGO];
    while (read(fd, buffer, sizeof(buffer) - 1) > 0) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remover salto de línea al final
        printf("Noticia recibida: %s\n", buffer);
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    char *pipeSSC = NULL;

    // Parsear los argumentos de la línea de comandos
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-s") == 0) {
            pipeSSC = argv[i + 1];
        }
    }

    if (!pipeSSC) {
        fprintf(stderr, "Uso: suscriptor -s pipeSSC\n");
        exit(EXIT_FAILURE);
    }

    // Crear el pipe si no existe
    crear_pipe_si_no_existe(pipeSSC);

    // Suscribirse a los temas de interés y recibir noticias
    suscribirse_a_temas(pipeSSC);
    recibir_noticias(pipeSSC);

    return 0;
}

