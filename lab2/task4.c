#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

enum {
    SUCCESS,
    IS_CONVEX,
    IS_NOT_CONVEX,
    MALLOC_FAILED
};

struct Point {
    double x;
    double y;
};

int comp(const void *a, const void *b) {
    struct Point *point1 = (struct Point *)a;
    struct Point *point2 = (struct Point *)b;

    double x1 = point1->x;
    double y1 = point1->y;
    double x2 = point2->x;
    double y2 = point2->y;

    if (x1 < x2) {
        if (y1 < y2) {
            return 1;
        }
        return -1;
    }
    if (y1 < y2) {
        return -1;
    }
    return 1;
}

int IsConvex(int count, ...) {
    va_list arg;
    va_start(arg, count);

    struct Point * array = (struct Point*)malloc(sizeof(struct Point) * count);
    if (array == NULL) {
        va_end(arg);
        return MALLOC_FAILED;
    }

    for (int i = 0; i < count; i++) {
        double x = va_arg(arg, double);
        double y = va_arg(arg, double);
        array[i].x = x;
        array[i].y = y;
    }
    printf("OLD TABLE\n");
    for (int i = 0; i < count / 2; i++) {
        printf("X: %f | Y: %f\n", array[i].x, array[i].y);
    }
    printf("\n-------NEW TABLE------\n");

    qsort(array, count / 2, sizeof(struct Point), comp);

    for (int i = 0; i < count / 2; i++) {
        printf("X: %f | Y: %f\n", array[i].x, array[i].y);
    }

    va_end(arg);
    free(array);
    return SUCCESS;
}

int main() {
    int ret = IsConvex(12, 1.0, 0.0, 4.0, 0.0, 4.0, 4.0, 0.0, 1.0, 2.0, 3.0, 5.0, 2.0);
    if (ret == SUCCESS) {}
    return 0;
}