%{

#include <stdio.h>
#include <ctype.h>

#define YYSTYPE double /* Tipo de dato de la pila de yacc */

int yylex(void);
void yyerror(char *s);
void warning(char *s, char *t);

%}

%token NUMBER

%left '+' '-' /* Asociatividad por la izquierda, misma precedencia */
%left '*' '/' /* Asociatividad por la izquierda, mayor precedencia */
%left UNARYMINUS /* Nuevo */

%%

list: /* Nada */
    | list '\n'
    | list expr '\n' { printf("\t%.8g\n", $2); }
    ;

expr: NUMBER { $$ = $1; }
    | expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { $$ = $1 / $3; }
    | '(' expr ')' { $$ = $2; }
    | '-' expr %prec UNARYMINUS { $$ = -$2; } /* Nuevo */
    ;

%%

char *progname; /* Para mensajes de error */
int lineno = 1;

int main(int argc, char *argv[])
{
    progname = argv[0];
    yyparse();

    return 0;
}

int yylex(void)
{
    int c;

    while ((c = getchar()) == ' ' || c == '\t')
        ;

    if (c == EOF)
        return 0;

    if (c == '.' || isdigit(c)) { /* Número */
        ungetc(c, stdin);
        scanf("%lf", &yylval);
        return NUMBER;
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
