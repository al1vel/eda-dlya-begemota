#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("File must be inputted.\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "rw");
    if (file == NULL) {
        printf("Error opening file <%s>.\n", argv[1]);
        return -1;
    }

    char* HashTable[128];

    int cont = 1, debugCnt = 0;
    char line[BUFSIZ];
    while (cont) {
        if (fgets(line, sizeof(line), file) == NULL) {
            cont = 0;
        }
        debugCnt++;
        int len = (int)strlen(line);
        if (len > 11) {
            char word[8];
            for (int i = 0; i < 7; ++i) {
                word[i] = line[i];
            }
            word[7] = '\0';
            if (strcmp(word, "#define") == 0) {
                //printf("hui: %d\n", debugCnt);
                char *directive = (char *)malloc(len);
                char *value = (char *)malloc(len);
                int i = 8, ind = 0;
                while (line[i] != ' ') {
                    //printf("<%c> ", line[i]);
                    directive[ind] = line[i];
                    i++;
                    ind++;
                }
                directive[ind] = '\0';
                ind = 0;
                while (line[i] != '\n') {
                    value[ind] = line[i];
                    i++;
                    ind++;
                }
                value[ind] = '\0';
                printf("Dir: <%s>. Val: <%s>.\n", directive, value);
            }
        } else if (line[0] == '\n') {
            continue;
        } else {
            cont = 0;
        }
    }



    return 0;
}