#include <stdio.h>
#include <stdlib.h>
#define MAX 20
struct node {
    int st;
    struct node* link;
};
struct node1 {
    int nst[MAX];
};
int nostate, noalpha, start, nofinal, notransition;
int finalstate[MAX], set[MAX];
char alphabet[MAX];
struct node* transition[MAX][MAX] = {NULL};
struct node1 hash[MAX];
int complete = -1;
// Compare two DFA states
int compare(struct node1 a, struct node1 b) {
    for (int i = 1; i <= nostate; i++)
        if (a.nst[i] != b.nst[i])
            return 0;
    return 1;
}
int insertDfaState(struct node1 newstate) {
    for (int i = 0; i <= complete; i++)
        if (compare(hash[i], newstate))
            return 0;
    complete++;
    hash[complete] = newstate;
    return 1;
}
int findalpha(char c) {
    for (int i = 0; i < noalpha; i++)
        if (alphabet[i] == c)
            return i;
    return 999;
}
void insert(int r, char c, int s) {
    int j = findalpha(c);
    if (j == 999) {
        printf("Error: invalid alphabet in insert function.\n");
        exit(0);
    }
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->st = s;
    temp->link = transition[r][j];
    transition[r][j] = temp;
}

void printnewstate(struct node1 state) {
    printf("{");
    int first = 1;
    for (int j = 1; j <= nostate; j++) {
        if (state.nst[j] != 0) {
            if (!first) printf(" ");
            printf("q%d", state.nst[j]);
            first = 0;
        }
    }
    printf("}");
}
int isFinalState(struct node1 state) {
    for (int i = 1; i <= nostate; i++)
        for (int j = 0; j < nofinal; j++)
            if (state.nst[i] == finalstate[j])
                return 1;
    return 0;
}
int main() {
    int i, j, k, l, r, s;
    struct node* temp;
    struct node1 newstate = {0}, tmpstate = {0};
    printf("NOTE: Use letter 'e' as epsilon (if present, it must be last alphabet)\n");
    printf("Enter the number of alphabets: ");
    scanf("%d", &noalpha);
    getchar();
    printf("Enter the alphabets:\n");
    for (i = 0; i < noalpha; i++) {
        alphabet[i] = getchar();
        getchar();
    }
    printf("Enter the number of states: ");
    scanf("%d", &nostate);
    printf("Enter the start state: ");
    scanf("%d", &start);
    printf("Enter the number of final states: ");
    scanf("%d", &nofinal);
    printf("Enter the final states:\n");
    for (i = 0; i < nofinal; i++)
        scanf("%d", &finalstate[i]);
    printf("Enter the number of transitions: ");
    scanf("%d", &notransition);
    printf("Enter transitions in form: qno alphabet qno\n");
    for (i = 0; i < notransition; i++) {
        char alph;
        scanf("%d %c %d", &r, &alph, &s);
        insert(r, alph, s);
    }

    newstate.nst[start] = start;
    insertDfaState(newstate);
    i = -1;

    printf("\nEquivalent DFA.....\n");
    printf(".......................\n");

    // Print DFA transition table header
    printf("Transitions of DFA\n\t");
    for (k = 0; k < noalpha; k++)
        printf("%c\t", alphabet[k]);
    printf("\n");
    while (i != complete) {
        i++;
        newstate = hash[i];
        printnewstate(newstate);
        printf("\t");

        for (k = 0; k < noalpha; k++) {
            int c = 0;
            for (j = 1; j <= nostate; j++)
                set[j] = 0;

            for (j = 1; j <= nostate; j++) {
                l = newstate.nst[j];
                if (l) {
                    temp = transition[l][k];
                    while (temp) {
                        set[temp->st] = temp->st;
                        temp = temp->link;
                    }
                }
            }
            for (j = 1; j <= nostate; j++)
                tmpstate.nst[j] = set[j];

            if (tmpstate.nst[1] != 0 || tmpstate.nst[2] != 0) // not empty
                insertDfaState(tmpstate);

            printnewstate(tmpstate);
            printf("\t");
        }
        printf("\n");
    }
    printf("\nStates of DFA:\n");
    for (i = 0; i <= complete; i++) {
        printnewstate(hash[i]);
        printf("\t");
    }

    printf("\nAlphabets: ");
    for (i = 0; i < noalpha; i++)
        printf("%c\t", alphabet[i]);

    printf("\nStart State: ");
    printnewstate(hash[0]);
    printf("\nFinal states: ");
    for (i = 0; i <= complete; i++)
        if (isFinalState(hash[i])) {
            printnewstate(hash[i]);
            printf("\t");
        }
    printf("\n");
    return 0;
}
