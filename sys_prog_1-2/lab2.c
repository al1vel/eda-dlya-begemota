#include <stdio.h>

enum {
    INVALID_FLAG = -2,
    WRONG_NUMBER_OF_ARGUMENTS,
    SUCCESS,
    xor,
    mask,
    copy,
    find
};

int contains(const char *str, const char* substr) {
    while (*substr != '\0') {
        if (*str != *substr) {
            return 0;
        }
        substr++;
        str++;
    }
    return 1;
}

int toInt(const char *str, const int base) {
    int result = 0;
    while (*str != '\0') {
        if ((*str < '0' || *str > '9') && (*str < 'a' || *str > 'f')) {
            return -1;
        }
        if (*str >= '0' && *str <= '9') {
            result = result * base + *str - '0';
        } else {
            result = result * base + *str - 'a' + 10;
        }
        str++;
    }
    return result;
}

int containsFlag(const char *arg) {
    if (contains(arg, "xor")) {
        return xor;
    }
    if (contains(arg, "mask")) {
        return mask;
    }
    if (contains(arg, "copy")) {
        return copy;
    }
    if (contains(arg, "find")) {
        return find;
    }
    return INVALID_FLAG;
}

int main(const int argc, char *argv[]) {
    if (argc < 3) {
        printf("Provide file paths and a flag.\n\n");
        return WRONG_NUMBER_OF_ARGUMENTS;
    }

    int code = containsFlag(argv[argc - 1]);

    if (code == INVALID_FLAG) {
        printf("Invalid flag.\nAvailable flags:\n  xorN\n  mask<hex>\n  copyN\n  find<SomeString>\n\n");
        return code;
    }
    printf("flag - %d\n", code);

    int N = -1;
    char *pattern = NULL;

    if (code == xor) {
        char *p = argv[argc - 1];
        p += 3;
        if (*p == '2' || *p == '3' || *p == '4' || *p == '5' || *p == '6') {
            if (*(p +1 ) != '\0') {
                printf("Invalid flag.\nParameter of flag xor must be a number 2-6 only.\n");
                return INVALID_FLAG;
            }
            N = toInt(p, 10);
        } else {
            printf("Invalid flag.\nParameter of flag xor must be a number 2-6 only.\n");
            return INVALID_FLAG;
        }
    }

    else if (code == mask) {
        char *p = argv[argc - 1];
        p += 4;
        if (*p != '<') {
            printf("Invalid flag.\nCorrect usage: mask<hex>\n");
            return INVALID_FLAG;
        }
        p++;
        char buf[9];
        int i = 0;
        while (*p != '>') {
            if (i >= 8) {
                printf("Invalid flag.\nCorrect usage: mask<hex>\nhex can be 8 symbols long maximum\n");
                return INVALID_FLAG;
            }
            if (*p == '\0') {
                printf("Invalid flag.\nCorrect usage: mask<hex>\n");
                return INVALID_FLAG;
            }
            buf[i] = *p;
            i++;
            p++;
        }
        buf[i] = '\0';
        printf("<%s>\n", buf);
        N = toInt(buf, 16);
    }


    printf("N - %d\n", N);
    return 0;
}