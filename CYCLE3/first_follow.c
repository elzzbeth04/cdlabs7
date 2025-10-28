/* firstfollow.c - safer, instrumented FIRST/FOLLOW calculator */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PRODS 50
#define MAX_LEN 100
#define MAX_SET 100

int n_prods;
char prods[MAX_PRODS][MAX_LEN];
char firsts[26][MAX_SET];
char follows[26][MAX_SET];
int is_first_done[26];
int is_follow_done[26];

/* recursion-in-progress flags to avoid infinite recursion in cycles */
int first_in_progress[26];
int follow_in_progress[26];

int is_terminal(char c) {
    return (c < 'A' || c > 'Z');
}

int in_set(const char set[], char c) {
    if (c == '\0') return 0;
    for (int i = 0; set[i] != '\0'; ++i)
        if (set[i] == c) return 1;
    return 0;
}

void add_to_set_safe(char set[], char c) {
    if (c == '\0') return;
    if (in_set(set, c)) return;
    int len = strlen(set);
    if (len + 2 >= MAX_SET) {
        fprintf(stderr, "ERROR: set overflow when adding '%c'\n", c);
        return;
    }
    set[len] = c;
    set[len+1] = '\0';
}

void first(char nonterm) {
    if (nonterm < 'A' || nonterm > 'Z') return;
    int idx = nonterm - 'A';
    if (is_first_done[idx]) return;
    if (first_in_progress[idx]) {
        /* already in progress (cycle) — avoid infinite recursion */
        return;
    }
    first_in_progress[idx] = 1;

    char result[MAX_SET];
    result[0] = '\0';

    for (int p = 0; p < n_prods; p++) {
        if (prods[p][0] != nonterm) continue;

        /* safety: ensure production format like A=... */
        if (prods[p][1] != '=') continue;

        int i = 2;
        int nullable = 1;

        while (prods[p][i] != '\0' && nullable) {
            nullable = 0;
            char sym = prods[p][i];

            if (is_terminal(sym)) {
                add_to_set_safe(result, sym);
                break;
            } else {
                /* nonterminal */
                if (sym < 'A' || sym > 'Z') {
                    /* malformed symbol, treat as terminal */
                    add_to_set_safe(result, sym);
                    break;
                }
                /* recursive compute FIRST(sym) if needed */
                first(sym);

                /* copy FIRST(sym) \ {e} */
                for (int j = 0; firsts[sym - 'A'][j] != '\0'; j++) {
                    char f = firsts[sym - 'A'][j];
                    if (f == 'e') {
                        nullable = 1;
                    } else {
                        add_to_set_safe(result, f);
                    }
                }
                if (!nullable) break;
            }
            i++;
        }
        if (nullable) add_to_set_safe(result, 'e');
    }

    strcpy(firsts[idx], result);
    is_first_done[idx] = 1;
    first_in_progress[idx] = 0;
}

void follow(char nonterm) {
    if (nonterm < 'A' || nonterm > 'Z') return;
    int idx = nonterm - 'A';
    if (is_follow_done[idx]) return;
    if (follow_in_progress[idx]) {
        /* already in progress (cycle) — avoid infinite recursion */
        return;
    }
    follow_in_progress[idx] = 1;

    char result[MAX_SET];
    result[0] = '\0';

    /* start symbol gets $ */
    if (n_prods > 0 && prods[0][0] == nonterm) {
        add_to_set_safe(result, '$');
    }

    for (int p = 0; p < n_prods; p++) {
        int len = strlen(prods[p]);
        for (int i = 2; i < len; i++) {
            if (prods[p][i] != nonterm) continue;

            /* if there is symbol after it */
            if (i + 1 < len) {
                char next = prods[p][i+1];
                if (is_terminal(next)) {
                    add_to_set_safe(result, next);
                } else {
                    /* add FIRST(next) \ {e} */
                    for (int j = 0; firsts[next - 'A'][j] != '\0'; j++) {
                        char f = firsts[next - 'A'][j];
                        if (f != 'e') add_to_set_safe(result, f);
                    }
                    /* if FIRST(next) contains e, add follow(lhs) */
                    if (in_set(firsts[next - 'A'], 'e')) {
                        if (prods[p][0] != nonterm) {
                            follow(prods[p][0]);
                            for (int j = 0; follows[prods[p][0] - 'A'][j] != '\0'; j++)
                                add_to_set_safe(result, follows[prods[p][0] - 'A'][j]);
                        }
                    }
                }
            } else {
                /* last symbol: add follow of LHS */
                if (prods[p][0] != nonterm) {
                    follow(prods[p][0]);
                    for (int j = 0; follows[prods[p][0] - 'A'][j] != '\0'; j++)
                        add_to_set_safe(result, follows[prods[p][0] - 'A'][j]);
                }
            }
        }
    }

    strcpy(follows[idx], result);
    is_follow_done[idx] = 1;
    follow_in_progress[idx] = 0;
}

int main(void) {
    printf("Enter number of productions (<= %d): ", MAX_PRODS);
    if (scanf("%d", &n_prods) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (n_prods <= 0 || n_prods > MAX_PRODS) {
        fprintf(stderr, "n_prods out of range\n");
        return 1;
    }

    printf("Enter productions (format A=XYZ, use e for epsilon):\n");
    for (int i = 0; i < n_prods; i++) {
        if (scanf("%s", prods[i]) != 1) {
            fprintf(stderr, "Failed to read production %d\n", i);
            return 1;
        }
        if ((int)strlen(prods[i]) >= MAX_LEN) {
            fprintf(stderr, "Production too long at index %d\n", i);
            return 1;
        }
    }

    memset(firsts, 0, sizeof(firsts));
    memset(follows, 0, sizeof(follows));
    memset(is_first_done, 0, sizeof(is_first_done));
    memset(is_follow_done, 0, sizeof(is_follow_done));
    memset(first_in_progress, 0, sizeof(first_in_progress));
    memset(follow_in_progress, 0, sizeof(follow_in_progress));

    /* compute FIRST for lhs of productions */
    for (int i = 0; i < n_prods; i++) {
        char lhs = prods[i][0];
        if (lhs >= 'A' && lhs <= 'Z')
            first(lhs);
    }

    /* compute FOLLOW for lhs of productions */
    for (int i = 0; i < n_prods; i++) {
        char lhs = prods[i][0];
        if (lhs >= 'A' && lhs <= 'Z')
            follow(lhs);
    }

    printf("\nFIRST sets:\n");
    for (int i = 0; i < 26; i++) {
        if (is_first_done[i]) {
            printf("%c : { ", 'A' + i);
            for (int j = 0; firsts[i][j] != '\0'; j++) {
                printf("%c ", firsts[i][j]);
            }
            printf("}\n");
        }
    }

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < 26; i++) {
        if (is_follow_done[i]) {
            printf("%c : { ", 'A' + i);
            for (int j = 0; follows[i][j] != '\0'; j++) {
                printf("%c ", follows[i][j]);
            }
            printf("}\n");
        }
    }

    return 0;
}

