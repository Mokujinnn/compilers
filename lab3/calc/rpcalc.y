%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

int yylex();
void yyerror(char const *);
%}

/* Type for semantic values (default int) */
%define api.value.type {double}

%token NUMBER
%token PLUS MINUS MULTIPLY DIVIDE POWER
%token LPAREN RPAREN
%token UMINUS 

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right POWER
%left UMINUS 

%%

input:
    %empty
    | input line
    ;

line:
    '\n'
    | expr '\n' { printf("%g\n", $1); }
    ;

expr:
    NUMBER                         { $$ = $1; }
    | expr PLUS expr               { $$ = $1 + $3; }
    | expr MINUS expr              { $$ = $1 - $3; }
    | expr MULTIPLY expr           { $$ = $1 * $3; }
    | expr DIVIDE expr             { 
                                        if ($3 == 0.0) {
                                            yyerror("Division by zero");
                                            YYABORT;
                                        }
                                        $$ = $1 / $3; 
                                    }
    | LPAREN expr RPAREN            { $$ = $2; }
    | MINUS expr %prec UMINUS       { $$ = -$2; }
    | expr POWER expr               { $$ = pow($1, $3); }
    ;

%%


int yylex()
{
    int c = getchar();
    while (c == ' ' || c == '\t') {
        c = getchar();
    }

    if (c == '.' || isdigit(c)) {
        ungetc(c, stdin);
        if (scanf("%lf", &yylval) != 1)
            abort();
        return NUMBER;
    } else if (c == '(') {
        return LPAREN;
    } else if (c == ')') {
        return RPAREN;
    } else if (c == '+') {
        return PLUS;
    } else if (c == '-') {
        return MINUS;
    } else if (c == '*') {
        return MULTIPLY;
    } else if (c == '/') {
        return DIVIDE;
    } else if (c == '^') {
        return POWER;
    }  else if (c == EOF) {
        return YYEOF;
    }
    return c;
}

/* Called by yyparse on error */
void yyerror (char const *s)
{
    fprintf (stderr, "Error: %s\n", s);
}

int main()
{
    return yyparse();
}
