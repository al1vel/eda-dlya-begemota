#include <stdio.h>
#include <stdlib.h>
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
        result[ind++] = (ostatok > 9) ? (ostatok + 'A' - 10) : (ostatok + '0');
    }
    if (num == 1) {
        result[ind++] = '1';
    }
    result[ind] = '\0';

    char* reversed = reverse(result);
    return reversed;
}

void vizov(int base, int num) {
    char *res = func(base, num);
    if (strcmp(res, "Invalid base number\0") == 0) {
        printf("Invalid base number: %d.\n", base);
        return;
    }
    printf("Number %d in <%d> number system is: %s\n", num, base, res);
    free(res);
}

int main() {
    vizov(0, 25);
    vizov(1, 25);
    vizov(3, 25);
    vizov(33, 25);
    vizov(-1, 25);

    vizov(2, 25);
    vizov(4, 25);
    vizov(8, 25);
    vizov(16, 25);
    vizov(32, 25);

    vizov(2, 152);
    vizov(4, 152);
    vizov(8, 152);
    vizov(16, 152);
    vizov(32, 152);

    vizov(2, 251);
    vizov(4, 251);
    vizov(8, 251);
    vizov(16, 251);
    vizov(32, 251);

    return 0;
}