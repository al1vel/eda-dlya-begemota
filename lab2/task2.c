#include <stdio.h>
#include <stdarg.h>
#include <math.h>


double srGeom(int argc, ...) {
    va_list args;
    va_start(args, argc);
    double total = 1;
    for (int i = 0; i < argc; ++i) {
        total *= pow(va_arg(args, double), 1.0/argc);
    }
    va_end(args);
    return total;
}

double fastPow(double number, int power) {
    if (power < 2) {
        if (power) {
            return number;
        }
        return 1;
    }
    double res = fastPow(number, (power/2));
    res = res * res;
    if (power % 2 == 1) {
        res = res * number;
    }
    return res;
}

int main() {
    double res;
    res = srGeom(3, 2.0, 4.0, 3.0);
    printf("%lf\n", res);

    res = srGeom(5, 1.2, 3.0, 4.5, 1.6, 8.0);
    printf("%lf\n", res);

    res = srGeom(6, 15.0, 1.8, 23.22, 12.12, 5.2, 1.0);
    printf("%lf\n", res);

    res = fastPow(3.14, 6);
    printf("%lf\n", res);

    res = fastPow(2.71, 17);
    printf("%lf\n", res);
}