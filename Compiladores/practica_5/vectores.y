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
%token <simbolo> IMPRIMIR MIENTRAS SI SINO
%type <instruccion> sentencia sentencias condicion mientras si fin
%type <instruccion> asignacion expresion escalar vector componente

%right '='
%left O
%left Y
%left MAYQUE MAYIGU MENQUE MENIGU IGUAL NOIGUAL
%left '+' '-'
%left 'x' '*'
%left MENOSUNARIO NO

%%

lista: /* Epsilon */ {
			printf(">>> ");
		}
    | lista '\n' {
			printf(">>> ");
		} 
	| lista asignacion '\n' {
			codigo((Instruccion)pop);
			codigo(PARO);
			return 1;
		}
	| lista sentencia '\n' {
			codigo((Instruccion)pop);
			codigo(PARO);
			return 1;
			printf(">>> ");
		}
	| lista error '\n' {
			yyerrok;
			printf(">>> ");
		}
    ;

asignacion: VARIABLE '=' expresion {
			codigo(insertar_variable);
			codigo((Instruccion)$1);
			codigo(asignar);
		}
	;

expresion: asignacion
	| vector
	| VARIABLE {
			codigo(insertar_variable);
			codigo((Instruccion)$1);
			codigo(evaluar);
		}
	| '-' VARIABLE %prec MENOSUNARIO {
			codigo(insertar_variable);
			codigo((Instruccion)$2);
			codigo(evaluar);
			codigo(vector_negativo);
		}
	| '(' expresion ')' {
			$$ = $2;
		}
    | expresion '+' expresion {
			codigo(maquina_suma);
		}
    | expresion '-' expresion {
			codigo(maquina_resta);
		}
    | expresion 'x' expresion {
			codigo(maquina_cruz);
		}
    | escalar '*' expresion {
			codigo(maquina_ppescalar_escexp);
		}
    | expresion '*' escalar {
			codigo(maquina_ppescalar_expesc);
		}
    ;

escalar:  expresion '*' expresion {
			codigo(maquina_punto);
		}
	| expresion IGUAL expresion {
			codigo(vectores_iguales);
		}
	| expresion NOIGUAL expresion {
			codigo(vectores_no_iguales);
		}
	| escalar MAYQUE escalar {
			codigo(mayor_que);
		}
	| escalar MAYIGU escalar {
			codigo(mayor_igual);
		}
	| escalar MENQUE escalar {
			codigo(menor_que);
		}
	| escalar MENIGU escalar {
			codigo(menor_igual);
		}
	| escalar O escalar {
			codigo(o_logico);
		}
	| escalar Y escalar {
			codigo(y_logico);
		}
	| escalar IGUAL escalar {
			codigo(escalares_iguales);
		}
	| escalar NOIGUAL escalar {
			codigo(escalares_no_iguales);
		}
	| NO escalar {
			codigo(negacion);
		}
	| ESCALAR {
			codigo(insertar_escalar);
			codigo((Instruccion)$1);
		}
	| '-' ESCALAR %prec MENOSUNARIO {
			codigo(insertar_escalar);
			codigo((Instruccion)$2);
			codigo(escalar_negativo);
		}
	| '(' escalar ')' {
			$$ = $2;
		}
	| FUNPREDEF '(' expresion ')' {
			codigo(ejecutar_predefinida);
			codigo((Instruccion)$1->u.apuntador);
		}
	;

vector: '[' componente ']' {
			codigo(maquina_crear_vector);
		}
	;

componente:  ESCALAR ',' componente {
			codigo(insertar_escalar);
			codigo((Instruccion)$1);
			codigo(maquina_crear_componente);
		}
	| ESCALAR {
			codigo(insertar_escalar);
			codigo((Instruccion)$1);
			codigo(maquina_crear_primer_componente);
		}
	| '-' ESCALAR %prec MENOSUNARIO {
			codigo(insertar_escalar);
			codigo((Instruccion)$2);
			codigo(escalar_negativo);
		}
	;

sentencia: expresion
	| escalar
	| IMPRIMIR expresion {
			codigo(imprimir_vector);
			codigo(PARO);
		}
	| IMPRIMIR escalar {
			codigo(imprimir_escalar);
			codigo(PARO);
		}
	| mientras condicion sentencia fin {
			printf("Mientras\n");
		}
	| si condicion sentencia fin {
			printf("Condicional sin else\n");
		}
	| si condicion sentencia fin SINO condicion fin {
			printf("Condicional con else\n");
		}
	| '{' sentencias '}' {
			$$ = $2;
			printf("Sentencias\n");
		}
	;

mientras: MIENTRAS {
			printf("Mientras\n");
		}
	;

si: SI {
			printf("Si\n");
		}
	;

condicion: '(' escalar ')' {
			printf("Condicion\n");
		}
	;

sentencias: /* Epsilon */ {
			printf("Epsilon\n");
		}
	| sentencias '\n'
	| sentencias sentencia
	;

fin: /* Epsilon */ {
			printf("Fin\n");
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
		ejecutar(programa);

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

	switch (c) {
		case '>':
			return siguiente('=', MAYIGU, MAYQUE);
			break;
		case '<':
			return siguiente('=', MENIGU, MENQUE);
			break;
		case '=':
			return siguiente('=', IGUAL, '=');
			break;
		case '!':
			return siguiente('=', NOIGUAL, NO);
			break;
		case '|':
			return siguiente('|', O, '|');
			break;
		case '&':
			return siguiente('&', Y, '&');
			break;
		case '\n':
			numero_linea++;
			return '\n';
		default:
			return c;
	}
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

int siguiente(int caracter_esperado, int si_es, int no_es)
{
	int c = getchar();

	if (c == caracter_esperado)
		return si_es;

	ungetc(c, stdin);
	
	return no_es;
}
