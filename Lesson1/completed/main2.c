#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

map arr[1000];
int idx = 0;

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

int readfromfile(char **stopw, int n) {
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

        char *token = strtok(buf, " ,()\r\n\t");
        while (token != NULL) {
            // printf("%-15scontains: %d - isNumber: %d - isName: %d, flag=%d\n", token, contains(stopw, n, token), isNumber(token), isName(token, flag), flag);

            char temp[100];
            strcpy(temp, token);

            if (temp[strlen(temp) - 1] == '.')
                temp[strlen(temp) - 1] = '\0';
            
            if (!contains(stopw, n, temp) && !isNumber(temp) && !isName(temp, flag)) {
                // printf("%s\n", temp);
                char linestr[10];
                sprintf(linestr, "%d", line);  //int -> string

                int isFind = 0;
                for (int i = 0; i < idx; i++) {
                    if (strcmp(arr[i].key, temp) == 0) {
                        arr[i].appearance++;
                        if (strstr(arr[i].lineIndex, linestr) == NULL) {
                            strcat(arr[i].lineIndex, ", ");
                            strcat(arr[i].lineIndex, linestr);
                        }
                        isFind = 1;
                        break;
                    }
                }
                if (isFind == 0) {
                    strcpy(arr[idx].key, temp);
                    arr[idx].appearance = 1;
                    strcat(arr[idx].lineIndex, linestr);
                    idx++;
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
    char **stopw = (char **)malloc(20 * sizeof(char *));
    int n;                 // stopw arr length
    
    readstopw(stopw, &n);  // read stopw
    
    printf("--- STOP WORDS:\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s\n", i, stopw[i]);
    }

    readfromfile(stopw, n);                 // read and filter words

    printf("\n---- INDEX DETAILS:\n");

    qsort(arr, idx, sizeof(map), cmpf);     // sort array alphabetically by key 
    for (int i = 0; i < idx; i++) {
        printf("%-15s - #appearances: %d - #line: %s\n", arr[i].key, arr[i].appearance, arr[i].lineIndex);
    }

    return 0;
}
