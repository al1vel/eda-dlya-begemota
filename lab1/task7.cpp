#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum CODES {
    VALID,
    NO_FORMAT,
    INVALID_PATH,
    SUCCESS
};

char * From10toBase(int num, int base) {
    char buf[BUFSIZ], *p = buf + BUFSIZ - 1;
    int r;
    *p-- = 0;

    if (num < 0) {
        num = -num;
        *p = '-';
    }

    while (num) {
        *p-- = ((r = num % base) > 9) ? r - 10 + 'a' : r + '0';
        num /= base;
    }
    return p + 1;
}

void ValidateCode(const CODES code) {
    switch (code) {
        case NO_FORMAT: printf("Path has invalid format\n"); break;
        case INVALID_PATH: printf("Path is invalid\n"); break;
        default: printf("Invalid code\n"); break;
    }
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

void ClearR(FILE* in1, FILE* in2, FILE* out, char* PathIn1, char* PathIn2, char* PathOut) {
    fclose(in1);
    fclose(in2);
    fclose(out);
    free(PathIn1);
    free(PathIn2);
    free(PathOut);
}

void ClearA(FILE* in1, FILE* out, char* PathIn1, char* PathOut) {
    fclose(in1);
    fclose(out);
    free(PathIn1);
    free(PathOut);
}

void funcForR (FILE* in1, FILE* in2, FILE* out) {
    int flag = 1, stop1 = 1, stop2 = 1;
    char c;

    while (stop1 or stop2) {
        if (flag == 1) {
            int editFlag = 0;
            while (isalnum(c = fgetc(in1))) {
                fputc(c, out);
                editFlag = 1;
            }
            flag = 2;
            if (c == EOF) {
                stop1 = 0;
            }
            if (editFlag) {
                fputc(' ', out);
            }
        } else if (flag == 2) {
            int editFlag = 0;
            while (isalnum(c = fgetc(in2))) {
                fputc(c, out);
                editFlag = 1;
            }
            flag = 1;
            if (c == EOF) {
                stop2 = 0;
            }
            if (editFlag) {
                fputc(' ', out);
            }
        }
    }
}

void funcForA(FILE* in1, FILE* out) {
    int curLex = 1, fl = 0;

    while (1) {
        char c = fgetc(in1);

        if (c == EOF) {
            break;
        }
        if (c == '\n' or c == ' ' or c == '\t') {
            if (fl == 0) {
                curLex++;
                fl = 1;
                fputc(' ', out);
            }
            continue;
        }
        fl = 0;

        if (curLex % 10 == 0) {
            if (isalpha(c)) {
                if (islower(c)) {
                    fprintf(out, "%s", From10toBase(c, 4));
                } else {
                    c = c + 32;
                    fprintf(out, "%s", From10toBase(c, 4));
                }
            } else {
                fputc(c, out);
            }

        } else if (curLex % 2 == 0 and curLex % 10 != 0) {
            if (isalpha(c)) {
                if (islower(c)) {
                    fputc(c, out);
                } else {
                    c = c + 32;
                    fputc(c, out);
                }
            } else {
                fputc(c, out);
            }

        } else if (curLex % 5 == 0 and curLex % 10 != 0) {
            //printf("%d %s\n", c, From10toBase(curLex, 8));
            fprintf(out, "%s", From10toBase(c, 8));

        } else {
            fputc(c, out);
        }
    }
}

int main(const int argc, char *argv[]) {
    if (argc == 1) {
        printf("Wrong number of arguments\n");
        return -1;
    }
    int flag;
    if (!strcmp(argv[1], "-r") || !strcmp(argv[1], "/r")) {
        flag = 0;
    } else if (!strcmp(argv[1], "-a") || !strcmp(argv[1], "/a")) {
        flag = 1;
    } else {
        printf("Flag <%s> is not supported", argv[1]);
        return -1;
    }
    //printf("Flag: %d\n", flag);

    if (flag == 0) {
        if (argc < 6) {
            printf("Wrong number of arguments\n");
            return -1;
        }
        CODES ret_code = ValidatePath(argv[3]);
        //printf("%s %s %s\n", argv[3], argv[4], argv[argc - 1]);
        if (ret_code != VALID) {
            ValidateCode(ret_code);
            return -1;
        }
        ret_code = ValidatePath(argv[4]);
        if (ret_code != VALID) {
            ValidateCode(ret_code);
            return -1;
        }
        ret_code = ValidatePath(argv[argc - 1]);
        if (ret_code != VALID) {
            ValidateCode(ret_code);
            return -1;
        }
        printf("Paths are valid\n");

        char* inFile1 = (char*)malloc(strlen(argv[3]) + 1);
        if (inFile1 == NULL) {
            printf("Error allocating memory for input file1\n");
            return -1;
        }
        strcpy(inFile1, argv[3]);

        char* outFile = (char*)malloc(strlen(argv[argc - 1]) + 1);
        if (outFile == NULL) {
            printf("Error allocating memory for output file\n");
            free(inFile1);
            return -1;
        }
        strcpy(outFile, argv[argc - 1]);

        char* inFile2 = (char*)malloc(strlen(argv[4]) + 1);
        if (inFile2 == NULL) {
            printf("Error allocating memory for input file2\n");
            free(inFile1);
            free(outFile);
            return -1;
        }
        strcpy(inFile2, argv[4]);

        if (!strcmp(inFile1, outFile) or !strcmp(inFile2, outFile)) {
            printf("Input and output files must be different\n");
            return -1;
        }

        printf("Input 1: %s\nInput 2: %s\nOutput: %s\n", inFile1, inFile2, outFile);

        FILE* in1 = fopen(inFile1, "r");
        FILE* in2 = fopen(inFile2, "r");
        FILE* out = fopen(outFile, "w");
        if (in1 == NULL || in2 == NULL || out == NULL) {
            printf("Error opening files\n");
            ClearR(in1, in2, out, inFile1, inFile2, outFile);
            return -1;
        }

        funcForR(in1, in2, out);
        ClearR(in1, in2, out, inFile1, inFile2, outFile);

    } else {
        if (argc < 5) {
            printf("Wrong number of arguments\n");
            return -1;
        }
        CODES ret_code = ValidatePath(argv[3]);
        //printf("%s %s\n", argv[3], argv[argc - 1]);
        if (ret_code != VALID) {
            ValidateCode(ret_code);
            return -1;
        }
        ret_code = ValidatePath(argv[argc - 1]);
        if (ret_code != VALID) {
            ValidateCode(ret_code);
            return -1;
        }
        char* inFile1 = (char*)malloc(strlen(argv[3]) + 1);
        if (inFile1 == NULL) {
            printf("Error allocating memory for input file1\n");
            return -1;
        }
        strcpy(inFile1, argv[3]);

        char* outFile = (char*)malloc(strlen(argv[argc - 1]) + 1);
        if (outFile == NULL) {
            printf("Error allocating memory for output file\n");
            free(inFile1);
            return -1;
        }
        strcpy(outFile, argv[argc - 1]);

        printf("Input 1: %s\nOutput: %s\n", inFile1, outFile);

        FILE* in1 = fopen(inFile1, "r");
        FILE* out = fopen(outFile, "w");
        if (in1 == NULL || out == NULL) {
            printf("Error opening files\n");
            ClearA(in1, out, inFile1, outFile);
            return -1;
        }

        funcForA(in1, out);
        ClearA(in1, out, inFile1, outFile);
    }
    return 0;
}