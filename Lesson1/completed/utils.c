#include <string.h>
#include <ctype.h>
#include "utils.h"

int contains(char **stopw, int n, char *str) {
    for (int i = 0; i < n; i++)
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

int cmpf(const void *a, const void *b) {  //compare function for qsort
    map *mapA = (map *)a;
    map *mapB = (map *)b;
    return (strcasecmp(mapA->key, mapB->key));
}