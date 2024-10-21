#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int my_strcmp(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    return 1;
}

int find_flag(const char* arg, const char** flags) {
    for (int i = 0; i < 5; ++i) {
        if (my_strcmp(arg, flags[i])) {
            return i;
        }
    }
    return -1;
}

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

char toUpper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    } else {
        return c;
    }
}

char *everyOddToUpper(const char *str) {
    int len = strLength(str);
    char *result = (char*)malloc(len + 1);
    if (result == NULL) {
        printf("malloc failed\n");
        return NULL;
    }

    for (int i = 0; i < len; i++) {
        if (i % 2 == 1) {
            result[i] = toUpper(str[i]);
        } else {
            result[i] = str[i];
        }
    }
    result[len] = '\0';
    return result;
}

char *newOrder(const char *str) {
    int len = strLength(str);
    char digits[len], alpha[len], syms[len];
    int a = 0, b = 0, c = 0;

    char *result = (char*)malloc(len + 1);
    if (result == NULL) {
        printf("malloc failed\n");
        return NULL;
    }

    for (int i = 0; i < len; ++i) {
        if (isdigit(str[i])) {
            digits[a] = str[i];
            a++;
        } else if (isalpha(str[i])) {
            alpha[b] = str[i];
            b++;
        } else {
            syms[c] = str[i];
            c++;
        }
    }

    int i = 0;
    while (i < len) {
        for (int j = 0; j < a; ++j) {
            result[i] = digits[j];
            i++;
        }
        for (int j = 0; j < b; ++j) {
            result[i] = alpha[j];
            i++;
        }
        for (int j = 0; j < c; ++j) {
            result[i] = syms[j];
            i++;
        }
    }
    result[len] = '\0';
    return result;
}

int main(int argc, char *argv[]) {
    char* flags[] = { "-l", "-r", "-u", "-n", "-c" };

    if (argc <= 1) {
        printf("Flag must be inputted\n");
        return -1;
    }

    const int ret = find_flag(argv[1], flags);
    if (ret == -1) {
        printf("Flag <%s> is not supported.\n", argv[1]);
        return -1;
    }
    printf("Flag returned: %d\n", ret);

    switch (ret) {
        case 0: {
            int len = strLength(argv[2]);
            printf("Length is %d.\n", len);
            break;
        }
        case 1: {
            char *rev = reverse(argv[2]);
            printf("Reverse is %s\n", rev);
            free(rev);
            break;
        }
        case 2: {
            char *res = everyOddToUpper(argv[2]);
            printf("EveryOddToUpped is %s\n", res);
            free(res);
            break;
        }
        case 3: {
            char *res = newOrder(argv[2]);
            printf("NewOrder is %s\n", res);
            free(res);
            break;
        }
        default: {
            printf("Invalid func type.\n");
            break;
        }
    }


    return 0;
}