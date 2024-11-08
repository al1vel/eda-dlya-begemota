#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum ret_type_t {
    SUCCESS,
    ERROR_WRONG_NUMB_OF_ARGS,
    NOT_A_STR,
    NULL_NUM,
    NOT_A_NUM,
    NUM_TOO_LONG,
    NEG_NUM,
    NOT_PATH,
    FILE_OPENING_ERROR,
    ERROR_SAME_IN_AND_OUT_FILES,
    MEMORY_ALLOCATION_ERROR,
    FILE_ENDED
};

void ValidateCode(int code) {
    switch (code) {
        case NEG_NUM: printf("Negative value was entered.\n"); break;
        case NULL_NUM: printf("Not number was entered.\n"); break;
        case NOT_A_NUM: printf("Value is not a number.\n"); break;
        case NUM_TOO_LONG: printf("Too long string was entered.\n"); break;
        case FILE_OPENING_ERROR: printf("File opening error.\n"); break;
        case NOT_PATH: printf("Value is not a path.\n"); break;
        case ERROR_SAME_IN_AND_OUT_FILES: printf("Input and outfile files can not be equal.\n"); break;
        case MEMORY_ALLOCATION_ERROR: printf("Memory allocation error.\n"); break;
        case NOT_A_STR: printf("String can not be name or surname.\n"); break;
        default: break;
    }
}

typedef int(*callback)(char*);

struct Employee {
    unsigned int id;
    char* name;
    char* surname;
    float salary;
    char* salary_str;
};

int is_number(const char* s) {
    if (*s == '\0') return NULL_NUM;
    while (*s == ' ') s++;
    if (*s == '-') {
        return NEG_NUM;
    }
    int len = 0;
    while (isdigit(*s)) {
        s++;
        if (len++ > 18) return NUM_TOO_LONG;
    }
    if (*s == '\0') return SUCCESS;
    return NOT_A_NUM;
}

int is_float(const char* s) {
    if (*s == '\0') return NULL_NUM;

    while (*s == ' ') s++;
    if (*s == '-') {
        return NEG_NUM;
    }
    int len = 0;
    int point_was = 0;
    while (isdigit(*s) || (*s == '.' && !point_was)) {
        if (*s == '.')
            point_was += 1;
        s++;
        if (len++ > 20)
            return NUM_TOO_LONG;
    }

    if (*s == '\0') return SUCCESS;
    return NOT_A_NUM;
}

int is_str(const char* s) {
    while (*s) {
        if (!isalpha(*s)) {
            printf("huinya: <%c>\n", *s);
            return NOT_A_STR;
        }
        s++;
    }
    return SUCCESS;
}

int path_checker(char* path) {
    char* ptr = path + strlen(path);
    while (*ptr != '.') {
        --ptr;
    }
    ++ptr;
    if(strcmp(ptr, "txt") != 0) {
        return NOT_PATH;
    }
    while (isalpha(*ptr) || isdigit(*ptr) || *ptr == '-'
           || *ptr == '_' || *ptr == '.' || *ptr == '\\') {
        --ptr;
    }
    if (*ptr != ':' || !isupper(*(ptr - 1)) || *(ptr + 1) != '\\') {
        return NOT_PATH;
    }
    return SUCCESS;
}

unsigned int StrToUint(const char* s) {
    while (*s == ' ') s++;
    unsigned int res = 0;
    while (*s != '\0') {
        res = (res * 10) + (*s - '0');
        s++;
    }
    return res;
}

int newEmployee(FILE *infile, struct Employee* emp) {
    char curSym = '\0';
    char *data = (char*)malloc(sizeof(char));
    int data_ind = 0;
    int data_type = 0;
    while (curSym != EOF && curSym != '\n') {
        curSym = (char)fgetc(infile);
        //printf("char <%c>\n", curSym);
        if (curSym == EOF && data_type != 3) {
            return FILE_ENDED;
        }
        if (curSym != '\n' && curSym != ' ' && curSym != EOF) {
            //printf("<%c> ", curSym);
            data[data_ind] = curSym;
            //printf("data: %c\n", data[data_ind]);
            data_ind++;
            if (data_ind >= strlen(data)) {
                char *ptr = (char *)realloc(data, 2 * (int)sizeof(data) * sizeof(char));
                if (ptr == NULL) {
                    free(data);
                    return MEMORY_ALLOCATION_ERROR;
                }
                data = ptr;
            }
        } else {
            if (!data_ind) {
                continue;
            }
            data[data_ind] = '\0';
            //printf("DATA S 0 <%s> ", data);
            int code;
            switch (data_type) {
                case 0:
                    if ((code = is_number(data))) {
                        free(data);
                        return code;
                    }
                    emp->id = StrToUint(data);
                    break;
                case 1:
                    if ((code = is_str(data)) != SUCCESS) {
                        //printf("name\n");
                        free(data);
                        return code;
                    }
                    emp->name = (char*)malloc(sizeof(char) * strlen(data));
                    strcpy(emp->name, data);
                    //printf("Name: <%s>\n", emp->name);
                    break;
                case 2:
                    if ((code = is_str(data)) != SUCCESS) {
                        //printf("surname: <%s>\n", data);
                        free(data);
                        return code;
                    }
                    emp->surname = (char*)malloc(sizeof(char) * strlen(data));
                    strcpy(emp->surname, data);
                    break;
                case 3:
                    if ((code = is_float(data))) {
                        free(data);
                        return code;
                    }
                    emp->salary = atof(data);
                    emp->salary_str = (char*)malloc(sizeof(char) * strlen(data));
                    strcpy(emp->salary_str, data);
                    break;
                default:
                    printf("Something went wrong!\n");
                    break;
            }
            data_type++;
            data_ind = 0;
        }
    }
    free(data);
    if (data_type != 4) {
        printf("Wrong number of arguments.");
        return ERROR_WRONG_NUMB_OF_ARGS;
    }
    return SUCCESS;
}

int comporatorD(const void* y1, const void* y2) {
    struct Employee* x1 = (struct Employee*)y1;
    struct Employee* x2 = (struct Employee*)y2;
    int res;
    if (strcmp(x1->salary_str, x2->salary_str) < 0) {
        return 1;
    } else if (strcmp(x1->salary_str, x2->salary_str) > 0) {
        return -1;
    } else {
        if ((res = strcmp(x1->surname, x2->surname)) != 0) {
            return res;
        }
        if ((res = strcmp(x1->name, x2->name)) != 0) {
            return res;
        }
        if (x1->id < x2->id) {
            //printf("hui\n");
            return 1;
        } else if (x1->id > x2->id) {
            //printf("hui2\n");
            return -1;
        }
        return 0;
    }
}

int comporatorA(const void* y1, const void* y2) {
    struct Employee* x1 = (struct Employee*)y1;
    struct Employee* x2 = (struct Employee*)y2;
    int res;
    if (strcmp(x1->salary_str, x2->salary_str) < 0) {
        return -1;
    } else if (strcmp(x1->salary_str, x2->salary_str) > 0) {
        return 1;
    } else {
        if ((res = strcmp(x1->surname, x2->surname)) != 0) {
            return res;
        }
        if ((res = strcmp(x1->name, x2->name)) != 0) {
            return res;
        }
        if (x1->id < x2->id) {
            return -1;
        } else if (x1->id > x2->id) {
            return 1;
        }
        return 0;
    }
}

int findFlag(char* fl, const char** flags, int size) {
    for (int i = 0; i < size; ++i) {
        if (!(strcmp(fl, flags[i]))) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments.\n");
        return -1;
    }

    if (path_checker(argv[1]) || path_checker(argv[3])) {
        ValidateCode(NOT_PATH);
        return -1;
    }

    if (!strcmp(argv[1], argv[3])) {
        ValidateCode(ERROR_SAME_IN_AND_OUT_FILES);
        return -1;
    }

    FILE *infile = fopen(argv[1], "r");
    FILE *outfile = fopen(argv[3], "w");

    if (infile == NULL || outfile == NULL) {
        ValidateCode(FILE_OPENING_ERROR);
        return -1;
    }

    struct Employee *emp = (struct Employee*)malloc(sizeof(struct Employee));
    int data_size = 1;
    struct Employee prom_data;
    int ind = 0, code;
    while ((code = newEmployee(infile, &prom_data)) != FILE_ENDED) {
        if (code) {
            for (int i = 0; i < ind; ++i) {
                free(emp[i].name);
                free(emp[i].surname);
                free(emp[i].salary_str);
            }
            free(emp);
            ValidateCode(code);
            return -1;
        }

        if (ind >= data_size) {
            data_size *= 2;
            struct Employee *ptr;
            ptr = (struct Employee *) realloc(emp, data_size * sizeof(struct Employee));
            if (ptr == NULL) {
                for (int i = 0; i < ind; ++i) {
                    free(emp[i].name);
                    free(emp[i].surname);
                    free(emp[i].salary_str);
                }
                free(emp);
                ValidateCode(MEMORY_ALLOCATION_ERROR);
                return -1;
            }
            emp = ptr;
        }
        emp[ind] = prom_data;
        ++ind;
    }

    const char *flags[] = { "-a", "/a", "-d", "/d"};
    int ret = findFlag(argv[2], flags, sizeof(flags) / sizeof(char*));
    if (ret == -1) {
        printf("Flag <%s> is not supported!\n", argv[2]);
        return -1;
    }
    if (ret < 2) {
        qsort(emp, ind, sizeof(struct Employee), comporatorA);
    } else {
        qsort(emp, ind, sizeof(struct Employee), comporatorD);
    }
    for (int i = 0; i < ind; ++i) {
        fprintf(outfile, "| Id: %-3u| Name: %-10s| Surname: %-10s| Salary: %-7.3f|\n", emp[i].id, emp[i].name, emp[i].surname, emp[i].salary);
    }

    printf("Answer in file: %s\n", argv[3]);

    fclose(infile);
    fclose(outfile);

    for (int i = 0; i < ind; ++i) {
        free(emp[i].name);
        free(emp[i].surname);
        free(emp[i].salary_str);
    }
    free(emp);
    return 0;
}