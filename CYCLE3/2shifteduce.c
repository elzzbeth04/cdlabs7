#include <stdio.h>
#include <string.h>

#define MAX_INPUT 100
#define MAX_STACK 100

char input[MAX_INPUT], stack[MAX_STACK];
int input_len, stack_top = -1;
int curr_pos = 0;

void check();
void print_stack();

int main() {
    printf("GRAMMAR:\n");
    printf("E → E + E | E * E | (E) | id\n");
    printf("---------------------------------\n");

    printf("Enter input string: ");
    scanf("%s", input);

    input_len = strlen(input);

    printf("\n%-20s %-20s %-20s\n", "Stack", "Input", "Action");
    printf("------------------------------------------------------------------\n");

    while (curr_pos < input_len) {
        // Handle 'id' as one token
        if (input[curr_pos] == 'i' && input[curr_pos + 1] == 'd') {
            stack[++stack_top] = 'I'; // represent id by 'I'
            stack[stack_top + 1] = '\0';

            printf("$%-18s $%-18s SHIFT → id\n", stack, input + curr_pos + 2);
            curr_pos += 2;
            check();
        } 
        else {
            // Handle single symbols +, *, (, )
            stack[++stack_top] = input[curr_pos];
            stack[stack_top + 1] = '\0';

            printf("$%-18s $%-18s SHIFT → %c\n", stack, input + curr_pos + 1, input[curr_pos]);
            curr_pos++;
            check();
        }
    }

    printf("------------------------------------------------------------------\n");
    if (stack_top == 0 && stack[0] == 'E')
        printf("\n✅ Input string is VALID.\n");
    else
        printf("\n❌ Input string is INVALID.\n");

    return 0;
}

void check() {
    int reduced = 1;

    while (reduced) {
        reduced = 0;

        // E → id
        if (stack_top >= 0 && stack[stack_top] == 'I') {
            stack[stack_top] = 'E';
            printf("$%-18s $%-18s REDUCE → E → id\n", stack, input + curr_pos);
            reduced = 1;
        }
        // E → E + E
        else if (stack_top >= 2 &&
                 stack[stack_top - 2] == 'E' &&
                 stack[stack_top - 1] == '+' &&
                 stack[stack_top] == 'E') {
            stack_top -= 2;
            stack[stack_top] = 'E';
            stack[stack_top + 1] = '\0';
            printf("$%-18s $%-18s REDUCE → E → E+E\n", stack, input + curr_pos);
            reduced = 1;
        }
        // E → E * E
        else if (stack_top >= 2 &&
                 stack[stack_top - 2] == 'E' &&
                 stack[stack_top - 1] == '*' &&
                 stack[stack_top] == 'E') {
            stack_top -= 2;
            stack[stack_top] = 'E';
            stack[stack_top + 1] = '\0';
            printf("$%-18s $%-18s REDUCE → E → E*E\n", stack, input + curr_pos);
            reduced = 1;
        }
        // E → (E)
        else if (stack_top >= 2 &&
                 stack[stack_top - 2] == '(' &&
                 stack[stack_top - 1] == 'E' &&
                 stack[stack_top] == ')') {
            stack_top -= 2;
            stack[stack_top] = 'E';
            stack[stack_top + 1] = '\0';
            printf("$%-18s $%-18s REDUCE → E → (E)\n", stack, input + curr_pos);
            reduced = 1;
        }
    }
}

void print_stack() {
    for (int i = 0; i <= stack_top; i++) {
        if (stack[i] == 'I')
            printf("id");
        else
            printf("%c", stack[i]);
    }
}
