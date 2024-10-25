#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

int strLength(const char *str) {
    int res = 0;
    while (*str != '\0') {
        res++;
        str++;
    }
    return res;
}

void strcpy(char *str, char *src) {
    while (*src != '\0') {
        *str++ = *src++;
    }
    *str = '\0';
}

char *reverse(const char *str) {
    int len = strLength(str);
    char *reversed = (char*)malloc(len + 1);
    if (reversed == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - i - 1];
    }
    reversed[len] = '\0';
    return reversed;
}

char * plus(char* first, char* second, int base) {
    int lenFirst = strLength(first), lenSecond = strLength(second), lenUp, lenDown;

    char *res = (char*)malloc((lenFirst + lenSecond + 1)*sizeof(char));
    if (res == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    char *up, *down;
    if (lenFirst >= lenSecond) {
        up = first + lenFirst - 1;
        down = second + lenSecond - 1;
        lenDown = lenSecond;
        lenUp = lenFirst;
    } else {
        up = second + lenSecond - 1;
        down = first + lenFirst - 1;
        lenDown = lenFirst;
        lenUp = lenSecond;
    }
    int ind = 0, dobavka = 0, razr;
    while (lenDown > 0) {
        int d = (isdigit(*down)) ? *down - '0' : *down - 'a' + 10;
        int u = (isdigit(*up)) ? *up - '0' : *up - 'a' + 10;
        int s = d + u + dobavka;
        if (s >= base) {
            razr = s - base;
            dobavka = (s / base);
        } else {
            razr = s;
            dobavka = 0;
        }
        char ch = (razr > 9) ? (razr - 10) + 'a' : razr + '0';
        res[ind++] = ch;
        down--;
        up--;
        lenDown--;
        lenUp--;
    }

    while (lenUp > 0) {
        char ch;
        if (dobavka > 0) {
            int s = dobavka + ((isdigit(*up)) ? *up - '0' : *up - 'a' + 10);
            if (s >= base) {
                razr = s - base;
                dobavka = (s / base);
            } else {
                razr = s;
                dobavka = 0;
            }
            ch = (razr > 9) ? (razr - 10) + 'a' : razr + '0';
        } else {
            ch = *up;
        }
        res[ind++] = ch;
        up--;
        lenUp--;
    }
    if (dobavka > 0) {
        res[ind++] = (dobavka > 9) ? dobavka - 10 + 'a' : dobavka + '0';
    }
    res[ind] = '\0';

    char *reversed = reverse(res);
    free(res);
    return reversed;
}

char * summ(int base, int count, ...) {
    va_list arg;
    va_start(arg, count);

    if (count < 1) {
        printf("Nothing to summ.\n");
        va_end(arg);
        return NULL;
    } else if (count == 1) {
        char *str = va_arg(arg, char *);
        va_end(arg);
        return str;
    }

    char * first = va_arg(arg, char *);
    char * second = va_arg(arg, char *);
    char *firstSum = plus(first, second, base);
    int len = strLength(firstSum);

    char *res = (char*)malloc((len + 1)*sizeof(char));
    if (res == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    strcpy(res, firstSum);
    free(firstSum);

    for (int i = 2; i < count; ++i) {
        char * num = va_arg(arg, char *);
        char * promRes = plus(res, num, base);
        strcpy(res, promRes);
        free(promRes);
    }
    return res;
}

int main() {
    char* res = summ(10, 4, "123", "165", "123456789000", "1");
    printf("%s\n", res);
    free(res);
}