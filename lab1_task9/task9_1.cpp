#include <stdio.h>
#include <stdlib.h>

int FindClosest(const int *arr, const int size, const int val) {
    int res, minDelta = 1000000;
    for (int i = 0; i < size; i++) {
        int delta = abs(arr[i] - val);
        if (delta < minDelta) {
            res = arr[i];
            minDelta = delta;
        }
    }
    //printf("Closest element is %d\n", res);
    return res;
}

int main() {
    const int upBound = 10;
    const int loBound = 1;

    const int sizeA = rand() % (upBound - loBound + 1) + 10;
    const int sizeB = rand() % (upBound - loBound + 1) + 10;
    printf("The size of A: %d\nThe size of B: %d\n", sizeA, sizeB);

    int *A = (int*)malloc(sizeA * sizeof(int));
    if (A == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    int *B = (int*)malloc(sizeB * sizeof(int));
    if (B == NULL) {
        printf("Memory allocation failed\n");
        free(A);
        return -1;
    }

    printf("\nArray A:\n");
    for (int i = 0; i < sizeA; i++) {
        A[i] = (rand() % 200) - 100;
        printf("%d ", A[i]);
    }
    printf("\n\n");

    printf("Array B:\n");
    for (int i = 0; i < sizeB; i++) {
        B[i] = (rand() % 200) - 100;
        printf("%d ", B[i]);
    }
    printf("\n\n");

    int *C = (int*)malloc(sizeA * sizeof(int));
    if (C == NULL) {
        printf("Memory allocation failed\n");
        free(A);
        free(B);
        return -1;
    }

    printf("Array C:\n");
    for (int i = 0; i < sizeA; i++) {
        int addNum = FindClosest(B, sizeB, A[i]);
        C[i] = A[i] + addNum;
        printf("%d ", C[i]);
    }


    free(A);
    free(B);
    free(C);
    return 0;
}