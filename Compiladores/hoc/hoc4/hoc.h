typedef struct Symbol { /* Entrada de la tabla de símbolos */
    char *name;
    short type; /* VAR, BLTIN, UNDEF */
    union {
        double val; /* Si es VAR */
        double (*ptr)(double); /* Si es BLTIN */
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
