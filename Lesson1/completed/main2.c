#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

map arr[1000];
int arr_size = 0;

void readstopw(char **stopw, int *stopw_size) {
    FILE *fin = fopen("stopw.txt", "r");
    if (fin == NULL) {
        printf("Cant open file to read\n");
        return;
    }

    char buf[1000];
    int id = 0;
    while (fgets(buf, 1000, fin) != NULL) {
        buf[strlen(buf) - 1] = '\0';

        stopw[id] = (char *)malloc(20 * sizeof(char));
        strcpy(stopw[id], buf);
        id++;
    }

    *stopw_size = id;
    fclose(fin);
}

void readfromfile(char **stopw, int stopw_size) {
    FILE *fin2 = fopen("vanban.txt", "r");
    if (fin2 == NULL) {
        printf("Cant open file to read\n");
        return;
    }

    char buf[10000];
    int flag = 1;  // check if it's the first word in the sentence -> not name
    int line = 1;

    while (fgets(buf, 10000, fin2) != NULL) {
        buf[strlen(buf) - 1] = '\0';

        int column = 0;
        char *token = strtok(buf, " ,()\r\n\t");

        while (token != NULL) {
            // printf("%-15scontains: %d - isNumber: %d - isName: %d, flag=%d\n", token, contains(stopw, n, token), isNumber(token), isName(token, flag), flag);

            char temp[100];
            strcpy(temp, token);
            column++;

            if (temp[strlen(temp) - 1] == '.')
                temp[strlen(temp) - 1] = '\0';

            if (!contains(stopw, stopw_size, temp) && !isNumber(temp) && !isName(temp, flag)) {
                // printf("%s\n", temp);

                for (int i = 0; i < strlen(temp); i++)
                    temp[i] = tolower(temp[i]);

                int isFind = 0;
                for (int i = 0; i < arr_size; i++) {
                    if (strcmp(arr[i].key, temp) == 0) {
                        int apperance = arr[i].appearance++;
                        arr[i].cordinates[apperance].col = column;
                        arr[i].cordinates[apperance].line = line;
                        isFind = 1;
                        break;
                    }
                }
                if (isFind == 0) {
                    strcpy(arr[arr_size].key, temp);
                    arr[arr_size].appearance = 1;
                    arr[arr_size].cordinates[0].col = column;
                    arr[arr_size].cordinates[0].line = line;

                    arr_size++;
                }
            }
            flag = token[strlen(token) - 1] == '.' ? 1 : 0;
            token = strtok(NULL, " ,()\r\n");
        }
        line++;
    }
    fclose(fin2);
}

// ---------------------------------------------------------

int main() {
    char **stopw = (char **)malloc(200 * sizeof(char *));
    int stopw_size;  // stopw arr length

    readstopw(stopw, &stopw_size);  // read stopw

    printf("--- STOP WORDS:\n");
    for (int i = 0; i < stopw_size; i++)
        printf("%d. %s\n", i, stopw[i]);

    readfromfile(stopw, stopw_size);  // read and filter words

    qsort(arr, arr_size, sizeof(map), cmpf);  // sort array alphabetically by key
    printMap(arr, arr_size);

    return 0;
}
