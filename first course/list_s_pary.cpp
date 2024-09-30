#include <bits/stdc++.h>
using namespace std;

struct Node{
    int val;
    Node* next{};
    Node* prev{};

    Node() : val(0), next(nullptr), prev(nullptr) {}
    Node(int v) : val(v), next(nullptr), prev(nullptr) {}
    Node(Node *n) : val(n -> val), next(n -> next), prev(n -> prev) {}
};

struct List{
    Node* head;
    int length;

    List() : head(nullptr), length(0) {}
    List(int v, int len) {
        head = new Node(v);
        Node *newNode = new Node(v);
        head -> next = newNode;
        newNode -> prev = head;
        length = len;
        for (int i = 0; i < len - 2; ++i) {
            newNode -> next = new Node(v);
            newNode -> next -> prev = newNode;
            newNode = newNode -> next;
        }
        newNode -> next = head -> next;
    }
};

bool isEmptyList(List &ls){ return ls.head == nullptr; }

void printNode(Node *ptn){
    if (ptn == nullptr) throw -1;
    printf("%-13d %-10d %-13d %-13d\n", ptn, ptn -> val, ptn -> next, ptn -> prev);
}

void printList(List &ls){
    printf("%-13s %-10s %-13s %-13s\n", "CUR ADDRESS", "VALUE", "NEXT ADDRESS", "PREV ADDRESS");
    Node *ptn = ls.head;
    int len = ls.length;
    for (int i = 0; i < len; ++i){
        printNode(ptn);
        ptn = ptn -> next;
    }
}

void deleteList(List &ls){
    Node *ptn = ls.head;
    while (!isEmptyList(ls)){
        ptn = ptn -> next;
        delete ls.head;
        ls.head = ptn;
    }
}

void addNode(List &ls, int val){
    Node *newN = new Node(val);
    if (isEmptyList(ls)){
        ls.head = newN;
        return;
    }
    newN -> next = ls.head -> next;
    Node *ptn = ls.head;
    int len = ls.length;
    for (int i = 0; i < len - 1; ++i) {
        ptn = ptn -> next;
    }
    ptn -> next = newN;
    newN -> prev = ptn;
    ls.head -> next -> prev = newN;
    ls.length += 1;
}

int list_len(List &ls) {
    return ls.length;
}

void swap_sec_and_second_last(List &ls) {
    Node* sec = ls.head->next;
    Node* second_last = ls.head;
    for (int i = 0; i < ls.length - 2; ++i) {
        second_last = second_last->next;
    }
    swap(sec->val, second_last->val);
}

int main(){
    List a(3, 4);
    addNode(a, 10);
    addNode(a, 20);
    swap_sec_and_second_last(a);
    printList(a);
}