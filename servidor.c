/**************************************************************
    Nombre:Juan Martin Trejos
  *Fecha:29 de Octubre
  *Autor_John Corredor
  *Materia:Sistemas Operativos
  *Tema:Fork
  *Topico:Productor y consumidor
  *
  *****************************************************************************/


#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_FILE "/tmp/fifo_twoway"
void invertir_cadena(char *);

int main() {
    int fifo_archivo;             // Cambiado de 'fd'
    char buffer_lectura[80];      // Cambiado de 'readbuf'
    char comando_fin[10];         // Cambiado de 'end'
    int es_fin;                   // Cambiado de 'to_end'
    int bytes_leidos;             // Cambiado de 'read_bytes'

    /* Crear el FIFO si no existe */
    mkfifo(FIFO_FILE, 0640);
    strcpy(comando_fin, "end");

    fifo_archivo = open(FIFO_FILE, O_RDWR);
    if (fifo_archivo == -1) {
        perror("Error al abrir el archivo FIFO");
        return 1;
    }

    while (1) {
        bytes_leidos = read(fifo_archivo, buffer_lectura, sizeof(buffer_lectura));
        if (bytes_leidos <= 0) {
            perror("Error al leer del FIFO o la conexión se cerró");
            close(fifo_archivo);
            break;
        }

        buffer_lectura[bytes_leidos] = '\0';
        printf("FIFOSERVER: Cadena recibida: \"%s\" y longitud es %d\n", buffer_lectura, (int)strlen(buffer_lectura));

        es_fin = strcmp(buffer_lectura, comando_fin);
        if (es_fin == 0) {
            close(fifo_archivo);
            break;
        }

        invertir_cadena(buffer_lectura);
        printf("FIFOSERVER: Enviando cadena invertida: \"%s\" y longitud es %d\n", buffer_lectura, (int)strlen(buffer_lectura));

        int bytes_escritos = write(fifo_archivo, buffer_lectura, strlen(buffer_lectura));
        if (bytes_escritos == -1) {
            perror("Error al escribir en el FIFO");
            close(fifo_archivo);
            break;
        }

        /* sleep - Esto asegura que el otro proceso lea este mensaje,
           de lo contrario, este proceso podría recuperar el mensaje */
        sleep(2);
    }

    return 0;
}

void invertir_cadena(char *cadena) {
    int ultimo, limite, primero;
    char temporal;
    ultimo = strlen(cadena) - 1;
    limite = ultimo / 2;
    primero = 0;

    while (primero < ultimo) {
        temporal = cadena[primero];
        cadena[primero] = cadena[ultimo];
        cadena[ultimo] = temporal;
        primero++;
        ultimo--;
    }
}
