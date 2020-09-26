#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

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

void printMap(map arr[], int arr_size) {
    printf("\n---- INDEX DETAILS:\n");
    for (int i = 0; i < arr_size; i++) {
        printf("%-15s - #appearances: %d - #index: ", arr[i].key, arr[i].appearance);
        for (int j = 0; j < arr[i].appearance; j++) 
            printf("(%d, %d) ", arr[i].cordinates[j].col, arr[i].cordinates[j].line);
        printf("\n");
    }
}

int cmpf(const void *a, const void *b) {  //compare function for qsort
    map *mapA = (map *)a;
    map *mapB = (map *)b;
    return (strcasecmp(mapA->key, mapB->key));
}