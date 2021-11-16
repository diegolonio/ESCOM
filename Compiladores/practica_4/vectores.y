%{

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <setjmp.h>
#include "vectores.h"

bool tamano_diferente = false;

%}

%union {
	// int escalar;
	// Vector *vector;
	// Componente *componente;
	Instruccion *instruccion;
	Simbolo *simbolo;
}

// %token <escalar> ESCALAR
%token <simbolo> ESCALAR VARIABLE INDEFINIDO FUNPREDEF
// %type <vector> asignacion expresion vector
// %type <componente> componente
// %type <escalar> escalar

%right '='
%left '+' '-'
%left 'x' '*'
%left MENOSUNARIO

%%

lista: /* Epsilon */ { printf(">>> "); }
    | lista '\n' { printf(">>> "); } 
	| lista asignacion '\n' { printf(">>> "); codigo(pop); codigo(STOP); return 1; }
    | lista expresion '\n' {
			// if (!tamano_diferente) {
			// 	printf("\t"); mostrar_vector($2);
			// } 
			// tamano_diferente = false;
			codigo(imprimir_vector); codigo(STOP); return 1;
			printf(">>> ");
		}
    | lista escalar '\n' {
			// if (!tamano_diferente)
			// 	printf("\t%d\n", $2);
			// tamano_diferente = false;
			codigo(imprimir_escalar); codigo(STOP); return 1;
			printf(">>> ");
		}
	| lista error '\n' { yyerrok; printf(">>> "); }
    ;

asignacion: VARIABLE '=' expresion { /*$$ = $1->u.vector = $3; $1->tipo = VARIABLE;*/
			codigo(insertar_variable); codigo($1); codigo(asignacion); 
		}
	;

expresion: vector { /*$$ = $1;*/ }
	| '-' VARIABLE %prec MENOSUNARIO { /*$$ = ppescalar(-1, $2->u.vector);*/ 
			codigo(insertar_variable); codigo($1); codigo(evaluar); codigo(vector_negativo);
		}
	| VARIABLE {
			// if ($1->tipo == INDEFINIDO)
			// 	ejecutar_error("variable indefinida", $1->nombre);
			// $$ = $1->u.vector;
			codigo(insertar_variable); codigo($1); codigo(evaluar);
		}
	| asignacion
    | expresion '+' expresion {
			// if (dimension($1) != dimension($3)) {
			// 	printf("Los vectores deben tener la misma dimensión\n");
			// 	tamano_diferente = true;
			// } else {
			// 	$$ = suma($1, $3);
			// }
			codigo(maquina_suma);
		}
    | expresion '-' expresion {
			// if (dimension($1) != dimension($3)) {
			// 	printf("Los vectores deben tener la misma dimensión\n");
			// 	tamano_diferente = true;
			// } else {
			// 	$$ = resta($1, $3);
			// }
			codigo(maquina_resta);
		}
    | expresion 'x' expresion {
			// if (dimension($1) != 3 || dimension($3) != 3) {
			// 	printf("El producto cruz solo puede realizarse con vectores de dimensión 3\n");
			// 	tamano_diferente = true;
			// } else {
			// 	$$ = cruz($1, $3);
			// }
			codigo(maquina_cruz);
		}
    | escalar '*' expresion { /*$$ = ppescalar($1, $3);*/ codigo(maquina_ppescalar); }
    | expresion '*' escalar { /*$$ = ppescalar($3, $1);*/ codigo(maquina_ppescalar); }
    | '(' expresion ')' { /*$$ = $2;*/ }
    ;

escalar:  expresion '*' expresion {
			// if (dimension($1) != dimension($3)) {
			// 	printf("Los vectores deben tener la misma dimensión\n");
			// 	tamano_diferente = true;
			// } else {
			// 	$$ = punto($1, $3);
			// }
			codigo(maquina_punto);
		}
	| '|' expresion '|' { /*$$ = norma($2);*/ codigo(maquina_norma); }
	| ESCALAR { /*$$ = $1;*/ codigo(insertar_escalar); codigo($1); }
	| '-' ESCALAR %prec MENOSUNARIO { /*$$ = -$2;*/
			codigo(insertar_escalar); codigo($1); codigo(escalar_negativo);
		}
	| '(' escalar ')' { /*$$ = $2;*/ }
	| FUNPREDEF '(' expresion ')' { /*$$ = (*($1->u.apuntador))($3);*/
			codigo(insertar_predefinida); codigo($1->u.apuntador);
		}
	;

vector: '[' componente ']' { /*$$ = crear_vector($2);*/
			codigo(maquina_crear_vector);
		}
	;

componente:  ESCALAR ',' componente { /*$$ = crear_componente($1, $3);*/
			codigo(insertar_escalar); codigo($1); codigo(maquina_crear_componente);
		}
	| ESCALAR { /*$$ = crear_componente($1, NULL);*/
			codigo(insertar_escalar); codigo($1); codigo(maquina_crear_primer_componente);
		}
	| '-' ESCALAR %prec MENOSUNARIO { /*$$ = crear_componente(-$2, NULL);*/
			codigo(insertar_escalar); codigo($2); codigo(escalar_negativo);
		}
	;

%%

char *nombre_programa;
int numero_linea = 1;
jmp_buf inicio;

int main(int argc, char *argv[])
{
	nombre_programa = argv[0];
	inicializar();
	setjmp(inicio);
	
	for (iniciar_codigo(); yyparse(); iniciar_codigo())
		ejecutar(prog);

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
		int d;
		ungetc(c, stdin);
		scanf("%d", &d);
		yylval.simbolo = instalar("", ESCALAR, NULL, d);
		return ESCALAR;
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
