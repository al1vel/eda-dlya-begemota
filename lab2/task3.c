#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int strLength(const char *str) {
    int res = 0;
    while (*str != '\0') {
        res++;
        str++;
    }
    return res;
}

int my_strstr(char *str, char *pattern) {
    int pos = 1;
    while (*str) {
        char *s = str;
        char *p = pattern;

        while (*p && (*s == *p)) {
            s++;
            p++;
        }
        if (!*p) {
            return pos;
        }
        str++;
        pos++;
    }
    return -1;
}

void func(char *substr, int cnt, ...) {
    va_list args;
    va_start(args, cnt);

    for (int i = 0; i < cnt; i++) {
        char *str = va_arg(args, char *);

        FILE* file = fopen(str, "r");
        if (file == NULL) {
            printf("Error opening file <%s>.\n", str);
            return;
        }
        printf("\nFile <%s>\n", str);

        char c = fgetc(file);
        int lineNum = 0;
        while (c != EOF) {
            int size = 2, ind = 0, kolvo = 0;
            char* line = (char*)malloc((size + 1)*sizeof(char));
            if (line == NULL) {
                printf("Error allocating memory.\n");
                return;
            }
            if (c == '\n') {
                c = fgetc(file);
            }
            while (c != '\n' && c != EOF) {
                if (kolvo == (size + 1)) {
                    size *= 2;
                    char* ptr = (char*)realloc(line, (size + 1)*sizeof(char));
                    if (ptr == NULL) {
                        printf("Error reallocating memory.\n");
                        free(line);
                        return;
                    }
                    line = ptr;
                }
                line[ind] = c;
                c = fgetc(file);
                ind++;
                kolvo++;
            }
            lineNum++;
            line[ind] = '\0';
            char *p = line;
            int dobavka = 0;
            while (my_strstr(p, substr) != -1) {
                int cmp = my_strstr(p, substr);
                //printf("%s - %s\n", p, substr);
                if (cmp != -1) {
                    printf("Line %d from %d symbol.\n", lineNum, cmp + dobavka);
                }
                p += cmp;
                dobavka += cmp;
            }
            free(line);
        }
        fclose(file);
    }
    va_end(args);
}


int main() {
    func("hui", 1, "D:/aboba.txt", "D:/hui.txt", "D:/hui2.txt");
}