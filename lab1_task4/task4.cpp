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

typedef CODES(*callback)(FILE*, FILE*);

void ValidateCode(const CODES code) {
    switch (code) {
        case NO_FORMAT: printf("Path has invalid format\n"); break;
        case INVALID_PATH: printf("Path is invalid\n"); break;
        default: printf("Invalid code\n"); break;
    }
}

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

CODES ValidatePath(char* path) {
    int l = strlen(path);
    char* ptr = path + l - 1;
    while (*ptr != '.' && l > 0) {
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

CODES funcForD (FILE* inFile, FILE* outFile) {
    char c;
    do {
        c = (char)fgetc(inFile);
        if (c != EOF && !isdigit(c)) {
            fputc(c, outFile);
        }
    } while (c != EOF);
    return SUCCESS;
}

CODES funcForI (FILE* inFile, FILE* outFile) {
    char c;
    int cnt = 0;
    do {
        c = (char)fgetc(inFile);
        if (c != EOF) {
            if (c == '\n') {
                fprintf(outFile, "%d\n", cnt);
                cnt = 0;
            }
            else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                cnt++;
            }
        }
    } while (c != EOF);
    fprintf(outFile, "%d\n", cnt);
    return SUCCESS;
}

CODES funcForS (FILE* inFile, FILE* outFile) {

    char c;
    int cnt = 0;
    do {
        c = (char)fgetc(inFile);
        //printf("%c %d\n", c, (!isdigit(c) && !(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z') && (c != ' ')));
        if (c != EOF) {
            if (c == '\n') {
                fprintf(outFile, "%d\n", cnt);
                cnt = 0;
            }
            else if (!isdigit(c) && !(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z') && (c != ' ')) {
                cnt++;
            }
        }
    } while (c != EOF);
    fprintf(outFile, "%d\n", cnt);
    return SUCCESS;
}

CODES funcForA (FILE* inFile, FILE* outFile) {
    printf("hui");
    char c;

    do {
        c = (char)fgetc(inFile);
        if (c != EOF) {
            if (!isdigit(c)) {
                //int code = (int)c;
                fprintf(outFile, "%X", c);
            } else {
                fprintf(outFile, "%c", c);
            }
        }
    } while (c != EOF);

    return SUCCESS;
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

    CODES ret_code = ValidatePath(argv[2]);
    if (ret_code != VALID) {
        ValidateCode(ret_code);
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
        ret_code = ValidatePath(argv[3]);
        if (ret_code != VALID) {
            ValidateCode(ret_code);
            return -1;
        }
        strcpy(outFile, argv[3]);
    }

    printf("Input file: %s\n", inFile);
    printf("Output file: %s\n", outFile);

    if (!strcmp(inFile, outFile)) {
        printf("Input && output files are the same\n");
        return -1;
    }

    FILE* in = fopen(inFile, "r");
    FILE* out = fopen(outFile, "w");
    if (in == NULL) {
        printf("Error opening INPUT file\n");
        return -1;
    }
    if (out == NULL) {
        printf("Error opening OUT file\n");
        return -1;
    }

    int FuncType = flag % 4;
    callback functions[4] = {funcForD, funcForI, funcForS, funcForA};

    callback func = functions[FuncType];
    func(in, out);

    fclose(in);
    fclose(out);
    free(inFile);
    free(outFile);
    return 0;
}