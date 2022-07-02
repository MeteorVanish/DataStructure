#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
using namespace std;

class LinkedList
{
private:
    struct node
    {
        int data;
        node *next;
    }*head;

public:
    LinkedList(int n=0)
    {
        head = new node;
        node *pos = head;
        for(int i=1; i<=n; ++i)
        {
            node *temp = new node;
            pos->next = temp;
            scanf("%d", &temp->data);
            pos = pos->next;
        }
        pos->next = NULL;
    }
    ~LinkedList()
    {
        node *temp = head;
        do
        {
            head = head->next;
            delete temp;
            temp = head;
        }while(head);
    }
    void Insert(int pos, int val)
    {
        node *p = head;
        for(int i=1; i<=pos; ++i) p = p->next;
        node *add = new node;
        add->data = val;
        add->next = p->next;
        p->next = add;
    }
    void Delete(int pos)
    {
        node *p = head;
        for(int i=1; i<pos; ++i) p = p->next;
        node *temp = p->next;
        p->next = temp->next;
        delete temp;
    }
    void print()
    {
        node *p = head->next;
        while(p)
        {
            printf("%d ", p->data);
            p = p->next;
        }
        printf("\n");
    }
};

int main()
{
	int n, m, opt, x, y;
    scanf("%d%d",&n,&m);
    LinkedList L(n);
    for(int i=1; i<=m; ++i)
    {
        scanf("%d",&opt);
        if(opt == 1)
        {
            scanf("%d%d",&x,&y);
            L.Insert(x,y);
        }
        else
        {
            scanf("%d",&x);
            L.Delete(x);
        }
    }
    L.print();
    return 0;
}