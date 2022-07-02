#include <iostream>
using namespace std;
#define FOR(alpha, start, end) for(int alpha = start; alpha <= end; ++alpha)
const int maxN = 5005;
int n, PreOrder[maxN], InOrder[maxN];

class BinaryTree
{
private:
    struct node
    {
        int data;
        node *lchild, *rchild;
    };
    int nodeNum;
    node *root;

    node* private_Construct(int leftPre, int rightPre, int leftIn, int rightIn)
    {
        if(leftPre > rightPre || leftIn > rightIn)
            return nullptr;
        node *temp = new node;
        int base = PreOrder[leftPre], pos = 0;
        while(InOrder[++pos] != base);
        temp->data = base;
        temp->lchild = private_Construct(leftPre + 1, leftPre + (pos - leftIn), leftIn, pos - 1);
        temp->rchild = private_Construct(rightPre - (rightIn - pos) + 1, rightPre, pos + 1, rightIn);
        return temp;
    }
    void private_PrintPostOrder(node *t)
    {
        if(t == nullptr) return;
        private_PrintPostOrder(t->lchild);
        private_PrintPostOrder(t->rchild);
        printf("%d ", t->data);
    }
    void free(node *t)
    {
        if(t->lchild != nullptr) free(t->lchild);
        if(t->rchild != nullptr) free(t->rchild);
        delete t;
    }

public:
    BinaryTree(int n = 0) :nodeNum(n) {};
    ~BinaryTree() {free(root);}
    void Construct() {root = private_Construct(1, nodeNum, 1, nodeNum);}
    void PrintPostOrder() {private_PrintPostOrder(root);}
};


int main()
{
    scanf("%d", &n);
    FOR(i,1,n) scanf("%d", &PreOrder[i]);
    FOR(i,1,n) scanf("%d", &InOrder[i]);
    BinaryTree T(n);
    T.Construct();
    T.PrintPostOrder();
    return 0;
}