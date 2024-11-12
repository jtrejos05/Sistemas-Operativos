/************************************
Autor:Juan Martin Trejos
Fecha: 1/10/2024
Materia:Sistemas Operativos
Tema: Pipes Bidireccionales
Topico: Pipe servidor
Descripcion: Se tiene un archivo .C el cual tiene el pipe que recibe el mensaje del cliente
***************************************/

#include <string.h>      // Para operaciones con cadenas (e.g., strlen, strcmp, strcpy)
#include <stdio.h>       // Para funciones de entrada/salida (e.g., printf, perror)
#include <unistd.h>      // Para funciones de sistema UNIX (e.g., read, write, close)
#include <sys/types.h>   // Para tipos de datos específicos del sistema
#include <sys/stat.h>    // Para manejo de atributos de archivos (e.g., permisos)
#include <fcntl.h>       // Para funciones de control de archivos (e.g., open)

// Definimos la ruta del archivo FIFO
#define FIFO_FILE "/tmp/fifo_twoway"

// Declaración de la función que invierte una cadena
void invertir_cadena(char *);

int main() {
    int fifo_archivo;             // Descriptor de archivo para el FIFO
    char buffer_lectura[80];      // Buffer para almacenar datos leídos del FIFO
    char comando_fin[10];         // Comando para identificar el fin de la comunicación
    int es_fin;                   // Variable para verificar si se recibe el comando "end"
    int bytes_leidos;             // Cantidad de bytes leídos desde el FIFO

    // Crear el FIFO con permisos 0640 si no existe (usuario: lectura/escritura, grupo: lectura)
    mkfifo(FIFO_FILE, 0640);
    
    // Copiamos el texto "end" en `comando_fin` para usarlo más adelante
    strcpy(comando_fin, "end");

    // Abre el FIFO en modo de lectura/escritura
    fifo_archivo = open(FIFO_FILE, O_RDWR);
    if (fifo_archivo == -1) {
        perror("Error al abrir el archivo FIFO");
        return 1;
    }

    // Bucle principal para leer y procesar datos del FIFO
    while (1) {
        // Lee datos del FIFO y los almacena en `buffer_lectura`
        bytes_leidos = read(fifo_archivo, buffer_lectura, sizeof(buffer_lectura) - 1);
        if (bytes_leidos <= 0) {
            perror("Error al leer del FIFO o la conexión se cerró");
            close(fifo_archivo);
            break;
        }

        // Agregamos el terminador de fin de cadena al buffer leído para evitar basura
        buffer_lectura[bytes_leidos] = '\0';
        
        // Imprime la cadena recibida y su longitud
        printf("FIFOSERVER: Cadena recibida: \"%s\" y longitud es %d\n", buffer_lectura, (int)strlen(buffer_lectura));

        // Compara el buffer con el comando "end" para verificar si debe terminar el programa
        es_fin = strcmp(buffer_lectura, comando_fin);
        if (es_fin == 0) {
            // Si el comando "end" se recibe, se cierra el FIFO y se rompe el bucle
            close(fifo_archivo);
            break;
        }

        // Invierte la cadena recibida
        invertir_cadena(buffer_lectura);
        printf("FIFOSERVER: Enviando cadena invertida: \"%s\" y longitud es %d\n", buffer_lectura, (int)strlen(buffer_lectura));

        // Escribe la cadena invertida de vuelta en el FIFO
        int bytes_escritos = write(fifo_archivo, buffer_lectura, strlen(buffer_lectura));
        if (bytes_escritos == -1) {
            perror("Error al escribir en el FIFO");
            close(fifo_archivo);
            break;
        }

        // Hace una pausa de 2 segundos para asegurar que el otro proceso lea la respuesta
        sleep(2);
    }

    return 0;
}

// Función para invertir una cadena de caracteres en su lugar
void invertir_cadena(char *cadena) {
    int ultimo, limite, primero;
    char temporal;

    // Inicializa los índices de inicio y final para el intercambio
    ultimo = strlen(cadena) - 1;
    limite = ultimo / 2;
    primero = 0;

    // Intercambia los caracteres desde los extremos hacia el centro
    while (primero < ultimo) {
        temporal = cadena[primero];
        cadena[primero] = cadena[ultimo];
        cadena[ultimo] = temporal;
        primero++;
        ultimo--;
    }
}


                                 