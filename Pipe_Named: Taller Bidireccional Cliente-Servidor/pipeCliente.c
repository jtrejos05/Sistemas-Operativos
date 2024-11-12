/************************************
Autor:Juan Martin Trejos
Fecha: 1/10/2024
Materia:Sistemas Operativos
Tema: Pipes Bidireccionales
Topico: Pipe cliente
Descripcion: Se tiene un archivo .C el cual tiene el pipe que envia los mensajes al cliente.
***************************************/


#include <string.h>      // Para funciones de manipulación de cadenas (e.g., strlen, strcmp, strcpy)
#include <stdio.h>       // Para funciones de entrada/salida (e.g., printf, perror)
#include <unistd.h>      // Para funciones de sistema UNIX (e.g., read, write, close)
#include <sys/types.h>   // Para tipos de datos específicos del sistema
#include <sys/stat.h>    // Para manejo de atributos de archivos (e.g., permisos)
#include <fcntl.h>       // Para funciones de control de archivos (e.g., open)

// Definimos la ruta del archivo FIFO
#define FIFO_FILE "/tmp/fifo_twoway"

int main() {
    int fifo_archivo;          // Descriptor de archivo para el FIFO
    int es_fin_proceso;        // Variable para verificar si el usuario ingresó "end"
    int longitud_cadena;       // Longitud de la cadena ingresada por el usuario
    int bytes_leidos;          // Cantidad de bytes leídos desde el FIFO
    char buffer_entrada[80];   // Buffer para almacenar la cadena que el usuario ingresa
    char comando_fin[5];       // Cadena "end" para verificar el comando de fin de comunicación

    printf("FIFO_CLIENT: Enviar mensajes infinitamente, para terminar ingrese \"end\"\n");

    // Abre el FIFO en modo de lectura/escritura; el servidor es quien debe crear el FIFO
    fifo_archivo = open(FIFO_FILE, O_RDWR);
    if (fifo_archivo == -1) {
        perror("Error al abrir el archivo FIFO");
        return 1;
    }

    // Asigna el valor "end" a la variable comando_fin para usarla más adelante en la comparación
    strcpy(comando_fin, "end");

    // Bucle principal para enviar y recibir mensajes a través del FIFO
    while (1) {
        printf("Ingrese una cadena: ");
        fgets(buffer_entrada, sizeof(buffer_entrada), stdin);  // Lee la entrada del usuario
        longitud_cadena = strlen(buffer_entrada);

        // Elimina el salto de línea que fgets inserta al final de la cadena
        if (longitud_cadena > 1) {
            buffer_entrada[longitud_cadena - 1] = '\0';
        }

        // Compara la cadena ingresada con "end" para determinar si debe finalizar
        es_fin_proceso = strcmp(buffer_entrada, comando_fin);

        if (es_fin_proceso != 0) {
            // Si no es "end", envía la cadena al servidor a través del FIFO
            write(fifo_archivo, buffer_entrada, strlen(buffer_entrada));
            printf("FIFOCLIENT: Cadena enviada: \"%s\" y longitud de la cadena es %d\n", buffer_entrada, (int)strlen(buffer_entrada));

            // Lee la respuesta del servidor a través del FIFO
            bytes_leidos = read(fifo_archivo, buffer_entrada, sizeof(buffer_entrada));
            if (bytes_leidos <= 0) {
                perror("Error al leer del FIFO o la conexión se cerró");
                close(fifo_archivo);
                break;
            }

            // Agrega un terminador nulo al final de la cadena recibida para evitar basura
            buffer_entrada[bytes_leidos] = '\0';
            printf("FIFOCLIENT: Cadena recibida: \"%s\" y longitud es %d\n", buffer_entrada, (int)strlen(buffer_entrada));
        } else {
            // Si el usuario ingresa "end", envía el comando al servidor y cierra el FIFO
            write(fifo_archivo, buffer_entrada, strlen(buffer_entrada));
            printf("FIFOCLIENT: Cadena enviada: \"%s\" y longitud de la cadena es %d\n", buffer_entrada, (int)strlen(buffer_entrada));
            close(fifo_archivo);
            break;
        }
    }

    return 0;
}

