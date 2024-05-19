#include <bits/stdc++.h>
using namespace std;

struct Node{
    string val;
    Node* next{};
    Node* prev{};
};

struct List{
    Node* head;
    int length;
};

bool isEmptyList(List ls){ return ls.head == nullptr; }

void printNode(Node *ptn){
    if (ptn == nullptr) throw -1;
    printf("%-13d %-10s %-13d %-13d\n", ptn, ptn -> val.c_str(), ptn -> next, ptn -> prev);
}

void printList(Node *head){
    printf("%-13s %-10s %-13s %-13s\n", "CUR ADDRESS", "VALUE", "NEXT ADDRESS", "PREV ADDRESS");
    while (!isEmptyList(head)){
        printNode(head);
        head = head -> next;
    }
}

void deleteList(Node* & head){
    Node *ptn = head;
    while (!isEmptyList(head)){
        ptn = head -> next;
        delete head;
        head = ptn;
    }
}

void addNode(Node*&head, string val){
    Node *newN = new Node;
    if (isEmptyList(head)){
        head = newN;
        return;
    }
    newN -> next = nullptr;
    newN -> val = val;
    Node *ptn = head;
    while (ptn -> next != nullptr){
        ptn = ptn -> next;
    }
    ptn -> next = newN;
    newN -> prev = ptn;
}
int main(){
    Node *listH = nullptr;
    listH = new Node;
    listH -> val = "aha";
    listH -> next = nullptr;
    addNode(listH, "hui");
    printList(listH);
}