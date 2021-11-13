#include <stdlib.h>
#include <string.h>
#include "hoc.h"
#include "hoc.tab.h"

void execerror(char *s, char *t);
static Symbol *symlist = 0; /* Tabla de símbolos: lista simplemente enlazada */

Symbol *lookup(char *s)
{
    Symbol *sp;

    for (sp = symlist; sp != (Symbol *)0; sp = sp->next)
        if (strcmp(sp->name, s) == 0)
            return sp;

    return 0; /* Símbolo no encontrado */
}

Symbol *install(char *s, int t, double d)
{
    Symbol *sp;
    char *emalloc();

    sp = (Symbol *)emalloc(sizeof(Symbol));
    sp->name = emalloc(strlen(s)+1); /* +1 para agregar el '\0' */
    strcpy(sp->name, s);
    sp->type = t;
    sp->u.val = d;
    sp->next = symlist; /* Ponemos el símbolo al frente de la lista */
    symlist = sp;

    return sp; 
}

char *emalloc(unsigned n)
{
    char *p;

    p = malloc(n);
    
    if (p == 0)
        execerror("out of memory", (char *)0);

    return p; 
}
