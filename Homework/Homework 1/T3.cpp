#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
using namespace std;
#define maxN 100005
bool IsInqueue[maxN];

class LinkedList
{
private:
    struct node 
    {
        int data;
        node *prev, *next;
    };
    node *head, *list;

public:
    LinkedList(int n=0)
    {
        list = new node[n+1];
        for(int i=1; i<=n; ++i)
            list[i].data = i;
        head = &list[1];
        list[1].prev = NULL;
        list[1].next = NULL;
    }
    ~LinkedList()
    {
        if(list) delete[] list;
    }
    void Insert(int k, int p, int i)
    {
        if(p)
        {
            list[i].prev = &list[k];
            list[i].next = list[k].next;
            if(list[k].next)
                list[k].next->prev = &list[i];
            list[k].next = &list[i];
        }
        else
        {
            list[i].next = &list[k];
            list[i].prev = list[k].prev;
            if(list[k].prev)
                list[k].prev->next = &list[i];
            list[k].prev = &list[i];
            if(head == &list[k])
                head = &list[i];
        }
    }
    void print()
    {
        node *temp = head;
        while(temp != NULL)
        {
            if(IsInqueue[temp->data])
                printf("%d ",temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
};

int main()
{
	int n, k, p, m, x;
    scanf("%d",&n);
    LinkedList L(n);
    for(int i=2; i<=n; ++i)
    {
        scanf("%d%d",&k,&p);
        L.Insert(k,p,i);
    }
    for(int i=1; i<=n; ++i)
        IsInqueue[i] = true;
    scanf("%d",&m);
    for(int i=1; i<=m; ++i)
    {
        scanf("%d",&x);
        IsInqueue[x] = false;
    }
    L.print();
    return 0;
}