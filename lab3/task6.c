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

struct Node {
    int BusNumber;
    int stationCoords;
    char* arrivalTime;
    char* departureTime;
    int marker;
    struct Node *next;
};

struct Node* Create(int BusNum, int stCoords, char* arrivalTime, char* departureTime, int marker) {
    struct Node* ptr = NULL;
    ptr = (struct Node*)malloc(sizeof(struct Node));
    ptr->BusNumber = BusNum;
    ptr->stationCoords = stCoords;
    ptr->arrivalTime = arrivalTime;
    ptr->departureTime = departureTime;
    ptr->marker = marker;
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

// Главный список
struct NodeMain {
    struct Node* BusHead;
    struct NodeMain *next;
};

struct NodeMain* CreateForMain(struct Node* BusHead) {
    struct NodeMain* ptr = NULL;
    ptr = (struct NodeMain*)malloc(sizeof(struct NodeMain));
    ptr->BusHead = BusHead;
    ptr->next = NULL;
    return ptr;
}

void InsertInMain(struct NodeMain* pNode, struct NodeMain* newNode) {
    newNode->next = pNode->next;
    pNode->next = newNode;
}

void PushFrontInMain(struct NodeMain** pHead, struct NodeMain* newNode) {
    newNode->next = *pHead;
    *pHead = newNode;
}

void deleteFromMain(struct NodeMain* pNode) {
    struct NodeMain* ptr;
    ptr = pNode->next;
    pNode->next = ptr->next;
    free(ptr);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("You must provide files.");
        return -1;
    }
    struct NodeMain* MainHead = NULL;

    for (int i = 1; i < argc; i++) {

        FILE* file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("Error opening file: %s\n", argv[i]);
            return -1;
        }

        char stCoords[256];
        fgets(stCoords, sizeof(stCoords), file);
        int stationCoords = toInt(stCoords);

        char line[256];
        while (1) {
            int busNum, marker;
            char *arrivalTime = (char*)malloc(sizeof(char) * 256);
            char *departureTime = (char*)malloc(sizeof(char) * 256);

            if (fgets(line, sizeof(line), file) == NULL) {
                break;
            }
            busNum = toInt(line);

            fgets(line, sizeof(line), file);
            strcpy(arrivalTime, line);

            fgets(line, sizeof(line), file);
            strcpy(departureTime, line);

            fgets(line, sizeof(line), file);
            marker = toInt(line);

            //printf("Bus #: %d\nArrTime: %s\nDepTime: %s\nMarker: %d\n\n", busNum, arrivalTime, departureTime, marker);

            struct Node* busStoppedNode = Create(busNum, stationCoords, arrivalTime, departureTime, marker);
            if (MainHead == NULL) {
                struct Node* busHead = busStoppedNode;
                struct NodeMain* Bus = CreateForMain(busHead);
                MainHead = Bus;
            } else {
                int thisBus = 0;
                struct NodeMain* ptr = MainHead;
                while (ptr != NULL) {
                    if (ptr->BusHead->BusNumber == busNum) {
                        thisBus = 1;
                        Insert(ptr->BusHead, busStoppedNode);
                    }
                    ptr = ptr->next;
                }
                if (!thisBus) {
                    struct Node* busHead = busStoppedNode;
                    InsertInMain(MainHead, CreateForMain(busHead));
                }
            }
        }
    }

    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        printf("-------------Bus Route -----------\n");
        struct Node* p = ptr->BusHead;
        while (p != NULL) {
            printf("BusNum: %d\nSt.Coords: %d\nArrTime: %sDepTime: %sMarker: %d\n\n", p->BusNumber, p->stationCoords, p->arrivalTime, p->departureTime, p->marker);
            p = p->next;
        }

        ptr = ptr->next;
    }


    return 0;
}