#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
            (*HashTable)[i] = NULL;
            //printf("%d FREE--\n", i);
        }
        //printf("%d null\n", i);
    }
    free(*HashTable);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("File must be inputted.\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file <%s>.\n", argv[1]);
        return -1;
    }

    FILE *temp = fopen("C:\\begemotik\\temp.txt", "w");
    if (temp == NULL) {
        printf("Error opening temporary file\n");
        return -1;
    }

    int TableSize = 128;
    //struct Macro* HashTable[TableSize];
    struct Macro **HashTable = (struct Macro**)malloc(sizeof(struct Macro*) * TableSize);
    for (int i = 0; i < TableSize; i++) {
        HashTable[i] = NULL;
    }

    int cont = 1, debugCnt = 0, maxChain = 0, curChain = 0, minChain = 1000000;
    char line[BUFSIZ];
    while (cont) {
        if (fgets(line, sizeof(line), file) == NULL) {
            cont = 0;
        }

        debugCnt++;
        //printf("DC: %d\n", debugCnt);
        int len = (int)strlen(line);
        if (len > 11) {
            char word[8];
            for (int i = 0; i < 7; ++i) {
                word[i] = line[i];
            }
            word[7] = '\0';
            if (strcmp(word, "#define") == 0) {
                //printf("hui: %d\n", debugCnt);
                fprintf(temp, "%s", line);
                char *directive = (char *)malloc(len);
                char *value = (char *)malloc(len);
                int i = 8, ind = 0;
                while (line[i] != ' ') {
                    //printf("<%c> ", line[i]);
                    directive[ind] = line[i];
                    i++;
                    ind++;
                }
                directive[ind] = '\0';
                ind = 0;
                i++;
                while (line[i] != '\n') {
                    value[ind] = line[i];
                    i++;
                    ind++;
                }
                value[ind] = '\0';
                //printf("Dir: <%s>. Val: <%s>.\n", directive, value);
                int hash = GetHash(directive, TableSize) % TableSize;
                //printf("Hash: %d\n", hash);

                struct Macro* curMacro = malloc(sizeof(struct Macro));
                curMacro->directive = (char *)malloc(len);
                strcpy(curMacro->directive, directive);
                free(directive);
                curMacro->value = (char *)malloc(len);
                strcpy(curMacro->value, value);
                free(value);
                curMacro->next = NULL;

                if (HashTable[hash] == NULL) {
                    HashTable[hash] = curMacro;
                    printf("Dir: <%s>. Val: <%s>. HASH: %d\n", directive, value, hash);
                } else {
                    curChain = 1;
                    struct Macro *ptr = HashTable[hash];
                    while (ptr->next != NULL) {
                        ptr = ptr->next;
                        curChain++;
                        //printf("Chain: %d\n", curChain);
                    }
                    ptr->next = curMacro;
                    printf("Dir: <%s>. Val: <%s>. HASH: %d\n", directive, value, hash);
                    if (curChain > maxChain) {
                        maxChain = curChain;
                    }
                    if (curChain < minChain) {
                        minChain = curChain;
                    }
                    if ((maxChain / minChain) >= 2) {
                        TableSize += 100;
                        struct Macro **p = malloc(sizeof(struct Macro*) * TableSize);
                        if (p == NULL) {
                            Clear(&HashTable, TableSize);
                            printf("Error reallocating memory.\n");
                            return -1;
                        }
                        for (int j = 0; j < TableSize; j++) {
                            p[j] = NULL;
                        }
                        for (int j = 0; j < (TableSize - 100); ++j) {
                            if (HashTable[j] != NULL) {
                                struct Macro *tmp = HashTable[j];
                                while (tmp != NULL) {
                                    printf("D: %s; V: %s\n", tmp->directive, tmp->value);
                                    hash = GetHash(tmp->directive, TableSize) % TableSize;
                                    printf("Dir: <%s>. New hash: %d\n", tmp->directive, hash);
                                    if (p[hash] == NULL) {
                                        p[hash] = tmp;
                                        p[hash]->next = NULL;
                                    } else {
                                        struct Macro *pointer = p[hash];
                                        while (pointer->next != NULL) {
                                            pointer = pointer->next;
                                        }
                                        pointer->next = tmp;
                                        pointer->next->next = NULL;
                                    }
                                    tmp = tmp->next;
                                }
                            }
                        }
                        HashTable = p;
                        printf("REALLOCATED\n");
                    }

                }
            } else {
                cont = 0;
            }
        } else if (line[0] == '\n') {
            fprintf(temp, "%s", line);
            continue;
        } else {
            cont = 0;
        }

    }

    for (int i = 0; i < TableSize; i++) {
        if (HashTable[i] != NULL) {
            printf("%d | Dir: <%s> | Val: <%s>\n", i, HashTable[i]->directive, HashTable[i]->value);
            struct Macro *ptr = HashTable[i]->next;
            while (ptr != NULL) {
                printf("%d next | Dir: <%s> | Val: <%s>\n", i, ptr->directive, ptr->value);
                ptr = ptr->next;
            }
        }
    }
    printf("MaxChain: %d; MinChain: %d\n", maxChain, minChain);

    printf("\n----SECOND PART----\n\n");

    do {
        char buf[BUFSIZ];
        char inFileBuf[BUFSIZ];
        int ind = 0, fileInd = 0;
        for (int i = 0; i < strlen(line) + 1; i++) {
            if (line[i] == '\n' || line[i] == '\0' || line[i] == ' ') {
                buf[ind] = '\0';
                int hash = GetHash(buf, TableSize) % TableSize;
                printf("Hash: %d. Word: <%s>\n", hash, buf);
                ind = 0;

                if (HashTable[hash] != NULL) {
                    struct Macro *ptr = HashTable[hash];
                    while (ptr != NULL) {
                        if (strcmp(ptr->directive, buf) == 0) {
                            char *val = ptr->value;
                            while (*val != '\0') {
                                inFileBuf[fileInd] = *val;
                                fileInd++;
                                val++;
                            }
                            inFileBuf[fileInd] = ' ';
                            fileInd++;
                        }
                        ptr = ptr->next;
                    }
                } else {
                    for (int j = 0; j < strlen(buf); j++) {
                        inFileBuf[fileInd] = buf[j];
                        fileInd++;
                    }
                    inFileBuf[fileInd] = ' ';
                    fileInd++;
                }
            } else {
                buf[ind] = line[i];
                ind++;
            }
            if (line[i] == '\n' || line[i] == '\0') {
                inFileBuf[fileInd] = '\n';
                fileInd++;
                inFileBuf[fileInd] = '\0';
                fprintf(temp, "%s", inFileBuf);
                break;
            }
        }
    } while (fgets(line, sizeof(line), file) != NULL);


    //Clear
    printf("cl1\n");
    Clear(&HashTable, TableSize);
    printf("cl2\n");

    fclose(file);
    fclose(temp);
    printf("\n\nCORRECT FINISH\n");
    return 0;
}