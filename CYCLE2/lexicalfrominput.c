#include <stdio.h>
#include <ctype.h>
void analyze(const char *code) {
    int i = 0;
    char buffer[100];
    int j = 0;
    while (code[i] != '\0') {
        char ch = code[i];
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            i++;
            continue;
        }
        if (isalpha(ch)) {
            j = 0;
            while (isalnum(code[i])) {
                buffer[j++] = code[i++];
            }
            buffer[j] = '\0';
            printf("IDENTIFIER: %s\n", buffer);
        }
        else if (isdigit(ch)) {
            j = 0;
            while (isdigit(code[i])) {
                buffer[j++] = code[i++];
            }
            buffer[j] = '\0';
            printf("NUMBER: %s\n", buffer);
        }
        else {
            // Special symbol
            printf("SYMBOL: %c\n", ch);
            i++;
        }
    }
}
int main() {
    char input[1000];
    printf("Enter code (end with $):\n");
    int i = 0;
    char ch;
    while ((ch = getchar()) != '$') {
        input[i++] = ch;
    }
    input[i] = '\0';
    printf("\n--- Tokens ---\n");
    analyze(input);
    return 0;
}
