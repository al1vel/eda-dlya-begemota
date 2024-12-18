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

struct Macro {
    char *directive;
    char *value;
    struct Macro *next;
};

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


struct HashTable {
    struct Macro ** HashTable;
    int size;
};

struct HashTable * CreateTable(int size) {
    struct HashTable *ht = (struct HashTable *)malloc(sizeof(struct HashTable));
    ht->HashTable = (struct Macro**)malloc(sizeof(struct Macro*) * size);
    if (ht->HashTable == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        ht->HashTable[i] = NULL;
    }
    ht->size = size;
    return ht;
}

void InsertMacro (struct HashTable * ht, int hash, struct Macro *newMacro) {
    if (ht->HashTable[hash] == NULL) {
        ht->HashTable[hash] = newMacro;
        printf("Dir: <%s>. Val: <%s>. HASH: %d\n", newMacro->directive, newMacro->value, hash);
    } else {
        struct Macro *ptr = ht->HashTable[hash];
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = newMacro;
        printf("XD Dir: <%s>. Val: <%s>. HASH: %d\n", newMacro->directive, newMacro->value, hash);
    }
}

int HasSuchValue (struct HashTable *ht, struct Macro * macro) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->HashTable[i] != NULL) {
            struct Macro *p = ht->HashTable[i];
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

int ResizeNeed(struct HashTable * ht) {
    int curChain = 0, maxChain = 0, minChain = 10000;

    for (int i = 0; i < ht->size; i++) {
        if (ht->HashTable[i] != NULL) {
            struct Macro *current = ht->HashTable[i];
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

void CopyTable(struct HashTable *old, struct HashTable * new) {
    for (int j = 0; j < old->size; ++j) {
        if (old->HashTable[j] != NULL) {
            struct Macro *tmp = old->HashTable[j];
            while (tmp != NULL) {
                printf("D: %s; V: %s\n", tmp->directive, tmp->value);
                int hash = GetHash(tmp->directive, new->size);
                printf("Dir: <%s>. New hash: %d\n", tmp->directive, hash);
                if (new->HashTable[hash] == NULL) {
                    new->HashTable[hash] = tmp;

                    tmp = new->HashTable[hash]->next;
                    new->HashTable[hash]->next = NULL;
                } else {
                    struct Macro *pointer = new->HashTable[hash];
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
    //free(old->HashTable);
}

void Clear(struct HashTable * ht) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->HashTable[i] != NULL) {
            struct Macro *current = ht->HashTable[i];
            while (current != NULL) {
                struct Macro *temp = current;
                current = current->next;
                free(temp->directive);
                free(temp->value);
                free(temp);
            }
            free(current);
            ht->HashTable[i] = NULL;
        }
    }
    free(ht->HashTable);
    ht->HashTable = NULL;
}

void PrintTable(struct HashTable * ht) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->HashTable[i] != NULL) {
            printf("%d | Dir: <%s> | Val: <%s>\n", i, ht->HashTable[i]->directive, ht->HashTable[i]->value);
            struct Macro *ptr = ht->HashTable[i]->next;
            while (ptr != NULL) {
                printf("%d next | Dir: <%s> | Val: <%s>\n", i, ptr->directive, ptr->value);
                ptr = ptr->next;
            }
        }
    }
}


char * ProcessLine (char * line, struct HashTable * ht) {
    char * ptr = line;
    char buf[BUFSIZ];
    int i = 0, fileInd = 0;

    char * inFile = (char *)malloc(BUFSIZ * sizeof(char));

    while (*ptr != '\0') {
        if (isalnum(*ptr)) {
            buf[i] = *ptr;
            i++;
        } else {
            buf[i] = '\0';
            i = 0;
            //printf("%s\n", buf);
            int hash = GetHash(buf, ht->size);
            if (ht->HashTable[hash] != NULL) {
                struct Macro *p = ht->HashTable[hash];
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
    printf("%s", inFile);
    return inFile;
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
    //FILE *temp = fopen("C:\\Users\\begemot\\temp.txt", "w");
    if (temp == NULL) {
        printf("Error opening temporary file\n");
        return ERROR_OPEN_TEMP;
    }

    struct HashTable *table = CreateTable(128);
    if (table == NULL) {
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
            int ret = HasSuchValue(table, macro);
            if (ret != -1) {
                struct Macro *p = table->HashTable[ret];
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
                int hash = GetHash(macro->directive, table->size);
                InsertMacro(table, hash, macro);

                if (ResizeNeed(table)) {
                    int new_size = table->size + 100;
                    struct HashTable *newTable = CreateTable(new_size);
                    if (newTable == NULL) {
                        Clear(table);
                        fclose(file);
                        fclose(temp);
                        printf("Error reallocating table.\n");
                        return ERROR_REALLOCATING_TABLE;
                    }
                    CopyTable(table, newTable);
                    free(table);
                    table = newTable;
                    printf("REALLOCATED\n");
                }
            }


        } else if (line[0] == '\n') {
            fprintf(temp, "%s", line);
        } else {
            break;
        }
    }
    PrintTable(table);

    printf("\n----SECOND PART----\n\n");

    do {
        char * inLine = ProcessLine(line, table);
        fprintf(temp, "%s", inLine);
        free(inLine);
    } while (fgets(line, sizeof(line), file) != NULL);


    //Clear
    Clear(table);
    fclose(file);
    fclose(temp);

    remove(argv[1]);
    if (rename("./temp.txt", argv[1]) != 0) {
        printf("Error moving file\n");
    }

    printf("\nCORRECT FINISH\n");
    return 0;
}