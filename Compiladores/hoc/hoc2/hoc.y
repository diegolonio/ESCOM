%{

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

int yylex();
void yyerror(char *s);
void warning(char *s, char *t);
void execerror(char *s, char *t);
void fpecatch();

double mem[26];

%}

%union { /* Tipo de datos de la pila */
    double val; /* Valor actual */
    int index; /* Índice en mem[] */
}

%token <val> NUMBER
%token <index> VAR
%type <val> expr

%right '='
%left '+' '-' /* Asociatividad por la izquierda, misma precedencia */
%left '*' '/' /* Asociatividad por la izquierda, mayor precedencia */
%left UNARYMINUS /* Nuevo */

%%

list: /* Nada */
    | list '\n'
    | list expr '\n' { printf("\t%.8g\n", $2); }
    | list error '\n' { yyerrok; }
    ;

expr: NUMBER { $$ = $1; }
    | VAR { $$ = mem[$1]; }
    | VAR '=' expr { $$ = mem[$1] = $3; }
    | expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { 
        if ($3 == 0.0)
            execerror("division by zero", "");
        $$ = $1 / $3;
     }
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

    if (islower(c)) {
        yylval.index = c - 'a'; /* solo ASCII */
        return VAR;
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
