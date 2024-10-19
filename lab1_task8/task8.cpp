#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum CODES {
    VALID,
    NO_FORMAT,
    INVALID_PATH,
    SUCCESS,
    INVALID_NUMBER,
    NUMBER_SYSTEM_OUT_OF_RANGE,
    MALLOC_FAILED
};

void ValidateCode(const CODES code) {
    switch (code) {
        case NO_FORMAT: printf("Path has invalid format\n"); break;
        case INVALID_PATH: printf("Path is invalid\n"); break;
        case NUMBER_SYSTEM_OUT_OF_RANGE: printf("Number system is out of range\n"); break;
        case INVALID_NUMBER: printf("Number is invalid\n"); break;
        default: printf("Invalid code\n"); break;
    }
}

int FromBaseTo10 (const int base, char *str) {
    int res = 0, negFlag = 0;
    char *p = str;

    if (*p == '-') {
        negFlag = 1;
        p++;
    }

    while (*p != '\0') {
        //printf("%c ", *p);
        if (isalpha(*p)) {
            res = res * base + *p++ - 'A' + 10;
        }
        else {
            res = res * base + *p++ - '0';
        }
    }
    if (negFlag) {
        printf("minus");
        res = -res;
    }
    return res;
}

CODES ValidatePath(char* path) {
    int l = strlen(path);
    char* ptr = path + l - 1;
    while (*ptr != '.' and l > 0) {
        --ptr;
        l--;
    }
    ++ptr;
    if(strcmp(ptr, "txt") != 0) {
        return NO_FORMAT;
    }
    while (isalpha(*ptr) || isdigit(*ptr) || *ptr == '-'
    || *ptr == '_' || *ptr == '.' || *ptr == '\\') {
        --ptr;
    }
    if (*ptr != ':' || !isupper(*(ptr - 1)) || *(ptr + 1) != '\\') {
        return INVALID_PATH;
    }
    return VALID;
}

CODES solve(FILE* in, FILE* out) {
    int base = 0, i = 0, numFound = 0, notNull = 0;

    char *buff = (char *) malloc(BUFSIZ);
    if (buff == NULL) {
        fclose(in);
        fclose(out);
        return MALLOC_FAILED;
    }

    while (1) {
        //printf("fl: %d\n", fl);
        const char c = fgetc(in);
        //printf("<%c> nf: %d\n", c, numFound);

        if (c == EOF) {
            //printf("test");
            break;
        }
        if (!isalnum(c) or islower(c)) {
            if (c != '\n' and c != ' ' and c != '\t') {
                //printf("c: %d ", c);
                free(buff);
                fclose(in);
                fclose(out);
                return INVALID_NUMBER;
            }
        }

        if ((c == '\n' or c == ' ' or c == '\t')) {
            if (numFound) {
                if (base >= 50 and base <= 57) {
                    base = base - 47;
                } else if (base >= 65 and base <= 90) {
                    base = base - 54;
                } else {
                    printf("<%c> ", c);
                    free(buff);
                    fclose(in);
                    fclose(out);
                    return NUMBER_SYSTEM_OUT_OF_RANGE;
                }

                buff[i] = '\0';

                for (int k = 0; k < i; ++k) {
                    fputc(buff[k], out);
                }
                fputc(' ', out);
                fprintf(out, "%d ", base);

                int number = FromBaseTo10(base, buff);
                fprintf(out, "%d\n", number);

                i = 0;
                numFound = 0;
            }
        } else {
            if (!(numFound == 0 and c == '0')) {
                buff[i] = c;
                i++;
                if (c > base) {
                    base = c;
                }
                numFound = 1;
            }
        }
    }
    free(buff);
    return SUCCESS;
}

int main(const int argc, char *argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    CODES ret_code = ValidatePath(argv[2]);
    if (ret_code != VALID) {
        ValidateCode(ret_code);
        return -1;
    }
    ret_code = ValidatePath(argv[3]);
    if (ret_code != VALID) {
        ValidateCode(ret_code);
        return -1;
    }
    printf("Paths are valid\n");

    char* inFile = (char*)malloc(strlen(argv[2]) + 1);
    if (inFile == NULL) {
        printf("Error allocating memory for input file\n");
        return -1;
    }
    strcpy(inFile, argv[2]);

    char* outFile = (char*)malloc(strlen(argv[2]) + 1);
    if (outFile == NULL) {
        printf("Error allocating memory for input file\n");
        free(inFile);
        return -1;
    }
    strcpy(outFile, argv[3]);

    FILE* in = fopen(inFile, "r");
    FILE* out = fopen(outFile, "w");
    if (in == NULL || out == NULL) {
        printf("Error opening file\n");
        free(inFile);
        free(outFile);
        return -1;
    }

    ret_code = solve(in, out);
    if (ret_code != SUCCESS) {
        ValidateCode(ret_code);
        free(inFile);
        free(outFile);
        return -1;
    }

    fclose(in);
    fclose(out);
    free(inFile);
    free(outFile);
}