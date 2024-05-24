#include<cstdio>
#include<cstdlib>

int size;
char ch;
int counmin = 0;
int f = 0;

struct node {
    char data;
    node* left;
    node* right;
};

void printtree(node* t) {
    int l = 0;
    l++;
    if (t)
    {
        printtree(t->right);
        for (int i = 0; i < l; i++)
            printf(" ");
        printf("%c\n", t->data);
        printtree(t->left);
    }
    l--;
}

int Is_Letter_or_Number() {
    return ((ch >= 'a') && (ch <= 'z') || (ch >= '0') && (ch <= '9'));
}

node* mknode(char c, node* l, node* r) {
    node* t = (node*)malloc(sizeof(node));
    t->data = c;
    t->left = l;
    t->right = r;
    return t;
}

node* expr();

node* fact() {
    node* t = nullptr;
    scanf("%c", &ch);
    if (ch == '(') {
        t = expr();
        if (ch != ')') printf("Error, no ')'\n");
    }
    else if (Is_Letter_or_Number()) t = mknode(ch, 0, 0);
    else if (ch == '-') {
        scanf("%c", &ch);
        counmin++;
        t = mknode(ch, 0, 0);
    }
    else printf("Error: not alpha not digit.\n");
    return t;
}

node* term() {
    node* tm;
    int done;
    char ch1;
    tm = fact();
    done = 0;
    while ((ch != '\n') && (!done)) {
        scanf("%c", &ch);
        if ((ch == '*') || (ch == '/') || (ch == '^')) {
            ch1 = ch;
            tm = mknode(ch1, tm, fact());
        }
        else done = 1;
    }
    return tm;
}

node* expr() {
    node* ex;
    int done;
    char ch1;
    ex = term();
    done = 0;
    while ((ch != '\n') && (!done)) {
        if ((ch == '+') || (ch == '^')) {
            ch1 = ch;
            ex = mknode(ch1, ex, term());
        }
        else done = 1;
    }
    return ex;
}
void tree2expr(node* tree) {
    if (counmin % 2 == 1 && f == 0) {
        printf("-");
        f++;
    }
    if (tree) {
        size++;
        tree2expr(tree->left);
        printf("%c", tree->data);
        tree2expr(tree->right);
    }
}

int main() {
    printf("Input expression: \n");
    node* tree = expr();
    printf("\n");
    tree2expr(tree);
    printf("\n\n");
    printtree(tree);
    f = 0;
    printf("\n");
    tree2expr(tree);
    printf("\n\n");
    printtree(tree);
};