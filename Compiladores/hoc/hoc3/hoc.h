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
