#include <stdio.h>
#include <ctype.h>
#include <math.h>

enum CODES {
    VALID,
    NOT_A_NUM,
    NUM_TOO_LARGE,
    NEG_NUM,
    EPS_TOO_BIG,
    NULL_EPS
};

typedef double(*callback)(double);

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

double Integral(const double eps, const double a, const double b, const callback func) {
    double summ = 0, x = a;

    while (x <= b) {
        summ += func(x) * eps;
        x += eps;
        //printf("%f\n", summ);
    }
    return summ;
}

double funcForA (double x) {
    double res = log(1 + x) / x;
    return res;
}

double funcForB (double x) {
    double res = pow(2.7182818284, -(x * x / 2));
    return res;
}

double funcForC (double x) {
    double res = log((1.0 / (1 - x)));
    return res;
}

double funcForD (double x) {
    double res = pow(x, x);
    return res;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Epsilon must be inputted once\n");
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

    callback functions[4] = {funcForA, funcForB, funcForC, funcForD};
    for (int i = 0; i < 4; i++) {
        double result = Integral(eps, eps, 1, functions[i]);
        printf("Result for %d: %f\n", i + 1, result);
    }
    return 0;
}