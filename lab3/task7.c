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
    char gender;
    double salary;
};

char * GetReversedDate(char * date) {
    char * revDate = (char*)malloc(sizeof(char) * 11);
    printf("Allocated in GetReversedDate\n");
    revDate[10] = '\0';
    for (int i = 0; i < 4; ++i) {
        revDate[i] = date[6 + i];
    }
    revDate[4] = '.';
    for (int i = 5; i < 7; ++i) {
        revDate[i] = date[i - 2];
    }
    revDate[7] = '.';
    for (int i = 8; i < 10; ++i) {
        revDate[i] = date[i - 8];
    }
    return revDate;
}

//--------------------------------------------------LIST------------------------------------------------------
struct Node {
    struct Liver * liver;
    struct Node *next;
};

struct Node* Create(struct Liver * liver) {
    struct Node* ptr = NULL;
    ptr = (struct Node*)malloc(sizeof(struct Node));
    printf("Allocated in Create\n");
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

void InsertLiver(struct Node ** head, struct Liver * newLiver) {
    struct Node * newNode = Create(newLiver);
    if ((*head) == NULL) {
        (*head) = newNode;
    } else {
        char * newBDrev = GetReversedDate(newLiver->birthday);
        char * headBDrev = GetReversedDate((*head)->liver->birthday);
        if (strcmp(newBDrev, headBDrev) > 0) {
            PushFront(head, newNode);
        } else {
            struct Node* ptr = *head;
            int flag = 0;

            while (ptr->next != NULL) {
                char * ptrBDrev = GetReversedDate(ptr->next->liver->birthday);
                if (strcmp(newBDrev, ptrBDrev) > 0) {
                    Insert(ptr, newNode);
                    flag = 1;
                    break;
                }
                ptr = ptr->next;
                free(ptrBDrev);
                printf("free reversed\n");
            }
            if (!flag) {
                Insert(ptr, newNode);
            }

        }
        free(newBDrev);
        printf("free reversed\n");
        free(headBDrev);
        printf("free reversed\n");
    }
}

void Display(struct Node* head) {
    struct Node* ptr;
    ptr = head;
    int index = 0;
    while (ptr != NULL) {
        printf("--------------- LIVER %d ---------------\n", index);
        printf("Surname: %s\n", ptr->liver->surname);
        printf("Name: %s\n", ptr->liver->name);
        printf("Otchestvo: %s\n", ptr->liver->otchestvo);
        printf("Birthday: %s\n", ptr->liver->birthday);
        printf("Gender: %c\n", ptr->liver->gender);
        printf("Salary: %.3lf\n", ptr->liver->salary);
        printf("----------------------------------------\n\n");
        ptr = ptr->next;
        index++;
    }
}

void Clear(struct Node** head) {
    struct Node* ptr;
    int index = 0;
    while (*head != NULL) {
        ptr = *head;
        *head = (*head)->next;
        free(ptr->liver->name);
        free(ptr->liver->surname);
        free(ptr->liver->otchestvo);
        free(ptr->liver->birthday);
        free(ptr->liver);
        printf("free liver\n");
        free(ptr);
        printf("free ptr\n");
        printf("%d\n", index);
        index++;
    }
    *head = NULL;
}
//--------------------------------------------------LIST------------------------------------------------------


struct Liver * GetLiver (FILE* file) {


    char line[BUFSIZ];

    //surname
    if (fgets(line, BUFSIZ, file) == NULL) {
        return NULL;
    }

    struct Liver * liver = (struct Liver*)malloc(sizeof(struct Liver));
    printf("Allocated in GetLiver\n");
    if (liver == NULL) {
        printf("Memory allocation error while creating liver.\n");
        return NULL;
    }

    int len = (int)strlen(line);
    liver->surname = (char*)malloc(len + 1);
    line[len - 1] = '\0';
    strcpy(liver->surname, line);

    //name
    fgets(line, BUFSIZ, file);
    len = (int)strlen(line);
    liver->name = (char*)malloc(len + 1);
    line[len - 1] = '\0';
    strcpy(liver->name, line);

    //otchestvo
    fgets(line, BUFSIZ, file);
    len = (int)strlen(line);
    liver->otchestvo = (char*)malloc(len + 1);
    line[len - 1] = '\0';
    strcpy(liver->otchestvo, line);

    //birthday
    fgets(line, BUFSIZ, file);
    len = (int)strlen(line);
    liver->birthday = (char*)malloc(len + 1);
    line[len - 1] = '\0';
    strcpy(liver->birthday, line);

    //gender
    fgets(line, BUFSIZ, file);
    len = (int)strlen(line);
    line[len - 1] = '\0';
    liver->gender = line[0];

    //salary
    fgets(line, BUFSIZ, file);
    double sal = atof(line);
    liver->salary = sal;

    return liver;
}

int main(int argc ,char *argv[]) {
     // if (argc != 2) {
     //     printf("Wrong input.\nProvide full path to data file.\n");
     //     return WRONG_ARGUMENTS;
     // }
    //FILE* file = fopen(argv[1], "r");
    //FILE* file = fopen("D:\\in3_7.txt", "r");
    FILE* file = fopen("/mnt/d/in3_7.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return FAILED_OPEN_FILE;
    }

    struct Node* head = NULL;

    while (1) {
        struct Liver * liver = GetLiver(file);
        if (liver == 0) {
            break;
        }
        InsertLiver(&head, liver);
    }
    Display(head);
    Clear(&head);

    return SUCCESS;
}