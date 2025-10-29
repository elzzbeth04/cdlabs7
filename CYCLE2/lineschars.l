%{
#include <stdio.h>

int lines = 0, words = 0, chars = 0;
%}

%%
\n      { lines++; chars++; }
[ \t]+  { chars += yyleng; }
[^ \t\n]+ { words++; chars += yyleng; }
.       { chars++; }
%%

int main() {
    yylex();
    printf("\nNumber of lines: %d\n", lines);
    printf("Number of words: %d\n", words);
    printf("Number of characters: %d\n", chars);
    return 0;
}

int yywrap() { return 1; }
