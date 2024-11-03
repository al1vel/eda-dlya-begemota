#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

enum CODES {
    VALID,
    NOT_A_NUM,
    NUMBER_TOO_BIG
};

void ValidateCodes (CODES ret_code) {
    switch (ret_code) {
        case VALID: printf("Valid\n"); break;
        case NOT_A_NUM: printf("is NOT a number\n"); break;
        case NUMBER_TOO_BIG: printf("is too big\n"); break;
        default: printf("UNKNOWN ERROR CODE\n");
    }
}

int toInt(const char* argv) {
    int res = 0;
    int i = 0;
    int neg_flag = 0;
    if (argv[0] == '-') {
        neg_flag = 1;
        i = 1;
    }
    while (argv[i] != '\0') {
        res = res * 10 + argv[i] - '0';
        i++;
    }
    if (neg_flag) {
        res *= -1;
    }
    //printf("%d\n", res);
    return res;
}

CODES ValidateIntNum(const char* argv) {
    if (*argv == '-') argv++;
    int first = *argv - '0';
    int cnt = 0;
    while (*argv) {
        if (!isdigit(*argv)) return NOT_A_NUM;
        argv++;
        cnt++;
    }
    //printf("%d %d", cnt, first);
    if (cnt >= 11) return NUMBER_TOO_BIG;
    if (cnt >= 10 and first > 2) return NUMBER_TOO_BIG;
    return VALID;
}

void SwapMaxMin(int arr[], const int size, const int a, const int b) {
    int ind_of_max = -1, ind_of_min = -1, max = a - 1, min = b + 1;
    for (int i = 0; i < size; ++i) {
        if (arr[i] > max) {
            ind_of_max = i;
            max = arr[i];
        } else if (arr[i] < min) {
            ind_of_min = i;
            min = arr[i];
        }
    }
    const int temp = arr[ind_of_min];
    arr[ind_of_min] = arr[ind_of_max];
    arr[ind_of_max] = temp;
}

int main(const int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments\n");
        return -1;
    }
    CODES ret_code = ValidateIntNum(argv[1]);
    if (ret_code != VALID) {
        ValidateCodes(ret_code);
        return -1;
    }
    ret_code = ValidateIntNum(argv[2]);
    if (ret_code != VALID) {
        ValidateCodes(ret_code);
        return -1;
    }
    const int a = toInt(argv[1]);
    const int b = toInt(argv[2]);

    if (a == b) {
        printf("Borders can't be equal\n");
        return -1;
    } else if (a > b) {
        printf("a can't be more then b\n");
        return -1;
    }

    int arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = rand() % (b - a + 1) + a;
        printf("%d ", arr[i]);
    }
    printf("\n");
    //int *p = arr;
    SwapMaxMin(arr, 10, a, b);

    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}