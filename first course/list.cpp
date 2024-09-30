#include <cstdio>
#include <cstdlib>


struct node {
    int data;
    node *next;
};

//struct node *head = nullptr;
//struct node *current = nullptr;

bool isEmpty(node *head) {
    return head == nullptr;
}

int length(node *head) {
    node *current = nullptr;
    int length = 0;
    if(head == nullptr) {
        return 0;
    }
    current = head->next;
    while(current != head) {
        length++;
        current = current->next;
    }
    return length;
}

void insertFirst(node *head, int data) {
    node *link = (struct node*) malloc(sizeof(struct node));
    link->data = data;

    if (isEmpty(head)) {
        head = link;
        head->next = head;
    } else {
        //указать на старый первый элемент
        link->next = head;

        //указать начало на новый первый узел
        head = link;
    }
}

void deleteFirst(node *head) {
    node *tempLink = head;
    if(head->next == head) {
        head = nullptr;
    }
    head = head->next;
}

void printList(node *head) {
    node *ptr = head;
    printf("[");
    if(head != nullptr) {
        printf("(%d)",ptr->data);
        while(ptr->next != ptr) {
            ptr = ptr->next;
            printf("(%d)",ptr->data);

        }
    }
    printf("]\n");
}

int main() {
    node *head = nullptr;
    insertFirst(head, 10);
    insertFirst(head, 20);
    insertFirst(head, 30);
    insertFirst(head, 1);
    insertFirst(head, 40);
    insertFirst(head, 56);
    printf("Original List: \n");
    printList(head);

    printf("List after deleting all items:\n");
    printList(head);
}
