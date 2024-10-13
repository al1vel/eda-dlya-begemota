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
    X_TOO_LARGE
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

double summ (const double start, const double EPS, const callback func, const double x) {
    double n = start;
    double val = 1, summ = val;

    do {
        n += 1;
        val = val * func(n, x);
        summ += val;

        //printf("%lf, %lf\n", val, func(n, x));
    } while (fabs(val) > EPS);
    return summ;
}

double funcForSum1(const double n, const double x) {
    double res = x / (double)(n);
    return  res;
}

double funcForSum2(const double n, const double x) {
    double up = ((int)n % 2 == 0) ? 1 : -1;
    double res = up * pow(x, 2 * n) / fact(2 * n);
    return res;
}

double funcForSum3(const double n, const double x) {
    double res = pow(3, 3 * n) / fact(3 * n) * pow(fact(n), 3) * pow(x, 2 * n);
    return res;
}

int main(const int argc, char* argv[]) {
    double res;
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

    //printf("%lf %lf\n", eps, x);

    res = summ(0, eps, funcForSum1, x);
    printf("Result for a: %lf\n", res);

    res = summ(0, eps, funcForSum2, x);
    printf("Result for b: %lf\n", res);

    res = summ(0, eps, funcForSum3, x);
    printf("Result for c: %lf\n", res);

    return 0;
}