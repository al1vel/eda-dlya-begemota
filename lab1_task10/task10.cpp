#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum CODES {
    VALID,
    INVALID_INPUT,
    TOO_LARGE,
    BASE_OUT_OF_RANGE,
    EMPTY_LINE
};

void ValidateCode(const CODES code) {
    switch (code) {
        case INVALID_INPUT: printf("Invalid inputted number\n"); break;
        case TOO_LARGE: printf("too large\n"); break;
        case BASE_OUT_OF_RANGE: printf("base out of range\n"); break;
        case EMPTY_LINE: printf("empty line\n"); break;
        default: printf("Unknown error code\n"); break;
    }
}

CODES Validate(const char* argv) {
    int cnt = 0;
    if (argv[0] == '-') {
        argv++;
    }
    while (*argv != '\0') {
        if (!isdigit(*argv) and (!islower(*argv))) {
            return INVALID_INPUT;
        } else {
            argv++;
            cnt++;
        }
    }
    if (cnt > 9) {
        return TOO_LARGE;
    }
    if (cnt == 0) {
        return EMPTY_LINE;
    }
    return VALID;
}

CODES ValidateBase(const int base) {
    if (base < 2 or base > 36) {
        return BASE_OUT_OF_RANGE;
    }
    return VALID;
}

void Clear(const int i, char ** arr, const char *base) {
    for (int j = 0; j < i; ++j) {
        free(arr[j]);
    }
    free(arr);
    //printf("ok");
    delete base;
}

int FromBaseTo10 (const int base, char *str) {
    int res = 0;
    char *p = str;
    while (*p) {
        if (isalpha(*p)) {
            res = res * base + *p++ - 'A' + 10;
        }
        else {
            res = res * base + *p++ - '0';
        }
    }
    return res;
}

int main() {
    char baseArr[BUFSIZ];
    printf("Enter base:\n");
    gets(baseArr);
    CODES ret = Validate(baseArr);
    if (ret != VALID) {
        ValidateCode(ret);
        return -1;
    }
    const int base = atoi(baseArr);
    ret = ValidateBase(base);
    if (ret != VALID) {
        ValidateCode(ret);
        return -1;
    }


    char ** numArr = NULL, **ptr;
    char buf[BUFSIZ] = { };
    int cnt = 0, arrSize = 2;

    numArr = (char**)malloc(arrSize * sizeof(char*));
    if (numArr == NULL) {
        printf("Unable to allocate memory for numbers");
        return -1;
    }

    printf("Input numbers:\n");
    while(1) {
        gets(buf);
        if (!strcmp(buf, "stop")) {
            break;
        }
        ret = Validate(buf);
        if (ret != VALID) {
            ValidateCode(ret);
            Clear(cnt, numArr, baseArr);
            return -1;
        }

        if (cnt == arrSize) {
            arrSize *= 2;
            ptr = (char**)realloc(numArr, arrSize * sizeof(char*));
            if (ptr == NULL) {
                printf("Error reallocating memory");
                Clear(cnt, numArr, baseArr);
                return -1;
            }
            numArr = ptr;
        }
        numArr[++cnt - 1] = (char*)malloc((strlen(buf) + 1) * sizeof(char));
        if (numArr[cnt - 1] == NULL) {
            printf("Error allocating memory");
            Clear(cnt, numArr, baseArr);
            return -1;
        }
        strcpy(numArr[cnt - 1], buf);
    }

    //Обработка строк
    for (int j = 0; j < cnt; ++j) {
        printf("%d\n", FromBaseTo10(base, numArr[j]));
    }

    //Очистка
    Clear(cnt, numArr, baseArr);
    return 0;
}