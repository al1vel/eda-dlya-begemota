#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum CODES {
    VALID,
    NO_FORMAT,
    INVALID_PATH,
    SUCCESS
};

void ValidateCode(const CODES code) {
    switch (code) {
        case NO_FORMAT: printf("Path has invalid format\n"); break;
        case INVALID_PATH: printf("Path is invalid\n"); break;
        default: printf("Invalid code\n"); break;
    }
}

CODES ValidatePath(char* path) {
    int l = strlen(path);
    char* ptr = path + l - 1;
    while (*ptr != '.' and l > 0) {
        --ptr;
        l--;
    }
    ++ptr;
    if(strcmp(ptr, "txt") != 0) {
        return NO_FORMAT;
    }
    while (isalpha(*ptr) || isdigit(*ptr) || *ptr == '-'
    || *ptr == '_' || *ptr == '.' || *ptr == '\\') {
        --ptr;
    }
    if (*ptr != ':' || !isupper(*(ptr - 1)) || *(ptr + 1) != '\\') {
        return INVALID_PATH;
    }
    return VALID;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Wrong number of arguments\n");
        return -1;
    }
    int flag;
    if (!strcmp(argv[1], "-r") || !strcmp(argv[1], "/r")) {
        flag = 0;
    } else if (!strcmp(argv[1], "-a") || !strcmp(argv[1], "/a")) {
        flag = 1;
    } else {
        printf("Flag <%s> is not supported", argv[1]);
        return -1;
    }
    //printf("Flag: %d\n", flag);

    if (flag == 0) {
        if (argc < 6) {
            printf("Wrong number of arguments\n");
            return -1;
        }
        CODES ret_code = ValidatePath(argv[3]);
        //printf("%s %s %s\n", argv[3], argv[4], argv[argc - 1]);
        if (ret_code != VALID) {
            ValidateCode(ret_code);
            return -1;
        }
        ret_code = ValidatePath(argv[4]);
        if (ret_code != VALID) {
            ValidateCode(ret_code);
            return -1;
        }
        ret_code = ValidatePath(argv[argc - 1]);
        if (ret_code != VALID) {
            ValidateCode(ret_code);
            return -1;
        }
        printf("Paths are valid\n");

        char* inFile1 = (char*)malloc(strlen(argv[3]) + 1);
        if (inFile1 == NULL) {
            printf("Error allocating memory for input file1\n");
            return -1;
        }
        strcpy(inFile1, argv[3]);

        char* outFile = (char*)malloc(strlen(argv[argc - 1]) + 1);
        if (outFile == NULL) {
            printf("Error allocating memory for output file\n");
            return -1;
        }
        strcpy(outFile, argv[argc - 1]);

        char* inFile2 = (char*)malloc(strlen(argv[4]) + 1);
        if (inFile2 == NULL) {
            printf("Error allocating memory for input file2\n");
            return -1;
        }
        strcpy(inFile2, argv[4]);

        printf("Input 1: %s\nInput 2: %s\nOutput: %s\n", inFile1, inFile2, outFile);



        free(inFile2);
        free(inFile1);
        free(outFile);

    } else {

    }
    return 0;
}