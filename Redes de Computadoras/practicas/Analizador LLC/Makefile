all: analizador
	@echo "-*Compilación completada*-"

analizador: main.o llc.o redes.o
	@gcc main.o llc.o redes.o -o analizador

main.o: main.c ../../include/llc.h ../../include/redes.h
	@gcc -c main.c -o main.o

llc.o: ../../src/llc.c ../../include/llc.h ../../include/redes.h
	@gcc -c ../../src/llc.c -o llc.o

redes.o: ../../src/redes.c ../../include/redes.h
	@gcc -c ../../src/redes.c -o redes.o

.PHONY: clean

clean:
	@rm -f analizador *.o
	@echo "-*Limpieza completada*-"
