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

void initcode() /* Inicialización para la generación de código */
{
    stackp = stack;
    progp = prog;
}

void push(Datum d)
{
    if (stackp >= &stack[NSTACK])
        execerror("stack overflow", (char *)0);

    *stackp++ = d;
}

Datum pop() /* Saca y retorna el elemento en la cima de la pila */
{
    if (stackp <= stack)
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
    for (pc = p; *pc != STOP; )
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
    d1.val = (double)(d1.val && d2.val);
    push(d1);
}

void or()
{
    Datum d1, d2;

    d2 = pop();
    d1 = pop();
    d1.val = (double)(d1.val || d2.val);
    push(d1);
}

void not()
{
    Datum d;

    d = pop();
    d.val = (double)(!d.val);
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
        execute(savepc+2);
        d = pop();
    }

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

    pc = *((Inst **)(savepc+2)); /* Siguiente sentencia */
}

void prexpr() /* Imprimir el valor numérico */
{
    Datum d;

    d = pop();
    printf("\t%.8g\n", d.val);
}
