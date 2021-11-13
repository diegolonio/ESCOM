#include "hoc.h"
#include "hoc.tab.h"
#include <math.h>

extern double Log(), Log10(), Exp(), Sqrt(), integer();

static struct { /* Constantes */
    char *name;
    double cval;
} consts[] = {
    "PI",     3.14159265358979323846,
    "E",      2.71828182845904523536, /* Euler */
    "GAMMA",  0.57721566490153286060,
    "DEG",   57.29577951308232087680, /* deg/radian */
    "PHI",    1.61803398874989484820, /* proporción aurea */
    0, 0
};

static struct { /* Built-ins */
    char *name;
    double (*func)(double);
} builtins[] = {
    "sin", sin,
    "cos", cos,
    "atan", atan,
    "log", Log,
    "log10", Log10,
    "exp", Exp,
    "sqrt", Sqrt,
    "int", integer,
    "abs", fabs,
    0, 0
};

static struct { /* Palabras reservadas */
    char *name;
    int kval;
} keywords[] = {
    "if", IF,
    "else", ELSE,
    "while", WHILE,
    "print", PRINT,
    0, 0
};

void init() /* Instalar las constantes y las built-ins en la tabla de símbolos */
{
    int i;
    Symbol *s;

    for (i = 0; consts[i].name; i++)
        install(consts[i].name, VAR, consts[i].cval);

    for (i = 0; builtins[i].name; i++) {
        s = install(builtins[i].name, BLTIN, 0.0);
        s->u.ptr = builtins[i].func;
    }

    for (i = 0; keywords[i].name; i++)
        install(keywords[i].name, keywords[i].kval, 0.0);
}

