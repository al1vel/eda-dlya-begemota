#include <stdio.h>
#include <math.h>
#include <ctype.h>

enum CODES {
    VALID,
    NOT_A_NUM,
    NUM_TOO_LARGE,
    NEG_NUM,
    EPS_TOO_BIG,
    NULL_EPS,
    X_NOT_A_NUM,
    X_TOO_LARGE,
    DIVERGES,
    SUCCESS,
    NULL_X
};

typedef double(*callback)(double, double);

double fact(double n) {
    if (n == 0) {
        return 1.0;
    } else {
        return n * fact(n - 1);
    }
}

CODES ValidateEps(const char* argv) {
    int dotCnt = 0;
    int cnt = 0;
    if (argv[0] == '-') {
        argv++;
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

void ValidateCode(CODES code) {
    switch (code) {
        case NOT_A_NUM: printf("EPS is not a num\n"); break;
        case NUM_TOO_LARGE: printf("Num too large\n"); break;
        case EPS_TOO_BIG: printf("Num too big\n"); break;
        case NEG_NUM: printf("Negative number\n"); break;
        case NULL_EPS: printf("EPS can't be null\n"); break;
        case X_NOT_A_NUM: printf("X is not a num\n"); break;
        case X_TOO_LARGE: printf("X is too large\n"); break;
        case DIVERGES: printf("Func diverges\n"); break;
        case SUCCESS: break;
        case NULL_X: printf("X is null\n"); break;
        default: printf("Unknown error code\n"); break;
    }
}

CODES ValidateEpsNum (double eps) {
    //printf("%lf\n", eps);
    if (eps < 0) {
        return NEG_NUM;
    } else if (eps > 0.1) {
        return EPS_TOO_BIG;
    } else if (eps == 0.0) {
        return NULL_EPS;
    }
    return VALID;
}

CODES ValidateXNum (double x) {
    //printf("%lf\n", eps);
    if (x < 0) {
        return NEG_NUM;
    } else if (x == 0.0) {
        return NULL_X;
    }
    return VALID;
}

CODES ValidateX(const char* argv) {
    int dotCnt = 0;
    int cnt = 0;
    if (argv[0] == '-') {
        argv++;
    }
    while (*argv != '\0') {
        if (!isdigit(*argv) and (*argv != '.')) {
            return X_NOT_A_NUM;
        } else if (*argv == '.') {
            dotCnt++;
            argv++;
        } else {
            argv++;
            cnt++;
        }
    }
    if (dotCnt > 1) {
        return X_NOT_A_NUM;
    }
    if (cnt > 9) {
        return X_TOO_LARGE;
    }
    return VALID;
}

// void PrintRes(double res) {
//     if (res == 123.123123123123123) {
//         printf("The function diverges.\n");
//     }
//     else {
//         printf("%lf\n", res);
//     }
// }

// double summ (const double start, const double EPS, const callback func, const double x, const double startVal) {
//     double n = start, val = startVal, summ = val;
//
//     do {
//         n += 1;
//         val = val * func(n, x);
//         summ += val;
//
//         if (isinf(summ) || isnan(summ)) {
//             return 123.123123123123123;
//         }
//
//         printf("%lf, %lf, %lf\n", val, func(n, x), summ);
//     } while (fabs(val) > EPS);
//     return summ;
// }

CODES summ1 (const double start, const double EPS, const callback func, const double x, const double startVal) {
    double n = start, val = startVal, summ = val;

    n += 1;
    val = val * func(n, x);
    summ += val;

    for (int i = 0; i < 100000; ++i) {
        n += 1;
        val = val * func(n, x);
        summ += val;
        if (fabs(val) < EPS) {
            printf("Result: %lf\n", summ);
            return SUCCESS;
        }
    }
    return DIVERGES;
}

double funcForSum1(const double n, const double x) {
    double res = x / (double)(n);
    return  res;
}

double funcForSum2(const double n, const double x) {
    double res = -(x * x / (2*n - 1) / (2*n));
    return res;
}

double funcForSum3(const double n, const double x) {
    double res = (((9 * pow(n, 3) * x * x /(3*n - 1)) / (3*n - 2)) / n);
    return res;
}

double funcForSum4(const double n, const double x) {
    double res = (-(2 * n - 1) * x * x / (2 * n));
    if ((int)n % 2 == 1) {
        res = -res;
    }
    return res;
}

double funcForStartD(const double x) {
    double res = -x * x / 2;
    return res;
}

int main(const int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments\n");
        return -1;
    }
    CODES ret = ValidateEps(argv[1]);
    if (ret != VALID) {
        ValidateCode(ret);
        return -1;
    }
    const double eps = atof(argv[1]);
    ret = ValidateEpsNum(eps);
    if (ret != VALID) {
        ValidateCode(ret);
        return -1;
    }
    ret = ValidateX(argv[2]);
    if (ret != VALID) {
        ValidateCode(ret);
        return -1;
    }
    const double x = atof(argv[2]);
    ret = ValidateXNum(x);
    if (ret != VALID) {
        ValidateCode(ret);
        return -1;
    }

    CODES response = summ1(0, eps, funcForSum1, x, 1);
    ValidateCode(response);

    response = summ1(0, eps, funcForSum2, x, 1);
    ValidateCode(response);

    response = summ1(0, eps, funcForSum3, x, 1);
    ValidateCode(response);

    response = summ1(1, eps, funcForSum4, x, funcForStartD(x));
    ValidateCode(response);

    return 0;
}