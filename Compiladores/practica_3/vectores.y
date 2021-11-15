%{

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <setjmp.h>
#include "vectores.h"

bool tamano_diferente = false;

%}

%union {
	int escalar;
	Vector *vector;
	Componente *componente;
	Simbolo *simbolo;
}

%token <escalar> NUMERO
%token <simbolo> VARIABLE INDEFINIDO
%type <vector> asignacion expresion vector
%type <componente> componente
%type <escalar> escalar

%right '='
%left '+' '-'
%left 'x' '*'
%left MENOSUNARIO

%%

lista:  
    | lista '\n'
	| lista asignacion '\n'
    | lista expresion '\n' {
			if (!tamano_diferente) {
				printf("\t"); mostrar_vector($2);
			} 
			tamano_diferente = false;
		}
    | lista escalar '\n' {
			if (!tamano_diferente)
				printf("\t%d\n", $2);
			tamano_diferente = false;
		}
	| lista error '\n' { yyerrok; }
    ;

asignacion: VARIABLE '=' expresion { $$ = $1->u.vector = $3; $1->tipo = VARIABLE; }
	;

expresion: vector { $$ = $1; }
	| '-' VARIABLE %prec MENOSUNARIO { $$ = ppescalar(-1, $2->u.vector); }
	| VARIABLE {
			if ($1->tipo == INDEFINIDO)
				ejecutar_error("variable indefinida", $1->nombre);
			$$ = $1->u.vector;
		}
	| asignacion
    | expresion '+' expresion {
			if (dimension($1) != dimension($3)) {
				printf("Los vectores deben tener la misma dimensión\n");
				tamano_diferente = true;
			} else {
				$$ = suma($1, $3);
			}
		}
    | expresion '-' expresion {
			if (dimension($1) != dimension($3)) {
				printf("Los vectores deben tener la misma dimensión\n");
				tamano_diferente = true;
			} else {
				$$ = resta($1, $3);
			}
		}
    | expresion 'x' expresion {
			if (dimension($1) != 3 || dimension($3) != 3) {
				printf("El producto cruz solo puede realizarse con vectores de dimensión 3\n");
				tamano_diferente = true;
			} else {
				$$ = cruz($1, $3);
			}
		}
    | escalar '*' expresion { $$ = ppescalar($1, $3); }
    | expresion '*' escalar { $$ = ppescalar($3, $1); }
    | '(' expresion ')' { $$ = $2; }
    ;

escalar:  expresion '*' expresion {
			if (dimension($1) != dimension($3)) {
				printf("Los vectores deben tener la misma dimensión\n");
				tamano_diferente = true;
			} else {
				$$ = punto($1, $3);
			}
		}
	| '|' expresion '|' { $$ = norma($2); }
	| NUMERO { $$ = $1; }
	| '-' NUMERO %prec MENOSUNARIO { $$ = -$2; }
	| '(' escalar ')' { $$ = $2; }
	;

vector: '[' componente ']' { $$ = crear_vector($2); }
	;

componente:  NUMERO ',' componente { $$ = crear_componente($1, $3); }
	| NUMERO { $$ = crear_componente($1, NULL); }
	| '-' NUMERO %prec MENOSUNARIO { $$ = crear_componente(-$2, NULL); }
	;

%%

char *nombre_programa;
int numero_linea = 1;
jmp_buf inicio;

int main(int argc, char *argv[])
{
	nombre_programa = argv[0];
	setjmp(inicio);
	yyparse();

	return 0;
}

/* Funciones del parser */

int yylex()
{
	int c;

	while ((c = getchar()) == ' ' || c == '\t')
		;

	if (c == EOF)
		return 0;

	if (c == '.' || isdigit(c)) {
		ungetc(c, stdin);
		scanf("%d", &yylval.escalar);
		return NUMERO;
	}

	if (isalpha(c) && c != 'x') {
		Simbolo *nuevo_simbolo;
		char sbuf[100], *nombre = sbuf;

		do {
			*nombre++ = c;
		} while ((c = getchar()) != EOF && isalnum(c));

		ungetc(c, stdin);
		*nombre = '\0';

		if ((nuevo_simbolo = buscar(sbuf)) == 0)
			nuevo_simbolo = instalar(sbuf, INDEFINIDO, NULL);

		yylval.simbolo = nuevo_simbolo;

		return nuevo_simbolo->tipo == INDEFINIDO ? VARIABLE : nuevo_simbolo->tipo;
	}

	if (c == '\n')
		numero_linea++;

	return c;
}

void yyerror(char *s)
{
	advertencia(s, (char *)0);
}

void advertencia(char *s, char *t)
{
	fprintf(stderr, "%s: %s", nombre_programa, s);
	
	if (t)
		fprintf (stderr, " %s", t);

	fprintf(stderr, " cerca de la linea %d\n", numero_linea);
}

void ejecutar_error(char *s, char *t)
{
	advertencia(s, t);
	longjmp(inicio, 0);
}
