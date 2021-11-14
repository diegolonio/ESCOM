%{

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>
#include "hoc.h"

int yylex();
void yyerror(char *s);
void warning(char *s, char *t);
void execerror(char *s, char *t);
void fpecatch();
void init();
void execute(Inst *p);
void initcode();
int follow(int expect, int ifyes, int ifno);
void defonly(char *s);

int indef;


%}

%union { /* Tipo de datos de la pila */
    Inst *inst; /* Instrucción de máquina */
    Symbol *sym; /* apuntador al símbolo de la tabla */
    int narg; /* Número de argumentos */
}

%token <sym> NUMBER STRING PRINT VAR BLTIN UNDEF WHILE IF ELSE
%token <sym> FUNCTION PROCEDURE RETURN FUNC PROC READ
%token <narg> ARG
%type <inst> expr stmt asgn prlist stmtlist
%type <inst> cond while if begin end
%type <sym> procname
%type <narg> arglist

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
    | list defn '\n'
    | list asgn '\n' { code((Inst)pop); code(STOP); return 1; }
    | list stmt '\n' { code(STOP); return 1; }
    | list expr '\n' { code(print); code(STOP); return 1; }
    | list error '\n' { yyerrok; }
    ;

asgn: VAR '=' expr { code(varpush); code((Inst)$1); code(assign); $$ = $3; }
    | ARG '=' expr { defonly("$"); code(argassign); code((Inst)$1); $$ = $3; }
    ;

stmt: expr { code((Inst)pop); }
    | RETURN { defonly("return"); code(procret); }
    | RETURN expr { defonly("return"); $$ = $2; code(funcret); }
    | PROCEDURE begin '(' arglist ')' { $$ = $2; code(call); code((Inst)$1); code((Inst)$4); }
    | PRINT prlist { $$ = $2; }
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
    | ARG { defonly("$"); $$ = code(arg); code((Inst)$1); }
    | asgn
    | FUNCTION begin '(' arglist ')' { $$ = $2; code(call); code((Inst)$1); code((Inst)$4); }
    | READ '(' VAR ')' { $$ = code(varread); code((Inst)$3); }
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

begin: /* Nada */ { $$ = progp; }
    ;

prlist: expr { code(prexpr); }
    | STRING { $$ = code(prstr); code((Inst)$1); }
    | prlist ',' expr { code(prexpr); }
    | prlist ',' STRING { code(prstr); code((Inst)$3); }
    ;

defn: FUNC procname { $2->type = FUNCTION; indef = 1; } '(' ')' stmt { code(procret); define($2); indef = 0; }
    | PROC procname { $2->type = PROCEDURE; indef = 1; } '(' ')' stmt { code(procret); define($2); indef = 0; }
    ;

procname: VAR
    | FUNCTION
    | PROCEDURE
    ;

arglist: /* Nada */ { $$ = 0; }
    | expr { $$ = (int)$1; }
    | arglist ',' expr { $$ = $1 + 1; }
    ;

%%

char *progname; /* Para mensajes de error */
int lineno = 1;
jmp_buf begin;
char *infile; /* Nombre del archivo de entrada */
char **gargv; /* Lista global de argumentos */
int gargc;

int c; /* Variable global para uso de warning */

int main(int argc, char *argv[])
{
    int i;
    progname = argv[0];

    if (argc == 1) {
        static char *stdinonly[] = { "-" };
        gargv = stdinonly;
        gargc = 1;
    } else {
        gargv = argv+1;
        gargc = argc-1;
    }

    init();

    while (moreinput())
        run();

    return 0;
}

int yylex()
{
    while ((c = getc(fin)) == ' ' || c == '\t')
        ;

    if (c == EOF)
        return 0;

    if (c == '.' || isdigit(c)) { /* Número */
        double d;
        ungetc(c, fin);
        fscanf(fin, "%lf", &d);
        yylval.sym = install("", NUMBER, d);

        return NUMBER;
    }

    if (isalpha(c)) {
        Symbol *s;
        char sbuf[100], *p = sbuf;

        do {
            if (p >= sbuf + sizeof(sbuf) - 1) {
                *p = '\0';
                execerror("name too long", sbuf);
            }
            *p++ = c;
        } while ((c = getc(fin)) != EOF && isalnum(c));

        ungetc(c, fin);
        *p = '\0';

        if ((s = lookup(sbuf)) == 0)
            s = install(sbuf, UNDEF, 0.0);

        yylval.sym = s;

        return s->type == UNDEF ? VAR : s->type;
    }

    if (c == '$') { /* ¿Es un argumento? */
        int n = 0;

        while (isdigit(c = getc(fin)))
            n = 10 * n + c - '0';

        ungetc(c, fin);

        if (n == 0)
            execerror("strange $...", (char *)0);

        yylval.narg = n;

        return ARG;
    }

    if (c == '"') { /* Cadena entre comillas */
        char sbuf[100], *p, *emalloc();

        for (p = sbuf; (c = getc(fin)) != '"'; p++) {
            if (c == '\n' || c == EOF)
                execerror("missing quote", "");

            if (p >= sbuf + sizeof(sbuf) - 1) {
                *p = '\0';
                execerror("string too long", sbuf);
            }

            *p = backslash(c);
        }

        *p = 0;
        yylval.sym = (Symbol *)emalloc(strlen(sbuf)+1);
        strcpy(yylval.sym->u.str, sbuf);

        return STRING;
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

    if (infile)
        fprintf(stderr, " in %s\n", infile);

    fprintf(stderr, " near line %d\n", lineno);

    while (c != '\n' && c != EOF)
        c = getc(fin);

    if (c == '\n')
        lineno++;
}

void execerror(char *s, char *t) /* Recuperarse del error en tiempo de ejecución */
{
    warning(s, t);
    fseek(fin, 0L, 2);
    longjmp(begin, 0);
}

void fpecatch() /* Atrapar excepciones de punto flotante */
{
    execerror("floating point exception", (char *)0);
}

int follow(int expect, int ifyes, int ifno) /* Busca los >=, <=, etc. */
{
    int c = getc(fin);

    if (c == expect)
        return ifyes;

    ungetc(c, fin);

    return ifno;
}

void defonly(char *s) /* Advertir de una definición ilegal */
{
    if (!indef)
        execerror(s, "used outside definition");
}

int backslash(int c) /* Obtener el siguiente caracter con los \'s interpretados */
{
    char *index(); /* 'strchr()' en algunos sistemas */
    static char transtab[] = "b\bf\fn\nr\rt\t";

    if (c != '\\')
        return c;

    c = getc(fin);

    if (islower(c) && index(transtab, c))
        return index(transtab, c)[1];

    return c;
}

int moreinput()
{
    if (gargc-- <= 0)
        return 0;
    
    if (fin && fin != stdin)
        fclose(fin);

    infile = *gargv++;
    lineno = 1;

    if (strcmp(infile, "-") == 0) {
        fin = stdin;
        infile = 0;
    } else if ((fin = fopen(infile, "r")) == NULL) {
        fprintf(stderr, "%s: can't open %s\n", progname, infile);
        return moreinput();
    }

    return 1;
}

void run() /* Ejecutar hasta encontrar EOF */
{
    setjmp(begin);
    signal(SIGFPE, fpecatch);
    for (initcode(); yyparse(); initcode())
        execute(progbase);
}
