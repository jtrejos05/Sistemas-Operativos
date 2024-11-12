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
#define MAX_LARGO_NOTICIA 80

// Función para crear un pipe si no existe
void crear_pipe_si_no_existe(const char *nombrePipe) {
    if (mkfifo(nombrePipe, 0666) == -1 && errno != EEXIST) {
        perror("Error al crear el pipe");
        exit(EXIT_FAILURE);
    }
}

// Función para validar el formato de una noticia
int validar_formato_noticia(const char *linea) {
    if (strlen(linea) > MAX_LARGO_NOTICIA) {
        fprintf(stderr, "Error: La noticia excede 80 caracteres: %s\n", linea);
        return 0;
    }
    char tipo = linea[0];
    if (!(tipo == 'A' || tipo == 'E' || tipo == 'C' || tipo == 'P' || tipo == 'S')) {
        fprintf(stderr, "Error: Categoría de noticia inválida '%c' en: %s\n", tipo, linea);
        return 0;
    }
    if (linea[1] != ':' || linea[strlen(linea) - 1] != '.') {
        fprintf(stderr, "Error: Formato incorrecto en: %s\n", linea);
        return 0;
    }
    return 1;
}

// Función para enviar datos al pipe con reintentos si el buffer está lleno
ssize_t escribir_con_reintento(int fd, const char *buffer, size_t longitud) {
    ssize_t total_escrito = 0;
    while (total_escrito < longitud) {
        ssize_t escrito = write(fd, buffer + total_escrito, longitud - total_escrito);
        if (escrito == -1) {
            if (errno == EAGAIN || errno == EINTR) {
                // Si el pipe está temporalmente lleno o la escritura se interrumpe, reintentamos
                continue;
            } else {
                perror("Error al escribir en el pipe");
                return -1;
            }
        }
        total_escrito += escrito;
    }
    return total_escrito;
}

// Función para enviar noticias al Sistema de Comunicación
void enviar_noticias(const char *archivo, const char *pipePSC, int intervalo) {
    FILE *f = fopen(archivo, "r");
    if (!f) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    int pipe_fd = open(pipePSC, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Error al abrir el pipe");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    char linea[MAX_LARGO];
    while (fgets(linea, MAX_LARGO, f)) {
        linea[strcspn(linea, "\n")] = '\0';
        if (!validar_formato_noticia(linea)) {
            continue;
        }
        strcat(linea, "\n"); // Añadir delimitador al final de la noticia
        printf("Enviando noticia: %s\n", linea);
        
        if (escribir_con_reintento(pipe_fd, linea, strlen(linea)) == -1) {
            fprintf(stderr, "Error: No se pudo enviar la noticia '%s'\n", linea);
        }
        
        sleep(intervalo);
    }

    close(pipe_fd);
    fclose(f);
    printf("Fin del envío de noticias.\n");
}

int main(int argc, char *argv[]) {
    char *pipePSC = NULL, *archivo = NULL;
    int intervalo = 1;

    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-p") == 0) pipePSC = argv[i + 1];
        else if (strcmp(argv[i], "-f") == 0) archivo = argv[i + 1];
        else if (strcmp(argv[i], "-t") == 0) intervalo = atoi(argv[i + 1]);
    }

    if (!pipePSC || !archivo) {
        fprintf(stderr, "Uso: publicador -p pipePSC -f archivo -t intervalo\n");
        exit(EXIT_FAILURE);
    }

    crear_pipe_si_no_existe(pipePSC);
    sleep(1);
    enviar_noticias(archivo, pipePSC, intervalo);
    return 0;
}