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
        case NOT_A_NUM: printf("NOT_A_NUM or INCORRECT_NUMBER_SYSTEM\n"); break;
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

double PolynomeValue(double x, int power, ...) {
    va_list arg;
    va_start(arg, power);

    double res = 0;
    for (int i = 0; i < power; i++) {
        int koef = va_arg(arg, int);
        res = res * x + koef;
    }
    va_end(arg);
    return res;
}

long long FromBaseTo10 (const int base, char *str) {
    long long res = 0, negFlag = 0;
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

char * From10toBase(long long num, int base) {
    char buf[BUFSIZ], *p = buf + BUFSIZ - 1;
    long long r;
    int len = 0;
    *p-- = '\0';

    while (num) {
        r = num % base;
        if (r > 9) {
            *p = r - 10 + 'a';
        } else {
            *p = r + '0';
        }
        p--;
        num /= base;
        len++;
    }
    char * res = (char*)malloc(len + 1);
    if (res == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        res[i] = *(p+1+i);
    }
    res[len] = '\0';
    return res;
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
    const char * p = str;
    int res = 0;
    while (*p != '\0') {
        res++;
        p++;
    }
    return res;
}

long long FromBaseTo10withBorders (const int base, const char *str, int l, int r) {
    long long res = 0;
    int len = r - l + 1;
    const char *p = str;

    p += l;

    while (*p && len > 0) {
        if (isalpha(*p)) {
            res = res * base + *p++ - 'a' + 10;
        }
        else {
            res = res * base + *p++ - '0';
        }
        len--;
    }
    return res;
}

int IsKaprekar(char * num, int base) {
    long long number = FromBaseTo10(base, num);
    long long square = number * number;

    char * sq = From10toBase(square, base);
    //printf("Sq: %s\n", sq);

    int sqLen = strLength(sq);

    for (int split = 0; split < sqLen; split++) {
        long long l = FromBaseTo10withBorders(base, sq, 0, split);
        long long r = FromBaseTo10withBorders(base, sq, split + 1, sqLen - 1);
        if (l + r == number) {
            free(sq);
            return IS_KAPREKAR;
        }
    }
    free(sq);
    return IS_NOT_KAPREKAR;
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

    printf("%f\n", PolynomeValue(0.5, 7, 19, 5, 0, 4, -10, 0, -100));

    ret = FindKaprekar(16, 4, "ff", "1", "8d874", "9d036");
    if (ret != SUCCESS) {
        ValidateCode(ret);
    }
    return 0;
}