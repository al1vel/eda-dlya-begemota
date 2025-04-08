#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int simplePow(int num, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= num;
    }
    return result;
}

enum {
    PROCESS_DIED = -8,
    FORK_FAILURE = -7,
    INVALID_NUMBER = -6,
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
    uint64_t xor_result;
    int error;
};

struct maskThreadData {
    char *file_path;
    int mask;
    int answer;
    int error;
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
    FILE* file = fopen(data->file_path, "rb");
    if (file == NULL) {
        data->error = FILE_OPEN_FAILURE;
        return NULL;
    }

    uint64_t result = 0;
    int size = data->bytesCnt == 0 ? 1 : data->bytesCnt;
    uint8_t buffer[size];

    int bytesRead = fread(buffer, 1, size, file);
    if (bytesRead == 0) {
        data->xor_result = 0;
        return NULL;
    }

    if (data->bytesCnt == 0) {
        result = buffer[0] >> 4;
        result = result ^ (buffer[0] & 0x0F);
    } else {
        uint64_t block = 0;
        for (int i = 0; i < size; ++i) {
            block = block << 8;
            block |= buffer[i];
        }
        result = block;
    }

    while ((bytesRead = fread(buffer, 1, size, file)) > 0) {
        if (data->bytesCnt == 0) {
            result = result ^ (buffer[0] >> 4);
            result = result ^ (buffer[0] & 0x0F);
        } else {
            uint64_t block = 0;
            for (int j = 0; j < size; ++j) {
                block = block << 8;
                if (j < bytesRead) {
                    block |= buffer[j];
                }
            }
            result = result ^ block;
        }
    }
    fclose(file);
    data->xor_result = result;
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
        thr_ids[i] = thr_id;
    }

    for (int i = 0; i < fileCnt; i++) {
        pthread_join(thr_ids[i], NULL);
        if (data[i].error == FILE_OPEN_FAILURE) {
            printf("File open failure.\n");
            free(thr_ids);
            return FILE_OPEN_FAILURE;
        }
        printf("xor%d result of file %s: %ld\n", N, data[i].file_path, data[i].xor_result);
    }

    free(thr_ids);
    return SUCCESS;
}

int getNumber(char *str) {
    int result = 0, cnt = 0;
    while (*str != '\n' && *str != '\0') {
        if (cnt > 11) {
            return INVALID_NUMBER;
        }
        if (!isdigit(*str)) {
            return INVALID_NUMBER;
        }
        result = result * 10 + *str - '0';
        str++;
        cnt++;
    }
    return result;
}

void *threadMask(void *arg) {
    struct maskThreadData *data = (struct maskThreadData*)arg;
    FILE* file = fopen(data->file_path, "r");
    if (file == NULL) {
        data->error = FILE_OPEN_FAILURE;
        return NULL;
    }

    int answer = 0;
    char buf[11];
    while (fgets(buf, 11, file)!= NULL) {
        int num = getNumber(buf);
        if (num == INVALID_NUMBER) {
            data->error = INVALID_NUMBER;
            printf("Invalid number.\n");
            return NULL;
        }
        if ((num & data->mask) == data->mask) {
            answer++;
        }
    }

    fclose(file);
    data->error = SUCCESS;
    data->answer = answer;
    return NULL;
}

int funcMASK(char *arg[], int fileCnt, int mask) {
    pthread_t *thr_ids = (pthread_t*)malloc(fileCnt * sizeof(pthread_t));
    if (thr_ids == NULL) {
        printf("malloc failed\n");
        return MALLOC_FAIL;
    }

    struct maskThreadData data[fileCnt];

    for (int i = 0; i < fileCnt; i++) {
        pthread_t thr_id = 0;
        data[i].file_path = arg[i];
        data[i].mask = mask;
        if (pthread_create(&thr_id, NULL, &threadMask, &data[i]) != 0) {
            printf("Thread creation error");
            return THREAD_CREATION_ERROR;
        }
        thr_ids[i] = thr_id;
    }

    for (int i = 0; i < fileCnt; i++) {
        pthread_join(thr_ids[i], NULL);
        if (data[i].error != SUCCESS) {
            printf("Failure.\n");
            free(thr_ids);
            return data[i].error;
        }
        printf("mask result of file %s: %d\n", data[i].file_path, data[i].answer);
    }

    free(thr_ids);
    return SUCCESS;
}

size_t determineNumLength(int num) {
    size_t len = 0;
    while (num != 0) {
        num = num / 10;
        len++;
    }
    return len;
}

void createName(char *name, int num, char *res) {
    char buf[determineNumLength(num)];
    int k = 0, temp = num;
    while (temp != 0) {
        buf[k] = (temp % 10) + '0';
        temp /= 10;
        k++;
    }

    int i = 0, pos = 0;
    while (name[i] != '.') {
        res[i] = name[i];
        i++;
        pos = i;
    }
    for (int j = (int)determineNumLength(num) - 1; j >= 0; --j) {
        res[i] = buf[j];
        i++;
    }
    while (name[pos] != '\0') {
        res[i] = name[pos];
        i++;
        pos++;
    }
    res[i] = '\0';
}

int funcCOPY(char *arg[], int fileCnt, int N) {
    pid_t pids[fileCnt];

    for (int i = 0; i < fileCnt; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            printf("fork failed\n");
            return FORK_FAILURE;
        }

        else if (pid == 0) {
            printf("%s - %d\n", arg[i], N);

            FILE *input = fopen(arg[i], "rb");
            if (input == NULL) {
                printf("File %s open failed.\n", arg[i]);
                exit(-1);
            }

            for (int k = 0; k < N; k++) {
                size_t len = strlen(arg[i]);
                len += determineNumLength(k + 1);
                char buf[len + 1];
                createName(arg[i], k + 1, buf);
                printf("buf: %s\n", buf);
            }
            fclose(input);
            exit(0);

        } else {
            pids[i] = pid;
        }
    }
    for (int i = 0; i < fileCnt; i++) {
        int status;
        waitpid(pids[i], &status, 0);

        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            if (exit_code != SUCCESS) {
                return FILE_OPEN_FAILURE;
            }
        } else {
            printf("Abnormal process finish.\n");
            return PROCESS_DIED;
        }
    }
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

        int ret = funcMASK(++argv, argc - 2, N);
        if (ret != SUCCESS) {
            return code;
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

        int ret = funcCOPY(++argv, argc - 2, N);
        if (ret != SUCCESS) {
            printf("Finishing with code: %d\n", ret);
            return code;
        }
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


    if (pattern != NULL) {
        free(pattern);
    }
    return 0;
}