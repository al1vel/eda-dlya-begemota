#include "stack.h"

int main() {
    Stack st1 = new_Stack(0);

    push(st1, 1);
    push(st1, 15);
    push(st1, 2);
    push(st1, 10);
    push(st1, 11);
    push(st1, 4);
    push(st1, 7);

    Stack st2 = new_Stack(0);

    push(st2, 0);
    push(st2, 3);
    push(st2, 45);
    push(st2, -12);
    push(st2, 5);
    push(st2, 6);
    push(st2, 8);

    Stack res = conc(st1, st2);
    quickSort(res.data, 0, res.top);

    PrintStack(res);
}