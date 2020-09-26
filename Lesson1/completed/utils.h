typedef struct cordinate {
    int col; 
    int line;
} cordinate;

typedef struct dt {
    char key[50];
    int appearance;
    cordinate cordinates[100];
} map;

int contains(char **stopw, int n, char *str);
int isNumber(char *str);
int isName(char *str, int flag);
void printMap(map arr[], int arr_size);
int cmpf(const void *a, const void *b);