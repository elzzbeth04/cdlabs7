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
    printf("GRAMMAR is:\nE -> E+E | E*E | (E) | id\n");
    printf("Enter input string: ");
    scanf("%s", input);

    input_len = strlen(input);

    printf("\nStack\t\tInput\t\tAction\n");

    while (curr_pos < input_len) {
        // Handle 'id' as one token
        if (input[curr_pos] == 'i' && input[curr_pos + 1] == 'd') {
            stack[++stack_top] = 'I'; // represent id by 'I'
            stack[stack_top + 1] = '\0';
            printf("$");
            print_stack();
            printf("\t\t%s$\t\tSHIFT->id\n", input + curr_pos + 2);
            curr_pos += 2;
            check();
        } else {
            // Handle single symbols +, *, (, )
            stack[++stack_top] = input[curr_pos];
            stack[stack_top + 1] = '\0';
            printf("$");
            print_stack();
            printf("\t\t%s$\t\tSHIFT->%c\n", input + curr_pos + 1, input[curr_pos]);
            curr_pos++;
            check();
        }
    }

    if (stack_top == 0 && stack[0] == 'E') {
        printf("\nInput string is VALID.\n");
    } else {
        printf("\nInput string is INVALID.\n");
    }

    return 0;
}

void check() {
    char *handle;
    int handle_size;

    while (1) {
        // Reduce id → E
        if (stack_top >= 0 && stack[stack_top] == 'I') {
            handle = "id";
            handle_size = 1;
        }

        // Reduce E+E → E
        else if (stack_top >= 2 &&
                 stack[stack_top - 2] == 'E' &&
                 stack[stack_top - 1] == '+' &&
                 stack[stack_top] == 'E') {
            handle = "E+E";
            handle_size = 3;
        }

        // Reduce E*E → E
        else if (stack_top >= 2 &&
                 stack[stack_top - 2] == 'E' &&
                 stack[stack_top - 1] == '*' &&
                 stack[stack_top] == 'E') {
            handle = "E*E";
            handle_size = 3;
        }

        // Reduce (E) → E
        else if (stack_top >= 2 &&
                 stack[stack_top - 2] == '(' &&
                 stack[stack_top - 1] == 'E' &&
                 stack[stack_top] == ')') {
            handle = "(E)";
            handle_size = 3;
        }

        else {
            // no handle found — stop reducing
            return;
        }

        // Perform the reduction
        stack_top -= handle_size - 1;
        stack[stack_top] = 'E';
        stack[stack_top + 1] = '\0';

        printf("$");
        print_stack();
        printf("\t\t%s$\t\tREDUCE->%s\n", input + curr_pos, handle);
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
