#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_flag(const char* arg, const char** flags) {
    for (int i = 0; i < 16; ++i) {
        if (!strcmp(arg, flags[i])) {
            return i;
        }
    }
    return -1;
}

void Adding_prefix(char* prefix, char* outFile, char* inFile) {
    char* ptr_pref = prefix;
    char* ptr = inFile + strlen(inFile);
    while (*ptr != '\\') {
        --ptr;
    }
    ++ptr;
    while (inFile != ptr) {
        *outFile++ = *inFile++;
    }
    while (*ptr_pref != '\0') {
        *outFile++ = *ptr_pref++;
    }
    while ((*outFile++ = *ptr++)) {};
    //printf("hui");
}

int main(const int argc, char *argv[]) {
    if (argc == 1) {
        printf("Wrong number of arguments\n");
        return -1;
    }
    const char* flags[] = { "-d", "-i", "-s", "-a", "/d", "/i", "/s", "/a", "-nd", "-ni", "-ns", "-na", "/nd", "/ni", "/ns", "/na" };
    const int flag = find_flag(argv[1], flags);
    if (flag == -1) {
        printf("Flag <%s> is not supported\n", argv[1]);
        return -1;
    }

    char* inFile = (char*)malloc(strlen(argv[2]) + 1);
    if (inFile == NULL) {
        printf("Error allocating memory for input file\n");
        return -1;
    }
    strcpy(inFile, argv[2]);
    char* outFile = NULL;

    if (flag < 8) {
        if (argc != 3) {
            printf("Wrong number of arguments\n");
            return -1;
        }
        char prefix[] = "out_";
        outFile = (char*)malloc(strlen(argv[2]) + strlen(prefix) + 1);
        if (outFile == NULL) {
            printf("Error allocating memory for output file\n");
            return -1;
        }
        Adding_prefix(prefix, outFile, inFile);
    } else {
        if (argc != 4) {
            printf("Wrong number of arguments\n");
            return -1;
        }
        outFile = (char*)malloc(strlen(argv[3]) + 1);
        if (outFile == NULL) {
            printf("Error allocating memory for output file\n");
            return -1;
        }
        strcpy(outFile, argv[3]);
    }

    printf("Output file: %s\n", outFile);
    printf("Input file: %s\n", inFile);

    free(inFile);
    free(outFile);
    return 0;
}