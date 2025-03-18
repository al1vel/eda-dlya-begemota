#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <crypt.h> // To use it, add "-crypt" lib to gcc args

#define SUCCESS 0

// --- ERRORS ---
#define ENCRYPT_FAILURE (-2)
#define DECRYPT_FAILURE (-3)
#define FILE_FAILURE (-4)
#define TIME_FORMAT_FAILURE (-7)
#define TIME_IS_FUTURE (-8)
#define TIME_BELOW_1900 (-9)

// --- CONTROLS ---
#define NEED_LOGIN 2
#define NEED_REGISTER 3

// --- STATES ---
#define LOGIN_NOT_FOUND (-5)
#define LOGOUT (-6)
#define SHUTDOWN (-10)

char username[256];
int availableCommandsCount;

int contains(char *str, char *sub) {
    while (*sub != '\0') {
        if (*str != *sub) {
            return 0;
        }
        str++;
        sub++;
    }
    return 1;
}

int getNumber(const char *str, int start, int stop) {
    int result = 0;
    while (start <= stop) {
        result = result * 10 + (str[start++] - '0');
    }
    return result;
}

int hasFlag(char *str, char *flag) {
    //printf("str: <%s> | flag: <%s>\n", str, flag);
    while (*str != '-') {
        if (*str == '\0') {
            return 0;
        }
        str++;
    }
    while (*flag != '\0') {
        if (*str != *flag) {
            return 0;
        }
        str++;
        flag++;
    }
    return 1;
}

int getTime(char *str, time_t* arg) {
    while (*str != ' ') {
        if (*str == '\0') {
            //printf("Ended to early\n");
            return TIME_FORMAT_FAILURE;
        }
        str++;
    }
    while (*str == ' ') {
        str++;
    }
    //printf("doehali\n");
    char timeStr[20];

    int cnt = 0;
    int i = 0;
    while (*str != '\0' && cnt < 19) {
        timeStr[i] = *str;
        str++;
        i++;
        cnt++;
    }
    timeStr[i] = '\0';
    //printf("Time: <%s>\n", time);
    if (cnt != 19) {
        //printf("Less than 19\n");
        return TIME_FORMAT_FAILURE;
    }

    i = 0;
    while (timeStr[i] != '\0') {
        if ((i == 2 || i == 5) && (timeStr[i] != '.')) {
            //printf("Dots issue\n");
            return TIME_FORMAT_FAILURE;
        }
        if (i == 10 && timeStr[i] != ' ') {
            //printf("Space issue\n");
            return TIME_FORMAT_FAILURE;
        }
        if ((i == 13 || i == 16) && (timeStr[i] != ':')) {
            //printf("Doubledots issue\n");
            return TIME_FORMAT_FAILURE;
        }
        if (!isdigit(timeStr[i]) && (i != 2 && i != 5 && i != 10 && i != 13 && i != 16)) {
            //printf("%c Not a number\n", time[i]);
            return TIME_FORMAT_FAILURE;
        }
        i++;
    }

    struct tm t;
    t.tm_year = getNumber(timeStr, 6,9) - 1900;
    t.tm_mon = getNumber(timeStr, 3,4) - 1;
    t.tm_mday = getNumber(timeStr, 0,1);
    t.tm_hour = getNumber(timeStr, 11,12);
    t.tm_min = getNumber(timeStr, 14,15);
    t.tm_sec = getNumber(timeStr, 17,18);

    time_t now = time(NULL);
    struct tm *tn = localtime(&now);

    if (t.tm_year < 0) {
        return TIME_BELOW_1900;
    }

    if (tn->tm_year - t.tm_year < 0 || tn->tm_mon - t.tm_mon < 0 || tn->tm_mday - t.tm_mday < 0 || tn->tm_hour - t.tm_hour < 0 || tn->tm_min - t.tm_min < 0 || tn->tm_sec - t.tm_sec < 0) {
        return TIME_IS_FUTURE;
    }

    *arg = mktime(&t);
    return SUCCESS;
}

void validateCode(int code) {
    switch (code) {
        case ENCRYPT_FAILURE: printf("ENCRYPT_FAILURE\n"); break;
        case FILE_FAILURE: printf("FILE_FAILURE\n"); break;
        case DECRYPT_FAILURE: printf("DECRYPT_FAILURE\n"); break;
        default: printf("INVALID_CODE\n"); break;
    }
}

int encrypt_password(const char *password, char **encrypted_password) {
    char *salt = crypt_gensalt_ra(NULL, 0, NULL, 0);
    if (!salt) {
        return ENCRYPT_FAILURE;
    }

    void *enc_ctx = NULL;
    int enc_cxt_sz = 0;
    char *tmp_encrypted_password = crypt_ra(password, salt, &enc_ctx, &enc_cxt_sz);

    if (tmp_encrypted_password == NULL) {
        free(salt);
        return ENCRYPT_FAILURE;
    }

    *encrypted_password = (char *)calloc((strlen(tmp_encrypted_password) + 1), sizeof(char));
    strcpy(*encrypted_password, tmp_encrypted_password);
    free(enc_ctx);
    free(salt);
    return SUCCESS;
}

int compare_passwords(const char *password, const char *hashed_password, int *compare_res) {
    void *enc_ctx = NULL;
    int enc_cxt_sz = 0;

    char *hashed_entered_password = crypt_ra(password, hashed_password, &enc_ctx, &enc_cxt_sz);
    if (!hashed_entered_password) {
        return DECRYPT_FAILURE;
    }

    *compare_res = strcmp(hashed_password, hashed_entered_password);
    free(enc_ctx);
    return SUCCESS;
}

int isLoginValid(char *login) {
    while (*login != '\0') {
        if (!isalnum(*login)) {
            return 0;
        }
        login++;
    }
    return 1;
}

int isPasswordValid(char *password) {
    int cnt = 0;
    while (*password != '\0') {
        if (!isdigit(*password)) {
            return 0;
        }
        password++;
        cnt++;
        if (cnt > 6) {
            return 0;
        }
    }
    return 1;
}

int isUserAlreadyRegistered(char *newLogin) {
    FILE *file = fopen("userData.txt", "a+");
    if (file == NULL) {
        return FILE_FAILURE;
    }
    char login[256];
    int lineNum = 1;
    while (fgets(login, sizeof(login), file)) {
        for (int i = 0; i < 256; ++i) {
            if (login[i] == ' ') {
                login[i] = '\0';
                break;
            }
        }
        if (strcmp(login, newLogin) == 0) {
            return lineNum;
        }
        lineNum++;
    }
    fclose(file);
    return LOGIN_NOT_FOUND;
}

int getCommandFromUser(int commandsCnt) {
    int com;
    printf("Enter command: ");
    while (1) {
        if (scanf("%d", &com) != 1) {
            while (getchar() != '\n') {}
            printf("Invalid command, please try again: ");
            continue;
        }
        if (com > 0 && com <= commandsCnt) {
            return com;
        }
        printf("Invalid command, please try again: ");
    }
}

int loginUser() {
    printf("\nLOG IN\n");
    char login[256];
    int lineNum = 0;

    while (1) {
        printf("Enter login: ");
        scanf("%s", login);
        int code = isUserAlreadyRegistered(login);
        if (code == FILE_FAILURE) {
            return FILE_FAILURE;
        }
        if (code == LOGIN_NOT_FOUND) {
            printf("\nLogin not found.\n1 - Register\n2 - Continue log in\n\n");
            int command = getCommandFromUser(2);
            if (command == 1) {
                return NEED_REGISTER;
            }
        } else {
            lineNum = code;
            break;
        }
    }

    FILE *file = fopen("userData.txt", "r");
    if (file == NULL) {
        return FILE_FAILURE;
    }

    char name[256], encrypted_password[256], password[10];
    int line_count = 1, number;

    while (fscanf(file, "%s %s %d", name, encrypted_password, &number) == 3) {
        if (line_count == lineNum) {
            break;
        }
        line_count++;
    }

    printf("Enter password: ");
    while (1) {
        scanf("%s", password);
        int cmp;
        if (compare_passwords(password, encrypted_password, &cmp)) {
            return DECRYPT_FAILURE;
        }
        if (cmp == 0) {
            printf("Login succeeded!\n\n");
            strcpy(username, login);
            availableCommandsCount = number;
            printf("com num: %d\n", availableCommandsCount);
            return SUCCESS;
        }
        printf("\nWrong password.\n1 - Register\n2 - Try again\n\n");
        int command = getCommandFromUser(2);
        if (command == 1) {
            return NEED_REGISTER;
        } if (command == 2) {
            printf("Enter password: ");
        }
    }
}

int registerUser() {
    printf("\nREGISTER\n");
    char login[256], password[10], *hased_pass = NULL;
    while (1) {
        printf("Enter login: ");
        scanf("%s", login);
        if (!isLoginValid(login)) {
            printf("Login must contain ONLY latin letters and numbers.\nTry again: ");
        } else {
            int code = isUserAlreadyRegistered(login);
            if (code == FILE_FAILURE) {
                return FILE_FAILURE;
            }
            if (code >= 1) {
                printf("\nThis login is already registered.\n1 - Log in\n2 - Continue registration\n\n");
                int command = getCommandFromUser(2);
                if (command == 1) {
                    return NEED_LOGIN;
                }
            } else {
                break;
            }
        }
    }

    printf("Enter password (number from 0 to 100000): ");
    while (1) {
        scanf("%s", password);
        if (!isPasswordValid(password)) {
            printf("Invalid password. Try again: ");
        } else {
            break;
        }
    }
    if (encrypt_password(password, &hased_pass)) {
        return ENCRYPT_FAILURE;
    }

    FILE *file = fopen("userData.txt", "a+");
    if (file == NULL) {
        return FILE_FAILURE;
    }
    fprintf(file, "%s %s %s\n", login, hased_pass, "-1");
    fclose(file);
    strcpy(username, login);
    printf("Register succeeded!\n\n");
    availableCommandsCount = -1;
    return SUCCESS;
}

int welcome() {
    printf("\nWELCOME\n1 - Log in\n2 - Register\n3 - Shut down\n\n");
    int command = getCommandFromUser(3);
    switch (command) {
        case 1: {
            int code = loginUser();
            while (code == NEED_REGISTER || code == NEED_LOGIN) {
                if (code == NEED_REGISTER) {
                    code = registerUser();
                } else if (code == NEED_LOGIN) {
                    code = loginUser();
                }
            }
            return code;
        }
        case 2: {
            int code = registerUser();
            while (code == NEED_REGISTER || code == NEED_LOGIN) {
                if (code == NEED_REGISTER) {
                    code = registerUser();
                } else if (code == NEED_LOGIN) {
                    code = loginUser();
                }
            }
            return code;
        }
        case 3: {
            return SHUTDOWN;
        }
        default: break;
    }
    return SUCCESS;
}

int imposeSanctions(int lineNum, int commandsCnt) {
    FILE *file = fopen("userData.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        return FILE_FAILURE;
    }

    char login[128], password[128];
    int number;
    int curLine = 0;

    while (fscanf(file, "%s %s %d", login, password, &number) == 3) {
        curLine++;
        if (curLine == lineNum) {
            number = commandsCnt;
        }
        fprintf(temp, "%s %s %d\n", login, password, number);
    }
    fclose(file);
    fclose(temp);

    remove("userData.txt");
    rename("temp.txt", "userData.txt");
    return SUCCESS;
}

int readCommand(time_t* arg) {
    char com[40];
    int comNum = 0;
    printf("%s$ ", username);
    while (1) {
        fgets(com, sizeof(com), stdin);
        com[strcspn(com, "\n")] = '\0';

        if (strcmp(com, "Logout") == 0) {
            comNum = 4;
            break;
        }

        if (availableCommandsCount == 0) {
            printf("You have no commands left. You can only log out.\n%s$ ", username);
            continue;
        }

        if (strcmp(com, "Time") == 0) {
            comNum = 1;
            availableCommandsCount--;
            break;
        }
        if (strcmp(com, "Date") == 0) {
            comNum = 2;
            availableCommandsCount--;
            break;
        }
        if (contains(com, "Howmuch")) {
            int code = getTime(com, arg);
            if (code != SUCCESS) {
                if (code == TIME_BELOW_1900) {
                    printf("Invalid time. Year must be 1900 or above.\n%s$ ", username);
                } else if (code == TIME_IS_FUTURE) {
                    printf("Invalid time. Provided time must be past.\n%s$ ", username);
                } else if (code == TIME_FORMAT_FAILURE) {
                    printf("Invalid time format. Format needed: dd.mm.yyyy hh:mm:ss\n%s$ ", username);
                }
                continue;
            }

            if (hasFlag(com, "-s")) {
                comNum = 5;
                availableCommandsCount--;
                break;
            }
            if (hasFlag(com, "-m")) {
                comNum = 6;
                availableCommandsCount--;
                break;
            }
            if (hasFlag(com, "-h")) {
                comNum = 7;
                availableCommandsCount--;
                break;
            }
            if (hasFlag(com, "-y")) {
                comNum = 8;
                availableCommandsCount--;
                break;
            }
            printf("%s$ <Howmuch> must have valid flag: -s, -m, -h, -y.\n%s$ ", username, username);

        } else if (contains(com, "Sanctions")) {
            char login[256];
            int commandsCnt = 0, i = 0, j = 0, error = 0;

            while (com[i] != ' ') {
                if (com[i] == '\0') {
                    printf("Invalid command syntax. Expected: Sanctions username <number>.\n%s$ ", username);
                    error = 1;
                    break;
                }
                i++;
            }
            if (error) {
                continue;
            }
            i++;
            while (com[i] != ' ') {
                if (com[i] == '\0') {
                    printf("Invalid command syntax. Expected: Sanctions username <number>.\n%s$ ", username);
                    error = 1;
                    break;
                }
                login[j++] = com[i++];
            }
            login[j] = '\0';
            if (error) {
                continue;
            }
            i++;
            int cnt = 0;
            while (com[i] != '\0') {
                if (!isdigit(com[i])) {
                    error = 1;
                    break;
                }
                commandsCnt = commandsCnt * 10 + com[i++] - '0';
                cnt++;
            }
            if (cnt == 0) {
                printf("Invalid command syntax. Expected: Sanctions username <number>.\n%s$ ", username);
                continue;
            }
            if (error) {
                continue;
            }

            if (strcmp(login, username) == 0) {
                printf("It is not allowed to impose sanctions on yourself.\n%s$ ", username);
                continue;
            }

            int code = isUserAlreadyRegistered(login);
            if (code == LOGIN_NOT_FOUND) {
                printf("Login <%s> doesn't exist.\n%s$ ", login, username);
                continue;
            }

            code = imposeSanctions(code, commandsCnt);
            if (code != SUCCESS) {
                validateCode(code);
                continue;
            }
            availableCommandsCount--;
            printf("Sanctions were successfully imposed.\n\n");
            break;
        }
        else {
            printf("%s$ <%s> is not a command\n%s$ ", username, com, username);
        }
    }
    return comNum;
}

void printCurrentTime() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_str[9];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
    printf("Current time: %s\n\n", time_str);
}

void printCurrentDate() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char date_str[11];
    strftime(date_str, sizeof(date_str), "%d-%m-%Y", tm_info);
    printf("Current date: %s\n\n", date_str);
}

void printHowMuch(time_t* pastTime, int flag) {
    time_t now = time(NULL);
    if (flag == 1) {
        double passed = difftime(now, *pastTime);
        printf("Seconds passed: %.2f\n\n", passed);
    } else if (flag == 2) {
        double passed = difftime(now, *pastTime) / 60;
        printf("Minutes passed: %.2f\n\n", passed);
    } else if (flag == 3) {
        double passed = difftime(now, *pastTime) / 3600;
        printf("Hours passed: %.2f\n\n", passed);
    } else if (flag == 4) {
        double passed = difftime(now, *pastTime) / 31536000;
        printf("Years passed: %.2f\n\n", passed);
    }
}

int loop() {
    while (1) {
        time_t* time = (time_t*)malloc(sizeof(time_t));
        int command = readCommand(time);

        if (command == 1) {
            printCurrentTime();
        } else if (command == 2) {
            printCurrentDate();
        } else if (command == 5) {
            printHowMuch(time, 1);
        } else if (command == 6) {
            printHowMuch(time, 2);
        }else if (command == 7) {
            printHowMuch(time, 3);
        }else if (command == 8) {
            printHowMuch(time, 4);
        } else if (command == 4) {
            return LOGOUT;
        }
        free(time);
    }
}

int main() {
    while (1) {
        int code = welcome();
        if (code != SUCCESS) {
            if (code == SHUTDOWN) {
                printf("Shutting down...\n");
                return 0;
            }
            validateCode(code);
            printf("\nEmergency shutdown. CODE: %d.\n\n", code);
            return code;
        }
        while (getchar() != '\n') {}
        code = loop();
    }
}