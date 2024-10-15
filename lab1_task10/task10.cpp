#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum CODES {
    VALID,
    INVALID_BASE,
    TOO_LARGE,
    BASE_OUT_OF_RANGE,
    EMPTY_LINE,
    INCORRECT_NUMBER_SYSTEM,
    INVALID_NUMBER
};

void ValidateCode(const CODES code) {
    switch (code) {
        case INVALID_BASE: printf("Invalid inputted base\n"); break;
        case TOO_LARGE: printf("too large\n"); break;
        case BASE_OUT_OF_RANGE: printf("base out of range\n"); break;
        case EMPTY_LINE: printf("empty line\n"); break;
        case INCORRECT_NUMBER_SYSTEM: printf("incorrect number system\n"); break;
        case INVALID_NUMBER: printf("invalid number\n"); break;
        default: printf("Unknown error code\n"); break;
    }
}

CODES ValidateBase(const char* argv) {
    int cnt = 0;
    while (*argv != '\0') {
        if (!isdigit(*argv) and (!islower(*argv))) {
            return INVALID_BASE;
        } else {
            argv++;
            cnt++;
        }
    }
    if (cnt == 0) {
        return EMPTY_LINE;
    }
    return VALID;
}

CODES Validate(const char* argv, int base) {
    int cnt = 0;
    if (argv[0] == '-') {
        argv++;
    }
    while (*argv != '\0') {
        //printf("%c", *argv);
        if (!isdigit(*argv) and (!islower(*argv))) {
            return INVALID_NUMBER;
        } else if (isdigit(*argv)) {
            if (*argv - '0' >= base) {
                return INCORRECT_NUMBER_SYSTEM;
            }
            argv++;
            cnt++;
        } else if (isalpha(*argv)) {
            if ((*argv - 'a') >= (base - 10)) {
                return INCORRECT_NUMBER_SYSTEM;
            }
            argv++;
            cnt++;
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
    //printf("hui\n");
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
    int res = 0, negFlag = 0;
    char *p = str;

    if (*p == '-') {
        negFlag = 1;
        p++;
    }

    while (*p) {
        if (isalpha(*p)) {
            res = res * base + *p++ - 'a' + 10;
        }
        else {
            res = res * base + *p++ - '0';
        }
    }
    if (negFlag) {
        res = -res;
    }
    return res;
}

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

int main() {
    char baseArr[BUFSIZ];
    printf("Enter base:\n");
    gets(baseArr);
    CODES ret = ValidateBase(baseArr);
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

    printf("Base: %d\n", base);


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
        ret = Validate(buf, base);
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
    int maxNumber = 0;

    for (int j = 0; j < cnt; ++j) {
        printf("%d\n", FromBaseTo10(base, numArr[j]));
        int num = FromBaseTo10(base, numArr[j]);
        if (abs(num) > abs(maxNumber)) {
            maxNumber = num;
        }
    }
    printf("Max number: %d\n", maxNumber);
    printf("In 9th num. sys: %s\n", From10toBase(maxNumber, 9));
    printf("In 18th num. sys: %s\n", From10toBase(maxNumber, 18));
    printf("In 27th num. sys: %s\n", From10toBase(maxNumber, 27));
    printf("In 36th num. sys: %s\n", From10toBase(maxNumber, 36));

    //Очистка
    Clear(cnt, numArr, baseArr);
    return 0;
}