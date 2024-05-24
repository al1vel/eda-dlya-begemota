#include <iostream>
using namespace std;

struct Stack {
    int* data;
    int length;
    int top;
};

void new_Stack(Stack& ns, int length) {
    ns.data = new int[length];
    ns.length = length;
    ns.top = -1;
}

Stack new_Stack(int length) {
    Stack res;
    if (length <= 0) {
        res.length = 0;
        res.data = NULL;
    }
    res.length = length;
    res.data = new int[length];
    res.top = -1;
    return res;
}

int pull(Stack& s) {
    if (s.top >= 0) {
        return s.data[s.top--];
    }
    else {
        cout << "error" << "\n";
        return 0;
    }
}

void resize(Stack& s) {
    if (s.length) {
        int* arr = new int[s.length * 2];
        for (int i = 0; i < s.top; i++)
        {
            arr[i] = s.data[i];
        }
        delete[]s.data;
        s.data = arr;
        s.length = s.length * 2;
    }
    else {
        s = new_Stack(1);
    }
}

int top(Stack s) {
    if (s.top >= 0) return s.data[s.top];
    else {
        cout << "Error";
        return 0;
    }
}

bool Stack_is_empty(Stack ns) {
    if (ns.top < 0) {
        return true;
    }
    else {
        return false;
    }
}

void push(Stack& s, int e) {
    if (s.top < s.length - 1) {
        s.data[++s.top] = e;
    }
    else {
        resize(s);
        push(s, e);
        s.top++;
    }

}

void Delete_Stack(Stack& s) {
    if (s.data != NULL) {
        delete[]s.data;
        s.length = 0;
        s.top = -1;
    }
}

void quickSort(int* arr, int lo, int hi) {
    if (lo >= hi) return;
    int pivot = arr[(lo + hi) / 2];
    int left = lo, right = hi;
    while (left <= right) {
        while (arr[left] < pivot) left++;
        while (pivot < arr[right]) right--;
        if (left <= right) {
            swap(arr[left], arr[right]);
            left++;
            right--;
        }
    }
    quickSort(arr, lo, right);
    quickSort(arr, left, hi);
}

Stack conc(Stack& s1, Stack& s2) {
    Stack res = new_Stack(0);
    for (int i = 0; i < s1.top; ++i) {
        push(res, s1.data[i]);
    }
    for (int i = 0; i < s2.top; ++i) {
        push(res, s2.data[i]);
    }
    return res;
}

void PrintStack(Stack& st) {
    for (int i = 0; i < st.top; ++i) {
        cout << st.data[i] << '\n';
    }
}


int main() {
    Stack s1 = new_Stack(0);
    push(s1, 1);
    push(s1, 22);
    for (int i = 0; i < 10; i++)
    {
        push(s1, i);
    }
}