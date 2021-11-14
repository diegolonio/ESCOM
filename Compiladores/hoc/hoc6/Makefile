OBJS = hoc.o init.o math.o symbol.o code.o
DISWARNS = -Wno-pointer-to-int-cast -Wno-int-conversion -Wno-int-to-pointer-cast -Wno-incompatible-pointer-types

hoc : $(OBJS) hoc.h
	@gcc $(OBJS) -o hoc -lm
	@echo "-*Compilación completada*-"

hoc.o : hoc.tab.c hoc.h
	@gcc $(DISWARNS) -c hoc.tab.c -o hoc.o

hoc.tab.c : hoc.y
	@bison -Wno-conflicts-sr -d hoc.y

init.o : init.c hoc.h hoc.tab.h
	@gcc -c init.c -o init.o

math.o : math.c
	@gcc -c math.c -o math.o

symbol.o : symbol.c hoc.h hoc.tab.h
	@gcc -c symbol.c -o symbol.o

code.o : code.c hoc.h hoc.tab.h
	@gcc $(DISWARNS) -c code.c -o code.o

.PHONY : all clean

all : hoc

clean:
	@rm -f hoc.tab.[ch] $(OBJS) hoc
	@echo "-*Limpieza completada*-"