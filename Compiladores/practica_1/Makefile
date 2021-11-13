OBJS = vectores.tab.o vectores.o

vectores : $(OBJS) vectores.h
	@gcc $(OBJS) -o vectores -lm
	@echo "-*Compilación completada*-"

vectores.o : vectores.c vectores.h
	@gcc -c vectores.c -o vectores.o

vectores.tab.o : vectores.tab.c vectores.h
	@gcc -c vectores.tab.c -o vectores.tab.o

vectores.tab.c : vectores.y
	@bison vectores.y

.PHONY : all clean

all : vectores

clean :
	@rm -f $(OBJS) vectores.tab.c vectores
	@echo "-*Limpieza completada*-"
