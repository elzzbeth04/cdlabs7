    #include <stdio.h>
    #include <ctype.h>
    #include <string.h>
    #include <stdlib.h>

    #define MAX_TOKEN_LEN 100

    // List of keywords

    char *keywords[] = {"int", "float", "if", "else", "while", "return"};
    int keywordCount = 6;

    // Function to check if token is a keyword
    int isKeyword(char *token) {
        for (int i = 0; i < keywordCount; i++) {
            if (strcmp(keywords[i], token) == 0)
                return 1;
        }
        return 0;
    } 

    // Function to recognize tokens
    void lexicalAnalyzer(FILE *fp) {
        char ch;
        char buffer[MAX_TOKEN_LEN];
        int i = 0;

        while ((ch = fgetc(fp)) != EOF) {
            // Ignore whitespace, tabs, newlines
            if (ch == ' ' || ch == '\t' || ch == '\n')
                continue;

            // Identifiers or keywords (start with letter)
            if (isalpha(ch)) {
                i = 0;
                buffer[i++] = ch;
                while (isalnum(ch = fgetc(fp))) {
                    buffer[i++] = ch;
                }
                buffer[i] = '\0';
                ungetc(ch, fp); // put back last read character

                if (isKeyword(buffer))
                    printf("Keyword: %s\n", buffer);
                else
                    printf("Identifier: %s\n", buffer);
            }

            // Numbers
            else if (isdigit(ch)) {
                i = 0;
                buffer[i++] = ch;
                while (isdigit(ch = fgetc(fp))) {
                    buffer[i++] = ch;
                }
                buffer[i] = '\0';
                ungetc(ch, fp);
                printf("Number: %s\n", buffer);
            }

            // Operators
            else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
                    ch == '=' || ch == '<' || ch == '>') {
                printf("Operator: %c\n", ch);
            }

            // Punctuation
            else if (ch == ';' || ch == ',' || ch == '(' || ch == ')' ||
                    ch == '{' || ch == '}') {
                printf("Punctuation: %c\n", ch);
            }
        }
    }

    int main() {
        FILE *fp;
        char filename[50];

        printf("Enter source code filename: ");
        scanf("%s", filename);

        fp = fopen(filename, "r");
        if (!fp) {
            printf("Cannot open file %s\n", filename);
            return 1;
        }

        lexicalAnalyzer(fp);
        fclose(fp);

        return 0;
    }
