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
    FILE_OPEN_FAILURE = -5,
    THREAD_CREATION_ERROR,
    MALLOC_FAIL,
    INVALID_FLAG,
    WRONG_NUMBER_OF_ARGUMENTS,
    SUCCESS,
    xor,
    mask,
    copy,
    find
};

struct xorThreadData {
    char *file_path;
    int bytesCnt;
    long long xor_result;
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

void* threadXOR(void *arg) {
    struct xorThreadData *data = (struct xorThreadData*)arg;
    printf("%s - %d\n", data->file_path, data->bytesCnt);
    FILE* file = fopen(data->file_path, "r");
    if (file == NULL) {
        data->xor_result = FILE_OPEN_FAILURE;
    } else {
        int result;
        int size = data->bytesCnt == 0 ? 1 : data->bytesCnt;
        char buffer[size];

    }
    return NULL;
}

int funcXOR(char *arg[], int fileCnt, int N) {
    int bytesCnt = simplePow(2, N) / 8;

    pthread_t *thr_ids = (pthread_t*)malloc(fileCnt * sizeof(pthread_t));
    if (thr_ids == NULL) {
        printf("malloc failed\n");
        return MALLOC_FAIL;
    }

    struct xorThreadData data[fileCnt];

    for (int i = 0; i < fileCnt; i++) {
        pthread_t thr_id = 0;
        data[i].file_path = arg[i];
        data[i].bytesCnt = bytesCnt;
        if (pthread_create(&thr_id, NULL, &threadXOR, &data[i]) != 0) {
            printf("Thread creation error");
            return THREAD_CREATION_ERROR;
        }
        //printf("Thread %d created\n", i);
        thr_ids[i] = thr_id;
    }

    for (int i = 0; i < fileCnt; i++) {
        pthread_join(thr_ids[i], NULL);
        if (data[i].xor_result == FILE_OPEN_FAILURE) {
            printf("File open failure.\n");
            free(thr_ids);
            return FILE_OPEN_FAILURE;
        }
        printf("xor%d result of file %s: %lld\n", N, data[i].file_path, data[i].xor_result);
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