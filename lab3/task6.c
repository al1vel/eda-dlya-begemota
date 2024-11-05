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

int ValidateCommand(char* command) {
    char c = '0';
    for (int i = 0; i < 9; ++i) {
        char st[2] = "0\0";
        c++;
        st[0] = c;
        //printf("%s", st);
        if (strcmp(command, st) == 0) {
            return i + 1;
        }
    }
    return -1;
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

            //printf("Bus #: %d. Station: %d\nArrTime: %sDepTime: %sMarker: %d\n\n", busNum, stationCoords, arrivalTime, departureTime, marker);

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
                        //printf("This bus\n");
                        thisBus = 1;
                        // if (marker == 0) {
                        //     PushFront(&ptr->BusHead, busStoppedNode);
                        // } else {
                        if (strcmp(arrivalTime, ptr->BusHead->arrivalTime) < 0) {
                            //printf("Cur arr time: %sHead arr time: %s", arrivalTime, ptr->BusHead->arrivalTime);
                            //printf("Iz ifa\n\n\n");
                            PushFront(&ptr->BusHead, busStoppedNode);
                        } else {
                            struct Node* busPtr = ptr->BusHead;
                            while (busPtr->next != NULL) {
                                if (strcmp(arrivalTime, busPtr->next->arrivalTime) < 0) {
                                    break;
                                }
                                busPtr = busPtr->next;
                            }
                            //printf("Iz elsa\n\n\n");
                            Insert(busPtr, busStoppedNode);
                        }

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
        printf("------------- <%d> Bus Route -----------\n", ptr->BusHead->BusNumber);
        struct Node* p = ptr->BusHead;
        while (p != NULL) {
            printf("BusNum: %d\nSt.Coords: %d\nArrTime: %sDepTime: %sMarker: %d\n\n", p->BusNumber, p->stationCoords, p->arrivalTime, p->departureTime, p->marker);
            p = p->next;
        }
        ptr = ptr->next;
    }

    printf("Enter command:\n"
           "1. Bus with the biggest amount of routes\n"
           "2. Bus with the smallest amount of routes\n"
           "3. Bus with longest path\n"
           "4. Bus with shortest path\n"
           "5. Bus with longest route\n"
           "6. Bus with shortest route\n"
           "7. Bus with longest stop\n"
           "8. Bus with shortest stop\n"
           "9. Exit\n\n");

    int flag = 1;
    while (flag) {
        char command[256];
        scanf("%s", &command);
        int com = ValidateCommand(command);
        //printf("Command: %d\n", com);
        switch (com) {
            case -1: printf("Invalid Command\n"); break;
            case 9: flag = 0; break;
        }
    }

    return 0;
}