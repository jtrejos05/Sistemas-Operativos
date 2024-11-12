GCC = gcc
FLAGS = -c

PROGS = sistema publicador suscriptor

# Compilación de cada programa
sistema: 
		$(GCC) $(FLAGS) sistema.c
		$(GCC) sistema.o -o sistema

publicador: 
		$(GCC) $(FLAGS) publicador.c
		$(GCC) publicador.o -o publicador

suscriptor: 
		$(GCC) $(FLAGS) suscriptor.c
		$(GCC) suscriptor.o -o suscriptor

# Limpiar los archivos compilados
clear:
		$(RM) $(PROGS) *.o