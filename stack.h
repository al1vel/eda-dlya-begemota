#ifndef STACK_H
#define STACK_H

struct Stack {
    int* data;
    int length;
    int top;
};

void new_Stack(Stack& ns, int length);
Stack new_Stack(int length);
int pull(Stack& s);
void resize(Stack& s);
int top(Stack s);
bool Stack_is_empty(Stack ns);
void push(Stack& s, int e);
void Delete_Stack(Stack& s);
void quickSort(int* arr, int lo, int hi);
Stack conc(Stack& s1, Stack& s2);
void PrintStack(Stack& st);

#endif