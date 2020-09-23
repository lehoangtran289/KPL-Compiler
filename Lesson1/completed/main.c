#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dllist.h"
#include "jrb.h"

int addWord(JRB map, char *word, int line) {
    printf("%s %d\n", word, line);

    char t[100];
    strcpy(t, word);

    JRB node = jrb_find_str(map, t);

    char temp[10];
    sprintf(temp, "%d", line);

    if (node != NULL) {
        // char *val = jval_s(node->val);
        // strcat(val, " ,");
        // strcat(val, temp);
        // printf("%s -> val: %s\n", jval_s(node->key), val);
        // jrb_insert_str(map, word, new_jval_s(val));
    } else {
        printf("herer %s\n", t);
        jrb_insert_str(map, t, new_jval_s(temp));
    }
}

void printMap(JRB map) {
    printf("\n\n--- Index detail: \n");
    JRB node;

    jrb_traverse(node, map) {
        char *key = jval_s(node->key);
        char *val = jval_s(node->val);
        printf("%s\t%s\n", key, val);
    }
}

// -----------------------------------------------------------------

int contains(char **stopw, int n, char *str) {
    for (int i = 0; i < n; i++)
        if (strcmp(stopw[i], str) == 0)
            return 1;
    return 0;
}

int isNumber(char *str) {
    for (int i = 0; i < strlen(str); i++)
        if (!isdigit(str[i]) && str[i] != '.')
            return 0;
    return 1;
}

int isName(char *str, int flag) {
    if (!flag && str[0] >= 'A' && str[0] <= 'Z')
        return 1;
    return 0;
}

// -----------------------------------------------------------------

int readstopw(char **stopw, int *n) {
    FILE *fin = fopen("stopw2.txt", "r");
    if (fin == NULL) {
        printf("Cant open file to read\n");
        return -1;
    }

    char buf[1000];
    int id = 0;
    while (fgets(buf, 1000, fin) != NULL) {
        buf[strlen(buf) - 1] = '\0';

        stopw[id] = (char *)malloc(20 * sizeof(char));
        strcpy(stopw[id], buf);
        id++;
    }

    *n = id;
    fclose(fin);
}

int readfromfile(JRB map, char **stopw, int n) {
    FILE *fin2 = fopen("vanban2.txt", "r");
    if (fin2 == NULL) {
        printf("Cant open file to read\n");
        return -1;
    }

    char buf[10000];
    int flag = 1;  // check if it's the first word in the sentence -> not name
    int line = 1;

    while (fgets(buf, 10000, fin2) != NULL) {
        buf[strlen(buf) - 1] = '\0';
        // printf("%s\n", buf);

        char *token = strtok(buf, " ,()\r\n\t");
        while (token != NULL) {
            // printf("%-15scontains: %d - isNumber: %d - isName: %d, flag=%d\n", token, contains(stopw, n, token), isNumber(token), isName(token, flag), flag);

            char temp[100];
            strcpy(temp, token);

            if (temp[strlen(temp) - 1] == '.')
                temp[strlen(temp) - 1] = '\0';

            if (!contains(stopw, n, temp) && !isNumber(temp) && !isName(temp, flag)) {
                // printf("%s\n", temp);
                // TODO
                addWord(map, temp, line);
            }

            flag = token[strlen(token) - 1] == '.' ? 1 : 0;
            token = strtok(NULL, " ,()\r\n\t");
        }
        line++;
    }
    fclose(fin2);
}

// -----------------------------------------------------------------

int main() {
    JRB map = make_jrb();
    char **stopw = (char **)malloc(20 * sizeof(char *));

    int n;
    readstopw(stopw, &n);

    // printf("--- Stop words:\n");
    // for (int i = 0; i < n; i++) {
    //     printf("%d. %s\n", i, stopw[i]);
    // }
    // printf("\n\n");

    readfromfile(map, stopw, n);

    printMap(map);

    jrb_free_tree(map);
    return 0;
}
