#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
using namespace std;
#define maxN 100005
int n, m, tree[maxN], len, nowpos=1, nextpos, delpos, step, dir=1;//0:left 1:right

inline int lowbit(int x) {return x&-x;}
void add(int pos, int val)
{
    for(int i=pos; i<=n; i+=lowbit(i))
        tree[i]+=val;
}
int sum(int pos)
{
    int ans = 0;
    for(int i=pos; i; i-=lowbit(i))
        ans+=tree[i];
    return ans;
}
int nextIndex(int pos, int stepx)
{
    if(dir)
    {
        if(len-pos >= stepx)
            return pos+stepx;
        else
        {
            dir = 0;
            return nextIndex(len, stepx-len+pos);
        }
    }
    else
    {
        if(pos-1 >= stepx)
            return pos-stepx;
        else
        {
            dir = 1;
            return nextIndex(1, stepx-pos+1);
        }
    }
}
int findpos(int x)
{
    int left=1, right=n, mid = left+right>>1;
    while(left < right)
    {
        if(sum(mid) >= x) right = mid;
        else left = mid+1;
        mid = left+right>>1;
    }
    return left;
}

int main()
{
    scanf("%d%d", &n, &m);
    len = n;
    for(int i=1; i<=n; ++i)
        add(i,1);
    while(len > 1)
    {
        step = (m-1)%(2*len-2);
        nextpos = nextIndex(nowpos, step);
        delpos = findpos(nextpos);
        add(delpos, -1);
        if(dir)
        {
            if(nextpos == len)
            {
                dir = 0;
                nowpos = len-1;
            }
            else nowpos = nextpos;
        }
        else
        {
            if(nextpos == 1)
            {
                dir = 1;
                nowpos = 1;
            }
            else nowpos = nextpos-1;
        }
        --len;
    }
    printf("%d\n",findpos(1));
    return 0;
}

//链表实现（最后两个点TLE）
/*
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
using namespace std;
#define maxN 100005

class LinkedList
{
private:
    int len, dir=1; //0:left 1:right
    struct node
    {
        int data;
        node* next;
        node* prev;
    }*head, *tail, *nowpos;
public:
    LinkedList(int n)
    {
        len = n;
        head = new node;
        tail = new node;
        head->prev = NULL;
        tail->next = NULL;
        node *pos = head;
        for(int i=1; i<=n; ++i)
        {
            node *temp = new node;
            pos->next = temp;
            temp->prev = pos;
            temp->data = i;
            pos = pos->next;
        }
        pos->next = tail;
        tail->prev = pos;
        nowpos = head->next;
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
    int lastval() {return head->next->data;}
    void Delete(node *t)
    {
        t->prev->next = t->next;
        t->next->prev = t->prev;
        delete t;
    }
    void pop(int m)
    {
        m %= 2*len-2;
        if(!m) m = 2*len-2;
        --m;
        len--;
        while(m--)
        {
            if(dir)
            {
                nowpos = nowpos->next;
                if(nowpos == tail)
                {
                    dir = 0;
                    nowpos = tail->prev;
                    ++m;
                }
            }
            else
            {
                nowpos = nowpos->prev;
                if(nowpos == head)
                {
                    dir = 1;
                    nowpos = head->next;
                    ++m;
                }
            }
        }
        node *temp = nowpos;
        if(dir)
        {
            if(nowpos->next == tail)
            {
                nowpos = nowpos->prev;
                dir = 0;
            }
            else nowpos = nowpos->next;
        }
        else
        {
            if(nowpos->prev == head)
            {
                nowpos = nowpos->next;
                dir = 1;
            }
            else nowpos = nowpos->prev;
        }
        Delete(temp);
    }
};

int main()
{
    int n, m;
    scanf("%d%d", &n, &m);
    LinkedList L(n);
    while(--n)
        L.pop(m);
    printf("%d\n",L.lastval());
    return 0;
}
*/