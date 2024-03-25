all: udp
	@echo "-*Compilación completada*-"

udp: main.o udp.o redes.o
	@gcc main.o udp.o redes.o -o udp

main.o: main.c ../../include/udp.h ../../include/redes.h
	@gcc -c main.c -o main.o

udp.o: ../../src/udp.c ../../include/udp.h ../../include/redes.h
	@gcc -c ../../src/udp.c -o udp.o

redes.o: ../../src/redes.c ../../include/redes.h
	@gcc -c ../../src/redes.c -o redes.o

.PHONY: clean

clean:
	@rm -f udp *.o
	@echo "-*Limpieza completada*-"
