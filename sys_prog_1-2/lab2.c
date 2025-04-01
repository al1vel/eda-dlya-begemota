#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>

int simplePow(int num, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= num;
    }
    return result;
}

enum {
    THREAD_CREATION_ERROR = -4,
    MALLOC_FAIL,
    INVALID_FLAG,
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

void *threadXOR(void *arg) {
    char *file_path = (char*)arg;
    printf("%s\n", file_path);
    return NULL;
}

int funcXOR(char *arg[], int fileCnt, int N) {
    int bytesCnt = simplePow(2, N) / 8;
    printf("%d\n", bytesCnt);

    pthread_t *thr_ids = (pthread_t*)malloc(fileCnt * sizeof(pthread_t));
    if (thr_ids == NULL) {
        printf("malloc failed\n");
        return MALLOC_FAIL;
    }

    for (int i = 0; i < fileCnt; i++) {
        //printf("%s\n", arg[i]);
        pthread_t thr_id = 0;
        if (pthread_create(&thr_id, NULL, &threadXOR, (void*)arg[i]) != 0) {
            printf("Thread creation error");
            return THREAD_CREATION_ERROR;
        }
        printf("Thread %d created\n", i);
    }

    free(thr_ids);
    return SUCCESS;
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

        int ret = funcXOR(++argv, argc - 2, N);
        if (ret != SUCCESS) {
            return code;
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
        if (N == -1) {
            printf("Invalid argument to flag <mask>.\nArgument must be a hex number only.\n");
            return INVALID_FLAG;
        }
    }

    else if (code == copy) {
        char *p = argv[argc - 1];
        p += 4;
        if (*p == '\0') {
            printf("Invalid argument to flag <copy>.\nArgument must be a decimal number.\n");
            return INVALID_FLAG;
        }
        char buf[4];
        int i = 0;
        while (*p != '\0') {
            if (i >= 3) {
                printf("The argument of flag <copy> must be under 1000.\n");
                return INVALID_FLAG;
            }
            if (!isdigit(*p)) {
                printf("The argument of flag <copy> must be a decimal number.\n");
                return INVALID_FLAG;
            }
            buf[i] = *p;
            i++;
            p++;
        }
        buf[i] = '\0';
        N = toInt(buf, 10);
    }

    else if (code == find) {
        char *p = argv[argc - 1];
        p += 4;
        if (*p != '<') {
            printf("Invalid flag.\nCorrect usage: find<SomeString>\n");
            return INVALID_FLAG;
        }
        p++;
        int size = 16;
        pattern = (char*)malloc(size * sizeof(char));
        if (pattern == NULL) {
            printf("Memory allocation failed.\n");
            return MALLOC_FAIL;
        }
        int i = 0;
        while (*p != '>') {
            if (*p == '\0') {
                printf("Invalid flag.\nCorrect usage: find<SomeString>\n");
                free(pattern);
                return INVALID_FLAG;
            }

            if (i == (size - 1)) {
                size = size * 2;
                char *p = realloc(pattern, size);
                if (p == NULL) {
                    printf("Memory allocation failed.\n");
                    free(pattern);
                    return MALLOC_FAIL;
                }
                pattern = p;
                printf("reallocated!\n");
            }

            pattern[i] = *p;
            i++;
            p++;
        }
        pattern[i] = '\0';
    }

    if (pattern != NULL) {
        printf("pattern: (%s)\n", pattern);
    }
    printf("N: %d\n", N);


    if (pattern != NULL) {
        free(pattern);
    }
    return 0;
}