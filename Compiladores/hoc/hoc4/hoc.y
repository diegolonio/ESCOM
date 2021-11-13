%{

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include "hoc.h"

int yylex();
void yyerror(char *s);
void warning(char *s, char *t);
void execerror(char *s, char *t);
void fpecatch();
void init();
void execute(Inst *p);
Inst *code(Inst f);
void initcode();

%}

%union { /* Tipo de datos de la pila */
    Inst *inst; /* Instrucción de máquina */
    Symbol *sym; /* apuntador al símbolo de la tabla */
}

%token <sym> NUMBER VAR BLTIN UNDEF

%right '='
%left '+' '-' /* Asociatividad por la izquierda, misma precedencia */
%left '*' '/' /* Asociatividad por la izquierda, mayor precedencia */
%left UNARYMINUS /* Nuevo */
%right '^' /* Exponenciación */

%%

list: /* Nada */
    | list '\n'
    | list asgn '\n' { code((Inst)pop); code(STOP); return 1; }
    | list expr '\n' { code(print); code(STOP); return 1; }
    | list error '\n' { yyerrok; }
    ;

asgn: VAR '=' expr { code(varpush); code((Inst)$1); code(assign); }
    ;

expr: NUMBER { code(constpush); code((Inst)$1); }
    | VAR { code(varpush); code((Inst)$1); code(eval); }
    | asgn
    | BLTIN '(' expr ')' { code(bltin); code((Inst)$1->u.ptr); }
    | expr '+' expr { code(add); }
    | expr '-' expr { code(sub); }
    | expr '*' expr { code(mul); }
    | expr '/' expr { code(division); }
    | expr '^' expr { code(power); }
    | '(' expr ')'
    | '-' expr %prec UNARYMINUS { code(negate); } /* Nuevo */
    ;

%%

char *progname; /* Para mensajes de error */
int lineno = 1;
jmp_buf begin;

int main(int argc, char *argv[])
{
    progname = argv[0];
    init();
    setjmp(begin);
    signal(SIGFPE, fpecatch);

    for (initcode(); yyparse(); initcode())
        execute(prog);

    return 0;
}

int yylex()
{
    int c;

    while ((c = getchar()) == ' ' || c == '\t')
        ;

    if (c == EOF)
        return 0;

    if (c == '.' || isdigit(c)) { /* Número */
        double d;
        ungetc(c, stdin);
        scanf("%lf", &d);
        yylval.sym = install("", NUMBER, d);

        return NUMBER;
    }

    if (isalpha(c)) {
        Symbol *s = NULL;
        char sbuf[100], *p = sbuf;

        do {
            *p++ = c;
        } while ((c = getchar()) != EOF && isalnum(c));

        ungetc(c, stdin);
        *p = '\0';

        if ((s = lookup(sbuf)) == 0)
            s = install(sbuf, UNDEF, 0.0);

        yylval.sym = s;

        return s->type == UNDEF ? VAR : s->type;
    }

    if (c == '\n')
        lineno++;

    return c;
}

void yyerror(char *s)
{
    warning(s, (char *)0);
}

void warning(char *s, char *t)
{
    fprintf(stderr, "%s: %s", progname, s);

    if (t)
        fprintf(stderr, " %s", t);

    fprintf(stderr, " near line %d\n", lineno);
}

void execerror(char *s, char *t) /* Recuperarse del error en tiempo de ejecución */
{
    warning(s, t);
    longjmp(begin, 0);
}

void fpecatch() /* Atrapar excepciones de punto flotante */
{
    execerror("floating point exception", (char *)0);
}
