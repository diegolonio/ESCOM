hoc : hoc.o
	@gcc hoc.o -o hoc
	@echo "-*Compilación completada*-"

hoc.o : hoc.tab.c
	@gcc -c hoc.tab.c -o hoc.o

hoc.tab.c : hoc.y
	@bison hoc.y

.PHONY : all clean

all : hoc

clean :
	@rm -f *.o *.c hoc
	@echo "-*Limpieza completada*-"
	