
#include <stdio.h> 
#include <string.h> 
 
void gen_code_for_operator(char *expr, char operator, char *reg) { 
    int i = 0, j = 0; 
    char temp[100]; 
    while (expr[i] != '\0') { 
        if (expr[i] == operator) { 
            printf("%c\t%c\t%c\t%c\n", operator, *reg, expr[i-1], expr[i+1]); 
            temp[j-1] = *reg; // Replace left operand with register 
            i += 2;           // Skip operator and right operand 
            (*reg)--;         // Use next register 
            continue; 
        } 
        temp[j++] = expr[i++]; 
    } 
    temp[j] = '\0'; 
    strcpy(expr, temp); 
} 
void gen_code(char *expr) { 
    char reg = 'Z'; // Start from register Z 
    // Precedence: /, *, +, -, = 
    gen_code_for_operator(expr, '/', &reg); 
    gen_code_for_operator(expr, '*', &reg); 
    gen_code_for_operator(expr, '+', &reg); 
    gen_code_for_operator(expr, '-', &reg); 
    gen_code_for_operator(expr, '=', &reg); 
} 
int main() { 
    char expr[100]; 
    printf("Enter expression:\n"); 
    scanf("%s", expr); 
 
    printf("Op\tDestn\tArg1\tArg2\n"); 
    gen_code(expr); 
 
    return 0; 
}