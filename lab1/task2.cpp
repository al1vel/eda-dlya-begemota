#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <bits/ranges_base.h>

enum CODES {
    VALID,
    NOT_A_NUM,
    NUM_TOO_LARGE,
    NEG_NUM,
    EPS_TOO_BIG,
    NULL_EPS
};

typedef double(*callback)(double);
typedef double(*callback2)(double, double);

double fact(double n) {
    if (n == 0) {
        return 1.0;
    } else {
        return n * fact(n - 1);
    }
}

int is_prime(int x) {
    if (x < 2) return 0;
    if (x == 2) return 1;
    if (x % 2 == 0) return 0;
    for (int i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            return 0;
        }
    }
    return 1;
}

// double fact (double n) {
//     if (n == 0) return 1.0;
//     double res = 1;
//     while (n > 0) {
//         res *= n;
//         n--;
//     }
//     return res;
// }

// double c(double k, double m) {
//     double res = fact(m) / (fact(k) * fact(m - k));
//     //printf("%lf\n", res);
//     return res;
// }

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
    static double res;
    if (x == 1) res = -0.5;
    res = res - res * res / 2 + 1;
    return res;
}

double funcForSum4 (double x) {
    double res = pow(2, pow(2, -x));
    return res;
}

double funcForEq4 (double x) {
    double res = pow(x, 2) - 2;
    return res;
}

// double funcForLim5 (double m) {
//     double res = 0.0;
//     for (int k = 1; k <= m; k++) {
//         double up = (k % 2 == 0) ? 1 : -1;
//         res += (c(k, m) * (up / k) * log(fact(k)));
//     }
//     //printf("%.20lf %lf\n", res, m);
//     return res;
// }

double funcForLim52 (double n) {
    double res = 0;
    for (int k = 1; k <= n; k++) {
        res += 1 / (double)k;
    }
    res -= log(n);
    return res;
}

double funcForSum5 (double k) {
    double res = (1 / (pow(floor(sqrt(k)), 2)) - (1 / k));
    return res;
}

// double funcForLimInGamma3 (double t) {
//     double res = log(t);
//     for (int p = 1; p <= t; p++) {
//         if (is_prime(p)) {
//             res *= (((double)p - 1) / (double)p);
//         }
//     }
//     return res;
// }
//
// double funcForEq5 (double x, double lim_val) {
//     double res = pow(2.7182818284, -x) - lim_val;
//     return res;
// }

double limit(const double EPS, const callback func) {
    double val;
    double cur_val;
    // double prev_val;
    double x = 1;
    do {
        cur_val = func(x);
        x += 1;
        val = func(x);
        //printf("%.20f %.20f\n", val, cur_val);
    } while ((fabs(cur_val - val)) > EPS);
    return cur_val;
}

double proizvedeniye(double cur_proizv, int t){
    if (is_prime(t)){
        cur_proizv = cur_proizv * (t - 1) / t;
    }
    return cur_proizv;
}

double funcForGammaEq(double ep) {
    int t = 2;
    double proizv = 0.5, res = 0, gamma;
    do {
        res = log(t) * proizv;
        t++;
        proizv = proizvedeniye(proizv, t) ;
    } while (fabs(res - (log(t + 1) * proizv)) > ep);
    gamma = -log(res);
    return gamma;
}

double limit4(const double EPS, const callback func) {
    double val;
    double cur_val;
    double prev_val;
    double x = 1;
    do {
        prev_val = func(x);
        x += 1;
        val = func(x);
        x += 1;
        cur_val = func(x);
        //printf("%.20f %.20f %.20f\n", prev_val, val, cur_val);
    } while (((fabs(cur_val - val)) > EPS) and (fabs(cur_val - prev_val) > EPS));
    return cur_val;
}

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
    //printf("%lf\n", summ * multiplier);
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

// double equationGamma(const double EPS, const callback2 func, double l, double r, double lim_val) {
//     double x = (l + r) / 2;
//     double prev = 100;
//     while (fabs(func(x, lim_val) - prev) > EPS) {
//         //printf("%-5f %-5f %-5f %.10f\n", l, r, x, func(x, lim_val));
//         if (func(x, lim_val) < 0) {
//             r = x;
//         } else {
//             l = x;
//         }
//         prev = func(x, lim_val);
//         x = (l + r) / 2;
//     }
//     return l;
// }

double mult(const double start, const double EPS, const callback func) {
    double x = start;
    double res = 1;
    double cur_res;

    do {
        res *= func(x);
        x += 1;
        cur_res = res;
        cur_res *= func(x);
        //printf("%lf, %lf\n", res, cur_res);
    } while ((fabs(res - cur_res) > EPS));
    return res;
}

int main(const int argc, char* argv[]) {
    //printf("%d %d %d %d %d\n", is_prime(0), is_prime(1), is_prime(2), is_prime(3), is_prime(4));
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
    res_lim = limit4(eps, &funcForLim4);
    res_sum = mult(2, eps, funcForSum4);
    res_eq = equation(eps, &funcForEq4, 0, 2);
    printf("%-8f | %-8f | %-8f\n", res_lim, res_sum, res_eq);

    //COUNT GAMMA
    res_lim = limit(eps, &funcForLim52);
    res_sum = summ(2, eps, funcForSum5, 1) - (pow(3.1415926535, 2) / 6);
    //double limitForGamma3 = limit(eps * 10000, &funcForLimInGamma3);
    //printf("%lf\n", limitForGamma3);
    //res_eq = equationGamma(eps, funcForEq5, 0, 1, limitForGamma3);
    res_eq = funcForGammaEq(eps);
    printf("%-8f | %-8f | %-8f\n", res_lim, res_sum, res_eq);

    printf("\n");return 0;
}