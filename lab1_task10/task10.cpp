#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum CODES {
    VALID,
    NOT_A_NUM,
    TOO_LARGE,
    BASE_OUT_OF_RANGE
};

void ValidateCode(CODES code) {
    switch (code) {
        case NOT_A_NUM: printf("not a num\n"); break;
        case TOO_LARGE: printf("too large\n"); break;
        case BASE_OUT_OF_RANGE: printf("base out of range\n"); break;
        default: printf("Unknown error code\n"); break;
    }
}

CODES Validate(const char* argv) {
    int cnt = 0;
    if (argv[0] == '-') {
        argv++;
    }
    while (*argv != '\0') {
        if (!isdigit(*argv) and (*argv != '.')) {
            return NOT_A_NUM;
        } else {
            argv++;
            cnt++;
        }
    }
    if (cnt > 9) {
        return TOO_LARGE;
    }
    return VALID;
}

CODES ValidateBase(int base) {
    if (base < 2 or base > 36) {
        return BASE_OUT_OF_RANGE;
    }
    return VALID;
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
    char buf[BUFSIZ];
    int i = 0, j, count = 2;

    numArr = (char**)realloc(numArr, count * sizeof(char*));
    if (numArr == NULL) {
        printf("Pizdec");
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
            for (j = 0; j < i; ++j) {
                free(numArr[j]);
            }
            free(numArr);
            return -1;
        }

        if (i == count) {
            count *= 2;
            ptr = (char**)realloc(numArr, count * sizeof(char*));
            if (ptr == NULL) {
                for (j = 0; j < i; ++j) {
                    free(numArr[j]);
                }
                free(numArr);
                printf("Error");
                return -1;
            }
            numArr = ptr;
        }
        numArr[++i - 1] = (char*)malloc((strlen(buf) + 1) * sizeof(char));
        //Обработка ошибки malloc - освободить всё ранее заданное и послать пользователя нахуй
        strcpy(numArr[i - 1], buf);
    }

    //печать строк
    for (j = 0; j < i; ++j) {
        printf("%s\n", numArr[j]);
    }

    //Очистка
    for (j = 0; j < i; ++j) {
        free(numArr[j]);
    }
    free(numArr);
}
