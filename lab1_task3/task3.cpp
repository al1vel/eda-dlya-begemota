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
    UNKNOWN_FUNC_TYPE,
    MALLOC_FAIL
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
        case WRONG_ARG_NUMBER: printf("Wrong number of arguments\n"); break;
        case EPS_NOT_A_NUM: printf("EPS is NOT a number\n"); break;
        case EPS_TOO_BIG: printf("EPS is too big\n"); break;
        case NULL_EPS: printf("EPS can't be null\n"); break;
        case NEG_EPS: printf("EPS can't be negative\n"); break;
        case EPS_TOO_LARGE: printf("EPS is too large\n"); break;
        case NOT_A_NUM: printf("is NOT a number\n"); break;
        case NUM_TOO_LARGE: printf("is too large\n"); break;
        case NUMBER_TOO_BIG: printf("is too big\n"); break;
        case UNKNOWN_FUNC_TYPE: printf("UNKNOWN_FUNC_TYPE\n"); break;
        case MALLOC_FAIL: printf("Failed to allocate memory\n"); break;
        default: printf("UNKNOWN ERROR CODE\n");
    }
}

void ResolveEq(double eps, double a, double b, double c) {
    double r1, r2;
    if (fabs(a) < eps) {
        r1 = -c / b;
        printf("Root: %f\n", r1);
    } else {
        double disk = b * b - 4 * a * c;
        //printf("disk %f\n", disk);
        if (disk < 0) {
            printf("No roots, negative disk\n\n");
            return;
        }
        r1 = (-b + sqrt(disk)) / (2 * a);
        r2 = (-b - sqrt(disk)) / (2 * a);
        printf("Root1: %f\nRoot2: %f\n\n", r1, r2);
    }
}

int Permutation (double *arr, double eps, const double first, const double sec, const double th) {
    const double koef[3] = {first, sec, th};
    double a, b, c;
    int ind = 0, cnt = 0;
    for (int i = 0; i < 3; i++) {
        a = koef[i];
        for (int j = 0; j < 3; j++) {
            if (j == i) {
                continue;
            } else {
                b = koef[j];
            }
            for (int k = 0; k < 3; k++) {
                if (k == i or k == j) {
                } else {
                    c = koef[k];
                }
            }
            //printf("%.3f %.3f %.3f\n", a, b, c);
            int flag = 1;
            for (int l = 0; l < cnt; ++l) {
                if ((fabs(arr[l * 3] - a) < eps) and (fabs(arr[l * 3 + 1] - b) < eps) and (fabs(arr[l * 3 + 2] - c) < eps)) {
                    flag = 0;
                }
            }
            if (flag) {
                arr[ind++] = a;
                arr[ind++] = b;
                arr[ind++] = c;
                cnt++;
            }
        }
    }
    return cnt;
}

CODES funcForQ(const double eps, const double first, const double sec, const double th) {
    double * arr;
    arr = (double *)malloc(18 * sizeof(double));
    if (arr == NULL) {
        return MALLOC_FAIL;
    }
    const int cnt = Permutation(arr, eps, first, sec, th);
    for (int i = 0; i < cnt * 3; i+=3) {
        printf("%.3f %.3f %.3f\n", arr[i], arr[i + 1], arr[i + 2]);
        ResolveEq(eps, arr[i], arr[i + 1], arr[i + 2]);
    }
    free(arr);
    return VALID;
}

void funcForT(const double eps, const double a, const double b, const double c) {
    const double st[3] = {a, b, c};
    int flag = 0;
    for (int i = 0; i < 3; i++) {
        const double aKv = st[i] * st[i];
        const double bKv = st[((i + 1) > 2) ? i - 1 : i + 1] * st[((i + 1) > 2) ? i - 1 : i + 1];
        const double cKv = st[((i + 2) > 2) ? 0 : i + 2] * st[((i + 2) > 2) ? 0 : i + 2];
        //printf("%f %f %f\n", aKv, bKv, cKv);
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
            CODES r = funcForQ(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
            if (r != VALID) {
                ValidateCodes(r);
                return -1;
            }
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