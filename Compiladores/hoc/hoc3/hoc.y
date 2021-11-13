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

extern double Pow(double x, double y);

%}

%union { /* Tipo de datos de la pila */
    double val; /* Valor actual */
    Symbol *sym; /* apuntador al símbolo de la tabla */
}

%token <val> NUMBER
%token <sym> VAR BLTIN UNDEF
%type <val> expr asgn

%right '='
%left '+' '-' /* Asociatividad por la izquierda, misma precedencia */
%left '*' '/' /* Asociatividad por la izquierda, mayor precedencia */
%left UNARYMINUS /* Nuevo */
%right '^' /* Exponenciación */

%%

list: /* Nada */
    | list '\n'
    | list asgn '\n'
    | list expr '\n' { printf("\t%.8g\n", $2); }
    | list error '\n' { yyerrok; }
    ;

asgn: VAR '=' expr { $$ = $1->u.val = $3; $1->type = VAR; }
    ;

expr: NUMBER { $$ = $1; }
    | VAR {
        if ($1->type == UNDEF)
            execerror("undefined variable", $1->name);
        $$ = $1->u.val;
    }
    | asgn
    | BLTIN '(' expr ')' { $$ = (*($1->u.ptr))($3); }
    | expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { 
        if ($3 == 0.0)
            execerror("division by zero", "");
        $$ = $1 / $3;
     }
    | expr '^' expr { $$ = Pow($1, $3); }
    | '(' expr ')' { $$ = $2; }
    | '-' expr %prec UNARYMINUS { $$ = -$2; } /* Nuevo */
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

    yyparse();

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
        ungetc(c, stdin);
        scanf("%lf", &yylval.val);
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

    // if (islower(c)) {
    //     yylval.index = c - 'a'; /* solo ASCII */
    //     return VAR;
    // }

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
