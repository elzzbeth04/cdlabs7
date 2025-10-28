#include <stdio.h>
#include <string.h>
#define MAX 100

char stack[MAX], input[MAX], terminals[100], table[10][10];
int n;

int idx(char symbol) {
    for (int i = 0; i < n; i++) {
        if (terminals[i] == symbol)
            return i;
    }
    return -1;
}

int main() {
    int top = 0, i = 0;
    printf("Enter number of terminals: ");
    scanf("%d", &n);

    printf("Enter the terminals (no spaces, e.g. i+*()$): ");
    scanf("%s", terminals);

    printf("\nEnter operator precedence table (<, >, =):\n");
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            printf("%c vs %c: ", terminals[r], terminals[c]);
            scanf(" %c", &table[r][c]);  // leading space to skip newlines
        }
    }

    printf("\nEnter input string: ");
    scanf("%s", input);
    strcat(input, "$"); // append end marker

    stack[top] = '$';
    stack[top + 1] = '\0';

    printf("\nSTACK\t\tINPUT\t\tACTION\n");

    while (1) {
        char a = stack[top];  // top of stack
        char b = input[i];   // next input symbol

        int row = idx(a);
        int col = idx(b);

        if (row == -1 || col == -1) {
            printf("Error: invalid symbol.\n");
            break;
        }

        if (a == '$' && b == '$') {
            printf("%s\t\t%s\t\tAccept\n", stack, input + i);
            break;
        }

        char relation = table[row][col];
        if (relation == '<' || relation == '=') {
            stack[++top] = relation;
            stack[++top] = b;
            stack[top + 1] = '\0';
            printf("%s\t\t%s\t\tShift %c\n", stack, input + i, b);
            i++;
        } else if (relation == '>') {
            while (stack[top] != '<' && top > 0)
                top--;
            if (top > 0)
                top--;
            stack[top + 1] = '\0';
            printf("%s\t\t%s\t\tReduce\n", stack, input + i);
        } else {
            printf("%s\t\t%s\t\tError (no relation)\n", stack, input + i);
            break;
        }
    }

    return 0;
}
