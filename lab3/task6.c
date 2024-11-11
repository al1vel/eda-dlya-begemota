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
    for (int i = 0; i <= 9; ++i) {
        char st[2] = "0\0";

        st[0] = c;
        //printf("%s", st);
        if (strcmp(command, st) == 0) {
            return i;
        }
        c++;
    }
    return -1;
}

void AmOfRoutes(struct NodeMain* MainHead, int flag) {
    int BusNum, amount = 0, biggestBus = 0, biggestAm = -1, smallestBus = 0, smallestAm = 100000000;

    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        BusNum = ptr->BusHead->BusNumber;
        //printf("BusNum = %d\n", BusNum);
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            if (bus->marker == 2) {
                amount++;
            }
            bus = bus->next;
        }
        if (amount > biggestAm) {
            biggestAm = amount;
            biggestBus = BusNum;
        }
        if (amount < smallestAm) {
            smallestAm = amount;
            smallestBus = BusNum;
        }
        amount = 0;
        ptr = ptr->next;
    }
    if (flag) {
        printf("Bus #%d traveled the most routes: %d\n\n", biggestBus, biggestAm);
    } else {
        printf("Bus #%d traveled the least routes: %d\n\n", smallestBus, smallestAm);
    }
}

void Path(struct NodeMain* MainHead, int flag) {
    int BusNum, amount = 0, biggestBus = 0, biggestAm = -1, smallestBus = 0, smallestAm = 100000000;

    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        BusNum = ptr->BusHead->BusNumber;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            amount++;
            bus = bus->next;
        }
        if (amount > biggestAm) {
            biggestAm = amount;
            biggestBus = BusNum;
        }
        if (amount < smallestAm) {
            smallestAm = amount;
            smallestBus = BusNum;
        }
        amount = 0;
        ptr = ptr->next;
    }
    if (flag) {
        printf("Bus #%d traveled the biggest path: %d\n\n", biggestBus, biggestAm);
    } else {
        printf("Bus #%d traveled the smallest path: %d\n\n", smallestBus, smallestAm);
    }
}

void RouteLength (struct NodeMain* MainHead, int flag) {
    int BusNum, amount = 0, biggestBus = 0, biggestAm = -1, smallestBus = 0, smallestAm = 100000000;

    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        BusNum = ptr->BusHead->BusNumber;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            if (bus->marker == 2) {
                amount++;
                if (amount > biggestAm) {
                    biggestAm = amount;
                    biggestBus = BusNum;
                }
                if (amount < smallestAm) {
                    smallestAm = amount;
                    smallestBus = BusNum;
                }
                amount = 0;
            } else {
                amount++;
            }
            bus = bus->next;
        }

        ptr = ptr->next;
    }
    if (flag) {
        printf("Bus #%d traveled the longest route: %d\n\n", biggestBus, biggestAm);
    } else {
        printf("Bus #%d traveled the shortest route: %d\n\n", smallestBus, smallestAm);
    }
}

int GetDuration (struct Node* bus) {
    int duration = 0, arrTime = 0, depTime = 0, razr = 1;

    char *ar = bus->arrivalTime;
    while (*ar != '\n') {
        ar++;
    }
    ar--;
    while (*ar != ':') {
        arrTime += (*ar - '0') * razr;
        razr *= 10;
        ar--;
    }
    ar--;
    razr = 1;
    while (*ar != ':') {
        arrTime += (*ar - '0') * 60 * razr;
        razr *= 10;
        ar--;
    }
    ar--; razr = 1;
    while (*ar != ' ') {
        arrTime += (*ar - '0') * 3600 * razr;
        razr *= 10;
        ar--;
    }
    razr = 1;

    char *dp = bus->departureTime;
    while (*dp != '\n') {
        dp++;
    }
    dp--;
    while (*dp != ':') {
        depTime += (*dp - '0') * razr;
        razr *= 10;
        dp--;
    }
    razr = 1; dp--;
    while (*dp != ':') {
        depTime += (*dp - '0') * 60 * razr;
        razr *= 10;
        dp--;
    }
    razr = 1; dp--;
    while (*dp != ' ') {
        depTime += (*dp - '0') * 3600 * razr;
        razr *= 10;
        dp--;
    }
    duration = depTime - arrTime;
    return duration;
}

char *DurToString(int dur) {
    char *durString = (char*)malloc(9 * sizeof(char));
    if (durString == NULL) {
        return NULL;
    }
    int hrs, min, sec;
    hrs = dur / 3600;
    min = (dur / 60) % 60;
    sec = dur % 60;

    sprintf(durString, "%02d:%02d:%02d", hrs, min, sec);
    return durString;
}

void StopDuration (struct NodeMain* MainHead, int flag) {
    int BusNum, biggestBus = 0, biggestAm = -1, smallestBus = 0, smallestAm = 100000000, smallestSt = 0, biggestSt = 0;

    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        BusNum = ptr->BusHead->BusNumber;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            int dur = GetDuration(bus);
            if (dur > biggestAm) {
                biggestAm = dur;
                biggestBus = BusNum;
                biggestSt = bus->stationCoords;
            }
            if (dur < smallestAm) {
                smallestAm = dur;
                smallestBus = BusNum;
                smallestSt = bus->stationCoords;
            }
            bus = bus->next;
        }
        ptr = ptr->next;
    }

    if (flag) {
        char *durString = DurToString(biggestAm);
        printf("Bus #%d had longest stop for %s on <%d> station\n\n", biggestBus, durString, biggestSt);
        free(durString);
    } else {
        char *durString = DurToString(smallestAm);
        printf("Bus #%d had shortest stop for %s on <%d> station\n\n", smallestBus, durString, smallestSt);
        free(durString);
    }
}

void Print(struct NodeMain* MainHead) {
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
            char *arrivalTime = (char*)malloc(sizeof(char) * 256);
            char *departureTime = (char*)malloc(sizeof(char) * 256);

            if (fgets(line, sizeof(line), file) == NULL) {
                break;
            }
            int busNum = toInt(line);

            fgets(line, sizeof(line), file);
            strcpy(arrivalTime, line);

            fgets(line, sizeof(line), file);
            strcpy(departureTime, line);

            fgets(line, sizeof(line), file);
            int marker = toInt(line);

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
                        if (strcmp(arrivalTime, ptr->BusHead->arrivalTime) < 0) {
                            PushFront(&ptr->BusHead, busStoppedNode);
                        } else {
                            struct Node* busPtr = ptr->BusHead;
                            while (busPtr->next != NULL) {
                                if (strcmp(arrivalTime, busPtr->next->arrivalTime) < 0) {
                                    break;
                                }
                                busPtr = busPtr->next;
                            }
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

    printf("Available commands:\n"
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
        printf("Enter command: ");
        scanf("%s", &command);
        int com = ValidateCommand(command);
        //printf("Command: %d\n", com);
        switch (com) {
            case -1: printf("Invalid Command\n"); break;
            case 0: Print(MainHead); break;
            case 1: AmOfRoutes(MainHead, 1); break;
            case 2: AmOfRoutes(MainHead, 0); break;
            case 3: Path(MainHead, 1); break;
            case 4: Path(MainHead, 0); break;
            case 5: RouteLength(MainHead, 1); break;
            case 6: RouteLength(MainHead, 0); break;
            case 7: StopDuration(MainHead, 1); break;
            case 8: StopDuration(MainHead, 0); break;
            case 9: flag = 0; break;
            default: printf("No way\n");break;
        }
    }
    if (MainHead != NULL) {
        while (MainHead->next != NULL) {
            struct NodeMain* p = MainHead->next;
            struct Node* busHead = p->BusHead;
            while (busHead != NULL) {
                struct Node* next = busHead->next;
                free(busHead);
                busHead = next;
            }
            free(MainHead);
            MainHead = p;
        }
    }
    return 0;
}