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
void execute(Inst *p);
Inst *code(Inst f);
void initcode();
int follow(int expect, int ifyes, int ifno);

%}

%union { /* Tipo de datos de la pila */
    Inst *inst; /* Instrucción de máquina */
    Symbol *sym; /* apuntador al símbolo de la tabla */
}

%token <sym> NUMBER PRINT VAR BLTIN UNDEF WHILE IF ELSE
%type <inst> stmt asgn expr stmtlist cond while if end

%right '='
%left OR
%left AND
%left GT GE LT LE EQ NE
%left '+' '-' /* Asociatividad por la izquierda, misma precedencia */
%left '*' '/' /* Asociatividad por la izquierda, mayor precedencia */
%left UNARYMINUS NOT /* Nuevo */
%right '^' /* Exponenciación */

%%

list: /* Nada */
    | list '\n'
    | list asgn '\n' { code((Inst)pop); code(STOP); return 1; }
    | list stmt '\n' { code(STOP); return 1; }
    | list expr '\n' { code(print); code(STOP); return 1; }
    | list error '\n' { yyerrok; }
    ;

asgn: VAR '=' expr { $$ = $3; code(varpush); code((Inst)$1); code(assign); }
    ;

stmt: expr { code((Inst)pop); }
    | PRINT expr { code(prexpr); $$ = $2; }
    | while cond stmt end {
        ($1)[1] = (Inst)$3; /* Cuerpo del ciclo */
        ($1)[2] = (Inst)$4; /* Finaliza si la condición falla */
    }
    | if cond stmt end { /* if sin else */
        ($1)[1] = (Inst)$3; /* Bloque a ejecutar si la condición se cumple */
        ($1)[3] = (Inst)$4; /* Finaliza si la condición falla */
    }
    | if cond stmt end ELSE stmt end { /* if con else */
        ($1)[1] = (Inst)$3; /* Bloque a ejecutar si la condición se cumple */
        ($1)[2] = (Inst)$6; /* Bloque else */
        ($1)[3] = (Inst)$7; /* Finaliza si la condición falla */
    }
    | '{' stmtlist '}' { $$ = $2; }
    ;

cond: '(' expr ')' { code(STOP); $$ = $2; }
    ;

while: WHILE { $$ = code(whilecode); code(STOP); code(STOP); }
    ;

if: IF { $$ = code(ifcode); code(STOP); code(STOP); code(STOP); }
    ;

end: /* Nada */ { code(STOP); $$ = progp; } 
    ;

stmtlist: /* Nada */ { $$ = progp; }
    | stmtlist '\n'
    | stmtlist stmt
    ;

expr: NUMBER { $$ = code(constpush); code((Inst)$1); }
    | VAR { $$ = code(varpush); code((Inst)$1); code(eval); }
    | asgn
    | BLTIN '(' expr ')' { $$ = $3; code(bltin); code((Inst)$1->u.ptr); }
    | expr '+' expr { code(add); }
    | expr '-' expr { code(sub); }
    | expr '*' expr { code(mul); }
    | expr '/' expr { code(division); }
    | expr '^' expr { code(power); }
    | '(' expr ')' { $$ = $2; }
    | '-' expr %prec UNARYMINUS { $$ = $2; code(negate); } /* Nuevo */
    | expr GT expr { code(gt); }
    | expr GE expr { code(ge); }
    | expr LT expr { code(lt); }
    | expr LE expr { code(le); }
    | expr EQ expr { code(eq); }
    | expr NE expr { code(ne); }
    | expr AND expr { code(and); }
    | expr OR expr { code(or); }
    | NOT expr { $$ = $2; code(not); }
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

    for (initcode(); yyparse(); initcode())
        execute(prog);

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
        double d;
        ungetc(c, stdin);
        scanf("%lf", &d);
        yylval.sym = install("", NUMBER, d);

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

    switch (c) {
        case '>':
            return follow('=', GE, GT);
            break;
        case '<':
            return follow('=', LE, LT);
            break;
        case '=':
            return follow('=', EQ, '=');
            break;
        case '!':
            return follow('=', NE, NOT);
            break;
        case '|':
            return follow('|', OR, '|');
            break;
        case '&':
            return follow('&', AND, '&');
            break;
        case '\n':
            lineno++;
            return '\n';
            break;
        default:
            return c;
            break;
    }

    // if (c == '\n')
    //     lineno++;

    // return c;
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

int follow(int expect, int ifyes, int ifno) /* Busca los >=, <=, etc. */
{
    int c = getchar();

    if (c == expect)
        return ifyes;

    ungetc(c, stdin);

    return ifno;
}
