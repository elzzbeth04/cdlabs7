%{
#include "y.tab.h"
%}

%%
"for"            { return FOR; }             // keyword
"++"             { return INCOP; }           // increment operator
"--"             { return DECOP; }           // decrement operator
"<="|">="|"=="|"!="|"<"|">"  { return RELOP; }  // relational operators
"="              { return ASSIGN; }          
";"              { return SEMICOLON; }
"("              { return LPAREN; }
")"              { return RPAREN; }
[0-9]+           { return NUM; }             // numbers
[a-zA-Z_][a-zA-Z0-9_]*   { return ID; }      // identifiers (variables)
[ \t\n]          { /* skip whitespace */ }   // ignore spaces, tabs, newlines
.                { /* ignore invalid characters */ }
%%

int yywrap() {
    return 1;
}
%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror(const char *s);
%}
%token FOR ID NUM RELOP INCOP DECOP ASSIGN SEMICOLON LPAREN RPAREN

%%
for_stmt : FOR LPAREN expr SEMICOLON condition SEMICOLON expr RPAREN
           { printf("Valid FOR statement syntax\n"); }
         ;

expr     : ID ASSIGN ID
         | ID ASSIGN NUM
         | ID INCOP
         | ID DECOP
         | /* empty */
         ;

condition : ID RELOP ID
          | ID RELOP NUM
          | /* empty */
          ;
%%

void yyerror(const char *s) {
    printf("Invalid FOR statement syntax\n");
}
int main() {
    printf("Enter a FOR statement:\n");
    yyparse();
    return 0;
}

