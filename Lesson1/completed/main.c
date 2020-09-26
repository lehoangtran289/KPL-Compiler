#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dllist.h"
#include "jrb.h"

int addWord(JRB map, char *word, int line, int column) {
    JRB node = jrb_find_str(map, word);
    JRB indexTree = make_jrb();
    JRB columnTree = make_jrb();

    if (node != NULL) {
        indexTree = (JRB)jval_v(node->val);
        jrb_insert_int(indexTree, line, new_jval_v(columnTree));
        jrb_insert_int(columnTree, column, new_jval_v(NULL));
    } else {
        jrb_insert_str(map, word, new_jval_v(indexTree));
        jrb_insert_int(indexTree, line, new_jval_v(columnTree));
        jrb_insert_int(columnTree, column, new_jval_v(NULL));
    }
}

void printMap(JRB map) {
    printf("\n\n--- Index detail: \n");
    JRB node, node2, node3;

    jrb_traverse(node, map) {
        printf("%s - ", jval_s(node->key));
        JRB tree = jval_v(node->val);
        jrb_traverse(node2, tree) {
            JRB tree2 = jval_v(node2->val);
            jrb_traverse(node3, tree2) {
                printf("(%d, %d) ", node2->key, node3->key);
            }
        }
        printf("\n");
    }
}

// -----------------------------------------------------------------

int contains(char **stopw, int size, char *str) {
    for (int i = 0; i < size; i++)
        if (strcasecmp(stopw[i], str) == 0)
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
    FILE *fin = fopen("stopw.txt", "r");
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
    FILE *fin2 = fopen("vanban.txt", "r");
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

        int column = 0;
        char *token = strtok(buf, " ,()\r\n\t");
        while (token != NULL) {     
            // printf("%-15scontains: %d - isNumber: %d - isName: %d, flag=%d\n", token, contains(stopw, n, token), isNumber(token), isName(token, flag), flag);

            char *wordToAdd = (char *)malloc(sizeof(char) * 200);
            *wordToAdd = '\0';
            strcat(wordToAdd, token);

            column++;

            if (wordToAdd[strlen(wordToAdd) - 1] == '.')
                wordToAdd[strlen(wordToAdd) - 1] = '\0';

            if (!contains(stopw, n, wordToAdd) && !isNumber(wordToAdd) && !isName(wordToAdd, flag)) {
                for (int i = 0; i < strlen(wordToAdd); i++)
                    wordToAdd[i] = tolower(wordToAdd[i]);

                addWord(map, wordToAdd, line, column);
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

    printf("--- Stop words:\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s\n", i, stopw[i]);
    }

    readfromfile(map, stopw, n);

    printMap(map);

    jrb_free_tree(map);
    return 0;
}
