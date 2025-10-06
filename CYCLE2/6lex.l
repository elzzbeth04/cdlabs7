%{
#include "y.tab.h"
#include <stdlib.h>
%}

%%

[0-9]+              { yylval.num = atoi(yytext); return NUMBER; }
[ \t\r]+            { /* skip whitespace */ }
\n                  { return '\n'; }
"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"("                 { return '('; }
")"                 { return ')'; }
.                   { return yytext[0]; }   /* any other single char */

%%

int yywrap(void) { return 1; }














%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AST {
    char *nodeType;
    int value;
    struct AST *left, *right;
} AST;

AST* makeNode(const char *type, AST *l, AST *r) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->nodeType = strdup(type);
    n->value = 0;
    n->left = l;
    n->right = r;
    return n;
}

AST* makeNum(int val) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->nodeType = strdup("NUM");
    n->value = val;
    n->left = n->right = NULL;
    return n;
}

void printAST(AST *root, int lvl) {
    if (!root) return;
    for (int i = 0; i < lvl; i++) printf("  ");
    if (strcmp(root->nodeType, "NUM") == 0)
        printf("NUM(%d)\n", root->value);
    else
        printf("%s\n", root->nodeType);
    printAST(root->left, lvl + 1);
    printAST(root->right, lvl + 1);
}

int evalAST(AST *root) {
    if (!root) return 0;
    if (strcmp(root->nodeType, "NUM") == 0) return root->value;
    if (strcmp(root->nodeType, "+") == 0) return evalAST(root->left) + evalAST(root->right);
    if (strcmp(root->nodeType, "-") == 0) return evalAST(root->left) - evalAST(root->right);
    if (strcmp(root->nodeType, "*") == 0) return evalAST(root->left) * evalAST(root->right);
    if (strcmp(root->nodeType, "/") == 0) return evalAST(root->left) / evalAST(root->right);
    if (strcmp(root->nodeType, "NEG") == 0) return - evalAST(root->left);
    return 0;
}

int yylex(void);
int yyerror(const char *s);
%}

%union {
    int num;
    AST *node;
}

%token <num> NUMBER
%type <node> expr

%left '+' '-'
%left '*' '/'
%right UMINUS

%%

input:
      /* empty */
    | input line
    ;

line:
      '\n'
    | expr '\n'   { 
          printf("\nAST:\n"); 
          printAST($1, 0); 
          printf("Result: %d\n\n", evalAST($1)); 
      }
    ;

expr:
      expr '+' expr   { $$ = makeNode("+", $1, $3); }
    | expr '-' expr   { $$ = makeNode("-", $1, $3); }
    | expr '*' expr   { $$ = makeNode("*", $1, $3); }
    | expr '/' expr   { $$ = makeNode("/", $1, $3); }
    | '-' expr  %prec UMINUS  { $$ = makeNode("NEG", $2, NULL); }
    | '(' expr ')'    { $$ = $2; }
    | NUMBER          { $$ = makeNum($1); }
    ;

%%

int main(void) {
    printf("Enter expressions, one per line (Ctrl+D to quit):\n");
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
    return 0;
}
