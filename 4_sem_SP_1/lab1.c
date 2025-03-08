#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <crypt.h> // To use it, add "-crypt" lib to gcc args

#define SUCCESS 0

// --- ERRORS --- (negative numbers)
#define ENCRYPT_FAILURE (-2)
#define DECRYPT_FAILURE (-3)
#define FILE_FAILURE (-4)

// --- CONTROLS --- (2 - 10)
#define NEED_LOGIN 2
#define NEED_REGISTER 3

// --- STATES --- (11 - ...)
#define LOGIN_NOT_FOUND (-5)
#define LOGOUT (-6)

char username[256];

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

    char buffer[256], encrypted_password[256], password[10];
    int line_count = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (line_count == lineNum) {
            int i = 0, j = 0;
            while (1) {
                if (buffer[i] == ' ') {
                    break;
                }
                i++;
            }
            i++;
            while (buffer[i] != '\n') {
                encrypted_password[j++] = buffer[i++];
            }
            encrypted_password[j] = '\0';
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
            return SUCCESS;
        }
        printf("Wrong password.\n1 - Register\n2 - Try again\n\n");
        int command = getCommandFromUser(2);
        if (command == 1) {
            return NEED_REGISTER;
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
    fprintf(file, "%s %s\n", login, hased_pass);
    fclose(file);
    strcpy(username, login);
    printf("Register succeeded!\n\n");
    return SUCCESS;
}

int welcome() {
    printf("\nWELCOME\n1 - Log in\n2 - Register\n\n");
    int command = getCommandFromUser(2);
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
        default: break;
    }
    return SUCCESS;
}

int readCommand() {
    char com[20];
    int comNum = 0;
    printf("%s$ ", username);
    while (1) {
        fgets(com, sizeof(com), stdin);
        com[strcspn(com, "\n")] = '\0';
        if (strcmp(com, "Time") == 0) {
            comNum = 1;
            break;
        }
        if (strcmp(com, "Logout") == 0) {
            comNum = 4;
            break;
        }
        printf("%s$ <%s> is not a command\n%s$ ", username, com, username);

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

int loop() {
    while (1) {
        int command = readCommand();
        if (command == 1) {
            printCurrentTime();
        } else if (command == 4) {
            return LOGOUT;
        }
    }
}

int main() {
    int code = welcome();
    if (code != SUCCESS) {
        validateCode(code);
        printf("\nEmergency shutdown. CODE: %d.\n\n", code);
        return code;
    }

    while (getchar() != '\n') {}
    code = loop();

    while (1) {
        if (code == LOGOUT) {
            code = welcome();
            if (code != SUCCESS) {
                validateCode(code);
                printf("\nEmergency shutdown. CODE: %d.\n\n", code);
                return code;
            }
            while (getchar() != '\n') {}
            code = loop();
        }
    }

    printf("\nRegular finishing with code 0.\n\n");
    return 0;
}