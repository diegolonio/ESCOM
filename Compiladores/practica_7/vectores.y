%{

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <setjmp.h>
#include "vectores.h"

bool fuera_de_definicion;

%}

%union {
	Instruccion *instruccion;
	Simbolo *simbolo;
	int nargumentos;
}

%token <simbolo> ESCALAR VARIABLE INDEFINIDO FUNPREDEF CADENA
%token <simbolo> IMPRIMIR MIENTRAS SI SINO PARA
%token <simbolo> FUNCION PROCEDIMIENTO RETORNAR FUNC PROC
%token <nargumentos> ARGUMENTO
%type <instruccion> si mientras para condicion sentencia sentencias inicio fin
%type <instruccion> expresion expresiones asignacion escalar vector componente
%type <simbolo> nombreproc
%type <nargumentos> argumentos

%right '='
%left O
%left Y
%left MAYQUE MAYIGU MENQUE MENIGU IGUAL NOIGUAL
%left '+' '-'
%left '%'
%left 'x' '*'
%left MENOSUNARIO NO

%%

lista: /* Epsilon */ {
			printf(">>> ");
		}
    | lista '\n' {
			printf(">>> ");
		}
	| lista definicion '\n' {
			printf("\u2022\u2022\u2022 ");
		}
	| lista asignacion '\n' {
			codigo((Instruccion)pop);
			codigo(PARO);
			return 1;
		}
	| lista sentencia '\n' {
			codigo(PARO);
			return 1;
		}
	| lista error '\n' {
			yyerrok;
			printf(">>> ");
		}
    ;

asignacion: VARIABLE '=' expresion {
			$$ = $3;
			codigo(insertar_variable);
			codigo((Instruccion)$1);
			codigo(asignar);
		}
	| ARGUMENTO '=' expresion {
			$$ = $3;
			dentro_de_definicion("$");
			codigo(asignacion_argumento);
			codigo((Instruccion)$1);
		}
	;

expresion: asignacion
	| vector
	| VARIABLE {
			$$ = codigo(insertar_variable);
			codigo((Instruccion)$1);
			codigo(evaluar);
		}
	| '-' VARIABLE %prec MENOSUNARIO {
			$$ = codigo(insertar_variable);
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
	| ARGUMENTO {
			dentro_de_definicion("$");
			$$ = codigo(argumento);
			codigo((Instruccion)$1);
		}
	| FUNCION inicio '(' argumentos ')' {
			$$ = $2;
			codigo(llamada);
			codigo((Instruccion)$1);
			codigo((Instruccion)$4);
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
	| escalar '%' escalar {
			codigo(modulo);
		}
	| NO escalar {
			$$ = $2;
			codigo(negacion);
		}
	| ESCALAR {
			$$ = codigo(insertar_escalar);
			codigo((Instruccion)$1);
		}
	| '-' ESCALAR %prec MENOSUNARIO {
			$$ = codigo(insertar_escalar);
			codigo((Instruccion)$2);
			codigo(escalar_negativo);
		}
	| '(' escalar ')' {
			$$ = $2;
		}
	| FUNPREDEF '(' expresion ')' {
			$$ = $3;
			codigo(ejecutar_predefinida);
			codigo((Instruccion)$1->u.apuntador);
		}
	;

vector: '[' componente ']' {
			$$ = codigo(maquina_crear_vector);
		}
	;

componente:  ESCALAR ',' componente {
			codigo(insertar_escalar);
			codigo((Instruccion)$1);
			codigo(maquina_crear_componente);
			$$ = $3;
		}
	| ESCALAR {
			$$ = codigo(insertar_escalar);
			codigo((Instruccion)$1);
			codigo(maquina_crear_primer_componente);
		}
	| '-' ESCALAR %prec MENOSUNARIO {
			$$ = codigo(insertar_escalar);
			codigo((Instruccion)$2);
			codigo(escalar_negativo);
		}
	;

sentencia: expresion {
			codigo((Instruccion)pop);
		}
	| escalar {
			codigo((Instruccion)pop);
		}
	| IMPRIMIR '(' expresiones ')' {
			$$ = $3;
			codigo(imprimir_cadena);
			codigo((Instruccion)"\n");
		}
	| mientras '(' condicion ')' sentencia fin {
			($1)[1] = (Instruccion)$5; /* Cuerpo del ciclo */
			($1)[2] = (Instruccion)$6; /* Fin del ciclo */
		}
	| si '(' condicion ')' sentencia fin {
			($1)[1] = (Instruccion)$5; /* Cuerpo del condicional */
			($1)[3] = (Instruccion)$6; /* Fin del condicional */
		}
	| si '(' condicion ')' sentencia fin SINO sentencia fin {
			($1)[1] = (Instruccion)$5; /* Cuerpo del condicional */
			($1)[2] = (Instruccion)$8; /* Si la primera condición no se cumple */
			($1)[3] = (Instruccion)$9; /* Fin del condicional */
		}
	| para '(' sentencia { codigo(PARO); } ';' condicion ';' sentencia { codigo(PARO); } ')' sentencia fin {
			($1)[1] = (Instruccion)$6; /* Condición */
			($1)[2] = (Instruccion)$8; /* Paso */
			($1)[3] = (Instruccion)$11; /* Cuerpo del ciclo */
			($1)[4] = (Instruccion)$12; /* Fin del ciclo */
		}
	| '{' sentencias '}' {
			$$ = $2;
		}
	| RETORNAR {
			dentro_de_definicion("retornar");
			codigo(procedimiento_retornar);
		}
	| RETORNAR expresion {
			dentro_de_definicion("retornar");
			$$ = $2;
			codigo(funcion_retornar);
		}
	| PROCEDIMIENTO inicio '(' argumentos ')' {
			$$ = $2;
			codigo(llamada);
			codigo((Instruccion)$1);
			codigo((Instruccion)$4);
		}
	;

expresiones: expresion {
			codigo(imprimir_vector);
		}
	| escalar {
			codigo(imprimir_escalar);
		}
	| CADENA {
			$$ = codigo(imprimir_cadena);
			codigo((Instruccion)$1);
		}
	| expresiones ',' expresion {
			codigo(imprimir_vector);
		}
	| expresiones ',' escalar {
			codigo(imprimir_escalar);
		}
	| expresiones ',' CADENA {
			codigo(imprimir_cadena);
			codigo((Instruccion)$3);
		}
	;

para: PARA {
			$$ = codigo(para);
			codigo(PARO);
			codigo(PARO);
			codigo(PARO);
			codigo(PARO);
		}
	;

mientras: MIENTRAS {
			$$ = codigo(mientras);
			codigo(PARO);
			codigo(PARO);
		}
	;

si: SI {
			$$ = codigo(si);
			codigo(PARO);
			codigo(PARO);
			codigo(PARO);
		}
	;

condicion: escalar {
			codigo(PARO);
		}
	;

sentencias: /* Epsilon */ {
			$$ = cima_programa;
		}
	| sentencias '\n' {
			printf("\u2022\u2022\u2022 ");
		}
	| sentencias sentencia
	;

definicion: FUNC nombreproc {
			$2->tipo = FUNCION;
			fuera_de_definicion = true;
		} '(' ')' sentencia {
			codigo(procedimiento_retornar);
			definir($2);
			fuera_de_definicion = false;
		}
	| PROC nombreproc {
			$2->tipo = PROCEDIMIENTO;
			fuera_de_definicion = true;
		} '(' ')' sentencia {
			codigo(procedimiento_retornar);
			definir($2);
			fuera_de_definicion = false;
		}
	;

nombreproc: VARIABLE
	| FUNCION
	| PROCEDIMIENTO
	;

argumentos: /* Epsilon */ {
			$$ = 0;
		}
	| expresion {
			$$ = 1;
		}
	| argumentos ',' expresion {
			$$ = $1 + 1;
		}
	;

inicio: /* Epsilon */ {
			$$ = cima_programa;
		}
	;

fin: /* Epsilon */ {
			codigo(PARO);
			$$ = cima_programa;
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
		ejecutar(cima_subprograma);

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

	if (c == '"') {
		char sbuf[100], *p, *emalloc();

		for (p = sbuf; (c = getchar()) != '"'; p++) {
			if (c == '\n' || c == EOF)
				ejecutar_error("faltan comillas dobles de cierre", "");
			if (p >= sbuf + sizeof(sbuf) - 1) {
				*p = '\0';
				ejecutar_error("cadena demasiado grande", sbuf);
			}
			*p = backslash(c);
		}

		*p = 0;
		yylval.simbolo = (Simbolo *)emalloc(strlen(sbuf)+1);
		strcpy(yylval.simbolo, sbuf);

		return CADENA;
	}

	if (c == '$') {
		int n = 0;

		while (isdigit(c = getchar()))
			n = 10 * n + c - '0';

		ungetc(c, stdin);

		if (n == 0)
			ejecutar_error("simbolo extraño $...", (char *)0);

		yylval.nargumentos = n;
		return ARGUMENTO;
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

int backslash(int c)
{
	char *index();
	static char transtab[] = "b\bf\fn\nr\rt\t";

	if (c != '\\')
		return c;

	c = getchar();

	if (islower(c) && index(transtab, c))
		return index(transtab, c)[1];

	return c;
}

void dentro_de_definicion(char *s)
{
	if (!fuera_de_definicion)
		ejecutar_error(s, "uso fuera de funcion/procedimiento");
}

void definir(Simbolo *simbolo) /* Insertar una función/procedimiento en la tabla de símbolos */
{
	simbolo->u.definicion = (Instruccion)cima_subprograma; /* Inicio del código de la función/procedimiento */
	cima_subprograma = cima_programa; /* El código después del código de la función/procedimiento comienza aquí */
}
