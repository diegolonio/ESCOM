all: ping
	@echo "-*Compilación completada*-"

ping: main.o ping.o redes.o
	@gcc main.o ping.o redes.o -o ping

main.o: main.c ../../include/ping.h ../../include/redes.h
	@gcc -c main.c -o main.o

ping.o: ../../src/ping.c ../../include/ping.h ../../include/redes.h
	@gcc -c ../../src/ping.c -o ping.o

redes.o: ../../src/redes.c ../../include/redes.h
	@gcc -c ../../src/redes.c -o redes.o

.PHONY: clean

clean:
	@rm -f ping *.o
	@echo "-*Limpieza completada*-"
