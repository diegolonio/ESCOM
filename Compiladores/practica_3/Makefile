# Nombre: Apolonio Villegas Diego Armando 
# Práctica: 3
# Tema: Tabla de símbolos
# Opción: Calculadora de vectores
# Fecha 15/11/2021
# Grupo: 3CM17
# Materia: Compiladores

OBJS = vectores.tab.o vectores.o simbolo.o inicializacion.o

vectores : $(OBJS)
	@gcc $(OBJS) -o vectores -lm
	@echo "-*Compilación completada*-"

vectores.tab.o : vectores.tab.c vectores.h
	@gcc -c vectores.tab.c -o vectores.tab.o

vectores.tab.c : vectores.y
	@bison -Wno-conflicts-sr -Wno-conflicts-rr -d vectores.y -o vectores.tab.c

vectores.o : vectores.c vectores.h
	@gcc -c vectores.c -o vectores.o

vectores.c :
	@touch vectores.c

simbolo.o : simbolo.c vectores.h vectores.tab.h
	@gcc -c simbolo.c -o simbolo.o

inicializacion.o : inicializacion.c vectores.h vectores.tab.h
	@gcc -c inicializacion.c -o inicializacion.o

.PHONY : all clean

all : vectores

clean :
	@rm -f $(OBJS) vectores.tab.[ch] vectores
	@echo "-*Limpieza completada*-"
