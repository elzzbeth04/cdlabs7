%{
#include "var.tab.h"   // include tokens from YACC
%}

%%
[a-zA-Z][a-zA-Z0-9_]*    { return VARIABLE; }   // valid identifier pattern
\n                       { return '\n'; }       // newline
.                        { return yytext[0]; }  // any other char
%%

int yywrap(void) {
    return 1;   // stop scanning at EOF
}


%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);                   // declare Lex function
int yyerror(const char *s);        // declare error function
%}

%token VARIABLE

%%
input:
      /* empty */
    | input line
    ;

line:
      VARIABLE '\n'   { printf("Valid variable name\n"); }
    | error '\n'      { printf("Invalid variable name\n"); yyerror; }
    ;
%%

int main(void) {
    printf("Enter a variable name:\n");
    yyparse();   // start parsing
    return 0;
}

int yyerror(const char *s) {
    return 0;   // suppress default error message
}
