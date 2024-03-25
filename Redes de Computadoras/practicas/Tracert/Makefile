all: tracert
	@echo "-*Compilación completada*-"

tracert: main.o tracert.o redes.o
	@gcc main.o tracert.o redes.o -o tracert

main.o: main.c ../../include/tracert.h ../../include/redes.h
	@gcc -c main.c -o main.o

tracert.o: ../../src/tracert.c ../../include/tracert.h ../../include/redes.h
	@gcc -c ../../src/tracert.c -o tracert.o

redes.o: ../../src/redes.c ../../include/redes.h
	@gcc -c ../../src/redes.c -o redes.o

.PHONY: clean

clean:
	@rm -f tracert *.o
	@echo "-*Limpieza completada*-"
