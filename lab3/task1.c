#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int strLength(const char *str) {
    int res = 0;
    while (*str != '\0') {
        res++;
        str++;
    }
    return res;
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

int powerOfBase(int base) {
    int result = 0;
    while (base) {
        base >>= 1;
        result++;
    }
    return result - 1;
}

int determineLen(int num) {
    int res = 0;
    while (num) {
        num >>= 1;
        res++;
    }
    return res + 2;
}

int getMask(int power) {
    int res = 0;
    switch(power) {
        case 1: res = 1;break;
        case 2: res = 3;break;
        case 3: res = 7;break;
        case 4: res = 15;break;
        case 5: res = 31;break;
        default: res = -1;break;
    }
    return res;
}

char *func(int base, int num) {
    if (base != 2 && base != 4 && base != 8 && base != 16 && base != 32) {
        char *msg = "Invalid base number\0";
        return msg;
    }
    int power = powerOfBase(base), len = determineLen(num), mask = getMask(power);
    char *result = (char *)malloc(len);
    if (result == NULL) {
        printf("Error allocating memory\n");
        return NULL;
    }

    int ind = 0, ostatok;
    while (num > 1) {
        ostatok = num & mask;
        num >>= power;
        result[ind++] = (ostatok + '0');
    }
    if (num == 1) {
        result[ind++] = '1';
    }
    result[ind] = '\0';

    char* reversed = reverse(result);
    return reversed;
}

int main() {
    char *res = func(32, 35);
    printf("%s\n", res);

    if (strcmp(res, "Invalid base number\0") != 0) {
        free(res);
    }

    return 0;
}