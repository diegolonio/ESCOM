CFLAGS = -I $(INCDIR) -lpthread
INCDIR = ../include

servidor : servidor.o
	@gcc servidor.o -o servidor $(CFLAGS)
	@rm -f *.o *.d

cliente : cliente.o
	@gcc cliente.o -o cliente $(CFLAGS)
	@rm -f *.o *.d

%.o : %.c
	@gcc -c -MD $< -o $@ $(CFLAGS)

-include *.d

.PHONY : all clean

all : servidor cliente
	@echo "-*Compilación completa*-"

clean :
	@ipcrm -a
	@rm -f sem_* shm_*
	@rm -f servidor cliente *.o *.d
