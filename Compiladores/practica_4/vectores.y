%{

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <setjmp.h>
#include "vectores.h"

%}

%union {
	Instruccion *instruccion;
	Simbolo *simbolo;
}

%token <simbolo> ESCALAR VARIABLE INDEFINIDO FUNPREDEF

%right '='
%left '+' '-'
%left 'x' '*'
%left MENOSUNARIO

%%

lista: /* Epsilon */ { printf(">>> "); }
    | lista '\n' { printf(">>> "); } 
	| lista asignacion '\n' { printf(">>> "); /*codigo((Instruccion)pop);*/ codigo(STOP); return 1; }
    | lista expresion '\n' { codigo(imprimir_vector); codigo(STOP); return 1; printf(">>> "); }
    | lista escalar '\n' { codigo(imprimir_escalar); codigo(STOP); return 1; printf(">>> "); }
	| lista error '\n' { yyerrok; printf(">>> "); }
    ;

asignacion: VARIABLE '=' expresion { codigo(insertar_variable); /*codigo((Instruccion)$1);*/ codigo(asignar); }
	;

expresion: vector { /*$$ = $1;*/ }
	| '-' VARIABLE %prec MENOSUNARIO { codigo(insertar_variable); /*codigo((Instruccion)$2);*/ codigo(evaluar); codigo(vector_negativo); }
	| VARIABLE { codigo(insertar_variable); /*codigo((Instruccion)$1);*/ codigo(evaluar); }
	| asignacion
    | expresion '+' expresion { codigo(maquina_suma); }
    | expresion '-' expresion { codigo(maquina_resta); }
    | expresion 'x' expresion { codigo(maquina_cruz); }
    | escalar '*' expresion { codigo(maquina_ppescalar); }
    | expresion '*' escalar { codigo(maquina_ppescalar); }
    | '(' expresion ')'
    ;

escalar:  expresion '*' expresion {codigo(maquina_punto); }
	| '|' expresion '|' { codigo(maquina_norma); }
	| ESCALAR { codigo(insertar_escalar); /*codigo((Instruccion)$1);*/ }
	| '-' ESCALAR %prec MENOSUNARIO { codigo(insertar_escalar); /*codigo((Instruccion)$2);*/ codigo(escalar_negativo); }
	| '(' escalar ')'
	| FUNPREDEF '(' expresion ')' { codigo(insertar_predefinida); codigo((Instruccion)$1->u.apuntador); }
	;

vector: '[' componente ']' { codigo(maquina_crear_vector); }
	;

componente:  ESCALAR ',' componente { codigo(insertar_escalar); /*codigo((Instruccion)$1);*/ codigo(maquina_crear_componente); }
	| ESCALAR { codigo(insertar_escalar); /*codigo((Instruccion)$1);*/ codigo(maquina_crear_primer_componente); }
	| '-' ESCALAR %prec MENOSUNARIO { codigo(insertar_escalar); /*codigo((Instruccion)$2);*/ codigo(escalar_negativo); }
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
		ejecutar_instruccion(programa);

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
			nuevo_simbolo = instalar(sbuf, INDEFINIDO, NULL, 0);

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
