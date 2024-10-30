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

int main() {
    int fifo_archivo;          // Cambiado de 'fd'
    int es_fin_proceso;        // Cambiado de 'end_process'
    int longitud_cadena;       // Cambiado de 'stringlen'
    int bytes_leidos;          // Cambiado de 'read_bytes'
    char buffer_entrada[80];   // Cambiado de 'readbuf'
    char comando_fin[5];       // Cambiado de 'end_str'

    printf("FIFO_CLIENT: Enviar mensajes infinitamente, para terminar ingrese \"end\"\n");

    fifo_archivo = open(FIFO_FILE, O_RDWR);  // Eliminado O_CREAT, ya que el servidor debe crear el FIFO
    if (fifo_archivo == -1) {
        perror("Error al abrir el archivo FIFO");
        return 1;
    }

    strcpy(comando_fin, "end");

    while (1) {
        printf("Ingrese una cadena: ");
        fgets(buffer_entrada, sizeof(buffer_entrada), stdin);
        longitud_cadena = strlen(buffer_entrada);

        if (longitud_cadena > 1) {
            buffer_entrada[longitud_cadena - 1] = '\0';  // Eliminar el salto de línea
        }

        es_fin_proceso = strcmp(buffer_entrada, comando_fin);

        if (es_fin_proceso != 0) {
            write(fifo_archivo, buffer_entrada, strlen(buffer_entrada));
            printf("FIFOCLIENT: Cadena enviada: \"%s\" y longitud de la cadena es %d\n", buffer_entrada, (int)strlen(buffer_entrada));

            bytes_leidos = read(fifo_archivo, buffer_entrada, sizeof(buffer_entrada));
            if (bytes_leidos <= 0) {
                perror("Error al leer del FIFO o la conexión se cerró");
                close(fifo_archivo);
                break;
            }
            buffer_entrada[bytes_leidos] = '\0';
            printf("FIFOCLIENT: Cadena recibida: \"%s\" y longitud es %d\n", buffer_entrada, (int)strlen(buffer_entrada));
        } else {
            write(fifo_archivo, buffer_entrada, strlen(buffer_entrada));
            printf("FIFOCLIENT: Cadena enviada: \"%s\" y longitud de la cadena es %d\n", buffer_entrada, (int)strlen(buffer_entrada));
            close(fifo_archivo);
            break;
        }
    }

    return 0;
}
