
%{
#include "y.tab.h"
%}

%%
[0-9]+          { yylval = atoi(yytext); return NUMBER; }   // integers
[ \t]           ;   // ignore spaces and tabs
\n              { return '\n'; }
.               { return yytext[0]; }
%%

int yywrap(void) { return 1; }

%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
int yyerror(const char *s);
%}

%token NUMBER

%%
input:
      /* empty */
    | input line
    ;

line:
      expr '\n'   { printf("= %d\n", $1); }
    | '\n'
    ;

expr:
      expr '+' expr   { $$ = $1 + $3; }
    | expr '-' expr   { $$ = $1 - $3; }
    | expr '*' expr   { $$ = $1 * $3; }
    | expr '/' expr   { if ($3==0) { printf("Error: Divide by zero\n"); $$=0; } else $$ = $1 / $3; }
    | '(' expr ')'    { $$ = $2; }
    | NUMBER          { $$ = $1; }
    ;
%%

int main(void) {
    printf("Enter expressions (Ctrl+D to quit):\n");
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    printf("Error: %s\n", s);
    return 0;
}
