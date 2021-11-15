OBJS = vectores.tab.o vectores.o simbolo.o

vectores : $(OBJS)
	@gcc $(OBJS) -o vectores -lm
	@echo "-*Compilación completada*-"

vectores.tab.o : vectores.tab.c vectores.h
	@gcc -c vectores.tab.c -o vectores.tab.o

vectores.tab.c : vectores.y
	@bison -Wno-conflicts-sr -Wno-conflicts-rr -d vectores.y -o vectores.tab.c

vectores.o : vectores.c vectores.h
	@gcc -c vectores.c -o vectores.o

vectores.c :
	@touch vectores.c

simbolo.o : simbolo.c vectores.h vectores.tab.h
	@gcc -c simbolo.c -o simbolo.o

.PHONY : all clean

all : vectores

clean :
	@rm -f $(OBJS) vectores.tab.[ch] vectores
	@echo "-*Limpieza completada*-"
