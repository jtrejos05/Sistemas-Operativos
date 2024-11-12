#!/usr/bin/perl
#**************************************************************
#         		Pontificia Universidad Javeriana
#     Autor: J. Corredor
#     Fecha: Febrero 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************/


# Obtener el directorio de trabajo actual y almacenarlo en $Path
$Path = `pwd`;
chomp($Path);  # Remover el salto de línea del final de $Path

# Definir el nombre del ejecutable
$Nombre_Ejecutable = "MM_ejecutable";

# Definir los tamaños de la matriz que se usarán en las pruebas
@Size_Matriz = ("200", "300");

# Definir los números de hilos a usar en las pruebas
@Num_Hilos = (1, 2);

# Definir cuántas veces se ejecutará cada combinación de tamaño de matriz y número de hilos
$Repeticiones = 2;

# Bucle para iterar sobre cada tamaño de matriz
foreach $size (@Size_Matriz) {
    # Bucle para iterar sobre cada número de hilos
    foreach $hilo (@Num_Hilos) {
        # Definir el nombre del archivo de salida que almacenará los resultados de cada combinación
        $file = "$Path/$Nombre_Ejecutable-" . $size . "-Hilos-" . $hilo . ".dat";
        
        # Bucle para repetir la ejecución el número de veces indicado en $Repeticiones
        for ($i = 0; $i < $Repeticiones; $i++) {
            # Ejecución del comando y almacenamiento de la salida en el archivo (comentado aquí)
            # Descomentar la siguiente línea despues de prueba mas abajo, sirve para guardar la salida en el archivo:
            # system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");

            # Imprimir el comando que se ejecutaría en lugar de ejecutarlo (para depuración), eliminar despues de prueba
            printf("$Path/$Nombre_Ejecutable $size $hilo \n");
        }
        # Cerrar el archivo (aunque en este caso no se está escribiendo directamente)
        close($file);
        
        # Incrementar el contador $p (aunque aquí no se usa más adelante en el código)
        $p = $p + 1;
    }
}