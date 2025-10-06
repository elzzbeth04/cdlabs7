%{
#include <stdio.h>
#include <string.h>
int flag = 0;
char name[100] = "";
%}

%%
[a-zA-Z]*   {
                // check if "eliz" is inside the current word
                for(int i=0; i<yyleng; i++) {
                    if(yytext[i]=='e' && yytext[i+1]=='l' && yytext[i+2]=='i' && yytext[i+3]=='z') {
                        flag = 1;
                    }
                }
                strcpy(name, yytext);
            }

\n          {
                if(flag==1) {
                    printf("%s contains 'eliz' as substring\n", name);
                } else {
                    printf("%s does not contain 'eliz' as substring\n", name);
                }
                // reset values for next input
                flag = 0;
                strcpy(name, "");
                printf("Enter the string: ");
            }
%%

int main() {
    printf("Enter the string: ");
    yylex();   // start lexical analysis
    return 0;
}
