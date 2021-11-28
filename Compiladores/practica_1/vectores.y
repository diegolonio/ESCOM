/*
 * Nombre: Apolonio Villegas Diego Armando 
 * Práctica: 1
 * Tema: yacc básico
 * Opción: Calculadora de vectores
 * Fecha 06/10/2021
 * Grupo: 3CM17
 * Materia: Compiladores
 */

%{

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "vectores.h"

/* Funciones del parser */
void yyerror(char *s);
int yylex();
void warning(char *s, char *t);

bool tamano_diferente = false;

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
%left UNARYMINUS

%%

list: /* Epsilon */ {
			printf(">>> ");
		}
    | list '\n' {
			printf(">>> ");
		}
    | list exp '\n' {
			if (!tamano_diferente) {
				mostrar_vector($2);
				liberar_vector($2);
			}
			
			printf(">>> ");
			tamano_diferente = false;
		}
    | list escalar '\n' {
			if (!tamano_diferente)
				printf("%d\n", $2);
				
			printf(">>> ");
			tamano_diferente = false;
		}
    ;

exp: vector { $$ = $1; }
    | exp '+' exp {
			if (dimension($1) != dimension($3)) {
				printf("Los vectores deben tener la misma dimensión\n");
				tamano_diferente = true;
			} else {
				$$ = suma($1, $3);
				tamano_diferente = false;
			}

			liberar_vector($1); liberar_vector($3);
		}
    | exp '-' exp {
			if (dimension($1) != dimension($3)) {
				printf("Los vectores deben tener la misma dimensión\n");
				tamano_diferente = true;
			} else {
				$$ = resta($1, $3);
				tamano_diferente = false;
			}

			liberar_vector($1); liberar_vector($3);
		}
    | exp 'x' exp {
			if (dimension($1) != 3 || dimension($3) != 3) {
				printf("El producto cruz solo puede realizarse con vectores de dimensión 3\n");
				tamano_diferente = true;
			} else {
				$$ = cruz($1, $3);
				tamano_diferente = false;
			}

			liberar_vector($1); liberar_vector($3);
		}
    | escalar '*' exp {
			$$ = ppescalar($1, $3);
			liberar_vector($3);
		}
    | exp '*' escalar {
			$$ = ppescalar($3, $1);
			liberar_vector($1);
		}
    | '(' exp ')' {
			$$ = $2;
		}
    ;

escalar:  exp '*' exp {
			if (dimension($1) != dimension($3)) {
				printf("Los vectores deben tener la misma dimensión\n");
				tamano_diferente = true;
			} else {
				$$ = punto($1, $3);
				tamano_diferente = false;
			}
			
			liberar_vector($1); liberar_vector($3);
		}
	| '|' exp '|' {
			$$ = norma($2);
			liberar_vector($2);
		}
	| NUMERO {
			$$ = $1;
		}
	| '-' NUMERO %prec UNARYMINUS {
			$$ = -$2;
		}
	| '(' escalar ')' {
			$$ = $2;
		}
	;

vector: '[' componente ']' {
			$$ = crear_vector($2);
		}
	;

componente:  NUMERO ',' componente {
			$$ = crear_componente($1, $3);
		}
	| NUMERO {
			$$ = crear_componente($1, NULL);
		}
	| '-' NUMERO %prec UNARYMINUS {
			$$ = crear_componente(-$2, NULL);
		}
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
