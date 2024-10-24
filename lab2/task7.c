#include <stdio.h>
#include <math.h>

enum {
    SUCCESS,
    EPS_INVALID,
    BORDERS_INVALID
} CODES;

void ValidateCode(int code) {
    switch (code) {
        case EPS_INVALID: printf("Epsilon must be positive and less than 1.\n"); break;
        case BORDERS_INVALID: printf("Left border must be less than right.\n"); break;
        default: printf("Unknown error code.\n"); break;
    }
}

typedef double(*callback)(double);

void equation(const double EPS, const callback func, double l, double r) {
    double x = (l + r) / 2;
    double prev = 100;

    if ((func(r) - func(l)) > 0) {
        while (fabs(func(x) - prev) > EPS) {
            //printf("%-5f %-5f %-5f %-5f\n", l, r, x, func(x));
            if (func(x) > 0) {
                r = x;
            } else {
                l = x;
            }
            prev = func(x);
            x = (l + r) / 2;
        }
    } else {
        while (fabs(func(x) - prev) > EPS) {
            //printf("%-5f %-5f %-5f %.10f\n", l, r, x, func(x));
            if (func(x) < 0) {
                r = x;
            } else {
                l = x;
            }
            prev = func(x);
            x = (l + r) / 2;
        }
    }
    printf("%lf\n", l);
}

int solve(const double EPS, const callback func, double l, double r) {
    if (EPS <= 0 || EPS >= 1) {
        return EPS_INVALID;
    }
    if (l >= r) {
        return BORDERS_INVALID;
    }
    equation(EPS, func, l, r);
    return SUCCESS;
}

double funcForEq1 (double n) {
    double res = log(n) - 1;
    return res;
}

double funcForEq2 (double n) {
    double res = sin(n);
    return res;
}

double funcForEq3 (double n) {
    double res = pow(2.7182818284, n) - 2;
    return res;
}

int main() {
    int res;
    res = solve(0.000001, &funcForEq1, 1, 4);
    if (res != SUCCESS) {
        ValidateCode(res);
    }

    res = solve(0.001, &funcForEq2, 2, 4);
    if (res != SUCCESS) {
        ValidateCode(res);
    }

    res = solve(0.00001, &funcForEq3, 0, 2);
    if (res != SUCCESS) {
        ValidateCode(res);
    }
}