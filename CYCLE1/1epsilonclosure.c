#include <stdio.h>
#include <stdlib.h>

#define MAX 20

int n;
int eclosure[MAX][MAX];
int visited[MAX];
int epsilon[MAX][MAX];

void dfs(int state, int index) {
    for (int i = 0; i < n; i++) {
        if (epsilon[state][i] && !visited[i]) {
            visited[i] = 1;
            eclosure[index][i] = 1;
            dfs(i, index);
        }
    }
}

int main() {
    int i, j, m, from, to;

    printf("Enter number of states: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            epsilon[i][j] = 0;
            eclosure[i][j] = 0;
        }
    }

    printf("Enter number of epsilon transitions: ");
    scanf("%d", &m);

    printf("Enter epsilon transitions (from to):\n");
    for (i = 0; i < m; i++) {
        scanf("%d %d", &from, &to);
        epsilon[from][to] = 1;
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) 
        visited[j] = 0;

        visited[i] = 1;
        eclosure[i][i] = 1;
        dfs(i, i);
    }

    printf("\nEpsilon-closures:\n");
    for (i = 0; i < n; i++) {
        printf("ε-closure(q%d): { ", i);
        for (j = 0; j < n; j++) {
            if (eclosure[i][j])
                printf("q%d ", j);
        }
        printf("}\n");
    }

    return 0;
}
