#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum {
    SUCCESS,
    INVALID_ARGS,
    ERROR_CREATING_TABLE,
    ERROR_OPEN_INPUT,
    ERROR_OPEN_TEMP,
    ERROR_CREATING_MACRO,
    ERROR_REALLOCATING_TABLE
};

int GetHash(char *directive, int tableSize) {
    int res = 0, base = 62;;
    char *p = directive;

    while (*p) {
        if (isalpha(*p)) {
            res = (res * base + (((*p >= 'A') && (*p <= 'Z')) ? (*p - 'A' + 10) : (*p - 'a' + 10 + 26))) % tableSize;
            p++;
        }
        else {
            res = (res * base + *p++ - '0') % tableSize;
        }
    }
    return res;
}

struct Macro {
    char *directive;
    char *value;
    struct Macro *next;
};

int ResizeNeed(struct Macro ***HashTable, int TableSize) {
    int curChain = 0, maxChain = 0, minChain = 10000;

    for (int i = 0; i < TableSize; i++) {
        if ((*HashTable)[i] != NULL) {
            struct Macro *current = (*HashTable)[i];
            curChain = 0;
            while (current != NULL) {
                curChain++;
                current = current->next;
            }
            if (curChain == 1) {
                curChain = 0;
            }

            if (curChain > maxChain) {
                maxChain = curChain;
            }
            if (curChain < minChain && curChain != 0) {
                minChain = curChain;
            }
        }
    }
    //printf("Max: %d | Min: %d\n", maxChain, minChain);
    if (maxChain == 0) {
        return 0;
    }
    if ((maxChain / 2) >= minChain) {
        return 1;
    }
    return 0;
}

struct Macro ** CreateTable(int size) {
    struct Macro **HashTable = (struct Macro**)malloc(sizeof(struct Macro*) * size);
    if (HashTable == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        HashTable[i] = NULL;
    }
    return HashTable;
}

struct Macro *CreateMacro(char *directive, char *value, int len) {
    struct Macro* curMacro = malloc(sizeof(struct Macro));
    if (curMacro == NULL) {
        free(directive);
        free(value);
        return NULL;
    }
    curMacro->directive = (char *)malloc(len);
    if (curMacro->directive == NULL) {
        free(directive);
        free(value);
        free(curMacro);
        return NULL;
    }
    strcpy(curMacro->directive, directive);
    free(directive);

    curMacro->value = (char *)malloc(len);
    if (curMacro->value == NULL) {
        free(curMacro->directive);
        free(curMacro);
        free(value);
        return NULL;
    }
    strcpy(curMacro->value, value);
    free(value);
    curMacro->next = NULL;
    return curMacro;
}

void CopyTable(struct Macro *** HashTable, struct Macro *** p, int TableSize) {
    for (int j = 0; j < (TableSize - 100); ++j) {
        if ((*HashTable)[j] != NULL) {
            struct Macro *tmp = (*HashTable)[j];
            while (tmp != NULL) {
                printf("D: %s; V: %s\n", tmp->directive, tmp->value);
                int hash = GetHash(tmp->directive, TableSize) % TableSize;
                printf("Dir: <%s>. New hash: %d\n", tmp->directive, hash);
                if ((*p)[hash] == NULL) {
                    (*p)[hash] = tmp;

                    tmp = (*p)[hash]->next;
                    (*p)[hash]->next = NULL;
                } else {
                    struct Macro *pointer = (*p)[hash];
                    while (pointer->next != NULL) {
                        pointer = pointer->next;
                    }
                    pointer->next = tmp;
                    tmp = pointer->next->next;
                    pointer->next->next = NULL;
                }
            }
        }
    }
    free(*HashTable);
}

void PrintTable(struct Macro *** HashTable, int TableSize) {
    for (int i = 0; i < TableSize; i++) {
        if ((*HashTable)[i] != NULL) {
            printf("%d | Dir: <%s> | Val: <%s>\n", i, (*HashTable)[i]->directive, (*HashTable)[i]->value);
            struct Macro *ptr = (*HashTable)[i]->next;
            while (ptr != NULL) {
                printf("%d next | Dir: <%s> | Val: <%s>\n", i, ptr->directive, ptr->value);
                ptr = ptr->next;
            }
        }
    }
}

void InsertMacro (struct Macro *** HashTable, int hash, struct Macro *newMacro) {
    if ((*HashTable)[hash] == NULL) {
        (*HashTable)[hash] = newMacro;
        printf("Dir: <%s>. Val: <%s>. HASH: %d\n", newMacro->directive, newMacro->value, hash);
    } else {
        struct Macro *ptr = (*HashTable)[hash];
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = newMacro;
        printf("XD Dir: <%s>. Val: <%s>. HASH: %d\n", newMacro->directive, newMacro->value, hash);
    }
}

void Clear(struct Macro ***HashTable, int TableSize) {
    for (int i = 0; i < TableSize; i++) {
        if ((*HashTable)[i] != NULL) {
            struct Macro *current = (*HashTable)[i];
            while (current != NULL) {
                struct Macro *temp = current;
                current = current->next;
                free(temp->directive);
                free(temp->value);
                free(temp);
            }
            free(current);
            (*HashTable)[i] = NULL;
        }
    }
    free(*HashTable);
    *HashTable = NULL;
}

struct Macro * GetMacro(char * line) {
    struct Macro *macro = (struct Macro *)malloc(sizeof(struct Macro));
    if (macro == NULL) {
        return NULL;
    }
    macro->directive = malloc(BUFSIZ * sizeof(char));
    if (macro->directive == NULL) {
        free(macro);
        return NULL;
    }
    macro->value = malloc(BUFSIZ * sizeof(char));
    if (macro->value == NULL) {
        free(macro->directive);
        free(macro);
        return NULL;
    }

    while (*line != '\0' && *line != '\n') {
        while (*line != '#') {
            line++;
        }
        for (int i = 0; i < 7; ++i) {
            line++;
        }
        while (*line == ' ' || *line == '\t') {
            line++;
        }
        int ind = 0;
        while (*line != ' ') {
            macro->directive[ind] = *line;
            line++;
            ind++;
        }
        macro->directive[ind] = '\0';
        while (*line == ' ' || *line == '\t') {
            line++;
        }
        ind = 0;
        while (*line != ' ' && *line != '\t' && *line != '\0' && *line != '\n') {
            macro->value[ind] = *line;
            line++;
            ind++;
        }
        macro->value[ind] = '\0';
    }
    macro->next = NULL;
    printf("\nGot macro! Dir: %s; Val: %s;\n", macro->directive, macro->value);
    return macro;
}

int HasDefine(char * line) {
    while (*line != '\0' && *line != '\n') {
        if (*line == '#') {
            if (strncmp(line, "#define", 7) == 0) {
                return 1;
            }
        }
        line++;
    }
    return 0;
}

char * ProcessLine (char * line, struct Macro *** HashTable, int TableSize) {
    char * ptr = line;
    char buf[BUFSIZ];
    int i = 0, fileInd = 0;

    char * inFile = (char *)malloc(BUFSIZ * sizeof(char));

    while (*ptr != '\0' && *ptr != '\n') {
        if (isalnum(*ptr)) {
            buf[i] = *ptr;
            i++;
        } else {
            buf[i] = '\0';
            i = 0;
            //printf("%s\n", buf);
            int hash = GetHash(buf, TableSize);
            if ((*HashTable)[hash] != NULL) {
                struct Macro *p = (*HashTable)[hash];
                int found = 0;
                while (p != NULL) {
                    if (strcmp(p->directive, buf) == 0) {
                        found = 1;
                        char *val = p->value;
                        while (*val != '\0') {
                            inFile[fileInd] = *val;
                            fileInd++;
                            val++;
                        }
                    }
                    p = p->next;
                }
                if (!found) {
                    int k = 0;
                    while (buf[k] != '\0') {
                        inFile[fileInd] = buf[k];
                        fileInd++;
                        k++;
                    }
                }
            } else {
                int k = 0;
                while (buf[k] != '\0') {
                    inFile[fileInd] = buf[k];
                    fileInd++;
                    k++;
                }
            }
            inFile[fileInd] = *ptr;
            fileInd++;
        }
        ptr++;
    }
    inFile[fileInd] = '\0';
    printf("%s\n", inFile);
    return inFile;
}

int HasSuchValue (struct Macro *** HashTable, int TableSize, struct Macro * macro) {
    for (int i = 0; i < TableSize; i++) {
        if ((*HashTable)[i] != NULL) {
            struct Macro *p = (*HashTable)[i];
            while (p != NULL) {
                if (strcmp(p->value, macro->directive) == 0) {
                    return i;
                }
                p = p->next;
            }
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("File must be inputted.\n");
        return INVALID_ARGS;
    }

    FILE *file = fopen(argv[1], "r");
    //FILE *file = fopen("C:\\Users\\begemot\\CLionProjects\\eda-dlya-begemota\\lab4\\files\\in4_1.txt", "r");
    if (file == NULL) {
        printf("Error opening file <%s>.\n", argv[1]);
        return ERROR_OPEN_INPUT;
    }

    FILE *temp = fopen("./temp.txt", "w");
    //FILE *temp = fopen("D:\\temp.txt", "w");
    if (temp == NULL) {
        printf("Error opening temporary file\n");
        return ERROR_OPEN_TEMP;
    }

    int TableSize = 128;
    struct Macro **HashTable = CreateTable(TableSize);
    if (HashTable == NULL) {
        printf("Error creating hash table\n");
        fclose(file);
        fclose(temp);
        return ERROR_CREATING_TABLE;
    }


    char line[BUFSIZ];
    while (1) {
        if (fgets(line, sizeof(line), file) == NULL) {
            break;
        }

        if (HasDefine(line)) {
            fprintf(temp, "%s", line);
            struct Macro *macro = GetMacro(line);
            if (macro == NULL) {
                printf("Error getting macro\n");
                fclose(file);
                fclose(temp);
                return ERROR_CREATING_MACRO;
            }
            int ret = HasSuchValue(&HashTable, TableSize, macro);
            if (ret != -1) {
                struct Macro *p = HashTable[ret];
                while (p != NULL) {
                    if (strcmp(p->value, macro->directive) == 0) {
                        strcpy(p->value, macro->value);
                    }
                    p = p->next;
                }
                free(macro->directive);
                free(macro->value);
                free(macro);
            } else {
                int hash = GetHash(macro->directive, TableSize) % TableSize;
                InsertMacro(&HashTable, hash, macro);

                if (ResizeNeed(&HashTable, TableSize)) {
                    TableSize += 100;
                    struct Macro **p = CreateTable(TableSize);
                    if (p == NULL) {
                        Clear(&HashTable, TableSize);
                        fclose(file);
                        fclose(temp);
                        printf("Error reallocating table.\n");
                        return ERROR_REALLOCATING_TABLE;
                    }
                    CopyTable(&HashTable, &p, TableSize);
                    HashTable = p;
                    printf("REALLOCATED\n");
                }
            }


        } else if (line[0] == '\n') {
            fprintf(temp, "%s", line);
        } else {
            break;
        }
    }
    PrintTable(&HashTable, TableSize);

    printf("\n----SECOND PART----\n\n");

    do {
        char * inLine = ProcessLine(line, &HashTable, TableSize);
        fprintf(temp, "%s\n", inLine);
        free(inLine);
    } while (fgets(line, sizeof(line), file) != NULL);


    //Clear
    Clear(&HashTable, TableSize);
    fclose(file);
    fclose(temp);

    remove(argv[1]);
    if (rename("./temp.txt", argv[1]) != 0) {
        printf("Error moving file\n");
    }

    printf("\nCORRECT FINISH\n");
    return 0;
}