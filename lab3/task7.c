#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    SUCCESS,
    WRONG_ARGUMENTS,
    FAILED_OPEN_FILE
};

struct Liver {
    char * surname;
    char * name;
    char * otchestvo;
    char * birthday;
    char * gender;
    double * salary;
};

//--------------------------------------------------LIST------------------------------------------------------
struct Node {
    struct Liver * liver;
    struct Node *next;
};

struct Node* Create(struct Liver * liver) {
    struct Node* ptr = NULL;
    ptr = (struct Node*)malloc(sizeof(struct Node));
    if (ptr == NULL) {
        printf("Memory allocation error while creating node.\n");
        return NULL;
    }
    ptr->liver = liver;
    ptr->next = NULL;
    return ptr;
}

void Insert(struct Node* pNode, struct Node* newNode) {
    newNode->next = pNode->next;
    pNode->next = newNode;
}

void PushFront(struct Node** pHead, struct Node* newNode) {
    newNode->next = *pHead;
    *pHead = newNode;
}

void delete(struct Node* pNode) {
    struct Node* ptr;
    ptr = pNode->next;
    pNode->next = ptr->next;
    free(ptr);
}
//--------------------------------------------------LIST------------------------------------------------------

int GetPersonInfo (FILE* file, char *surname, char *name, char *otchestvo, char *birthday, char *gender, double *salary) {
    char line[BUFSIZ];

    //name
    if (fgets(line, BUFSIZ, file) == NULL) {
        return 0;
    }
    int len = (int)strlen(line);
    surname = (char*)malloc(len + 1);
    line[len - 1] = '\0';
    strcpy(surname, line);


    //surname
    fgets(line, BUFSIZ, file);
    len = strlen(line);
    name = (char*)malloc(len + 1);
    line[len - 1] = '\0';
    strcpy(name, line);

    //otchestvo
    fgets(line, BUFSIZ, file);
    len = strlen(line);
    otchestvo = (char*)malloc(len + 1);
    line[len - 1] = '\0';
    strcpy(otchestvo, line);

    //birthday
    fgets(line, BUFSIZ, file);
    len = strlen(line);
    birthday = (char*)malloc(len + 1);
    line[len - 1] = '\0';
    strcpy(birthday, line);

    //gender
    fgets(line, BUFSIZ, file);
    len = strlen(line);
    line[len - 1] = '\0';
    gender = (char*)malloc(2);
    strcpy(gender, line);

    //salary
    fgets(line, BUFSIZ, file);
    len = strlen(line);
    char * salaryStr = (char*)malloc(len + 1);
    strcpy(salaryStr, line);
    double sal = atof(salaryStr);
    salary = (double*)malloc(sizeof(double));
    *salary = sal;
    return 1;
}

struct Liver * CreateLiver (char * surname, char * name, char * otchestvo, char * birthday, char * gender, double * salary) {
    struct Liver * liver = (struct Liver*)malloc(sizeof(struct Liver));
    if (liver == NULL) {
        printf("Memory allocation error while creating liver.\n");
        return NULL;
    }
    liver->surname = surname;
    liver->name = name;
    liver->otchestvo = otchestvo;
    liver->birthday = birthday;
    liver->gender = gender;
    liver->salary = salary;
    return liver;
}

int main(int argc ,char *argv[]) {
    // if (argc != 2) {
    //     printf("Wrong input.\nProvide full path to data file.\n");
    //     return WRONG_ARGUMENTS;
    // }
    //FILE* file = fopen(argv[1], "r");
    FILE* file = fopen("D:\\in3_7.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return FAILED_OPEN_FILE;
    }



    while (1) {
        char * surname, name, otchestvo, birthday, gender;
        double * salary = 0;
        printf("hui\n");
        int ret = GetPersonInfo(file, &surname, &name, &otchestvo, &birthday, &gender, &salary);
        if (ret == 0) {
            break;
        }
        printf("hui2\n");
        printf("%s\n", surname);
        struct Liver * liver = CreateLiver(surname, name, otchestvo, birthday, gender, salary);
        printf("%s\n%s\n%s\n%s\n%c\n%lf\n\n", liver->surname, liver->name, liver->otchestvo, liver->birthday, liver->gender, liver->salary);
    }


    return SUCCESS;
}