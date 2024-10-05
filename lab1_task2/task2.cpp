#include <ctype.h>
#include <stdio.h>
#include <math.h>

enum CODES {
    VALID,
    NOT_A_NUM,
    NUM_TOO_LARGE,
    NEG_NUM,
    EPS_TOO_BIG
};

typedef double(*callback)(double);

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
    if (cnt > 18) {
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
        default: printf("Unknown error code\n"); break;
    }
}

CODES ValidateEpsNum (double eps) {
    //printf("%lf\n", eps);
    if (eps < 0) {
        return NEG_NUM;
    } else if (eps > 0.2) {
        return EPS_TOO_BIG;
    }
    return VALID;
}

double funcForLim1 (double n) {
    double res = pow((1 + 1 / n), n);
    return res;
}

double funcForSumm1 (double n) {
    double res = (1 / fact(n));
    return res;
}

double funcForEq1 (double n) {
    double res = log(n) - 1;
    return res;
}

double funcForLim2 (double n) {
    double res = pow((pow(2, n) * fact(n)),4) / (n * pow(fact(2 * n), 2));
    return res;
}

double funcForSum2 (double n) {

    int upPower = (int)n - 1;
    //printf("%d\n", upPower);
    double up = 1.0;

    if (upPower % 2 == 1) {
        up = -1.0;
    }
    double res = up / (2 * n - 1);
    return res;
    // double res = pow(-1, (n - 1)) / (2 * n - 1);
    // return res;
}

double funcForEq2 (double n) {
    double res = sin(n);
    return res;
}

double funcForLim3 (double n) {
    double res = n * (pow(2, 1 / n) - 1);
    return res;
}

double funcForSum3 (double n) {
    int upPower = (int)n - 1;
    //printf("%d\n", upPower);
    double up = 1.0;

    if (upPower % 2 == 1) {
        up = -1.0;
    }
    double res = up / n;
    return res;
}

double funcForEq3 (double n) {
    double res = pow(2.7182818284, n) - 2;
    return res;
}

double funcForLim4 (double x) {
    // static int flag = 0;
    // double res;
    // if (flag == 0) {
    //     res = -0.5 - (-0.5 / 2 * -0.5) + 1;
    //     flag = 1;
    // } else {
    //     res = n - (n * n / 2) + 1;
    // }
    // //printf("%lf, %lf\n", res, n);
    // return res;
    static double res;
    if (x == 1) res = -0.5;
    res = res - res * res / 2 + 1;
    return res;
}

double limit(const double EPS, const callback func) {
    double val;
    double cur_val;
    // double prev_val;
    double x = 1;
    do {
        cur_val = func(x);
        x += 1;
        val = func(x);
        //printf("%f %f\n", val, cur_val);
    } while ((fabs(cur_val - val)) > EPS);
    return cur_val;
}

// double limit4(const double EPS, const callback func) {
//     double val;
//     double cur_val;
//     double x = 1;
//     do {
//         cur_val = func(x);
//         x = func(x);
//         val = func(x);
//         printf("%.18f %.18f\n", val, cur_val);
//     } while ((fabs(cur_val - val)) > EPS);
//     return cur_val;
// }

double summ (const double start, const double EPS, const callback func, double multiplier) {
    double x = start;
    double summ = 0;

    double val = func(x);
    summ += func(x);
    double prev = func(x);
    x += 1;

    do {
        val = func(x);
        summ += val;
        prev = val;
        x += 1;
        //printf("%lf, %lf\n", val, prev);
    } while (fabs(fabs(func(x) - fabs(prev))) > EPS);

    return summ * multiplier;
}

double equation(const double EPS, const callback func, double l, double r) {
    double x = (l + r) / 2;
    double prev = 100;

    if ((func(r) - func(l)) > 0) {
        //printf("test");
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

    return l;
}

int main(const int argc, char* argv[]) {
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

    double res_lim = 0;
    double res_sum = 0;
    double res_eq = 0;

    //HEADER
    printf("%-8s | %-8s | %-8s\n", "LIMIT", "ROW", "EQUATION");
    printf("==============================\n");

    //COUNT E
    res_lim = limit(eps, &funcForLim1);
    res_sum = summ(0, eps, &funcForSumm1, 1);
    res_eq = equation(eps, &funcForEq1, 1, 4);
    printf("%-8f | %-8f | %-8f\n", res_lim, res_sum, res_eq);

    // //COUNT PI
    res_lim = limit(eps, &funcForLim2);
    res_sum = summ(1, eps, &funcForSum2, 4);
    res_eq = equation(eps, &funcForEq2, 2, 4);
    printf("%-8f | %-8f | %-8f\n", res_lim, res_sum, res_eq);

    // //COUNT ln2
    res_lim = limit(eps, &funcForLim3);
    res_sum = summ(1, eps, &funcForSum3, 1);
    res_eq = equation(eps, &funcForEq3, 0, 2);
    printf("%-8f | %-8f | %-8f\n", res_lim, res_sum, res_eq);

    //COUNT sqrt2
    res_lim = limit(eps, &funcForLim4);
    printf("%-8f | %-8f | %-8f\n", res_lim, res_sum, res_eq);

    printf("\n");return 0;
}