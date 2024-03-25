all: portscan
	@echo "-*Compilación completada*-"

portscan: main.o puertos.o redes.o
	@gcc main.o puertos.o redes.o -o portscan

main.o: main.c ../../include/puertos.h ../../include/redes.h
	@gcc -c main.c -o main.o

puertos.o: ../../src/puertos.c ../../include/puertos.h ../../include/redes.h
	@gcc -c ../../src/puertos.c -o puertos.o

redes.o: ../../src/redes.c ../../include/redes.h
	@gcc -c ../../src/redes.c -o redes.o

.PHONY: clean

clean:
	@rm -f portscan *.o
	@echo "-*Limpieza completada*-"
