# Nombre: Apolonio Villegas Diego Armando
# Práctica: 1
# Tema: yacc básico
# Opción: Calculadora de vectores
# Fecha 06/10/2021
# Grupo: 3CM17
# Materia: Compiladores

OBJS = vectores.tab.o vectores.o

vectores : $(OBJS)
	@gcc $(OBJS) -o vectores -lm
	@echo "-*Compilación completada*-"

vectores.o : vectores.c vectores.h
	@gcc -c vectores.c -o vectores.o

vectores.c :
	@touch vectores.c

vectores.tab.o : vectores.tab.c vectores.h
	@gcc -c vectores.tab.c -o vectores.tab.o

vectores.tab.c : vectores.y
	@bison vectores.y -o vectores.tab.c

.PHONY : all clean

all : vectores

clean :
	@rm -f $(OBJS) vectores.tab.c vectores
	@echo "-*Limpieza completada*-"
