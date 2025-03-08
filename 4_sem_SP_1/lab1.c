#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h> // To use it, add "-crypt" lib to gcc args

enum {
    SUCCESS,
    ENCRYPT_FAILURE,
    DECRYPT_FAILURE,
    FILE_FAILURE,
    FROM_REG_TO_LOGIN
};

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
        return -1;
    }

    void *enc_ctx = NULL;
    int enc_cxt_sz = 0;
    char *tmp_encrypted_password = crypt_ra(password, salt, &enc_ctx, &enc_cxt_sz);

    if (tmp_encrypted_password == NULL) {
        free(salt);
        return -1;
    }

    *encrypted_password = (char *)calloc((strlen(tmp_encrypted_password) + 1), sizeof(char));
    strcpy(*encrypted_password, tmp_encrypted_password);
    free(enc_ctx);
    free(salt);
    return 0;
}

int compare_passwords(const char *password, const char *hashed_password, int *compare_res) {
    void *enc_ctx = NULL;
    int enc_cxt_sz = 0;

    char *hashed_entered_password = crypt_ra(password, hashed_password, &enc_ctx, &enc_cxt_sz);
    if (!hashed_entered_password) {
        return -1;
    }

    *compare_res = strcmp(hashed_password, hashed_entered_password);
    free(enc_ctx);
    return 0;
}

int isLoginValid(char *login) {
    while (*login != '\0') {
        if (!isalnum(*login)) {
            //printf("Invalid input <%c>\n", *login);
            return 0;
        }
        login++;
    }
    //printf("Valid input\n");
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
        return -1;
    }
    char login[256];
    int lineNum = 1;
    while (fgets(login, sizeof(login), file)) {
        //printf("Got line: <%s>", login);
        for (int i = 0; i < 256; ++i) {
            if (login[i] == ' ') {
                login[i] = '\0';
                break;
            }
        }
        //printf("Cut: <%s>", login);
        if (strcmp(login, newLogin) == 0) {
            //printf("found\n");
            return lineNum;
        }
        lineNum++;
    }
    //printf("End\n");
    fclose(file);
    return 0;
}

int loginUser() {
    printf("\nLOG IN\n");
    char login[256];
    int lineNum = 0;

    while (1) {
        printf("Enter login: ");
        scanf("%s", login);
        int code = isUserAlreadyRegistered(login);
        if (code == -1) {
            return FILE_FAILURE;
        }
        if (code == 0) {
            printf("Login not found. ");
        } else {
            lineNum = code;
            break;
        }
    }

    FILE *file = fopen("userData.txt", "r");
    if (file == NULL) {
        return FILE_FAILURE;
    }

    char buffer[256];
    char encrypted_password[256];
    int line_count = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (line_count == lineNum) {
            int i = 0;
            while (1) {
                if (buffer[i] == ' ') {
                    break;
                }
                i++;
            }
            i++;
            int j = 0;
            while (buffer[i] != '\n') {
                encrypted_password[j++] = buffer[i++];
            }
            encrypted_password[j] = '\0';
            break;
        }
        line_count++;
    }

    //printf("Enc pass: <%s>\n", encrypted_password);

    char password[10];
    printf("Enter password: ");
    while (1) {
        scanf("%s", password);
        int cmp;
        if (compare_passwords(password, encrypted_password, &cmp)) {
            return DECRYPT_FAILURE;
        }
        if (cmp == 0) {
            printf("Logged in\n");
            return SUCCESS;
        }
        printf("Wrong password. Try again: ");
    }
}

int registerUser() {
    printf("\nREGISTER\n");
    char login[256];
    while (1) {
        printf("Enter login: ");
        scanf("%s", login);
        login[strcspn(login, "\n")] = '\0';
        if (!isLoginValid(login)) {
            printf("Login must contain ONLY latin letters and numbers.\nTry again: ");
        } else {
            int code = isUserAlreadyRegistered(login);
            if (code == -1) {
                return FILE_FAILURE;
            }
            if (code >= 1) {
                printf("\nThis login is already registered.\n1 - Log in\n2 - Continue registration\n\nEnter command: ");
                int command;
                while(1) {
                    if (scanf("%d", &command) != 1) {
                        while (getchar() != '\n') {}
                        printf("Invalid command, please try again: ");
                        continue;
                    }
                    if (command == 1) {
                        printf("LOGIN\n");
                        int ans = loginUser();
                        return ans;
                    }
                    if (command == 2) {

                        break;
                    }
                    printf("Invalid command, please try again: ");
                }
            } else {
                break;
            }
        }
    }

    char password[10];
    printf("Enter password (number from 0 to 100000): ");
    while (1) {
        scanf("%s", password);
        if (!isPasswordValid(password)) {
            printf("Invalid password. Try again: ");
        } else {
            break;
        }
    }
    char *hased_pass = NULL;
    if (encrypt_password(password, &hased_pass)) {
        return ENCRYPT_FAILURE;
    }

    FILE *file = fopen("userData.txt", "a+");
    if (file == NULL) {
        return FILE_FAILURE;
    }
    fprintf(file, "%s %s\n", login, hased_pass);
    printf("Successfully registered: %s\n", login);
    fclose(file);
    return SUCCESS;
}

int welcome() {
    printf("\nWELCOME\n1 - Log in\n2 - Register\n\nEnter command: ");
    int command;
    while(1) {
        if (scanf("%d", &command) != 1) {
            while (getchar() != '\n') {}
            printf("Invalid command, please try again: ");
            continue;
        }
        if (command == 1) {
            int code = loginUser();
            if (code != SUCCESS) {
                validateCode(code);
                return -1;
            }
            break;
        }
        if (command == 2) {
            int code = registerUser();
            if (code != SUCCESS) {
                validateCode(code);
                return -1;
            }
            break;
        }
        printf("Invalid command, please try again: ");
    }
    return 0;
}

int main() {
    int code = welcome();
    if (code != 0) {
        return code;
    }
    return 0;
}