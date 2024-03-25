all: cliente servidor
	@echo "-*Compilación completada*-"

cliente: cliente.o
	@gcc cliente.o -o cliente

cliente.o: cliente.c
	@gcc -c cliente.c -o cliente.o

servidor: servidor.o
	@gcc servidor.o -o servidor

servidor.o: servidor.c
	@gcc -c servidor.c -o servidor.o

.PHONY: clean

clean:
	@rm -f cliente servidor *.o
	@echo "-*Limpieza completada*-"
