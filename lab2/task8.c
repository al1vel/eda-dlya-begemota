#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

char * summ(char* first, char* second, int base) {
    int lenFirst = strLength(first), lenSecond = strLength(second);

    char *res = (char*)malloc((lenFirst + lenSecond + 1)*sizeof(char));
    if (res == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    char *up, *down;
    if (lenFirst >= lenSecond) {
        up = first + lenFirst - 1;
        down = second + lenSecond - 1;
    } else {
        up = second + lenSecond - 1;
        down = first + lenFirst - 1;
    }
    int ind = 0, dobavka = 0, razr;
    while (*down) {
        int d = (isdigit(*down)) ? *down - '0' : *down - 'a' + 10;
        int u = (isdigit(*up)) ? *up - '0' : *up - 'a' + 10;
        int s = d + u + dobavka;
        if (s > base) {
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
    }

    while (*up) {
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
    }
    if (dobavka > 0) {
        res[ind++] = (dobavka > 9) ? dobavka - 10 + 'a' : dobavka + '0';
    }
    res[ind] = '\0';

    char *reversed = reverse(res);
    free(res);
    return reversed;
}

int main() {
    char * res = summ("63c", "1ef35", 16);
    printf("%s\n", res);
    free(res);
}
