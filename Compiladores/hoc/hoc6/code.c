#include <stdio.h>
#include "hoc.h"
#include "hoc.tab.h"

extern double Pow(double x, double y);
void execerror(char *s, char *t);

#define NSTACK 256
static Datum stack[NSTACK]; /* La pila */
static Datum *stackp; /* El siguiente espacio disponible en la pila */

#define NPROG 2000
Inst prog[NPROG]; /* La máquina */
Inst *progp; /* El siguiente espacio disponible para la generación de código */
Inst *pc; /* Contador de programa durante la ejecución */
Inst *progbase = prog; /* Inicio del subprograma actual */
int returning; /* 1 si se encuentra 'return stmt' */

typedef struct Frame { /* marco de pila de llamada a proc/func */
    Symbol *sp; /* Entrada de la tabla de símbolos */
    Inst *retpc; /* A dónde regresar después del return */
    Datum *argn; /* n-ésimo argumento en la pila */
    int nargs; /* Número de argumentos */
} Frame;

#define NFRAME 100
Frame frame[NFRAME];
Frame *fp; /* Apuntador al marco de pila */

// extern FILE *fin;


void initcode() /* Inicialización para la generación de código */
{
    progp = progbase;
    stackp = stack;
    // progp = prog;
    fp = frame;
    returning = 0;
}

void push(Datum d)
{
    if (stackp >= &stack[NSTACK])
        execerror("stack too deep", (char *)0);

    *stackp++ = d;
}

Datum pop() /* Saca y retorna el elemento en la cima de la pila */
{
    if (stackp == stack)
        execerror("stack underflow", (char *)0);

    return *--stackp;
}

Inst *code(Inst f) /* Instalar una instrucción o un operando */
{
    Inst *oprogp = progp;

    if (progp >= &prog[NPROG])
        execerror("program to big", (char *)0);

    *progp++ = f;

    return oprogp;
}

void execute(Inst *p) /* Ejecutar la máquina */
{
    for (pc = p; *pc != STOP && !returning; )
        (*(*pc++))();
}

void constpush() /* Insertar una constante en la pila */
{
    Datum d;

    d.val = ((Symbol *)*pc++)->u.val;
    push(d);
}

void varpush()
{
    Datum d;

    d.sym = (Symbol *)(*pc++);
    push(d);
}

void add() /* Sumar los dos elementos en la cima de la pila */
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val += d2.val;
    push(d1);
}

void sub() /* Restar los dos elementos en la cima de la pila */
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val -= d2.val;
    push(d1);
}

void mul() /* Multiplicar los dos elementos en la cima de la pila */
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val *= d2.val;
    push(d1);
}

void division() /* Dividir los dos elementos en la cima de la pila */
{
    Datum d1, d2;

    d2 = pop();

    if (d2.val == 0.0)
        execerror("division by zero", (char *)0);

    d1 = pop();
    d1.val /= d2.val;
    push(d1);
}

void power() /* Elevar el penúltimo elemento de la cima de la pila a la potencia del elemento en la cima de la pila */
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = Pow(d1.val, d2.val);
    push(d1);
}

void negate() /* Negar el elemento en la cima de la pila */
{
    Datum d;

    d = pop();
    d.val = -d.val;
    push(d);
}

void eval()
{
    Datum d;

    d = pop();

    if (d.sym->type != VAR && d.sym->type != UNDEF)
        execerror("attempt to evaluate non-variable", d.sym->name);

    if (d.sym->type == UNDEF)
        execerror("undefined variable", d.sym->name);

    d.val = d.sym->u.val;
    push(d);
}

void assign() /* Asignar el siguiente valor al valor de la cima */
{
    Datum d1, d2;

    d1 = pop();
    d2 = pop();

    if (d1.sym->type != VAR && d1.sym->type != UNDEF)
        execerror("assignment to non-variable", d1.sym->name);

    d1.sym->u.val = d2.val;
    d1.sym->type = VAR;
    push(d2);
}

void print() /* Sacar el elemento de la cima de la pila e imprimirlo */
{
    Datum d;

    d = pop();
    printf("\t%.8g\n", d.val);
}

void bltin() /* Evaluar la built-in en la cima de la pila */
{
    Datum d;

    d = pop();
    d.val = (*(double (*)())(*pc++))(d.val);
    push(d);
}

void gt()
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = (double)(d1.val > d2.val);
    push(d1);
}

void lt()
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = (double)(d1.val < d2.val);
    push(d1);
}

void eq()
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = (double)(d1.val == d2.val);
    push(d1);
}

void ge()
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = (double)(d1.val >= d2.val);
    push(d1);
}

void le()
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = (double)(d1.val <= d2.val);
    push(d1);
}

void ne()
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = (double)(d1.val != d2.val);
    push(d1);
}

void and()
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = (double)(d1.val != 0.0 && d2.val != 0.0);
    push(d1);
}

void or()
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = (double)(d1.val != 0.0 || d2.val != 0.0);
    push(d1);
}

void not()
{
    Datum d;

    d = pop();
    d.val = (double)(d.val == 0.0);
    push(d);
}

void whilecode()
{
    Datum d;
    Inst *savepc = pc; /* Cuerpo del ciclo */

    execute(savepc+2); /* Condición */
    d = pop();

    while (d.val) {
        execute(*((Inst **)(savepc)));

        if (returning)
            break;
        
        execute(savepc+2);
        d = pop();
    }

    if (!returning)
        pc = *((Inst **)(savepc+1)); /* Siguiente sentencia */
}

void ifcode()
{
    Datum d;
    Inst *savepc = pc; /* Bloque de código a ejecutar si la condición se cumple */

    execute(savepc+3); /* Condición */
    d = pop();

    if (d.val)
        execute(*((Inst **)(savepc)));
    else if (*((Inst **)(savepc+1))) /* condición del else */
        execute(*((Inst **)(savepc+1)));

    if (!returning)
        pc = *((Inst **)(savepc+2)); /* Siguiente sentencia */
}

void define(Symbol *sp) /* Pon func/proc en la tabla de símbolos */
{
    sp->u.defn = (int)progbase; /* Iniciar código */
    progbase = progp; /* El siguiente código comienza aquí */
}

void call() /* Llamar a una función */
{
    Symbol *sp = (Symbol *)pc[0]; /* Entrada de la tabla de símbolos */

    if (fp++ >= &frame[NFRAME-1])
        execerror(sp->name, "call nested too deeply");

    fp->sp = sp;
    fp->nargs = (int)pc[1];
    fp->retpc = pc + 2;
    fp->argn = stackp - 1; /* El último argumento */
    execute((Inst *)(sp->u.defn));
    returning = 0;
}

void funcret() /* Retornar de una función */
{
    Datum d;

    if (fp->sp->type == PROCEDURE)
        execerror(fp->sp->name, "(proc) returns value");

    d = pop();
    ret();
    push(d);
}

void procret() /* Retornar de un procedimiento */
{
    if (fp->sp->type == FUNCTION)
        execerror(fp->sp->name, "(func) returns no value");

    ret();
}

void ret() /* Retorno común de func o proc */
{
    int i;

    for (i = 0; i < fp->nargs; i++)
        pop(); /* Sacar los argumentos de la pila */

    pc = (Inst *)fp->retpc;
    --fp;
    returning = 1;
}

double *getarg() /* Retornar el puntero al argumento */
{
    int nargs = (int)*pc++;

    if (nargs > fp->nargs)
        execerror(fp->sp->name, "not enough arguments");

    return &fp->argn[nargs-fp->nargs].val;
}

void arg() /* Insertar un argumento en la pila */
{
    Datum d;

    d.val = *getarg();
    push(d);
}

void argassign() /* Guardar el elemento en la cima de la pila en el argumento */
{
    Datum d;

    d = pop();
    push(d); /* Dejar el valor en la pila */
    *getarg() = d.val;
}

void prstr() /* Imprimir valor de cadena */
{
    printf("%s", (char *)*pc++);
}

void prexpr() /* Imprimir valor numérico */
{
    Datum d;

    d = pop();
    printf("%.8g ", d.val);
}

void varread() /* Leer un valor e ingresarlo en una variable */
{
    Datum d;

    Symbol *var = (Symbol *)*pc++;
Again:
    switch (fscanf(fin, "%lf", &var->u.val)) {
        case EOF:
            if (moreinput())
                goto Again;
            
            d.val = var->u.val = 0.0;
            break;
        case 0:
            execerror("non-number read into", var->name);
            break;
        default:
            d.val = 1.0;
            break;
    }

    var->type = VAR;
    push(d);
}
