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
#include <pthread.h>

#define MAX_LARGO_NOTICIA 100
#define MAX_SUSCRIPTORES 10
#define MAX_TEMAS 5

// Estructura para representar un suscriptor
typedef struct {
    int id;
    char temas[MAX_TEMAS];    // Almacena las categorías de interés
    int num_temas;
} Suscriptor;

Suscriptor suscriptores[MAX_SUSCRIPTORES];
int num_suscriptores = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Función para crear un pipe si no existe
void crear_pipe_si_no_existe(const char *nombrePipe) {
    if (mkfifo(nombrePipe, 0666) == -1 && errno != EEXIST) {
        perror("Error al crear el pipe");
        exit(EXIT_FAILURE);
    }
}

// Función para agregar un suscriptor
void agregar_suscriptor(int id, const char *temas, int num_temas) {
    pthread_mutex_lock(&mutex);
    if (num_suscriptores >= MAX_SUSCRIPTORES) {
        fprintf(stderr, "Error: Límite de suscriptores alcanzado\n");
        pthread_mutex_unlock(&mutex);
        return;
    }

    Suscriptor *nuevo_suscriptor = &suscriptores[num_suscriptores++];
    nuevo_suscriptor->id = id;
    nuevo_suscriptor->num_temas = num_temas;
    for (int i = 0; i < num_temas; i++) {
        nuevo_suscriptor->temas[i] = temas[i];
    }

    printf("Suscriptor %d agregado con temas %s\n", id, temas);
    pthread_mutex_unlock(&mutex);
}

// Función para manejar y distribuir noticias a los suscriptores interesados
void manejar_noticias(const char *pipePSC, const char *pipeSSC) {
    int fd_publicador = open(pipePSC, O_RDONLY);
    if (fd_publicador == -1) {
        perror("Error al abrir el pipe para noticias");
        exit(EXIT_FAILURE);
    }

    int fd_suscriptor = open(pipeSSC, O_RDWR);
    if (fd_suscriptor == -1) {
        perror("Error al abrir el pipe para enviar noticias");
        close(fd_publicador);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_LARGO_NOTICIA];
    char noticia[MAX_LARGO_NOTICIA];
    int index = 0;
    ssize_t bytes_leidos;

    // Bucle de lectura del pipe de noticias
    while ((bytes_leidos = read(fd_publicador, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_leidos] = '\0'; // Terminar la cadena leída

        for (int i = 0; i < bytes_leidos; i++) {
            if (buffer[i] == '\n') { // Detectar el final de una noticia
                noticia[index] = '\0'; // Terminar la noticia
                printf("Noticia recibida para procesar: %s\n", noticia);

                // Distribuir la noticia a los suscriptores interesados
                char tipo_noticia = noticia[0];
                pthread_mutex_lock(&mutex);
                for (int j = 0; j < num_suscriptores; j++) {
                    for (int k = 0; k < suscriptores[j].num_temas; k++) {
                        if (suscriptores[j].temas[k] == tipo_noticia) {
                            write(fd_suscriptor, noticia, strlen(noticia));
                            write(fd_suscriptor, "\n", 1);  // Añadir salto de línea como delimitador
                            break;
                        }
                    }
                }
                pthread_mutex_unlock(&mutex);

                // Reiniciar el índice para la próxima noticia
                index = 0;
                memset(noticia, 0, sizeof(noticia));
            } else {
                // Acumular caracteres en el buffer de noticia
                noticia[index++] = buffer[i];
                if (index >= MAX_LARGO_NOTICIA - 1) {
                    fprintf(stderr, "Error: Noticia demasiado larga, truncada.\n");
                    index = 0;
                    break;
                }
            }
        }
    }

    close(fd_publicador);
    close(fd_suscriptor);
}
// Función para manejar las suscripciones recibidas desde los suscriptores
void recibir_suscripciones(const char *pipeSSC) {
    int fd_suscriptor = open(pipeSSC, O_RDONLY);
    if (fd_suscriptor == -1) {
        perror("Error al abrir el pipe para recibir suscripciones");
        exit(EXIT_FAILURE);
    }

    char temas[MAX_TEMAS];
    int id = num_suscriptores + 1;

    while (read(fd_suscriptor, temas, sizeof(temas)) > 0) {
        int num_temas = strlen(temas);
        agregar_suscriptor(id++, temas, num_temas);
    }

    close(fd_suscriptor);
}

int main(int argc, char *argv[]) {
    char *pipePSC = NULL, *pipeSSC = NULL;
    int tiempoF = 1;

    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-p") == 0) pipePSC = argv[i + 1];
        else if (strcmp(argv[i], "-s") == 0) pipeSSC = argv[i + 1];
        else if (strcmp(argv[i], "-t") == 0) tiempoF = atoi(argv[i + 1]);
    }

    if (!pipePSC || !pipeSSC) {
        fprintf(stderr, "Uso: sistema -p pipePSC -s pipeSSC -t tiempoF\n");
        exit(EXIT_FAILURE);
    }

    // Crear los pipes si no existen
    crear_pipe_si_no_existe(pipePSC);
    crear_pipe_si_no_existe(pipeSSC);

    // Crear hilos para recibir suscripciones y manejar noticias
    pthread_t thread_suscripciones;
    pthread_create(&thread_suscripciones, NULL, (void *)recibir_suscripciones, pipeSSC);

    manejar_noticias(pipePSC, pipeSSC);

    pthread_join(thread_suscripciones, NULL);

    sleep(tiempoF);
    printf("El sistema de comunicación ha terminado su operación.\n");
    return 0;
}
