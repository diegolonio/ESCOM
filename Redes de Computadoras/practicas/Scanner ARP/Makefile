all: scanner
	@echo "-*Compilación completada*-"

scanner: main.o arp.o redes.o
	@gcc main.o arp.o redes.o -o scanner -l sqlite3

main.o: main.c ../../include/scanner_arp.h ../../include/redes.h
	@gcc -c main.c -o main.o -l sqlite3

arp.o: ../../src/scanner_arp.c ../../include/scanner_arp.h
	@gcc -c ../../src/scanner_arp.c -o arp.o -l sqlite3

redes.o: ../../src/redes.c ../../include/redes.h
	@gcc -c ../../src/redes.c -o redes.o

.PHONY: clean

clean:
	@rm -f scanner direcciones.db *.o
	@echo "-*Limpieza completada*-"
