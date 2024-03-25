all: arp
	@echo "-*Compilación completada*-"

arp: arp.o redes.o
	@gcc arp.o redes.o -o arp

arp.o: main.c ../../include/redes.h 
	@gcc -c main.c -o arp.o

redes.o: ../../src/redes.c ../../include/redes.h
	@gcc -c ../../src/redes.c -o redes.o

.PHONY: clean

clean:
	@rm -f arp *.o
	@echo "-*Limpieza completada*-"
