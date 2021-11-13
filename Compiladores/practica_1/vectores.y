%{

#include <stdio.h>
#include <ctype.h>
#include "vectores.h"

/* Funciones del parser */
void yyerror(char *s);
int yylex();
void warning(char *s, char *t);

%}

%union {
	int escalar;
	Vector *vector;
	Componente *componente;
}

%token <escalar> NUMERO
%type <vector> exp vector
%type <componente> componente
%type <escalar> escalar

%left '+' '-'
%left 'x' '*'

%%

list:  
    | list '\n'
    | list exp '\n' { mostrar_vector($2); liberar_vector($2); }
    | list escalar '\n' { printf("%d\n", $2); }
    ;

exp: vector { $$ = $1; }
    | exp '+' exp { $$ = suma($1, $3); liberar_vector($1); liberar_vector($3); }
    | exp '-' exp { $$ = resta($1, $3); liberar_vector($1); liberar_vector($3); }
    | exp 'x' exp { $$ = cruz($1, $3); liberar_vector($1); liberar_vector($3); }
    | escalar '*' exp { $$ = ppescalar($1, $3); liberar_vector($3); }
    | exp '*' escalar { $$ = ppescalar($3, $1); liberar_vector($1); }
    | '(' exp ')' { $$ = $2; }
    ;

escalar:  exp '*' exp { $$ = punto($1, $3); liberar_vector($1); liberar_vector($3); }
	| '|' exp '|' { $$ = norma($2); liberar_vector($2); }
	| NUMERO { $$ = $1; }
	;

vector: '[' componente ']' { $$ = crear_vector($2); }
	;

componente:  NUMERO ',' componente { $$ = crear_componente($1, $3); }
	   | NUMERO { $$ = crear_componente($1, NULL); }
	   ;

%%

char *progname;
int lineno = 1;

int main(int argc, char *argv[])
{
	progname = argv[0];

	return yyparse();
}

/* Funciones del parser */

int yylex()
{
	int c;

	while ((c = getchar()) == ' ' || c == '\t') // ignora blancos
		; // enunciado nulo

	if (c == EOF)
		return 0;

	if (c == '.' || isdigit(c)) { // num
		ungetc(c, stdin);
		scanf("%d", &yylval.escalar); //lexema
		return NUMERO; // tipo del token
	}

	if (c == '\n')
		lineno++;

	return c; // +, -, *, /, (, )
}

void yyerror(char *s)
{
	warning(s, (char *)0);
}

void warning(char *s, char *t)
{
	fprintf(stderr, "%s: %s", progname, s);
	
	if (t)
		fprintf (stderr, " %s", t);

	fprintf(stderr, " cerca de la linea %d\n", lineno);
}
