#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

enum {
    SUCCESS,
    IS_CONVEX,
    IS_NOT_CONVEX,
    WRONG_ARGS,
    MALLOC_FAILED,
    INVALID_BASE,
    VALID,
    NOT_A_NUM,
    INCORRECT_NUMBER_SYSTEM,
    NUM_TOO_LARGE,
    EMPTY_LINE,
    IS_KAPREKAR,
    IS_NOT_KAPREKAR
};

void ValidateCode(int code) {
    switch (code) {
        case SUCCESS: printf("SUCCESS\n"); break;
        case IS_CONVEX: printf("IS_CONVEX\n"); break;
        case IS_NOT_CONVEX: printf("IS_NOT_CONVEX\n"); break;
        case WRONG_ARGS: printf("WRONG_ARGS\n"); break;
        case MALLOC_FAILED: printf("MALLOC_FAILED\n"); break;
        case INVALID_BASE: printf("INVALID_BASE\n"); break;
        case VALID: printf("VALID\n"); break;
        case INCORRECT_NUMBER_SYSTEM: printf("INCORRECT_NUMBER_SYSTEM\n"); break;
        case NUM_TOO_LARGE: printf("NUM_TOO_LARGE\n"); break;
        case EMPTY_LINE: printf("EMPTY_LINE\n"); break;
        default: printf("UNKNOWN ERROR\n"); break;
    }
}

struct Point {
    double x;
    double y;
};

double CalculateOpr(struct Point p1, struct Point p2, struct Point p3) {
    double Ux = p2.x - p1.x;
    double Uy = p2.y - p1.y;
    double Vx = p3.x - p2.x;
    double Vy = p3.y - p2.y;
    return (Ux * Vy) - (Uy * Vx);
}

int IsConvex(int count, ...) {
    if (count < 6 || count % 2 != 0) {
        return WRONG_ARGS;
    }
    va_list arg;
    va_start(arg, count);

    struct Point * array = (struct Point*)malloc(sizeof(struct Point) * count);
    if (array == NULL) {
        va_end(arg);
        return MALLOC_FAILED;
    }

    for (int i = 0; i < count; i++) {
        double x = va_arg(arg, double);
        double y = va_arg(arg, double);
        array[i].x = x;
        array[i].y = y;
    }

    int n = count / 2;

    for (int i = 0; i < n; i++) {
        printf("X: %f | Y: %f\n", array[i].x, array[i].y);
    }

    double sign = 0;
    for (int i = 0; i < n; i++) {
        double opr = CalculateOpr(array[i], array[(i + 1) % n], array[(i + 2) % n]);

        if (opr != 0) {
            if (sign == 0) {
                sign = (opr > 0) ? 1 : -1;
            } else if ( (opr > 0 && sign < 0) || (opr < 0 && sign > 0) ) {
                free(array);
                return IS_NOT_CONVEX;
            }
        }
    }
    va_end(arg);
    free(array);
    return IS_CONVEX;
}

double fastPow(double number, int power) {
    if (power < 2) {
        if (power) {
            return number;
        }
        return 1;
    }
    double res = fastPow(number, (power/2));
    res = res * res;
    if (power % 2 == 1) {
        res = res * number;
    }
    return res;
}

double PolynomeValue(double x, int power, ...) {
    va_list arg;
    va_start(arg, power);

    double res = 0;
    for (int i = power; i >= 0; i--) {
        int koef = va_arg(arg, int);
        double slag = fastPow(x, i) * koef;
        res += slag;
    }
    va_end(arg);
    return res;
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

int Validate(const char* argv, int base) {
    int cnt = 0;
    while (*argv != '\0') {
        if (!isdigit(*argv) && (!islower(*argv))) {
            return NOT_A_NUM;
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
        return NUM_TOO_LARGE;
    }
    if (cnt == 0) {
        return EMPTY_LINE;
    }
    return VALID;
}

int strLength(const char *str) {
    int res = 0;
    while (*str != '\0') {
        res++;
        str++;
    }
    return res;
}

char * IntToString(int number) {
    int len = 0, tmp = number;
    while (tmp) {
        len++;
        tmp /= 10;
    }
    char * res = (char*)malloc(len + 1);
    int i = 0, tmpNum = number;
    while (tmpNum) {
        res[i] = (tmpNum % 10) + '0';
        tmpNum /= 10;
        i++;
    }
    char * rev = (char*)malloc(len + 1);
    for (int j = 0; j < len; j++) {
        rev[j] = res[len - j - 1];
    }
    rev[len] = '\0';
    free(res);
    return rev;
}

int IsKaprekar(char * num, int base) {
    int number = FromBaseTo10(base, num);
    int square = number * number;

    int numLen = strLength(num);
    char * sq = IntToString(square);
    printf("Sq: %s\n", sq);

    int sqLen = strLength(sq);
    int lSize = sqLen - numLen;
    if (lSize == 0) {
        lSize = 1;
    }
    char * L = (char*)malloc(lSize + 1);
    char * R = (char*)malloc(numLen + 1);

    if (L == NULL || R == NULL) {
        return MALLOC_FAILED;
    }
    L[lSize] = '\0';
    R[numLen] = '\0';

    if (sqLen == numLen) {
        L[0] = '0';
    } else {
        for (int i = 0; i < lSize; ++i) {
            L[i] = sq[i];
        }
    }

    for (int i = 0; i < numLen; ++i) {
        R[i] = sq[sqLen - numLen + i];
    }
    printf("L: <%s> | R: <%s>\n", L, R);

    if ((FromBaseTo10(10, L) + FromBaseTo10(10, R)) == number) {
        free(L);
        free(R);
        free(sq);
        return IS_KAPREKAR;
    } else {
        free(L);
        free(R);
        free(sq);
        return IS_NOT_KAPREKAR;
    }
}

int FindKaprekar(int base, int count, ...) {
    if (base < 2 || base > 36) {
        return INVALID_BASE;
    }
    if (count < 1) {
        return WRONG_ARGS;
    }

    va_list arg;
    va_start(arg, count);

    for (int i = 0; i < count; i++) {
        char *num = va_arg(arg, char *);
        //printf("%s\n", num);
        int ret = Validate(num, base);
        if (ret != VALID) {
            va_end(arg);
            return ret;
        }
        ret = IsKaprekar(num, base);
        if (ret == IS_KAPREKAR) {
            printf("Kaprekar %s\n", num);
        } else if (ret == IS_NOT_KAPREKAR) {
            printf("Not kaprekar %s\n", num);
        } else {
            return ret;
        }
    }

    return SUCCESS;
}

int main() {
    int ret = IsConvex(12, 1.0, 0.0, 4.0, 0.0, 5.0, 2.0, 4.0, 4.0, 2.0, 3.0, 0.0, 1.0);
    ValidateCode(ret);

    printf("%f\n", PolynomeValue(0.5, 6, 19, 5, 0, 4, -10, 0, -100));

    ret = FindKaprekar(10, 3, "9", "297", "2223");
    ValidateCode(ret);
    return 0;
}