# Proyecto para Creación de Procesos en C

Este repositorio contiene un programa en C que ilustra cómo se generan procesos hijos mediante el uso de la función `fork()`. A través de un ciclo, el programa crea una cantidad específica de procesos hijos y muestra información detallada sobre cada uno.

## Descripción del Programa

El archivo `fork_procesos.c` permite al usuario definir cuántos procesos hijos desea crear. Cada vez que se invoca la función `fork()`, se genera un nuevo proceso, y el programa muestra un mensaje que indica si el proceso corresponde a un hijo o al padre.

### Funciones Principales

- **creacion_fork(int linea)**: 
  - Genera un nuevo proceso hijo usando `fork()`.
  - Imprime un mensaje que indica si el proceso generado es un hijo o el padre, junto con el ID del proceso.
  
- **main(int argc, char *argv[])**: 
  - Función principal del programa.
  - Verifica que el usuario haya indicado el número de procesos a crear.
  - Ejecuta `creacion_fork` dentro de un ciclo para crear la cantidad especificada de procesos.

# Guía para Compilar y Ejecutar el Proyecto

Aquí se incluyen los pasos para compilar y ejecutar el proyecto completo. Espero que esta guía sea de gran ayuda.

## Instalación de g++ con MinGW en Windows (Este paso puede omitirse si ya tienes el compilador o usas un sistema Unix)

Para compilar este proyecto, se recomienda usar `g++`. Puedes instalarlo siguiendo la guía de [MSYS2](https://www.msys2.org/), que proporciona `g++`, así como `gcc` y `gdb`.

## Compilación

Para compilar el proyecto, abre la consola de tu preferencia y escribe el siguiente comando, reemplazando los parámetros según tus necesidades:

```bash
g++ [nombre_del_fichero(s) (.c, .cpp o .cxx)] -o [nombre_del_ejecutable]
