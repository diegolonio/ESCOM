#include <stdio.h>

typedef struct Symbol { /* Entrada de la tabla de símbolos */
    char *name;
    short type; /* VAR, BLTIN, UNDEF */
    union {
        double val; /* Si es VAR */
        double (*ptr)(double); /* Si es BLTIN */
        int (*defn)(); /* Si es FUNCTION o PROCEDURE */
        char *str; /* Si es STRING */
    } u;
    struct Symbol *next; /* Para enlazar con el siguiente símbolo */
} Symbol;

Symbol *install(char *s, int t, double d);
Symbol *lookup(char *s);

typedef union Datum { /* Tipo de dato de la pila del interprete */
    double val;
    Symbol *sym;
} Datum;
extern Datum pop();

typedef void (*Inst)(); /* Instrucción de máquina */
#define STOP (Inst)0

extern Inst prog[];
extern Inst *progp;
extern Inst *progbase;
extern Inst *code(Inst f);
extern void eval();
extern void add();
extern void sub();
extern void mul();
extern void division();
extern void negate();
extern void power();
extern void assign();
extern void bltin();
extern void varpush();
extern void constpush();
extern void print();
extern void prexpr();
extern void gt();
extern void lt();
extern void eq();
extern void ge();
extern void le();
extern void ne();
extern void and();
extern void or();
extern void not();
extern void ifcode();
extern void whilecode();
extern void argassign();
extern void procret();
extern void funcret();
extern void call();
extern void varread();
extern void prstr();
extern int backslash();
extern void ret();
extern int moreinput();
extern void run();
extern void arg();
extern void define();

FILE *fin;
