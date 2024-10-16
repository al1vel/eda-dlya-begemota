#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

enum CODES {
    VALID,
    WRONG_ARG_NUMBER,
    EPS_NOT_A_NUM,
    EPS_TOO_LARGE,
    NEG_EPS,
    EPS_TOO_BIG,
    NULL_EPS,
    NUM_TOO_LARGE,
    NOT_A_NUM,
    NUMBER_TOO_BIG,
    UNKNOWN_FUNC_TYPE
};

int toInt(const char* argv) {
    int res = 0;
    int i = 0;
    int neg_flag = 0;
    if (argv[0] == '-') {
        neg_flag = 1;
        i = 1;
    }
    while (argv[i] != '\0') {
        res = res * 10 + argv[i] - '0';
        i++;
    }
    if (neg_flag) {
        res *= -1;
    }
    //printf("%d\n", res);
    return res;
}

CODES ValidateEps(const char *argv) {
    int dotCnt = 0;
    int cnt = 0;
    if (argv[0] == '-') {
        argv++;
    }
    while (*argv != '\0') {
        if (!isdigit(*argv) and (*argv != '.')) {
            return EPS_NOT_A_NUM;
        } else if (*argv == '.') {
            dotCnt++;
            argv++;
        } else {
            argv++;
            cnt++;
        }
    }
    if (dotCnt > 1) {
        return EPS_NOT_A_NUM;
    }
    if (cnt > 9) {
        return EPS_TOO_LARGE;
    }
    return VALID;
}

CODES ValidateFloatNum(const char *argv, const int neg_flag) {
    int dotCnt = 0;
    int cnt = 0;
    if (neg_flag) {
        if (argv[0] == '-') {
            argv++;
        }
    }
    while (*argv != '\0') {
        if (!isdigit(*argv) and (*argv != '.')) {
            return NOT_A_NUM;
        } else if (*argv == '.') {
            dotCnt++;
            argv++;
        } else {
            argv++;
            cnt++;
        }
    }
    if (dotCnt > 1) {
        return NOT_A_NUM;
    }
    if (cnt > 9) {
        return NUM_TOO_LARGE;
    }
    return VALID;
}

CODES ValidateEpsNum(const double eps) {
    //printf("%lf\n", eps);
    if (eps < 0) {
        return NEG_EPS;
    } else if (eps > 0.1) {
        return EPS_TOO_BIG;
    } else if (eps == 0.0) {
        return NULL_EPS;
    }
    return VALID;
}

CODES ValidateIntNum(const char* argv) {
    if (*argv == '-') argv++;
    int first = *argv - '0';
    int cnt = 0;
    while (*argv) {
        if (!isdigit(*argv)) return NOT_A_NUM;
        argv++;
        cnt++;
    }
    //printf("%d %d", cnt, first);
    if (cnt >= 11) return NUMBER_TOO_BIG;
    if (cnt >= 10 and first > 2) return NUMBER_TOO_BIG;
    return VALID;
}

CODES ValidateArgs(const int argc, const int funcType, char *argv[]) {
    switch (funcType) {
        case 0: {
            //printf("%s\n", argv[2]);
            if (argc != 6) {
                return WRONG_ARG_NUMBER;
            }
            CODES ret = ValidateEps(argv[2]);
            if (ret != VALID) {
                return ret;
            }
            ret = ValidateEpsNum(atof(argv[2]));
            if (ret != VALID) {
                return ret;
            }
            for (int i = 0; i < 3; i++) {
                //printf("%s\n", argv[i + 3]);
                ret = ValidateFloatNum(argv[i + 3], 1);
                if (ret != VALID) {
                    printf("Argument %d: ", i + 2);
                    return ret;
                }
            }
            return VALID;
        }

        case 1: {
            if (argc != 4) {
                return WRONG_ARG_NUMBER;
            }
            for (int i = 0; i < 2; i++) {
                //printf("%s\n", argv[i + 3]);
                CODES ret = ValidateIntNum(argv[i + 2]);
                if (ret != VALID) {
                    printf("Argument %d: ", i + 1);
                    return ret;
                }
            }
            return VALID;
        }

        case 2: {
            if (argc != 6) {
                return WRONG_ARG_NUMBER;
            }
            CODES ret = ValidateEps(argv[2]);
            if (ret != VALID) {
                return ret;
            }
            ret = ValidateEpsNum(atof(argv[2]));
            if (ret != VALID) {
                return ret;
            }
            for (int i = 0; i < 3; i++) {
                //printf("%s\n", argv[i + 3]);
                ret = ValidateFloatNum(argv[i + 3], 0);
                if (ret != VALID) {
                    printf("Argument %d: ", i + 2);
                    return ret;
                }
            }
            return VALID;
        }

        default: {
            return UNKNOWN_FUNC_TYPE;
        }
    }
}

int find_flag(const char *arg, const char **flags) {
    for (int i = 0; i < 6; ++i) {
        if (!strcmp(arg, flags[i])) {
            return i;
        }
    }
    return -1;
}

void ValidateCodes (CODES ret_code) {
    switch (ret_code) {
        case VALID: printf("Valid\n"); break;
        case WRONG_ARG_NUMBER: printf("WRONG_ARG_NUMBER\n"); break;
        case EPS_NOT_A_NUM: printf("EPS_NOT_A_NUM\n"); break;
        case EPS_TOO_BIG: printf("EPS_TOO_BIG\n"); break;
        case NULL_EPS: printf("NULL_EPS\n"); break;
        case NEG_EPS: printf("NEG_EPS\n"); break;
        case EPS_TOO_LARGE: printf("EPS_TOO_LARGE\n"); break;
        case NOT_A_NUM: printf("NOT_A_NUM\n"); break;
        case NUM_TOO_LARGE: printf("NUM_TOO_LARGE\n"); break;
        case NUMBER_TOO_BIG: printf("NUMBER_TOO_BIG\n"); break;
        case UNKNOWN_FUNC_TYPE: printf("UNKNOWN_FUNC_TYPE\n"); break;
        default: printf("UNKNOWN ERROR CODE\n");
    }
}

void funcForQ() {

}

void funcForT(const double eps, const double a, const double b, const double c) {
    const double st[3] = {a, b, c};
    int j, flag = 0;
    for (int i = 0; i < 3; i++) {
        const double aKv = st[i] * st[i];
        const double bKv = st[((i + 1) > 2) ? i - 1 : i + 1] * st[((i + 1) > 2) ? i - 1 : i + 1];
        const double cKv = st[((j = i + 2) > 2) ? j - (4 - i) : i + 2] * st[((j = i + 2) > 2) ? j - (4 - i) : i + 2];

        printf("%f %f %f\n", aKv, bKv, cKv);
        if (fabs(aKv - (bKv + cKv)) < eps) {
            flag = 1;
        }
    }
    if (flag) {
        printf("right-angled triangle\n");
    } else {
        printf("NOT right-angled triangle\n");
    }
}

void funcForM (const int first, const int second) {
    if (first % second == 0) {
        printf("First number is a multiple of the second\n");
    } else {
        printf("First number is NOT a multiple of the second\n");
    }
}

int main(const int argc, char *argv[]) {
    if (argc == 1) {
        printf("You must input flag\n");
        return -1;
    }

    const char *flags[] = {"-q", "-m", "-t", "/q", "/m", "/t"};

    int ret = find_flag(argv[1], flags);
    if (ret == -1) {
        printf("No such flag: %s\n", argv[1]);
        return -1;
    }
    int funcType = ret % 3;
    CODES ret_code = ValidateArgs(argc, funcType, argv);
    if (ret_code != VALID) {
        ValidateCodes(ret_code);
        return -1;
    }

    switch (funcType) {
        case 0: {
            funcForQ();
            break;
        }
        case 1: {
            funcForM(toInt(argv[2]), toInt(argv[3]));
            break;
        }
        case 2: {
            funcForT(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
            break;
        }
        default: {
            printf("Invalid function-type code\n");
            return -1;
        }
    }

    return 0;
}
