#include <iostream>

using namespace std;

struct NodeBST {
    int v;
    NodeBST* l;
    NodeBST* r;
};

void insert_NodeBST(NodeBST *& root, int val) {
    if (root == nullptr) {
        root = new NodeBST;
        root -> v = val;
        root -> l = nullptr;
        root -> r = nullptr;
        return;
    }
    if (val <= (root -> v)) {insert_NodeBST(root -> l, val);}
    else {insert_NodeBST(root -> r, val);}
}

void print_LrootR(NodeBST *& root) {
    if (root == nullptr) {return;}
    print_LrootR(root -> l);
    cout << root -> v << '\n';
    print_LrootR(root -> r);
}

void print_rootLR(NodeBST *& root) {
    if (root == nullptr) {return;}
    cout << root -> v << '\n';
    print_rootLR(root -> l);
    print_rootLR(root -> r);
}

int width(NodeBST * head) {
    if (head == nullptr) {return 0;}
    if (head -> l == nullptr and head -> r == nullptr) {return 1;}
    return (width(head -> l) + width(head -> r));
}

int main() {
    NodeBST * mama = nullptr;
    insert_NodeBST(mama, 2);
    insert_NodeBST(mama, 10);
    insert_NodeBST(mama, 6);
    insert_NodeBST(mama, 7);
    insert_NodeBST(mama, 11);
    insert_NodeBST(mama, -4);
    insert_NodeBST(mama, 5);
    print_rootLR(mama);
    cout << "Width: " << width(mama);
}