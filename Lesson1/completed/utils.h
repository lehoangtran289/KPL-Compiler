typedef struct dt {
    char key[30];
    int appearance;
    char lineIndex[30];
} map;

int contains(char **stopw, int n, char *str);
int isNumber(char *str);
int isName(char *str, int flag);
int cmpf(const void *a, const void *b);