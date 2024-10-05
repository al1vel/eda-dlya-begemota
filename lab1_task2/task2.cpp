#include <locale>
#include <stdio.h>
#include <math.h>

enum CODES {
    VALID,
    NOT_A_NUM,
    NUM_TOO_LARGE,
    NEG_NUM,
    EPS_TOO_BIG
};

typedef float(*callback)(float);

float fact(float n) {
    if (n == 0) return 1.0;
    else return n * fact(n - 1);
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
    if (cnt > 10) {
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

CODES ValidateEpsNum (float eps) {
    if (eps < 0) {
        return NEG_NUM;
    } else if (eps > 1) {
        return EPS_TOO_BIG;
    }
    return VALID;
}

float funcForLim1(float n) {
    float res = pow((1 + 1 / n), n);
    return res;
}

float funcForSumm1(float n) {
    float res = (1 / fact(n));
    return res;
}

float limit(const float EPS, const callback func) {
    float val;
    float cur_val;
    float x = 1;
    do {
        cur_val = func(x);
        x += 1;
        val = func(x);
        //printf("%f %f\n", val, cur_val);
    } while ((fabs(cur_val - val)) > EPS);
    return cur_val;
}

float summ (const float start, const float EPS, const callback func) {
    printf("hui");
    float val;
    float cur_val;
    float x = start;
    float summ = 0;
    float cur_summ = 0;
    do {
        cur_val = func(x);

        x += 1;
        val = func(x);
        printf("%f %f\n", val, cur_val);
    } while ((fabs(cur_val - val)) > EPS);
    return cur_val;
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

    const float eps = atof(argv[1]);
    ret = ValidateEpsNum(eps);
    if (ret != VALID) {
        ValidateCode(ret);
    }
    printf("Count e");
    float res_lim = limit(eps, &funcForLim1);
    float res_sum = summ(0, eps, &funcForSumm1);
    printf("Result: %5f %5f", res_lim, res_sum);

    return 0;
}
