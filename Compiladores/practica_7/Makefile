OBJS = vectores.tab.o vectores.o simbolo.o inicializacion.o maquina.o

vectores : $(OBJS)
	@gcc $(OBJS) -o vectores -lm
	@echo "-*Compilación completada*-"

vectores.tab.o : vectores.tab.c vectores.h
	@gcc -Wno-incompatible-pointer-types -Wno-int-to-pointer-cast -c vectores.tab.c -o vectores.tab.o

vectores.tab.c : vectores.y
	@bison -Wno-conflicts-sr -Wno-conflicts-rr -d vectores.y -o vectores.tab.c

vectores.o : vectores.c vectores.h
	@gcc -c vectores.c -o vectores.o

vectores.c :
	@touch vectores.c

simbolo.o : simbolo.c vectores.h vectores.tab.h
	@gcc -c simbolo.c -o simbolo.o

inicializacion.o : inicializacion.c vectores.h vectores.tab.h
	@gcc -c inicializacion.c -o inicializacion.o

maquina.o : maquina.c vectores.h vectores.tab.h
	@gcc -Wno-pointer-to-int-cast -c maquina.c -o maquina.o

.PHONY : all clean

all : vectores

clean :
	@rm -f $(OBJS) vectores.tab.[ch] vectores
	@echo "-*Limpieza completada*-"
