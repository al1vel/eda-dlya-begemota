#include <locale>
#include <stdio.h>

enum CODES {
    VALID,
    NOT_A_NUM
};

CODES ValidateEps(const char* argv) {
    int dotCnt = 0;
    while (*argv != '\0') {
        if (!isdigit(*argv) and (*argv != '.')) {
            return NOT_A_NUM;
        } else {
            argv++;
        }
    }
    return VALID;
}

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        printf("Epsilon must be inputted once\n");
        return -1;
    }
    const int ret = ValidateEps(argv[1]);
    printf("%d\n", ret);
}
