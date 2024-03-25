all: calculadora
	@echo "-*Compilación completada*-"

calculadora: main.o calculadora.o redes.o
	@gcc main.o calculadora.o redes.o -o calculadora

main.o: main.c ../../include/calculadora.h ../../include/redes.h
	@gcc -c main.c -o main.o

calculadora.o: ../../src/calculadora.c ../../include/calculadora.h ../../include/redes.h
	@gcc -c ../../src/calculadora.c -o calculadora.o

redes.o: ../../src/redes.c ../../include/redes.h
	@gcc -c ../../src/redes.c -o redes.o

.PHONY: clean

clean:
	@rm -f calculadora subredes hosts *.o
	@echo "-*Limpieza completada*-"
