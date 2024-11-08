#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int toInt(const char* argv) {
    int res = 0;
    int i = 0;
    int neg_flag = 0;
    if (argv[0] == '-') {
        neg_flag = 1;
        i = 1;
    }
    while (argv[i] != '\0' && argv[i] != '\n') {
        res = res * 10 + argv[i] - '0';
        i++;
    }
    if (neg_flag) {
        res *= -1;
    }
    //printf("%d\n", res);
    return res;
}

enum {
    SUCCESS,
    MALLOC_FAILED,
    ID_TOO_LARGE,
    MARK_TOO_LARGE,
    NAME_TOO_LARGE,
    SURNAME_TOO_LARGE,
    GROUP_TOO_LARGE,
    INVALID_ID,
    INVALID_NAME,
    INVALID_STUDENT_DATA,
    TO_STR_FAIL,
    FILE_WRITE_FAIL,
    NOT_A_PATH
} CODES;

#define ID 0
#define NAME 1
#define SURNAME 2
#define GROUP 3
#define MARKS 4

void ValidateCode(const int code) {
    switch (code) {
        case MALLOC_FAILED: printf("Memory allocation failed.\n"); break;
        case ID_TOO_LARGE: printf("Id is too large.\n"); break;
        case MARK_TOO_LARGE: printf("Mark is too large.\n"); break;
        case NAME_TOO_LARGE: printf("Name is too large.\n"); break;
        case SURNAME_TOO_LARGE: printf("Surname is too large.\n"); break;
        case GROUP_TOO_LARGE: printf("Group is too large.\n"); break;
        case INVALID_ID: printf("Id is not valid.\n"); break;
        case INVALID_NAME: printf("Name or surname is invalid.\n"); break;
        case INVALID_STUDENT_DATA: printf("Information about student is invalid.\n"); break;
        case SUCCESS: break;
        case TO_STR_FAIL: printf("Failed to transform to string.\n"); break;
        case FILE_WRITE_FAIL: printf("Failed to write to a file.\n"); break;
        case NOT_A_PATH: printf("Value is not a path.\n"); break;
        default: printf("Unknown error code.\n"); break;
    }
}

int path_checker(char* path) {
    char* ptr = path + strlen(path);
    while (*ptr != '.') {
        --ptr;
    }
    ++ptr;
    if(strcmp(ptr, "txt") != 0) {
        return NOT_A_PATH;
    }
    while (isalpha(*ptr) || isdigit(*ptr) || *ptr == '-'
           || *ptr == '_' || *ptr == '.' || *ptr == '\\') {
        --ptr;
           }
    if (*ptr != ':' || !isupper(*(ptr - 1)) || *(ptr + 1) != '\\') {
        return NOT_A_PATH;
    }
    return SUCCESS;
}

struct Student {
    unsigned int id;
    char group[100], surname[100], name[100];
    unsigned char* marks;
};

struct SelectedStudents {
    struct Student* selected_students;
    int size;
};

int AddElemToStudent(int pos, struct Student* Students, int student_Ind, char curElem[100]) {
    if (pos == ID) {
        int tek_id = toInt(curElem);
        Students[student_Ind].id = tek_id;
    }
    if (pos == NAME) {
        for (int i = 0 ; i < 100; i++) {
            Students[student_Ind].name[i] = curElem[i];
        }
    }
    if (pos == SURNAME) {
        for (int i = 0 ; i < 100; i++) {
            Students[student_Ind].surname[i] = curElem[i];
        }
    }
    if (pos == GROUP) {
        for (int i = 0 ; i < 100; i++) {
            Students[student_Ind].group[i] = curElem[i];
        }
    }
    if (pos >= MARKS && pos < MARKS + 5) {
        Students[student_Ind].marks[pos - MARKS] = curElem[0];
    }
    return SUCCESS;
}

int ValidateID(char ch) {
    if (!isdigit(ch)) {
        return INVALID_ID;
    }
    return SUCCESS;
}

int ValidateName(char ch) {
    if (!isalpha(ch)) {
        return INVALID_NAME;
    }
    return SUCCESS;
}

int ValidateStrLen(int len, int pos) {
    if  (len > 9 && pos == 0) {
        return ID_TOO_LARGE;
    }
    if (len > 99 && pos == 1) {
        return NAME_TOO_LARGE;
    }
    if (len > 99 && pos == 2) {
        return SURNAME_TOO_LARGE;
    }
    if (len > 99 && pos == 3) {
        return GROUP_TOO_LARGE;
    }
    if (len > 1 && pos > 3) {
        return MARK_TOO_LARGE;
    }
    return SUCCESS;
}

int ValidateCurStr(const char CurStr[], struct Student* Students, int student_Ind) {
    int ind = 0, pos = 0;
    char ch = CurStr[ind];
    Students[student_Ind].marks = (unsigned char*)malloc(sizeof(unsigned char) * 5);
    if (Students[student_Ind].marks == NULL) {
        return MALLOC_FAILED;
    }
    while(ch != '\n' && ch != '\0' && ch != EOF) {
        if (pos > 8) {
            return INVALID_STUDENT_DATA;
        }
        while (ch == ' ' || ch == '\t') {
            ch = CurStr[++ind];
        }
        char curElem[100];
        int len = 0;
        while(ch != ' ' && ch != '\t' && ch != '\n' && ch != EOF && ch != '\0') {
            len++;
            int ret_code = ValidateStrLen(len, pos);
            if (ret_code != SUCCESS) {
                return ret_code;
            }
            if (pos == ID || pos >= MARKS) {
                ret_code = ValidateID(ch);
                if (ret_code != SUCCESS) {
                    return ret_code;
                }
            }
            if (pos == NAME || pos == SURNAME) {
                ret_code = ValidateName(ch);
                if (ret_code != SUCCESS) {
                    return ret_code;
                }
            }
            curElem[len - 1] = ch;
            ch = CurStr[++ind];
        }
        curElem[len] = '\0';
        AddElemToStudent(pos, Students, student_Ind, curElem);
        pos++;
        while (ch == ' ' || ch == '\t') {
            ch = CurStr[++ind];
        }
    }
    return SUCCESS;
}

int find_id(struct Student* Student, char id[100]) {
    if (Student->id == toInt(id)) {
        return 1;
    }
    return 0;
}

int find_name(struct Student* Student, char name[100]) {
    if (strcmp(Student->name, name) == 0) {
        return 1;
    }
    return 0;
}

int find_surname(struct Student* Student, char surname[100]) {
    if (strcmp(Student->surname, surname) == 0) {
        return 1;
    }
    return 0;
}

int find_group(struct Student* Student, char group[100]) {
    if (strcmp(Student->group, group) == 0) {
        return 1;
    }
    return 0;
}

int WriteStudents(struct Student* selected_students, int size, FILE *fileOut, char findCondition[8]) {
    for (int i = 0; i < size; i++) {
        char curStudent[500];
        if (strcmp(findCondition, "id") != 0) {
            if (snprintf(curStudent, 500, "%d %s %s %s %c %c %c %c %c\n", selected_students[i].id, selected_students[i].name, selected_students[i].surname, selected_students[i].group, selected_students[i].marks[0],
                    selected_students[i].marks[1], selected_students[i].marks[2], selected_students[i].marks[3], selected_students[i].marks[4]) < 0) {
                return TO_STR_FAIL;
            }
        }
        else {
            if (snprintf(curStudent, 500, "%d %s %s %s %f\n", selected_students[i].id, selected_students[i].name, selected_students[i].surname, selected_students[i].group,
                (selected_students[i].marks[0] - '0' + selected_students[i].marks[1] - '0' + selected_students[i].marks[2] - '0' + selected_students[i].marks[3] - '0' + selected_students[i].marks[4] - '0') / 5.0) < 0) {
                return TO_STR_FAIL;
            }
        }
        if (fputs(curStudent, fileOut) < 0) {
            return FILE_WRITE_FAIL;
        }
    }
    return SUCCESS;
}

struct SelectedStudents find(int(*funс)(struct Student Student, char[100]), int number_of_students, struct Student* Students, char el[100]) {
    int cnt_founded_students = 1;
    struct SelectedStudents res;
    struct Student* selected_students = malloc(sizeof(struct Student) * cnt_founded_students);
    int founded_students_ind = 0;
    if (selected_students == NULL) {
        ValidateCode(MALLOC_FAILED);
        res.selected_students = NULL;
        res.size = 0;
        return res;
    }
    for (int i = 0; i < number_of_students; i++) {
        if (founded_students_ind == cnt_founded_students) {
            cnt_founded_students *= 2;
            struct Student* new_founded_students = realloc(selected_students, sizeof(struct Student) * cnt_founded_students);
            if (new_founded_students == NULL) {
                ValidateCode(MALLOC_FAILED);
                free(selected_students);
                res.selected_students = NULL;
                res.size = 0;
                return res;
            }
            selected_students = new_founded_students;
        }
        if (funс(Students[i], el) == 1) {
            selected_students[founded_students_ind++] = Students[i];
        }
    }
    if (founded_students_ind == 0) {
        free(selected_students);
        printf("No one has been found\n");
        res.selected_students = NULL;
        res.size = -1;
        return res;
    }
    res.selected_students = selected_students;
    res.size = founded_students_ind;
    return res;
}

int IdComparator(const void* x1, const void* x2) {
    unsigned int id_1 = ((struct Student*)x1)->id;
    unsigned int id_2 = ((struct Student*)x2)->id;
    if (id_1 < id_2) {
        return -1;
    }
    if (id_1 > id_2) {
        return 1;
    }
    return 0;
}

int NameComparator(const void* y1, const void* y2) {
    struct Student* x1 = (struct Student*)y1;
    struct Student* x2 = (struct Student*)y2;
    int res = strcmp(x1->name, x2->name);
    return res;
}

int SurnameComparator(const void* y1, const void* y2) {
    struct Student* x1 = (struct Student*)y1;
    struct Student* x2 = (struct Student*)y2;
    int res = strcmp(x1->surname, x2->surname);
    return res;
}

int GroupComparator(const void* y1, const void* y2) {
    struct Student* x1 = (struct Student*)y1;
    struct Student* x2 = (struct Student*)y2;
    int res = strcmp(x1->group, x2->group);
    return res;
}

void Clear(struct Student* Students, int student_Ind, FILE* fileIn) {
    for (int i = 0; i <= student_Ind; i++) {
        free(Students[i].marks);
    }
    free(Students);
    fclose(fileIn);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments.\n");
        return -1;
    }
    int code;
    if ((code = path_checker(argv[2])) != SUCCESS) {
        ValidateCode(code);
        return -1;
    }
    int number_of_students = 5;
    FILE* fileIn = fopen(argv[1], "r");
    if (fileIn == NULL) {
        fclose(fileIn);
        printf("Error opening file.\n");
        return -1;
    }
    struct Student* Students = malloc(sizeof(struct Student) * number_of_students);
    if (Students == NULL) {
        ValidateCode(MALLOC_FAILED);
        fclose(fileIn);
        return -1;
    }
    double AVG_MARK = 0.0;
    int student_Ind = 0;
    char CurStr[500];
    while(fgets(CurStr, 500, fileIn) != NULL) {
        int ret_code = ValidateCurStr(CurStr, Students, student_Ind);
        if (ret_code != SUCCESS) {
            ValidateCode(ret_code);
            Clear(Students, student_Ind, fileIn);
            return -1;
        }
        CurStr[499] = '\0';
        if (student_Ind + 1 == number_of_students) {
            number_of_students *= 2;
            struct Student* new_students = realloc(Students, sizeof(struct Student) * number_of_students);
            if (new_students == NULL) {
                ValidateCode(MALLOC_FAILED);
                Clear(Students, student_Ind, fileIn);
                return -1;
            }
            Students = new_students;
        }

        AVG_MARK += (Students[student_Ind].marks[0] - '0' + Students[student_Ind].marks[1] - '0' + Students[student_Ind].marks[2] - '0' + Students[student_Ind].marks[3] - '0' + Students[student_Ind].marks[4] - '0') / 5.0;
        student_Ind++;
    }
    number_of_students = student_Ind;
    student_Ind--;
    AVG_MARK /= number_of_students;
    struct SelectedStudents find_res;
    struct Student* res_students = Students;
    int size_of_founded_students = number_of_students;
    char findCondition[8];
    printf("Find using\nEnter one of variants (id | name | surname | group | no): ");
    scanf("%s", findCondition);
    if (strcmp(findCondition, "id") == 0) {
        char id[10];
        printf("print id: ");
        scanf("%9s", id);
        find_res = find(find_id, number_of_students, Students, id);
    }
    else if(strcmp(findCondition, "name") == 0) {
        char name[100];
        printf("print name: ");
        scanf("%99s", name);
        find_res = find(find_name, number_of_students, Students, name);
    }
    else if(strcmp(findCondition, "surname") == 0) {
        char surname[100];
        printf("print surname: ");
        scanf("%99s", surname);
        find_res = find(find_surname, number_of_students, Students, surname);
    }
    else if(strcmp(findCondition, "group") == 0) {
        char group[100];
        printf("print group: ");
        scanf("%99s", group);
        find_res = find(find_group, number_of_students, Students, group);
    }
    else if(strcmp(findCondition, "no") == 0) {}
    else {
        printf("error: unknown command");
        Clear(Students, student_Ind, fileIn);
        return -1;
    }
    if (strcmp(findCondition, "no") != 0) {
        res_students = find_res.selected_students;
        size_of_founded_students = find_res.size;
        if (res_students == NULL && size_of_founded_students == 0) {
            Clear(Students, student_Ind, fileIn);
            return -1;
        }
    }
    char sortCondition[8];
    printf("Sort by\nEnter one of variants (id | name | surname | group | no): ");
    scanf("%s", sortCondition);
    if (strcmp(sortCondition, "id") == 0) {
        qsort(res_students, size_of_founded_students, sizeof(struct Student), IdComparator);
    }
    else if(strcmp(sortCondition, "name") == 0) {
        qsort(res_students, size_of_founded_students, sizeof(struct Student), NameComparator);
    }
    else if(strcmp(sortCondition, "surname") == 0) {
        qsort(res_students, size_of_founded_students, sizeof(struct Student), SurnameComparator);
    }
    else if(strcmp(sortCondition, "group") == 0) {
        qsort(res_students, size_of_founded_students, sizeof(struct Student), GroupComparator);
    }
    else if(strcmp(sortCondition, "no") == 0) {}
    else {
        printf("error: unknown command");
        Clear(Students, student_Ind, fileIn);
        return -1;
    }
    FILE *fileOut = fopen(argv[2], "w");
    if (fileOut == NULL) {
        Clear(Students, student_Ind, fileIn);
        printf("No such file");
        return -1;
    }
    int res_writing_to_a_file = WriteStudents(res_students, size_of_founded_students, fileOut, findCondition);
    if (res_writing_to_a_file != SUCCESS) {
        ValidateCode(res_writing_to_a_file);
        Clear(Students, student_Ind, fileIn);
        fclose(fileOut);
        return -1;
    }
    char answer[2];
    printf("Do you need students that have average estimate above average estimate of all students\nYour answer (y \\ n): ");
    scanf("%s", answer);
    if (strcmp(answer, "y") == 0) {
        fputs("Students that have average estimate above average estimate of all students:\n", fileOut);
        for(int i = 0; i < number_of_students; i++) {
            double CurAvgMark = (Students[i].marks[0] - '0' + Students[i].marks[1] - '0' + Students[i].marks[2] - '0' + Students[i].marks[3] - '0' + Students[i].marks[4] - '0') / 5.0;
            if (CurAvgMark > AVG_MARK) {
                char curStudent[202];
                snprintf(curStudent, 201, "%s %s\n", Students[i].name, Students[i].surname);
                fputs(curStudent, fileOut);
            }
        }
        printf("Done");
        Clear(Students, student_Ind, fileIn);
        fclose(fileOut);
        return 0;
    }
    if (!strcmp(answer, "n")) {
        printf("Done");
        Clear(Students, student_Ind, fileIn);
        fclose(fileOut);
        return 0;
    }
    printf("error: unknown command");
    Clear(Students, student_Ind, fileIn);
    fclose(fileOut);
    return -1;
}